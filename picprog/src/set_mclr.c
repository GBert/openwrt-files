/*
 * set MCLR to 0 or 1
 */

#include "lpicp_icsp.h"
#include "k8048.h"

int main(int argc, char **argv) {
	int ret;
	int icsp_dev_file;
	if (argc < 2) {
		printf ("Usage: %s [0,1]\n",argv[0]);
		return(1);
	}

	icsp_dev_file = open("/dev/icsp0", O_RDWR);
	if (icsp_dev_file <= 0) {
		printf("Failed to open ICSP device /dev/icsp0\n");
	}

	switch (argv[1][0]) {
	case '1':
		ret = (ioctl(icsp_dev_file, MC_ICSP_IOC_MCLR_HIGH) == 0);		
		break;

	case '0':	
		ret = (ioctl(icsp_dev_file, MC_ICSP_IOC_MCLR_LOW) == 0);
		break;
	default: printf("Usegae: %s [0,1]", argv[0]);
	}

	return(0);
}
