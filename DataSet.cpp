//---------------------------------------------------------------------------

#pragma hdrstop

#include "DataSet.h"
#include "Renderer.h"


std::vector< DataSet * > DataSet::dataSets;
const int DataSet::VBUFFER_SIZE = 65536;

GraphInput::GraphInput( void )
{
    created = false;
    type = 0;
    iValues = NULL;

    hasMax = false;
    hasMin = false;
    maxValue = 0.0f;
    minValue = 0.0f;
};

void GraphInput::create( DynamicValue *value, int type )
{
    iValues = NULL;
    this->type = type;

    this->value = value;

    if ( type & IS_FLOAT )
    {
        fValues = new std::vector< float >();
    } else
    {
        iValues = new std::vector< int >();
    }

    created = true;
};

void GraphInput::destroy( void )
{
    if ( type & IS_FLOAT )
    {
        if ( fValues != NULL )
        {
            delete fValues;
            fValues = NULL;
        }
    } else
    {
        if ( iValues != NULL )
        {
            delete iValues;
            iValues = NULL;
        }
    }
    created = false;
};

// pushes val onto vec if val is within any restrictions.
// returns false if the value cannot be pushed; true if it can
// does not work if T is non-primitive
template< typename T > bool GraphInput::pushValue( std::vector< T > *vec, T val )
{
    if ( hasMin && val < minValue )
    {
        return false;
    } else if ( hasMax && val > maxValue )
    {
        return false;
    } else
    {
        vec->push_back( val );
        return true;
    }
};


bool GraphInput::update( void )
{
    if ( type & IS_FLOAT )
    {
        return pushValue< float >( fValues, value->get() );
    } else
    {
        return pushValue< int >( iValues, ( int ) value->get() );
    }
};

void GraphInput::popLast( void )
{
    if ( type & IS_FLOAT )
    {
        fValues->pop_back();
    } else
    {
        iValues->pop_back();
    }
};



/*
void DataSet::trackData( int   *data, Axis axNum )
{
    if ( axes[ axNum ].created )
    {
        axes[ axNum ].destroy();
    }

    axes[ axNum ].create( INPUT_INT_DATA, data, NULL );
};

void DataSet::trackData( float *data, Axis axNum )
{
    if ( axes[ axNum ].created )
    {
        axes[ axNum ].destroy();
    }

    axes[ axNum ].create( INPUT_FLOAT_DATA, data, NULL );
};

void DataSet::trackFunction( int   ( *data )( void ), Axis axNum, void *arg )
{
    if ( axes[ axNum ].created )
    {
        axes[ axNum ].destroy();
    }

    axes[ axNum ].create( INPUT_INT_FUNCTION | ( arg == NULL ? 0 : USES_ARG ), data, arg );
};

void DataSet::trackFunction( float ( *data )( void ), Axis axNum, void *arg )
{
    if ( axes[ axNum ].created )
    {
        axes[ axNum ].destroy();
    }

    axes[ axNum ].create( INPUT_FLOAT_FUNCTION | ( arg == NULL ? 0 : USES_ARG ), data, arg );
}; */

void DataSet::draw( Renderer *r )
{
    if ( !made[ 0 ] || !made[ 1 ] )
    {
        return;
    }

    LPDIRECT3DDEVICE9 device = r->getDevice();

    // Draw the box of the graph, complete with vertical and horizontal lines
    device->SetFVF( UI_FVF );

    device->SetStreamSource( 0, vBuffer, 0, sizeof( UIPoint ) );


    Point drawScale = owner->getScale();
    Point screenPos = owner->getScreenPos( 0, 0 );

    r->setupWorldTransform( screenPos[ 0 ], screenPos[ 1 ], 0.0,
                            0.0, 0.0, 0.0,
                            drawScale[ 0 ], drawScale[ 1 ], drawScale[ 2 ] );
                            //scale[ 0 ], scale[ 1 ], 1.0 );

    device->SetTexture( 0, NULL );
    device->DrawPrimitive( D3DPT_LINESTRIP, 0, numPoints - 1 );
};

void DataSet::update( void )
{
    /* Forget domain/range; that's crap
    if ( axes[ AXIS_X ].update() )
    {
        // if the next point is within the domain, check the range.
        if ( !axes[ AXIS_Y ].update() )
        {
            axes[ AXIS_X ].popLast();
        }
    }
    */
    if ( !made[ 0 ] || !made[ 1 ] )
    {
        return;
    }

    if ( numPoints >= VBUFFER_SIZE )
    {
        return;
    }

    axes[ AXIS_X ].update();
    axes[ AXIS_Y ].update();

    UIPoint addPoint = UIPoint( axes[ AXIS_X ].getLast(), axes[ AXIS_Y ].getLast(), 1.0f, color );

    // update the vertex buffer with the new point.
    VOID *pVoid = NULL;
    vBuffer->Lock( 0, 0, &pVoid, 0 );
    //memset( pVoid, 0, sizeof( UIPoint ) * VBUFFER_SIZE );
    memcpy( ( ( char *) pVoid ) + ( numPoints * sizeof( UIPoint ) ), &addPoint, sizeof( UIPoint ) );
    vBuffer->Unlock();

    numPoints++;
};

//void DataSet::constFunction( float ( *fptr )( int, void *info ), Axis axNum ) {};
//void DataSet::constFunction( float ( *fptr )( float, void *info ), Axis axNum ) {};

void DataSet::reset( void )
{
    setDomain( false, false );
    setDomain( 0.0f, 0.0f );
    setRange( false, false );
    setRange( 0.0f, 0.0f );
};


void DataSet::setDomain( bool hasMin, bool hasMax )
{
    axes[ AXIS_X ].hasMin = hasMin;
    axes[ AXIS_X ].hasMax = hasMax;
};

void DataSet::setDomain( float min, float max )
{
    axes[ AXIS_X ].minValue = min;
    axes[ AXIS_X ].maxValue = max;
};

void DataSet::setRange( bool hasMin, bool hasMax )
{
    axes[ AXIS_Y ].hasMin = hasMin;
    axes[ AXIS_Y ].hasMax = hasMax;
};

void DataSet::setRange( float min, float max )
{
    axes[ AXIS_Y ].minValue = min;
    axes[ AXIS_Y ].maxValue = max;
};


void DataSet::setup( Renderer *r )
{
    r->getDevice()->CreateVertexBuffer( sizeof( UIPoint ) * VBUFFER_SIZE,
                           0,                           UI_FVF,
                           D3DPOOL_MANAGED,
                           &vBuffer,
                           NULL );

    VOID *pVoid = NULL;
    vBuffer->Lock( 0, 0, &pVoid, 0 );
    memset( pVoid, 0, sizeof( UIPoint ) * VBUFFER_SIZE );
    vBuffer->Unlock();
};

void DataSet::setOwner( Graph *g )
{
    owner = g;
};


//---------------------------------------------------------------------------
#pragma package(smart_init)
