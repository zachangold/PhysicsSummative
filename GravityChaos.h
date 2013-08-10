//---------------------------------------------------------------------------

#ifndef GravityChaosH
#define GravityChaosH

#include "Util.h"
#include "Sphere.h"
#include "Renderer.h"
#include "Graph.h"
#include "TrackVar.h"


extern float getArg( int argNum );

void gravityChaosSetup( int numHeavies );
void gravityChaosMainLoop( void );

//---------------------------------------------------------------------------
#endif
