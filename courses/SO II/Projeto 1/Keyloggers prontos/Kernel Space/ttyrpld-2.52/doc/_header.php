<?php
$section_index = array(0, 0);
function h1($title) {
    global $section_index;
    $section_index[1] = 0;
    echo "<h1>", ++$section_index[0], " &nbsp; ",
         htmlspecialchars($title, ENT_NOQUOTES), "</h1>";
}
function h2($title) {
    global $section_index;
    echo "<h2>", $section_index[0], ".", ++$section_index[1], " &nbsp; ",
         htmlspecialchars($title, ENT_NOQUOTES), "</h2>";
}
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>ttyrpld - tty replay daemon</title>
<link rel="stylesheet" href="screen.css" type="text/css" title="Default" />
<link rel="alternate stylesheet" href="printer.css" type="text/css" media="screen" title="Print" />
<link rel="stylesheet" href="printer.css" type="text/css" media="print" title="Print" />
</head>

<body>

<div style="background-color: #FFF6D5; border-bottom: 1px solid #808080;">
<table style="margin: 0;">
  <tr>
    <td style="vertical-align: middle; padding: 0 1em 0 2em;" valign="middle"><a
      href="http://ttyrpld.sourceforge.net/"><img src="ttyrpld.png"
      alt="ttyrpld logo" border="0" /></a></td>
    <td style="vertical-align: middle; font-size: 150%;">ttyrpld&nbsp;-
      tty logging daemon</td>
  </tr>
</table></div>

<hr style="display: none;" />

<div id="content">
