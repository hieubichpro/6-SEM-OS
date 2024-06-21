#include <linux/init.h> 
#include <linux/module.h> 
#include "md.h" 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Vu H. D"); 

static int __init md_init( void ) { 
    printk( "+ module md2 start!\n" ); 
    printk( "+ data number exported from md1 : %d\n", md1_data ); 
    printk( "+ number returned md1_proc() is : %d\n", md1_func() ); 

    // printk( "+ number returned md1_local() : %d\n", md1_local() );
    // printk( "+ number returned md1_noexport() : %d\n", md1_noexport() ); 

    return 0; 
} 
static void __exit md_exit( void ) { 
    printk( "+ module md2 unloaded!\n" ); 
} 
module_init( md_init ); 
module_exit( md_exit );