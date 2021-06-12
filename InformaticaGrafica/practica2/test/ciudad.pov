#include "colors.inc"
#include "rand.inc"
#include "textures.inc"

camera { location <-200,200,1000> look_at 0 angle 18 up<0,1,0> right<1.5,0,0>  translate z*50}

background { rgb <0.0,0.4,0.5> }

light_source { <0,1000,0>, rgb 1 }
light_source { <0,-500,0>, rgb 1 }

#local n = 8;
#local base = 2;
#local lado = pow(base,n)+2;

#local height = array[lado][lado];
#local buffer = array[lado][lado];
#local height[1][1] = 0;
#local buffer[1][1] = 0;

#local suelo = -200;

#for (ii, 0, lado-1)
	#local buffer[0][ii]      = suelo;
	#local buffer[ii][0]      = suelo;
	#local buffer[lado-1][ii] = suelo;
	#local buffer[ii][lado-1] = suelo;
	#local height[0][ii]      = suelo;
	#local height[ii][0]      = suelo;
	#local height[lado-1][ii] = suelo;
	#local height[ii][lado-1] = suelo;
#end

#local s = seed(52);

#local intensity = 30;
#local base = 2;
#local epsilon = 0.5;

#local ii = 1;
#while (ii-1 < lado-2)
	#debug concat("Calculando alturas: Iteracion ", str(ii,0,0), "\n")
	#for (jj, 0, ii-1)
		#for (kk, 0, ii-1)
			#local h = height[floor(jj/2)+1][floor(kk/2)+1];
			#local brownian = Rand_Gauss(0, 1, s);
			#local buffer[jj+1][kk+1] = h + brownian*intensity;
		#end
	#end
	#for (jj, 0, ii-1)
		#for (kk, 0, ii-1)
			#local height[jj+1][kk+1] = buffer[jj+1][kk+1];
		#end
	#end
	#local ii = ii*base;
	#local intensity = intensity * epsilon;
#end

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
		M_NB_Winebottle_Glass
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
