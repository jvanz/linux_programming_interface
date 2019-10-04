#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/types.h>

#define MODULE_NAME "testudpserver"

struct mythread_t 
{
	struct task_struct *thread;
};

struct mythread_t *kthread = NULL;

static void server_start(void) 
{
	while(1){}
}


int __init udpserver_init(void)
{
	//start kthread
	kthread = kmalloc(sizeof(struct mythread_t), GFP_KERNEL);
	memset(kthread, 0, sizeof(struct mythread_t));

	kthread->thread = kthread_run((void*)server_start, NULL, MODULE_NAME);
	if (IS_ERR(kthread->thread)) {
		printk(KERN_INFO "%s: cannot start kthread\n", MODULE_NAME);
		kfree(kthread);
		kthread = NULL;
		return -ENOMEM;
	}
	printk(KERN_INFO "%s module initialized!\n", MODULE_NAME);
	return 0;
}

void __exit udpserver_exit(void)
{
	if (kthread->thread == NULL) {
		printk(KERN_INFO "%s: no kernel thread\n", MODULE_NAME);
	} else {
		int ret;
		if ((ret = kthread_stop(kthread->thread)) == 0){
			printk(KERN_INFO "%s kernel thread terminated.\n", MODULE_NAME);
		} else {
			printk(KERN_INFO "%s cannot terminate kernel thread.\n", MODULE_NAME);
		}
	}
	printk(KERN_INFO "%s module exited!\n", MODULE_NAME);
}

/* init and cleanup functions */
module_init(udpserver_init);
module_exit(udpserver_exit);

/* module information */
MODULE_DESCRIPTION("Sample UDP server");
MODULE_AUTHOR("José Guilherme Vanz <jvanz@jvanz.com>");
MODULE_LICENSE("GPL");
