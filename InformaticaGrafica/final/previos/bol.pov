#include "colors.inc"
#include "stones.inc"

#declare base_exterior =
    sor {
        4, <2,0>, <1.75,1.1>, <1.7,2>, <2.8,2.4>
        sturm
    }

#declare base_interior =
    sor {
        4, <1.9,-0.1>, <1.65,1.0>, <1.6,1.9>, <2.7,2.3>
        sturm
    }

#declare base =
    difference {
        object { base_exterior }
        object { base_interior }
    }

#declare copa =
    difference {
        difference {
            sphere { <0, 4.5, 0>, 3 }
            box { <4, 3.5, -4>, <-4, 8.5, 4> }
        }
        sphere { <0, 4.5, 0>, 2.9 }
    }

#declare bol =
	merge {
		object { copa }
		object { base translate 0.015*y }
		translate -1.15*y
		texture { 
			pigment { color rgb<0.7,0.7,0.7> }
		}
		finish {
			reflection 0.1
			phong 0.1
			phong_size 0.2
			diffuse 0.2
			ambient 0.2
		}
	}
