package controller;

import java.io.IOException;
import java.util.*;

import javax.servlet.ServletException;
import javax.servlet.http.*;

import util.HibernateUtil;

import model.Company;
import model.Investment;
import model.Quotation;
import model.Investor;
import model.Stock;

public class InvestimentManager extends HttpServlet{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public void doPost(HttpServletRequest request, HttpServletResponse response) 
		throws ServletException, IOException {
		try {
		
		 String companyID = request.getParameter("companyID");
		 String investorID = request.getParameter("investorID");
		 String amount = request.getParameter("amount");
		 
		 try {
			
			 
			 Company c = (Company) HibernateUtil.getSession().get(Company.class, Integer.valueOf(companyID));
			 Investor i = (Investor) HibernateUtil.getSession().get(Investor.class, Integer.valueOf(investorID));
			 Integer value = Integer.valueOf(amount);
			 
			 HibernateUtil.getSession().beginTransaction();
		
			 // Descobrindo se o usuário já tem investimento com a aquela companhia
			 
			Stock s = (Stock) HibernateUtil.getSession().createQuery(
				"FROM Stock WHERE owner = :owner AND company = :company ").
				setParameter("owner", i).setParameter("company",c).uniqueResult();
		
			if(s == null) this.createInvestment(i,value,c);
			
			 else this.updateInvestment(i, value, c);
			 
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
	 * Criando um novo investimento
	 * 
	 * @param account
	 * O investidor que irá comprar ações
	 * 
	 * @param amount
	 * A quantidade de papéis de uma empresa que o investidor irá adquirir
	 * 
	 * @param company
	 * A companhia na qual o investidor irá investir 
	 */
	
	
	public void createInvestment(Investor account, Integer amount, Company company){
		
		Investment i = new Investment();
		
		i.setAccount(account);
		i.setAmount(amount);
		
		Quotation q = new Quotation();
		q.setCompany(company);
		q.setTimestamp(new Date());
		
		List<Quotation> l = company.getQuotations();
		
		int k;
		
		for(k = 0; k < l.size(); k++)
			if(l.get(k) == q) break;
		
		if(k == l.size()) throw new IllegalArgumentException();
		
		else{
			
			q.setValue(l.get(k).getValue());
		
			i.setPurchase(q);
			i.getInverse().setAccount(account);
			i.setTimestamp(q.getTimestamp());
			
			// Atualizando a carteira de ações do investidor
			
			try{
				Stock s = (Stock) HibernateUtil.getSession().createQuery(
						"FROM Stock WHERE owner = :owner AND company = :company ").
						setParameter("owner", i).setParameter("company",company).uniqueResult();
				
				if(s != null) s.setAmount(s.getAmount() + amount);
				
			}
			catch(Exception e){
				
				
			}
			
			HibernateUtil.getSession().save(i);

		}
		
	}// end createInvestment
	
	public void updateInvestment(Investor account, Integer amount, Company company){
			
		try{
		
			Stock s = (Stock) HibernateUtil.getSession().createQuery(
						"FROM Stock WHERE owner = :owner AND company = :company ").
						setParameter("owner", account).setParameter("company",company).uniqueResult();
				
				if(s != null) s.setAmount(s.getAmount() + amount);
				
			}
			
			catch(Exception e){ }
			

		
	}// end createInvestment


} //end InvestimentManager
