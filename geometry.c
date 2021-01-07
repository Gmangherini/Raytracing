#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "definitions.h"

int point_isonplane (point point_a, plane plane_a)
	{
	if (fabs((point_a.x*plane_a.a)+(point_a.y*plane_a.b)+(point_a.z*plane_a.c)+plane_a.d)>0.0000000001) return(0);
	else return(1);
	}

int points_areequal (point point_a, point point_b)
	{
	if ((point_a.x==point_b.x) && (point_a.y==point_b.y) && (point_a.z==point_b.z)) return(1);
	else return(0);
	}

double points_distance (point point_a, point point_b)
	{
	double x,y,z;
	x=point_b.x-point_a.x;
	y=point_b.y-point_a.y;
	z=point_b.z-point_a.z;
	x=x*x;
	y=y*y;
	z=z*z;
	x=sqrt(x+y+z);
	return(x);
	}

int point_isbetween (point point_a, point point_b, point point_p)
	{
	point point_m;
	point_m.x=(point_a.x+point_b.x)/2;
	point_m.y=(point_a.y+point_b.y)/2;
	point_m.z=(point_a.z+point_b.z)/2;
	if (points_distance(point_p,point_m)<=points_distance(point_a,point_m)) return(1);
	else return(0);
	}

int lines_intersection (line line_a, line line_b, point *intersection_point)
	{
	int i=0;
	double t;
	matrix_2x2 system;
	if (line_a.xt!=0)
		{
		if (line_b.yt!=0)
			{
			i=1;
			system.a=line_a.xt;
			system.b=-1*line_b.xt;
			system.c=line_a.yt;
			system.d=-1*line_b.yt;
			system.e=line_b.x-line_a.x;
			system.f=line_b.y-line_a.y;
			}
		else if (line_b.zt!=0)
			{
			i=1;
			system.a=line_a.xt;
			system.b=-1*line_b.xt;
			system.c=line_a.zt;
			system.d=-1*line_b.zt;
			system.e=line_b.x-line_a.x;
			system.f=line_b.z-line_a.z;
			}
		}
	if (line_a.yt!=0)
		{
		if (line_b.xt!=0)
			{
			i=1;
			system.a=line_a.xt;
			system.b=-1*line_b.xt;
			system.c=line_a.yt;
			system.d=-1*line_b.yt;
			system.e=line_b.x-line_a.x;
			system.f=line_b.y-line_a.y;
			}
		else if (line_b.zt!=0)
			{
			i=1;
			system.a=line_a.yt;
			system.b=-1*line_b.yt;
			system.c=line_a.zt;
			system.d=-1*line_b.zt;
			system.e=line_b.y-line_a.y;
			system.f=line_b.z-line_a.z;
			}
		}
	if (line_a.zt!=0)
		{
		if (line_b.xt!=0)
			{
			i=1;
			system.a=line_a.xt;
			system.b=-1*line_b.xt;
			system.c=line_a.zt;
			system.d=-1*line_b.zt;
			system.e=line_b.x-line_a.x;
			system.f=line_b.z-line_a.z;
			}
		else if (line_b.yt!=0)
			{
			i=1;
			system.a=line_a.yt;
			system.b=-1*line_b.yt;
			system.c=line_a.zt;
			system.d=-1*line_b.zt;
			system.e=line_b.y-line_a.y;
			system.f=line_b.z-line_a.z;
			}
		}
	if (i)
		{
		t=((system.e*system.d)-(system.b*system.f))/((system.a*system.d)-(system.b*system.c));
		intersection_point->x=line_a.x+(line_a.xt*t);
		intersection_point->y=line_a.y+(line_a.yt*t);
		intersection_point->z=line_a.z+(line_a.zt*t);
		}
	return (i);
	}
	
void line_paralleltoline_throughpoint (line line_a, point point_p, line *line_b)
	{
	line_b->x=point_p.x; line_b->y=point_p.y; line_b->z=point_p.z;
	line_b->xt=line_a.xt; line_b->yt=line_a.yt; line_b->zt=line_a.zt;
	}
	
void line_throughpoints (point point_a, point point_b, line *line_a)
	{
	line_a->x=point_a.x;
	line_a->xt=(point_b.x-point_a.x);
	line_a->y=point_a.y;
	line_a->yt=(point_b.y-point_a.y);
	line_a->z=point_a.z;
	line_a->zt=(point_b.z-point_a.z);
	}
	
void plane_throughpoints (point point_a, point point_b, point point_c, plane *plane_a)
	{
	point vect_a,vect_b;
	int i;
	double coeff[4],temp=0;
	vect_a.x=point_b.x-point_a.x; vect_a.y=point_b.y-point_a.y; vect_a.z=point_b.z-point_a.z;
	vect_b.x=point_c.x-point_b.x; vect_b.y=point_c.y-point_b.y; vect_b.z=point_c.z-point_b.z;
	plane_a->a=(vect_a.y*vect_b.z)-(vect_a.z*vect_b.y);
	plane_a->b=(vect_a.z*vect_b.x)-(vect_a.x*vect_b.z);
	plane_a->c=(vect_a.x*vect_b.y)-(vect_a.y*vect_b.x);
	plane_a->d=-1*((plane_a->a*point_b.x)+(plane_a->b*point_b.y)+(plane_a->c*point_b.z));
	//coefficients normalization
	coeff[0]=fabs(plane_a->a);
	coeff[1]=fabs(plane_a->b);
	coeff[2]=fabs(plane_a->c);
	//coeff[3]=fabs(plane_a->d);
	for(i=0;i<3;i++) if(temp<coeff[i]) temp=coeff[i];
	//printf("temp: %f\ta: %f\tb: %f\tc: %f\td: %f\n",temp,plane_a->a,plane_a->b,plane_a->c,plane_a->d);
	if (temp!=0)
		{
		plane_a->a/=temp;
		plane_a->b/=temp;
		plane_a->c/=temp;
		plane_a->d/=temp;
		}
	}
	
int point_isinside_area (point *points, int points_number, point point_p)
	{
	int i,j,intersect_p;
	point point_i;
	line *lines,line_p;
	lines=(line *)malloc(points_number*sizeof(line));
	for(j=0;j<(points_number-1);j++)
		{
		line_throughpoints(points[j],points[j+1],&lines[j]);
		}
	line_paralleltoline_throughpoint(lines[0],point_p,&line_p);
	intersect_p=0;
	//intersect_n=0;
	for(j=1;j<(points_number-1);j++)
		{
		i=lines_intersection(line_p,lines[j],&point_i);
		if (i)
			{
			if (point_isbetween(points[j],points[j+1],point_i))
				{
				if ((point_i.x-point_p.x)+(point_i.y-point_p.y)+(point_i.z-point_p.z)>0) intersect_p++;
				//else intersect_n++;
				}
			}
		//if (!i) puts("rette non secanti");
		}
	//printf("Trovate %d intersezioni_p e %d intersezioni_n\n",intersect_p,intersect_n);
	//i=(intersect_p%2 && intersect_n%2);
	//i=(intersect_p%2);
	//if (i) puts("Punto interno al poligono\n");
	//else  puts("Punto esterno al poligono\n");
	free (lines);
	return(intersect_p%2);
	}

void vector_toline (vector vector_a, line *line_a)
	{
	line_a->x=vector_a.pos.x;
	line_a->xt=vector_a.dir.x;
	line_a->y=vector_a.pos.y;
	line_a->yt=vector_a.dir.y;
	line_a->z=vector_a.pos.z;
	line_a->zt=vector_a.dir.z;
	}
	
int line_plane_intersection (line line_a, plane plane_a, point *point_a)
	{
	double t,t_den,t_num;
	t_den=(line_a.xt*plane_a.a)+(line_a.yt*plane_a.b)+(line_a.zt*plane_a.c);
	if (t_den!=0)
		{
		t_num=(line_a.x*plane_a.a)+(line_a.y*plane_a.b)+(line_a.z*plane_a.c)+plane_a.d;
		t=-1*t_num/t_den;
		point_a->x=line_a.x+(line_a.xt*t);
		point_a->y=line_a.y+(line_a.yt*t);
		point_a->z=line_a.z+(line_a.zt*t);
//		if (fabs(point_a->x)<0.000000001) point_a->x=0;
//		if (fabs(point_a->y)<0.000000001) point_a->y=0;
//		if (fabs(point_a->z)<0.000000001) point_a->z=0;
		//puts("line_plane_intersection ");
		//printf("(%f %f %f)\n",point_a->x,point_a->y,point_a->z);
		return (1);
		}
	else
		{
		//puts("line_plane_parallel ");
		return (0);
		}
	}

int point_projection_onplane (point point_p, plane plane_a, point *point_h)
	{
	int i;
	line line_h;
	line_h.x=point_p.x;
	line_h.xt=plane_a.a;
	line_h.y=point_p.y;
	line_h.yt=plane_a.b;
	line_h.z=point_p.z;
	line_h.zt=plane_a.c;
	//puts("point_projection_onplane ");
	i=line_plane_intersection(line_h,plane_a,point_h);
	//printf ("(%f %f %f)\t",point_p.x,point_p.y,point_p.z);
	//printf ("[%f %f %f %f]\t",plane_a.a,plane_a.b,plane_a.c,plane_a.d);
	//printf ("(%f %f %f)\n",point_h->x,point_h->y,point_h->z);
	return(i);
	}
	
int point_isinside_volume (volume *volume_a, point point_p)
	{
	int i,j,temp_inside[volume_a->surfaces_number];
	point point_h;
	
	//Compute if projected point is inside the given surface
	for(i=0;i<volume_a->surfaces_number;i++)
		{
		point_projection_onplane(point_p,volume_a->surfaces[i].surfPlane,&point_h);
		temp_inside[i]=point_isinside_area(volume_a->surfaces[i].points,volume_a->surfaces[i].points_number,point_h);
		//printf("Surface:%d OnPlane:%d P(%f %f %f) H(%f %f %f)\n",i,temp_inside[i],point_p.x,point_p.y,point_p.z,point_h.x,point_h.y,point_h.z);
		}

	//Check if other surfaces lie on the same plane
	for(i=0;i<volume_a->surfaces_number;i++)
		{
		if (!temp_inside[i])
			for (j=0;j<volume_a->surfaces_number;j++)
				{
				if (i!=j && volume_a->surfaces[j].surfPlane.a==volume_a->surfaces[i].surfPlane.a && volume_a->surfaces[j].surfPlane.b==volume_a->surfaces[i].surfPlane.b && volume_a->surfaces[j].surfPlane.c==volume_a->surfaces[i].surfPlane.c && volume_a->surfaces[j].surfPlane.d==volume_a->surfaces[i].surfPlane.d && temp_inside[j]) temp_inside[i]=1;
				}
		}
	for(i=0,j=1;i<volume_a->surfaces_number;j*=temp_inside[i],i++);
	
	return(j);
	}

int point_isinside_volume_2 (volume *volume_a, point point_p)
	{
	int i,j=0;
	point point_h;
	line tempLine;
	
	//Calculate the line passing through the given point ad the first vertex
	line_throughpoints(point_p, volume_a->surfaces[0].points[0], &tempLine);
	
	//If a surface intersects the line calculate the distance
	for(i=0;i<volume_a->surfaces_number;i++)
		{
		if (line_plane_intersection(tempLine, volume_a->surfaces[i].surfPlane, &point_h))
			{
			if (point_isinside_area(volume_a->surfaces[i].points, volume_a->surfaces[i].points_number, point_h))
				{
				if ((point_p.x+point_p.y+point_p.z-point_h.x-point_h.y-point_h.z)>0) j++; //Count the number of intersections with a positive distance
				}
			}
		}
	
	//If the number of intersections is odd the point is inside the given volume
	return(j%2);
	}

void vector_reflected_byplane(vector vector_inciding, plane plane_reflecting, vector *vector_reflected)
	{
	double modulo, product;
	modulo=(plane_reflecting.a*plane_reflecting.a)+(plane_reflecting.b*plane_reflecting.b)+(plane_reflecting.c*plane_reflecting.c);
	product=(vector_inciding.dir.x*plane_reflecting.a)+(vector_inciding.dir.y*plane_reflecting.b)+(vector_inciding.dir.z*plane_reflecting.c);
	product/=modulo;
	vector_reflected->dir.x=vector_inciding.dir.x-(2*product*plane_reflecting.a);
	vector_reflected->dir.y=vector_inciding.dir.y-(2*product*plane_reflecting.b);
	vector_reflected->dir.z=vector_inciding.dir.z-(2*product*plane_reflecting.c);
	}
	
double vector_reflection_angle(vector vector_inciding, plane plane_reflecting)
	{
		double a,b,ab_product;
		a=sqrt((vector_inciding.dir.x*vector_inciding.dir.x)+(vector_inciding.dir.y*vector_inciding.dir.y)+(vector_inciding.dir.z*vector_inciding.dir.z));
		b=sqrt((plane_reflecting.a*plane_reflecting.a)+(plane_reflecting.b*plane_reflecting.b)+(plane_reflecting.c*plane_reflecting.c));
		ab_product=((vector_inciding.dir.x*plane_reflecting.a)+(vector_inciding.dir.y*plane_reflecting.b)+(vector_inciding.dir.z*plane_reflecting.c));
		return(acos(ab_product/(a*b)));
	}
