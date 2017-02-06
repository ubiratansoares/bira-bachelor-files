<?php include_once("_header.php"); ?>

<h1>Description</h1>

<p class="block">There is a simple way on how to scoop <code>/dev/rpl</code>
data directly to another host without logging it on the one where it was
captured. This is commonly referred to as "network logging".</p>

<p class="block"><i>Note:</i> This does not work reliably (i.e. you get wrong
device names) when the major/minor numbers are different between two hosts!
Network logging is currently only of use between Linux where tty classes have
the same major number. Another constraint is that the tty node must also exist
on the destination system, which is not always the case with dynamic
<code>/dev</code> directories (either by devfs or udev).</p>

<h1>Setup</h1>

<p class="block">Beginning with <i>ttyrpld 2.00</i>, the timestamp is added in
kernelspace, so that you can directly <code>cat /dev/rpl</code> to a file and
process it <u>later</u>. The "later" was not possible before ttyrpld 2.00.
Therefore, the remote capture process has become extremely simple. Run this on
the capture host:</p>

<p class="code"><code>
# <b>ssh -Te none daemon@storage "rpld -D -" &lt;/dev/rpl</b>
</code></p>

<p class="block">You might want to use a different account name for ssh'ing
(because <code>daemon</code> is a system account and remote logins for this
"user" would look like an intrusion). I also suggest using SSH keys so that you
do not need to enter a password when you connect.</p>

<?php include_once("_footer.php"); ?>
