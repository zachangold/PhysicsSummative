//---------------------------------------------------------------------------

#pragma hdrstop

#include "Graph.h"
#include "Renderer.h"

std::vector< Graph * > Graph::graphs;

std::vector< UIPoint > Graph::points = setupPoints();
LPDIRECT3DVERTEXBUFFER9 Graph::vBuffer = NULL;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

void Graph::addDataSet( DataSet *data )
{
    dataSets.push_back( data );
};

void Graph::update( void )
{
    /*
    for ( size_t i = 0; i < dataSets.size(); ++i )
    {
        dataSets[ i ]->update();
    }
    */
};

void Graph::draw( Renderer *r )
{
    if ( !draws )
    {
        return;
    }

    LPDIRECT3DDEVICE9 device = r->getDevice();

    // Draw the box of the graph, complete with vertical and horizontal lines
    device->SetFVF( UI_FVF );

    device->SetStreamSource( 0, vBuffer, 0, sizeof( UIPoint ) );
    r->setupWorldTransform( pos[ 0 ], pos[ 1 ], pos[ 2 ],
                            0.0, 0.0, 0.0,
                            scale[ 0 ], scale[ 1 ], 1.0 );

    device->SetTexture( 0, NULL );
    device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
    device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
    device->DrawPrimitive( D3DPT_LINELIST, 6, points.size() / 2 - 2 );

    char buf[ 32 ];
    sprintf( buf, "%.3f", yMax );
    topValue->setText( std::string( buf ) );

    sprintf( buf, "%.3f", yMin );
    bottomValue->setText( std::string( buf ) );

    if ( labelOnLeft )
    {
        topValue->setPos( pos[ 0 ] - scale[ 0 ] * 0.8 / 2.0 - D3DFont::CHAR_HEIGHT * 2.5, pos[ 1 ] + scale[ 1 ] * 0.5 * 0.75 );
        bottomValue->setPos( pos[ 0 ] - scale[ 0 ] * 0.8 / 2.0 - D3DFont::CHAR_HEIGHT * 2.5, pos[ 1 ] - scale[ 1 ] * 0.5 * 0.75 );
    } else
    {
        topValue->setPos( pos[ 0 ] + scale[ 0 ] * 0.8 / 2.0 + D3DFont::CHAR_HEIGHT * 2.5, pos[ 1 ] + scale[ 1 ] * 0.5 * 0.75 );
        bottomValue->setPos( pos[ 0 ] + scale[ 0 ] * 0.8 / 2.0 + D3DFont::CHAR_HEIGHT * 2.5, pos[ 1 ] - scale[ 1 ] * 0.5 * 0.75 );
    }

    titleText->setPos( pos[ 0 ], pos[ 1 ] + scale[ 1 ] / 2.0 - D3DFont::CHAR_HEIGHT * 1.2 + 2.0 );
    xText->setPos( pos[ 0 ] - scale[ 0 ] / 4.0, pos[ 1 ] - scale[ 1 ] / 2.0 + D3DFont::CHAR_HEIGHT * 1.2 - 2.0 );
    yText->setPos( pos[ 0 ] + scale[ 0 ] / 4.0, pos[ 1 ] - scale[ 1 ] / 2.0 + D3DFont::CHAR_HEIGHT * 1.2 - 2.0 );

    if ( dataSets.size() != 0 )
    {
        if ( dataSets.size() == 3 )
        {
            int e = 0;
        }

        xMin = dataSets[ 0 ]->getMin( DataSet::AXIS_X );
        yMin = dataSets[ 0 ]->getMin( DataSet::AXIS_Y );

        xMax = dataSets[ 0 ]->getMax( DataSet::AXIS_X );
        yMax = dataSets[ 0 ]->getMax( DataSet::AXIS_Y );

        dataSets[ 0 ]->setOwner( this );

        for ( size_t i = 1; i < dataSets.size(); ++i )
        {
            float ixMin = dataSets[ i ]->getMin( DataSet::AXIS_X );
            float iyMin = dataSets[ i ]->getMin( DataSet::AXIS_Y );

            float ixMax = dataSets[ i ]->getMax( DataSet::AXIS_X );
            float iyMax = dataSets[ i ]->getMax( DataSet::AXIS_Y );

            if ( ixMin < xMin )
            {
                xMin = ixMin;
            }
            if ( iyMin < yMin )
            {
                yMin = iyMin;
            }
            if ( ixMax > xMax )
            {
                xMax = ixMax;
            }
            if ( iyMax > yMax )
            {
                yMax = iyMax;
            }

            dataSets[ i ]->setOwner( this );
        }

        // Give the graph its text labels along the x- and y- axes, it title, etc.

        // Draw the Data sets on this graph:
        for ( size_t i = 0; i < dataSets.size(); ++i )
        {
            dataSets[ i ]->draw( r );
        }

    }

    device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
};

void Graph::setup( Renderer *r )
{
    r->getDevice()->CreateVertexBuffer( sizeof( UIPoint ) * points.size(),
                           0,                           UI_FVF,
                           D3DPOOL_MANAGED,
                           &vBuffer,
                           NULL );

    VOID *pVoid = NULL;
    vBuffer->Lock( 0, 0, &pVoid, 0 );
    memcpy( pVoid, &( points[ 0 ] ), sizeof( UIPoint ) * points.size() );
    vBuffer->Unlock();
};

void Graph::addToRenderer( Renderer *r )
{
    r->addUIElement( this );
    titleText->addToRenderer( r );
    xText->addToRenderer( r );
    yText->addToRenderer( r );

    topValue->addToRenderer( r );
    bottomValue->addToRenderer( r );
};


//---------------------------------------------------------------------------
#pragma package(smart_init)
