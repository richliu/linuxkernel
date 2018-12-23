          
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define TOTAL_ITERATION_NUM  100000000

struct process_switch_info
{
     unsigned int   counter;
     struct timeval time;
};


struct PROCESSTIME {
        int pid;
        int pswcount; //process switch counter
        int ioswcount; // IO switch counter;
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

main()
{
int              a;
unsigned int     i=0;
struct timeval   start, end;
long int ret_status; 

pid_t current_pid; 

current_pid = getpid();

printf("Current Pid :%d \n", current_pid);

ret_status = syscall(360, current_pid, &pt); // start systemcall 
gettimeofday(&start, NULL);           //total time of existence - begin

for(i=1;i<=TOTAL_ITERATION_NUM;i++)
	++a;

   /*===========================================================================*/
   /*     prototype of the new system call is as follows                        */
   /*     void get_process_switch_info(struct process_switch_info *)            */
   /*===========================================================================*/
//get_process_switch_info(&ps_info);    //new system call
ret_status = syscall(360, current_pid, &pt); // get systemcall  result 

/*
gettimeofday(&end, NULL);             //total time of existence - end 
printf("The process spent %ld uses in the system after it stated its execution.\n", 
((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
temp_ul=(ps_info.time.tv_sec * 1000000 + ps_info.time.tv_usec);
printf("The process has made %ld process switches\n", ps_info.counter);
printf("This process has idle %ul usecs\n", temp_ul); 
*/

}
