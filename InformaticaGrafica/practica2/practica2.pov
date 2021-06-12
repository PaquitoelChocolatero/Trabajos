#include "colors.inc"
#include "textures.inc"
#include "sierp/pyramid.pov"
#include "previos/bol.pov"
#include "previos/bola.pov"
#include "previos/portavelas.pov"
#include "previos/ambientador.pov"

global_settings { ambient_light 0.9 }

#local cam_tilt = 22.5;
#local cam_rotate = 117;
#local cam_dist = 750;
#local cam_lookat = <50,25,350>;

#local cam_position = function {
	transform {
		scale cam_dist
		rotate z*cam_tilt
		rotate y*cam_rotate
		translate cam_lookat
	}
}

#local sun_tilt = 60;
#local sun_rotate = 60;
#local sun_dist = 100000;

#local sun_position = function {
	transform {
		scale sun_dist
		rotate z*sun_tilt
		rotate y*sun_rotate
	}
}

/*
// pájaro
camera {
	location <-170,270,270>
	look_at <-50,70,160>
	up<0,1,0> right<2,0,0>
}

// horizonte
camera {
	location <550,40,100>
	look_at <100,10,350>
	angle 35
	up<0,1,0> right<2,0,0>
}

// frente
camera {
	location <-500,25,-50>
	look_at <100,25,350>
	angle 35 
	up<0,1,0> right<2,0,0>
}
*/

// angular
camera {
	location cam_position(1, 0, 0)
	look_at  cam_lookat
	angle 55 
	up<0,1,0> right<1.77777,0,0>
}

// sol
light_source {
	sun_position(1,0,0), rgb <0.9,0.9,1>
}

// terreno
union {
	height_field {
		png "fields/terreno.png"
		scale (x+z)*4096+y*50
		translate -(x+z)*(2048)-y*25
		rotate y*90
		translate (x+z)*(512+1024)
	}
	plane { y, -50 }
	texture { T_Grnt25 scale 1000 }
}

// ciudad
height_field {
	png "fields/ciudad.png"
	translate <-0.5,0,-0.5>
	rotate -y*270
	scale <200,120,200>
	translate <50,-25,350>
	material {
		M_Ruby_Glass
	}
}

// diamante
union {
	object {
		Sierpinsky_Pyramid(6)
	}
	object {
		Sierpinsky_Pyramid(6)
		rotate x*180
		translate y*(-128)
	}
	texture {
		pigment {
			P_Wires(
				pigment {
					color rgb <0,1,0>
				},
				pigment {
					color rgbf <0.05,0.025,0.05,0.1>
				},
				0.1
			)
		}
	}
	rotate x*180
	translate <50,100,350>
}

// objetos previos
object { bol translate<-230,0,200> }
object { bola scale 1.2 translate <50,90,350> }
object { portavelas translate<230,5,200> }
object { ambientador translate<-200,0,370> }

// luces ambientales
light_source {
	<50,25,350>	color Red spotlight
	point_at <50,100,350>
	radius 30
	falloff 90
}
light_source {
	<50,100+60,350> color Green spotlight
	point_at <50,100,350>
	radius 40
	falloff 85
	tightness 5
}

// cielo
sky_sphere { pigment { Blue_Sky } }

// agua
plane {
	y, 0
	material {
		M_Dark_Green_Glass
	}
}
