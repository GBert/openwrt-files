/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _UTIL_H
#define _UTIL_H

int areyousure(const char *s);
const char *mystrcasestr(const char *, const char *);
char *getarg(const char *);

#endif /* !_UTIL_H */
