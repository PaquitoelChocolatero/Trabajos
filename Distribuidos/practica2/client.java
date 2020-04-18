import java.io.*;
import java.net.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
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
					inp.close();
				}

			}
		}
		catch(Exception e){
			System.err.println("excepcion "+ e.toString());
			e.printStackTrace();
		}
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
			os.write("\0");
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
		}
	}
	
	/******************* ATTRIBUTES *******************/
	
	private static String _server   = null;
	private static int _port = -1;
		
	
	/********************* METHODS ********************/
	
	/**
	 * @param user - User name to register in the system
	 * 
	 * @return ERROR CODE
	 */
	static int register(String user) 
	{
		// Write your code here
		System.out.println("REGISTER " + user);
		try{
			socket sc = new socket(_server, _port);
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			escribir(os, "REGISTER");
			escribir(os, user);

			String respuesta = leer(in);
			int result = Integer.parseInt(respuesta);

			sc.close();
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result = 0){
			System.out.println("c> REGISTER OK");
		}
		else if(result = 1){
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
		// Write your code here
		System.out.println("UNREGISTER " + user);
		try{
			socket sc = new socket(_server, _port);
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			escribir(os, "UNREGISTER");
			escribir(os, user);

			String respuesta = leer(in);
			int result = Integer.parseInt(respuesta);
			
			sc.close();
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result = 0){
			System.out.println("c> UNREGISTER OK");
		}
		else if(result = 1){
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
		// Write your code here
		System.out.println("CONNECT " + user);
		try{
			socket sc = new socket(_server, _port);
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			ServerSocket serverclient = new ServerSocket(0);
			int port = serverclient.getLocalPort();
			escribir(os, "CONNECT");
			escribir(os, user);
			escribir(os, port.toString());

			String respuesta = leer(in);
			int result = Integer.parseInt(respuesta);
			
			sc.close();
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 3;
		}
		if(result = 0){
			System.out.println("c> CONNECT OK");
		}
		else if(result = 1){
			System.out.println("c> CONNECT FAIL, USER DOES NOT EXIST");
		}
		else if(result = 2){
			System.out.println("c> USER ALREADY CONNECTED");
		}
		else {
			System.out.println("c> CONNECT FAIL");
		}
		return result;
	}
	
	 /**
	 * @param user - User name to disconnect from the system
	 * 
	 * @return ERROR CODE
	 */
	static int disconnect(String user) 
	{
		// Write your code here
		System.out.println("DISCONNECT " + user);
		return 0;
	}

	 /**
	 * @param file_name    - file name
	 * @param description - descrition
	 * 
	 * @return ERROR CODE
	 */
	static int publish(String file_name, String description) 
	{
		// Write your code here
		System.out.println("PUBLISH " + file_name + " " + description);
		return 0;
	}

	 /**
	 * @param file_name    - file name
	 * 
	 * @return ERROR CODE
	 */
	static int delete(String file_name)
	{
		// Write your code here
		System.out.println("DELETE " + file_name);
		return 0;
	}

	 /**
	 * @return ERROR CODE
	 */
	static int list_users()
	{
		// Write your code here
		System.out.println("LIST_USERS " );
		return 0;
	}


	 /**
	 * @param user_name    - user name
	 * 
	 * @return ERROR CODE
	 */
	static int list_content(String user_name)
	{
		// Write your code here
		System.out.println("LIST_CONTENT " + user_name);
		return 0;
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
		// Write your code here
		System.out.println("GET_FILE " + user_name + " "  + remote_file_name + " " + local_file_name);
		return 0;
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
	
	
	
	/********************* MAIN **********************/
	
	public static void main(String[] argv) 
	{
		if(!parseArguments(argv)) {
			usage();
			return;
		}
		
		// Write code here
		
		shell();
		System.out.println("+++ FINISHED +++");
	}
}