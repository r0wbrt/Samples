/*
Copyright (c) 2015, Robert Christian Taylor
All rights reserved.
*/


#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define PI  3.1415926535897932384626433832795028
#define N 128




int main()
{

	
	float pos = 0;
	float incr = (PI * .5 )/ N;
	float out = 0;	
	int dif;
	int final;
	float data[N];
	int i = 0;

	printf(" uint16_t cos_table[] = {\r\n");
	while(i < N)
	{
		out = cos(pos);
		data[i] = out;
		i++;
		pos = pos + incr;
		final = 30000.0*out;
		printf("%i, \r\n", final);
	}
	
	printf("0\r\n};\r\n");
	i = 0;
	/*printf(" uint16_t cos_table_dif[] = {\r\n");

	while( i < (N-1))
	{
		dif = 32768.0*(data[i] - data[i+1])*B_F;
		printf("%i,\r\n", dif);
		i++;
	}		
	*/
	return 0;
}


