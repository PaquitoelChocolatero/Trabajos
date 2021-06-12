#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "woods.inc"
#include "stones.inc"
#include "glass.inc"
#include "metals.inc"

#include "tree/ATree_sassafras_m_01.pov"
#include "roca.inc"

#default{ finish{ ambient <1,1,0.7>*1.2 diffuse 0.8 }} 
global_settings {
	assumed_gamma 0.8
	radiosity {
		count 10
		recursion_limit 5
		brightness 1.25
		media off
	}
	photons {
		spacing 1
		load_file "photons"
	}
}

background {
	rgb <0.2, 0.22, 0.53>
}

#local quad = polygon {
	4,
	<1, 0>, <1, 1>, <0, 1>, <0, 0>
}

light_source {
	#local sunlight = 10000*y;
	#local sunlight = vaxis_rotate(sunlight, z, 20);
	#local sunlight = vaxis_rotate(sunlight, x, 10);
	#debug vstr(3, sunlight, ",", 0, -1)
	#debug "\n"
	sunlight, rgb<1,1,0.9>
	parallel
	media_interaction off
	point_at 0
		photons {
			reflection on
			refraction on
		}
}
light_source {
	#local sunlight = 10000*y;
	#local sunlight = vaxis_rotate(sunlight, z, 20);
	#local sunlight = vaxis_rotate(sunlight, x, 10);
	#debug vstr(3, sunlight, ",", 0, -1)
	#debug "\n"
	sunlight, rgb<1,1,0.9>
	parallel
	media_interaction off
	point_at 0
}


// NORMAL
camera {
	#local view = z;
	location view
	look_at 0
	up <0,1301,0>
	right <1080,0,0>
	direction <0,0,1080>
	angle 90
	translate <0, 620, 510>
}

// AEREA
/*
camera {
	location <0,500,200>
	look_at <0, 0, 200>
	up <0,1,0>
	right <0.83,0,0>
	rotate y*180
}*/

#declare t_suelo = texture {
		pigment{
			image_map{ 
				jpeg "textures/marmol-crema.jpg"
				map_type 0
			}
			rotate 90*x
			scale 4000
		}
		finish {
			reflection 0
			diffuse 0.85
		}
	}

#declare t_pared = texture {
			pigment{
				image_map{ 
					jpeg "textures/hormigon.jpg"
					map_type 0
				}
				scale 2000
			}
			finish{
				ambient <1.5,1.5,1>
			}
		}

#declare t_azulejo = texture {
			pigment{
				image_map{ 
					jpeg "textures/aulejo.jpg"
					map_type 0
				}
				scale 1000
			}
			finish{
				reflection 0.07
			}
		}

#declare suelo =
	box {
		<-1483+5222+10, -600, 0+10>, <-1483-10, 0, -4748-10>
	}

#declare pared_derecha =
	object {
		quad
		rotate y*90
		scale <0, 3248, 5248>
		translate <-1483, -90, 500>
		texture{
			t_pared
			rotate 90*y
		}
	}

#declare rodapie_derecha =
	object {
		quad
		rotate y*90
		scale <0, 70, 4748>
		translate <-1482, 0, 0>
		texture{
			pigment{
				image_map{ 
					jpeg "textures/marmol-crema.jpg"
					map_type 0
				}
				rotate 90*y
			}
		}
	}

#declare pared_izquierda =
	object {
		quad
		rotate y*90
		scale <0, 3248, 4748>
		translate <-1483+5222, -90, 0>
		texture{
			t_pared
			rotate -90*y
		}
	}

#declare rodapie_izquierda =
	object {
		quad
		rotate y*90
		scale <0, 70, 4748>
		translate <-1483+5222-1, 0, 0>
		texture{
			pigment{
				image_map{ 
					jpeg "textures/marmol-crema.jpg"
					map_type 0
				}
				rotate -90*y
			}
		}
	}

#declare pared_fondo =
	object {
		quad
		scale <5222, 3248, 0>
		translate <-1483, -90, -4748>
		texture{
			t_pared
		}
	}

#declare rodapie_fondo =
	object {
		quad
		scale <5222, 70, 0>
		translate <-1483, 0, -4748+1>
		texture{
			pigment{
				image_map{ 
					jpeg "textures/marmol-crema.jpg"
					map_type 0
				}
			}
		}
	}

#declare pared_central =
	box {
		<-1483+5222, 1924, -2826>, <-698, 0, -2826-306>
		texture{
			t_pared
		}
	}

#declare rodapie_central =
	box {
		<-1483+5222+1, 70, -2826+1>, <-698-1, 0, -2826-306-1>
		texture{
			pigment{
				image_map{ 
					jpeg "textures/marmol-crema.jpg"
					map_type 0
				}
			}
		}
	}

#declare arbol =
	object{
		Tree
		scale 350
		translate <1539-288, 0, -2154-128>
	}

// piwi
#declare piwi = 
	box{
		<-1483+320, 2, -2308>, <534, -590, 0>
		texture { 
			pigment {
				rgb <0.8, 0.7, 0.63>*0.75
			}
		}
	}

#declare agua =
	box{
		<-1483+320, -70, -2308>, <534, -600, 0>
		material{ 
			texture {
				pigment { rgbf <0.66, 0.83, 0.75, 1>*0.5 }
				finish  {
					ambient 0
					diffuse 0.7
					reflection 0.35
					//specular 0.8
					roughness 0.0003
					//phong 1 
					//phong_size 400
				 }
				 normal {
					 bumps
					 scale <200,10,200>
				 }
			}
			interior {ior 1.33}
		}
		photons {
			target 
			reflection on
			refraction on
		}
	}

#declare azulejos = 
	box {
		<-1483, 0, -2193>, <-1481, 848, 0>
		texture {t_azulejo rotate 90*y}
	}

	// parterre
#declare parterre =
	box{
		<1539, -10, -2154>, <1539-576, 2, -2145-256>
		texture{
			pigment{
				image_map{ 
					jpeg "textures/tierra.jpg"
					map_type 0
				}
				rotate 90*x
				scale 50
			}
		}
	}

#declare piedra =
	object {
		piedra
		texture {
			pigment { color rgb <0.75, 0.75, 0.75> }
			finish { ambient 0.1 diffuse 0.6 phong 0.0}
		}
		scale 50
		rotate z*40
		translate <-1300, 20, -1000>
	}

#declare piedras =
	object{ piedra }
	object{ 
		piedra
		translate -100*z + 100*x
	}
	object{ 
		piedra
		translate -200*z
	}
	object{ 
		piedra
		translate -300*z + 50*x
	}
	object{ 
		piedra
		translate -400*z
	}
	object{ 
		piedra
		translate -300*z + 30*y
	}
	object{ 
		piedra
		translate -200*z + 30*y + 30*x
	}
	object{ 
		piedra
		translate -100*z + 30*y
	}
	object{ 
		piedra
		translate -400*z + 60*y + 20*x
	}
	object{ 
		piedra
		translate -100*z + 60*y + 50*x
	}
	object{ 
		piedra
		translate -200*z + 60*y
	}
	object{ 
		piedra
		translate -100*z + 60*y
	}
	object{ 
		piedra
		translate 60*y + 20*x
	}



	object{
		union{
			difference{
				object { suelo }
				object { piwi }
				object { parterre }
				texture{ t_suelo }
			}
			object{ pared_derecha }
			object{ rodapie_derecha }
			object{ pared_izquierda }
			object{ rodapie_izquierda }
			object{ pared_fondo }
			object{ rodapie_fondo }
			object{ pared_central }
			object{ rodapie_central }
			object{ azulejos }
			object{ agua }
			object{ arbol }
			object{ piedras }
		}
	}

	object {
		quad
		scale 400*u+1250*v
		rotate -90*y
		translate <1200,6200,-200>
		texture {
			pigment {
				rgb 1
			}
			finish {
				reflection 1
				specular 1
				diffuse 0
			}
			normal {
				waves
				scale <0.5,10,10>
			}
		}
		photons {
			target
			reflection on
			refraction off
		}
	}
