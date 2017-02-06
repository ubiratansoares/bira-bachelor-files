
public class Campos {

    public String ident;
    public Integer nivel;
    public String categoria;
    public String tipo;
    public Integer ender;
    public Integer nbytes;
    public Integer valor;
    public Integer classetransf;
    public Integer npar;
    public Integer inf;
    public Integer sup;

    Campos() {
        ident = null;
        nivel = 0;
        categoria = null;
        tipo = null;
        ender = 0;
        nbytes = 0;
        valor = 0;
        classetransf = 0;
        npar = 0;
        inf = 0;
        sup = 0;
    }

    public void setIdent(String ident) {
        this.ident = ident;
    }

    public String getIdent() {
        return ident;
    }

    public void setNivel(Integer nivel) {
        this.nivel = nivel;
    }

    public Integer getNivel() {
        return nivel;
    }

    public void setCategoria(String categoria) {
        this.categoria = categoria;
    }

    public String getCategoria() {
        return categoria;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public String getTipo() {
        return tipo;
    }

    public void setEnder(Integer ender) {
        this.ender = ender;
    }

    public Integer getEnder() {
        return ender;
    }

    public void setNbytes(Integer nbytes) {
        this.nbytes = nbytes;
    }

    public Integer getNbytes() {
        return nbytes;
    }

    public void setValor(Integer valor) {
        this.valor = valor;
    }

    public Integer getValor() {
        return valor;
    }

    public void setClasseTransf(Integer cf) {
        this.classetransf = cf;
    }

    public Integer getClasseTransf() {
        return classetransf;
    }

    public void setNpar(Integer npar) {
        this.npar = npar;
    }

    public Integer getNpar() {
        return npar;
    }

    public void setInf(Integer inf) {
        this.inf = inf;
    }

    public Integer getInf() {
        return inf;
    }

    public void setSup(Integer sup) {
        this.sup = sup;
    }

    public Integer getSup() {
        return sup;
    }
}
