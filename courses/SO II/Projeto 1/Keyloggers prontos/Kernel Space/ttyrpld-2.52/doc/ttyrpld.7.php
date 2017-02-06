<?php include_once("_header.php"); ?>

<p><b>ttyrpld architecture</b></p>

<h1>First stage - kernel patch</h1>

<p class="block">Different keyloggers take different ways. <i>Linspy</i> (see
Phrack 50, File 5) modifies the syscall table to provide its own functions,
<i>vlogger</i> (Phrack 59, File 0x0E) chooses to intercept the
<code>receive_buf()</code> function. <i>ttyrpld</i> instead hooks in at the top
of the tty layer level.</p>

<p class="block">The first step in capturing the data off a tty is directly in
the kernel, within <code>drivers/char/tty_io.c</code>. It is easier than it
sounds, actually, I was pretty much driven by the two lines of UML code, as
stated above. I did not take any further action within that file, but merely
duplicated what UML already had in, yet more flexible.</p>

<p class="block">Seven variable function pointers&nbsp;--
<code>rpl_init</code>, <code>rpl_open</code>, <code>rpl_read</code>,
<code>rpl_write</code>, <code>rpl_close</code>, <code>rpl_deinit</code> and
<code>rpl_ioctl</code>&nbsp;-- are exported from <code>tty_io.c</code> for
module hook-up. That means that a module has to redirect them to its own
functions, when it shall go into action. That way, there is no overhead (read:
(usually) 2 CPU instructions) when rpldev logging is not active.</p>

<p class="block"><i>Excerpt from <code>drivers/char/tty_io.c</code></i></p>
<p class="code"><code>
<b>#</b>ifdef CONFIG_RPLDHK<br />
<b>#</b> &nbsp; &nbsp;include &lt;linux/km_rpldev.h&gt;<br />
<b>int (*</b>rpl_open<b>)</b>(<b>struct</b> tty_struct <b>*</b>, <b>struct</b> tty_struct <b>*</b>);<br />
<b>int (*</b>rpl_read<b>)</b>(<b>const char __user *</b>, <b>size_t</b>, <b>struct</b> tty_struct <b>*</b>);<br />
<b>int (*</b>rpl_write<b>)</b>(<b>const char __user *</b>, <b>size_t</b>, <b>struct</b> tty_struct <b>*</b>);<br />
<b>int (*</b>rpl_lclose<b>)</b>(<b>struct</b> tty_struct <b>*</b>, <b>struct</b> tty_struct <b>*</b>);<br />
<b>#</b>endif</code></p>

<p class="code"><code>
<b>#</b>ifdef CONFIG_RPLDHK<br />
<b>if</b> (rpl_write <b>!=</b> NULL)<br />
&nbsp; &nbsp; rpl_write(buf, ret, tty);<br />
<b>#</b>endif</code></p>

<p class="block"><code>tty_write()</code>, etc. is the spot to take, because
this is directly below before the tty buffers spread into their disciplines.
That way, any tty (teletype) is logged, <code>vc</code> (virtual console, aka
Ctrl+Alt+F1), <code>pts</code> (Unix98 pty, used e.g. with SSH),
<code>ttyp</code> (BSD ptys, rarely used anymore), <code>ttyS</code> (serial),
and if someone is going to try it, even ISDN lines like <code>ttyI</code> may
show up.</p>

<h1>Second stage - kernel module</h1>

<p class="block">The 2nd stage consists of the functions behind the
<code>rpl_*</code> variables, the functions as present in the module. They copy
the data "captured" (let's say it was passed voluntarily) by the tty driver to
a buffer, so that <code>rpl_open()</code> can return as soon as possible to not
block the tty driver.</p>

<p class="block">This can get a little problematic if you have a lot of
<i>simultaneous</i> users entering or producing a lot of text. I am not out to
the disk's speed and/or the log size, but the module buffer size where the tty
data is temporarily copied to before it is passed on. There is only one global
ring buffer for all ttys, and I do not see the use of one buffer per tty as
useful, for this would raise the memory and CPU overhead. Overall, the whole
RPL structure speaks a compact binary protocol to minize overhead of data
passing. See <a href="rpldev.4.php">rpldev(4)</a> for details on the
protocol.</p>

<p class="block"><i>Excerpt from <code>k_linux-2.6/rpldev.c</code></i></p>

<p class="code"><code>
<b>static int</b> rpldhk_write(<b>const char</b> __user <b>*</b>buf, <b>size_t</b> count, <b>struct</b> tty_struct <b>*</b>tty)<br />
{<br />
&nbsp; &nbsp; <b>struct</b> rpld_packet p;<br />
<br />
&nbsp; &nbsp; SKIP_PTM(tty);<br />
&nbsp; &nbsp; if (count == 0) return 0;<br />
<br />
&nbsp; &nbsp; p.dev &nbsp; = TTY_DEVNR(tty);<br />
&nbsp; &nbsp; p.size &nbsp;= cpu_to_le16(count);<br />
&nbsp; &nbsp; p.event = EVT_READ;<br />
&nbsp; &nbsp; p.magic = MAGIC_SIG;<br />
&nbsp; &nbsp; fill_time(&p.time);<br />
&nbsp; &nbsp; <b>return</b> circular_put_packet(&amp;p, buf, count);<br />
}</code></p>

<p class="block">The default buffer size of 32K has proven to be enough for a
small network. If it does not suffice, you can raise the buffer size. Thanks to
<i>rpldev</i> being a module, you can remove it and re-<code>modprobe</code>
with the corresponding parameter <code>Bufsize</code>.</p>

<p class="block">I have decided not to block processes trying to write to a
full ring buffer because that could create a real bad userspace deadlock.</p>

<h1>Third stage - userspace gateway</h1>

<p class="block">The module provides a character-based device node,
<code>/dev/rpl</code> (or <code>/dev/misc/rpl</code>) to read from. Device and
userspace logging daemon must use the same protocol for the data passed over
the channel.</p>

<p class="block">As far as <i>ttyrpld</i> is concerned, its kernel module
<i>rpldev</i> allocates the buffer and hooks up on the <code>rpl_*</code>
function pointers when the device is successfully <i>opened</i>, not when the
module is loaded. This saves us an unnecessary call to, say,
<code>kio_write()</code> if it would anyway just return because of an
<code>if()</code> condition for checking if the device is open. (That means, I
did not want to have a global variable indicating the device is opened and
having an <code>if()</code> within <code>kio_*()</code>.)</p>

<p class="block">Similar applies when closing the device, the memory is
released and the function hooks are set back. This saves memory and also CPU
time, because an early <code>if()</code> within <code>tty_io.c</code> will skip
everything when no one is reading <code>/dev/rpl</code>.</p>

<p class="block"><i>Excerpt from <code>kernel-2.6/rpldev.c</code></i></p>

<p class="code"><code>
<b>static ssize_t</b> rpldev_read(<b>struct</b> file <b>*</b>filp, <b>char *</b>buf, <b>size_t</b> count, <b>loff_t *</b>ppos)<br />
{<br />
&nbsp; &nbsp; ...<br />
&nbsp; &nbsp; <b>/* Data is available, so give it to the user */</b><br />
&nbsp; &nbsp; count = imin(count, avail_R());<br />
&nbsp; &nbsp; mv_to_user(buf, count);<br />
&nbsp; &nbsp; ...<br />
&nbsp; &nbsp; <b>return</b> count;<br />
}</code></p>

<h1>Fourth stage - userspace logging daemon</h1>

<p class="block">A userspace daemon reads, evaluates and stores the data
retrieved via the device.</p>

<p class="block">The reasons why I think this design is good are that:</p>

<ul>
  <li>Only the real necessary stuff is compiled <i>into</i> the kernel,
    everything else goes as a <i>module</i>.</li>
  <li>(This goes in accordance with the above even if it sounds weird:) Only
    the real necessary stuff is done <i>within</i> kernel space, everything
    else is put into user-space.</li>
  <li>The amount of changes you need to apply when modifying one stage.  You
    could change the inner working of the module, change the way the device
    responds to the logging daemon, etc. without needing to change too
    much.</li>
  <li>You can implement something in <i>rpld</i> while not changing any kernel
    components.</li>
  <li>User memory can be swapped out if is not used, kernel memory can not.
    (Though, <i>rpld</i> locks the buffer in memory, heh heh.) If there is no
    tty activity, the logging daemon will not become active and thus can be
    swapped out to give other applications some more physical memory.</li>
  <li>Do whatever you like. For example, compress the records, userspace is
    open, because you can link all your favorite libraries.</lI>
</ul>

<h1>Fifth stage - replaying logs</h1>

<p class="block"><i>ttyreplay</i> is that thing to analyze the logs stored by
<i>rpld</i>. Thanks to the time-stamping of <i>rpld</i> and the 1:1 passing of
arbitrary data allows <i>ttyreplay</i> to show what has happened on the chosen
terminal in real-time, and exactly as the original user saw it. (Objection: You
need to use the same terminal type and size, because also the ANSI codes are
reprinted as-is.)</p>

<p class="block">Being able to play it back in real-time also requires a fine
timer for the delays between two keystrokes&nbsp;/ data packets. I found out
that delays have a maximum precision (in the <code>SCHED_OTHER</code> policy
domain), so I invented some kind of algorithm to bypass this visible effect for
the user:</p>

<p class="block">It's about the "delay overhead correction algorithm". The
maximum delay precision for user-space applications within
<code>SCHED_OTHER</code> is <code>1/HZ</code> seconds. (See
<code>linux/include/asm/param.h</code>). So when wanting a 5000 microsecond
delay, the real time we are waiting is between 10k to 15k µs. To get around
this, the algorithm checks the time it has actually spent for a particular
delay. For more details, please see the <code>usleep_ovcorr()</code> function
in <code>user/replay.c</code> which has been commented thoroughly.</p>

<h1>See also</h1>

<ul>
  <li><a href="ttyreplay.1.php"><code>ttyreplay(1)</code></a></li>
  <li><a href="rpldev.4.php"><code>rpldev(4)</code></a></li>
  <li><a href="rpld.8.php"><code>rpld(8)</code></a></li>
</ul>

<?php include_once("_footer.php"); ?>
