/********************************************************************
 * File: keyboard_port.h
 * Author: Patrick Carroll <pcarrol1@umbc.edu>
 * Date: November 2007
 * CMSC 426 Semester Project
 *
 * This file contains prototypes for the functions necessary for 
 * reading characters from the PS/2 port.
 ********************************************************************/
#ifndef __KEYBOARD_PORT_H
#define __KEYBOARD_PORT_H

#include <stdio.h>

void set_up_permissions(void);

unsigned char get_character_from_port(FILE *out);

#endif

