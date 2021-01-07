#ifndef _GEOMETRY_H
#define	_GEOMETRY_H

int point_isonplane (point point_a, plane plane_a);
int points_areequal (point point_a, point point_b);
double points_distance (point point_a, point point_b);
int point_isbetween (point point_a, point point_b, point point_p);
int lines_intersection (line line_a, line line_b, point *intersection_point);
void line_paralleltoline_throughpoint (line line_a, point point_p, line *line_b);
void line_throughpoints (point point_a, point point_b, line *line_a);
void plane_throughpoints (point point_a, point point_b, point point_c, plane *plane_a);
int point_isinside_area (point *points, int points_number, point point_a);
void vector_toline (vector vector_a, line *line_a);
int line_plane_intersection (line line_a, plane plane_a, point *point_a);
void point_projection_onplane (point point_p, plane plane_a, point *point_h);
int point_isinside_volume (volume *volume_a, point point_p);
int point_isinside_volume_2 (volume *volume_a, point point_p);
void vector_reflected_byplane(vector vector_inciding, plane plane_reflecting, vector *vector_reflected);
double vector_reflection_angle(vector vector_inciding, plane plane_reflecting);

#endif
