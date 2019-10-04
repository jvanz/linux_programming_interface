// https://kernelnewbies.org/Simple_UDP_Server
// https://github.com/esposem/Kernel_UDP
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/types.h>

#define MODULE_NAME "testudpserver"
#define DEFAULT_PORT 8088
#define BUFFER_SIZE 128

struct mythread_t 
{
	struct task_struct *thread;
	struct socket *sock;
	struct sockaddr_in addr;
};

struct mythread_t *kthread = NULL;

static void server(void) 
{
	int err, size;
	unsigned char buf[BUFFER_SIZE+1];
	struct msghdr msg;
	struct kvec vec;

	// create a socket
	if ((err = sock_create(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &kthread->sock)) < 0) {
		printk(KERN_INFO "%s: Cannot create socket.\n", MODULE_NAME);
		goto out;
	}
	memset(&kthread->addr, 0, sizeof(struct sockaddr));
	kthread->addr.sin_family = AF_INET;
	kthread->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	kthread->addr.sin_port = htons(DEFAULT_PORT);

	if ((err = kthread->sock->ops->bind(kthread->sock, 
		(struct sockaddr *)&kthread->addr, sizeof(struct sockaddr))) < 0) {
		printk(KERN_INFO "%s: Cannot bind socket.\n", MODULE_NAME);
		goto close_and_out;
	}
	printk(KERN_INFO "%s: listening on port %d.\n", MODULE_NAME, DEFAULT_PORT);

	for(;;) {
		memset(&buf, 0, BUFFER_SIZE+1);
		vec.iov_len = BUFFER_SIZE;
		vec.iov_base = buf;
		size = kernel_recvmsg(kthread->sock, &msg, &vec, 1, BUFFER_SIZE, MSG_WAITALL);
		if (size < 0) {
			printk(KERN_INFO "%s: error getting datagram, sock_recvmsg error = %d.\n", MODULE_NAME, size);
		} else {
			printk(KERN_INFO "%s: received %d bytes.\n", MODULE_NAME, size);
			printk(KERN_INFO "%s: data %s.\n", MODULE_NAME, buf);
			
		}
	}
close_and_out:
        sock_release(kthread->sock);
        kthread->sock = NULL;

out:
        kthread->thread = NULL;

}

int __init udpserver_init(void)
{
	//start kthread
	kthread = kmalloc(sizeof(struct mythread_t), GFP_KERNEL);
	memset(kthread, 0, sizeof(struct mythread_t));

	kthread->thread = kthread_run((void*)server, NULL, MODULE_NAME);
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
