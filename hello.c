#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_DESCRIPTION("Hello Module!");
MODULE_AUTHOR("winterver@github.com");
MODULE_LICENSE("GPL");

static int hello_init(void)
{
    pr_info("Hello Module!\n");
    return 0;
}

static void hello_exit(void)
{
    pr_info("Bye Module!\n");
}

module_init(hello_init);
module_exit(hello_exit);
