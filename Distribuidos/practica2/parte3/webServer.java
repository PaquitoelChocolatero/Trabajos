package parte3;

import metro.javax.jws.WebService;
import metro.javax.jws.WebMethod;
import metro.javax.xml.ws.Endpoint;

@WebService
public class webServer {
    @WebMethod
    public String conversion(String cadena) {
        return cadena.toUpperCase();
    }

    public static void main(String[] args) {
        Endpoint.publish("http://localhost:8080/SistemasDistribuidos", new webServer());
    }
}
