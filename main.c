#include <linux/module.h>

static int test_init(void)
{
    printk("Hello my module");
    return 0;
}

static void test_exit(void)
{
    printk("Bye my module");
}

module_init(test_init);
module_exit(test_exit);
