<?php include_once("_header.php"); ?>

<h1>Description</h1>

<p class="block"><i>ttyrpld</i> is a multi-os kernel-level tty logger (key- and
screenlogger for ttys) with (a)synchronous replay supprt. It supports most tty
types, including <i>vc</i>, bsd and unix98-style <i>pty</i>s (xterm/ssh),
<i>serial</i>, <i>isdn</i>, etc. Being implemented within the kernel makes it
unavoidable for the default user. It runs with no overhead if the logging
daemon is not active.</p>

<h1>Technical aspects</h1>

<p><i>... which cause it to differ from existing solutions:</i></p>

<p class="block">Most other solutions deploy the logging completely in
userspace and make it dependent on the user to activate the logging.
Definitely, an intruder does not start e.g. <code>/usr/bin/script</code>
voluntarily. I have only seen few other kernel-based loggers. One of them is
outdated, others query the x86 keyboard driver using interrupt hijacking and
fixed translation. This is very unportable, because it only catches raw AT
keyboard scancodes, but not USB or even other keyboards (unless they are in
compatibility mode), and it does not work at all for network traffic.</p>

<p class="block">Only the FreeBSD snooper <i>watch</i>, which operates on the
<code>/dev/snp*</code> devices, can be taken for a comparison. However, it can
only do the interactive live feed mode, no logging is possible for later
replay. Packet time stamps are also missing.</p>

<h1>Components</h1>

<p class="block">This kit (<i>ttyrpld</i>) consists of four components:</p>

<p class="block"><i>kpatch</i>: The kernel patch adds a few lines to provide
the <i>rpldev</i> extension hooks, which (any) module can then get onto.</p>

<p class="block"><i>rpldev</i>: The kernel module is responsible for grabbing
the data off the tty line and providing a character device for the user-space
logging daemon. Data grabbed of the tty is directly passed to the overlying
daemons, so with the correct terminal settings you can get a 1:1 replay.</p>

<p class="block">For systems where module loading is not possible or not widely
supported (OpenBSD for example), <i>rpldev</i> is integrated into the
kpatch.</p>

<p class="block"><i>rpld</i>: Having received the captured data, the logging
daemon can store them in any format and/or facility, with or without
compression, just as it likes, for this happens in user-space and thus you have
all the fluffy libraries available. (That would not be the case from kernel
space.)</p>

<p class="block"><i>ttyreplay</i>: real-time log analyzer. Think of it as a
simple video player.</p>

<h1>Misc info</h1>

<table border="1">
  <tr>
    <td class="t1">License:</td>
    <td class="t1"><ul>
      <li>GNU Lesser General Public License (LGPL) version 2.x (userspace
        programs)</li>
      <li>GNU General Public License (GPL) version 2.x (Linux kernel
        parts)</li>
      <li>BSD-style (BSD kernel parts)</li>
      <li>CDDL (Solaris kernel parts)</li>
    </ul></td>
  </tr>
  <tr>
    <td class="t2">Localization for:</td>
    <td class="t2">English, Dutch, French, German, Italian, Norwegian-Bokmål,
      Persian (Farsi), Spanish, Swedish</td>
  </tr>
  <tr>
    <td class="t1">Operating Systems:</td>
    <td class="t1">Linux 2.6, FreeBSD 5.3/6.0/6.1, OpenBSD 3.9,
      NetBSD 3.0, Solaris 11_SNV39(dev)<br />
      (and possibly derivates)</td>
  </tr>
</table>

<?php include_once("_footer.php"); ?>
