#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/list.h>
#include<linux/slab.h>

static LIST_HEAD(birthday_list);

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};


struct birthday *person;
struct birthday *person1;
struct birthday *person2;
struct birthday *person3;
struct birthday *person4;

int simple_init(void) {

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 13;
	person->month = 9;
	person->year = 1999;
	INIT_LIST_HEAD(&person->list);

	list_add_tail(&person->list, &birthday_list);

	person1 = kmalloc(sizeof(*person1), GFP_KERNEL);
	person1->day = 25;
	person1->month = 8;
	person1->year = 1999;
	INIT_LIST_HEAD(&person1->list);

	list_add_tail(&person1->list, &person->list);

	person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
	person2->day = 25;
	person2->month = 9;
	person2->year = 1998;
	INIT_LIST_HEAD(&person2->list);

	list_add_tail(&person2->list, &person1->list);

	person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
	person3->day = 24;
	person3->month = 12;
	person3->year = 1996;
	INIT_LIST_HEAD(&person3->list);

	list_add_tail(&person3->list, &person2->list);

	person4 = kmalloc(sizeof(*person4), GFP_KERNEL);
	person4->day = 19;
	person4->month = 8;
	person4->year = 1995;
	INIT_LIST_HEAD(&person4->list);

	list_add_tail(&person4->list, &person3->list);

	struct birthday *ptr;

	list_for_each_entry(ptr, &birthday_list, list) {
		printk(KERN_INFO "Date: %d, Month %d, Year %d", ptr->day, ptr->month, ptr-> year);
	}
	
	return 1;
}

void simple_exit(void) {
	struct birthday *ptr, *next;
	
	list_for_each_entry_safe(ptr,next,&birthday_list,list) {
		/* on each iteration ptr points */
		/* to the next birthday struct */
		list_del(&ptr->list);
		kfree(ptr);
	}	
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DEMO USING LINKED LIST");
MODULE_AUTHOR("SGG");
