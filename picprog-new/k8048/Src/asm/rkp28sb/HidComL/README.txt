Origin:
	http://openprog.altervista.org/USB_firm_eng.html

About:
	HidCom, an example of communication between a PC and an HID device.
	For more information see http://openprog.altervista.org/
	These sources are given free of charge under the GNU General Public License version 2 

	How to compile:
	gcc HidCOm.cpp -o HidCom

	Example:
	HidCOm --pid 4D8 --vid 1FF --info 0 1 2 3 12 15 1a 1c 1f 4

Changes:
	Added Makefile

	Used INDENT(1) to tidy
