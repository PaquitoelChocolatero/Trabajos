import java.io.*;

public class copy
{
	static void copyFile(DataInputStream inputStream, DataOutputStream outputStream) throws IOException {
		try {
			byte[] buffer = new byte[8192];
			int length = 0;
			while ((length = inputStream.read(buffer)) > 0) {
				outputStream.write(buffer, 0, length);
			}
		}
		catch(Exception e){
			System.err.println("excepcion en copyfile " + e.toString());
			throw e;
		}
	}
}