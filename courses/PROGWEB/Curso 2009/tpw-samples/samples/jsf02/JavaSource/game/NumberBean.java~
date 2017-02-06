package game;

import javax.faces.context.FacesContext;
import javax.servlet.http.HttpSession;
import javax.faces.application.FacesMessage;
import java.util.ResourceBundle;

public class NumberBean {

  Integer userNumber; 
  int randomNumber ; 

  public NumberBean () {
    randomNumber = (int)(Math.random()*100);
    System.out.println ( "Random number: "+randomNumber);
  }

  public void setUserNumber (Integer value){ this.userNumber = value; }
  public Integer getUserNumber () { return this.userNumber; } 

  // invalida a sessao atual
  public String playagain () {
    FacesContext context = FacesContext.getCurrentInstance();
    HttpSession session = (HttpSession) context.getExternalContext().getSession(false);
    session.invalidate();
    return "playagain"; 
  }

  // checa o numero escolhido pelo usuario
  public String checkGuess () {
     
    if ( userNumber.intValue() == randomNumber ) {
      return "success";
    } else {
      // Pegando referencia para arquivo de mensagens
      FacesContext context = FacesContext.getCurrentInstance();
      ResourceBundle bundle =
                ResourceBundle.getBundle("game.messages",
                    		context.getViewRoot().getLocale());

      System.out.println("Locale: "+context.getViewRoot().getLocale());

      String msg = "";

      // se o numero for maior
      if ( userNumber.intValue() > randomNumber ) 
         msg = bundle.getString("tryagain_smaller");
      else // se o numero for menor
         msg = bundle.getString("tryagain_bigger");
      
      // addicionando a mensagem para ser apresentada <h:messages>
      context.addMessage (null, new FacesMessage(msg)); 

      // null indica a apresentacao da mesma pagina
      return null;
    }
  }
}
