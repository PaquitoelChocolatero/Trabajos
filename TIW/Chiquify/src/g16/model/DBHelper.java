package g16.model;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.sql.DataSource;

public class DBHelper {

	private Connection _connection;
	
	private void connect() {
		try {
			Context _context = new InitialContext();
			DataSource _datasource = (DataSource) _context.lookup("jdbc/chiqify");
			 _connection = _datasource.getConnection();
		} catch (NamingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void disconect() {
		try {
			_connection.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	public void insert(Usuario nueva) {
		connect();
	
		try {
			PreparedStatement ps = _connection.prepareStatement("INSERT INTO usuario ('EMAIL', 'PASSWD', 'NOMBRE', 'APELLIDO1', 'APELLIDO2', 'CIUDAD')  values (?,MD5(?),?,?,?,?)");
			ps.setString(1, nueva.getEmail());
			ps.setString(2, nueva.getPasswd());
			ps.setString(3, nueva.getNombre());
			ps.setString(4, nueva.getApellido1());
			ps.setString(5, nueva.getApellido2());
			ps.setString(6, nueva.getCiudad());

			ps.execute();

			ps.close();
			
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		disconect();
	}
	
	
	public void delete(String email) {
		connect();
		
		try {
			PreparedStatement ps = _connection.prepareStatement("delete from usuario where email = ?");
			ps.setString(1, email);

			ps.execute();

			ps.close();
			
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
		
		disconect();
	}
}