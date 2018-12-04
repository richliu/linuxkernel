
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function

#include "project1.h"
#define  DEVICE_NAME "ebbchar"    ///< The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "ebb"        ///< The device class -- this is a character device driver
 
MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Richard Liu");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Loadable module for debug syscall");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users


void ptdump_walk_pgd(void);

long project1_sys_func(int pid, char *result);

long project1_sys_func(int pid, char *result)
{
  struct task_struct *task;
  struct mm_struct *mm;
  void *cr3_virt;
  unsigned long cr3_phys;

  task = pid_task(find_vpid(pid), PIDTYPE_PID);

  if (task == NULL)
    return 0; // pid has no task_struct

  mm = task->mm;

  // mm can be NULL in some rare cases (e.g. kthreads)
  // when this happens, we should check active_mm
  if (mm == NULL) {
    mm = task->active_mm;
  }

  if (mm == NULL)
    return 0; // this shouldn't happen, but just in case

  struct vm_area_struct *vma = mm->mmap;
  while (vma != NULL){
    printk("PID: %d virtual: %08lx-%08lx\n", pid, vma->vm_start, vma->vm_end);
    printk("PID: %d physical: %08lx-%08lx\n", pid, virt_to_phys(vma->vm_start), virt_to_phys(vma->vm_end));
    vma = vma->vm_next;
  }

   ptdump_walk_pgd();
  return 0;
}

#define PTRS_PER_PGD	4
#define PTRS_PER_PUD	1
#define PTRS_PER_PMD	512
#define PTRS_PER_PTE	512


void ptdump_walk_pgd(void){
  pgd_t *start = pid_task(find_vpid(1), PIDTYPE_PID)->mm->pgd; // swapper_pg_dir, from init_mm
  int i;
  for(i=0;i<PTRS_PER_PGD;i++){
    if(pgd_val(start[i]) && pgd_present(start[i])){
      printk("PUD: %08lx\n", start[i]); // print pud loc
      pud_t *pud = (pud_t *)pgd_page_vaddr(start[i]);
      int j;
      for(j=0;j<PTRS_PER_PUD;j++){
	if(!pud_none(pud[j])){
	  printk("PMD: %08lx\n", pud[i]); // print pmd loc
	  pmd_t *pmd = (pmd_t *)pud_page_vaddr(pud[j]);
	  int k;
	  for(k=0;k<PTRS_PER_PMD;k++){
	    if(!pmd_none(pmd[k]) && pmd_present(pmd[k])){
	      printk("PTE: %08lx\n", pmd[k]); // print pte loc
	      if((unsigned long)(pmd+k) & 0x00000002){
	        break;
	      }
	      pte_t *pte = (pte_t *)pmd_page_vaddr(pmd[k]);
	      int L;
	      for(L=0;L<PTRS_PER_PTE;L++){
		if(pte_none(pte[L]) || pte_present(pte[L])){
		  break;
		}
	      }
	      printk("SIZE: %d\n", L);
	    }
	  }
	}
      }
    }
  }
}

static int __init project1_init(void){
    project1_hook = project1_sys_func;
    project1_hook_ready = 1;
    return 0;
}
static void __exit project1_exit(void){
    project1_hook_ready = 0;
    project1_hook = NULL;

}

module_init(project1_init);
module_exit(project1_exit);
