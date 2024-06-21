#include <linux/init.h> 
#include <linux/module.h> 
#include "md.h" 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Vu Hai Dang"); 

static int __init md_init( void ) { 
    printk( "+ module md3 start!\n" ); 
    printk( "+ data int exported from md1 : %d\n", md1_data ); 
    printk( "+ number returned md1_func() is : %d\n", md1_func() ); 
    // printk( "+ number returned md1_local() is : %d\n", md1_noexport() ); 
    return 1; 
} 
static void __exit md_exit( void ) { 
    printk( "+ module md3 unloaded!\n" ); 
} 
module_init( md_init ); 
module_exit( md_exit );