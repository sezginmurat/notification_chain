/* 
 * Sample counter kernel module.
 *
 *	This module runs a timer and it increases a counter variable
 *	and sends the value of this counter as an atomic notification
 *	message to the other modules which are registered to these
 *	notification messages.
 *
 * (C) 2018 by Murat Sezgin <msezgin@iotdweb.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/notifier.h>
#include "counter.h"

static struct timer_list counter_timer;
static int count;

/* Define the notification callback */
static ATOMIC_NOTIFIER_HEAD(counter_notifier_list);

/*
 * counter_timer_callback()
 *	Callback is called in every 1 seconds.
 */
static void counter_timer_callback(unsigned long data)
{
	count++;
	counter_timer.expires = jiffies + HZ;
	add_timer(&counter_timer);

	/*
	 * Call the notification callback with the right flag
	 * (COUNTER_EVEN or COUNTER_ODD). 
	 */
	if ((count % 2) != 0) {
		atomic_notifier_call_chain(&counter_notifier_list,
                                           COUNTER_ODD_EVENT,
                                           (void *)count);
	} else {
		atomic_notifier_call_chain(&counter_notifier_list,
                                           COUNTER_EVEN_EVENT,
                                           (void *)count);
	}
}

/*
 * counter_notifier_register()
 */
void counter_notifier_register(struct notifier_block *nb)
{
	atomic_notifier_chain_register(&counter_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(counter_notifier_register);

/*
 * counter_notifier_unregister()
 */
void counter_notifier_unregister(struct notifier_block *nb)
{
	atomic_notifier_chain_unregister(&counter_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(counter_notifier_unregister);

/*
 * counter_init()
 */
static int counter_init(void)
{
	printk(KERN_INFO "counter init\n");

	/*
	 * Initialize the timer and add it to the timer list.
	 */
	init_timer(&counter_timer);
	counter_timer.function = counter_timer_callback;
	counter_timer.data = 0;
	counter_timer.expires = jiffies + HZ;
	add_timer(&counter_timer);

	return 0;
}

/*
 * counter_exit()
 */
static void counter_exit(void)
{
	printk(KERN_INFO "counter exit\n");

	/*
	 * Delete the timer.
	 */
	del_timer_sync(&counter_timer);
}
module_init(counter_init);
module_exit(counter_exit);

MODULE_DESCRIPTION("Counter Module");
MODULE_LICENSE("Dual BSD/GPL");
