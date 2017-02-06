/* Executa tarefas de inicialização. */
function init() {
	document.newsurl = 'news.xml';
	document.lastUpdate = 0;
	loadRSS(document.newsurl);
	listenForNews();
	return;
}

/* Carrega um arquivo de notícias em RSS da URL dada. Requer 
 * implementação nativa do objeto XMLHttpRequest. Caso não atenda
 * a este requisito, a página exibe uma mensagem de erro ao invés
 * das notícias.
 * */
function loadRSS(url) {
	
	//cria uma mensagem vazia
	p = document.getElementById('msg');
	p.id = 'msg';
	
	if(window.XMLHttpRequest) {
		//carrega o arquivo rss (xml)
		p.innerHTML = 'Navegador OK.';
		request = new XMLHttpRequest();
		request.onreadystatechange = stateChanged;
		request.open('GET', url, true);
		request.send();
	} else {
		//exibe uma mensagem de incompatibilidade
		p.innerHTML = 'Desculpe, seu navegador não é suportado pela \
		nossa página web. Recomendamos atualizar seu navegador, ou \
		usar o navegador <a href=\"http://br.mozdev.org/\">\
		Mozilla Firefox</a>.';
	}
	
	return;
}

/* Atualiza a interface de acordo com o progresso da requsição
 * AJAX. Não leva em conta respostas de status HTTP.
 * */
function stateChanged() {
	p = document.getElementById('msg');
	switch(this.readyState) {
		case 2:
			//HEADERS_RECEIVED
			p.innerHTML = 'Servidor OK.';
			break;
			
		case 3:
			//LOADING
			p.innerHTML = 'Carregando...';
			break;
			
		case 4:
			//DONE
			p.innerHTML = 'Carregado.';
			displayNews(this.responseXML);
			break;
	}
	return;
}

/* Exibe as notícias contidas no documento XML/RSS. */
function displayNews(rssDoc) {
	
	//limpa a seção principal
	container = document.getElementById('body');
	p = document.getElementById('msg');
	p.style.display = 'none';
	
	//lista as notícias
	
	rss = rssDoc.getElementsByTagName('rss')[0];
	channel = rssDoc.getElementsByTagName('channel')[0]
	items = channel.getElementsByTagName('item');
	
	date = new Date(); //hora atual
	
	for(i = 0; i < items.length; i++) {

		//verifica se a notícia é nova
		pbdate = items[i].getElementsByTagName('pubDate');
		isNew = true;
		if(pbdate.length > 0) {
			pbdate = new Date(pbdate[0].textContent);
			isNew = pbdate.getTime() > document.lastUpdate;
		}
		
		if(isNew) {
			/* <div class="item">
			 *     <a class="altlink hide" href="#" onclick="hideItem()">[X]</a>
			 *     <h2><a class="altlink" href="${link}">${title}</a></h2>
			 *     <p class="gray">
			 *         <span class="source"><a class="altlink" href="${source@url}">${source}</a></span>
			 *         <span class="pbdate">${pubDate}</span>
			 *     </p>
			 *     <p>${description}</p>
			 * </div>
			 * */
			
			div = document.createElement('div');
			container.appendChild(div);
			div.className = 'item';
			
			//link de ocultar
			
			a = document.createElement('a');
			a.innerHTML = '[X]';
			a.className = 'altlink hide';
			a.setAttribute('href', '#');
			a.container = div;
			a.onclick = hideItem;
			div.appendChild(a);
			
			//título
			
			h2 = document.createElement('h2');
			div.appendChild(h2);
			a = document.createElement('a');
			h2.appendChild(a);
			
			title = items[i].getElementsByTagName('title')[0];
			a.innerHTML = title.textContent; //title
			
			nlink = items[i].getElementsByTagName('link')[0];
			a.setAttribute('href', nlink.textContent); //link
			a.className = 'altlink';

			p = document.createElement('p');
			p.className = 'gray';
			div.appendChild(p);
			
			//source (opcional)
			
			source = items[i].getElementsByTagName('source');
			if(source.length > 0) {
				source = source[0];
				
				span = document.createElement('span');
				span.className = 'source';
				p.appendChild(span);
				
				a = document.createElement('a');
				a.className = 'altlink';
				span.appendChild(a);
			
				a.innerHTML = source.textContent;
				a.setAttribute('href', source.getAttribute('url'));
			}
			
			//pubDate (opcional)
			pbdate = items[i].getElementsByTagName('pubDate');
			if(pbdate.length > 0) {
				pbdate = pbdate[0];
				
				span = document.createElement('span');
				span.className = 'pbdate';
				p.appendChild(span);
			
				pbdate = new Date(pbdate.textContent);
				span.innerHTML = timeText(date, pbdate);
			}
			
			//description
			
			p = document.createElement('p');
			div.appendChild(p);
			
			desc = items[i].getElementsByTagName('description')[0];
			p.innerHTML = desc.textContent;
		}
	}
	
	document.lastUpdate = date.getTime();
	return;
}

/* Obtém um texto descrevendo há quantas horas a hora de referência
 * ocorreu em relação à hora atual.
 * */
function timeText(currDate, refDate) {
	delta = currDate.getTime() - refDate.getTime();
	if(delta < 0) {
		text = 'ainda não publicado';
	} else {
		delta = new Date(delta);
		year = new Number(delta.getUTCFullYear());
		if(year < 1971) {
			if(delta.getUTCMonth() < 1) {
				days = delta.getUTCDate() - 1;
				if(days < 1) {
					text = timeText2(delta);
				} else if(days == 1)
					text = 'há 1 dia';
				else
					text = 'há ' + days + ' dias';
			} else if(delta.getUTCMonth() == 1)
				text = 'há 1 mês';
			else
				text = 'há ' + delta.getUTCMonth() + ' meses';
		} else if(year == 1971)
			text = 'há 1 ano';
		else
			text = 'há ' + (year - 1970) + ' anos';
	}
	return text;
}

/* Idem à primeira função, continuando a 
 * analisar a partir da precisão de horas.
 * */
function timeText2(delta) {
	if(delta.getUTCHours() < 1) {
		if(delta.getUTCMinutes() < 1) {
			text = 'há menos de 1 minuto';
		} else if(delta.getUTCMinutes() == 1)
			text = 'há 1 minuto';
		else
			text = 'há ' + delta.getUTCMinutes() + ' minutos';
	} else if(delta.getUTCHours() == 1)
		text = 'há 1 hora';
	else
		text = 'há ' + delta.getUTCHours() + ' horas';
	return text;
}

/* Remove o item da lista. */
function hideItem() {
	parent = this.container.parentNode;
	parent.removeChild(this.container);
	
	//exibe uma mensagem adequada
	//caso não haja mais notícias
	items = container.getElementsByTagName('div');
	if(items.length < 1) {
		p = document.getElementById('msg');
		p.innerHTML = 'Não há mais notícias.';
		p.style.display = 'block';
	}
	
	return;
}

/* Atualiza a página com novas notícias. */
function listenForNews() {
	setTimeout('updateNews();', 60 * 1000);
	return;
}

function updateNews() {
	loadRSS(document.newsurl);
	setTimeout('updateNews();', 60 * 1000);
	return;
}
