/*
 * Copyright (C) 2005-2019 Darron Broad
 * All rights reserved.
 *
 * This file is part of Pickle Microchip PIC ICSP.
 *
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 *
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#ifndef _UTIL_H
#define _UTIL_H

int areyousure(const char *s);
const char *mystrcasestr(const char *, const char *);
off_t filesize(const char *);
off_t myfopen(FILE **, const char *, char *);
void myfclose(FILE **, char *);
uint8_t hex2nibble(const char);
uint8_t hex2byte(const char *);
char *rmcrlf(char *, int);
char *rmws(char *, int);

#endif /* !_UTIL_H */
