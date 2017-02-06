package controller;

import java.io.IOException;
import java.util.*;

import javax.servlet.ServletException;
import javax.servlet.http.*;

import util.HibernateUtil;

import model.Bank;
import model.Investor;
import model.Loan;
import model.Repayment;

/*
 * LoanManager.java
 * 
 * Gerenciador de empréstimos do sistema
 * 
 * */

public class LoanManager extends HttpServlet{
	
	/**
	 * serialVersionUID para essa classe
	 */
	private static final long serialVersionUID = 1L;
	
	public void doPost(HttpServletRequest request, HttpServletResponse response) 
		throws ServletException, IOException {
		try {
			
			 String bankID = request.getParameter("bankID");
			 String investorID = request.getParameter("investorID");
			 String amount = request.getParameter("amount");
			 
			 try {
				
				 Bank bank = (Bank) HibernateUtil.getSession().get(Bank.class, Integer.valueOf(bankID));
				 Investor investor = (Investor) HibernateUtil.getSession().get(Investor.class, Integer.valueOf(investorID));
				 double value = Double.valueOf(amount);
				 
				 HibernateUtil.getSession().beginTransaction();
				
				 this.createLoan(bank, investor, value);
				
				 HibernateUtil.getSession().getTransaction().commit();
			
			 	} catch(Exception e) {
				
				HibernateUtil.getSession().getTransaction().rollback();
				throw e;
			 }
			
		//response.sendRedirect(this.getServletContext().getContextPath());
			
		} catch(IOException e) {
			throw e;
			
		} catch(Exception e) {
			throw new ServletException(e);
		
		}		

		
	}// end doPost
	
	/**
	 * Criando um novo empréstimo
	 * Assumimos que apenas bancos podem emprestar dinheiro.
	 * 
	 * @param bank
	 * O banco que está emprestando dinheiro.
	 * 
	 * @param investor
	 * O investidor que está solicitando o empréstimo
	 * 
	 * @param amount
	 * A quantidade de dinheiro envolvida 
	 */

	public void createLoan(Bank bank, Investor investor, double amount){
		
		Loan loan = new Loan();
		
		loan.setValue(amount);
		loan.setInterest(bank.getInterest());
		loan.setAccount(investor);
		loan.setTimestamp(new Date());
		loan.setDischarged(false);
		loan.getInverse().setAccount(bank);
		
		this.updateInterest(bank, amount);
				
		investor.setBalance(investor.getBalance() + amount);
		bank.setBalance(investor.getBalance() + amount);

		
		HibernateUtil.getSession().save(loan);
			
	} //end createLoan

	/**
	 * Efetuando um pagamento relacionado a um pagamento existente.
	 * Um pagamento sobre um empréstimo em aberto pode ser de qualquer quantia.
	 * 
	 * Caso essa seja menor do que o montante do empréstimo, essa transação fica
	 * registrada no histórico do empréstimos e o novo valor passa a ser considerado
	 * desde então.
	 * 
	 * @param payment
	 * O banco que está emprestando dinheiro.
	 * 
	 * @param investor
	 * O investidor que está solicitando o empréstimo
	 * 
	 * @param loan
	 * A quantidade de dinheiro envolvida 
	 */
	
	public void performRepayment(double payment, Investor investor, Loan loan){
		
		Repayment r = new Repayment();
		
		r.setAccount(investor);
		r.setValue(payment);
		r.setLoan(loan);
		r.getInverse().setAccount(loan.getAccount());
		
		HibernateUtil.getSession().save(r);
		
		Bank b = (Bank) (loan.getAccount() instanceof Bank ? 
				loan.getAccount() : loan.getInverse().getAccount());
		
		this.updateInterest(b, payment);

		
	} //end performPayment
	
	/**
	 * Atualizando o juros de empréstimo de um banco
	 * 
	 * O juros de mercado de um banco é dependente do montante em 
	 * dinheiro esse tem emprestado ao mercado.
	 * 
	 * Dessa forma, uma função de ajuste é implementada e será utilizada
	 * a cada novo empréstimo ou pagamento que for computado. 
	 * 
	 * Essa é ajustada por um parâmetro alpha - no caso, a estratégia de mercado 
	 * do banco - de maneira que a política de juros é mais ou menos agressiva.
	 * 
	 * @param bank
	 * O banco que está emprestando dinheiro.
	 * 
	 * @param amount
	 * A quantidade de dinheiro do último empréstimo 
	 */
	
	public void updateInterest(Bank bank, double amount){
				
		double actualInterest = bank.getInterest();
		
		double alpha = bank.getStrategy();
	
		// Fórmula que ajusta o juros para o próximo empréstimo
		
		double newInterest = (1.0 - alpha) * actualInterest + amount * alpha;
		
		bank.setInterest(newInterest);
		
		HibernateUtil.getSession().save(bank);
		
	} //end updateInterest
	
	/**
	 * Atualizando empréstimo
	 * 
	 * Diariamente, cada empréstimo não quitado é atualizado segundo
	 * a taxa de juros acordada no momento de sua efetuação.
	 * 
	 * Esse método será chamado por um agente externo
	 * (Classe quartz?)
	 * 
	 * @param Loan
	 * O empréstimo a ser atualizado
	 */
	
	public void updateLoan(Loan loan){
		
		// A atualização mediante juros é feita diariamente
		
		double value = (1+ loan.getInterest()) * loan.getValue();
		
		loan.setValue(value);
		
		HibernateUtil.getSession().save(loan);

		
	} //end updateLoan
	


}
