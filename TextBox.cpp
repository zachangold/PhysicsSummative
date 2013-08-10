//---------------------------------------------------------------------------

#pragma hdrstop

#include "TextBox.h"
#include "Renderer.h"

D3DFont TextBox::defaultFont;
LPDIRECT3DVERTEXBUFFER9 TextBox::vBuffer = NULL;
std::vector< UIPoint > TextBox::points = TextBox::setupPoints();


void TextBox::setup( Renderer *r )
{
    defaultFont.init( r->getDevice(), SCREEN_WIDTH, SCREEN_HEIGHT );
    

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

void TextBox::draw( Renderer *r )
{
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
    device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

    float BEZEL_X = 0.1f * scale[ 0 ];
    float BEZEL_Y = 0.1f * scale[ 1 ];

    font->setText( text );
    font->render( r->getDevice(), color, pos[ 0 ] + SCREEN_WIDTH_HALF - scale[ 0 ] / 2.0 + BEZEL_X / 2.0f,
                  - pos[ 1 ] + SCREEN_HEIGHT_HALF - scale[ 1 ] / 2.0 + BEZEL_Y / 2.0f, flags,
                  scale[ 0 ] - BEZEL_X, scale[ 1 ] - BEZEL_Y );
};


//---------------------------------------------------------------------------
#pragma package(smart_init)
