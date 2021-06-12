// PoVRay 3.7 Scene File "ATree_sassafras_m_01.pov"
// author: Friedrich A. Lohmueller, Oct-2013
// demo file for using Albaro tree 
//  +w480 +h640
//
//--------------------------------------------------------------------------
#version 3.7;
//--------------------------------------------------------------------------
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "metals.inc"
#include "golds.inc"
#include "stones.inc"
#include "woods.inc"
#include "shapes.inc"
#include "shapes2.inc"
#include "functions.inc"
#include "math.inc"
#include "transforms.inc"

// tree textures: ---------------------------------------
#declare Stem_Texture = 
 texture{ pigment{ color rgb< 0.70, 0.5, 0.30>*0.25 } 
          normal { bumps 0.75 scale <0.025,0.075,0.025> }
          finish { phong 0.2 reflection 0.00}         
        } // end of texture 
//------------------------------------------------------- 
#declare Leaves_Texture_1 = 
 texture{ pigment{ color rgbf< 0.15, 0.36, 0.0, 0.1>  }   
          normal { bumps 0.15 scale 0.05 }
          finish { phong 1 reflection 0.00}
        } // end of texture 
//-------------------------------------------------------- 
#declare Leaves_Texture_2 = 
 texture{ pigment{ color rgbf< 0.05, 0.15, 0.0, 0.1>  }   
          normal { bumps 0.15 scale 0.05 }
          finish { phong 0.2 reflection 0.00}
        } // end of texture 
//-------------------------------------------------------- 
//--------------------------------------------------------------------------
#include "tree/sassafras_m.inc"
//#declare Tree_Height = sassafras_13_height; // ~23.29 ft
//--------------------------------------------------------------------------
// tree with leaves
#declare Tree =
   union{ 
          object{ sassafras_13_stems
                  texture{ Stem_Texture }
                } //------------------------
          object{ sassafras_13_leaves  
                  double_illuminate
                  texture{ Leaves_Texture_1 }   
                  interior_texture{ Leaves_Texture_2 }   
                } //------------------------
      scale 1/3 
      rotate <0,90,0>
      translate<0,0,0>
    } // end of union 
//--------------------------------------------------------------------------


/*
//--------------------------------------------------------------------------
// tree without leaves
   object{ sassafras_13_stems
           texture{ Stem_Texture }
           scale 1 
           rotate <0,45,0>
           translate <11,0,25>
        } //------------------------
//--------------------------------------------------------------------------
*/
