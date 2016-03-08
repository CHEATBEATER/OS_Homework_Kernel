#include <linux/module.h>

MODULE_LICENSE("GPL");

int init_module(void) {
    printk("hello world\n");
    return 0;
}

void cleanup_module(void) {
    printk("byebye\n");
}

