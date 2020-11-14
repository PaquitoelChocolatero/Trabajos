package g16.handler;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import es.uc3m.tiw.model.Account;
import es.uc3m.tiw.model.AccountManager;
import es.uc3m.tiw.model.DBHelper;

public class LoginHandler implements RequestHandler {

	@Override
	public String process(HttpServletRequest request, HttpServletResponse response) {
		// TODO Auto-generated method stub
		
		String name = request.getParameter("owner");
		int balance = Integer.parseInt(request.getParameter("balance"));
		
		Account nuevo = new Account();
		nuevo.setName(name);
		nuevo.setBalance(balance);
		
		//DBHelper helper = new DBHelper();
		//helper.insert(nuevo);
		
		AccountManager am = new AccountManager();
		am.insert(nuevo);
		
		
		return "home.html";
	}

}