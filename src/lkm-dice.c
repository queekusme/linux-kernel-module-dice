#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Annabelle Kennedy");
MODULE_DESCRIPTION("Kernel Module to roll dice");
MODULE_VERSION("0.1");

static int __init lkm_dice_init(void)
{
    printk(KERN_INFO "Hello, World!\n");
    return 0;
}

static void __exit lkm_dice_exit(void)
{
    printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_dice_init);
module_exit(lkm_dice_exit);
