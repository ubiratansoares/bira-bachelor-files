<?php

header('Content-type: text/xml; charset=utf-8');


if ( isset($_GET['loadfeed']) ){
	
	if ( $_GET['loadfeed'] != "" ){
		
		// Obtendo dados da Internet
		$feed = "http://" . $_GET['loadfeed'];
		
		// Checando o Encoding
		$data = trim( file_get_contents(htmlspecialchars_decode($feed)) );

		// Carregando o feed
		$xml = simplexml_load_string($data);

		// Em caso de falha
		if ( !$xml ){
			echo "Error Loading Feed: ".htmlentities($_GET[loadfeed])."<br>";
			return;
		}


		$i = 1;
		
		// Mostrando a imagem que encabeça o feed 

		if ( $xml->channel->image->url != "" ){
			
			$url = $xml->channel->image->url;
			$title = $xml->channel->image->title;
			echo "<img src=\"$url\" alt=\"$title\" style=\"margin-bottom: 20px; margin-top: 20px; display:block; margin-left: auto; 			margin-right: auto; \">";
		}
		else echo "</br></br></br>";
		
		// Mostrando os feeds
			
		foreach($xml->xpath('//item') as $item){
			
			
			//echo "<div>";
			//echo "<div class=\"rss_light\">";

			// Decidindo se o feed é par ou ímpar para efeitos de layout
  		
			if($i%2) {echo "<div class=\"rss_light\">";}
			
			else {echo "<div class=\"rss_dark\">";}
			
			$i++;
						
			// Mostrando o link
			echo "<a class=\"rss_link\" href=\"$item->link\" target=\"_blank\">";
			
			// Mostrando o título
			echo "<font class=\"rss_title\">$item->title</font>";
			echo "</a>"; // end the link

			// Link para o topo
			//echo "&nbsp;&nbsp;<a class=\"top\" href=\"#top\">Ir para o topo</a>";
			
			echo "<font class=\"rss_date\">$item->pubDate</font>";
			
			// Mostrando a descrição segundo decisão do usuário
			if ( isset($_GET['fulltext']) && $_GET['fulltext'] == "true" ){
				
				if ( isset($_GET['showimages']) && $_GET['showimages'] == "false" ){
					
					$description = strip_tags($item->description,'<a>');
					echo "<font class=\"rss_description\">$description</font><br>";
				}
				else echo "<br><font class=\"rss_description\">$item->description</font>";
			}
				
			echo "<hr size=\"1\" style=\"color:gray\"></div>";

		}//end foreach

		// A imagem do feed também vai embaixo da página

		if ( $xml->channel->image->url != "" ){
				
			$url = $xml->channel->image->url;
			$title = $xml->channel->image->title;
			echo "<img src=\"$url\" alt=\"$title\" style=\"margin-bottom: 20px; margin-top: 20px; display:block; margin-left: auto;
		    margin-right: auto; \">";
		}
	}		
}

?>