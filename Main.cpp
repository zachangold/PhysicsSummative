//---------------------------------------------------------------------------

#include <windows.h>

#include "Renderer.h"
#include "InputSource.h"

#include "Box.h"
#include "Sphere.h"

#include "Graph.h"

#include "TrackVar.h"

#include "GravityChaos.h"
#include "simpleDemo.h"

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dinput.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma hdrstop

//---------------------------------------------------------------------------

const int NUM_SCENES = 9;

DynamicValue simTime;
DataSet *testDataSet = NULL;

float sceneArgs[ NUM_SCENES * 10 ];
int sceneNum = 1;

float getArg( int argNum )
{
    return sceneArgs[ ( sceneNum - 1 ) * 10 + argNum ];
};


float simulationTime = 0.0f;

void doNothing( void ) {};

void ( *sceneMainLoop )( void ) = doNothing;

Renderer *renderer;

Sphere *rocket, *earthCentre; 
Plane *ground;
GravityForce gravity;
Force thrust;

DynamicValue rocketDisp, rocketVel, rocketAccel, rocketMass;
DynamicValue valueTime;
bool rocketVarMass = false;

Force tmpGravity;

const int NUM_GRAPHS = 4;
Graph *g[ NUM_GRAPHS ] = { NULL, NULL, NULL, NULL };

char *sceneNames[ NUM_SCENES ] =
{
    // Simple Scenes
    "1) Intro",
    "2) Forces",
    "3) Forces & initial velocity",
    "4) Simple Rocket Question",

    // Complex Scenes
    "5) Variable Mass Rocket",
    "6) Variable Mass & Gravity Rocket",
    "7) Orbital Chaos",
    "8) Outtro",
    "9) Outtro 2.0"
};

void setupIntro( void )
{
    sceneMainLoop = doNothing;
    g[ 0 ]->draws = false;
    g[ 1 ]->draws = false;
    g[ 2 ]->draws = false;
    g[ 3 ]->draws = false;
};

LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

// return type is a floating point number, measured in metres
float tmpTrajectory( void *data )
{
    float dt = simulationTime;
    Entity *initial = ( Entity * ) data;

    // dt is the time since the object was dropped.
    // therefore, d = 1/2a(dt)^2 + v0(dt) + d0.

    return ( ( initial->getA() ) * dt * dt * 0.5 + Point( 0.0, 0.0, -5.0 ) ).getMagnitude();
};

float tmpGetDisplacement( void *data )
{
    Entity *e = ( Entity * ) data;
    return ( e->getD() + e->getD0() ).getMagnitude();
};

InputSource *input = NULL;

void baseSetup( int newScene )
{
    simulationTime = 0.0f;

    valueTime.trackData( &simulationTime );

    TrackVar *trackScene = new TrackVar();
    trackScene->setFormat( std::string( sceneNames[ newScene - 1 ] ) );
    trackScene->setTracker( NULL );
    trackScene->addToRenderer( renderer );

    TrackVar *trackTime = new TrackVar();
    trackTime->setFormat( "time: %f s" );
    trackTime->setTracker( &valueTime );
    trackTime->addToRenderer( renderer );

    sceneMainLoop = doNothing;

    simTime.trackData( &simulationTime );


    testDataSet = new DataSet();
    testDataSet->track( &simTime, DataSet::AXIS_X );
    //testDataSet->track( &simTime, DataSet::AXIS_Y );
    testDataSet->setup( renderer );

    for ( int i = 0; i < NUM_GRAPHS; ++i )
    {
        g[ i ] = new Graph();
        //g[ i ]->setTitle( "Sample Graph" );

        const float GRAPH_SIZE = SCREEN_HEIGHT / ( NUM_GRAPHS - 1 );
        #define GRAPH_SIZE_HALF ( GRAPH_SIZE / 2.0 )
        g[ i ]->setup( renderer );

        if ( i < 3 )
        {
            g[ i ]->setPos( SCREEN_WIDTH_HALF - GRAPH_SIZE_HALF, SCREEN_HEIGHT_HALF - GRAPH_SIZE_HALF - GRAPH_SIZE * i );
            g[ i ]->setSize( GRAPH_SIZE, GRAPH_SIZE );
        } else
        {
            g[ i ]->setPos( GRAPH_SIZE_HALF * 1.5 - SCREEN_WIDTH_HALF, GRAPH_SIZE_HALF * 1.5 - SCREEN_HEIGHT_HALF );
            g[ i ]->labelOnLeft = false;
            g[ i ]->setSize( GRAPH_SIZE * 1.5, GRAPH_SIZE * 1.5 );
        }
        g[ i ]->setDepth( 1.0f );
        //g[ i ]->setUnits( "time (s)", "displacement (m)" );
        //g[ i ]->addDataSet( testDataSet );
        g[ i ]->addToRenderer( renderer );
    }

};

void destroy( void )
{
    for ( int i = 0; i < NUM_GRAPHS; ++i )
    {
        if ( g[ i ] != NULL )
        {
            delete g[ i ];
            g[ i ] = NULL;
        }
    }

    DataSet::deleteAll();
    Entity::deleteAll();
    Graph::deleteAll();
    TrackVar::deleteAll();
    renderer->forget();
};

void reset( int newScene );

//const float ROCKET_EMPTY_MASS = 100.0; //kg
float ROCKET_EMPTY_MASS = 5.0; //kg
//const float ROCKET_FUEL_MASS = 5.0; //kg
float ROCKET_FUEL_MASS = 100.0; //kg
//const float ROCKET_FUEL_PER_SECOND = 1.0; //kg/s
float ROCKET_FUEL_PER_SECOND = 20.0; //kg/s
float ROCKET_FORCE = 1500; //N
float ROCKET_BURN_TIME = ROCKET_FUEL_MASS / ROCKET_FUEL_PER_SECOND;
float rocketGetMass( void )
{
    if ( !rocketVarMass )
    {
        return ROCKET_EMPTY_MASS + ROCKET_FUEL_MASS;
    }

    if ( simulationTime < ( ROCKET_FUEL_MASS / ROCKET_FUEL_PER_SECOND ) )
    {
        return ROCKET_EMPTY_MASS + ROCKET_FUEL_MASS - ROCKET_FUEL_PER_SECOND * simulationTime;
    } else
    {
        return ROCKET_EMPTY_MASS;
    }
};


float getMagnitude( void *data )
{
    Point *e = ( Point * ) data;

    return e->getMagnitude();
};

void rocketMainLoop( void );

void setupRocketProblem( bool varGravity, bool varMass )
{
    ROCKET_EMPTY_MASS = getArg( 0 ); //kg
    ROCKET_FUEL_MASS = getArg( 1 ); //kg
    ROCKET_FUEL_PER_SECOND = getArg( 2 ); //kg/s
    ROCKET_FORCE = getArg( 3 ); //N

    if ( varMass )
    {
        ROCKET_BURN_TIME = ROCKET_FUEL_MASS / ROCKET_FUEL_PER_SECOND;
    } else
    {
        ROCKET_BURN_TIME = getArg( 1 );
    }

    rocketVarMass = varMass;
    sceneMainLoop = rocketMainLoop;

    tmpGravity.dir = Point( 0.0, getArg( 4 ), 0.0 );
    tmpGravity.type = Force::FORCE_GRAVITY;

    simulationTime = 0.0;

    earthCentre = new Sphere();
    //earthCentre->setMass( 5.9736 * pow( 10, 24 ) );
    //earthCentre->setD0( 0.0, -6371000, 0.0 );
    earthCentre->setMass( getArg( 5 ) * pow( 10, 24 ) );
    earthCentre->setD0( 0.0, -getArg( 4 ), 0.0 );

    rocket = new Sphere();
    rocket->setMass( rocketGetMass() );
    rocket->setD0( 0.0, 1.0, 0.0 );

    ground = new Plane( 0.0, 0.0, 1.0 );
    renderer->addEntity( ground );

    gravity.set( rocket, earthCentre );
    rocket->addForce( varGravity ? &gravity : &tmpGravity );
    //earthCentre->addForce( &gravity );

    thrust.type = Force::FORCE_APPLIED;
    thrust.dir = Point( 0.0, ROCKET_FORCE, 0.0 );
    thrust.isTimed = true;
    thrust.timeLeft = ROCKET_BURN_TIME;
    rocket->addForce( &thrust );

    renderer->addEntity( rocket );

    //rocketDisp.trackFunc( getMagnitude, rocket->getDPtr() );
    //rocketVel.trackFunc( getMagnitude, rocket->getVPtr() );
    //rocketAccel.trackFunc( getMagnitude, rocket->getAPtr() );

    rocketDisp.trackData( &rocket->getDPtr()->v[ 1 ] );
    rocketVel.trackData( &rocket->getVPtr()->v[ 1 ] );
    rocketAccel.trackData( &rocket->getAPtr()->v[ 1 ] );
    rocketMass.trackData( rocket->getMassPtr() );

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

    TrackVar *trackM = new TrackVar();
    trackM->setFormat( "mass: %f kg" );
    trackM->setTracker( &rocketMass );
    trackM->addToRenderer( renderer );

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
    g[ 0 ]->setTitle( "rocket d-t graph" );
    g[ 0 ]->addDataSet( dSets[ 0 ] );

    dSets[ 1 ]->track( &simTime, DataSet::AXIS_X );
    dSets[ 1 ]->track( &rocketVel, DataSet::AXIS_Y );
    dSets[ 1 ]->setColor( 0.0, 0.0, 1.0, 1.0 );
    g[ 1 ]->addDataSet( dSets[ 1 ] );
    g[ 1 ]->setUnits( "time (s)", "v (m/s)" );
    g[ 1 ]->setTitle( "rocket v-t graph" );
    g[ 1 ]->addDataSet( dSets[ 1 ] );

    dSets[ 2 ]->track( &simTime, DataSet::AXIS_X );
    dSets[ 2 ]->track( &rocketAccel, DataSet::AXIS_Y );
    dSets[ 2 ]->setColor( 0.0, 1.0, 0.0, 1.0 );
    g[ 2 ]->addDataSet( dSets[ 2 ] );
    g[ 2 ]->setUnits( "time (s)", "a (m/s/s)" );
    g[ 2 ]->setTitle( "rocket a-t graph" );
    g[ 2 ]->addDataSet( dSets[ 2 ] );

    dSets[ 3 ]->track( &simTime, DataSet::AXIS_X );
    dSets[ 3 ]->track( &rocketMass, DataSet::AXIS_Y );
    dSets[ 3 ]->setColor( 0.0, 1.0, 1.0, 1.0 );
    g[ 3 ]->addDataSet( dSets[ 3 ] );
    g[ 3 ]->setUnits( "time (s)", "mass (kg)" );
    g[ 3 ]->setTitle( "rocket mass-time graph" );
    g[ 3 ]->addDataSet( dSets[ 3 ] );
};

void rocketMainLoop( void )
{
    rocket->setMass( rocketGetMass() );

    if ( simulationTime > 5.0 )
    {
        float vvel = rocket->getV()[ 1 ];
        float vdis = rocket->getPos()[ 1 ];
        int e = 0;
    }

    if ( rocket->getV()[ 1 ] < 0.0 )
    {
        float e = 0.0f;
    }

    // pause when the rocket hits the ground
    if ( rocket->collidesWith( ground ) && rocket->getV()[ 1 ] < 0.0 )
    {
        input->paused = true;
    }
};


void setupForceDemo( void )
{

};

int UPDATES_PER_TRACK = 10;

#pragma argsused
WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    FILE *cfg = fopen( "config.cfg", "r" );
    long fileLen = 0L;
    char *fileData = NULL;

    fseek( cfg, 0L, SEEK_END );
    fileLen = ftell( cfg );
    fseek( cfg, 0L, SEEK_SET );

    fileData = new char[ fileLen + 1 ];
    fileData[ fread( fileData, sizeof( char ), fileLen, cfg ) ] = 0;

    SCREEN_WIDTH = atoi( strtok( fileData, " \n" ) );
    SCREEN_HEIGHT = atoi( strtok( NULL, " \n" ) );
    UPDATES_PER_TRACK = atoi( strtok( NULL, " \n" ) );

    delete[] fileData;
    fclose( cfg );



    FILE *sceneArgFile = fopen( "args.cfg", "r" );
    fileLen = 0L;
    fileData = NULL;

    fseek( sceneArgFile, 0L, SEEK_END );
    fileLen = ftell( sceneArgFile );
    fseek( sceneArgFile, 0L, SEEK_SET );

    fileData = new char[ fileLen + 1 ];
    fileData[ fread( fileData, sizeof( char ), fileLen, sceneArgFile ) ] = 0;

    sceneArgs[ 0 ] = atof( strtok( fileData, " \n" ) );

    for ( int i = 1; i < NUM_SCENES * 10; ++i )
    {
        sceneArgs[ i ] = atof( strtok( NULL, " \n" ) );
    }

    delete[] fileData;
    fclose( sceneArgFile );
    


    HWND hWnd = NULL;

    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

    // fill in the struct with the needed information
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW;

    wc.lpfnWndProc = WindowProc;

    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = ( HBRUSH ) COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    // register the window class
    if ( FAILED ( RegisterClassEx( &wc ) ) )
    {
        return 0;
    }

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
                          "WindowClass1",    // name of the window class
                          "Physics Simulator",   // title of the window
                          WS_EX_TOPMOST | WS_POPUP,    // window style
                          0,    // x-position of the window
                          0,    // y-position of the window
                          SCREEN_WIDTH,    // width of the window
                          SCREEN_HEIGHT,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    if ( hWnd == NULL )
    {
        return 1;
    }

    // this struct holds Windows event messages
    MSG msg;

    input = new InputSource();
    input->init( &hWnd );

    renderer = new Renderer( hWnd, SCREEN_WIDTH, SCREEN_HEIGHT );
    renderer->setInputSource( input );


    reset( 1 );

    //setupRocketProblem();


    /*
    Force applied;
    applied.type = Force::FORCE_GRAVITY;
    applied.dir = Point( 0.0, -9.8067, 0.0 );

    
    Sphere *rolling = new Sphere();
    rolling->setD0( 0.0, 1.0, 5.0 );
    rolling->setMass( 1.0f );
    rolling->addForce( &applied );
    //rolling->setV0( 0.0, 0.0, 1.0 );

    Plane *floor = new Plane( 0.0, 2.0, 10.0 );



    renderer->addEntity( rolling );
    renderer->addEntity( floor );
    */



    //g.trackData( &simulationTime, Graph::AXIS_X );
    //g.trackFunction( (float (*)(void)) tmpGetMagnitude, Graph::AXIS_Y, ( void * ) sampleEntity->getV() );
    

    // display the window on the screen
    ShowWindow( hWnd, nCmdShow );

    const int UPDATES_PER_FRAME = 100;
    const int FRAMES_PER_SECOND = 60;
    const int UPDATES_PER_SECOND = UPDATES_PER_FRAME * FRAMES_PER_SECOND;
    const float SECONDS_PER_UPDATE = 1.0f / UPDATES_PER_SECOND;

    int numUpdatesPassed = 0;
    //const int UPDATE_LIMIT = 60000;

    while( true )
    {
        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            // Translate the message and dispatch it to WindowProc()
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

        input->updateMouse();

        int limit = UPDATES_PER_FRAME;
        if ( input->slowdown )
        {
            limit /= 10;
        } else if ( input->speedup )
        {
            limit *= 10;
        }

        for ( int u = 0; u < limit; ++u )
        {
            if ( !input->paused )
            {
                sceneMainLoop();

                Entity::updateAll( SECONDS_PER_UPDATE );

                if ( numUpdatesPassed % UPDATES_PER_TRACK == 0 )
                {
                    DataSet::updateAll();
                    Graph::updateAll();
                }

                simulationTime += SECONDS_PER_UPDATE;

                numUpdatesPassed++;
            }
        }

        renderer->draw( 1.0f / 60.0f );
        if ( msg.message == WM_QUIT || input->quit )
        {
            break;
        }

    }

    destroy();

    delete input;
    delete renderer;

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
};

LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    // sort through and find what code to run for the message given
        // this message is read when the window is closed

    if ( input != NULL )
    {
        if ( input->handleMessages( message, wParam, lParam ) )
        {
            return 0;
        }
    }

	switch ( message )                   // Check for windows messages
	{
		case WM_ACTIVATE:               // Watch for window activate message
		{
			if ( !HIWORD( wParam ) )    // Check minimization state
			{
				//active = true;  // Program is active
			}
			else
			{
				//active = false; // Program is no longer active
			}

			return 0;               // Return to the message loop
		}

		case WM_SYSCOMMAND:             // Intercept system commands
		{
			switch( wParam )         // Check system calls
			{
				case SC_SCREENSAVE:     // Screensaver trying to start?
				case SC_MONITORPOWER:	// Monitor trying to enter powersave?
				return 0;       // Prevent from happening
			}
			break;                  // Exit
		}
        case WM_DESTROY:
        {
            // close the application entirely
            PostQuitMessage( 0 );
            return 0;
        }
        case WM_SETCURSOR:
        {
            // Turn off window cursor

            SetCursor( NULL );

            //d3ddev->ShowCursor( TRUE );

            return TRUE; // prevent Windows from setting cursor to window class cursor
        }
	}


    // Handle any messages the switch statement didn't
    return DefWindowProc ( hWnd, message, wParam, lParam );
};


void reset( int newScene )
{
    destroy();
    baseSetup( newScene );

    sceneNum = newScene;

    // Do setup for each of the scenarios
    if ( newScene == 1 )
    {
        // Do an introduction - a blank screen!
        setupIntro();
    } else if ( newScene == 2 )
    {
        // Simple gravity problem; no acceleration except for gravity and one user-defined force, and v0 = 0
        // Variables include acceleration due to gravity and distance to fall
        simpleDemoSetup( false );
    } else if ( newScene == 3 )
    {
        // Simple trajectory problem; no acceleration except for gravity
        // Variables include acceleration due to gravity, initial velocity, and distance to fall
        // one user-defined applied force
        simpleDemoSetup( true );
    } else if ( newScene == 4 )
    {
        // Do the sample problem, that we did in class - constant mass and gravity
        // Now we have an initial acceleration that lasts for a set amount of time.
        // v0 = 0, total displacement = 0
        setupRocketProblem( false, false );
    } else if ( newScene == 5 )
    {
        // add more complexity to the problem, with variable mass as the rocket loses propellant
        setupRocketProblem( false, true );
    } else if ( newScene == 6 )
    {
        // add even more complexity by changing the gravitational pull of the earth
        setupRocketProblem( true, true );
    } else if ( newScene == 7 )
    {
        // Orbiting demonstrates chaos theory/chaotic motion - at the start, the orbit is stable,
        // but later on the orbit goes chaotic. Variables include initial states, mass, etc.
        gravityChaosSetup( 1 );
    } else if ( newScene == 8 )
    {
        // same as gravity chaos, except with 2 'heavy' objects
        gravityChaosSetup( true );
    } else if ( newScene == 9 )
    {
        // same as gravity chaos, except with 3 'heavy' objects
        gravityChaosSetup( 3 );
    }
};

//static LPDIRECTSOUND lpDS; //pointer to the directsound object

/* This function will initialize Direct Sound */
/* it returns TRUE if it was successful or FALSE otherwise */
/*
BOOL Sound_Init(void)
{
   if (DS_OK==DirectSoundCreate(NULL,&lpDS,NULL)) //create direct sound object
   {
      //ok, DirectSound Object created, let's take control now...
 
      if (DS_OK==IDirectSound_SetCooperativeLevel(lpDS, hMainWnd, DSSCL_EXCLUSIVE))
      {
         //MessageBox(hMainWnd,"Cooperative Level Succefully Set!","Good!!",MB_OK);
         return TRUE;
      }
      else //SetCoop was unsuccessful, let's give up control
      {
         IDirectSound_Release(lpDS);
         MessageBox(hMainWnd,"Set Cooperative Level didn't Work!","BAD!!",MB_OK);
         return FALSE;
      }
   }
   else  //DSObj creation was unsuccessful
   {
      MessageBox(hMainWnd,"Could Not Create Direct Sound Object","Error!",MB_OK);
      return (FALSE);
   }
}*/

/*here's a simple function to shut down direct sound when you're done with it */
/*
void Sound_Exit(void)
{
   if (lpDS)
   {
      IDirectSound_Release(lpDS);
   }
   else
   {
      MessageBox(hMainWnd,"Sound was not initialized and cannot Exit!","Error!",MB_OK);
   }
}
*/


//---------------------------------------------------------------------------
