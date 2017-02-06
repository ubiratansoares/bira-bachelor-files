<?php include_once("_header.php"); ?>

<h1>rplctl - control rpld</h1>

<p><code>rplctl <b>[</b>-A <i>tty</i><b>] [</b>-D <i>tty</i><b>] [</b>-L
<b>[</b><i>tty</i><b>]] [</b>-S <i>tty</i><b>] [</b>-X <i>tty</i><b>] [</b>-Z
<b>[</b><i>tty</i><b>]] [</b>-f <i>socket</i><b>] [</b>-t<b>]</b></code></p>

<h1>Description</h1>

<p class="block">The <code>rplctl</code> utility is used to query and control
<i>rpld</i> (and its <i>infod</i> subcomponent). It will print info about the
requested ttys, or all ttys currently monitored or optionized if no arguments
are given.</p>

<p class="block"><i>rplctl</i> will read the <code>INFOD_PORT</code> variable
from <code>rpld.conf</code> by default, if not overriden by the <code>-f</code>
option.</p>

<h1>Options</h1>

<table border="1">
  <tr>
    <td class="t1n"><code>-A <i>tty</i></code></td>
    <td class="t1">Activate logging for the given tty.  It can be a filename
      (which must exist) or a device number using
      "<code>=MAJOR,MINOR</code>".</td>
  </tr>
  <tr>
    <td class="t2n"><code>-D <i>tty</i></code></td>
    <td class="t2">Deactivate logging for the given tty.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-L <b>[</b><i>tty</i><b>]</b></td>
    <td class="t1">Explicitly request a listing of all ttys currently active or
      have option set. If a <i>tty</i> is given, only display info for that
      particular one.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-S <i>tty</i></code></td>
    <td class="t2">Deactivate logging for the given tty until it is
      deinitialized.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-X <i>tty</i></code></td>
    <td class="t1">Call <code>log_close()</code> for the given tty. <i>rpld</i>
      will close the logfile, and necessarily open up a new one whenever new
      activity is detected (and logging this terminal is not deactivated) on
      that terminal.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-Z <b>[</b><i>tty</i><b>]</b></code></td>
    <td class="t2">Zero all packet and byte counters, both of <i>rpld</i> and
      every single tty. If a <i>tty</i> is given, only zeroes the stats of that
      tty.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-f <i>socket</i></code></td>
    <td class="t1">Path to the <i>infod</i> socket to connect to. The default
      is <code>/var/run/.rplinfo_socket</code> or <code>INFOD_PORT</code> in
      the <code>rpld.conf</code> configuration file.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-t</td>
    <td class="t2">Generate output suitable for <code>sscanf()</code>,
      <code>split()</code> or anything that deals best with simple text
      strings. See below for details on the output format.</td>
  </tr>
</table>

<h1>Example output</h1>

<p class="block">A command like <code>rplctl</code> could output this:</p>

<p class="code"><code>
A TTY &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;BYTES IN &nbsp; &nbsp; &nbsp; OUT FILENAME<br />
==========================================================================<br />
&nbsp; (bytes) &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;2159 &nbsp; &nbsp; 81129 ttyrpld 2.19/Linux<br />
&nbsp; (packets) &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;14 &nbsp; &nbsp; &nbsp; 250<br />
--------------------------------------------------------------------------<br />
D tty7 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;937 &nbsp; &nbsp; &nbsp; &nbsp; 0 root/20050328.224208.tty7<br />
D pts-1 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 0 &nbsp; &nbsp; &nbsp; 586 boinc/20050328.224219.pts-1<br />
D pts-2 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 0 &nbsp; &nbsp; &nbsp;5812 root/20050328.224208.pts-2<br />
D pts-3 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 358 &nbsp; &nbsp; 22113 root/20050328.224208.pts-3<br />
D pts-4 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 864 &nbsp; &nbsp; 52618 root/20050328.224341.pts-4<br />
--------------------------------------------------------------------------
</code></p>

<p class="block">The status field (<code>A</code>) can be one of A, D or S:
<b>A</b>ctivated, <b>D</b>eactivated, Deactived for this <b>S</b>ession.</p>

<h1>sscanf() format</h1>

<p class="block">On the other hand, <code>rplctl -t</code> could produce
this:</p>

<p class="code"><code>
ttyrpld 2.19<br />
format 4<br />
0 144 143 0 3347 2496 3756 122881 0 0<br />
D 4 7 tty7 1507 0 /var/log/rpl/root/20050328.224208.tty7<br />
D 136 1 pts-1 0 1241 /var/log/rpl/boinc/20050328.224219.pts-1<br />
D 136 2 pts-2 0 7585 /var/log/rpl/root/20050328.224208.pts-2<br />
D 136 3 pts-3 365 22798 /var/log/rpl/root/20050328.224208.pts-3<br />
D 136 4 pts-4 1884 91257 /var/log/rpl/root/20050328.224341.pts-4<br />
</code></p>

<p class="block">The first line of the "parseable" output (<code>-t</code>) is
the program used. It can (and should) be ignored by programs querying
<i>infod</i>. The second is the status line about <i>rpld</i>. The
<code>sscanf</code> formula is "<code>%lu %lu %lu %lu %llu %llu %lu
%lu</code>". The fields are (in this order):</p>

<ul>
  <li><code>EVT_OPEN</code>&nbsp;-- number of <code>open()</code> operation on
    any tty</li>
  <li><code>EVT_LCLOSE</code>&nbsp;-- final tty closes</li>
  <li><code>EVT_READ</code>&nbsp;-- payload bytecount</li>
  <li><code>EVT_WRITE</code>&nbsp;-- payload bytecount</li>
  <li><code>EVT_READ</code>&nbsp;-- packets read from <i>rpldev</i></li>
  <li><code>EVT_WRITE</code>&nbsp;-- packets read from <i>rpldev</i></li>
  <li>packets considered bad (i.e. bad magic)&nbsp;-- might be more than there
    are actually damaged, because the algorithm needs to find <i>something</i>
    that looks good.</li>
</ul>

<p class="block">All other lines are tty information lines, whose formula is
"<code>%c %ld %ld %s %llu %llu %s</code>", and the fields are:</p>

<ul>
  <li>status&nbsp;-- <code>'A'</code> for activated, <code>'D'</code> is
    deactivated and <code>'S'</code> means deactivated until session ends.</li>
  <li>major number</li>
  <li>minor number</li>
  <li>mnemonic string for the major/minor number</li>
  <li><code>EVT_READ</code> payload bytecount</li>
  <li><code>EVT_WRITE</code> payload bytecount</li>
  <li>full filename being written to (this is different from the human-readable
    output which only shows it in part)</li>
</ul>

<h1>See also</h1>

<ul>
  <li><a href="rpld.8.php"><code>rpld(8)</code></a></li>
</ul>

<?php include_once("_footer.php"); ?>
