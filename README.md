## OpenWrt Feeds

This Git contains an OpenWrt feed. The main purpose is an in system low-voltage programmer for PIC18F Microcontroller.

The mc-icsp module was written by Eran Duchan http://www.pavius.net/2011/06/lpicp-the-embedded-linux-pic-programmer/ -
I just added a GPIO customizing module (mc-icsp-gpio-custom).

### OpenWrt Compiling

To use this feed just edit your feeds.conf or feeds.conf.default in the OpenWrt root dir:

<pre><code>src-git openwrtfiles git://github.com/GBert/openwrt-files</pre></code>
and do
<pre><code>script/feeds update
script/feeds install kmod-mc-icp kmod-mc-icp-gpio-custom</pre></code>
in your OpenWrt source tree.

### Using
On the OpenWrt Router just install both modules:
<pre><code>insmod mc-icsp
insmod mc-icsp-gpio-custom mc\_icsp=14,12,11,13</pre></code>
and add the device:
<pre><code>mknod /dev/icsp0 c 245 0</pre></code>
### Test

<pre><code>root@OpenWrt-MC-ICSP:~# lpicp -x devid -d /dev/icsp0
Found device (PIC18F2585)
Device ID: E7.0E
Done successfully in 0.000s</pre></code>

### Todo

The basic infrastructure to programm some PIC18F is working fine and quite fast, but
there are lots of todos:

* module provides only some basic commands (4 bit command - 16 bit data) - should be be able
  to programm all sorts of modern PICs
* timings aren't handled correctly
* the programmer itself isn't ready (please have a look at: https://github.com/pavius/lpicp)
* rmmod mc\_icsp\_gpio\_custom doesn't work properly

### Thanks

Many hanks to Eran Duchan for his help.
