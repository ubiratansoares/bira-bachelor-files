/********************************************************************
 * File: scancodes.h
 * Author: Patrick Carroll <pcarrol1@umbc.edu>
 * Date: November 2007
 * CMSC 426 Semester Project
 *
 * This file contains prototypes for functions for translating 
 * keycodes read from the PS/2 port into their corresponding 
 * keyboard character.
 ********************************************************************/
#ifndef __SCANCODES_H
#define __SCANCODES_H

void output_scancode(FILE *out, unsigned char c, unsigned int shift_pressed);

#endif

