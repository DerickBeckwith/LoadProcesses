#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 1000 // matrix dimension is N x N
#define ITERATIONS 10 // how many times to do the multiplication

 double a[N][N],b[N][N],c[N][N]; // matrices c = b*a;

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

 if (sched_setscheduler(0, SCHED_RR, &param) == 0) {
	 policy = sched_getscheduler(0);
	 show_policy(policy);
 } else {
	 fprintf(stdout,"There was an error when trying to change the scheduling policy\n");
 }

 char  *myId;

 double value; 


 int i,j,k,n; // index variables
 time_t currTime ; 
 clock_t procTime; 
 // set the ID string 

 if (argc > 1) { 
	myId = (char *) malloc(strlen(argv[1]));
	strcpy(myId,argv[1]); 
 }  else { 
	myId = (char *) malloc(strlen("default"));
	strcpy(myId,"default");
 }

 procTime = clock();
 currTime = time(NULL);
 fprintf(stdout,"CPU bound process %s originated at %s\n",myId,ctime(&currTime));

 // fill the matrices with 0,1,2,3,etc
 value = (double) 1.0;

 for (i=0;i<N;i++) { 
    for (j=0;j<N;j++) { 
       a[i][j] = b[i][j] = value += (double) 1.0;
    }
  }

 // do the multiplication ITERATIONS  times 
 for (n = 0; n < ITERATIONS; n ++) { 
    // multiply a x b and store in c
    for (i = 0; i< N; i++) { 
      for (j = 0; j < N; j++) { 
        c[i][j] = (double) 0.0;
        for (k = 0; k < N; k++) { 
           c[i][j] += a[i][k]*b[k][j];
        }
      }
    }
  }

  procTime = clock()-procTime; 
  currTime = time(NULL);
  fprintf(stdout,"CPU bound process %s terminated at %s\n",myId,ctime(&currTime));
  fprintf(stdout,"CPU bound process %s took %d ticks, %f seconds\n",myId, procTime, ((float) procTime)/CLOCKS_PER_SEC); 
  return 0;
}

