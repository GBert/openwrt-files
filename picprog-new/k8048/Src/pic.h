/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _PIC_H
#define _PIC_H

#define DATAWIDTH (16)
#define CODEWIDTH (8)

#define PIC_NCOLS (5)
int pic_cmp(const void *, const void *);
void pic_selector(struct k8048 *);

void pic_read_config(struct k8048 *, int);

int pic_get_program_flash_size(struct k8048 *);
int pic_get_data_flash_size(struct k8048 *);
int pic_get_data_eeprom_size(struct k8048 *);
int pic_read_flash_memory_block(struct k8048 *, unsigned short *, int);
int pic_read_eeprom_memory_block(struct k8048 *, unsigned char *, int);

void pic_program(struct k8048 *, const char *, int);
int pic_verify(struct k8048 *, const char *);
void pic_blank(struct k8048 *);
void pic_erase(struct k8048 *, int);
void pic_dumpdeviceid(struct k8048 *);
void pic_dumpconfig(struct k8048 *);
void pic_writeconfig(struct k8048 *, unsigned short);
void pic_writebandgap(struct k8048 *k, unsigned short);
void pic_dumposccal(struct k8048 *);
void pic_writeosccal(struct k8048 *, unsigned short);

void pic_dumpbyte(struct k8048 *k, unsigned short, unsigned char);
void pic_dumpword(struct k8048 *k, unsigned short, unsigned short);
void pic_dumpdevice(struct k8048 *);

void pic_dumpflash(struct k8048 *, int);
void pic_dumpeeprom(struct k8048 *);

void pic_dump_program_flash(struct k8048 *, int, int, int);
void pic_dump_data_eeprom(struct k8048 *, int, int);

#endif /* !_PIC_H */
