#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/types.h>

#include "linux/ktime.h"
#include "linux/slab.h"
#include "linux/list.h"

MODULE_AUTHOR("Bohdanenko Vladyslav IV-81");
MODULE_DESCRIPTION("Hello world printing Linux Kernel Training for Lab7");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "How many times hello world will be repeated");

struct list_time {
	struct list_head next;
	ktime_t time;
};

static LIST_HEAD(head);

static int __init hello_init(void)
{
	struct list_time *tail;
	uint i;
	if (count == 0) {
		printk(KERN_WARNING "count=%i is 0\n", count);
		return 0;
	} else if (count >= 5 && count <= 10) {
		printk(KERN_WARNING "count=%i in range of 5,10\n", count);
	}

	BUG_ON(count > 10);
	for (i = 0; i < count; i++) {
		tail = kmalloc(sizeof(struct list_time), GFP_KERNEL);
		if (i == 5)
			tail = 0;
		if (ZERO_OR_NULL_PTR(tail))
			goto mem_err;
		tail->time = ktime_get();
		printk(KERN_INFO "Hello world\n");

		list_add_tail(&(tail->next), &head);
	}
	return 0;

mem_err:
	{
		struct list_time *md, *tmp;
		printk(KERN_ERR "Runned out of memory");
		list_for_each_entry_safe(md, tmp, &head, next) {
			list_del(&md->next);
			kfree(md);
		}
		BUG();
		return -ENOMEM;
	}
}

static void __exit hello_exit(void)
{
	struct list_time *md, *tmp;
	list_for_each_entry_safe(md, tmp, &head, next) {
			list_del(&md->next);
			kfree(md);
	}
	BUG_ON(!list_empty(&head));
}

module_init(hello_init);
module_exit(hello_exit);
