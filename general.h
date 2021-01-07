#ifndef _GENERAL_H
#define	_GENERAL_H

int load_surfaces_fromfile(char *filename, volume *system);
void free_volume (volume *volume_a);
void print_points(volume *volume_a);
int vector_generate_inside(volume *volume_a,vector *vector_a);

#endif
