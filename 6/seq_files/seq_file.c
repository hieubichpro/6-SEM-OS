// #include <linux/module.h>
// #include <linux/kernel.h>
// #include <linux/proc_fs.h>
// #include <linux/seq_file.h>
// #include <linux/string.h>
// #include <linux/vmalloc.h>
// #include <asm/uaccess.h>

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Bogachenko Artem");

// #define DIRNAME "seqfiles"
// #define FILENAME "seqfile"
// #define SYMLINK "seqfile_link"
// #define FILEPATH DIRNAME "/" FILENAME

// #define MAX_COOKIE_LENGTH PAGE_SIZE

// static char tmp[256];

// static struct proc_dir_entry *proc_file = NULL;
// static struct proc_dir_entry *proc_dir = NULL;
// static struct proc_dir_entry *proc_link = NULL;

// static char *cookie_pot = NULL;  
// static int cookie_index = 0;  
// static int next_seq_file = 0;  

// ssize_t seq_file_write(struct file *filep, const char __user *buf, size_t len, loff_t *offp);

// static int seq_file_show(struct seq_file *m, void *v) {
  
//   printk(KERN_INFO "seq_file: show\n");
//   if (!cookie_index)
//     return 0;

//   if (next_seq_file >= cookie_index) {
//     next_seq_file = 0;
//   }

//   int len = snprintf(tmp, MAX_COOKIE_LENGTH, "%s\n", &cookie_pot[next_seq_file]);
//   seq_printf(m, "%s", tmp);
//   next_seq_file += len;
//   return 0;
// } 

// static void *seq_file_start(struct seq_file *m, loff_t *pos)
// {

//   printk(KERN_INFO "seq_file: start\n");
//   static unsigned long counter =0;
//   if (!*pos) {
//     return &counter;
//   }
//   else {
//     *pos = 0;
//     return NULL;
//   }
// }

// static void *seq_file_next(struct seq_file *m, void *v, loff_t *pos) {
//   printk(KERN_INFO "seq_file: next\n");
//   unsigned long *tmp = (unsigned long *) v;
//   (*tmp)++;
//   (*pos)++;
//   return NULL;
// }

// static void seq_file_stop(struct seq_file *m, void *v) {
//   printk(KERN_INFO "seq_file: stop\n");
// }

// static struct seq_operations seq_file_ops = {
//   .start = seq_file_start,
//   .next = seq_file_next,
//   .stop = seq_file_stop,
//   .show = seq_file_show
// };

// static int seq_file_open(struct inode *i, struct file * f) {
//     printk(KERN_DEBUG "seq_file: open seq_file\n");
//     return seq_open(f, &seq_file_ops);
// }

// static struct proc_ops ops = {
//     .proc_open = seq_file_open,
//     .proc_read = seq_read,
//     .proc_write = seq_file_write,
//     .proc_release = seq_release, 
// };

// void cleanup_seq_file_module(void);

// static int __init init_seq_file_module(void) {

//   cookie_pot = (char *)vmalloc(MAX_COOKIE_LENGTH);
//   if(!cookie_pot) {
//     printk(KERN_ERR "seq_file: vmalloc error\n");
//     return -ENOMEM;
//   }
//   memset(cookie_pot, 0, MAX_COOKIE_LENGTH);

//   proc_dir = proc_mkdir(DIRNAME, NULL);
//   if (!proc_dir) {
//     printk(KERN_INFO "seq_file: Couldn't create proc dir.\n");
    
//     cleanup_seq_file_module();
//     return -ENOMEM;
//   }

//   proc_file = proc_create(FILENAME, 0666, proc_dir, &ops);
//   if (!proc_file) {
//     printk(KERN_INFO "seq_file: Couldn't create proc file.\n");
//     cleanup_seq_file_module();
//     return -ENOMEM;
//   } 
//   proc_link = proc_symlink(SYMLINK, NULL, FILEPATH);
//   if (!proc_link)
//   {
//     printk(KERN_INFO "seq_file: Couldn't create proc symlink.\n");
//     cleanup_seq_file_module();
//     return -ENOMEM;
//   }

//   cookie_index = 0;
//   next_seq_file = 0;
//   printk(KERN_INFO "seq_file: Module loaded.\n");

//   return 0;
// }

// void cleanup_seq_file_module(void) {
//   if (cookie_pot)
//     vfree(cookie_pot);
//   if (proc_link)
//     remove_proc_entry(SYMLINK, NULL);
//   if (proc_file)
//     remove_proc_entry(FILENAME, proc_dir);
//   if (proc_dir)
//     remove_proc_entry(DIRNAME, NULL);
// }

// static void __exit exit_seq_file_module(void) {
//   cleanup_seq_file_module();
//   printk(KERN_INFO "seq_file: unloaded\n");
// }

// module_init(init_seq_file_module);
// module_exit(exit_seq_file_module);

// ssize_t seq_file_write(struct file *filep, const char __user *buf,
//     size_t len, loff_t *offp) {

//   printk(KERN_INFO "seq_file: write\n");

//   int avail = (MAX_COOKIE_LENGTH - cookie_index) + 1;

//   if(len > avail) {
//     printk(KERN_ERR "seq_file: cookie_pot overflow\n");
//     return -ENOSPC;
//   }

//   if(copy_from_user(&cookie_pot[cookie_index], buf, len)) {
    
//     printk(KERN_ERR "seq_file: copy from user error\n");
//     return -EFAULT;
//   }
//   if(cookie_index > 0)cookie_pot[cookie_index-1] = '\n';
//   cookie_index += len;
//   cookie_pot[cookie_index-1] = '\0';

//   return len;
// }

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");

static int limit = 1;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

static struct proc_dir_entry *seq_dir;
static struct proc_dir_entry *seq_file;
static struct proc_dir_entry *seq_symlink;

static void free_all(void)
{
    if (seq_symlink != NULL)
        remove_proc_entry("seq_symlink", NULL);

    if (seq_file != NULL)
        remove_proc_entry("seq_file", seq_dir);

    if (seq_dir != NULL)
        remove_proc_entry("seq_dir", NULL);
}

/**
 * start
 */
static void *ct_seq_start(struct seq_file *m, loff_t *pos)
{
    // printk(KERN_INFO "seq: call seq_start");

    printk(KERN_INFO "seq: call seq_start, m = %p, pos = %p, *pos = %Ld\n",
           m, pos, *pos);

    if (*pos >= limit)
    { 
        printk(KERN_INFO "Apparently, we're done.\n");
        return NULL;
    }

    loff_t *spos = kmalloc(sizeof(loff_t), GFP_KERNEL);

    if (!spos)
        return NULL;

    *spos = *pos;
    return spos;
}

/**
 * show
 */
static int ct_seq_show(struct seq_file *m, void *v)
{
    // printk(KERN_INFO "seq: call seq_show");
    loff_t *pos = (loff_t *)v;

    printk(KERN_INFO "seq: call seq_show, m = %p, v = %p\n", m, v);

    printk(KERN_INFO "cseq: current: comm - %s; pid - %d; parent comm - %s; ppid - %d; state - %d; on_cpu - %d; flags - %x; prio - %d; policy - %d; exit_state - %d; exit_code - %d; in_execve - %x; utime - %llu\n",
           current->comm,
           current->pid,
           current->parent->comm,
           current->parent->pid,
           current->__state,
           current->on_cpu,
           current->flags,
           current->prio,
           current->policy,
           current->exit_state,
           current->exit_code,
           current->in_execve,
           current->utime);

    loff_t *spos = v;
    seq_printf(m, "%lld \n", (long long)*spos);
    seq_printf(m, "current:\ncomm - %s\npid - %d\nparent comm - %s\nppid - %d\nstate - %d\non_cpu - %d\nflags - %x\nprio - %d\npolicy - %d\nexit_state - %d\nexit_code - %d\nin_execve - %x\nutime - %llu\n",
               current->comm,
               current->pid,
               current->parent->comm,
               current->parent->pid,
               current->__state,
               current->on_cpu,
               current->flags,
               current->prio,
               current->policy,
               current->exit_state,
               current->exit_code,
               current->in_execve,
               current->utime);

    return 0;
}

/**
 * next
 */
static void *ct_seq_next(struct seq_file *m, void *v, loff_t *pos)
{

    printk(KERN_INFO "seq: call seq_next, m = %p, v = %p, pos = %p, *pos = %Ld\n",
           m, v, pos, *pos);

    loff_t *spos = v;
    *pos = ++*spos;

    if (*pos >= limit) // заканчиваем?
        return NULL;

    return v;
}

/**
 * stop
 */
static void ct_seq_stop(struct seq_file *m, void *v)
{
    printk(KERN_INFO "seq: call seq_stop, m = %p, v = %p\n", m, v);

    kfree(v);
}

/**
 * Эта структура собирает функции для управления последовательным чтением
 */
static struct seq_operations ct_seq_ops = {
    .start = ct_seq_start,
    .next = ct_seq_next,
    .stop = ct_seq_stop,
    .show = ct_seq_show};

/**
 * Эта функция вызывается при открытии файла из /proc
 */
static int ct_seq_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "seq: call seq_open\n");

    return seq_open(file, &ct_seq_ops);
};

static ssize_t ct_seq_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    printk(KERN_INFO "seq: call seq_read\n");

    return seq_read(file, buf, size, ppos);
}

static int ct_seq_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "seq: call seq_release\n");

    return seq_release(inode, file);
}

static loff_t ct_seq_lseek(struct file *file, loff_t offset, int whence)
{
    printk(KERN_INFO "seq: call seq_lseek\n");

    return seq_lseek(file, offset, whence);
}

/**
 * Эта структура собирает функции для управления файлом из /proc
 */
#ifdef HAVE_PROC_OPS
static const struct proc_ops ct_file_ops = {
    .proc_read = ct_seq_read,
    .proc_open = ct_seq_open,
    .proc_release = ct_seq_release,
    .proc_lseek = ct_seq_lseek};
#else
static struct file_operations ct_file_ops = {
    .owner = THIS_MODULE,
    .open = ct_seq_open,
    .read = ct_seq_read,
    .llseek = ct_seq_lseek,
    .release = ct_seq_release};
#endif

/**
 * Эта функция вызывается, когда этот модуль загружается в ядро
 */
static int __init ct_init(void)
{
    printk(KERN_INFO "seq: call ct_init\n");

    if ((seq_dir = proc_mkdir("seq_dir", NULL)) == NULL)
    {
        free_all();
        printk(KERN_ERR "seq: [ERROR] [can't create fortune dir]\n");

        return -ENOMEM;
    }

    if ((seq_file = proc_create("seq_file", 0666, seq_dir, &ct_file_ops)) == NULL)
    {
        free_all();
        printk(KERN_ERR "seq: [ERROR] [can't create fortune file]\n");

        return -ENOMEM;
    }

    if ((seq_symlink = proc_symlink("seq_symlink", NULL, "seq_dir/seq_file")) == NULL)
    {
        free_all();
        printk(KERN_ERR "seq: [ERROR] [can't create fortune symlink]\n");

        return -ENOMEM;
    }

    printk(KERN_INFO "seq: loaded\n");

    return 0;
}

/**
 * Эта функция вызывается, когда этот модуль удаляют из ядра
 */
static void __exit ct_exit(void)
{
    free_all();

    printk(KERN_INFO "seq: сall md_exit\n");
}

module_init(ct_init);
module_exit(ct_exit);