#include "woods.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"

#declare paja = cylinder {
	<0,0,0>, <0,7.5,0>, 0.05
	texture {
		pigment {
			color rgb <0.15,0.70,0.05>
		}
	}
}

#declare bote_alt = 2.4;
#declare bote_rad = 0.85;
#declare bote_gro = 0.05;
#declare bote_int_rad = bote_rad - bote_gro;

#declare bote_ext = cylinder { 0 <0, bote_alt, 0> bote_rad }
#declare bote_int = union {
	cylinder {
		<0, bote_alt + 0.1, 0>
		<0, bote_alt - 0.1, 0>
		0.25
	}
	cylinder {
		<0, bote_gro, 0>
		<0, bote_alt - bote_gro, 0>
		bote_rad - bote_gro
	}
}

#declare bote = union {
	difference {
		object { bote_ext }
		object { bote_int }
		material { M_Glass3 }
	}
	cylinder {
		<0, bote_gro, 0>
		<0, bote_gro + 0.1, 0>
		bote_int_rad
		/*
		material {
			texture {
				Dark_Green_Glass 
				pigment { rgbf <0.1, 0.4, 0.1, 0.5> }
			} 
			interior {Glass_Interior}
		} */
		material{
			texture{
				pigment{Col_Glass_Winebottle}
				finish {F_Glass1}
			}
			interior{
				ior 1.5
			}
		}
	}
}


#declare caja_dim = <3, 3, 2>;
#declare caja_cen = caja_dim / 2;
#declare caja = difference {
	box { <0,0,0>, caja_dim }
	union {
		// z hole
		cylinder {
			<caja_dim.x/2, caja_dim.y/2, -0.1>
			<caja_dim.x/2, caja_dim.y/2, caja_dim.z+0.1>
			1
		}
		// y hole
		cylinder {
			<caja_dim.x/2, caja_dim.y-0.6, caja_dim.z/2>
			<caja_dim.x/2, caja_dim.y+0.1, caja_dim.z/2>
			0.375
		}
		object {
			bote_ext
			translate <caja_dim.x/2, 0.5, caja_dim.z/2>
		}
	}
	texture {
		T_Wood13
		rotate y * 90
	}
}

#declare bote_centro = <caja_dim.x/2, 0.5 + bote_gro, caja_dim.z/2>;
#declare ambientador = union {
	object { caja }
	object { bote translate <caja_dim.x/2, 0.5, caja_dim.z/2> }
	object {
		paja rotate x * 21 rotate y * 23
		translate bote_centro + (vaxis_rotate(x, y, 110) * (bote_int_rad - 0.1))
	}
	object {
		paja rotate x * 21 rotate y * -33
		translate bote_centro + (vaxis_rotate(x, y,  55) * (bote_int_rad - 0.1))
	}
	object {
		paja rotate x * 19 rotate y * -86
		translate bote_centro + (vaxis_rotate(x, y, 15) * (bote_int_rad - 0.1))
	}
}
