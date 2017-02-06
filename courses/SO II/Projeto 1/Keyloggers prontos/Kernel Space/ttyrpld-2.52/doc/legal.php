<?php include_once("_header.php"); ?>

<h1>Legal notice</h1>

<p class="block">As you might have guessed, logging is not without problems.
Especially <i>the usage</i> and <i>appliance</i> of this software might be
limited by local law. (The software itself is, of course, not affected.)</p>

<ol>

  <li>Local law might not allow monitoring users at all.</li>

  <li>Local law might allow monitoring, to some kind of degree</li>

  <li>If monitoring is allowed, you may need to notice your users about
  this</li>

  <li>Different kinds of environments have different monitoring restrictions,
  e.g. home, within companies, educational and/or governmental institutions, a
  company's shell services</li>

  <li>Monitoring is usually limited (maybe not by law, but through contracts)
  if the users are allowed to do private things</li>

  <li>Monitoring even private data <i>might</i> be granted (not necessarily!)
  if you (as sysadmin) can guarantee that any data captured whatsoever is only
  ever used in cases of abuse or suspected abuse.</li>

</ol>

<p class="block">As far as I could research about this, it seems like only
Germany has a Data Privacy law (<i>Datenschutzgesetz</i>) which prohibits
monitoring if anything <i>private</i> is involved. (This means, if e.g. a
computer at work may only used for business stuff, your boss is entitled to
check whether you fulfill your work.)</p>

<p class="block">Please consult a lawyer if you are uncertain about your
status.</p>

<h1>Use it</h1>

<p class="block">Despite all this limiting, there are reasons to actively use
it in a production system.</p>

<ul>

  <li><b>Intruders</b>. There have been several recent comprimises, e.g. at FSF
  (November 02 2003), Debian (November 19 2003) and Gentoo (December 08 2003,
  <a href="http://www.gentoo.org/news/en/gwn/20031208-newsletter.xml"
  target="_blank">http://www.gentoo.org/news/en/gwn/20031208-newsletter.xml</a>)</li>

  <li><b>Limited usage</b>. ISPs could use this as a usage tracker for e.g. a
  paid shell service. In this case, no data is captured, but only bytes
  counted.</li>

  <li><b>Cooperative usage</b>. Multiple administrators are working on the
  system and either do not notify the others of their changes, or make
  decisions that are later regarded as wrong by the group and they try to
  reverse it.</li>

  <li><b>Post-job tracking</b>. I have seen a lot of places where users had to
  hand over [superuser or normal] permissions to a friend or even stranger on
  the internet just because of the complexity of a certain task, or because of
  certain data not available to do it yourself. By doing so, you give away some
  responsibility and as such, are able to hold others resposible for making
  errors. <i>ttyrpld</i> gives you the proof for lawsuits.</p>

</ul>

<?php include_once("_footer.php"); ?>
