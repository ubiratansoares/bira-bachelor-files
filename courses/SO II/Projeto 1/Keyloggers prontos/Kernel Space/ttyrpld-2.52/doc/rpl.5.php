<?php include_once("_header.php"); ?>

<h1>ttyrpld log file format</h1>

<p class="block">The logfiles <i>rpld</i> creates are a small variation of the
packet protocol as described in <a href="rpldev.4.php">rpldev(4)</a>. The
<code>-&gt;dev</code> field is not present. (Since ttyrpld v2.00, the timestamp
is already added in the kernel.) All fields are little endian and packed, i.e.
there are no alignment gaps. The structure is similar to the <code>struct
rpldev_packet</code>:</p>

<p class="code"><code>
<b>struct</b> rpldsk_packet {<br />
&nbsp; &nbsp; <b>uint16_t</b> size;<br />
&nbsp; &nbsp; <b>uint8_t</b> event, magic;<br />
&nbsp; &nbsp; <b>struct</b> rpltime time;<br />
} <i>__attribute__((packed))</i>;</code></p>

<p class="block">The <code>-&gt;magic</code> is always <code>0xEE</code>.</p>
Possible values for <code>-&gt;event</code> equal those listed in <a
href="rpldev.4.php">rpldev(4)</a>, plus the following:</p>

<p class="code"><code>
<b>enum</b> {<br />
&nbsp; &nbsp; EVT_MAGIC <b>=</b> 0x4A,<br />
&nbsp; &nbsp; EVT_ID_PROG <b>=</b> 0xF0,<br />
&nbsp; &nbsp; EVT_ID_DEVPATH <b>=</b> 0xF1,<br />
&nbsp; &nbsp; EVT_ID_TIME <b>=</b> 0xF2,<br />
&nbsp; &nbsp; EVT_ID_USER <b>=</b> 0xF3,<br />
};</code></p>

<p class="block">A <code>EVT_ID_PROG</code> is added to each logfile by
<i>rpld</i> to contain the program with which it was created. (<i>rpld</i> in
this case&nbsp;-- but anyone is free to write a different daemon.)</p>

<p class="block">A <code>EVT_ID_DEVPATH</code> contains the path of the device
node that has been tapped. There is a special handling case in <i>rpld</i> when
multiple device nodes with the same major-minor number are used at the same
time:</p>

<ul>
  <li>if the first packet <i>rpld</i> picks up for a certain
    <code>-&gt;dev</code> line is a <code>EVT_INIT</code> or
    <code>EVT_OPEN</code> packet with a dentry name (e.g. 
    <code>/dev/tty1</code>), this name will be used for this major-minor
    number</li>
  <li><i>rpld</i> will look into <code>/dev</code> and take the first pick</li>
</ul>

<p class="block">In the default case, you should not worry, as each major-minor
number only has one node in <code>/dev</code>, and even if it does not, you are
able to know what tty was logged, because the dentries are similarly-named.</p>

<p class="block"><code>EVT_ID_TIME</code> packets carry the time the log was
started. This is useful because the logfile's timestamp on the filesystem may
change due to user interaction.</p>

<p class="block">Last, but not least, <code>EVT_ID_USER</code> contains the
username (or UID if no user was found) that was being traced. Especially useful
when users get removed after the logs have been recorded.</p>

<p class="block">All of these four packets are informational ones and are not
required for proper replay with <i>ttyreplay</i>.</p>

<h1>See also</h1>

<ul>
  <li><a href="rpldev.4.php"><code>rpldev(4)</code></a></li>
</ul>

<?php include_once("_footer.php"); ?>
