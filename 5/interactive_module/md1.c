#include <linux/init.h> 
#include <linux/module.h> 
#include "md.h"

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Ву Хай Данг");  
int md1_data = 7; 
extern int md1_func( void )
{ 
    return md1_data * 3; 
} 

static int md1_local(void)

{
    return md1_data * 3;
}

extern int md1_noexport(void)
{
    return md1_data * 3;
}

EXPORT_SYMBOL( md1_data ); 
EXPORT_SYMBOL( md1_func );

static int __init md_init( void ) { 
    printk( "+ module md1 start!\n" ); 
    return 0; 
}
static void __exit md_exit( void ) { 
    printk( "+ module md1 unloaded!\n" ); 
}
module_init( md_init ); 
module_exit( md_exit );