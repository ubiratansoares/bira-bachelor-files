

import java.util.LinkedList;
import java.util.List;

public class TS {

    public List<Campos> ts;

    //construtor já irá armazenar os identificadores pré-definidos na TS
    TS(){
        ts = new LinkedList<Campos>();
        Campos campos = new Campos();

        //adicionando int
        campos.setIdent("int");
        campos.setNivel(0);
        campos.setCategoria("tipo");
        campos.setTipo("int");
        campos.setNbytes(2);
        campos.setEnder(ts.size());
        ts.add(campos);
        //adicionando bool
        campos = new Campos();
        campos.setIdent("boolean");
        campos.setNivel(0);
        campos.setCategoria("tipo");
        campos.setTipo("boolean");
        campos.setNbytes(1);
        campos.setEnder(ts.size());
        ts.add(campos);
        //adicionando true
        campos = new Campos();
        campos.setIdent("true");
        campos.setNivel(0);
        campos.setCategoria("const");
        campos.setTipo("boolean");
        campos.setValor(1);
        campos.setEnder(ts.size());
        ts.add(campos);
        //adicionando false
        campos = new Campos();
        campos.setIdent("false");
        campos.setNivel(0);
        campos.setCategoria("const");
        campos.setTipo("boolean");
        campos.setValor(0);
        campos.setEnder(ts.size());
        ts.add(campos);
        //adicionando read
        campos = new Campos();
        campos.setIdent("read");
        campos.setNivel(0);
        campos.setCategoria("proc");
        campos.setEnder(ts.size());
        ts.add(campos);
        //adicionando write
        campos = new Campos();
        campos.setIdent("write");
        campos.setNivel(0);
        campos.setCategoria("proc");
        campos.setEnder(ts.size());
        ts.add(campos);
				//adicionando union
				campos = new Campos();
				campos.setIdent("union");
				campos.setNivel(0);
				campos.setCategoria("tipo");
				campos.setEnder(ts.size());
				ts.add(campos);
    }


    //busca por id e retorna o endereço e declarado caso o encontre
    public descritor busca(String id) {
        descritor d = new descritor();
        for (int i = 0; i < ts.size(); i++)
            if (ts.get(i).ident.contentEquals(id)) {
               d.achou=true;
               d.ender=i;
               return d;
            }
        d.achou=false;
        d.ender=-1;
        return d;
    }

      //elimina todos os id que estão num dado nível K (escopo)
    public void elimina(int K, int pos) {
        for (int i=0;i<ts.size();i++)
            if (ts.get(i).nivel==K)
/*


				// faz a checagem se a linha a ser removida corresponde a um parametro de procedimento
				// se for, guarda em uma cuja cabeça está apontada em um campo da entrada corresponde
				// ao procedimento

				if(ts.get(i).categoria.contentEquals("par")){

					ts.get(pos).listaParametros.add(i);
					ts.get(i).ident = null;

				}

                else */ ts.remove(i);
    }

    //insere id na TS; retorna um ponteiro para a entrada e um flag para indicar se o nome já estava presente
    public descritor insere(String id) {
        descritor d = new descritor();
        d = busca(id); //verifico se o identificador ja foi declarado
 
        Campos campos = new Campos(); //objeto com os 11 campos da TS
        campos.setIdent(id);  //define campo identificador com id
        campos.setEnder(ts.size()); //define endereco da entrada
        ts.add(campos); //adiciona na tabela de simbolos
        d.ender=ts.size(); 
        return d; //retorno a posicao do novo id e se ele ja estava presente
    }

    //verifica se o id está declarado no nível K (corrente)}
    public boolean declarado(String id, int k) {
        for (int i = 0; i < ts.size(); i++) { 
            if (ts.get(i).ident.contentEquals(id) && ts.get(i).nivel == k) { 
                return true;																				
            }
        }
        return false;
    }

    //Seta_atributos (nivel, categoria e tipo) no endereço passado
    public void setaAtributos(int nivel, String categoria, String tipo, int ender) {
        ts.get(ender).setNivel(nivel);
        ts.get(ender).setCategoria(categoria);
        ts.get(ender).setTipo(tipo);

    }

    //atualiza o número de parâmetros no endereço passado
    public void setaAtributos(int ender,int npar) {
        ts.get(ender).npar=npar;
    }

	  //define o tipo de uma lista de parametros
    public void setaAtributos(String tipo) {
        for (int i = 0; i < ts.size(); i++) { //percorre a tabela
            if (ts.get(i).tipo == null && ts.get(i).categoria!="proc") { //se nao a categoria nao for proc
                ts.get(i).tipo = tipo;																	 //e o tipo estiver null
            }																														 //atualiza lista de parametros
        }

    }

    //atualiza a lista de identificadores que estão sem categoria definida
    public void setaAtributos(int nivel, String categoria, String tipo) {
        for (int i = 0; i < ts.size(); i++) { //percorre toda a tabela
            if (ts.get(i).categoria == null) { //se alguma categoria estiver como null
                ts.get(i).setNivel(nivel);     //atualiza valores
                ts.get(i).setCategoria(categoria);
                ts.get(i).setTipo(tipo);
            }
        }
    }

    //obtem categoria atraves de endereco
    public String obtemAtributosCat(int ender){
        return ts.get(ender).categoria;
    }

		//obtem tipo de variavel atraves de endereco
    public String obtemAtributosTipoVar(int ender){
        return ts.get(ender).tipo;
    }

		//obtem numero de parametros atraves de endereco
    public int obtemAtributosNPar(int ender) {
        return ts.get(ender).npar;
    }

	  //obtem quantidade de elementos na tabela de simbolos
    public Integer size(){
        return ts.size()-1;
    }

		//metodo usado para debug de codigo
    public void imprimeTS() {
        for (int i = 0; i < ts.size(); i++) {
            System.out.println(i+ " "+ts.get(i).getEnder() + " "+
                    ts.get(i).getIdent()+ " "+
                    ts.get(i).getNivel()+ " "+
                    ts.get(i).getCategoria()+ " "+
                    ts.get(i).getTipo());
        }
    }
}
