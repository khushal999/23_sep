
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>             //kernel threads
#include <linux/sched.h>               //task_struct 
#include <linux/delay.h>
 
static int __init my_driver_init(void);
static void __exit my_driver_exit(void);

int count =10;

static struct task_struct *my_thread;

int thread_function(void *pv);

int thread_function(void *pv)
{
    while(!kthread_should_stop()) {
        pr_info("Thread Function %d",count);
	count++;
        msleep(1000);
    }
    return 0;
}

static int __init my_driver_init(void)
{
        my_thread = kthread_run(thread_function,NULL,"Thread");
        if(my_thread) {
            pr_info("Kthread Created Successfully...\n");
	    pr_info("\n Status :: %d",my_thread->thread_info.status);
	} 
	else {
            pr_err("Cannot create kthread\n");
        }

	pr_info("Device Driver Insert...Done!!!\n");
        return 0;
}

static void __exit my_driver_exit(void)
{
        kthread_stop(my_thread);
        pr_info("Device Driver Remove...Done!!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khushal");




/*EOC*/

