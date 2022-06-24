## OpenWrt Feeds

This Git contains an OpenWrt feed. The main purpose is an in system low-voltage programmer for PIC Microcontroller.

The mc-icsp module was written by Eran Duchan http://www.pavius.net/2011/06/lpicp-the-embedded-linux-pic-programmer/ -
I just added a GPIO customizing module (mc-icsp-gpio-custom).

Darron Broad was so kind to adapt his k8048 programmer code and greatly optimzed the GPIO bit-banging module. He is still
active and add new PICs. As of today all Microchip PICs (8, 16, and 32 bit) with LVP capability should be supported.

### OpenWrt Compiling

To use this feed just edit your feeds.conf or feeds.conf.default in the OpenWrt root dir:

<pre><code>src-git openwrtfiles git://github.com/GBert/openwrt-files</pre></code>
and do
<pre><code>scripts/feeds update
scripts/feeds install picprog-new</pre></code>
in your OpenWrt source tree. This will automatically add the kmod-gpio-bb module.
Mark 'picprog-new' under 'Utilities' to compile and install it.

### Using
On the OpenWrt Router install the module:
<pre><code>modprobe gpio-bb && mknod /dev/gpio-bb c 180 0</pre></code>
Please apply your GPIO settings in /root/.k8048/config .
### Test
<pre><code>root@OpenWrt:~# p16 lvp info
[000000] [PROGRAM]     8000 WORDS (0400 ROWS OF 0020 WORDS)
[200000] [IDLOCATION1] FF .
[200001] [IDLOCATION2] FF .
[200002] [IDLOCATION3] FF .
[200003] [IDLOCATION4] FF .
[200004] [IDLOCATION5] FF .
[200005] [IDLOCATION6] FF .
[200006] [IDLOCATION7] FF .
[200007] [IDLOCATION8] FF .
[300000] [CONFIG1]     125D
[300002] [CONFIG2]     7E7F
[300004] [CONFIG3]     8900
[300006] [CONFIG4]     0091
[300008] [CONFIG5]     C00F
[30000A] [CONFIG6]     E00F
[30000C] [CONFIG7]     400F
[3FFFFE] [DEVICEID]    6123 DEV:309 REV:03 PIC18F26K80
[F00000] [DATA]	       0400 BYTES
root@OpenWrt:~# # blanking the device is not needed before progamming - just to see if its working
root@OpenWrt:~# p16 lvp blank
Blank device: Are you sure [y/N]? y
root@OpenWrt:~# p16 lvp programm 18f26k80\_blink.hex 
Total: 132
root@OpenWrt:~# p16 lvp verify 18f26k80\_blink.hex 
Total: 118 Pass: 118 Fail: 0
root@OpenWrt:~# time k16 lvp flash
[000000] 0E8F 14D3 0970 6ED3 8C9B 010F 6B5D 6B5C ....p..n....]k\k
[000010] 6AC2 6AC1 6AC0 6B5F 6B5E 9092 8089 0100 .j.j.j\_k^k......
[000020] 0E9C 6E01 0EDD 6E02 0E25 6E03 2E03 EF16 ...n...n%..n....
[000030] F000 2E02 EF14 F000 2E01 EF12 F000 0000 ................
[000040] 9089 0100 0E9C 6E01 0EDD 6E02 0E25 6E03 .......n...n%..n
[000050] 2E03 EF28 F000 2E02 EF26 F000 2E01 EF24 ..(.....&.....$.
[000060] F000 0000 EF0E F000 FFFF FFFF FFFF FFFF ................
real	0m 0.96s
user	0m 0.07s
sys	0m 0.87s
</pre></code>

### Todo

Darron Broad is doing a very good job getting more and more PICs integrated. Also the programming speed is impressive.
PIC32 is also supported now and even the new MZ family (tests pending).
 
### Thanks

Many thanks to Eran Duchan and Darron Broad for their help.
