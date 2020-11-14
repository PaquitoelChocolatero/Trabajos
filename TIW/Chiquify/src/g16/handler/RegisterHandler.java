package g16.handler;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import g16.model.*;

public class RegisterHandler implements RequestHandler {

	@Override
	public String process(HttpServletRequest request, HttpServletResponse response) {
		// TODO Auto-generated method stub
		
		String name = request.getParameter("name");
		String lastname1 = request.getParameter("lastname1");
		String lastname2 = request.getParameter("lastname2");
		String city = request.getParameter("city");
		String email = request.getParameter("email");
		String password = request.getParameter("password");
		//int balance = Integer.parseInt(request.getParameter("balance"));
		
		Usuario _usuario = new Usuario();
		_usuario.setNombre(name);
		_usuario.setApellido1(lastname1);
		_usuario.setApellido2(lastname2);
		_usuario.setCiudad(city);
		_usuario.setEmail(email);
        _usuario.setPasswd(password);
		
		DBHelper helper = new DBHelper();
		helper.insert(_usuario);
		
		AccountManager am = new AccountManager();
		am.insert(_usuario);
		
		
		return "home.html";
	}

}