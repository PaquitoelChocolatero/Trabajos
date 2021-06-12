#include "colors.inc"
#include "textures.inc"

#declare m_bola = 
material {
	interior {
		ior 1.3
		caustics 1000
		fade_distance 4
		fade_power 1
	}
	texture {
		pigment { rgbf <0.8, 0.2, 0.05, 0.8> }
		finish  {
			Glass_Finish
			ambient 0.1
		}
	}
}

#declare nbur = 350;
#declare bola = difference {
	sphere { 0, 1.2 }
	union {
		#local s = seed(10);
		#for ( i, 0, nbur )
			// https://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability
			#local x1 = (rand(s)*2) - 1;
			#local x2 = (rand(s)*2) - 1;
			#local x3 = (rand(s)*2) - 1;
			#local ur = rand(s) * 1.15;
			#local vec = vnormaliz e(<x1, x2 x3>) * pow(ur, 1/3);
			sphere { vec, (rand(s)) * 0.04 + 0.01 }
		#end
	}
	translate y * 1.2
	material { m_bola }
}
