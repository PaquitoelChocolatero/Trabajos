package g16.handler;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public interface RequestHandler {
	String process(HttpServletRequest request, HttpServletResponse response);
}