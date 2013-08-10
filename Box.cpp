//---------------------------------------------------------------------------

#pragma hdrstop

#include "Box.h"

std::vector< Triangle > Box::boxModel = loadModel( "box.raw" );
const int Box::NUM_SIDES = 6;



//---------------------------------------------------------------------------
#pragma package(smart_init)
