//---------------------------------------------------------------------------

#pragma hdrstop

#include "simpleDemo.h"

void simpleDemoSetup( bool hasV0 );
void simpleDemoMainLoop( void );

extern void ( *sceneMainLoop )( void );
extern float simulationTime;
//extern GravityForce gravity;
extern Renderer *renderer;
extern Graph *g[];
extern InputSource *input;

extern DynamicValue rocketDisp, rocketVel, rocketAccel, simTime;
extern float getMagnitude( void *data );

Force gravity;
Force applied;

AirDragForce drag;
Sphere *object;
Plane *ground;

void simpleDemoSetup( bool hasV0 )
{
    sceneMainLoop = simpleDemoMainLoop;

    object = new Sphere();
    object->setMass( getArg( 5 ) );
    object->setD0( 0.0, getArg( 0 ) + 1, 0.0 );

    if ( hasV0 )
    {
        object->setV0( getArg( 6 ), getArg( 7 ), getArg( 8 ) );
    }

    ground = new Plane( 0.0, 0.0, 2.0 );

    gravity.dir = Point( 0.0, getArg( 1 ), 0.0 );
    gravity.type = Force::FORCE_GRAVITY;

    applied.dir = Point( getArg( 2 ), getArg( 3 ), getArg( 4 ) );
    applied.type = Force::FORCE_APPLIED;

    object->addForce( &gravity );
    //object->addForce( &applied );

    drag.set( object, 0.5, 1.3, 0.01 );
    object->addForce( &drag );

    renderer->addEntity( object );
    renderer->addEntity( ground );

    rocketDisp.trackFunc( getMagnitude, object->getDPtr() );
    rocketVel.trackFunc( getMagnitude, object->getVPtr() );
    rocketAccel.trackFunc( getMagnitude, object->getAPtr() );

    TrackVar *trackD = new TrackVar();
    trackD->setFormat( "d: %f m" );
    trackD->setTracker( &rocketDisp );
    trackD->addToRenderer( renderer );

    TrackVar *trackV = new TrackVar();
    trackV->setFormat( "v: %f m/s" );
    trackV->setTracker( &rocketVel );
    trackV->addToRenderer( renderer );

    TrackVar *trackA = new TrackVar();
    trackA->setFormat( "a: %f m/s/s" );
    trackA->setTracker( &rocketAccel );
    trackA->addToRenderer( renderer );

    DataSet *dSets[ 4 ];

    for ( int i = 0; i < 4; ++i )
    {
        dSets[ i ] = new DataSet();
        dSets[ i ]->setup( renderer );
    }

    dSets[ 0 ]->track( &simTime, DataSet::AXIS_X );
    dSets[ 0 ]->track( &rocketDisp, DataSet::AXIS_Y );
    dSets[ 0 ]->setColor( 1.0, 0.0, 0.0, 1.0 );
    g[ 0 ]->addDataSet( dSets[ 0 ] );
    g[ 0 ]->setUnits( "time (s)", "d (m)" );
    g[ 0 ]->setTitle( "object d-t graph" );
    g[ 0 ]->addDataSet( dSets[ 0 ] );

    dSets[ 1 ]->track( &simTime, DataSet::AXIS_X );
    dSets[ 1 ]->track( &rocketVel, DataSet::AXIS_Y );
    dSets[ 1 ]->setColor( 0.0, 0.0, 1.0, 1.0 );
    g[ 1 ]->addDataSet( dSets[ 1 ] );
    g[ 1 ]->setUnits( "time (s)", "v (m/s)" );
    g[ 1 ]->setTitle( "object v-t graph" );
    g[ 1 ]->addDataSet( dSets[ 1 ] );

    dSets[ 2 ]->track( &simTime, DataSet::AXIS_X );
    dSets[ 2 ]->track( &rocketAccel, DataSet::AXIS_Y );
    dSets[ 2 ]->setColor( 0.0, 1.0, 0.0, 1.0 );
    g[ 2 ]->addDataSet( dSets[ 2 ] );
    g[ 2 ]->setUnits( "time (s)", "a (m/s/s)" );
    g[ 2 ]->setTitle( "object a-t graph" );
    g[ 2 ]->addDataSet( dSets[ 2 ] );
};


void simpleDemoMainLoop( void )
{
    // pause when the rocket hits the ground
    if ( object->collidesWith( ground ) && object->getV()[ 1 ] < 0.0 )
    {
        input->paused = true;
    }
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
