package g16.userManagement;

public class Usuario {
	String name;
	String lastName1;
	String lastName2;
	String city;
    String username;
    String email;
    String password;

    public void setName(String value) {
        name = value;
    }
    
    public void setLastName1(String value) {
        lastName1 = value;
    }
    
    public void setLastName2(String value) {
        lastName2 = value;
    }
    
    public void setCity(String value) {
        city = value;
    }
    
    public void setUsername(String value) {
        username = value;
    }

    public void setEmail(String value) {
        email = value;
    }
    
    public void setPassword(String value) {
        password = value;
    }

	public String getName() {
		return name;
	}

	public String getLastName1() {
		return lastName1;
	}

	public String getLastName2() {
		return lastName2;
	}

	public String getCity() {
		return city;
	}

	public String getUsername() {
		return username;
	}

	public String getEmail() {
		return email;
	}

	public String getPassword() {
		return password;
	}

}