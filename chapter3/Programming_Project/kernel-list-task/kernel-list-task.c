#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/sched.h>

/*something it needs to be removed*/
#include<linux/sched/signal.h>

struct task_struct *task;

int simple_init(void) {
	printk(KERN_INFO "Loading list task module");
	
	for_each_process(task) {
		 pr_info(KERN_INFO "PID: %d\tName: %s\tState: %d", task->pid, task->comm, task->state);
	}

	return 0;
}

void simple_exit(void) {
	printk(KERN_INFO "Removing list task module\n");
}

module_init(simple_init);
module_exit(simple_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("List task module");
MODULE_AUTHOR("LE SANG");
