/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _DOTCONF_H
#define _DOTCONF_H

#define DOTFILENAME ".k8048"

void getdotfile(struct k8048 *);
void getconf(struct k8048 *, char *);

#endif /* !_DOTCONF_H */
