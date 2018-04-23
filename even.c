/* 
 * Even number handler kernel module.
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

/*
 * even_handler_callback()
 *	Handler callback for even numbers.
 */
static int even_handler_callback(struct notifier_block *nb,
                                               unsigned long val,
                                               void *data)
{
        int counter =  (int)data;

        if (val == COUNTER_EVEN_EVENT) {
                printk(KERN_INFO "Even event handler: %d\n", counter);
        }

        return NOTIFY_DONE;
}

static struct notifier_block even_nb = {
	.notifier_call = even_handler_callback,
};

/*
 * even_init()
 */
static int even_init(void)
{
	counter_notifier_register(&even_nb);

	return 0;
}

/*
 * even_exit()
 */
static void even_exit(void)
{
	counter_notifier_unregister(&even_nb);
}
module_init(even_init);
module_exit(even_exit);

MODULE_DESCRIPTION("Even Number Handler Module");
MODULE_LICENSE("Dual BSD/GPL");
