<?php include_once("_header.php"); ?>

<h1>rpldev - tty logger kernel module</h1>

<p class="block"><code>/dev/rpl</code> is a character device file which returns
the data captured from ttys through the <i>rpldev</i> kernel module. It has no
fixed device number, since it asks for a dynamic major/minor number. Usually,
it will get <code>char(10,63)</code> or <code>char(10,62)</code> on
Linux&nbsp;-- the Kernel module will emit a notice where it actually landed.
The kernel hotplug framework and <i>udev</i> will then create a device node in
<code>/dev</code>.</p>

<p class="block">On FreeBSD, devfs takes care of all this, so when the module
is loaded, a working <code>/dev/rpl</code> is also present. For OpenBSD and
NetBSD, it always loads at <code>char(228,0)</code>. Use <code>mknod /dev/rpl c
228 0</code> to create the node. It should be made mode <code>0400</code> and
owned by the low-privileged user that is set in <code>rpld.conf</code>.</p>

<h1>Module parameters (Linux)</h1>

<p class="block">The <i>rpldev</i> module has the following options. Some of
them may be changed dynamically at run-time via
<code>/sys/module/rpldev/</code>.</p>

<table border="1">
  <tr>
    <td class="t1">Bufsize</td>
    <td class="t1">Size of the ring buffer in bytes. Default is 32K.</td>
  </tr>
  <tr>
    <td class="t2">Minor_nr</td>
    <td class="t2">Force a minor number rather than automatically deciding for
      one.  Passing 255 here will use auto-selection, and is the default.</td>
  </tr>
</table>

<p class="block">These options can be provided to <i>modprobe</i> like
this:</p>

<p class="code"><code><b>#</b> modprobe rpldev Minor_nr=37</code></p>

<p class="block">You can put the options into
<code>/etc/modprobe.conf.local</code>, to be automatically sourced when
modprobe is called:</p>

<p class="code"><code>options rpldev Minor_nr=37</code></p>

<p class="block">I have not found any way for passing or modifying options
under BSD, so unless you are willing to recompile the kernel, you are stuck
with a fixed 32K buffer.</p>

<h1>Data structures</h1>

<p class="block">Everytime the kernel functions for reading/writing from/to a
terminal are executed, the call chain is extened into the rpldev hooks, i.e. an
external function in the <i>rpldev</i> kernel module is called, which transfers
the tty buffer into so-called packets, which are then returned as
<code>/dev/rpl</code> is read.</p>

<p class="block">A packet consists of a device number, magic byte (always
<code>0xEE</code>), size of the data, timestamp, and of course, the data
itself. All fields are little-endian and packed, i.e. there are no alignment
gaps. It can be represented in this C struct:</p>

<p class="code"><code>
<b>struct</b> rpltime {<br />
&nbsp; &nbsp; <b>uint64_t</b> tv_sec;<br />
&nbsp; &nbsp; <b>uint32_t</b> tv_usec;<br />
};<br />
<br />
<b>struct</b> rpldev_packet {<br />
&nbsp; &nbsp; <b>uint32_t</b> dev;<br />
&nbsp; &nbsp; <b>uint16_t</b> size;<br />
&nbsp; &nbsp; <b>uint8_t</b> event, magic;<br />
&nbsp; &nbsp; <b>struct</b> rpltime time;<br />
} <i>__attribute__((packed))</i>;</code></p>

<p class="block">The data itself is actually not included in the struct, since
it is of dynamic length. For further reference, the struct alone will be called
<i>packet header</i>, whereas packet header plus <i>data</i> (or
<i>payload</i>) equals <i>packet</i>. There are no byte-aligning gaps between
the members, i.e. it is a so-called <i>packed</i> structure. (No pun between
packet and packed.)</p>

<p class="block">The <code>-&gt;dev</code> member contains the device number on
which the event occurred. Since both the master and the slave side of a pty
pair can generate events, and we usually do not need the master side events,
packets from the master sides are already dropped at the kernel level.</p>

<p class="block">The device field is made up of 12 bits for the major number
and 20 bits for the minor number, in little-endian, and has this very same
layout on all rpldev architectures. (For reference, this 12/20 split comes from
Linux 2.6.) BSD rpldev will convert device numbers to this scheme. As a result,
a truncation is involved, since BSD uses an 8/24 split, so minors greater than
1048575 will be truncated. This should not pose a problem, as one million
devices is already more than a system could ever have in a meaningful way.</p>

<p class="block">The following event types exist:</p>

<p class="code"><code>
<b>enum</b> {<br />
&nbsp; &nbsp; EVT_OPEN <b>=</b> 0x01,<br />
&nbsp; &nbsp; EVT_READ <b>=</b> 0x02,<br />
&nbsp; &nbsp; EVT_WRITE <b>=</b> 0x03,<br />
&nbsp; &nbsp; EVT_LCLOSE <b>=</b> 0x64,<br />
};</code></p>

<p class="block"><code>EVT_OPEN</code>, <code>EVT_READ</code> and
<code>EVT_WRITE</code> are generated whenever an <code>open()</code>,
<code>read()</code> or <code>write()</code> is done, respectively.
<code>EVT_LCLOSE</code> is generated when the last file descriptor to the tty
is closed.</p>

<p class="block">During an <code>open()</code> on a tty line (Linux only), the
filename of the accessed dentry is extracted and passed to <i>rpld</i> within
the payload, for convenience, so that <i>rpld</i> does not need to do a
time-consuming search for it in <code>/dev</code>.</p>

<?php include_once("_footer.php"); ?>
