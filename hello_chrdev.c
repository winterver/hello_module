#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/cdev.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Hello Char Device!");
MODULE_AUTHOR("winterver@github.com");
MODULE_LICENSE("GPL");

static int chrdev_open(struct inode* inode, struct file *filp);
static int chrdev_release(struct inode* inode, struct file *filp);
static ssize_t chrdev_read(struct file *filp, char __user * buf, size_t count, loff_t *ppos);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = chrdev_open,
    .release = chrdev_release,
    .read = chrdev_read,
};

static dev_t devno;
static struct cdev chrdev;

static int chrdev_init(void)
{
    int ret = 0;
    ret = alloc_chrdev_region(&devno, 0, 1, "hello_chrdev");
    if (ret < 0) {
        pr_info("Failed to alloc devno\n");
        goto alloc_err;
    }

    cdev_init(&chrdev, &fops);
    ret = cdev_add(&chrdev, devno, 1);
    if (ret < 0) {
        pr_info("Failed to add device\n");
        goto add_err;
    }

    pr_info("Hello Char Device!\n");
    return 0;

add_err:
    unregister_chrdev_region(devno, 1);
alloc_err:
    return ret;
}

static void chrdev_exit(void)
{
    unregister_chrdev_region(devno, 1);
    cdev_del(&chrdev);

    pr_info("Bye Char Device!\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

static int chrdev_open(struct inode* inode, struct file *filp)
{
    pr_info("Char Device opened!\n");
    return 0;
}

static int chrdev_release(struct inode* inode, struct file *filp)
{
    pr_info("Char Device released!\n");
    return 0;
}

static ssize_t chrdev_read(struct file *filp, char __user * buf, size_t count, loff_t *ppos)
{
    const char msg[] = "Hello! Here is Char Device!";
    const int msgsz = sizeof(msg) - 1;
    
    int tmp = count;
    while (tmp > 0) {
        int cpsz = msgsz < tmp ? msgsz : tmp;
        if (copy_to_user(buf, msg, cpsz) != 0) {
            pr_info("Char Device read failed!\n");
            return -EFAULT;
        }
        tmp -= cpsz;
    }

    return count;
}
