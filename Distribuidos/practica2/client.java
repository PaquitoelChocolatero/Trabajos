import java.io.*;
import java.net.*;
//import java.nio.file.Files;
//import java.nio.file.Path;
//import java.nio.file.Paths;
import java.util.ArrayList;
import gnu.getopt.Getopt;


class serverthread extends Thread{
	private ServerSocket sc;
	
	public serverthread(ServerSocket ss){
		sc = ss;
	}
	public void run(){
		try{
			while(true){
				Socket sh = sc.accept();
				DataOutputStream os = new DataOutputStream(sh.getOutputStream());
				DataInputStream in = new DataInputStream(sh.getInputStream());
			
				String mensaje = leer(in);
				if(!mensaje.equals("GET_FILE")){
					System.out.println("Error");
					sc.close();
					sh.close();	
					return;
				}
				String rmt_file = leer(in);
				File f = new File(rmt_file);
				if(f.exists() && f.isFile()){
					escribir(os, "0");
					InputStream inp = new FileInputStream(rmt_file);
					DataInputStream inputfile = new DataInputStream(inp);

					copy.copyFile(inputfile, os);
					inputfile.close();
				}
				else{
					escribir(os, "1");
				}
				sh.close();	
			}
		}
		catch(SocketException e) {
			//System.out.println("Cerrando server thread...");
		}	
		catch(Exception e){
			System.err.println("excepcion "+ e.toString());
			e.printStackTrace();
		}
	}
	public void Stop(){
		try{
			sc.close();
		}
		catch(SocketException e) {}	
		catch(Exception e){
			System.err.println("excepcion "+ e.toString());
			e.printStackTrace();
		}
	}

	public static String leer(DataInputStream in){
    	byte[] ch = new byte[1];
    	String output = new String();
    	try {
        	do {
            	ch[0] = in.readByte();
            	if (ch[0] != '\0'){
               		String d = new String(ch);
               		output = output + d;
            	}
         	}while(ch[0] != '\0');
      	}
      	catch (Exception e) {
        	System.err.println("excepcion " + e.toString() );
      	}
      	return output;
   	}
	public static void escribir(DataOutputStream os, String salida){
		try{
			os.writeBytes(salida);
			os.write('\0');
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
		}
	}
	
}

class userInfo{
	String username;
	String ipadress;
	int port;

	public userInfo (String user, String ip, int pt){
		username = user;
		ipadress = ip;
		port = pt;
	}
}

class client {
	
	/*********FUNCIONES AUXILIARES*********************/
	public static String leer(DataInputStream in){
    	byte[] ch = new byte[1];
    	String output = new String();
    	try {
        	do {
            	ch[0] = in.readByte();
            	if (ch[0] != '\0'){
               		String d = new String(ch);
               		output = output + d;
            	}
         	}while(ch[0] != '\0');
      	}
      	catch (Exception e) {
        	System.err.println("excepcion " + e.toString() );
      	}
      	return output;
   	}
	public static void escribir(DataOutputStream os, String salida){
		try{
			os.writeBytes(salida);
			os.write('\0');
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
		}
	}
	
	/******************* ATTRIBUTES *******************/
	
	private static String _server   = null;
	private static int _port = -1;
	private static serverthread _serverthread = null;
	private static String _connecteduser = null;
	private static ArrayList<userInfo> _connectedusers;
	
	/********************* METHODS ********************/
	
	/**
	 * @param user - User name to register in the system
	 * 
	 * @return ERROR CODE
	 */
	static int register(String user) 
	{
		int result = 0;
		try{
			Socket sc = new Socket(_server, _port);
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			escribir(os, "REGISTER");
			escribir(os, user);

			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);

			sc.close();
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result == 0){
			System.out.println("c> REGISTER OK");
		}
		else if(result == 1){
			System.out.println("c> USERNAME IN USE");
		}
		else {
			System.out.println("c> REGISTER FAIL");
		}
		
		return result;
	}
	
	/**
	 * @param user - User name to unregister from the system
	 * 
	 * @return ERROR CODE
	 */
	static int unregister(String user) 
	{
		int result=0;
		try{
			Socket sc = new Socket(_server, _port);
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			escribir(os, "UNREGISTER");
			escribir(os, user);

			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);
			
			sc.close();
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result == 0){
			System.out.println("c> UNREGISTER OK");
		}
		else if(result == 1){
			System.out.println("c> USER DOES NOT EXIST");
		}
		else {
			System.out.println("c> UNREGISTER FAIL");
		}
		return result;
	}
	
    	/**
	 * @param user - User name to connect to the system
	 * 
	 * @return ERROR CODE
	 */
	static int connect(String user) 
	{
		if(_connecteduser==null){
			int result=0;
			try{
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				ServerSocket serverclient = new ServerSocket(0);
				int port = serverclient.getLocalPort();
				escribir(os, "CONNECT");
				escribir(os, user);
				escribir(os, Integer.toString(port));

				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);

				sc.close();
				if (result==0){
					_serverthread = new serverthread(serverclient);
					_serverthread.start();
					_connecteduser = user;
				}

			}
			catch(Exception e){
				System.err.println("excepcion " + e.toString());
				result = 3;
			}
			if(result == 0){
				System.out.println("c> CONNECT OK");
			}
			else if(result == 1){
				System.out.println("c> CONNECT FAIL, USER DOES NOT EXIST");
			}
			else if(result == 2){
				System.out.println("c> USER ALREADY CONNECTED");
			}
			else {
				System.out.println("c> CONNECT FAIL");
			}
			return result;
		}else{
			System.out.println("c> CONNECT FAIL, CONNECTION ACTIVE: " + _connecteduser);
			return -1;
		}
	}
	
	 /**
	 * @param user - User name to disconnect from the system
	 * 
	 * @return ERROR CODE
	 */
	static int disconnect(String user) 
	{
		int result=0;
		try{
			if(!user.equals(_connecteduser)){
				result = 2;
			}
			else{
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				escribir(os, "DISCONNECT");
				escribir(os, user);

				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);
			
				sc.close();
				_connectedusers = new ArrayList<userInfo>();
			}

		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			_connectedusers = new ArrayList<userInfo>();
			result = 3;
			if(_serverthread == null){
				_serverthread.Stop();
				_serverthread = null;
				_connecteduser = null;
			}
		}
		if(result == 0){
			_serverthread.Stop();
			_serverthread = null;
			_connecteduser = null;
			System.out.println("c> DISCONNECT OK");
		}
		else if(result == 1){
			System.out.println("c> DOSCONNECT FAIL / USER DOES NOT EXIST");
		}
		else if(result == 2){
			System.out.println("c> DISCONNECT FAIL / USER NOT CONNECTED");
		}
		else {
			System.out.println("c> DISCONNECT FAIL");
		}
		return result;
	}
	static int publish(String file_name, String description) 
	{
		int result=0;
		try{
			if(_serverthread == null){
				result = 2;
			}
			else{
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				escribir(os, "PUBLISH");
				escribir(os, _connecteduser);
				escribir(os, file_name);
				escribir(os, description);

				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);

				sc.close();
			}
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 4;
		}
		if(result == 0){
			System.out.println("c> PUBLISH OK");
		}
		else if(result == 1){
			System.out.println("c> PUBLISH FAIL, USER DOES NOT EXIST");
		}
		else if(result == 2){
			System.out.println("c> PUBLISH FAIL, USER NOT CONNECTED");
		}
		else if(result == 3){
			System.out.println("c> PUBLISH FAIL, CONTENT ALREADY PUBLISHED");
		}
		else{
			System.out.println("c> PUBLISH FAIL");
		}
		
		return result;
	}

	 /**
	 * @param file_name    - file name
	 * 
	 * @return ERROR CODE
	 */
	static int delete(String file_name)
	{
		int result = 0;
		try{
			if(_serverthread == null){
				result = 2;
			}
			else{
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				escribir(os, "DELETE");
				escribir(os, _connecteduser);

				escribir(os, file_name);

				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);

				sc.close();
			}
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 4;
		}
		if(result == 0){
			System.out.println("c> DELETE OK");
		}
		else if(result == 1){
			System.out.println("c> DELETE FAIL, USER DOES NOT EXIST");
		}
		else if(result == 2){
			System.out.println("c> DELETE FAIL, USER NOT CONNECTED");
		}
		else if(result == 3){
			System.out.println("c> DELETE FAIL, CONTENT NOT PUBLISHED");
		}
		else{
			System.out.println("c> DELETE FAIL");
		}
		
		return result;
	}

	 /**
	 * @return ERROR CODE
	 */
	static int list_users()
	{
		int result = 0;
		if (_serverthread == null){
			System.out.println("c> LIST_USERS FAIL, USER NOT CONNECTED");
			return 2;
		}
		try{
			Socket sc = new Socket(_server, _port); // conexión
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());
			escribir(os, "LIST_USERS");
			escribir(os, _connecteduser);
			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);
			if (result >= 0){
				System.out.println("c> LIST_USERS OK");
				int num_users = result;
				_connectedusers = new ArrayList<userInfo>();
				for (int i = 0; i < num_users; i++){
					String user = leer(in);
					String ip = leer(in);
					String puerto = leer(in);
					System.out.printf("%10s%15s%10s\n", user, ip, puerto);
					_connectedusers.add(new userInfo(user, ip, Integer.parseInt(puerto)));
				}
				result = 0;
			}	
			sc.close();
		}
		catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			result = 4;
		}
		if (result == 0) return result;
		else if (result == -1){
			result *= -1;
			System.out.println("c> LIST_USERS FAIL, USER DOES NOT EXIST");
		}
		else if (result == -2){
			result *= -1;
			System.out.println("c> LIST_USERS FAIL, USER NOT CONNECTED");
		}
		else {
			result *= -1;
			System.out.println("c> LIST_USERS FAIL");
		}
		return result;
	}


	 /**
	 * @param user_name    - user name
	 * 
	 * @return ERROR CODE
	 */
	static int list_content(String user_name)
	{
		int result = 0;
		if (_serverthread == null){
			System.out.println("c> LIST_CONTENT FAIL, USER NOT CONNECTED");
			return 2;
		}
		try{
			Socket sc = new Socket(_server, _port); // conexión
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());
			escribir(os, "LIST_CONTENT");
			escribir(os, _connecteduser);
			escribir(os, user_name);
			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);
			if (result > 0){
				System.out.println("c> LIST_CONTENT OK");
				int num_files = result;

				for (int i = 0; i < num_files; i++){
					String file = leer(in);
					String descripcion = leer(in);
					System.out.printf("\t%s\t%s\n", file, descripcion);
				}
				result = 0;
			}
			else if(result == 0) System.out.println("c> " + user_name + " hasn't published any files yet");
			sc.close();
		}
		catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			result = -5;
		}
		if (result == 0) return result;
		else if (result == -1){
			result *= -1;
			System.out.println("c> LIST_CONTENT FAIL, USER DOES NOT EXIST");
		}
		else if (result == -2){
			result *= -1;
			System.out.println("c> LIST_CONTENT FAIL, USER NOT CONNECTED");
		}
		else if (result == -3){
			result *= -1;
			System.out.println("c> LIST_CONTENT FAIL, REMOTE USER DOES NOT EXIST");
		}
		else if (result == -4){
			result *= -1;
			System.out.println("c> LIST_CONTENT FAIL, REMOTE USER NOT CONNECTED");
		}
		else {
			result *= -1;
			System.out.println("c> LIST_CONTENT FAIL");
		}
		return result;
	}

	 /**
	 * @param user_name    - user name
	 * @param remote_file_name    - remote file name
	 * @param local_file_name  - local file name
	 * 
	 * @return ERROR CODE
	 */
	static int get_file(String user_name, String remote_file_name, String local_file_name)
	{
		int result = 0;
		try{
			if(_connectedusers == null|| _connectedusers.isEmpty()){
				result = 2;
			}
			else{
				int i = 0;
				while(i < _connectedusers.size()){
					if(_connectedusers.get(i).username.equals(user_name)){
						break;
					}
					else{
						i++;
					}
				}
				if(_connectedusers.size() == i){
					System.out.println("c> DELETE FAIL, USER NOT CONNECTED");
					return 2;
				}
				String ip = _connectedusers.get(i).ipadress;
				int puerto = _connectedusers.get(i).port;

				Socket sc = new Socket(ip, puerto);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				escribir(os, "GET_FILE");
				escribir(os, remote_file_name);

				Thread.sleep(500);

				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);
				
				if(result == 0){
					Thread.sleep(500);
					OutputStream out = new FileOutputStream(local_file_name);
					DataOutputStream outputfile = new DataOutputStream(out);
					copy.copyFile(in,outputfile);
					outputfile.close();
				}
				sc.close();
			}
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result == 0){
			System.out.println("c> GET_FILE OK");
		}
		else if(result == 1){
			System.out.println("c> GET_FILE FAIL, FILE DOES NOT EXIST");
		}
		else{
			System.out.println("c> GET_FILE FAIL");
		}
		
		return result;
	}

	
	/**
	 * @brief Command interpreter for the client. It calls the protocol functions.
	 */
	static void shell() 
	{
		boolean exit = false;
		String input;
		String [] line;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		while (!exit) {
			try {
				System.out.print("c> ");
				input = in.readLine();
				line = input.split("\\s");

				if (line.length > 0) {
					/*********** REGISTER *************/
					if (line[0].equals("REGISTER")) {
						if  (line.length == 2) {
							register(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: REGISTER <userName>");
						}
					} 
					
					/********** UNREGISTER ************/
					else if (line[0].equals("UNREGISTER")) {
						if  (line.length == 2) {
							unregister(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: UNREGISTER <userName>");
						}
                    			} 
                    			
                    			/************ CONNECT *************/
                    			else if (line[0].equals("CONNECT")) {
						if  (line.length == 2) {
							connect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: CONNECT <userName>");
                    				}
                    			} 
                    
                    			/********** DISCONNECT ************/
                    			else if (line[0].equals("DISCONNECT")) {
						if  (line.length == 2) {
							disconnect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DISCONNECT <userName>");
                    				}
                    			} 
                    
                    			/************** PUBLISH **************/
                    			else if (line[0].equals("PUBLISH")) {
						if  (line.length >= 3) {
							// Remove first two words
							//String description = input.substring(input.indexOf(' ')+1).substring(input.indexOf(' ')+1);
							String description = input.substring(input.indexOf(' ')+1);
							description = description.substring(description.indexOf(' ')+1);
							publish(line[1], description); // file_name = line[1]
						} else {
							System.out.println("Syntax error. Usage: PUBLISH <file_name> <description>");
                    				}
                    			} 

                    			/************ DELETE *************/
                    			else if (line[0].equals("DELETE")) {
						if  (line.length == 2) {
							delete(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DELETE <file name>");
                    				}
                    			} 
                    
                    			/************** LIST_USERS **************/
                    			else if (line[0].equals("LIST_USERS")) {
						if  (line.length == 1) {
							// Remove first two words
							list_users(); 
						} else {
							System.out.println("Syntax error. Usage: LIST_USERS ");
                    				}
                    			} 
                    
                    			/************ LIST_CONTENT *************/
                    			else if (line[0].equals("LIST_CONTENT")) {
						if  (line.length == 2) {
							list_content(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: LIST_CONTENT <user name>");
                    				}
                    			} 
                    
                    			/************** GET_FILE **************/
                    			else if (line[0].equals("GET_FILE")) {
						if  (line.length == 4) {
							get_file(line[1], line[2], line[3]); 
						} else {
							System.out.println("Syntax error. Usage: GET_FILE <user> <remote_file_name> <local_file_name>");
                    				}
                    			} 

                    
                    			/************** QUIT **************/
                    			else if (line[0].equals("QUIT")){
						if (line.length == 1) {
							exit = true;
						} else {
							System.out.println("Syntax error. Use: QUIT");
						}
					} 
					
					/************* UNKNOWN ************/
					else {						
						System.out.println("Error: command '" + line[0] + "' not valid.");
					}
				}				
			} catch (java.io.IOException e) {
				System.out.println("Exception: " + e);
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * @brief Prints program usage
	 */
	static void usage() 
	{
		System.out.println("Usage: java -cp . client -s <server> -p <port>");
	}
	
	/**
	 * @brief Parses program execution arguments 
	 */ 
	static boolean parseArguments(String [] argv) 
	{
		Getopt g = new Getopt("client", argv, "ds:p:");

		int c;
		String arg;

		while ((c = g.getopt()) != -1) {
			switch(c) {
				//case 'd':
				//	_debug = true;
				//	break;
				case 's':
					_server = g.getOptarg();
					break;
				case 'p':
					arg = g.getOptarg();
					_port = Integer.parseInt(arg);
					break;
				case '?':
					System.out.print("getopt() returned " + c + "\n");
					break; // getopt() already printed an error
				default:
					System.out.print("getopt() returned " + c + "\n");
			}
		}
		
		if (_server == null)
			return false;
		
		if ((_port < 1024) || (_port > 65535)) {
			System.out.println("Error: Port must be in the range 1024 <= port <= 65535");
			return false;
		}

		return true;
	}

	/***** CAPTURAMOS EL CIERRE DE LA APLICACIÓN *****/
	static public void attachShutDownHook(){
		Runtime.getRuntime().addShutdownHook(new Thread() {
	  		@Override
	  		public void run() {
			  	System.out.println("\n+++ DISCONNECTING +++");
				if(_connecteduser != null) disconnect(_connecteduser);	//Desconectamos al usuario para proteger la base de datos
	   		}
		});
  	}
	
	

	/********************* MAIN **********************/
	
	public static void main(String[] argv) 
	{
	  	attachShutDownHook();

		if(!parseArguments(argv)) {
			usage();
			return;
		}
		System.out.println("Host: "+_server+" Port: "+ _port);
		System.out.println("To quit type: 'QUIT' or press Ctrl+C\n");
		
		try{
			shell();
			if(_serverthread != null){
				_serverthread.Stop();
				_serverthread = null;
				_connecteduser = null;
			}
		}
		catch(Exception e){
			System.out.println("+++ FINISHED +++");
			e.printStackTrace();
		}
	}
}