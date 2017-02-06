<?php include_once("_header.php"); ?>

<h1>Requirements</h1>

<table border="1">
  <tr>
    <th class="t1">Component</th>
    <th class="t1">Minimum</th>
    <th class="t1">Recommended, or better</th>
  </tr>
  <tr>
    <td class="t2"><a
      href="http://jengelh.hopto.org/coding/libHX.php">libHX</a></td>
    <td class="t2">1.25</li>
    <td class="t2">1.25</li>
  </tr>
  <tr>
    <td class="t1">GNU C Compiler</td>
    <td class="t1">3.3?</li>
    <td class="t1">4.x</li>
  </tr>
  <tr>
    <td class="t2">iconv/libiconv</td>
    <td colspan="2" class="t2">any</td>
  </tr>
  <tr>
    <td class="t1">gettext/libintl</td>
    <td colspan="2" class="t1">any</td>
  </tr>
  <tr>
    <td colspan="3" class="t2"><b>If you want to recreate the
      documentation:</b></td>
  </tr>
  <tr>
    <td class="t2">gmake (for doc)</td>
    <td class="t2">3.79(?)</td>
    <td class="t2">3.81</td>
  </tr>
  <tr>
    <td class="t1">perl (for doc)</td>
    <td class="t1">-</td>
    <td class="t1">5.6+</td>
  </tr>
  <tr>
    <td class="t2">php (for doc)</td>
    <td class="t2">-</td>
    <td class="t2">5.x</td>
  </tr>
  <tr>
    <td class="t1">w3m (for doc)</td>
    <td class="t1">0.5.0</td>
    <td class="t1">0.5.1</td>
  </tr>
</table>

<h1>Unpacking</h1>

<p class="block">In the following code snippets, <code><b>#</b></code> denotes
a root/superuser shell, while <code><b>$</b></code> <i>can</i> be a user, or a
root shell. Listed are only the commands you have to execute, their output is
not shown in the boxes.</p>

<p class="block">If you made it until here, I assume you already unpackaged the
source tarball. If not, use:</p>

<p class="code"><code>
<b>$</b> tar -xvjf ttyrpld-<i>VERSION</i>.tbz2;
</code></p>

<p class="block">Of course, replace <i>version</i> with the correct string.
Non-GNU users might need some additional steps or different ways to unpack
<i>tar-bzip2</i> archives.</p>

<h1>Patching the kernel sources (Linux)</h1>

<p class="block">This is probably the step most of you will hate, because it
involves a recompile of the kernel, or at least <i>bzImage</i>.  There is
currently no other way. Once you have it prepared with the <i>rpldev</i> hooks,
you will never need to do so again, when you either get a new kernel tree or if
the <i>kpatch</i>es change (which is rare). That way, you can upgrade
<i>ttyrpld</i> (userspace) or the <i>rpldev</i> kernel module without a
reboot.</p>

<p class="block">You can find different kernel patches in the
<code>kpatch</code> directory, each targeting a different series and/or
version.  You can patch your kernel source tree by issuing these commands:</p>

<p class="code"><code>
<b>$</b> cd ~/src/linux-2.6.25/;<br />
<b>$</b> patch -p1 -i ~/src/ttyrpld/k_linux-2.6/rpldhk-2.6.25.diff;<br />
<b>$</b> make menuconfig;
</code></p>

<p class="block">Run <code>make menuconfig</code> or anything else that suits
your needs to choose "<code>TTY logging via rpldev</code>" in the
"<code>Security options</code>" menu.</p>

<h1>Patching the kernel sources (FreeBSD)</h1>

<p class="block">The procedure is similar to the one needed for the Linux
kernel. The kpatch for FreeBSD is a little bigger than the one for Linux, since
it has to add a close-per-fd method and coherent hooks for these.  As such, not
all of the exotic tty devices might be supported under FreeBSD.</p>

<p class="code"><code>
<b>$</b> cd /usr/src/sys/;<br />
<b>$</b> patch -p1 -i ~/src/ttyrpld/k_freebsd-7.0/rpldhk.diff;
</code></p>

<p class="block">... and rebuild the kernel image. No option needs to be
enabled in <code>conf</code>. The procedure on how to build the kernel under
FreeBSD might vary from person to person, but I generally prefer to install all
the <code>src</code> packages, and use:</p>

<p class="code"><code>
<b>$</b> cd /usr/src/;<br />
<b>$</b> make kernel<br />
</code></p>

<h1>Patching the kernel sources (OpenBSD and NetBSD)</h1>

<p class="block">The process is not much different:</p>

<p class="code"><code>
<b>$</b> cd /usr/src/sys/;<br />
<b>$</b> patch -p1 -i ~/src/ttyrpld/k_openbsd-4.3/rpldhk_rpldev.diff;
</code></p>

<p class="block">Rebuild as described in <code>afterboot(8)</code>:</p>

<p class="code"><code>
<b>$</b> cd /usr/src/sys/arch/i386/conf/;<br />
<b>$</b> config GENERIC<br />
<b>$</b> cd ../compile/GENERIC<br />
<b>$</b> make depend<br />
<b>$</b> make<br />
---<br />
<b>$</b> cp bsd /bsd &amp;&amp; reboot
</code></p>

<h1>Compiling the kernel module</h1>

<p class="block">OpenBSD users can skip this step, as they have a monolithic
kernel.</p>

<p class="block">After having booted the new kernel, change to the appropriate
<code>k_</code> directory for your operating system and run
<code>make</code>.</p>

<p class="block">You can tune the location of the kernel sources and module
installation path by passing <code>KERNEL_DIR=xxx</code> and/or
<code>MODULES_DIR=xxx</code> parameters to <i>make</i>, respectively. After
successful compilation, load the module.</p>

<p class="block">To load the module at boot time, edit your distribution
specific startup scripts. A hint for openSUSE users: it is in
<code>/etc/sysconfig/kernel</code> in <code>MODULES_LOADED_ON_BOOT</code>. If
you do not know where, ask someone who knows or simply put a <code>modprobe
rpldev</code> into <code>/etc/init.d/boot.local</code>. However, the supplied
<code>etc-init.d-rpld</code> script (to be put as
<code>/etc/init.d/rpld</code>) (for Linux/LSB) will be so kind to automatically
load it.</p>

<p class="block">There is currently a start script for <i>rpld</i> for Linux
only.</p>

<h1>Compiling the user-space applications</h1>

<p class="block">ttyrpld uses autoconf/automake (the latter of which is not
entirely suiting my developer's needs, but whatever), so compiling and
installing the user-space applications is a matter of running the two-step
procedure:</p>

<p class="code"><code>./configure<br />
make</code></p>

<p class="block">You can optionally run `<code>make install</code>`, but it is
not strictly required.</p>

<h2>FreeBSD specific hacks</h2>

<p class="block">FreeBSD does not have <code>/usr/local</code> in its default
include and link search directory list. Meh.</p>

<p class="code"><code>./configure CFLAGS="-I/usr/local/include"
LDFLAGS="-L/usr/local/lib"</code></p>

<h2>OpenBSD specific hacks</h2>

<p class="block">Like FreeBSD, <code>/usr/local</code> is missing.
Additionally, OpenBSD's <code>libintl.so.3.0</code> is missing a reference to
<code>libiconv.so</code> so you have to explicitly specify it:</p>

<p class="code"><code>./configure CFLAGS="-I/usr/local/include"
LDFLAGS="-I/usr/local/lib -liconv"</code></p>

<h1>Device node</h1>

<p class="block">Details are on the <a
href="rpldev.4.php"><code>rpldev(4)</code></a> manpage.</p>

<?php include_once("_footer.php"); ?>
