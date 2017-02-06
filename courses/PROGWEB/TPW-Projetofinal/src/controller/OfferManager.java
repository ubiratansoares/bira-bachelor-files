package controller;

import java.io.IOException;
import java.lang.*;
import java.util.*;

import javax.servlet.ServletException;
import javax.servlet.http.*;

import model.*;
import model.Offer.Type;
import util.HibernateUtil;

public class OfferManager extends HttpServlet {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException, RuntimeException {

		try {
			// Dados que vêem da view
			// String companyID = request.getParameter("companyID");
			// String investorID = request.getParameter("investorID");
			// String amount = request.getParameter("amount");

			String action = request.getParameter("action");

			try {

				HibernateUtil.getSession().beginTransaction();

				if (action.equals("create")) {

					String companyID = request.getParameter("companyID");
					String investorID = request.getParameter("investorID");
					String amount = request.getParameter("amount");
					String price = request.getParameter("price");
					String type = request.getParameter("type");

					Company c = (Company) HibernateUtil.getSession().get(
							Company.class, Integer.valueOf(companyID));
					Investor i = (Investor) HibernateUtil.getSession().get(
							Investor.class, Integer.valueOf(investorID));
					Integer v = Integer.valueOf(amount);
					Double p = Double.valueOf(price);
					Integer t = Integer.valueOf(type);

					this.createOffer(i, c, v, p, t);

				} else if (action.equals("delete")) {

					String offerID = request.getParameter("offerID");

					try {

						Offer o = (Offer) HibernateUtil.getSession()
								.createQuery("FROM Offer WHERE id = :id")
								.setParameter("id", offerID).uniqueResult();

						this.deleteOffer(o);

					} catch (RuntimeException e) {

						throw e;
					}

				} else if (action.equals("buy")) {

				} else {

					HibernateUtil.getSession().getTransaction().commit();

				}

			} catch (Exception e) {

				HibernateUtil.getSession().getTransaction().rollback();
				throw e;
			}

			// response.sendRedirect(this.getServletContext().getContextPath());

		} catch (IOException e) {
			throw e;

		} catch (Exception e) {
			throw new ServletException(e);
		}

	} // end doPost

	public void createOffer(Investor investor, Company company, Integer amount, double price, Integer type)
		throws IllegalArgumentException{
		
		Offer offer = new Offer();
		
		if(Type.class.getEnumConstants()[type] == Type.SELL){
			
			// Checagem para validar se o usuário tentará vender mais ações do que possui
			
			Stock stock = (Stock) HibernateUtil.getSession().createQuery(
			"FROM Stock WHERE owner = :owner AND company = :company ").
			setParameter("owner", investor).setParameter("company",company).uniqueResult();

			if(stock.getAmount() < amount){
				// Está tentando vender mais ações do que tem
				throw new IllegalArgumentException(); 	
			}
			
			offer.setAmount(amount);
			offer.setCompany(company);
			offer.setOwner(investor);
			offer.setPrice(price);
			offer.setType(Type.class.getEnumConstants()[type]);
		
			// Tentar fazer com uma oferta compatível no mercado
			
			// Fazendo o matching nas ofertas já existentes
			
			if(Type.class.getEnumConstants()[type] == Type.SELL){
				
				List offers = HibernateUtil.getSession().createQuery(
				"FROM Offer WHERE  type = :type AND price <= :price AND amount >= :amount AND company = :company")
				.setParameter("type", Type.class.getEnumConstants()[type])
				.setParameter("price",price)
				.setParameter("amount",amount)
				.setParameter("company",company)
				.list();
	
				if(! offers.isEmpty()){
					this.doOfferSelling((Offer) offers.get(0), investor);		
				} else { HibernateUtil.getSession().save(offer);}
				
			}else {
				
					List offers = HibernateUtil.getSession().createQuery(
					"FROM Offer WHERE  type = :type AND price <= :price AND amount >= :amount AND company = :company")
					.setParameter("type", Type.class.getEnumConstants()[type])
					.setParameter("price",price)
					.setParameter("amount",amount)
					.setParameter("company",company)
					.list();
				
					if(! offers.isEmpty()){
						this.doOfferBuying((Offer) offers.get(0), investor);		
					
					} else { HibernateUtil.getSession().save(offer); }
			}
		
		}
			
			
	} // end createOffer

	public void doOfferBuying(Offer offer, Investor investor) {

		try {

			// Na compra de ofertas, investor é quem está comprando
			// Precisamos fazer a transação ocorrer da maneira correta, ou seja
			// investor terá o montante da oferta - que será determinado -
			// debitado
			// de seu saldo e ganhará mais ações na sua carteira

			double offerValue = offer.getAmount() * offer.getPrice();

			// Computando o débito do comprador e o crédito para o vendedor

			investor.setBalance(investor.getBalance() - offerValue);

			offer.getOwner().setBalance(
					offer.getOwner().getBalance() + offerValue);

			// Obtém a carteira de ações do comprador

			Stock s1 = (Stock) HibernateUtil.getSession().createQuery(
					"FROM Stock WHERE owner = :owner AND company = :company ")
					.setParameter("owner", investor).setParameter("company",
							offer.getCompany()).uniqueResult();

			// Atualiza carteira de ações do comprador
			s1.setAmount(s1.getAmount() + offer.getAmount());

			// Obtém a carteira de ações do vendedor

			Stock s2 = (Stock) HibernateUtil.getSession().createQuery(
					"FROM Stock WHERE owner = :owner AND company = :company ")
					.setParameter("owner", offer.getOwner()).setParameter(
							"company", offer.getCompany()).uniqueResult();

			// Atualiza carteira de ações do vendedor
			s2.setAmount(s2.getAmount() - offer.getAmount());

		} catch (RuntimeException e) {

			throw e;

		}

	} // end doOfferBuying

	public void doOfferSelling(Offer offer, Investor investor) {

		try {

			// Na venda de ofertas, investor é quem está vendendo
			// Precisamos fazer a transação ocorrer da maneira correta, ou seja
			// investor terá o montante da oferta - que será determinado -
			// creditado
			// de seu saldo e ganhará mais ações na sua carteira

			double offerValue = offer.getAmount() * offer.getPrice();

			// Computando o débito do comprador e o crédito para o vendedor

			investor.setBalance(investor.getBalance() + offerValue);

			offer.getOwner().setBalance(
					offer.getOwner().getBalance() - offerValue);

			// Obtém a carteira de ações do comprador

			Stock s1 = (Stock) HibernateUtil.getSession().createQuery(
					"FROM Stock WHERE owner = :owner AND company = :company ")
					.setParameter("owner", investor).setParameter("company",
							offer.getCompany()).uniqueResult();

			// Atualiza carteira de ações do comprador
			s1.setAmount(s1.getAmount() - offer.getAmount());

			// Obtém a carteira de ações do vendedor

			Stock s2 = (Stock) HibernateUtil.getSession().createQuery(
					"FROM Stock WHERE owner = :owner AND company = :company ")
					.setParameter("owner", offer.getOwner()).setParameter(
							"company", offer.getCompany()).uniqueResult();

			// Atualiza carteira de ações do vendedor
			s2.setAmount(s2.getAmount() - offer.getAmount());

		} catch (RuntimeException e) {

			throw e;

		}

	} // end doOfferSelling

	public void deleteOffer(Offer offer) {

		HibernateUtil.getSession().delete(offer);

	} // end deleteOffer

} // end OfferManager

