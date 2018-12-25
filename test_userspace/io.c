
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct process_switch_info
{
	unsigned int   counter;
	struct timeval time;
};

struct PROCESSTIME {
        pid_t pid;
        unsigned long pswcount; //process switch counter
        unsigned long ioswcount; // IO switch counter;
        struct timespec nprocess;
        struct timespec niotime;
        struct timeval process; // process timeing
        struct timeval iotime;  // iotimeing

        // for temperature use
        struct timeval temp1;
        struct timeval temp2;
        struct timeval temp3;
        struct timeval temp4;
        struct timespec ntemp1;
        struct timespec ntemp2;
        struct timespec ntemp3;
        struct timespec ntemp4;

};

struct PROCESSTIME pt;

int main() {
FILE             *out;
char             c;
int              a;
struct timeval   start, end;
long int ret_status; 
unsigned long temp_ul;

pid_t current_pid; 

current_pid = getpid();

printf("Current Pid :%d \n", current_pid);

ret_status = syscall(360, current_pid, &pt); // start systemcall 
gettimeofday(&start, NULL);                 //total time of existence - begin

if((out=fopen("io_bound.data","w"))!=NULL) {
		c=38;
		for(a=0; a<100000;a++){
				putc(c,out);
				if(++c>126)
				c=38;
		}
		fclose(out);
}
else
{
		printf("Cannot open file.\n");
		exit(0);
}

/*===========================================================================*/
/*     prototype of the new system call is as follows                        */
/*     void get_process_switch_info(struct process_switch_info *)            */
/*===========================================================================*/
//get_process_switch_info(&ps_info);    //new system call
ret_status = syscall(360, current_pid, &pt); // get systemcall  result 

gettimeofday(&end, NULL);             //total time of existence - end
printf("The process spent %ld uses in the system after it stated its execution.\n", 
((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

temp_ul=(pt.nprocess.tv_sec * 1000000000 + pt.nprocess.tv_nsec);
printf("The process has made %ld process switches\n", pt.pswcount);
printf("This process has idle %lu nsecs\n", temp_ul); 
}
