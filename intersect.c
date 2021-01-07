#define _USE_MATH_DEFINES

#include <math.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "definitions.h"
#include "geometry.h"
#include "random.h"
#include "general.h"

#define VERBOSE				0							//Verbose mode		(1=verbose, 0=hide)
#define ERRORS				0							//Show errors mode	(1=show errors, 0=hide)
#define RAYS_NUMBER			1000
#define MAX_REFLECTIONS		10000
#define	REFRACTIVE_INDEX	1.488						//1.488 per PMMA a 650nm
#define	CRITICAL_ANGLE		asin(1/REFRACTIVE_INDEX)
#define SHADE				(vector_temp_in.pos.x>125)							//(vector_temp_in.pos.x>125)
#define ROUNDING_ERROR		0.000000001

#define REFRACTIVE			0
#define	ABSORBER			1
#define REFLECTOR			2

typedef struct _simdata
{
int thread_number;
int rays_number;
volume lsc;
} simdata;

static unsigned __stdcall simulation(void *_data)
	{
	int i=0,j,reflections=0,last_surface,invalid_point=0,thread_number,rays_number,bufSize=10240000;
	double distance,angle,reflection_probability;
	char last_points_string[64];
	point point_p;
	line line_temp;
	vector vector_temp_out,vector_temp_in,vector_saved={{0,0,0},{0,0,0}};
	volume lsc;
	FILE *fp,*fp2;
	simdata *data;
	
	data=_data;
	
	thread_number=data->thread_number;
	rays_number=data->rays_number;
	lsc=data->lsc;
	if (VERBOSE) fp=fopen("simulation_log.txt","w");
	
	//Each thread saves its lon in a different file named after the thread id
	sprintf(last_points_string,"last_points_%u.txt",thread_number);
	fp2=fopen(last_points_string,"w");
	
	//Set the buffer size for the file output stream
	bufSize+=1024000*thread_number;
	if ((rays_number*60)<bufSize) bufSize=(rays_number*60);
	setvbuf(fp2, NULL, _IOFBF, bufSize);
	
	//Initialize the pseudorandom number generator, the function is thread safe but each thread must be initialized with a different number
	srand(time(NULL)+GetCurrentThreadId());
		
	for (i=0;i<rays_number;i++)
		{
		distance=0,reflections=0,last_surface=0;
		//Se il calcolo precendente ha creato problemi decremento il contatore e riprovo con un altro punto
		if (invalid_point)
			{
			invalid_point=0;
			i--;
			}
		//Genero un vettore posizione+direzione all'interno del sistema ed eventualmente (VERBOSE) ne salvo i dati
		//le posizioni hanno distribuzione lineare uniforme mentre le direzioni hanno distribuzione angolare uniforme
		vector_generate_inside(&lsc,&vector_temp_in);
		if (VERBOSE) fprintf(fp,"%+.8f\t%+.8f\t%+.8f\t%+.8f\t%+.8f\t%+.8f\t0.00000000\t255\n",vector_temp_in.pos.x,vector_temp_in.pos.y,vector_temp_in.pos.z,vector_temp_in.dir.x,vector_temp_in.dir.y,vector_temp_in.dir.z);
		//Se è nella zona ombraggiata non eseguo nemmeno il calcolo e continuo con il successivo
		if (SHADE) last_surface=1;
		//Eseguo il calcolo della traiettoria finchè non raggiunge una superficie assorbente o si genera un errore
		while (!last_surface && !invalid_point)
			{
			//Incremento il contatore del numero di riflessioni
			reflections++;
			//Calcolo la retta passante per l'ultimo punto di interazion  e con le superfici
			vector_toline(vector_temp_in, &line_temp);
			//Trovo il punto di interazione successivo iterando la ricerca su tutte le superfici
			for (j=0;j<lsc.surfaces_number;j++)
				{
				//For each surface calculates the intersections between the previously obtained inne and the plane where the surface lies
				if (line_plane_intersection(line_temp,lsc.surfaces[j].surfPlane,&point_p)) //If the line and the plane are non parallel
					{//A bit of constraints to ignore rounding errors
					if (fabs(point_p.x-lsc.x_max)<ROUNDING_ERROR) point_p.x=lsc.x_max;
					else if (fabs(point_p.x-lsc.x_min)<ROUNDING_ERROR) point_p.x=lsc.x_min;
					if (fabs(point_p.y-lsc.y_max)<ROUNDING_ERROR) point_p.y=lsc.y_max;
					else if (fabs(point_p.y-lsc.y_min)<ROUNDING_ERROR) point_p.y=lsc.y_min;
					if (fabs(point_p.z-lsc.z_max)<ROUNDING_ERROR) point_p.z=lsc.z_max;
					else if (fabs(point_p.z-lsc.z_min)<ROUNDING_ERROR) point_p.z=lsc.z_min;
					//Chech if the intersection point with the plane lies inside the surface
					if (point_isinside_area(lsc.surfaces[j].points,lsc.surfaces[j].points_number,point_p))
						{//If the point is inside the survace calculates the distance (with direction informations) from the starting point
						distance=(((point_p.x-vector_temp_in.pos.x)*vector_temp_in.dir.x)*((point_p.y-vector_temp_in.pos.y)*vector_temp_in.dir.y)*((point_p.z-vector_temp_in.pos.z)*vector_temp_in.dir.z));
						if (distance>0) //If the intersection point is in the same direction of the starting vector this is the point we were looking for
							{//Save the intersection point as the new starting point
							vector_temp_out.pos.x=point_p.x;
							vector_temp_out.pos.y=point_p.y;
							vector_temp_out.pos.z=point_p.z;
							//Takes into account the type of the inciding surface
							switch (lsc.surfaces[j].type)
								{
								//If the surface is an absorber the simulation for this photon ends here
								case ABSORBER: //Dark surface
									last_surface=1;
									break;
								//If the surface is reflective takes into account the reflectance R[0,1]
								case REFLECTOR: //Reflective surface
									reflection_probability=float30(1,0);
									if (reflection_probability>lsc.surfaces[j].reflectivity) last_surface=1;
									break;
								}
							break;
							}
						}
					}
				}
			//Errors handling, if the number of reflections is too high or there is no intersection with any surface the simulation for this photon ends
			if (j>=lsc.surfaces_number || reflections>MAX_REFLECTIONS)
				{
				invalid_point=1;
				//If the program is in "show errors" mode prints the debug informations for this photon
				if (ERRORS)
					{
					printf("\rError: surface %d, reflections %d\n",j,reflections);
					printf("%.16f\t%.16f\t%.16f\n",vector_temp_in.pos.x,vector_temp_in.pos.y,vector_temp_in.pos.z);
					printf("%.16f\t%.16f\t%.16f\n",vector_temp_in.dir.x,vector_temp_in.dir.y,vector_temp_in.dir.z);
					printf("%.16f\t%.16f\t%.16f\n",vector_saved.pos.x,vector_saved.pos.y,vector_saved.pos.z);
					printf("%.16f\t%.16f\t%.16f\n",vector_saved.dir.x,vector_saved.dir.y,vector_saved.dir.z);
					printf("%.16f\t%.16f\t%.16f\n",point_p.x,point_p.y,point_p.z);
					printf("%d%% complete",i/(rays_number/100));
					}
				}
			//If everything is fine continues the simulation
			else
				{
				//Calculate the vector reflected by the incidence plane
				vector_reflected_byplane(vector_temp_in,lsc.surfaces[j].surfPlane,&vector_temp_out);
				//Calculate the angle of incidence on the plane
				angle=vector_reflection_angle(vector_temp_in,lsc.surfaces[j].surfPlane);
				//Save the vector as the starting one for the next step
				vector_saved=vector_temp_in;
				vector_temp_in=vector_temp_out;
				//Check if the photon doesn't undergo total internal reflection
				if ((angle<CRITICAL_ANGLE || angle>(M_PI-CRITICAL_ANGLE)) && lsc.surfaces[j].type!=REFLECTOR) last_surface=1;
				if (VERBOSE) fprintf(fp,"%+.8f\t%+.8f\t%+.8f\t%+.8f\t%+.8f\t%+.8f\t%+.8f\t%d\n",vector_temp_out.pos.x,vector_temp_out.pos.y,vector_temp_out.pos.z,vector_temp_out.dir.x,vector_temp_out.dir.y,vector_temp_out.dir.z,angle,j);
				//If last surface was reached save the informations in the log and continue with another photon, else continue with next reflection
				if (last_surface)
					{
					if (VERBOSE) fputc('\n',fp);
					fprintf(fp2,"%+.8f\t%+.8f\t%+.8f\t%d\n",vector_temp_out.pos.x,vector_temp_out.pos.y,vector_temp_out.pos.z,j);
					}
				}
			}
		//Print simulation progess
		if (thread_number==1) if (!(i%(rays_number/100)))
			{
			printf("\r%d%% complete",i/(rays_number/100));
			}
		}
	if (thread_number==1) puts("\r100%% complete");
	//printf("\nThread number %d complete\n",thread_number);
	if (VERBOSE) fclose(fp);
	fclose(fp2);
    _endthreadex(0);
	}

int main(int argc, char *argv[])
{
char command[256];
int j,rays_number,nthreads=1,startup_time;
volume lsc;
simdata *temp_data;
HANDLE *handles;
SYSTEM_INFO sysinfo;

if (argc>1)
	{
	rays_number=atoi(argv[1]);
	}
else 
	{
	rays_number=RAYS_NUMBER;
	}
printf("Rays number: %d\n",rays_number);

if (argc>2) nthreads=atoi(argv[2]);
else
	{
	GetSystemInfo(&sysinfo);
	nthreads=(int)sysinfo.dwNumberOfProcessors;
	}
printf("Simulation threads: %d\n",nthreads);

handles=malloc(nthreads*sizeof(HANDLE));
temp_data=malloc(nthreads*sizeof(simdata));

//Load the surfaces from textfile
puts("Reading surfaces...");
j=load_surfaces_fromfile("surfaces.txt", &lsc);
if (j)
	{
	printf("Function load_surfaces_fromfile aborted with error %d\n",j);
	return(1);
	}

//Prints all the points of all the surfaces
puts("Displaying surfaces...");
print_points(&lsc);

puts("\nSimulation start...");
startup_time=time(NULL);
for (j=0; j<nthreads; j++)
	{
	unsigned tid;
	temp_data[j].thread_number=j+1;
	temp_data[j].rays_number=rays_number/nthreads;
	temp_data[j].lsc=lsc;
	_beginthreadex (NULL, 0, simulation, &temp_data[j], 0, &tid);
	handles[j] = OpenThread (SYNCHRONIZE, FALSE, tid);
    }

WaitForMultipleObjects(nthreads,handles,TRUE,INFINITE);
//Prints the execution time
printf("Elapsed time: %ds\n\n",(int)time(NULL)-startup_time);
startup_time=time(NULL);

//Hystogram calculation
sprintf(command,"histogram.exe %d",nthreads);
system(command);

//Free all the allocated memory
free_volume(&lsc);

//Prints the execution time
printf("Elapsed time: %ds\n",(int)time(NULL)-startup_time);
puts("Simulation done");

return(0);
}
