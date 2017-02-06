#include <iostream>

using namespace std;

class Televisao {
	private:
		char *modelo;

	protected:
		int volumeAtual;
		int volumeMaximo;

	public:
		Televisao(char *modelo, int volumeMaximo) {
			this->modelo = new char[strlen(modelo)+1];
			strcpy(this->modelo, modelo);
			this->volumeMaximo = volumeMaximo;
			this->volumeAtual = 0;
		}
		void print() {
			cout << "Modelo: " << this->modelo << endl;
			cout << "Volume Atual: " << this->volumeAtual << endl;
			cout << "Volume Máximo: " << this->volumeMaximo << endl;
		}
		virtual void mudarVolume(int value) {}
		void maisVolume() {
			mudarVolume(1);
		}
		void menosVolume() {
			mudarVolume(-1);
		}
};

class DiamondTV: public Televisao {
	public:
		DiamondTV(char *modelo, int volumeMaximo) : 
			Televisao(modelo, volumeMaximo) {
		}
		void mudarVolume(int value) {
			// cada marca de TV tem suas próprias instruções para
			// implementar o método. 
			// É muito similar à situação real, onde nós somente nos
			// preocupamos com o botão para aumentar e diminuir o
			// volume, e nem sabemos como a função foi implementada
			if ((this->volumeAtual + value < this->volumeMaximo) &&
    			    (this->volumeAtual + value > 0)) {
				this->volumeAtual += value;
			    }
		}
};
