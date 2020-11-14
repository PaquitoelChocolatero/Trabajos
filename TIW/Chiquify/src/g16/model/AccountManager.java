package g16.model;

import java.util.List;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.EntityTransaction;
import javax.persistence.Persistence;
import javax.persistence.Query;

public class AccountManager {

	private EntityManagerFactory emf;

	public AccountManager() {
		super();
		this.emf = Persistence.createEntityManagerFactory("chiquify");
	}
	
	public List<Usuario> getAll(){
		List<Usuario> usuarios = null;
		
		EntityManager em = emf.createEntityManager();
		
		Query q = em.createQuery("Select * from Usuario");
		
		usuarios = q.getResultList();
		
		return usuarios;
	}
	
	
	public void insert(Usuario u) {
		EntityManager em = emf.createEntityManager();
		EntityTransaction et = em.getTransaction();
		try {
			et.begin();
		
			em.persist(u);
		
			et.commit();
		}catch(Exception e) {
			if(et.isActive()) {
				et.rollback();
			}
		}
		
		
	}
	
	/*
	public void transfer(int a1, int a2, int m) {
		EntityManager em = emf.createEntityManager();
		EntityTransaction et = em.getTransaction();
		
		Usuario Usuario1 = em.find(Usuario.class, a1);
		Usuario Usuario2 = em.find(Usuario.class, a2);
		
		try {
			et.begin();
			Usuario1.setBalance(Usuario1.getBalance()-m);
			Usuario2.setBalance(Usuario2.getBalance()+m);
			em.merge(Usuario1);
			em.merge(Usuario2);
			et.commit();
		}catch(Exception e) {
			if(et.isActive()) {
				et.rollback();
			}
		}
		
	}
	*/
	
}
