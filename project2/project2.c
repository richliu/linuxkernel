
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include "project2.h"

int (*project2_hook)(int pid, char *result) = NULL;
int project2_hook_ready = 0 ; 


struct PROCESSTIME pt; 

int localinit = 0;

asmlinkage long sys_linux_survey_TT(int pid, char *buf) {
	printk("[%s] pid: %d : buf:%p \n", __FUNCTION__, pid, buf);

if (localinit == 0 || pt.pid != pid){
	//pt = &ppt;
	memset(&pt,0, sizeof(struct PROCESSTIME));
	localinit = 1;
	pt.pid = pid;
}else{
	printk("pid:%d count:%ld \n", pt.pid, pt.pswcount);
	printk("seconds:%lu.%09lu \n", (unsigned long)pt.nprocess.tv_sec, (unsigned long)pt.nprocess.tv_nsec);
	copy_to_user(buf,&pt,sizeof(struct PROCESSTIME));
}


	if(1 == project2_hook_ready){
		project2_hook(pid, buf);
	}
return 0;
}
asmlinkage long sys_listProcessInfo(void) {
    struct task_struct *proces;
 
    for_each_process(proces) {
 
    printk(
      "Process: %s\n \
       PID_Number: %ld\n \
       Process State: %ld\n \
       Priority: %ld\n \
       RT_Priority: %ld\n \
       Static Priority: %ld\n \
       Normal Priority: %ld\n", \
       proces->comm, \
       (long)task_pid_nr(proces), \
       (long)proces->state, \
       (long)proces->prio, \
       (long)proces->rt_priority, \
       (long)proces->static_prio, \
       (long)proces->normal_prio \
    );
  
  
   if(proces->parent) 
      printk(
        "Parent process: %s, \
         PID_Number: %ld", \ 
         proces->parent->comm, \
         (long)task_pid_nr(proces->parent) \
      );
  
   printk("\n\n");
  
  }
  
  return 0;
}

EXPORT_SYMBOL(project2_hook);
EXPORT_SYMBOL(project2_hook_ready);
EXPORT_SYMBOL(pt);
