#include "colors.inc"
#include "rand.inc"
#include "textures.inc"
#include "height.pov"

camera { location <-200,100,1000> look_at 0 angle 15 up<0,1,0> right<2,0,0>  translate z*50}

background { rgb <0.0,0.4,0.5> }

light_source { <0,1000,0>, rgb 1 }
light_source { <0,-500,0>, rgb 1 }

mesh2 {
	vertex_vectors {
		lado*lado,
		#for (jj, 0, lado-1)
			#for (kk, 0, lado-1)
				<jj,height[jj][kk],kk>,
			#end
		#end
	}
	face_indices {
		(lado-1)*(lado-1)*2,
		#local c = 0;
		#for (jj, 0, lado-1)
			#for (kk, 0, lado-1)
				#if (jj+1 < lado & kk+1 < lado)
					<jj*lado+kk, (jj+1)*lado+kk, jj*lado+(kk+1)>,
				#end
				#if (jj-1 >= 0 & kk-1 >= 0)
					<jj*lado+kk, (jj-1)*lado+kk, jj*lado+(kk-1)>,
				#end
			#end
		#end
	}
	material {
		M_NB_Beerbottle_Glass
	}
	translate (-lado/2)*(x+z)
}

plane { z, -1000 pigment { Bright_Blue_Sky  scale 1000 } }

plane {
	y, 0
	material {
		M_NB_Winebottle_Glass
	}
}
