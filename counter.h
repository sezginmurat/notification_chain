/* 
 * Counter kernel module defines and public function definitions.
 *
 * (C) 2018 by Murat Sezgin <msezgin@iotdweb.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define COUNTER_ODD_EVENT 1
#define COUNTER_EVEN_EVENT 2

void counter_notifier_register(struct notifier_block *nb);
void counter_notifier_unregister(struct notifier_block *nb);
