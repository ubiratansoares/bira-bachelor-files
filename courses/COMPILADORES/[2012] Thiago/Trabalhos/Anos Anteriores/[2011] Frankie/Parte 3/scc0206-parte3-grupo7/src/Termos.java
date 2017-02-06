

public class Termos {
    String t1;
    String t2;
    String sinal;


    Termos(){
        t1=null;
        t2=null;
        sinal=null;

    }

    public void set1(String t1){
        this.t1=t1;
    }

    public void set2(String t2){
        this.t2=t2;
    }

    public void setS(String sinal){
        this.sinal=sinal;
    }

    public String get1(){
        return this.t1;
    }

    public String get2(){
        return this.t2;
    }

    public String getS(){
        return this.sinal;
    }


}
