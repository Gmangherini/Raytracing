#include <stdlib.h>

#define		RAND_MAX_30		0x3FFFFFFF
#define		RAND_MAX_32		0xFFFFFFFF
#define		RAND_MAX_45		0x00001FFFFFFFFFFF
#define		RAND_MAX_60		0x0FFFFFFFFFFFFFFF
#define		RAND_MAX_64		0xFFFFFFFFFFFFFFFF

unsigned int rand30 (void)
	{
	unsigned int randomnumber=0;
	randomnumber|=(unsigned int)rand();
	randomnumber=randomnumber<<15;
	randomnumber|=(unsigned int)rand();
	return (randomnumber);
	}
	
unsigned int rand32 (void)
	{
	char i;
	unsigned int randomnumber=0;
	for (i=0;i<2;i++)
		{
		randomnumber|=(unsigned int)rand();
		randomnumber=randomnumber<<15;
		}
	randomnumber|=(unsigned int)rand();
	return (randomnumber);
	}
	
unsigned long long int rand45 (void)
	{
	char i;
	unsigned long long int randomnumber=0;
	for (i=0;i<2;i++)
		{
		randomnumber|=(unsigned long long int)rand();
		randomnumber=randomnumber<<15;
		}
	randomnumber|=(unsigned long long int)rand();
	return (randomnumber);
	}

unsigned long long int rand60 (void)
	{
	char i;
	unsigned long long int randomnumber=0;
	for (i=0;i<3;i++)
		{
		randomnumber|=(unsigned long long int)rand();
		randomnumber=randomnumber<<15;
		}
	randomnumber|=(unsigned long long int)rand();
	return (randomnumber);
	}

unsigned long long int rand64 (void)
	{
	char i;
	unsigned long long int randomnumber=0;
	for (i=0;i<4;i++)
		{
		randomnumber|=(unsigned long long int)rand();
		randomnumber=randomnumber<<15;
		}
	randomnumber|=(unsigned long long int)rand();
	return (randomnumber);
	}

long long int int45 (long long int max, long long int min)
{
	return(min+((long long int)(rand45()>>1)%(max-min+1)));
}

long long int int60 (long long int max, long long int min)
{
	return(min+((long long int)(rand60()>>1)%(max-min+1)));
}

long long int int64 (long long int max, long long int min)
{
	return(min+((long long int)(rand64()>>1)%(max-min+1)));
}

float float30 (float max, float min)
	{
	float result;
	if (max==1.0 && min==0.0) result=((float)rand30()/RAND_MAX_30);
	else result=((float)rand30()/RAND_MAX_30)*(max-min)+min;
	return (result);
	}
	
float float32 (float max, float min)
	{
	float result;
	if (max==1.0 && min==0.0) result=((float)rand32()/RAND_MAX_32);
	else result=((float)rand32()/RAND_MAX_32)*(max-min)+min;
	return (result);
	}
	
double float45 (double max, double min)
	{
	double result;
	if (max==1.0 && min==0.0) result=((double)rand45()/RAND_MAX_45);
	else result=((double)rand45()/RAND_MAX_45)*(max-min)+min;
	return (result);
	}
	
double float60 (double max, double min)
	{
	double result;
	if (max==1.0 && min==0.0) result=((double)rand60()/RAND_MAX_60);
	else result=((double)rand60()/RAND_MAX_60)*(max-min)+min;
	return (result);
	}
	
double float64 (double max, double min)
	{
	double result;
	if (max==1.0 && min==0.0) result=((double)rand64()/RAND_MAX_64);
	else result=((double)rand64()/RAND_MAX_64)*(max-min)+min;
	return (result);
	}
