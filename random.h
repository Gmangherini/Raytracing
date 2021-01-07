#ifndef _RANDOM_H
#define	_RANDOM_H

unsigned int rand30 (void);
unsigned int rand32 (void);
unsigned long long int rand45 (void);
unsigned long long int rand60 (void);
unsigned long long int rand64 (void);

int int30 (int max, int min);
int int32 (int max, int min);
long long int int45 (long long int max, long long int min);
long long int int60 (long long int max, long long int min);
long long int int64 (long long int max, long long int min);

float float30 (float max, float min);
float float32 (float max, float min);
double float45 (double max, double min);
double float60 (double max, double min);
double float64 (double max, double min);

#endif
