#ifndef _PROJECT2_H_
#define _PROJECT2_H_
asmlinkage long sys_listProcessInfo(void);
asmlinkage long sys_linux_survey_TT(int pid, char *buf);

extern int project2_hook_ready;
extern int (*project2_hook)(int pid, char *result);

struct PROCESSTIME {
		int pid;
		int pswcount; //process switch counter
		int ioswcount; // IO switch counter;
		struct timeval process; // process timeing
		struct timeval iotime;  // iotimeing 

		// for temperature use 
		struct timeval temp1;
		struct timeval temp2;
		struct timeval temp3;
		struct timeval temp4;
}; 


#endif
