#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "woods.inc"
#include "stones.inc"
#include "glass.inc"
#include "metals.inc"

// Frente
camera { location <0, 100, -200> look_at 0 }
//camera { location <0, 100, 0> look_at <0, 30, 90> }

// Atrás
//camera { location <0, 100, 200> look_at 0 }

// Lateral izq
//camera { location <-200, 100, 0> look_at 0 }

// Lateral dcha
//camera { location <200, 100, 0> look_at 0 }

// Esquina
//camera { location <60, 50, -90> look_at <60, 40, -70> }

light_source {
	#local sunlight = 10000*y;
	#local sunlight = vaxis_rotate(sunlight, z, 20);
	#local sunlight = vaxis_rotate(sunlight, x, -30);
	#debug vstr(3, sunlight, ",", 0, -1)
	#debug "\n"
	sunlight, rgb<1,1,0.9>
	parallel
	point_at 0
}

background {
	rgb <0.2, 0.22, 0.53>
}

#declare t_suelo = texture {
		pigment{
			image_map{ 
				jpeg "texturas/cesped.jpg"
				map_type 0
			}
			rotate 90*x
			scale 70
		}
		finish {
			reflection 0
			diffuse 0.85
		}
	}

#declare suelo =
	box {
		<100, -1, 100>, <-100, 0, -100>
        texture{t_suelo}
	}

#declare ventanas =	
	union {
		#macro position (xx, yy, rr, side)
			#switch (side)
				#case (0) // -z
					<xx, yy, -rr>
				#break
				#case (1) // +x
					<rr, yy, xx>
				#break
				#case (2) // +z
					<xx, yy, rr>
				#break
				#case (3) // -x
					<-rr, yy, xx>
				#break
			#end
		#end

		#local ventana =
			box {
				<-1.1, 0, -1>, <1.1, 5, 1>
			}

		#local primera_planta = 8;
		#local segunda_planta = 14.5;
		#local tercera_planta = 21.5;
		#local planta_baja = 2;

		#for (side, 0, 3)
			// ventanas centro
			object { ventana translate position(0, primera_planta, 70, side) }
			object { ventana translate position(0, segunda_planta, 70, side) }
			object { ventana scale y*3/5 translate position(0, tercera_planta, 70, side) }
			

			// ventanas a los lados
			#local sep = 6;
			#local inicial = 0.5;
			#for (i, 1, 7) 
				#for (s, -1, 1, 2)
					#local xx = s*(inicial+sep*i);
					object { ventana translate position(xx, primera_planta, 70, side) }
					object { ventana translate position(xx, segunda_planta, 70, side) }
					object { ventana scale y*3/5 translate position(xx, tercera_planta, 70, side) }
					object { ventana scale y*4/5 translate position(xx, planta_baja, 70, side) }
				#end
			#end

		#end

		pigment{rgb <1, 1, 1>}
	}

#declare puertas = 
	union {
		box{
			<-2.5, 0, -71>, <2.5, 6, -69>
		}
		pigment { rgb 1 }
	}

#declare tejadoExterior =
	prism {
		linear_spline
		0, 140, 4,
		<0, 0>, <25, 0>, <12.5, 10>, <0, 0>
		rotate -90*x + 90*y
		pigment{Red}
		translate <70, 30, 70>
	}

#declare tejadoInterior =
	prism {
		linear_spline
		0, 90, 4,
		<0, 0>, <5, 0>, <0, 5>, <0, 0>
		rotate -90*x + 90*y
		pigment{Red}
		translate <45, 25, 45>
	}

#declare tejadoEsquina =
	prism {
		conic_sweep
		linear_spline
		0, 0.5, 5,
		<4,4>,<-4,4>,<-4,-4>,<4,-4>,<4,4>
		rotate 180*x
		scale <6.75, 25, 6.75>
		translate <58.5, 42.5, -58.5>
		pigment { Red }
	}

#declare sabatini =
	union{
		// Anillo
		union{
			difference{
				box{
					<70, 0, 70>, <-70, 30, -70>
					pigment{rgb <1, 1, 1>}
				}
				union {
					box{
						<45, -10, 45>, <-45, 50, -45>
						pigment{rgb <1, 1, 1>}
					}
					object { ventanas }
					object { puertas }
				}
			}
			//Tejados
			object{
				tejadoExterior
				scale 0.8*x
			}
			object{
				tejadoExterior
				scale 0.8*x
				translate -115*z
			}
			object{
				tejadoExterior
				scale 0.8*x
				rotate -90*y
			}
			object{
				tejadoExterior
				scale 0.8*x
				rotate -90*y
				translate 115*x
			}

			object{tejadoInterior}
			object{
				tejadoInterior
				rotate -90*y
			}
			object{
				tejadoInterior
				rotate 90*y
			}
			object{
				tejadoInterior
				rotate 180*y
			}
			
			// Esquinas
			object{tejadoEsquina}
			object{
				tejadoEsquina
				translate 117*z
			}
			object{
				tejadoEsquina
				translate -117*x
			}
			object{
				tejadoEsquina
				translate 117*z -117*x
			}
		}
		// Esquinas
		box{
			<72, 0, -72>, <45, 30, -45>
			pigment{rgb <1, 1, 1>}
		}
		box{
			<-72, 0, 72>, <-45, 30, 45>
			pigment{rgb <1, 1, 1>}
		}
		box{
			<72, 0, 72>, <45, 30, 45>
			pigment{rgb <1, 1, 1>}
		}
		box{
			<-72, 0, -72>, <-45, 30, -45>
			pigment{rgb <1, 1, 1>}
		}
		// Anillo interno
		difference{
			box{
				<45, 0, 45>, <-45, 25, -45>
				pigment{rgb <1, 1, 1>}
			}
			box{
				<40, -10, 40>, <-40, 50, -40>
				pigment{rgb <1, 1, 1>}
			}
		}
	}

object{
	union{
		object{suelo}
		object{sabatini}
	}
}
