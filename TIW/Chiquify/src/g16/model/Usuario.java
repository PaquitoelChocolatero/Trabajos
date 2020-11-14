package g16.model;

import java.io.Serializable;
import javax.persistence.*;

import java.util.List;


/**
 * The persistent class for the usuario database table.
 * 
 */
@Entity
@NamedQuery(name="Usuario.findAll", query="SELECT u FROM Usuario u")
public class Usuario implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue(strategy=GenerationType.AUTO)
	private String email;

	private String apellido1;

	private String apellido2;

	private String ciudad;

	private String nombre;

	private String passwd;

	//bi-directional many-to-one association to Producto
	@OneToMany(mappedBy="usuario1")
	private List<Producto> productos1;

	//bi-directional many-to-one association to Producto
	@OneToMany(mappedBy="usuario2")
	private List<Producto> productos2;

	public Usuario() {
	}

	public String getEmail() {
		return this.email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getApellido1() {
		return this.apellido1;
	}

	public void setApellido1(String apellido1) {
		this.apellido1 = apellido1;
	}

	public String getApellido2() {
		return this.apellido2;
	}

	public void setApellido2(String apellido2) {
		this.apellido2 = apellido2;
	}

	public String getCiudad() {
		return this.ciudad;
	}

	public void setCiudad(String ciudad) {
		this.ciudad = ciudad;
	}

	public String getNombre() {
		return this.nombre;
	}

	public void setNombre(String nombre) {
		this.nombre = nombre;
	}

	public String getPasswd() {
		return this.passwd;
	}

	public void setPasswd(String passwd) {
		this.passwd = passwd;
	}

	public List<Producto> getProductos1() {
		return this.productos1;
	}

	public void setProductos1(List<Producto> productos1) {
		this.productos1 = productos1;
	}

	public Producto addProductos1(Producto productos1) {
		getProductos1().add(productos1);
		productos1.setUsuario1(this);

		return productos1;
	}

	public Producto removeProductos1(Producto productos1) {
		getProductos1().remove(productos1);
		productos1.setUsuario1(null);

		return productos1;
	}

	public List<Producto> getProductos2() {
		return this.productos2;
	}

	public void setProductos2(List<Producto> productos2) {
		this.productos2 = productos2;
	}

	public Producto addProductos2(Producto productos2) {
		getProductos2().add(productos2);
		productos2.setUsuario2(this);

		return productos2;
	}

	public Producto removeProductos2(Producto productos2) {
		getProductos2().remove(productos2);
		productos2.setUsuario2(null);

		return productos2;
	}

}