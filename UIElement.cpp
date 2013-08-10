//---------------------------------------------------------------------------

#pragma hdrstop

#include "UIElement.h"
#include "Renderer.h"

std::vector< UIElement * > UIElement::created;

void UIElement::addToRenderer( Renderer *r )
{
    r->addUIElement( this );
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
