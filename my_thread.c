
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
static struct task_struct *my_thread1;

struct mutex my_mutex;

int thread_function(void *pv);
int thread_function1(void *pv);

int thread_function(void *pv)
{
      
    while(!kthread_should_stop()) {
	mutex_lock(&my_mutex);
	count++;
        pr_info(" Thread 1 Function %d",count);
	mutex_unlock(&my_mutex);
       msleep(1000);
    }
    return 0;
}

int thread_function1(void *pv)
{
      
    while(!kthread_should_stop()) {
        mutex_lock(&my_mutex);
	count++;
        pr_info(" Thread 2 Function %d",count);
	mutex_unlock(&my_mutex);
       msleep(1000);
    }
    return 0;
}

static int __init my_driver_init(void)
{
        my_thread = kthread_run(thread_function,NULL," Thread");  //Gave same name to threads for testing
        if(my_thread) {
            pr_info("Kthread Created Successfully...\n");
	    pr_info("\n ID of thread1:: %d",my_thread->pid);  // Printing thread id
	} 
	else {
            pr_err("Cannot create kthread\n");
        }

	my_thread1 = kthread_run(thread_function1,NULL," Thread");  //Gave same name to threads for testing
        if(my_thread) {
            pr_info("Kthread1 Created Successfully...\n");
	    pr_info("\n ID of thread2:: %d",my_thread1->pid);  // Printing thread id
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
        kthread_stop(my_thread1);
        pr_info("Device Driver Remove...Done!!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
