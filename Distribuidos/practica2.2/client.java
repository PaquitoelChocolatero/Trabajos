import java.io.*;
import java.net.*;
//import java.nio.file.Files;
//import java.nio.file.Path;
//import java.nio.file.Paths;
import java.util.ArrayList;
import gnu.getopt.Getopt;

//Clase para el servidor local del cliente para la petición GET_FILE
class serverthread extends Thread{
	//Declaracion del serversocket
	private ServerSocket sc;
	//constructor de la clase
	public serverthread(ServerSocket ss){
		sc = ss;
	}
	//funcion principal del server
	public void run(){
		try{
			while(true){
				//recibe una conexion de un cliente y crea el socket
				Socket sh = sc.accept();
				DataOutputStream os = new DataOutputStream(sh.getOutputStream());
				DataInputStream in = new DataInputStream(sh.getInputStream());
				
				//Lee del socket
				String mensaje = leer(in);

				//si la cadena recibida no es GET_FILE error
				if(!mensaje.equals("GET_FILE")){
					System.out.println("Error");
					sc.close();
					sh.close();	
					return;
				}
				//recibe del socket el fichero
				String rmt_file = leer(in);
				File f = new File(rmt_file);

				//Si existe el fichero lo copia al socket y envia codigo 0
				if(f.exists() && f.isFile()){
					escribir(os, "0");
					InputStream inp = new FileInputStream(rmt_file);
					DataInputStream inputfile = new DataInputStream(inp);
					copy.copyFile(inputfile, os);
					//cierre del fichero
					inputfile.close();
				}
				//Si no existe envia codigo 1
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
	//funcion para terminar el servidor local del GET_FILE
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
	//Funcion para leer Strings de C en java
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
	//Funcion para escribir Strings de java a C
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
//Struct para almacenar los usuarios conectados en un Arraylist
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
	//Funcion para leer Strings de C en java
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
	//Funcion para escribir Strings de java a C
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
	//Variables globales 
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

	//Funcion register
	static int register(String user) 
	{
		int result = 0;
		try{
			//creacion del socket
			Socket sc = new Socket(_server, _port);
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			//enviamos cadena REGISTER y cadena con el nombre de usuario
			escribir(os, "REGISTER");
			escribir(os, user);
			
			//leemos respuesta del server
			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);
			
			//cierre del socket
			sc.close();
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result == 0){ //caso de result = 0
			System.out.println("c> REGISTER OK");
		}
		else if(result == 1){ //caso de result = 1
			System.out.println("c> USERNAME IN USE");
		}
		else {	//resto de casos
			System.out.println("c> REGISTER FAIL");
		}
		
		return result;
	}
	
	/**
	 * @param user - User name to unregister from the system
	 * 
	 * @return ERROR CODE
	 */

	//Funcion register
	static int unregister(String user) 
	{
		int result=0;
		try{
			//creacion del socket
			Socket sc = new Socket(_server, _port);
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			//enviamos cadena UNREGISTER y cadena con el nombre de usuario
			escribir(os, "UNREGISTER");
			escribir(os, user);

			//leemos respuesta del server
			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);
			
			//cierre del socket
			sc.close();
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result == 0){ //caso de result = 0
			System.out.println("c> UNREGISTER OK");
		}
		else if(result == 1){ //caso de result = 1
			System.out.println("c> USER DOES NOT EXIST");
		}
		else { //resto de casos
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
				//creacion del socket
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());
				
				//creacion del serversocket que va a utilizarse en GET_FILE
				ServerSocket serverclient = new ServerSocket(0);
				//obtencion del puerto local
				int port = serverclient.getLocalPort();

				//enviamos cadena CONNECT seguida de la cadena con el nombre de usuario y la cadena con el puerto
				escribir(os, "CONNECT");
				escribir(os, user);
				escribir(os, Integer.toString(port));
				
				//leemos respuesta del server
				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);

				//cierre del socket
				sc.close();
				
				//si la respuesta es 0 iniciamos el server para los GET_FILE
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
			if(result == 0){ //caso de result = 0
				System.out.println("c> CONNECT OK");
			} 
			else if(result == 1){ //caso de result = 1
				System.out.println("c> CONNECT FAIL, USER DOES NOT EXIST");
			}
			else if(result == 2){ //caso de result = 2
				System.out.println("c> USER ALREADY CONNECTED");
			}
			else { //resto de casos
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
			//Si el usuario introducido no está conectado devuelve 2
			if(!user.equals(_connecteduser)){
				result = 2;
			}
			else{
				//creacion del socket
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());
				
				//enviamos la cadena DISCONNECT y la cadena con el nombre de usuario
				escribir(os, "DISCONNECT");
				escribir(os, user);

				//leemos respuesta del servidor
				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);
				
				//cierre del socket
				sc.close();

				//clear en la lista de usuarios conectados
				_connectedusers = new ArrayList<userInfo>();
			}

		}
		catch(Exception e){
			//en caso de haber excepcion hacemos clear en la lista de usuarios y cerramos el server
			System.err.println("excepcion " + e.toString());
			_connectedusers = new ArrayList<userInfo>();
			result = 3;
			if(_serverthread != null){
				_serverthread.Stop();
				_serverthread = null;
				_connecteduser = null;
			}
		}
		if(result == 0){ //caso de result = 0
			//cierre del server local
			_serverthread.Stop();
			_serverthread = null;
			_connecteduser = null;
			System.out.println("c> DISCONNECT OK");
		}
		else if(result == 1){ //caso de result = 1
			System.out.println("c> DOSCONNECT FAIL / USER DOES NOT EXIST");
		}
		else if(result == 2){ //caso de result = 2
			System.out.println("c> DISCONNECT FAIL / USER NOT CONNECTED");
		}
		else { //resto de casos
			System.out.println("c> DISCONNECT FAIL");
		}
		return result;
	}
	static int publish(String file_name, String description) 
	{
		int result = 4;
		try{
			//si el server no está activo, el usuario no esta conectado, por lo tanto devuelve 2
			if(_serverthread == null){
				result = 2;
			}
			else{
				//comprobamos si el fichero introducido existe y si realmente es un fichero y no un directorio
				File f = new File(file_name);
				if(!f.exists() || !f.isFile()){
					System.out.println("c> PUBLISH FAIL, FILE DOES NOT EXIST");
					return 4;
				}

				//creacion del socket
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				//enviamos la cadena PUBLISH junto con la cadena con el nombre de usuario, la cadena con el fichero y la cadena con la descripcion
				escribir(os, "PUBLISH");
				escribir(os, _connecteduser);
				escribir(os, file_name);
				escribir(os, description);

				//leemos respuesta del servidor
				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);
				//cierre del socket
				sc.close();
			}
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 4;
		}
		if(result == 0){ //caso de result = 0
			System.out.println("c> PUBLISH OK");
		}
		else if(result == 1){ //caso de result = 1
			System.out.println("c> PUBLISH FAIL, USER DOES NOT EXIST");
		}
		else if(result == 2){ //caso de result = 2
			System.out.println("c> PUBLISH FAIL, USER NOT CONNECTED");
		}
		else if(result == 3){ //caso de result = 3
			System.out.println("c> PUBLISH FAIL, CONTENT ALREADY PUBLISHED");
		}
		else{ //resto de casos
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
		int result = 4;
		try{
			//si el server no está activo, el usuario no está conectado, por lo tanto devuelve 2
			if(_serverthread == null){
				result = 2;
			}
			else{
				//creacion del socket
				Socket sc = new Socket(_server, _port);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				//envia la cadena DELETE, la cadena con el nombre de usuario y la cadena del nombre del fichero
				escribir(os, "DELETE");
				escribir(os, _connecteduser);
				escribir(os, file_name);

				//leemos respuesta del server 
				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);

				//cierre del socket
				sc.close();
			}
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 4;
		}
		if(result == 0){ //caso de result = 0
			System.out.println("c> DELETE OK");
		}
		else if(result == 1){ //caso de result = 1
			System.out.println("c> DELETE FAIL, USER DOES NOT EXIST");
		}
		else if(result == 2){ //caso de result = 2
			System.out.println("c> DELETE FAIL, USER NOT CONNECTED");
		}
		else if(result == 3){ //caso de result = 3
			System.out.println("c> DELETE FAIL, CONTENT NOT PUBLISHED");
		}
		else{ //resto de casos
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
		//si el server no está activo, el usuario no está conectado, por lo tanto devuelve 2
		if (_serverthread == null){
			System.out.println("c> LIST_USERS FAIL, USER NOT CONNECTED");
			return 2;
		}
		try{
			//creacion del socket
			Socket sc = new Socket(_server, _port); // conexión
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			//enviamos la cadena LIST_USERS seguida de la cadena con el nombre de usuario
			escribir(os, "LIST_USERS");
			escribir(os, _connecteduser);

			//leemos respuesta del servidor
			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);

			//si la respuesta del servidor es 0, leemos de nuevo para obtener el numero de usuarios y limpiamos la lista de usuarios conectados
			if (result == 0){
				int num_users = Integer.parseInt(leer(in)) / 3;	//recibimos un usuario cada 3 lineas
				_connectedusers = new ArrayList<userInfo>();

				//imprimimos la lista de usuarios con su ip y puerto recibiendolos del servidor y añadimos los elementos al arraylist de usuarios conectados
				for (int i = 0; i < num_users; i++){
					String user = leer(in);
					String ip = leer(in);
					String puerto = leer(in);
					System.out.printf("%10s%15s%10s\n", user, ip, puerto);
					_connectedusers.add(new userInfo(user, ip, Integer.parseInt(puerto)));
				}
			}	
			//cierre del socket
			sc.close();
		}
		catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			result = 3;
		}

		if (result == 0){ //caso de result = 0
			System.out.println("c> LIST_USERS OK");
		}

		else if (result == 1){ //caso de result = 1
			System.out.println("c> LIST_USERS FAIL, USER DOES NOT EXIST");
		}

		else if (result == 2){ //caso de result = 2
			System.out.println("c> LIST_USERS FAIL, USER NOT CONNECTED");
		}
		else { //resto de casos
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
		
		//si el server no está activo, el usuario no está conectado, por lo tanto devuelve 2
		if (_serverthread == null){
			System.out.println("c> LIST_CONTENT FAIL, USER NOT CONNECTED");
			return 2;
		}
		try{
			//creacion del socket
			Socket sc = new Socket(_server, _port); // conexión
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());

			//enviamos la cadena LIST_CONTENT junto con la cadena del nombre del usuario
			escribir(os, "LIST_CONTENT");
			escribir(os, _connecteduser);
			escribir(os, user_name);

			//leemos respuesta del servidor
			String respuesta = leer(in);
			result = Integer.parseInt(respuesta);

			//Si el resultado es 0, se ha hecho correctamente
			if (result == 0){

				//leemos de nuevo para obtener el numero de ficheros del usuario
				int num_files = Integer.parseInt(leer(in)) / 2;

				//si el numero de ficheros es 0, imprime el siguiente error
				if(num_files == 0) System.out.println("c> " + user_name + " hasn't published any files yet");

				//Imprimimos la lista de ficheros junto con sus descripciones recibiendolos del servidor
				for (int i = 0; i < num_files; i++){
					String file = leer(in);
					String descripcion = leer(in);
					System.out.printf("\t%s\t%s\n", file, descripcion);
				}
			}
			
			sc.close();
		}
		catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			result = 4;
		}
		if (result == 0) { //caso de result = 0
			System.out.println("c> LIST_CONTENT OK");
		}
		else if (result == 1){ //caso de result = 1
			System.out.println("c> LIST_CONTENT FAIL, USER DOES NOT EXIST");
		}
		else if (result == 2){ //caso de result = 2
			System.out.println("c> LIST_CONTENT FAIL, USER NOT CONNECTED");
		}
		else if (result == 3){ //caso de result = 3
			System.out.println("c> LIST_CONTENT FAIL, REMOTE USER DOES NOT EXIST");
		}
		else { //resto de casos
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
			//si la lista de usuarios conectados esta vacia devuelve 2
			if(_connectedusers == null || _connectedusers.isEmpty()){
				result = 2;
			}
			else{
				//recorremos la lista de usuarios conectados
				int i = 0;
				while(i < _connectedusers.size()){
					//si encuentra el usuario en cuestion sale del bucle
					if(_connectedusers.get(i).username.equals(user_name)){
						break;
					}
					else{
						i++;
					}
				}
				//Si no encuentra al usuario devuelve 2 e imprime el siguiente error
				if(_connectedusers.size() == i){
					System.out.println("c> GET_FILE FAIL");
					return 2;
				}

				//almacenamos la ip y el puerto del usuario encontrado
				String ip = _connectedusers.get(i).ipadress;
				int puerto = _connectedusers.get(i).port;
				
				//creacion del socket
				Socket sc = new Socket(ip, puerto);
				DataOutputStream os = new DataOutputStream(sc.getOutputStream());
				DataInputStream in = new DataInputStream(sc.getInputStream());

				//enviamos la cadena GET_FILE junto con la cadena con el nombre del fichero remoto
				escribir(os, "GET_FILE");
				escribir(os, remote_file_name);

				//introducimos un sleep para darle tiempo al servidor local a enviar la respuesta antes de leerla
				Thread.sleep(500);

				//leemos la repuesta del servidor
				String respuesta = leer(in);
				result = Integer.parseInt(respuesta);
				
				//si el resultado es 0, copiamos el fichero remoto del socket al fichero local
				if(result == 0){
					//introducimos un sleep para dar tiempo al servidor de que comience a enviar el fichero
					Thread.sleep(500);
					OutputStream out = new FileOutputStream(local_file_name);
					DataOutputStream outputfile = new DataOutputStream(out);
					copy.copyFile(in,outputfile);
					//cierre del fichero 
					outputfile.close();
				}
				sc.close();
			}
			
		}
		catch(Exception e){
			System.err.println("excepcion " + e.toString());
			result = 2;
		}
		if(result == 0){	//caso de result = 0
			System.out.println("c> GET_FILE OK");
		}
		else if(result == 1){ //caso de result = 1
			System.out.println("c> GET_FILE FAIL, FILE DOES NOT EXIST");
		}
		else{ //resto de casos
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
		Getopt g = new Getopt("client", argv, "s:p:");

		int c;
		String arg;

		while ((c = g.getopt()) != -1) {
			switch(c) {

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
				if(_serverthread != null){
					System.out.println("\n+++ DISCONNECTING +++");
					disconnect(_connecteduser);	//Desconectamos al usuario para proteger la base de datos
				}
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
			//iniciamos la shell
			shell();
			//si el server esta activo lo desactiva
			if(_serverthread != null){
				System.out.println("\n+++ DISCONNECTING +++");
				disconnect(_connecteduser);	//Desconectamos al usuario para proteger la base de datos
			}
			System.out.println("+++ FINISHED +++");
		}
		catch(Exception e){
			System.out.println("+++ FINISHED +++");
			System.out.println("saliendo por la puerta de atrás");
		}
	}
}