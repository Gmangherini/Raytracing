#ifndef _DEFINITIONS_H
#define	_DEFINITIONS_H

typedef struct _matrix_2x2
{
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
} matrix_2x2;

typedef struct _plane
{
	double a;
	double b;
	double c;
	double d;
} plane;

typedef struct _line
{
	double x;
	double xt;
	double y;
	double yt;
	double z;
	double zt;
} line;

typedef struct _point
{
	double x;
	double y;
	double z;
} point;

typedef struct _vector
{
	point pos;
	point dir;
} vector;

typedef struct _surface
{
	int type;
	double reflectivity;
	int axisToPlot;
	int points_number;
	point *points;
	plane surfPlane;
} surface;

typedef struct _volume
{
	surface *surfaces;
	int surfaces_number;
	double x_min;
	double y_min;
	double z_min;
	double x_max;
	double y_max;
	double z_max;
} volume;

#endif
