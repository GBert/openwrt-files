## OpenWrt Feeds

This Git contains an OpenWrt feed. The main purpose is an in system low-voltage programmer for PIC18F Microcontroller.

The mc-icsp module was written by Eran Duchan http://www.pavius.net/2011/06/lpicp-the-embedded-linux-pic-programmer/ -
I just added a GPIO customizing module (mc-icsp-gpio-custom).

Use the picprog-new instead of picprog (deprecated).

### OpenWrt Compiling

To use this feed just edit your feeds.conf or feeds.conf.default in the OpenWrt root dir:

<pre><code>src-git openwrtfiles git://github.com/GBert/openwrt-files</pre></code>
and do
<pre><code>scripts/feeds update
scripts/feeds install kmod-mc-icsp kmod-mc-icsp-gpio-custom picprog-new</pre></code>
in your OpenWrt source tree.

### Using
On the OpenWrt Router just install both modules:
<pre><code>insmod mc-icsp
insmod mc-icsp-gpio-custom mc\_icsp=14,12,11,13 P2B=200 P2A=0</pre></code>
and add the device:
<pre><code>mknod /dev/icsp0 c 245 0</pre></code>
### Test

<pre><code>root@OpenWrt-MC-ICSP:~# cat /sys/kernel/debug/gpio
GPIOs 0-23, platform/10000600.gpio, 10000600.gpio:
 gpio-11  (MC ICSP PGC         ) out lo
 gpio-12  (MC ICSP PGD         ) out lo
 gpio-13  (MC ICSP PGM         ) out lo
 gpio-14  (MC ICSP MCLR        ) out hi
root@OpenWrt-MC-ICSP:~# ls -l /usr/bin/k\*
lrwxrwxrwx    1 root     root            14 Oct 10 13:26 /usr/bin/k16 -> /usr/bin/k8048
-rwxr-xr-x    1 root     root        198391 Oct 10 13:26 /usr/bin/k8048
root@OpenWrt-MC-ICSP:~# k16 info
[000000] [PROGRAM]     6000 WORDS
[200000] [IDLOCATION1] FF
[200001] [IDLOCATION2] FF
[200002] [IDLOCATION3] FF
[200003] [IDLOCATION4] FF
[200004] [IDLOCATION5] FF
[200005] [IDLOCATION6] FF
[200006] [IDLOCATION7] FF
[200007] [IDLOCATION8] FF
[300000] [CONFIG1]     0200
[300002] [CONFIG2]     1E1F
[300004] [CONFIG3]     8200
[300006] [CONFIG4]     0085
[300008] [CONFIG5]     C00F
[30000A] [CONFIG6]     E00F
[30000C] [CONFIG7]     400F
[3FFFFE] [DEVICEID]    0EE7 DEV:EE0 (E:7) REV:7 PIC18F2585
[F00000] [DATA]        0400 BYTES
root@OpenWrt-MC-ICSP:~# k16 blank
Blank device: Are you sure [y/N]? y
root@OpenWrt-MC-ICSP:~# k16 program 18f2585\_blink.hex 
Total: 120
root@OpenWrt-MC-ICSP:~# k16 verify 18f2585\_blink.hex 
Total: 106 Pass: 106 Fail: 0
</pre></code>

### Todo

The basic infrastructure to programm some PIC18F is working fine and quite fast, but
there are lots of todos:

* module provides only some basic commands (4 bit command - 16 bit data) - should be be able
  to programm all sorts of modern PICs
* timings aren't handled correctly -> improvements under way
* rmmod mc\_icsp\_gpio\_custom doesn't work properly - improved, but still GPIO/pinctrl issues

### Thanks

Many thanks to Eran Duchan and Darron Broad for their help.
