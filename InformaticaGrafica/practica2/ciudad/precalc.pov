#include "rand.inc"

#local n = 8;
#local base = 2;
#local lado = pow(base,n)+2;

#local height = array[lado][lado];
#local buffer = array[lado][lado];
#local height[1][1] = 0;
#local buffer[1][1] = 0;

#local suelo = 0;

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
	#for (jj, 0, ii-1)
		#for (kk, 0, ii-1)
			#local h = height[floor(jj/2)+1][floor(kk/2)+1];
			#local brownian = Rand_Gauss(0, 1, s);
			#local buffer[jj+1][kk+1] = h + brownian * intensity;
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

#debug "height = [\n"

#for (ii, 0, lado-1)
	#debug "["
	#for (jj, 0, lado-1)
		#debug str(height[ii][jj],0,-1)

		#if(jj<lado-1)
			#debug ","
			#if (mod(jj, 7) = 0)
				#debug "\n"
			#end
		#end
	#end
	#debug "]"

	#if(ii<lado-1)
		#debug ",\n"
	#else
		#debug "\n"
	#end
#end
#debug "]\n"
