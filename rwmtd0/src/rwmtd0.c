#include <linux/init.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>

int w2uboot_init(void) {
    struct mtd_info *mtd;
    
    mtd = get_mtd_device_nm("u-boot");
    mtd->flags |= MTD_WRITEABLE;
    
    printk(KERN_INFO "unlock u-boot, now it is writeable\n");
    return 0;
}

void w2uboot_exit(void)
{
}

module_init(w2uboot_init);
module_exit(w2uboot_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Qin Wei");
