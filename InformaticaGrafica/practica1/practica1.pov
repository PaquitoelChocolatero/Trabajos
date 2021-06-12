#include "colors.inc"
#include "ambientador.pov"
#include "bola.pov"
#include "bol.pov"
#include "portavelas.pov"

camera { location <20, 2, 0> look_at 0 }
global_settings { ambient_light White * 1.5 }

#local view = vaxis_rotate(x, z, 24);
#local view = vaxis_rotate(view, y, 95);

camera { location <-2,5,-6> look_at <3,3,0> }
camera { location y * 25 look_at 0 rotate 16 * y }
camera {
	angle 14.75
	location view * 40
	look_at 0
	translate 2.75*y
	translate 0.3*x
	up <0, 1.2, 0>
	right <1, 0, 0>
}

#local sun = vaxis_rotate(x, z, 40);
#local sun = vaxis_rotate(sun, y, -79);

#for ( i, 0, 2 )
	light_source {
		sun*10000
		color rgb<1,1,0.9>
	}
#end

/*
light_source {
	sun, color rgb<1,1,0.9>
	parallel
	point_at 0
}
*/

polygon {
	4, <1,0,1>, <-1,0,1>, <-1,0,-1>, <1,0,-1>
	scale (x+z) * 13
	scale z*0.75
	translate -0.01*y
	texture { pigment { color White } }
}

polygon {
	4, <1,0,1>, <-1,0,1>, <-1,1,1>, <1,1,1>
	scale x*8
	scale y*7.5
	translate z*8.75
	translate x*-1
	texture { pigment { color rgb <0.13,0.15,0.11> } }
}

object { ambientador rotate y * 20 translate <-0.5,0,-1> }
object { bola scale 1.4 translate -x*3 -z*3}
object { portavelas scale 0.7 scale y * 1.2 rotate 196*y translate <-1.2,0,3.4> }
object { bol scale 0.65 scale 1.08*y translate -4.8 * z + 2.7 * x}
