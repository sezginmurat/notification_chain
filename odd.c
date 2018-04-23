/* 
 * Odd number handler kernel module.
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
 * odd_handler_callback()
 *	Handler callback for odd numbers.
 */
static int odd_handler_callback(struct notifier_block *nb,
                                               unsigned long val,
                                               void *data)
{
        int counter =  (int)data;

        if (val == COUNTER_ODD_EVENT) {
                printk(KERN_INFO "Odd event handler: %d\n", counter);
        }

        return NOTIFY_DONE;
}


static struct notifier_block odd_nb = {
	.notifier_call = odd_handler_callback,
};

/*
 * odd_init()
 */
static int odd_init(void)
{
	counter_notifier_register(&odd_nb);

	return 0;
}

/*
 * odd_exit()
 */
static void odd_exit(void)
{
	counter_notifier_unregister(&odd_nb);
}
module_init(odd_init);
module_exit(odd_exit);

MODULE_DESCRIPTION("Odd Number Handler Module");
MODULE_LICENSE("Dual BSD/GPL");
