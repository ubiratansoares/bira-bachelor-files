<?php include_once("_header.php"); ?>

<h1>rpld - tty logging daemon</h1>

<p><code>rpld <b>[</b>-D <i>dev</i><b>] [</b>-I<b>] [</b>-O <i>ofmt</i><b>]
[</b>-Q<b>] [</b>-U <i>user</i><b>] [</b>-c <i>configfile</i><b>] [</b>-s<b>]
[</b>-v<b>]</b></code></p>

<h1>Description</h1>

<p class="block"><code>rpld</code> is the user-space daemon that reads
<code>/dev/rpl</code> and plexes the data to different files depending on which
tty they were logged. It also adds a timestamp so that replaying can be done in
real-time.</p>

<h1>Options</h1>

<table border="1">
  <tr>
    <td class="t1n"><code>-D <i>dev</i></code></td>
    <td class="t1">Path to the rpl device, e.g. <code>/dev/rpl</code>.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-I</code></td>
    <td class="t2">Start the <i>infod</i> component if it is not automatically
      started when the <code>INFOD_START</code> configuration variable is
      set.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-O <i>string</i></code></td>
    <td class="t1">Overrides the hardcoded and configuration file values
      (processed so far) for the log file naming scheme. You can use the
      printf-like placeholders explained below.  Subsequent <code>-c</code> may
      override this, if an <code>OFMT</code> variable is found in further
      configuration files. Make sure the user can create files according to
      <code>-O</code>/<code>OFMT</code>.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-Q</code></td>
    <td class="t2">All ttys start in deactivated mode (rather than activated).
      They can then be activated when needed.  Byte-counting is done in either
      case.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-U <i>user</i></code></td>
    <td class="t1">Drop all privilegues and change to <i>user</i> after
      initialization (memory lock, creating and opening RPL device). This can
      be either a username or a numeric UID.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-c <i>file</i></code></td>
    <td class="t2">Load configuration variables from <i>file</i>. The in-memory
      copies of the variables are not changed if no such var name is found in
      the file. Unknown var names in the configuration file are also
      ignored.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-s</code></td>
    <td class="t1">Print warnings and error messages to <i>syslog</i> rather
      than <code>stderr</code>.</td>
  </tr>
  <tr>
    <td class="t2"><code>-v</code></td>
    <td class="t2">Print statistics about recorded packets on
      <code>stdout</code> while <i>rpld</i> is running. This option overrides
      <code>-s</code>.</td>
  </tr>
</table>

<h1>Privilege separation</h1>

<p class="block"><i>rpld</i> offers the possibility to change to another user's
identity after the initialization phase is complete, even though if it is not
really required since there is very few to exploit (if at all!), and on top
it is local. The default package (from <i>ttyrpld.sf.net</i>) uses the
<code>daemon</code> user. You can change this in the configuration file.</p>

<h1>Logging</h1>

<p class="block"><i>rpld</i> does not detach itself to help debugging, but
using the <code>startproc</code>, <code>setsid</code> starter tools or bash's
<code>disown</code> builtin can help bringing it in the background, if you need
to. The only time when the daemon will output something is either during
initialization or when there is really trouble, like memory allocation failure.
In either case, error messages are rare and you would not need to worry about
redirecting <code>stderr</code>.</p>

<p class="block">You can run <i>rpld</i> with the <code>-v</code> option to
enable printing statistics on <code>stdout</code>. Even if you do not have
<code>-v</code> specified, you can send <i>rpld</i> a <code>SIGALRM</code>
signal to make it print the current statistics. That of course only makes sense
when <code>stdout</code> is connected to something but <code>/dev/null</code>
or <code>/dev/zero</code>. You can send it multiple <code>SIGALRM</code>s, of
course, but you should consider using <code>-v</code> then, maybe. The
statistics are printed with the move-to-beginning-of-line character
(<code>\r</code>), which is not that suitable for logging, though.</p>

<p class="block">Basically, every tty is monitored, but certain kinds are
excluded, like the master sides of BSD (major number 2 (Linux), 6 on BSD) and
Unix98 ptys (major number 128), because they are just a mirror of their slave
sides with things turned around and are rarely useful. (On BSD, they are not
explicitly excluded because they do not seem to generate any data.)</p>

<p class="block"><i>rpld</i> will divert logging of a tty to a new file when
the tty inode is opened the next time and if the owner of it has changed. This
will make logins on <code>/dev/tty</code><i>N</i> go to the right file. Note
that <code>su</code> does not change the ownership, so both sessions (normal
and <code>su</code>'ed) will go into the same file.  (And that's good, because
it then logs by "real person" rather than login name.) The byte count
statistics are zeroed upon change detection.</p>

<h1>Configuration file</h1>

<p class="block"><i>rpld</i> starts with its hardcoded defaults, then tries to
read <code>/etc/rpld.conf</code> and finally <code>rpld.conf</code> in the
directory where the <code>rpld</code> binary is in.</p>

<p class="block">Configuration files have a simple <code>KEY=VALUE</code>
syntax. Empty lines, lines beginning with a hash mark (<code>#</code>) or
unrecognized keys are ignored.</p>

<table border="1">
  <tr>
    <td class="t1"><code>DEVICE</code></td>
    <td class="t1">List of possible rpl devices, separated by space. The
      default is "<code>/dev/misc/rpl /dev/rpl</code>".</td>
  </tr>
  <tr>
    <td class="t2"><code>OFMT</code></code>

    <td class="t2"><p class="block"><code>OFMT</code> combines the directory to
    write the tty recordings into (relative to the working directory of
    <i>rpld</i>) and the name of the log files. Note that the working directory
    of <i>rpld</i> depends on where it was started from, e.g. with
    <code>startproc</code>, the CWD is mostly the root path (<code>/</code>). 
    It is wise to use absolute paths here. The following printf-style tags can
    be used in <code>OFMT</code>:</p>

    <p class="block"><code>%d</code>&nbsp;-- Time when tty was opened (usually
    when someone logs in or an xterm was opened). In <code>YYYYMMDD</code>
    format.</p>

    <p class="block"><code>%l</code>&nbsp;-- Terminal (line) the user logged
    on.  The string is taken from a string database. Typical strings are
    <code>tty*</code> for virtual consoles, <code>pts-*</code> for
    pseudo-terminals, <code>ttyS*</code> for serial lines. If there is no
    matching string entry, the device number is used in square brackets, e.g.
    <code>[240:0]</code>.</p>

    <p class="block"><code>%u</code>&nbsp;-- User who owned the tty when it was
    opened. If the device could not be <code>stat()</code>'ed, <code>%u</code>
    will be substituted by "<code>NONE</code>". If the UID could not be
    translated into a name, the UID is spit out instead.</p>

    <p class="block"><i>Directories will be created as needed</i> (if
    permission allows). Thus, you can have
    <code>/var/log/rpl/%u/%d.%t.%l</code>, and it will automatically create
    <code>%u</code>, provided that <code>/var/log/rpl</code> is writable for
    the user running <i>rpld</i>.</p></td>

  </tr>
  <tr>
    <td class="t1"><code>USER</code></p>
    <td class="t1">User to change to after all initialization. Make sure that
      the user can create files according to <code>OFMT</code>.</td>
  </tr>
  <tr>
    <td class="t2"><code>START_INFOD</code></p>
    <td class="t2">Starts the <i>infod</i> component.</td>
  </tr>
  <tr>
    <td class="t1"><code>INFOD_PORT</code></p>
    <td class="t1">Specify the path of the socket which <i>infod</i> provides
      for clients. The default value is
      <code>/var/run/.rplinfod_socket</code>.</td>
  </tr>
</table>

<h1>See also</h1>

<ul>
  <li><a href="ttyreplay.1.php"><code>ttyreplay(1)</code></a></li>
</ul>

<?php include_once("_footer.php"); ?>
