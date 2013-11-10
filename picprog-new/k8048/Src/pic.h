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

#define PICLF /* SUPPORT PIC LF */
#define MCHPKEY (0x4D434850) /* ENHANCED MID-RANGE    = MCHP (0x4D434850)          */
#define PHCMKEY (0x0A12C2B2) /* PIC18/PIC24 EXECUTIVE = MCHP (0x4D434850) REVERSED */
#define QHCMKEY (0x8A12C2B2) /* PIC24                 = MCHQ (0x4D434851) REVERSED */

void pic_x(struct k8048 *); /* DEBUGGING */

int pic_cmp(const void *, const void *);
#define PIC_NCOLS (5)
void pic_selector(struct k8048 *);

#define PIC_CONFIG_ONLY (0)
#define PIC_CONFIG_ALL (1)
void pic_read_config(struct k8048 *, int);

uint32_t pic_get_program_flash_size(struct k8048 *, uint32_t *);
uint32_t pic_get_data_flash_size(struct k8048 *, uint32_t *);
uint32_t pic_get_data_eeprom_size(struct k8048 *, uint32_t *);
uint32_t pic_get_executive_size(struct k8048 *, uint32_t *);

uint32_t pic_read_flash_memory_block(struct k8048 *, uint32_t *, uint32_t, uint32_t);
uint32_t pic_read_eeprom_memory_block(struct k8048 *, uint8_t *, uint32_t, uint16_t);

#define PIC_VOID (0xFFFF)
void pic_program(struct k8048 *, char *, int);
uint32_t pic_verify(struct k8048 *, char *);

#define PIC_INTERNAL (0xFFFF)
#define PIC_NOINTERNAL (0xFFFE)
void pic_writebandgap(struct k8048 *k, uint16_t);
void pic_writeosccal(struct k8048 *, uint16_t);
void pic_blank(struct k8048 *);

#define PIC_ERASE_ID (UINT32_MAX)
#define PIC_ERASE_CONFIG (PIC_ERASE_ID - 1)
#define PIC_ERASE_EEPROM (PIC_ERASE_ID - 2)
void pic_erase(struct k8048 *, uint32_t, uint32_t);

void pic_dumpdeviceid(struct k8048 *);

#define PIC_BRIEF   (0)
#define PIC_VERBOSE (1)
void pic_dumpconfig(struct k8048 *);
void pic_dumposccal(struct k8048 *);

void pic_dumpaddr(uint32_t, int);
void pic_dumpbyte(uint32_t, uint8_t);
void pic_dumpword(uint32_t, uint16_t);

void pic_dumpdevice(struct k8048 *);

void pic_dumpflash(struct k8048 *, uint32_t);
void pic_dumpeeprom(struct k8048 *);
void pic_dumpexec(struct k8048 *);

#define PIC_HEXDEC (1)
#define PIC_INHX32 (2)
#define PIC_CHAR(X) ((((X) >= ' ' && (X) < 127) ? (X) : ('.')))
void pic_dump_program_flash(struct k8048 *, uint32_t, uint32_t, int);
int pic_mtwords(uint32_t, uint32_t, uint32_t *);
void pic_dumphexwords(struct k8048 *, uint32_t, uint32_t, uint32_t *);
void pic_dumpinhxwords(struct k8048 *, uint32_t, uint32_t, uint32_t *);
void pic_dump_data_eeprom(struct k8048 *, uint32_t, uint32_t, int);
int pic_mtbytes(uint32_t, uint32_t, uint8_t *);
void pic_dumphexbytes(struct k8048 *, uint32_t, uint32_t, uint8_t *);
void pic_dumpinhxbytes(struct k8048 *, uint32_t, uint32_t, uint8_t *);

#endif /* !_PIC_H */
