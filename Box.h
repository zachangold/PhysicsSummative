//---------------------------------------------------------------------------

#ifndef BoxH
#define BoxH

#include "Entity.h"

class Box : public Entity
{
    public:
        Box( void )
        {
            addNew( this );
        };
        ~Box( void )
        {
            remove( this );
        };

        std::vector< Triangle > *getModelInfo( void )
        {
            return &boxModel;
        };

        unsigned __int32 getFVF( void )
        {
            return D3DFVF_XYZ;
        };

        // returns true if the line from prev->now cuts through this object
        bool collided( Point prev, Point now )
        {
            bool prevResult = isWithin( prev );
            bool nowResult = isWithin( now );

            if ( nowResult )
            {
                return true;
            }

            return false;
        };

        bool isWithin( Point p )
        {
            for ( int i = 0; i < NUM_SIDES; ++i )
            {
                //if ( sides[ i ].getSide( p ) > 0.0f )
                {
                    return false;
                }
            }

            return true;
        };

    private:
        static std::vector< Triangle > boxModel;

        enum
        {
            SIDE_TOP = 0,
            SIDE_BOTTOM,
            SIDE_LEFT,
            SIDE_RIGHT,
            SIDE_FRONT,
            SIDE_BACK
        } Side;
        static const int NUM_SIDES;

        //Plane sides[ 6 ];
};

//---------------------------------------------------------------------------
#endif
