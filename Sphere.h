//---------------------------------------------------------------------------

#ifndef SphereH
#define SphereH

#include "Util.h"
#include "Entity.h"
#include "Plane.h"
#include "InputSource.h"

extern InputSource *input;

class Sphere : public Entity
{
    public:
        Sphere( void )
        {
            addNew( this );
            type = TYPE_SPHERE;
        };
        ~Sphere( void ) {};

        std::vector< Triangle > *getModelInfo( void )
        {
            return &model;
        };

        unsigned __int32 getFVF( void )
        {
            return D3DFVF_XYZ;
        };

        bool collidesWith( Entity *other )
        {
            if ( other->type == TYPE_SPHERE )
            {
                Vector difference = getPos() - other->getPos();
                return difference.getMagnitude() < ( other->getScale() + getScale() )[ 0 ];
            } else if ( other->type == TYPE_PLANE )
            {
                Plane *plane = ( Plane * ) other;
                float dist = plane->getDistFrom( this->getPos() );
                if ( dist <= scale[ 0 ] )
                {
                    //setVel( 0.0, 0.0, 0.0 );
                    //input->paused = true;
                    return true;
                }

                return dist <= scale[ 0 ];
            }
        };

    private:
        static std::vector< Triangle > model;
};


//---------------------------------------------------------------------------
#endif
