#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/list.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
}

int simple_init(int) {
	struct birthday *person;
	struct list_head birthday_list;
	LIST_HEAD(birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 13;
	person->month = 9;
	person->year = 1999;
	INIT_LIST_HEAD(&person->list);

	list_add_tail(&person->list, birthday_list);
}
