#include "rand.inc"

#local n = 10;
#local s = seed(8);
#local epsilon = 0.6;
#local intensity = 30;
#local lado = pow(2,n)+1;
#local salto = pow(2, n-1);
#local height = array[lado][lado];
#local suelo = 0;

// Inicializar la matriz a 0
#for (ii, 0, lado-1)
	#local height[0][ii]      = Rand_Gauss(0,1,s) * intensity;
	#local height[ii][0]      = Rand_Gauss(0,1,s) * intensity;
	#local height[lado-1][ii] = Rand_Gauss(0,1,s) * intensity;
	#local height[ii][lado-1] = Rand_Gauss(0,1,s) * intensity;
#end

#macro assign_height(ii, jj, value)
	#if (ii = 0 | jj = 0 | ii = lado-1 | jj = lado-1)
		#declare height[ii][jj] = suelo;
	#else
		#declare height[ii][jj] = value;
	#end
#end

//Definir la altura iterativamente
#while (salto > 0)
	#local intensity = intensity * epsilon;
	#for (ii, 0, lado-salto-1, salto*2)
		#for (jj, 0, lado-salto-1, salto*2)

			// Cuadro con esquina superior izquierda en (ii, jj)
			#local x1 = height[ii][jj];
			#local x2 = height[ii+salto*2][jj];
			#local x3 = height[ii][jj+salto*2];
			#local x4 = height[ii+salto*2][jj+salto*2];
			
			// Diamante
			assign_height(ii+salto, jj+salto, (x1 + x2 + x3 + x4)/4 + Rand_Gauss(0,1,s) * intensity)

			// Cuadrado
			assign_height(ii+salto, jj, (x1 + x2) / 2 + Rand_Gauss(0,1,s) * intensity) 		 	// ARRIBA
			assign_height(ii, jj+salto, (x1 + x3) / 2 + Rand_Gauss(0,1,s) * intensity) 		 	// IZQUIERDA
			assign_height(ii+salto, jj+salto*2, (x3 + x4) / 2 + Rand_Gauss(0,1,s) * intensity) 	// ABAJO
			assign_height(ii+salto*2, jj+salto, (x2 + x4) / 2 + Rand_Gauss(0,1,s) * intensity) 	// DERECHA
		#end
	#end

	#local salto = floor(salto / 2);
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
