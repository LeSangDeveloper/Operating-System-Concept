#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/sched.h>

struct task_struct *task;

int simple_init(void) {
	printk(KERN_INFO "Loading list task module");
	
	for_each_process(task) {
		printk(KERN_INFO "task 1");
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
