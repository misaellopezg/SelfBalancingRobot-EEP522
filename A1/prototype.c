/*
 * MODULE: 
 *
 * prototype
 *
 * DESCRIPTION: 
 *
 * This module runs through a set of memory tests to determines.
 * To provide a baseline of information.
 *
 * HISTORY
 *
 * 2022-11-03 Andrew N. Sloss
 * -- added Raspberry Pi 3B (a22082)
 * -- fflush before closing file stream
 * -- added a g_version for the version number
 *
 * 2022-10-31 Andrew N. Sloss
 * -- started cleaning up the code
 * 
 */

// *********************************************************
// * IMPORTS
// *********************************************************

// -- standard liraries

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// -- local libraries

// none... 

// *********************************************************
// * NEW TYPES
// *********************************************************

typedef struct
{
uint32_t model;
uint8_t  memory_size_gb;
float    revision;
} raspberry_str;


// *********************************************************
// * GLOBALS
// *********************************************************

raspberry_str g_core;
int32_t g_dim1[501][501]; 
int32_t g_dim2[501][501]; 
int32_t g_dimd[501][501]; 

// *********************************************************
// * CONSTANT
// *********************************************************

const float g_version = 0.01;

// *********************************************************
// * ROUTINES
// *********************************************************

/*
 * NAME: 
 *
 * prototype_visual_progress()
 *
 * DESCRIPTION: 
 *
 * display a spinning progress character
 *
 * PARAMETRS:
 * 
 * n/a
 *
 * SIDE EFFECT:
 *
 * display the progress by spinning a character... 
 * flushes the stdout buffer
 *
 * RETURN
 *
 * int 0 sucessfull 
 *
 */
 
void prototype_visual_progress(void)
{
static uint32_t mode=1;
 
/* -- init */
  
assert(mode<=4);
putchar('\b');
  
/* -- process */
  
  switch(mode)
  {
  case 1: putchar ('-');  break;
  case 2: putchar ('\\'); break;
  case 3: putchar ('|');  break;
  case 4: putchar ('/'); 
    mode=0;
    break;
  }         
  
/* -- finalize */
  
fflush(stdout);
mode++;
}

/*
 * NAME: 
 *
 * prototype_modelname_read()
 *
 * DESCRIPTION: 
 *
 * Reas both the model name and cpu cores
 *
 * PARAMETRS:
 * 
 * char *model - name of Raspberry model
 * char *cpucore - cpu core string
 *
 * SIDE EFFECT:
 *
 * Update strings for model and cpucore
 *
 * RETURN
 *
 * int 0 successful
 *
 */
 
bool prototype_modelname_read(char *model,char *cpucores)
{
char *line;
FILE *fp = fopen("/proc/cpuinfo", "r");

// -- initialize

assert(model!=NULL);
assert(cpucores!=NULL);
assert(fp != NULL);

size_t n = 0;

//-- process

  while (getline(&line,&n, fp) > 0) 
  {
    if (strstr(line, "Revision"))
    {
    line[strlen(line)-1] = 0;
    strcpy(model,(line+(strlen(line)-6)));
    }
                
    if(strstr(line,"cpu cores")) 
      strcpy(cpucores,line);
                
    }

// -- finialize    
    
free(line);
fclose(fp);

return true;
}

/*
 * NAME: 
 *
 * prototype_temperature_read
 *
 * DESCRIPTION: 
 *
 * display a spinning progress character
 *
 * PARAMETRS:
 * 
 * n/a
 *
 * RETURN:
 *
 * double - temperature in celius
 *
 * SIDE EFFECT:
 *
 * if the temp file cannot be opened then program exists.
 *
 */
 
double prototype_temperature_read(void)
{
FILE *temperatureFile;
double T;

// -- initialize

temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");

// -- process

  if (temperatureFile == NULL)
  {
  printf ("-- E: problem reading temperature: exiting some messag %d, %s \n",__LINE__,__FILE__);
  exit(1);
  }

fscanf (temperatureFile, "%lf", &T);
T /= 1000;

// finalize

fclose (temperatureFile);

return T;
}

/*
 * NAME: 
 *
 * prototype_translate_information
 *
 * DESCRIPTION: 
 *
 * Takes the string of the model name updates the g_core structure
 *
 * PARAMETRS:
 * 
 * char *model - model string
 *
 * SIDE EFFECT:
 *
 * update g_core structure
 *
 * RETURN
 *
 * n/a
 *
 */
 
void prototype_translate_information(char *model)
{

// -- initialize

assert(model!=NULL);

g_core.memory_size_gb = -1; /* not unknown */
g_core.revision = 0.0;

// -- process

  if (!strcmp(model,"d03114"))
  {
  g_core.model = 0xd03114;
  g_core.memory_size_gb = 8;
  g_core.revision = 1.4;
  return;
  } 
 
  if (!strcmp(model,"a03111"))
  {
  g_core.model = 0xa03111;
  g_core.memory_size_gb = 1;
  g_core.revision = 1.1;
  return;
  } 
  
  if (!strcmp(model,"a22082"))
  {
  g_core.model = 0xa22082;
  g_core.memory_size_gb = 1;
  g_core.revision = 1.2;
  return;
  } 

  //Adding Raspberry Pi 4
  if (!strcmp(model,"a03115"))
  {
  g_core.model = 0xa03115;
  g_core.memory_size_gb = 1;
  g_core.revision = 1.5;
  return;
  } 
  // ** add more model types here
  
// -- finalize
  
printf("-- E: model type unknown %s",model);
exit(1); 
}

/*
 * NAME: 
 *
 * prototype_write_speed()
 *
 * DESCRIPTION: 
 *
 * Allocates two memory areas, A and B. Fill one areas with 0xFF and then
 * copy A to B,and then verifies the copy was successful.
 *
 * PARAMETRS:
 * 
 * int32_t - ns - size of 1K chunks
 *
 * SIDE EFFECT:
 *
 * Using a set of standard library functions. Note in a true emmbedded 
 * system malloc() (or dynamic memory) should only be used at 
 * initialization time.
 *
 * RETURN
 *
 * n/a 
 *
 */
 
void prototype_write_speed (uint32_t ns)
{
int8_t *ptr1, *ptr2, *pass;
int32_t x;

// -- initialize

assert(ns>0);

ptr1 = malloc(4+(ns*1024));
ptr2 = malloc(4+(ns*1024));

  if (ptr1==NULL)
  {
  printf ("-- E: failed to allocate enough memory, ptr1==NULL %d\n",__LINE__);
  exit(1);
  }  

  if (ptr2==NULL)
  {
  printf ("-- E: failed to allocate enough memory, ptr2==NULL, %d\n",__LINE__);
  exit(1);
  }
  

// -- process
   
memset(ptr1,0xff,ns*1024);
memcpy(ptr2,ptr1,ns*1024);
  
  if (memcmp(ptr2,ptr1,ns*1024))
  {
  printf ("-- E: problem memory not equal \n");
  exit(1);
  }
   
// -- finialize 
   
free(ptr1);
free(ptr2);
}

/*
 * NAME: 
 *
 * prototype_matrix_xy()
 *
 * DESCRIPTION: 
 *
 * Setup two matrix and then add them together. A[] + B[] = C[]
 * Row first, and then column.  
 *
 * PARAMETRS:
 * 
 * uint32_t n - byte 
 *
 * SIDE EFFECT:
 *
 * RETURN
 *
 * n/a 
 *
 */
 
void prototype_matrix_calc(uint32_t n,bool friendly)
{
uint32_t x,y;

// -- initialize

assert(n>0);

  for(x=0; x<n;x++)
    for (y=0; y<n;y++)
      g_dim1[x][y] = 1;
            
  for(x=0; x<n;x++)
    for (y=0; y<n;y++)
      g_dim2[x][y] = 1;       

// -- process

  if (friendly)
  {
    for(x=0; x<n;x++)
      for (y=0; y<n;y++)
        g_dimd[x][y]=g_dim1[x][y] + g_dim2[x][y];
  } 
  else
  {
    for(y=0; y<n;y++)
      for (x=0; x<n;x++)
        g_dimd[x][y]=g_dim1[x][y] + g_dim2[x][y];    
  }        
}

/*
 * NAME: 
 *
 * prototype_tests()
 *
 * DESCRIPTION: 
 *
 * This routines runs through the 3 tests repeatedly $tests times 
 *
 * PARAMETRS:
 * 
 * FILE *H1 - file handle of the test details
 * FILE *H2 - file handle of test run speeds
 * double temp_baseline - temperature baseline
 * uint32_t tests - test runs
 *
 * SIDE EFFECT:
 *
 * none, test should do busy work for timgin purposes
 * 
 * RETURN
 *
 * n/a 
 *
 */ 

void prototype_tests (FILE *H1, FILE *H2,double temp_baseline, uint32_t testruns)
{
int32_t test;
clock_t start, end;
clock_t test_start,test_end;
double final,cpu_time_used,xy_time,yx_time;

// -- initialize

assert(H1!=NULL);
assert(H2!=NULL);

test_start = clock();
//printf (" %ld \n",test_start); 
//fflush(stdout);

fprintf(H1,"# .test .cputime .temperature .time_mat1 .time_mat2\n");
fprintf(H2,"# .tests complete .time taken \n");

// -- process
        
  for (test=1; test<=testruns; test++)
  { 
  cpu_time_used = 55075;
  prototype_visual_progress();
    
    if ((test % 100)==0)
    {
    test_end = clock();
    final = ((double) (test_end - test_start)) / CLOCKS_PER_SEC;
    printf ("........... [%d] %lf sec \n",test,final);
    fprintf (H2,"%d %lf\n",test,final);
    fflush(H2);
    test_start = clock();
    }   
   
  //printf ("test 1: test size = %d\n",test); 
  // test: 1
      
  start = clock();
  prototype_write_speed(test*256);
  end = clock();   
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
  //printf("test 2: test size = %d \n",test);
  // test: 2 
    
  start = clock();
  prototype_matrix_calc(test,true);
  end = clock();   
  xy_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
  // test: 3
    
  //printf("test 3: test size = %d \n",test);
  start = clock();
  prototype_matrix_calc(test,false);
  end = clock();   
  yx_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
  // printf ("-- T: test %d time taken %lf %lf %lf \n",
  //   test,
  //   cpu_time_used,
  //   xy_time,
  //   yx_time
  //   );
  fprintf (H1,"%d %lf %6.3f %lf %lf\n",
       test,
       cpu_time_used,
       prototype_temperature_read() - temp_baseline,
       xy_time,
       yx_time
       );
  } 
}

/*
 * NAME: 
 *
 * main()
 *
 * DESCRIPTION: 
 *
 * main access point for the program.
 *
 * PARAMETRS:
 * 
 * n/a
 *
 * RETURN
 *
 * int 0 sucessfull 
 *
 */

int main(void)
{
double temp_baseline;
char model[40];
char cpucore[20];
FILE *H1,*H2;
uint32_t testruns;

// -- initialize

testruns = 500;

H1 = fopen("main.dat","w");
H2 = fopen("test.dat","w");
  
assert(H1!=NULL);
assert(H2!=NULL);
  
temp_baseline = prototype_temperature_read();  
  
// -- process

printf ("-- I: Raspberry Pi Simple Test [ver:%1.2f]\n",g_version);
  if (!prototype_modelname_read(model,cpucore))
    printf ("-- I: MOD %s \n",model); // revision
prototype_translate_information(model);
printf ("-- I: MEM %dGB\n",g_core.memory_size_gb);
printf ("-- I: REV %1.1f \n",g_core.revision);  
printf ("-- I: TEM (baseline):  %6.3f C (%s) \n", 
        temp_baseline, 
        temp_baseline > 45.0 ? "TOO HOT - NOT RUNNING" : "WARM");
printf ("-- I: TST %d\n",testruns);
  if (temp_baseline < 60.0)
    prototype_tests(H1,H2,temp_baseline,testruns);
printf ("-- I: TEM  %6.3f C\n", prototype_temperature_read()-temp_baseline);

// -- finalize

fflush(H1);
fflush(H2);

fclose(H1);
fclose(H2);

return 0;
}

