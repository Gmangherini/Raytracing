#define _USE_MATH_DEFINES

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "random.h"
#include "geometry.h"

int load_surfaces_fromfile(char *filename, volume *system)
	{
	FILE *fp;
	char str_temp[256], *str_point;
	int points_number=0,surfaces_number=0,i=0,j,type,axisToPlot=0;
	double x_min=DBL_MAX,y_min=DBL_MAX,z_min=DBL_MAX,x_max=DBL_MIN,y_max=DBL_MIN,z_max=DBL_MIN,reflectivity;
	point points[256];
	plane myplane;
	surface surfaces[256];
	
	fp=fopen(filename,"r");
	if (fp==NULL) return(1);
	while (!feof(fp)) 
		{
		points_number=0;
		fgets(str_temp,256,fp);
		str_point=strtok(str_temp,"\t\n ()");
		if (str_point!=NULL) sscanf(str_point,"%d",&type);
		str_point=strtok(NULL,"\t\n ()");
		sscanf(str_point,"%lf",&reflectivity);
		str_point=strtok(NULL,"\t\n ()");
		sscanf(str_point,"%c",(char*)&axisToPlot);
		str_point=strtok(NULL,"\t\n ()");
		while (str_point!=NULL)
			{
			sscanf(str_point,"%lf,%lf,%lf",&(points[points_number].x),&(points[points_number].y),&(points[points_number].z));
			if (points[points_number].x<x_min) x_min=points[points_number].x;
			if (points[points_number].y<y_min) y_min=points[points_number].y;
			if (points[points_number].z<z_min) z_min=points[points_number].z;
			if (points[points_number].x>x_max) x_max=points[points_number].x;
			if (points[points_number].y>y_max) y_max=points[points_number].y;
			if (points[points_number].z>z_max) z_max=points[points_number].z;
			points_number++;
			str_point=strtok(NULL,"\t\n ()");
			}
		//Chech if it is a close surface
		if (!points_areequal(points[0],points[points_number-1]))
			{
			puts("First and last point are not the same!\n");
			return(2);
			}
		//Checks if the points lie on the same plane
		if (points_number>=3)
			{
			plane_throughpoints(points[0],points[1],points[2],&myplane);
			for (j=3;j<points_number;j++)
				{
				if (!point_isonplane(points[j],myplane))
					{
					printf("Surface %d: point %d is not on plane\n",surfaces_number,j);
					printf("(%f,%f,%f)\n",points[j].x,points[j].y,points[j].z);
					printf("a=%.12f b=%.12f c=%.12f d=%.12f\n\n",myplane.a,myplane.b,myplane.c,myplane.d);
					return(3);
					}
				}
			}
		//Allocates memory to points vector
		surfaces[surfaces_number].points=(point *)malloc(points_number*sizeof(point));
		for(j=0;j<points_number;j++)
			{
			surfaces[surfaces_number].points[j]=points[j];
			}
		surfaces[surfaces_number].points_number=points_number;
		surfaces[surfaces_number].type=type;
		surfaces[surfaces_number].reflectivity=reflectivity;
		surfaces[surfaces_number].surfPlane=myplane;
		surfaces[surfaces_number].axisToPlot=axisToPlot;
		surfaces_number++;
		}
	system->surfaces=(surface *)malloc(surfaces_number*sizeof(surface));
	for (i=0;i<surfaces_number;i++)
		{
		system->surfaces[i]=surfaces[i];
		}
	system->surfaces_number=surfaces_number;
	system->x_max=x_max;
	system->y_max=y_max;
	system->z_max=z_max;
	system->x_min=x_min;
	system->y_min=y_min;
	system->z_min=z_min;
	fclose(fp);
	return(0);
	}
	
void free_volume (volume *volume_a)
	{
	int i;
	for (i=0;i<volume_a->surfaces_number;i++)
		{
		free(volume_a->surfaces[i].points);
		}
	free(volume_a->surfaces);
	}

void print_points(volume *volume_a)
	{
	int i,j;
	for(i=0;i<volume_a->surfaces_number;i++)
		{
		printf ("\nSurface %d",i);
		switch (volume_a->surfaces[i].type)
			{
			case 0:
				puts(":\tType: Refractive");
				break;
			case 1:
				puts(":\tType: Absorber");
				break;
			case 2:
				printf(":\tType: Reflector\tR=%.2f\n",volume_a->surfaces[i].reflectivity);
				break;
			}
		printf("Histogram plot axis: %c\n",volume_a->surfaces[i].axisToPlot);
		for(j=0;j<volume_a->surfaces[i].points_number;j++)
			{
			printf ("Point %d:\t",j);
			printf ("%f\t%f\t%f\n",volume_a->surfaces[i].points[j].x,volume_a->surfaces[i].points[j].y,volume_a->surfaces[i].points[j].z);
			}
		fputs("Plane equation is:\t",stdout);
		if (volume_a->surfaces[i].surfPlane.a)
			{
			if (volume_a->surfaces[i].surfPlane.a==1) fputs("x",stdout);
			else if (volume_a->surfaces[i].surfPlane.a==-1) fputs("-x",stdout);
			else printf("%+fx",volume_a->surfaces[i].surfPlane.a);
			}
		if (volume_a->surfaces[i].surfPlane.b)
			{
			if (volume_a->surfaces[i].surfPlane.b==1) fputs("y",stdout);
			else if (volume_a->surfaces[i].surfPlane.b==-1) fputs("-y",stdout);
			else printf("%+fy",volume_a->surfaces[i].surfPlane.b);
			}
		if (volume_a->surfaces[i].surfPlane.c)
			{
			if (volume_a->surfaces[i].surfPlane.c==1) fputs("z",stdout);
			else if (volume_a->surfaces[i].surfPlane.c==-1) fputs("-z",stdout);
			else printf("%+fz",volume_a->surfaces[i].surfPlane.c);
			}
		if (volume_a->surfaces[i].surfPlane.d)
			{
			if (volume_a->surfaces[i].surfPlane.d==1) fputs("+1",stdout);
			else if (volume_a->surfaces[i].surfPlane.d==-1) fputs("-1",stdout);
			else printf("%+f",volume_a->surfaces[i].surfPlane.d);
			}
		fputs("=0\n",stdout);
		}
	}

int vector_generate_inside(volume *volume_a,vector *vector_a)
{
	int i=0;
	double theta, phi;
	do
		{
		i++;
		vector_a->pos.x=(double)float30((float)volume_a->x_max,(float)volume_a->x_min);
		vector_a->pos.y=(double)float30((float)volume_a->y_max,(float)volume_a->y_min);
		vector_a->pos.z=(double)float30((float)volume_a->z_max,(float)volume_a->z_min);
		} while (!point_isinside_volume(volume_a, vector_a->pos) && i<1000);
	if (i>990) puts("timeout");
	theta=(double)float30(-1,1);
	vector_a->dir.z=theta; //cos(acos(x))=x | x € [0,PI]
	phi=(double)float30(0,2*M_PI);
	theta=sqrt(1-(theta*theta));
	vector_a->dir.x=theta*cos(phi);  //sin(acos(x))=sqrt(1-x^2)  | x € [0,PI]
	vector_a->dir.y=theta*sin(phi);  //sin(acos(x))=sqrt(1-x^2)  | x € [0,PI]
	return (i);
}
