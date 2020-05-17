package parte3;

import javax.jws.WebService;
import javax.jws.WebMethod;
import javax.xml.ws.Endpoint;

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
