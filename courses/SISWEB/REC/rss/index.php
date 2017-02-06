<link rel="icon" type="image/png" href="include/feed-icon-red.png">

<style language='text/css'>
 
 @import url('include/style.css');

</style>

<script language="javascript">

var XMLHttpRequestObject = false;

var img = new Image();
img.src = "include/loading.gif";
    
if ( window.XMLHttpRequest ) XMLHttpRequestObject = new XMLHttpRequest();

else if (window.ActiveXObject) XMLHttpRequestObject = new ActiveXObject("Microsoft.XMLHTTP");

function changeOptions(){
	LoadFeed();
}

	
function LoadFeed(value, initial){

	if ( XMLHttpRequestObject ){
	
    	var dsource = "include/ajax.php?loadfeed"; 

		if (value) dsource += "=" + value;
	
		else{
				
			var val = document.getElementById("rss_feed").value;
				
			if ( val == "" ) document.getElementById("rss_feed").selectedIndex += 1;

			dsource += "=" + document.getElementById("rss_feed").value;
		}

		dsource += "&fulltext=" + document.getElementById("showtext").checked;

		dsource += "&showimages=" + document.getElementById("showimages").checked;	
								
    	XMLHttpRequestObject.open("GET",dsource);

    	XMLHttpRequestObject.onreadystatechange = function(){
	
			if ( XMLHttpRequestObject.readyState != 4 ){
				document.getElementById("rss_div").innerHTML = "<center><img class=\"loading\" src=\"" + img.src + "\"></center>";	
			}
				
       		if ( XMLHttpRequestObject.readyState == 4 && XMLHttpRequestObject.status == 200 ){
				document.getElementById("rss_div").innerHTML = XMLHttpRequestObject.responseText;
        	}
    	}//end function

    	XMLHttpRequestObject.send(null);

		//listenForNews();

		InitializeTimer();
		
	}//end if		

}//end LoadFeed


// Funções para temporização. Atualizam o feed na página a cada 30s. Desincronização após várias chamadas.

/*
function listenForNews() {

	setTimeout('updateNews();', 30 * 1000);
	return;
}

function updateNews() {
	LoadFeed();
	setTimeout('updateNews();', 60 * 1000);
	return;
}
*/

var secs
var timerID = null
var timerRunning = false
var delay = 1000

function InitializeTimer(){
	
    // Ajusta o tempo em segundos entre atualizações
    secs = 10;
    StopTheClock();
    StartTheTimer();
}

function StopTheClock(){
	
    if(timerRunning) clearTimeout(timerID);
    timerRunning = false;
}

function StartTheTimer(){
	
    if (secs==0){
        StopTheClock();
 		LoadFeed();
   }
   else{
        self.status = secs;
        secs = secs - 1;
        timerRunning = true;
        timerID = self.setTimeout("StartTheTimer()", delay);
    }
}

</script>

<?php

$feeds_xml="feeds.xml";

function LoadFile($xml_file)
{
	$xml = null;
	if ( !file_exists($xml_file) || !($xml = simplexml_load_file($xml_file)) )
	{
		echo '</select>';
		echo "Error opening XML feeds file: $xml_file<br>";
		return;
	}
	
	return $xml;
}

function ShowFeedOptions($xml_file)
{		
	echo '<select id="rss_feed" onChange="LoadFeed(this.value)" title="Bira RSS Reader">';
	
	$xml = LoadFile($xml_file);
	
	if ( $xml == null ) return;
	
	$url = array();
	
	foreach($xml->feeds->category as $category){
		
		echo "<option value=\"\">" . $category['name'] . "</option>";
		
		foreach($category->feed as $feed){
			
			if ( $feed['name'] != "" ){
				
			// Montando a caixa de escolha
			$name = "&nbsp;&nbsp;&nbsp;&nbsp;" . $feed['name'];
			$url = str_replace("&", "%26", $feed['url']);
			$url = str_replace("http://", "", htmlspecialchars($url));
			echo "<option value=\"$url\">" . $name . "</option>";
			}
		}
	}
	
	echo '</select>';
	
	$options = array();
	
	$rss = $xml->options[0];

	// Escolhas do usuário para imagem e conteúdo

	if ( strcasecmp($rss['fulltext'],"true") == 0 ) array_push($options, "true");
	else array_push($options, "false");
		
	if ( strcasecmp($rss['images'],"true") == 0 ) array_push($options, "true");
	else array_push($options, "false");

	return $options;
}

function SetOptions($options)
{
	if ( sizeOf($options) == 2 )
	{
		echo "<script>document.getElementById(\"showtext\").checked = " . $options[0] . ";</script>";
		echo "<script>document.getElementById(\"showimages\").checked = " . $options[1] . ";</script>";
	}
}

if ( isset($_GET['links']) ){
	
	$xml = LoadFile($xml_file);
	if ( $xml == null )
		return;
	return;
}

?>

<!--Montando a formatação principal da página -->
	
<div class="container" id="container_div">
	
	<a name="top"></a>

	<div class="header_div">

		<div style="display:run-in;float:left;">

		<font class="graytext">RSS Feed:</font>
			
		<?php $options = ShowFeedOptions($feeds_xml); ?>
			
		<input type="checkbox" id="showtext" onChange="changeOptions(this)" checked>
			
		<font class="graytext"> Mostrar Texto Completo</font>
			
		<input type="checkbox" id="showimages" onChange="changeOptions(this)" checked>
			
		<font class="graytext">Mostrar Imagens</font>
			
	</div>
			
	<?php SetOptions($options); ?>
		
</div>

<div class="rss" id="rss_div">			
	<script> LoadFeed(document.getElementById('rss_feed').value, true); </script>
</div>
		
