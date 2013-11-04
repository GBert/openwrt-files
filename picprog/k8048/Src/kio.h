/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _KIO_H
#define _KIO_H

#include "k8048.h"

/*
 * Prototypes
 */
void usage(struct k8048 *, char *, char *);
unsigned char getbytearg(struct k8048 *, char *, char *);
unsigned short getshortarg(struct k8048 *, char *, char *);
unsigned long get24arg(struct k8048 *, char *, char *);
unsigned long getlongarg(struct k8048 *, char *, char *);
unsigned char getportarg(struct k8048 *, char *, char *);
int main(int, char **);

#endif /* !_KIO_H */
