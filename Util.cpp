//---------------------------------------------------------------------------

#pragma hdrstop

#include "Util.h"
#include "Entity.h"

int SCREEN_WIDTH = 1680;
int SCREEN_HEIGHT = 1050;
const float UI_BORDER = 10.0;

const int MOUSE_LEFT = 1;
const int MOUSE_RIGHT = 2;
const int MOUSE_MIDDLE = 3;
const int MOUSE_SCROLLUP = 4;
const int MOUSE_SCROLL_DOWN = 5;


UIPoint uiSquare[ 6 ] =
{
    // Top-left triangle
    UIPoint( 0.0, 0.0, 0.0, 1.0, 0.25, 0.25, 0.25 ),
    UIPoint( 0.0, 1.0, 0.0, 1.0, 0.25, 0.25, 0.25 ),
    UIPoint( 1.0, 0.0, 0.0, 1.0, 0.25, 0.25, 0.25 ),

    // Bottom-right triangle
    UIPoint( 1.0, 1.0, 0.0, 1.0, 0.25, 0.25, 0.25 ),
    UIPoint( 0.0, 1.0, 0.0, 1.0, 0.25, 0.25, 0.25 ),
    UIPoint( 1.0, 0.0, 0.0, 1.0, 0.25, 0.25, 0.25 )
};


Point::Point( float x, float y, float z )
{
    this->v[ 0 ] = x;
    this->v[ 1 ] = y;
    this->v[ 2 ] = z;
};

Point::Point( void )
{
    this->v[ 0 ] = 0.0f;
    this->v[ 1 ] = 0.0f;
    this->v[ 2 ] = 0.0f;
};


Triangle::Triangle( void )
{
    memset( this, 1, sizeof( Triangle ) );
};

Triangle::Triangle( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3 )
{
    p[ 0 ].v[ 0 ] = x1;
    p[ 0 ].v[ 1 ] = y1;
    p[ 0 ].v[ 2 ] = z1;

    p[ 1 ].v[ 0 ] = x2;
    p[ 1 ].v[ 1 ] = y2;
    p[ 1 ].v[ 2 ] = z2;

    p[ 2 ].v[ 0 ] = x3;
    p[ 2 ].v[ 1 ] = y3;
    p[ 2 ].v[ 2 ] = z3;
};

Triangle::Triangle( Point p1, Point p2, Point p3 )
{
    p[ 0 ] = p1;
    p[ 1 ] = p2;
    p[ 2 ] = p3;
};


Vector GravityForce::getForce( Entity *e, float dt )
{
    Entity *eOther;
    if ( e == e1 )
    {
        eOther = e2;
    } else
    {
        eOther = e1;
    }

    float magnitude = 6.67 * e->getMass() * eOther->getMass();
    magnitude = magnitude / pow( (((eOther->getPos())) - ((e->getPos()))).getMagnitude(), 2 );
    magnitude = magnitude * pow( 10, -11 );

    Vector direction = ((eOther->getPos())) - ((e->getPos()));
    direction.normalize();

    return direction * magnitude;
};

Vector AirDragForce::getForce( Entity *e, float dt )
{
    Vector v;
    v[ 0 ] = e->getV()[ 0 ];
    v[ 1 ] = e->getV()[ 1 ];
    v[ 2 ] = e->getV()[ 2 ];

    float magnitude = 0.5 * c * p * a * v.getMagnitude() * v.getMagnitude();

    if ( v.getMagnitude() == 0.0 )
    {
        return v;
    }
    
    v.normalize();
    v = v * -magnitude;

    return v;
};


std::vector< Triangle > loadModel( char *fileName )
{
    FILE *file = fopen( fileName, "r" );

    std::vector< Triangle > rtn;

    char buf[ 100 ];
    char *ps = 0;

    Triangle t;
    for( int i = 0; i < 3936 && fgets( buf, 99, file ) != NULL; ++i )
    {
        buf[ strlen( buf ) - 1 ] = 0;
        for ( int pntNum = 0; pntNum < 3; pntNum++ )
        {
            for( int axNum = 0; axNum < 3; axNum++ )
            {
                if ( pntNum == 0 && axNum == 0 )
                {
                    ps = strtok( buf, " \n" );
                } else
                {
                    ps = strtok( NULL, " \n" );
                }

                if ( ps != NULL )
                {
                    t.p[ pntNum ].v[ axNum ] = atof( ps );
                }
            }
        }

        rtn.push_back( t );

    }

    fclose( file );
    return rtn;
};



//---------------------------------------------------------------------------
#pragma package(smart_init)
