//---------------------------------------------------------------------------

#pragma hdrstop

#include "Entity.h"

std::vector< Triangle > Entity::defaultModel = loadModel( "monkHiRes.raw" );
std::vector< Entity * > Entity::created;

Entity::Entity( void )
{
    pos = 0.0f;
    rot = 0.0f;
    scale = 1.0f;
    m = 1.0f;
    v[ 0 ] = 0.0f;
    v[ 1 ] = 0.0f;
    v[ 2 ] = 0.0f;
};

Entity::~Entity( void )
{

};

void Entity::setRot( float x, float y, float z )
{
    rot.v[ 0 ] = x;
    rot.v[ 1 ] = y;
    rot.v[ 2 ] = z;
};

void Entity::setScale( float x, float y, float z )
{
    scale.v[ 0 ] = x;
    scale.v[ 1 ] = y;
    scale.v[ 2 ] = z;
};

std::vector< Triangle > *Entity::getModelInfo( void )
{
    return &defaultModel;
};

unsigned __int32 Entity::getFVF( void )
{
    return D3DFVF_XYZ;
};

void parseNextFloat( char **ptr, float *f )
{
    char *start = *ptr;

    while( ( ( **ptr ) >= '0' && ( **ptr ) <= '9' ) || ( **ptr ) == '.' || ( **ptr ) == '-' )
    {
        ( *ptr )++;
    }

    if ( ( **ptr ) == 0 )
    {
        ( *f ) = atof( start );
        ( *ptr ) = NULL;
        return;
    }

    ( **ptr ) = 0;
    ( *f ) = atof( start );
    ( *ptr )++;
};


//---------------------------------------------------------------------------
#pragma package(smart_init)
