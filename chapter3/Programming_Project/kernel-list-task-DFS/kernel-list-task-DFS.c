#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/sched.h>

// init_task there !!! (Ubuntu Desktop 20.04 LTS)
#include<linux/sched/task.h>
//extern struct task_struct init_task;

void dfs(struct task_struct *init_task) {	
	struct task_struct *task;
	struct list_head *list;	

	list_for_each(list, &init_task->children) {
		task = list_entry(list, struct task_struct, sibling);
                 pr_info(KERN_INFO "PID: %d\tName: %s\tState: %d", task->pid, task->comm, task->state);
	}
}

/* This function is called when the module is loaded. */
int simple_init(void) {
	printk(KERN_INFO "Loading Module\n");

	dfs(&init_task);

	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void){
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple module coded by Sang Le");
MODULE_AUTHOR("SL");
