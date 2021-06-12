#include "shapes_old.inc"
#include "shapes2.inc"
#include "shapes3.inc"
#include "textures.inc"

#macro Sierpinsky_Pyramid(N)
	#local faces = object { Pyramid_N (4, 1, 0, 1 ) }

	#local ff = 1;
	#for (ii, 1, N)
		#local faces = union {
			object { faces }
			object { faces translate< ff,-ff, ff> }
			object { faces translate< ff,-ff,-ff> }
			object { faces translate<-ff,-ff, ff> }
			object { faces translate<-ff,-ff,-ff> }
		}
		#local ff = ff * 2;
	#end
	object { faces translate <0,-1,0> }
#end

// Wired texture
#macro P_Wires(F, B, W)
	boxed scale <2, 0.6,2> translate <1,0.5,1>
	warp { repeat y }
	warp { repeat 2*x }
	warp { repeat 2*z }

	#local T = pigment {
		Tiles_Ptrn()
		pigment_map {
			[W/2 F]
			[W/2 B]
		}
		rotate x*90
	}

	pigment_map {
		[W F]
		[W T scale sqrt(2) rotate y*45]
	}
	translate <-1,0,-1>
#end
