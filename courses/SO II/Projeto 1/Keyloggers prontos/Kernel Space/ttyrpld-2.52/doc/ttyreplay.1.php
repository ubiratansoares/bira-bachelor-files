<?php include_once("_header.php"); ?>

<h1>ttyreplay - realtime log file player</h1>

<p><code>ttyreplay <b>[</b>--no-pctrl<b>] [</b>-F<b>|</b>-f<b>] [</b>-J
<i>time</i><b>|</b>-j <i>packets</i><b>] [</b>-S <i>factor</i><b>] [</b>-T<b>]
[</b>-m <i>msec</i><b>] [</b>-t<b>] [</b>-?<b>|</b>--help<b>]</b></code></p>

<h1>Description</h1>

<p class="block"><i>ttyreplay</i> is the tool to see&nbsp;-- to replay&nbsp;--
what has been captured by (the kernel module and) <i>rpld</i>. Give it any
number of files you want to see again. Options apply to all files.</p>

<p class="block">Without any arguments, <i>ttyreplay</i> will try to play STDIN
if it is not a tty. The special file "<code>-</code>" (so-called <i>lone
dash</i>) can be used to indicate STDIN, if STDIN is not the only file to
play.</p>

<h1>Options</h1>

<table border="1">
  <tr>
    <td class="t1n"><code>--no-pctrl</code></td>
    <td class="t1">Disable play control. When play control is enabled, you can
      pause replaying, skip forwards, raise/lower speed. See below for
      more.</p>
  </tr>
  <tr>
    <td class="t2n"><code>-F</code></td>
    <td class="t2">Live feed follow mode. Seek to the end of the file and
      tail-follow any new contents that are written to it. This does the same
      as <code>tail -f</code> does with normal text logfiles.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-J <i>time</i></code></td>
    <td class="t1">Skim to <i>time</i> position before starting to play.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-S <i>factor</i></code></td>
    <td class="t2">Plays the file back with a time warp of <i>factor</i>.
      <code>1.0</code> is normal time, <code>2.0</code> is twice as fast,
      <code>0.5</code> half speed, etc.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-T</code></td>
    <td class="t1">Shows (replays) the log without any delays. This can be used
      to generate a screen log like <i>script</i> or <i>screen</i> (rpl logfile
      without any packet headers, basically) for easy grepping.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-f</code></td>
    <td class="t2">Catch-up follow mode. Plays the file back from the start as
      usual and switches into <code>-F</code> mode when EOF is reached.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-j <i>count</i></code></td>
    <td class="t1">Skim <i>count</i> packets before starting to play.</td>
  </tr>
  <tr>
    <td class="t2n"><code>-m <i>msec</i></code></td>
    <td class="t2">Set the maximum delay in milliseconds that will be executed
      between packets. This check is done after the <code>-S</code> option, so
      <code>-m1000 -S2</code> will first crunch a delay request (from the
      logfile) by factor 2, and then see if it is more than 1 second.</td>
  </tr>
  <tr>
    <td class="t1n"><code>-t</code></td>
    <td class="t1">Shows the timestamp at the top right corner. Note that this
      might not work with all terminals, because for one, not all support it,
      and second, the escape code is currently hardcoded (no terminfo use), so
      it may break on some strange terminal types.</p>

      <p class="block"><code>-t</code> will use "<code>%H:%M:%S</code>" as the
      format, while <code>-tt</code> (or <code>-t -t</code>) uses "<code>%d.%m
      %H:%M</code>". The latter is useful for users on a tty which have been
      logged in across a day boundary.</p>

    </td>
  </tr>
</table>

<h1>Player control</h1>

<p class="block">While <i>ttyreplay</i> is running, you can use the following
keys to interactively instruct <i>ttyreplay</i> to do something:</p>

<table>
  <tr>
    <td class="t1">(spacebar)</td>
    <td class="t1">Toggle pause</td>
  </tr>
  <tr>
    <td class="t2" nowrap="nowrap"><code>&lt;</code> (less than)</td>
    <td class="t2">Move to the previous file. Note that this might not work if
      the previous one was a pipe, STDIN or similar, because such have been
      emptied already.</td>
  </tr>
  <tr>
    <td class="t1" nowrap="nowrap"><code>&gt;</code> (greater than)</td>
    <td class="t1">Move to the next file</td>
  </tr>
  <tr>
    <td class="t2" nowrap="nowrap"><code>y</code> or <code>z</code></td>
    <td class="t2">Multiply playing speed by factor 0.75</td>
  </tr>
  <tr>
    <td class="t1"><code>x</code></td>
    <td class="t1">Normal playing speed</td>
  </tr>
  <tr>
    <td class="t2"><code>c</code></td>
    <td class="t2">Multiply playing speed by factor 1.33</td>
  </tr>
  <tr>
    <td class="t1"><code>[</code></td>
    <td class="t1">Decrease playing speed by 0.10</td>
  </tr>
  <tr>
    <td class="t2"><code>]</code></td>
    <td class="t2">Increase playing speed by 0.10</td>
  </tr>
  <tr>
    <td class="t1">(keypad) <code>6</code></td>
    <td class="t1">Skip forward 10 seconds</td>
  </tr>
  <tr>
    <td class="t2">(keypad) <code>9</code></td>
    <td class="t2">Skip forward 60 seconds</td>
  </tr>
  <tr>
    <td class="t1" nowrap="nowrap"><code>q</code> or <code>Ctrl+C</code></td>
    <td class="t1">Quit</td>
  </tr>
  <tr>
    <td class="t2"><code>e</code></td>
    <td class="t2">Toggle ECHO mode, i.e. show <code>EVT_READ</code> packets
      (keypresses) as well, rather than just screen output. This is useful for
      echo-less (mostly password) prompts.</td>
  </tr>
</table>

<p class="block">Skipping backwards is not implemented and is even impossible
on pipes.</p>

<h1>See also</h1>

<ul>
  <li><a href="rpl.5.php"><code>rpl(5)</code></li>
  <li><a href="rpld.8.php"><code>rpld(8)</code></li>
</ul>

<?php include_once("_footer.php"); ?>
