#include "Gerente.h"
#include "Diretor.h"
#include "GerenteDiretor.h"

int main(int argc, char *argv[]) {
	GerenteDiretor *gd = new GerenteDiretor();
	gd->imprime();

	return 0;
}
