//---------------------------------------------------------------------------

#ifndef EntityH
#define EntityH

#include "Util.h"

class Entity// : public Drawable
{
    public:
        Entity( void );
        virtual ~Entity( void );

        static void deleteAll( void )
        {
            for ( size_t i = 0; i < created.size(); ++i )
            {
                if ( created[ i ] != NULL )
                {
                    delete created[ i ];
                    created[ i ] = NULL;
                }
            }
        };

        static void updateAll( float SECONDS_PER_UPDATE )
        {
            for ( size_t i = 0; i < created.size(); ++i )
            {
                if ( created[ i ] != NULL )
                {
                    created[ i ]->update( SECONDS_PER_UPDATE );
                    for ( size_t e = i + 1; e < created.size(); ++e )
                    {
                        if ( created[ i ]->collidesWith( created[ e ] ) )
                        {
                            // Do something?
                            int e = 0;
                        } else
                        {
                            int f = 0;
                        }
                    }
                }
            }
        };

        void setRot( float x, float y, float z );
        void setScale( float x, float y, float z );

        float getMass( void )
        {
            return m;
        };

        float *getMassPtr( void )
        {
            return &m;
        };

        Vector *getDPtr( void ){ return &d; };
        Vector *getVPtr( void ){ return &v; };
        Vector *getAPtr( void ){ return &a; };

        Vector getD( void ){ return d; };
        Vector getV( void ){ return v; };
        Vector getA( void ){ return a; };
        Vector getD0( void ){ return d0; };
        Vector getV0( void ){ return v0; };

        Point *getPosPtr( void ){ return &pos; };

        Point getPos( void ){ return pos; };
        Point getRot( void ){ return rot; };
        Point getScale( void ){ return scale; };

        Vector getMomentum( void ){ return v * m; };

        void setMass( float m )
        {
            this->m = m;
        };

        void setD0( float x, float y, float z )
        {
            d0.v[ 0 ] = x;
            d0.v[ 1 ] = y;
            d0.v[ 2 ] = z;

            d = Point( 0, 0, 0 );
            pos = d0;
        };
        void setV0( float x, float y, float z )
        {
            v0.v[ 0 ] = x;
            v0.v[ 1 ] = y;
            v0.v[ 2 ] = z;

            v = v0;
        };

        void addForce( Force *f )
        {
            forces.push_back( f );
        };

        virtual bool collidesWith( Entity *other ) = 0;

        void update( float delta_t )
        {
            if ( m == 0.0f )
            {
                return;
            }

            Point delta_d = Point( 0, 0, 0 );

            pD = d;
            pV = v;
            pA = a;

            // compute forces:
            fNet.dir = 0.0f;

            for ( size_t i = 0; i < forces.size(); ++i )
            {
                if ( forces[ i ]->type == Force::FORCE_GRAVITY )
                {
                    fNet.dir = fNet.dir + forces[ i ]->getForce( this, delta_t ) * m;
                } else
                {
                    fNet.dir = fNet.dir + forces[ i ]->getForce( this, delta_t );
                }
            }

            // f = ma, so a = f/m
            a = fNet.dir / m;

            // d = v*t + (1/2)*a*t*t
            //d = v * t + a * t * t * 0.5;
            delta_d = v * delta_t;

            // v2 = v1 + a * t
            v = v + a * delta_t;

            d += delta_d;

            pos += delta_d;
        };

        typedef enum
        {
            TYPE_SPHERE = 1,
            TYPE_PLANE = 2
        } EntityType;

        EntityType type;

        virtual std::vector< Triangle > *getModelInfo( void );
        virtual unsigned __int32 getFVF( void );

    protected:
        // Information used for drawing
        Point pos, rot, scale;

        // Physics
        float m;  // mass, in kilograms

        // values of previous frames
        Vector pD;
        Vector pV;
        Vector pA;

        // initial properties of the entity
        Vector d0;
        Vector v0;
        float m0;

        Vector d; // displacement, in metres
        Vector v; // velocity, in m/s
        Vector a; // acceleration, in m/s^2

        Force fNet; // net force, in N ( kg*m/s^2 )

        std::vector< Force * > forces; // all forces acting on this object, in N (kg*m/s^2)


        void addNew( Entity *self )
        {
            created.push_back( self );
        };
        void remove( Entity *self )
        {
            for ( size_t i = 0; i < created.size(); ++i )
            {
                if ( created[ i ] == self )
                {
                    created[ i ] = NULL;
                    break;
                }
            }
        };
        static std::vector< Entity * > created;

    private:
        static std::vector< Triangle > defaultModel;
};

//---------------------------------------------------------------------------
#endif
