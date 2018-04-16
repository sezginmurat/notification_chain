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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/notifier.h>

static struct timer_list counter_timer;
static int count;

/*
 * counter_timer_callback()
 *	Callback is called in every 1 seconds.
 */
static void counter_timer_callback(unsigned long data)
{
	printk(KERN_INFO "count: %d\n", count++);

	counter_timer.expires = jiffies + HZ;
	add_timer(&counter_timer);

	/*
	 * TODO: Call the notification callback with the right flag
	 * (COUNTER_EVEN or COUNTER_ODD). 
	 */
}

/* Define the notification callback */
static ATOMIC_NOTIFIER_HEAD(counter_notifier_list);

/*
 * counter_notifier_register()
 */
void counter_notifier_register(struct notifier_block *nb)
{
	atomic_notifier_chain_register(&counter_notifier_list, nb);
}
EXPORT_SYMBOL(counter_notifier_register);

/*
 * counter_notifier_unregister()
 */
void counter_notifier_unregister(struct notifier_block *nb)
{
	atomic_notifier_chain_unregister(&counter_notifier_list, nb);
}
EXPORT_SYMBOL(counter_notifier_unregister);

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

	/*
	 * TODO: Implement these function in their own modules.
	 */
	odd_handler_init();
	even_handler_init();

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

	/*
	 * TODO: Implement these function in their own modules.
	 */
	even_handler_exit();
	odd_handler_exit();
}
module_init(counter_init);
module_exit(counter_exit);
