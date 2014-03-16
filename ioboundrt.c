#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 1000 // how many times to do the  file operation
#define FILENAMEBASE "loadTempFile"
#define FILELENGTH 100000 

void show_policy(int policy) {
	switch (policy) {
		case SCHED_OTHER:
			printf ("Policy is normal\n");
			break;
		case SCHED_RR:
			printf ("Policy is round-robin\n");
			break;
		case SCHED_FIFO:
			printf ("Policy is first-in, first-out\n");
			break;
		case 4:
			printf ("Policy is wfb\n");
			break;
		case -1:
			perror ("sched_getscheduler");
			break;
		default:
			fprintf (stderr, "Unknown policy!\n");
	}
}

int main(int argc,char *argv[]) {

 int policy = sched_getscheduler(0);
 show_policy(policy);

 struct sched_param param;

 if (sched_setscheduler(0, SCHED_FIFO, &param) == 0) {
	 policy = sched_getscheduler(0);
	 show_policy(policy);
 } else {
	 fprintf(stdout,"There was an error when trying to change the scheduling policy\n");
 }

 char  *myId;
 char c; 



 int i,j,k,n; // index variables
 time_t currTime ;  // wall-clock time variable 
 clock_t procTime;  // ticks since process began
 FILE *fp; // file pointer 
 char *filename; 
 
// set the ID string 

 if (argc > 1) { 
	myId = (char *) malloc(strlen(argv[1]));
	strcpy(myId,argv[1]); 
 }  else { 
	myId = (char *) malloc(strlen("default"));
	strcpy(myId,"default");
 }

 // create the filename based on the string
 filename = (char *) malloc(strlen(FILENAMEBASE)+strlen(myId)+1);
 strcpy(filename,FILENAMEBASE);
 strcat(filename,myId);
 
 procTime = clock();
 currTime = time(NULL);
 fprintf(stdout, "IO bound process %s originated at %s\n",myId,ctime(&currTime));

for (n = 0; n < ITERATIONS; n++) { 
 // write the file
 fp = fopen(filename,"w");
 for(i=0;i < FILELENGTH;i++) { 
	fputc('x',fp); 
 } 
 fclose(fp);

 //read the file 
 fp = fopen(filename,"r");
 while (!feof(fp)) { 
   c = fgetc(fp); 
 } 
 fclose(fp); 

} 
 procTime = clock()- procTime; // number of ticks 
  currTime = time(NULL);
  fprintf(stdout,"IO bound process %s terminated at %s\n",myId,ctime(&currTime));
  fprintf(stdout,"IO bound process %s took ticks = %d, seconds = %f \n",myId, procTime, ((float) procTime)/CLOCKS_PER_SEC); 
  return 0;
}
