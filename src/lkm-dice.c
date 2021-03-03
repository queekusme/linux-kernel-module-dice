#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Annabelle Kennedy");
MODULE_DESCRIPTION("Kernel Module to roll dice");
MODULE_VERSION("0.1");

#define SUCCESS 0
#define DEVICE_NAME "dice"	/* Device Name */
#define DICE_ASCII_SIZE 3

static int majorId;		    /* Major number assigned */
static int in_use = 0;	    /* Device In Use? */
static int diceRoll = 0;

// 0 terminated ascii character
char asciiDiceRoll[DICE_ASCII_SIZE];
static char *msg_Ptr;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init lkm_dice_init(void)
{
    majorId = register_chrdev(0, DEVICE_NAME, &fops);

    if (majorId < 0) {
      printk(KERN_ALERT "[DICE] Registering char device failed with %d\n", majorId);
      return majorId;
    }

    printk(KERN_INFO "[DICE] Major ID %d assigned\n", majorId);

    return SUCCESS;
}

static void __exit lkm_dice_exit(void)
{
    unregister_chrdev(majorId, DEVICE_NAME);

    printk(KERN_ALERT "[DICE] Dice unregistered");
}

static int device_open(struct inode *inode, struct file *file)
{
    uint i, dice;

    if (in_use)
        return -EBUSY;

    in_use++;

    // Notify Kernel we are in use
    try_module_get(THIS_MODULE);

    // Generate random number for this roll
    dice = 6;
    get_random_bytes(&i, sizeof(i));
    diceRoll = (i % 6) + 1;

    // Move ascii data to asciiDiceRoll buffer
    snprintf(asciiDiceRoll, DICE_ASCII_SIZE, "%d\n", diceRoll);
    msg_Ptr = asciiDiceRoll;

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    in_use--;

    // Notify Kernel we are no longer in use
    module_put(THIS_MODULE);

    return SUCCESS;
}

/*  
 * Called when a process writes to dev file: cat /dev/dice 
 */
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset)
{

    /*
     * Number of bytes actually written to the buffer 
     */
    int bytes_read = 0;

    /*
     * If we're at the end of the message, 
     * return 0 signifying end of file 
     */
    if (*msg_Ptr == 0)
        return 0;

    /* 
     * Actually put the data into the buffer 
     */
    while (length && *msg_Ptr)
    {

        /* 
         * The buffer is in the user data segment, not the kernel 
         * segment so "*" assignment won't work.  We have to use 
         * put_user which copies data from the kernel data segment to
         * the user data segment. 
         */
        put_user(*(msg_Ptr++), buffer++);

        length--;
        bytes_read++;
    }

    return bytes_read;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/dice 
 */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    return -EINVAL;
}

module_init(lkm_dice_init);
module_exit(lkm_dice_exit);
