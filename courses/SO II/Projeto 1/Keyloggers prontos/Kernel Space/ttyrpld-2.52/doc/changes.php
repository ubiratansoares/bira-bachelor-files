<?php include_once("_header.php"); ?>

<h1>Overview</h1>

<p class="block">You do not need to repatch your kernel or recompile
rpldev if they have not changed significantly between ttyrpld
releases.</p>

<table border="1">
  <tr>
    <th class="t1">Operating system</th>
    <th class="t1">last time KPATCHes changed:</th>
    <th class="t1">last time rpldev changed:</th>
  </tr>
  <tr>
    <td class="t2">Linux</td>
    <td class="t2">2.50</td>
    <td class="t2">2.50</td>
  </tr>
  <tr>
    <td class="t1">FreeBSD</td>
    <td class="t1">2.50</td>
    <td class="t1">2.50</td>
  </tr>
  <tr>
    <td class="t2">OpenBSD</td>
    <td class="t2">2.50</td>
    <td class="t2">modules not really supported</td>
  </tr>
  <tr>
    <td class="t1">NetBSD</td>
    <td class="t1">2.50</td>
    <td class="t1">2.50</td>
  </tr>
</table>

<h1>v2.52 (2008-09-07)</h1>

<ul>
	<li class="star">Updated kernel components for Linux 2.6.27</li>
	<li class="star">Updated userspace code for libHX 1.25</li>
</ul>

<h1>v2.51 (2008-05-15)</h1>

<ul>
	<li class="star">Updated rpldhk and rpldev for Linux 2.6.25,
		OpenBSD 4.3, FreeBSD 7.0, NetBSD 4.0</li>
</ul>

<h1>v2.50 (2008-01-01)</h1>

<ul>
	<li class="star">changed the packet format to be compatible between
		32-bit and 64-bit</li>
	<li class="plus">added <i>rplcvt</i> utility for converting old
		on-disk rpld files</li>
	<li class="star">explicitly blacklist <code>/dev/stdout</code>,
		<code>/dev/stderr</code> and <code>/dev/stderr</code> -
		these have been popping up as tty names because there is such
		a symlink in Linux's <code>/dev</code>. (But we need symlink
		processing for Solaris, eww.)</li>
</ul>

<h1>v2.19 (2007-09-17)</h1>

<ul>
  <li class="star">The kernel interface has been reduced (hooks removed).
    The 2.19 userspace is still compatible (throws warnings though) with
    2.18 kernel components however. The 2.19 kernel patch is for 2.6.22.</li>
  <li class="plus">an experimental Solaris kernel driver has been added -
    tested with Solaris 11.snv only, but may work with older ones.</li>
  <li class="star">fix a file descriptor leak in rpld (thanks J.Eller)</li>
  <li class="star">fix a symlink recursion when walking <code>/dev</code>
    (J.Eller)</li>
  <li class="plus">raise maximum number of open files (J.Eller)</li>
  <li class="star">handle permission restrictions on <code>/dev</code> nodes</li>
  <li class="star">fix directory name of Persian translation</li>
  <li class="star">moved to autotools</li>
  <li class="star">build with new libHX 1.10</li>
</ul>

<h1>v2.18 (2006-07-01)</h1>

<ul>
  <li class="star">Fixed OpenBSD 3.9 kpatch</li>
  <li class="plus">Readded workaround for OpenBSD/NetBSD EINTR signal handling that got lost in 2.17</li>
</ul>

<h1>v2.17 (2006-06-19)</h1>

<ul>
  <li class="plus">Support for Linux 2.6.17, FreeBSD 6.1, OpenBSD 3.9</li>
  <li class="plus">added missing <code>-fprofile-arcs -ftest-coverage</code> to <code>LDFLAGS</code> for <code>PROF=1</code></li>
</ul>

<h1>v2.16 (2006-04-01)</h1>

<ul>
  <li class="minus"><code>user/rpld.c</code>: removed a superfluous variable</li>
  <li class="star">Added polling functions to the BSD rpldevs. This fixes the high CPU usage (which is due to the implementation of pthreads on OpenBSD) observed with <i>rpld</i>/OpenBSD.</li>
  <li class="star">OpenBSD part is again just a patch since modules are said to be unsupported on non-i386</li>
  <li class="star">Due to popular request, the docs now come with a black-on-white ("paper color") color scheme and no fixed font size. The old gray-on-black ("console color") is available as an alternate stylesheet and can be used in Firefox using <i>View&nbsp;&gt; Page Style&nbsp;&gt; Black background</i>.</li>
  <li class="plus">Added the <code>EVT_MAGIC</code> pseudo event type</li>
  <li class="plus">Added <code>share/ttyrpld.magic</code> for the <code>file(1)</code> utility</li>
</ul>

<h1>v2.15 (2006-03-11)</h1>

<p><b>General:</b></p>

<ul class="clog">
  <li class="plus">added support for NetBSD 3.0</li>
  <li class="star">shifts for majors<code>&gt;255</code> and minors<code>&gt;255</code> were broken in <code>include/dev.h</code></li>
</ul>

<p><b>kpatch:</b></p>

<ul class="clog">
  <li class="star">[All] fixed some SMP race conditions</li>
  <li class="plus">[OpenBSD] fixed: <code>rpl_deinit()</code> was not called
    in <code>ttyclose()</code></li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="star">[All] rename functions to make function more clear (<code>mv_to_user</code>&nbsp;-&gt; <code>circular_get</code>, <code>mv_buffer</code>&nbsp;-&gt; <code>circular_put_packet</code>, <code>mv_buffer2</code>&nbsp;-&gt; <code>circular_put</code>)</li>
  <li class="star">[All] make <code>avail_R()</code> and <code>avail_W()</code> return <code>size_t</code> instead of <code>ssize_t</code>, addressing some potential overflow problems</li>
  <li class="plus">[Linux] document circulary calculation of <code>avail_*()</code>, please report errors</li>
  <li class="star">[FreeBSD] merged <code>freebsd-*/rpldev.c</code> into one file</li>
  <li class="minus">[FreeBSD/OpenBSD] removed <code>SKIP_PTM</code>, since master ptys do not generate any noise (that's a kpatch feature)</li>
  <li class="star">[OpenBSD] rpldev is now a module (<code>rpldev_mod.o</code>)</li>
  <li class="star">[OpenBSD] loads at new device number (<code>228,0</code>), please re-mknod your <code>/dev/rpl</code> node</li>
</ul>

<h1>v2.12 (2006-01-21)</h1>

<ul class="clog">
  <li class="star">fixed: FreeBSD 6.0 hands <code>krpl_close()</code> a <code>NULL</code> pointer in some cases; handle it properly</li>
  <li class="star">some doc updates</li>
  <li class="star"><code>GNUmakefile</code>: "<code>all</code>" target does not include "<code>doc</code>" anymore, because BSDs (standard installation) often ship without <i>php</i>, <i>w3m</i> and <i>perl</i>.</li>
  <li class="plus">added the '<code>[</code>' and '<code>]</code>' keys to <i>ttyreplay</i> (lower/raise speed by 0.10)</li>
</ul>

<h1>v2.11 (2006-01-06)</h1>

<p class="block">Added support back in for the latest releases of the previous
generation of kernels. For Linux, that's 2.4.something (USE DISCOURAGED!), and
for FreeBSD that's 5.3.</p>

<h1>v2.10 (2005-12-04)</h1>

<p class="block">Support for OpenBSD 3.8 and FreeBSD 6.0 was added; permission
denied, syslog spamming, cpu hog, ownership and symlink recursion bugs were
fixed. Docs were updated to reflect advances since the last seven releases.</p>

<ul class="clog">
  <li class="plus">Support for OpenBSD 3.8 and FreeBSD 6.0</li>
  <li class="star">Documentation has been updated to reflect advances since the last seven releases.</li>
  <li class="star"><code>user/replay.c</code>: fixed a color bug when <code>-t</code> was active (spotted whilst under FreeBSD)</li>
  <li class="star"><code>user/*.c</code>: use <code>usleep(10000)</code> instead of <code>sched_yield()</code> to reduce idle cpu hog</li>
  <li class="star"><code>user/rpld.c</code>: umask was set wrong, so 1.) logging did not take place 2.) syslog was spammed with permission denied warnings</li>
  <li class="plus"><code>user/rpld.c</code>: added a rate limiter for syslog messages</li>
  <li class="star"><code>user/rpld.c</code>: find_devnode_dive() forgot to check for symlinks so that we dived into <code>/dev/fd</code>, which is not wanted, because <code>/dev/fd/*</code> itself includes <code>/dev</code> again, which lead to unwanted results.</li>
  <li class="star"><code>user/rpld.c</code>: fixed a filedescriptor leak in <code>fill_info()</code></li>
  <li class="plus"><code>user/rpld.c</code>: another try at getting ownerships right; rpld still had a case where the recorded owner differed from the owner of the <code>/dev</code> node (with respect to the design decision). See one below:</li>
  <li class="star"><code>linux-2.6/rpldev.c</code>: <code>/dev/tty</code> is a special case with respect to ownerships, so it must be filtered out in <code>krpl_init()</code> and <code>krpl_open()</code>.</li>
  <li class="star">using distribution-closer macros in specfile (e.g. <code>%_bindir</code> instead of <code>/usr/bin</code>)</li>
</ul>

<h1>v2.06 (2005-11-03)</h1>

<p class="block">This release was not publicly announced and rather served as a
testbed, because new bugs were expected.</p>

<ul class="clog">
  <li class="star">update to libHX 1.7.x</li>
  <li class="plus">rpld: added <code>EVT_ID_STAMP</code> containing the user and date the log was recorded</li>
  <li class="star">fixed: ttyreplay: <code>argv[0]</code>&nbsp;-- the program name, was also tried being replayed</li>
  <li class="star">ttyreplay: pctrl was not processed while waiting for more input data</li>
</ul>

<h1>v2.03.5 (2005-10-08)</h1>

<ul class="clog">
  <li class="star">update to libHX 1.6.2+</li>
</ul>

<h1>v2.03.4 (2005-09-22)</h1>

<ul class="clog">
  <li class="star"><code>check_gcc</code> macro gave a warning under *BSD because "<code>@gcc</code>" could not be found&nbsp;-- strange that it did not under Linux. Fixed anyway by reordering.</li>
  <li class="star">upstream upgrade: libHX 1.6.1</li>
  <li class="star"><code>linux-2.6/rpldev.c</code>: proper error propagation to userspace in <code>mv_to_user()</code></li>
</ul>

<h1>v2.03 (2005-07-27)</h1>

<ul class="clog">
  <li class="plus">updated locales</li>
  <li class="star">new postal FSF address</li>
  <li class="plus">Support for: OpenBSD 3.7, FreeBSD 5.4, FreeBSD 6.0-beta1, (PCBSD 0.7.8)</li>
</ul>

<h1>v2.02 (2005-07-07)</h1>

<ul class="clog">
  <li class="star">update to libHX 1.5.3</li>
  <li class="plus">the correct locale name for the Persian translation is fa_IR, not ar_IQ</li>
  <li class="star">compilation: simpler dependency generation model</li>
  <li class="minus">ripped <i>popt</i> out, and replaced it by <i>HX_getopt</i></li>
  <li class="plus">support for OpenBSD 3.7</li>
  <li class="star">fixed: <code>printf("It won't be possible ...")</code> was given an uninitialized string</li>
  <li class="star">corrected: OpenBSD: ptc data was only discarded on i386</li>
  <li class="star">corrected: OpenBSD: tty data was enqueued into the ring buffer even if <code>/dev/rpl</code> was not open</li>
  <li class="plus">patch from Nguyen Anh Quynh to have the Makefiles find out whether <code>-funit-at-a-time</code> is supported</li>
</ul>

<h1>v2.01 (2005-05-31)</h1>

<ul class="clog">
  <li class="star">update to libHX 1.5.0 (<code>so.0.5</code>)</li>
  <li class="star">some translation updates (by far not complete)</li>
</ul>

<h1>v2.00 (2005-03-29)</h1>

<p><b>kpatch:</b></p>

<ul class="clog">
  <li class="plus">support for Linux 2.4.29, 2.4.30-rc3</li>
  <li class="plus">support for 2.6.11, 2.6.11.*, 2.6.12-rc1 and 2.6.11.4-SUSE_9.3</li>
  <li class="plus">support for OpenBSD 3.6</li>
</ul>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="plus">support for OpenBSD 3.6</li>
  <li class="plus">support for dynamic major/minor device numbers of the ttys</li>
  <li class="plus">supports "<code>-</code>" as an alias for <code>/dev/stdin</code> for <code>-D</code> option</li>
  <li class="plus">device path is now recorded into logfiles</li>
  <li class="star">ttys will now also show up in <i>rplctl</i> if they have just been touched, i.e. <code>open()</code>ed. There was a misconception where a <code>/dev/pts/</code> entry existed, but were not listed&nbsp;-- it was not a bug, but kwrite (kde "talk") that just opened it without using it.</li>
  <li class="star"><code>-Q</code> now sets all tty channels to <code>IFP_DEACTIVATE</code>, which means that they can be activated on a per-tty basis. (Before, the logfile always was <code>/dev/null</code> which did not leave much room for activation.)</li>
  <li class="star">byte counters continue to count even if tty is in <code>IFP_DEACTIVE</code> or <code>IFP_DEACTIVSES</code></li>
  <li class="star">infod: now uses '<code>A</code>', '<code>D</code>' and '<code>S</code>' for both text and parseable formats; format number bumped to 3</li>
  <li class="star">fixed: <code>include/dev.h</code> did not handle *BSD device numbering</li>
</ul>

<p><b>rplctl:</b></p>

<ul class="clog">
  <li class="plus">fixed a "Broken pipe" which only showed up on *BSD&nbsp;-- it was not critical, but now the cosmetics fit again</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="plus">timestamping is now done within kernelspace, it is more efficient there than it is in userspace</li>
  <li class="plus">added ioctls <code>RPL_IOC_IDENTIFY</code>, <code>RPL_IOC_SEEK</code>, <code>RPL_IOC_FLUSH</code> (because *BSD lack seek handling on char devs), so it must be synthesized</li>
</ul>

<ul class="clog">
  <li class="star">[Linux 2.4] fixed: could not compile because of <code>tty-&gt;driver-&gt;major</code> in <code>PTY_MASTER</code>; <code>tty-&gt;driver.major</code> is correct</li>
  <li class="plus">[Linux] send filename on <code>kio_init()</code>/<code>kio_open()</code></li>
</ul>

<ul class="clog">
  <li class="star">[FreeBSD] fixed: device could not be reopened when malloc failed</li>
  <li class="plus">[FreeBSD] implemented <code>uio_ioctl()</code></li>
</ul>

<h1>v1.44.2 (2005-02-10)</h1>

<ul class="clog">
  <li class="star">update to libHX-0_3 (needs at least 20050206)</li>
  <li class="star">fixed: <code>kpatch/freebsd-5.3.diff</code> patched <code>sys/conf.h</code> in a way that could have possibly led to a memory corruption within the kernel</li>
  <li class="star">fixed: <code>LDFLAGS</code> was missing <code>-pg</code> in profiling mode</li>
  <li class="star">fixed: <code>CFLAGS</code> was missing <code>-f<b>no</b>-omit-frame-pointer</code> in profiling mode, being unable to compile (gcc exits with an error-warning)</li>
  <li class="plus">works with Linux 2.4.28 (vanilla) and 2.6.10 (vanilla)</li>
  <li class="plus">added <code>ROOT</code> and <code>PREFIX</code> makefile vars</li>
  <li class="plus">providing RPM packages for the userspace parts now. (kmod sources in <code>/usr/src/ttyrpld-x.yy-kd/</code>)</li>
  <li class="plus">added <code>/usr/sbin/rcrpld</code> as a symlink to <code>/etc/init.d/rpld</code> (rpm only)</li>
</ul>

<h1>v1.44 (2005-01-20)</h1>

<p><b>linux-2.*/rpldev.c:</b></p>

<ul class="clog">
  <li class="star">fixed a one-off bug, where <code>lseek()</code> allowed to
    jump to e.g. <code>0x10000</code> (when <code>Bufsize</code> is 64K)&nbsp;--
    that's invalid, <code>0x0ffff</code> is the last valid position. this had
    caused <i>rpld</i> to exit without an apparent reason.</p>
</ul>

<p>FreeBSD is not affected.</p>

<p><b>other:</b></p>

<ul class="clog">
  <li class="star">minor changes to the docs</li>
  <li class="minus"><code>--chkconf</code> option removed, md5 checking does
    not work like it was thought:</li>
  <li class="star">if <code>/etc/rpld.conf</code> already exists, it is not
    overwritten</li>
</ul>

<h1>v1.42 (2005-01-11)</h1>

<p class="block">The long-awaited translations are in! Includes: ar_IQ
(Persian-Farsi) [sic; fa_IR], de, es, fr, it, nb, nl and sv.</p>

<p><b>linux-2.*/rpldev.c:</b></p>

<ul class="clog">
  <li class="star">changed <code>Open_lock</code> scheme so that no mutex is held upon return to userspace</li>
  <li class="star"><code>lseek()</code> on the device will now change atime&nbsp;-- because we really slurp data rather than skipping it; it is lost after skip</li>
</ul>

<p><b>freebsd-5.3/rpldev.c:</b></p>

<ul class="clog">
  <li class="plus"><code>kio_ioctl()</code> has been implemented</li>
</ul>

<p><b>misc:</b></p>

<ul class="clog">
  <li class="star">fixed <code>USER=</code> in <code>rpld.conf</code> which became "<code>root</code>" by accident, also, <code>START_INFOD=</code> is now "<code>yes</code>" again</li>
  <li class="plus">updated docs with more BSD-specific stuff</li>
</ul>

<h1>v1.40 (2004-12-24)</h1>

<ul class="clog">
  <li class="plus">added support for FreeBSD 5.3</li>
  <li class="plus">added support for I18N</li>
</ul>

<h1>v1.31 (2004-12-20)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="star">fixed crash when <code>/sys/module/rpldev/Minor_nr</code> does not exist and <code>/proc/misc</code> is being tried to read</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="minus"><code>kio_{init,open,read,write,ioctl}()</code> now prepare data even more; they skip pty master data and transform the weird logic (as with <code>EVT_DEINIT</code>) to normal. This might reduce the CPU usage of <i>rpld</i>.</li>
  <li class="star"><code>kio_{read,write}()</code>: do not write zero-length data into ring buffer</li>
  <li class="star">fixed a <code>NULL</code> pointer dereference Oops which occurred when <code>misc_register()</code> failed</li>
  <li class="minus">fixed kernel memory corruption in <code>mv_buffer2()</code> and <code>mv_to_user()</code> when <code>count==x</code></li>
  <li class="minus">added missing <code>Buffer_lock</code> locks in <code>uio_{open,close}()</code></li>
  <li class="minus"><code>struct {rpld,mem}_packet.dev2</code> member has been obsoleted again</li>
</ul>

<p><b>other:</b></p>

<ul class="clog">
  <li class="star"><code>etc-init.d-rpldev</code>: fixed typo</li>
</ul>

<h1>v1.26 (2004-11-23)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="minus">pty master sides were also counted, which lead to correct but weird big numbers in the totale "BYTES IN" field in <i>rplctl</i></li>
</ul>

<p><b>infod:</b></p>

<ul class="clog">
  <li class="star">fixed: doing <code>rplctl -L</code> on a tty which did not have a logfile associated crashed <i>rpld</i> (this was due to <code>basename_pp()</code>, introduced in 1.24)</li>
  <li class="star">fixed: not all stats were zeroed</li>
  <li class="plus">added per-tty zeroing</li>
</ul>

<p><b>rplctl:</b></p>

<ul class="clog">
  <li class="star">fixed: <code>COMPAT_MAJOR</code> and <code>COMPAT_MINOR</code> macros were broken</li>
  <li class="plus"><code>-L</code> and <code>-Z</code> can now optionally accept a single tty</li>
  <li class="minus">removed <code>-F</code> option (<i>infod</i> code and doc part was removed long ago)</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="plus">now compiles with GCC 3.4</li>
  <li class="minus">removed overrun warning</li>
</ul>

<p><b>other:</b></p>

<ul class="clog">
  <li class="star">fixed: <code>etc-init.d-rpld</code> was copied to <code>/etc/init.d/etc-init.d-rpld</code> rather than <code>/etc/init.d/rpld</code></li>
  <li class="plus"><code>make install</code> will not overwrite <code>/etc/rpld.conf</code> if it is modified</li>
</ul>

<h1>v1.24 (2004-11-18)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="star">internal: device number fixup has been moved to rpldev</li>
  <li class="plus">support for <code>/sys/module/rpldev</code> has been added</li>
  <li class="plus">added compat-style <code>dev_t</code> support (<code>COMPAT_*</code> in <code>include/dev.h</code>)</li>
  <li class="plus">better device number differentiation for different kernel versions (i.e. only minor 128 for 2.6, while 128...143 for 2.4 (for ptms))</li>
  <li class="plus"><code>unlink()</code> before <code>mknod()</code></li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="star">its <code>Makefile</code> has been changed to also take a variable <code>KERNEL_DIR</code> and <code>MODULES_DIR</code></li>
  <li class="plus">using new <code>module_param()</code> for 2.6 environment</li>
  <li class="plus">added check for endianesses which are neither little nor big</li>
  <li class="plus">added the <code>WOverrun</code> flag controlling warning messages about buffer overrun</li>
</ul>

<p><b>kpatch:</b></p>

<ul class="clog">
  <li class="plus">added <code>kpatch/2.0.40.diff</code> and <code>kpatch/2.2.26.diff</code>, maybe it is not useful but it is there for completeness</li>
</ul>

<p><b>doc:</b><br />
Countless minor (sometimes major) adjustments.</p>

<ul class="clog">
  <li class="plus">desc: Staying in sync with the short desc and trove cat'gz. set on SF</li>
  <li class="plus">desc: Listing the four core components</li>
  <li class="plus">install: building notes for 2.4 and before</li>
  <li class="plus">rpldev(4): mentioning <code>/sys</code> things</li>
  <li class="star">support: largely rewritten to include the new things: FRS/SF, Patches and Feature Request Tracker, Mailing Lists</li>
</ul>

<h1>v1.21 (2004-11-13)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="star">corrected condition inversion in <code>G_skip()</code></li>
  <li class="plus">redesigned the locking scheme (which is needed between <i>rpld</i>&lt;-&gt;<i>infod</i>) down to one big lock (which is also taken less in certain places).</li>
  <li class="star"><code>log_write()</code> now uses a one-shot read</li>
  <li class="plus">will now automatically create parent directories if needed (like <code>mkdir -p</code>)</li>
  <li class="plus">sets umask so that the maximum permissions are <code>-wx------</code></li>
  <li class="plus">added <code>-i</code> option (counter <code>-I</code>)</li>
</ul>

<p><b>infod:</b></p>

<ul class="clog">
  <li class="minus"><code>FLUSH</code> request has been dropped, entries are automatically flushed by <i>rpld</i></li>
  <li class="minus">display of file descriptor has been removed</li>
</ul>

<p><b>rplctl:</b></p>

<ul class="clog">
  <li class="star">option change: <code>-D</code>&nbsp;=&gt; <code>-S</code> (close until session ends / tty is deinitialized), <code>-J</code>&nbsp;=&gt; <code>-D</code> (deactivate total)</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="star">(big-endian only) fixed <code>my_swab()</code> [had <code>count=={16,32,64}</code> instead of <code>{2,4,8}</code>]</li>
</ul>

<p><b>ttyreplay:</b></p>

<ul class="clog">
  <li class="minus">outsourced the <code>EVT_WRITE</code> branch from <code>replay_file()</code> into <code>e_proc()</code></li>
  <li class="plus">added the echo key to also show <code>EVT_READ</code> packets</li>
  <li class="star">fixed a possible loss of precision in the "<code>i-&gt;factor != 1.0</code>" branch (added <code>(double)</code> explicit conversion)</li>
  <li class="star">replaced "packet skipping" with <code>read_through()</code> because otherwise screen initializations might be missing</li>
</ul>

<p><b>doc:</b></p>

<ul class="clog">
  <li class="star">rpl(4) moved to rpl(5)&nbsp;-- now it's in the appropriate category</li>
  <li class="star">fixed some spellings, constructs and etc.</li>
  <li class="minus">took out some old paragraphs and</li>
  <li class="minus">removed <code>doc/design.txt</code>, it was already in ttyrpld(7)</li>
  <li class="plus">updated docs with new stuff, check it out!</li>
  <li class="plus">new design</li>
</ul>

<p><b>Makefile:</b></p>

<ul class="clog">
  <li class="minus"><code>MARCH</code> and <code>MCPU</code> variables have been taken out, superseded by <code>EXT_CFLAGS</code></li>
  <li class="star">"<code>kmod</code>" target will now <code>rmmod rpldev</code> before reloading it</li>
</ul>

<h1>v1.13.1 (2004-11-03)</h1>

<ul class="clog">
  <li class="minus">Removed stray <code>.orig</code> file in <code>kpatch/</code></li>
  <li class="plus">added <code>kpatch/2.6.9-vanilla</code></li>
  <li class="star">added a workaround for GCC &gt;= 3.4 which has problems with inline</li>
</ul>

<h1>v1.13 (2004-11-02)</h1>

<p class="block">Ttys still were not closed for some reasons, so that once
opened ttys dev numbers would never be logged again, or at least &lt;...&gt;
anyway something was wrong. And it was due some bad code in UML... I relied on,
sigh.</p>

<p class="block">Anyway, it's corrected now. And it was painstaking to find the
right spots myself, because actually UML placed a CLOSE where an OPEN should
have been. OTOH, UML already had a OPEN, so I had two of them. Which lead me to
the conclusion to redesign it a little bit.</p>

<p><b>rpld/rpldev:</b></p>

<ul class="clog">
  <li class="star">changed the <code>EV_</code> constants to <code>EVT_</code> to avoid clashes with <code>ELF</code> code</li>
  <li class="plus">new <code>EVT_{INIT,OPEN,CLOSE,DEINIT}</code> event types</li>
  <li class="star">the event type and magic bytes have not been touched since the on-disk format (as generated by rpld) remains the same, it's just a change between the "protocol" used between <i>rpldev</i> and <i>rpld</i>.</li>
  <li class="plus"><code>.dev2</code> member has been added to the <code>struct rpld_packet</code></li>
  <li class="star"><code>struct {rpld,mem,disk}_packet</code> are now overall tagged with PACKED</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="plus">new <code>krn_{init,deinit}()</code></li>
  <li class="plus">tagged certain vars as <code>__user</code></li>
  <li class="plus">properly including <code>&lt;linux/compiler.h&gt;</code> or defining <code>__user</code> to nothing</li>
  <li class="minus">removed unnecessary byteswap operations (only relevant for big-endian)</li>
</ul>

<p><b>kpatch:</b></p>

<ul class="clog">
  <li class="plus">new patch against 2.6.9-rc2</li>
</ul>

<h1>v1.01 (never actually released)</h1>

<p><b>infod:</b></p>

<ul class="clog">
  <li class="star">changed the protocol to allow requests with "void", "int" and "string" parameters</li>
</ul>

<p><b>rplctl:</b></p>

<ul class="clog">
  <li class="star">option parser behavior changed: <code>-tL</code> (<code>-t -L</code>) is now different from <code>-Lt</code> (<code>-L -t</code>) in that the order is significant... in <code>-Lt</code>, <code>-L</code> is executed before <code>-t</code> so it would still show readable text rather than parseable.</li>
</ul>

<p><b>other:</b></p>

<ul class="clog">
  <li class="plus">added one more block to the legal statement</li>
  <li class="plus">added <code>ETCINITD_QUIET</code> to <code>rpld.conf</code></li>
  <li class="star">corrected the make <code>clean</code> target for "<code>kernel-2.6</code>" directory so that it continues to work when something changes in the kernel tree</li>
</ul>

<h1>v1.00 (2004-09-26)</h1>

<p class="block">New edict: if I don't explicitly say "you need to/should
upgrade the rpldev kernel module" under "rpldev:" in this Changelog, then you
don't need to! (Though, you always can. It's just a module and a cheap
recompile without reboot.)</p>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="plus">will continue starting <i>rpld</i> even if config file could not be read</li>
  <li class="plus">warning message if the RPL device returns <code>EBUSY</code></li>
  <li class="star">fixed: <code>/var/run/.rpldev</code> was not unlinked if it could not be opened</li>
</ul>

<p><b>infod:</b></p>

<ul class="clog">
  <li class="star">the parsetext (<code>rplctl -t</code>) "status field" has been changed: from (A, D, S) to (A, J, D) to reflect the rplctl switches</li>
  <li class="plus">will now print a '<code>?</code>' as status character if something got us corrupted</li>
</ul>

<p><b>ttyreplay:</b></p>

<ul class="clog">
  <li class="plus">will now assume to play STDIN if no filenames are given and STDIN really is a pipe/file/etc. (anything but terminal)</li>
  <li class="plus">jumping to a given position (<code>-J and -j</code>) will now print the packets' contents to properly display e.g. graphical applications</li>
  <li class="plus">added play controls (forward 10/60 sec, pause, slower/faster playback, prev/next file, see DOCS!)</li>
  <li class="plus">displays the timestamp during replay (<code>-t</code> option)</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="star">worked around: the device driver could not be opened with 2.6.8 due to 2.6.8's new seeking design</li>
  <li class="minus">removed the debugging messages "RPL device opened" and .."closed"</li>
</ul>

<p><b>etc-init.d-ttyrpld:</b></p>

<ul class="clog">
  <li class="star">now uses the "install paths", i.e. <code>/usr/local/sbin/</code> rather than <code>/usr/lib/ttyrpld</code></li>
  <li class="plus">can work independent of <code>/etc/rc.status</code> (sometimes only to be found on SUSE Linux)</li>
</ul>

<p><b>other:</b></p>

<ul class="clog">
  <li class="plus">added "<code>install</code>" and "<code>uninstall</code>" targets to Makefile</li>
  <li class="star"><code>START_INFOD</code> in the default distribution is now "<code>yes</code>"</li>
  <li class="star">(all): big endian was never selected due to a typo in <code>__BYTE_ORER</code></li>
  <li class="plus">supporting Kernel 2.6.8-14 (SUSE) and 2.6.8 (vanilla)</li>
  <li class="star">Kernel patches moved into their own directory "<code>kpatch</code>"</li>
</ul>

<h1>v0.88 (2004-09-09)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="star">fixed a race condition in <code>log_close()</code>, where everything above <code>pthread_mutex_lock()</code> should have been while the lock is held</li>
  <li class="plus">user changes on tty device (such as on vc-*) will be detected and now correctly put into separate files. (See <code>doc/rpld.html</code> for details.)</li>
  <li class="star">fixed: a REMOVE request (<code>rplctl -X</code>) also triggered FLUSH (<code>-F</code>)</li>
</ul>

<p><b>infod:</b></p>

<ul class="clog">
  <li class="star">changed <code>IFP_GETINFO</code> into <code>IFP_GETINFO_T</code> (readable text output) and</li>
  <li class="plus">added <code>IFP_GETINFO</code> (parseable text output, e.g. <code>sscanf()</code>)</li>
</ul>

<p><b>rplctl:</b></p>

<ul class="clog">
  <li class="plus">added the <code>-L</code> switch to make <i>rplctl</i> ask a <code>IFP_GETINFO/_T</code> even if there are other arguments like <code>-ADFJX</code></li>
  <li class="plus">added the <code>-t</code> switch to use <code>IFP_GETINFO</code> instead of <code>IFP_GETINFO_T</code></li>
  <li class="star">fixed: <code>rplctl -X tty1 tty2</code> actually did not show info for tty2 (normal arguments ignored if any option switches were found)</li>
  <li class="plus">added the <code>-Z</code> switch to zero all stats counters</li>
</ul>

<h1>v0.87 (2004-09-06)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="star"><code>notify()</code> will now print to stderr and syslog if both of <code>-s</code> and <code>-v</code> are given (before, <code>-v</code> overrode <code>-s</code>)</li>
  <li class="star">fixed: <code>/proc/misc</code> was not closed</li>
  <li class="star">temporary device node path is now <code>/var/run/.rpldev</code> (was <code>./.rpldev</code>)</li>
</ul>

<ul class="clog">
  <li class="plus">added a subcomponent infod (socket is /var/run/.rplinfod_socket)
  <li class="plus">added the respective gather tool "rplctl"
</ul>

<ul class="clog">
  <li class="star">fixed: ttys were not removed from the tree in log_close()
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="plus">added <code>RPL_IOC_GET[RW]AVAIL</code> ioctl calls</li>
  <li class="star">fixed: a deadlock would occur if <code>kmalloc()</code> returned <code>NULL</code> and you tried to reload the module</li>
</ul>

<ul class="clog">
  <li class="plus">using <code>vmalloc()</code> to be able to allocate more than 64KB (<code>GI_bufsize</code>)</li>
</ul>

<p><b>ttyreplay:</b></p>

<ul class="clog">
  <li class="plus">added the <code>-m</code> option to specify a maximum delay</li>
  <li class="plus">added the <code>-j</code> (skip packet count) and <code>-J</code> (skip time) options to start playing at a given position in the logfile</li>
</ul>

<p><b>etc-init.d-ttyrpld:</b></p>

<ul class="clog">
  <li class="star"><i>rpld</i> will now be detached from the current session (but keep the current stdout/err to report errors).</li>
</ul>

<p><b>other:</b></p>

<ul class="clog">
  <li class="star">stepped once more through in theory and checked for necessary <code>swab()</code>s, both <i>rpldev</i> (added a lot) and <i>rpld</i> (more thought)</li>
  <li class="plus">logging on-the-fly-over-network has been tested and works well the two minutes I tested it</li>
</ul>

<h1>v0.85 (2004-08-27)</h1>

<ul class="clog">
  <li class="star">trying to make the different situations work (read/seek on file/pipe/dev)</li>
</ul>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="star">the on-disk packet format has been changed in that the device field has been removed; <i>rpld</i> was designed to analyze this field and write the buffer to a given file, which makes the <code>.dev</code> field a space waster.</li>
  <li class="plus">see above: now saving up to 10%</li>
</ul>

<ul class="clog">
  <li class="plus">will now generate an <code>EV_IDENT</code> packet for log files</li>
  <li class="plus">implemented the "text-only" output of logfiles (strip headers), option is <code>-T</code></li>
</ul>

<ul class="clog">
  <li class="minus">superfluous: removed the <code>BSIZE</code>, <code>FBSIZE</code> options. The device hands out its buffer size through an ioctl.</li>
  <li class="plus"><code>include/rpl_packet.h</code>: added an <code>EV_IGNORE</code> type</li>
</ul>

<ul class="clog">
  <li class="star">fixed: <code>EV_CLOSE</code> events only happen on pty master sides, so log files did not get closed for ptys</li>
  <li class="plus">added a debug option <code>-Q</code> to suppress creating files</li>
  <li class="plus">added a check to stop reading from non-rpldev when EOF is reached</li>
  <li class="star">some endianess fixes, untested though</li>
  <li class="star">fixed: exclusion of pty masters only worked for major 128</li>
</ul>

<ul class="clog">
  <li class="plus">BSD pty master fixup as it just went with Unix98 pty master (i.e. to catch the fact that the close is reported on the master side)</li>
</ul>

<p><b>ttyreplay:</b></p>

<ul class="clog">
  <li class="minus"><code>--std</code> option has been removed, Overhead Correction is also needed on 2.6 when using high values with <code>-S</code></li>
</ul>

<p><b>rpldev kernel module:</b></p>

<ul class="clog">
  <li class="plus"><code>uif_llseek()</code> will now not lock if it is not actually seeking</li>
</ul>

<p><b>other:</b></p>

<ul class="clog">
  <li class="plus"><code>etc-init.d-ttyrpld</code>: added missing "kick" help to (*) case</li>
  <li class="plus"><code>include/rpl_packet.h</code>: new event type: <code>EV_IDENT</code></li>
  <li class="star"><code>include/rpl_packet.h</code>: <code>struct log_packet</code> has been renamed to <code>struct disk_packet</code> to remove ambiguity</li>
  <li class="plus">new doc outfit and manual pages</li>
</ul>

<h1>v0.83 (2004-08-05)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="plus">automatically determines what minor number the rpl device is bound to and dynamically create a node</li>
</ul>

<p><b>ttyreplay:</b></p>

<ul class="clog">
  <li class="plus">added follow modes (<code>-F</code>, <code>-f</code> options)<br />
    <code>-F</code>: live feed (directly see what a user is typing), à la <code>tail -f</code><br />
    <code>-f</code>: play file as normal and switch into <code>-F</code> when EOF is reached</li>
  <li class="plus">added error recovery</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="star">the default minor number is now <code>MISC_DYNAMIC_MINOR</code> (see automatic minor number detection below for <i>rpld</i>)</li>
  <li class="plus">added an info upon module loading telling us what minor number the misc subsystem finally gave <i>rpldev</i></li>
</ul>

<h1>v0.82 (2004-08-03)</h1>

<p><b>rpld:</b></p>

<ul class="clog">
  <li class="star">a one-time variable was not freed (minimal impact)</li>
  <li class="plus">added <code>-v</code> option to interactively show statistics</li>
  <li class="plus">a new option "<code>OFMT</code>" has been added which combines (and thus obsoletes) the <code>LOGDIR</code> and <code>SBTTY</code> variables</li>
  <li class="plus">added "<code>%t</code>" (time in "<code>%H%M%S</code>" format) as a specifier for <code>OFMT</code></li>
  <li class="star">"<code>%d</code>" has been degraded to only provide the date ("<code>%Y%M%d</code>") rather than date-and-time</li>
</ul>

<p><b>ttyreplay:</b></p>

<ul class="clog">
  <li class="plus">finally implemented the "time warp" function (e.g. replay the log twice as fast as the original user on typed), it's the <code>-S</code> option ("speed"). It takes a multiplication factor (<code>2.0</code> = double, <code>0.5</code> = half speed, etc.)</li>
  <li class="star">the time correction algorithm has been redone (the one before still had a time skew); the main calculation unit is now µsec</li>
</ul>

<p><b>rpldev:</b></p>

<ul class="clog">
  <li class="plus">the user may now choose the minor number upon module loading</li>
</ul>

<p><b>other:</b></p>

<ul class="clog">
  <li class="plus">more precise error control in <code>etc-init.d-ttyrpld</code></li>
  <li class="star"><code>user/shared.c</code>: added defintions for BSD pty slaves, changed <code>pty-%</code> into proper <code>ptyp%%</code></li>
  <li class="star"><code>kernel-2.6/rpl-kernelpatch.diff</code>: the file has been replaced with patch which works for 2.6.5 and up</li>
</ul>

<h1>Initial Release 0.80 (2004-07-19)</h1>

<p>Initial release</p>

<?php include_once("_footer.php"); ?>
