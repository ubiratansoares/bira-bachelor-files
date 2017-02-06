/********************************************************************
 * File: scancodes.c
 * Author: Patrick Carroll <pcarrol1@umbc.edu>
 * Date: November 2007
 * CMSC 426 Semester Project
 *
 * This file contains functions for translating keycodes read from the
 * PS/2 port into their corresponding keyboard character.
 ********************************************************************/
#include <stdio.h>


/* each scancode corresponds to a key.  i'll use an array as a way to
 * index the strings.  I figured these out by printing the output of each
 * keypress and figuring out what letter it should be */
static char *keys[89] = { " ", " ", "1", "2", "3", "4", "5", "6", "7", "8", "9",
      	 	"0", "-", "=", "<Backspace>", "<Tab>", "q", "w", "e", "r", "t",
	       	"y", "u", "i", "o", "p", "[", "]", "<Return>", "<Ctrl>", "a", 
		"s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", 
		"<L-Shift>", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
	       	"/", "<R-Shift>", "*", "<Alt>", " ", "<Caps-Lock>", "<F1>",
		"<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>",
		"<F10>", "<Num-Lock>", "<Scroll-Lock>", "7", "8", "9", "-", "4",
		"5", "6", "+", "1", "2", "3", "0", ".", " ", " ", " ", "<F11>",
		"<F12>" };

static char *shift_keys[89] = { " ", " ", "!", "@", "#", "$", "%", "^", "&", 
		"*", "(", ")", "_", "+", "<Backspace>", "<Tab>", "Q", "W", "E", 
		"R", "T", "Y", "U", "I", "O", "P", "{", "}", "<Return>", 
		"<Ctrl>", "A", "S", "D", "F", "G", "H", "J", "K", "L", ":",
		"\"", "~", "<L-Shift>", "|", "Z", "X", "C", "V", "B", "N", "M",
		"<", ">", "?", "<R-Shift>", "*", "<Alt>", " ", "<Caps-Lock>", 
		"<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>",
	       	"<F9>", "<F10>", "<Num-Lock>", "<Scroll-Lock>", "7", "8", "9", 
		"-", "4", "5", "6", "+", "1", "2", "3", "0", ".", " ", " ", 
		" ", "<F11>", "<F12>" };


void output_scancode(FILE *out, unsigned int i, unsigned int shift_pressed)
{
	if (i < 90) {
		fprintf(out, "%s", (shift_pressed ? shift_keys[i] : keys[i]));
		fflush(out);
	}
}

