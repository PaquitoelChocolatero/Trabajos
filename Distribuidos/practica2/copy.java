import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class copy{

    static void copyFile(String src, String dest) throws IOException {
    	InputStream inputStream = null;
    	OutputStream outputStream = null;
    	try {
		    File infile =new File(src);
    	   	File outfile =new File(dest);
       		inputStream = new FileInputStream(src);
       		outputStream = new FileOutputStream(dest);

       		byte[] buffer = new byte[1024];
       		int length = 0;
       		while ((length = inputStream.read(buffer)) > 0) {
           		outputStream.write(buffer, 0, length);
       		}
   		} finally {
       		inputStream.close();
       		outputStream.close();
   		}
	}

    public static void main(String[] args){
		try {
			copyFile(args[0], args[1]);
		}catch(IOException e){
	    	e.printStackTrace();
		}
	}
}