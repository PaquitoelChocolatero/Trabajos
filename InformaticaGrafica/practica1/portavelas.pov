#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "glass.inc"
#include "metals.inc"

#declare prisma_triangular =
prism {
	linear_spline
	0, 5, 3,
	<0,4>, <3.5,-2>, <-3.5,-2>
}

#declare hex_medio =
prism {
	linear_spline
	0, 4, 6,
	<1.5,2.75>, <3.25,0>, <1.5,-2.75>, <-1.5,-2.75>, <-3.25,0>, <-1.5,2.75>
}

#declare hueco =
intersection {
	object { prisma_triangular }
	object {
		prisma_triangular
		scale 1.1*y
		translate -0.05*y
		rotate <0,180,0> 
	} 
}

#declare solido =
intersection {
	merge {
		object { prisma_triangular }
		object {
			prisma_triangular
			rotate <0,180,0>  
			scale 1.1*y
			translate -0.0000001*y
		}
		object {
			hex_medio
			translate 0.5*y
		}
	}
	prism {
		linear_spline
		-10, 10, 6,
		<2,-3.5>, <4,0>, <2,3.5>, <-2,3.5>, <-4,0>, <-2,-3.5>
	}
}

#declare portavelas =
difference {
	object { solido }
	object {
		hueco
		translate 1*y
		scale 1.0000001 * (x+z)
	}
	/*
	material {
		M_Glass
	}
	*/
	material {
		texture {
			pigment { rgbf<0.95, 1.0, 1.0, 0.8> }
			finish  {
				ambient 0.01
				diffuse 0.1
				reflection 0.1
				specular 1
				roughness 0.0003
				phong 1 
				phong_size 400
			}
		}
		interior { ior 1.5 }
	}
	normal{
		bumps 0.085
		scale .2
	}
}
