#include <stdio.h>
#include <stdlib.h>
int main()
{
	FILE *tempF;
	double temp;
	tempF = fopen("/sys/class/thermal/thermal_zone0/temp","r"); 
	if(tempF == NULL)
	{
		printf("Could not open file\n");
		return 1;
	}
	fscanf(tempF, "%lf", &temp);
	temp = temp/1000; 
	fclose(tempF);
	printf("Temperature: %f\n", temp); 
	return 0; 
}
