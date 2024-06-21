#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

#define MYFS_MAGIC_NUMBER 0x13131313;
#define SLABNAME "myfs_slab"

static struct inode_operations const myfs_inode_ops = {
	.lookup = simple_lookup,
};

static struct file_operations const myfs_fops = {
	.owner = THIS_MODULE,
};

struct myfs_inode
{
    int i_mode;
    unsigned long i_ino;
} myfs_inode;

static void myfs_put_super(struct super_block *sb)
{
    printk(KERN_INFO "MYFS superblock destroy called\n");
}

static struct super_operations const myfs_sup_ops = {
    .put_super = myfs_put_super,
    .statfs = simple_statfs,
    .drop_inode = generic_delete_inode,
};

static int my_simple_fill_super(struct super_block *s, void *data, int silent)
{
	struct inode *inode;
	struct dentry *root;
	
	s->s_blocksize = PAGE_SIZE;
	s->s_blocksize_bits = PAGE_SHIFT;
	s->s_magic = MYFS_MAGIC_NUMBER;
	s->s_op = &myfs_sup_ops;
	s->s_time_gran = 1;

	inode = new_inode(s);
	if (!inode)
    {
        printk (KERN_ERR "MYFS inode allocation failed!\n");
		return -ENOMEM;
    }

	inode->i_ino = 1;
	inode->i_mode = S_IFDIR | 0755;
	inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);
	inode->i_op = &myfs_inode_ops;
	inode->i_fop = &myfs_fops;
	set_nlink(inode, 2);
	root = d_make_root(inode);
	if (!root)
    {
        printk(KERN_ERR "MYFS root creation failed!\n");
        d_genocide(root);
        shrink_dcache_parent(root);
        dput(root);
        return -ENOMEM;
    }
    s->s_root = root;
	return 0;
	
}

static void my_kill_block_super(struct super_block *sb)
{
    printk(KERN_INFO "MYFS kill super block\n");

    kill_anon_super(sb);
}

static struct dentry* myfs_mount(struct file_system_type *type, int flags, char const *dev, void *data)
{
    struct dentry* const entry = mount_nodev(type, flags, data, my_simple_fill_super); 
    if (IS_ERR(entry))
        printk(KERN_ERR "MYFS mounting failed !\n");
    else
        printk(KERN_DEBUG "MYFS mounted!\n");
    return entry;
}

static struct file_system_type myfs_type =
{
    .owner = THIS_MODULE,
    .name = "myfs",
    .mount = myfs_mount,
    .kill_sb = my_kill_block_super,
};

static int number = 128;
static struct kmem_cache *cache = NULL;
static void* *slab = NULL;

static void ctor(void *p)
{
    *(int *)p = (int)p;
}

static int __init myfs_init(void)
{
    int ret = register_filesystem(&myfs_type);
    if(ret != 0)
    {
        printk(KERN_ERR "MYFS_MODULE cannot register filesystem!\n");
        return ret;
    }

    slab = kmalloc(sizeof(void*) * number, GFP_KERNEL);
    if(!slab)
    {
        printk(KERN_ERR "MYFS_MODULE kmalloc error\n" );
        kfree(slab);
        return -ENOMEM;
    }
    
    cache = kmem_cache_create_usercopy(SLABNAME, sizeof(myfs_inode), 0, SLAB_HWCACHE_ALIGN, 0, 0, ctor);
    if(!cache)
    {
        printk( KERN_ERR "MYFS_MODULE kmem_cache_create error\n" );
        kmem_cache_destroy(cache);
        kfree(slab);
        return -ENOMEM;
    }
    int i, j;
    for(i = 0; i < number; i++)
    {
		if(!(slab[i] = kmem_cache_alloc(cache, GFP_KERNEL)))
		{
		    printk(KERN_ERR "MYFS_MODULE kmem_cache_alloc error\n");
		    for (j = 0; j < i; j++)
		    {
		    	kmem_cache_free(cache, slab[j]);
		    }
		    kmem_cache_destroy(cache);
		    kfree(slab);
        	return -ENOMEM;
		}
	}
	
    printk(KERN_INFO "MYFS_MODULE loaded\n");
    return 0;
}

static void __exit myfs_exit(void)
{
    printk(KERN_INFO "MYFS_MODULE unloading\n");
    int j;
    for (j  = 0; j < number; j++)
	{
		kmem_cache_free(cache, slab[j]);
	}
	kmem_cache_destroy(cache);
	kfree(slab);
	
	printk(KERN_INFO "MYFS_MODULE cache mem freed\n");

    int ret = unregister_filesystem(&myfs_type);
    if (ret != 0)
        printk(KERN_ERR "MYFS_MODULE cannot unregister filesystem !\n");
    printk(KERN_DEBUG "MYFS_MODULE unloaded !\n");
}

module_init(myfs_init);
module_exit(myfs_exit);


// назначение ФС
// inode dentry super_block 
// slab
// 