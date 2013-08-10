//---------------------------------------------------------------------------

#ifndef UtilH
#define UtilH

#include <vector>
#include <stdio.h>
#include <math.h>
#include <iostream>
//#include <dsound.h>
#include <DirectX/dinput.h>
#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
#define SCREEN_WIDTH_HALF ( SCREEN_WIDTH / 2.0 )
#define SCREEN_HEIGHT_HALF ( SCREEN_HEIGHT / 2.0 )
extern const float UI_BORDER;

extern const int MOUSE_LEFT;
extern const int MOUSE_RIGHT;
extern const int MOUSE_MIDDLE;
extern const int MOUSE_SCROLLUP;
extern const int MOUSE_SCROLL_DOWN;


typedef struct Point
{
    Point( void );
    Point( float x, float y, float z );

    Point &operator = ( Point other )
    {
        this->v[ 0 ] = other.v[ 0 ];
        this->v[ 1 ] = other.v[ 1 ];
        this->v[ 2 ] = other.v[ 2 ];
        return *this;
    };
    Point &operator = ( float other )
    {
        v[ 0 ] = other;
        v[ 1 ] = other;
        v[ 2 ] = other;

        return *this;
    };

    Point operator + ( Point other )
    {
        Point rtn;
        rtn.v[ 0 ] = this->v[ 0 ] + other.v[ 0 ];
        rtn.v[ 1 ] = this->v[ 1 ] + other.v[ 1 ];
        rtn.v[ 2 ] = this->v[ 2 ] + other.v[ 2 ];
        return rtn;
    };
    Point operator - ( Point other )
    {
        Point rtn;
        rtn.v[ 0 ] = this->v[ 0 ] - other.v[ 0 ];
        rtn.v[ 1 ] = this->v[ 1 ] - other.v[ 1 ];
        rtn.v[ 2 ] = this->v[ 2 ] - other.v[ 2 ];
        return rtn;
    };
    Point operator * ( float other )
    {
        Point rtn;
        rtn.v[ 0 ] = this->v[ 0 ] * other;
        rtn.v[ 1 ] = this->v[ 1 ] * other;
        rtn.v[ 2 ] = this->v[ 2 ] * other;
        return rtn;
    };
    Point operator / ( float other )
    {
        Point rtn;
        rtn.v[ 0 ] = this->v[ 0 ] / other;
        rtn.v[ 1 ] = this->v[ 1 ] / other;
        rtn.v[ 2 ] = this->v[ 2 ] / other;
        return rtn;
    };

    Point &operator += ( Point &other )
    {
        ( *this ) = ( *this ) + other;
        return *this;
    };
    Point &operator -= ( Point &other );
    Point &operator *= ( float other );
    Point &operator /= ( float other );

    float &operator [] ( int index )
    {
        return v[ index ];
    };

    float getMagnitude( void )
    {
        return sqrt( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ] );
    };

    void normalize( void )
    {
        ( *this ) = ( *this ) / getMagnitude();
    };

    float dot( Point other )
    {
        return v[ 0 ] * other[ 0 ] + v[ 1 ] * other[ 1 ] + v[ 2 ] * other[ 2 ]; 
    };

    float v[ 3 ];
} Point;

typedef Point Vector;

class Entity;

class Force
{
    public:
        typedef enum
        {
            FORCE_NONE = 0,
            FORCE_GRAVITY,
            FORCE_NORMAL,
            FORCE_FRICTION,
            FORCE_APPLIED,
            FORCE_TENSION,
            FORCE_AIR_DRAG
        } Type;

        Force( void )
        {
            dir = 0.0f;
            type = FORCE_NONE;
            isTimed = false;
            timeLeft = 0.0f;
        };

        Force( Point dir, Type type )
        {
            this->dir = dir;
            this->type = type;
        };

        Force( float x, float y, float z, Type type )
        {
            this->dir.v[ 0 ] = x;
            this->dir.v[ 1 ] = y;
            this->dir.v[ 2 ] = z;

            this->type = type;
        };

        virtual Vector getForce( Entity *e, float SECONDS_PER_UPDATE )
        {
            if ( isTimed && timeLeft < 0.0f )
            {
                return Point( 0.0f, 0.0f, 0.0f );
            }

            if ( isTimed && timeLeft < SECONDS_PER_UPDATE )
            {
                Vector rtn = dir * ( timeLeft / SECONDS_PER_UPDATE );
                timeLeft = -1.0f;
                return dir;
            } else if ( isTimed )
            {
                timeLeft -= SECONDS_PER_UPDATE;
                return dir;
            } else
            {
                return dir;
            }
        };

        Point dir;
        Type type;
        bool isTimed;
        float timeLeft;
};

class GravityForce : public Force
{
    public:
        GravityForce( void ) {};
        GravityForce( Entity *e1, Entity *e2 )
        {
            this->e1 = e1;
            this->e2 = e2;
        };
        ~GravityForce( void ) {};

        void set( Entity *e1, Entity *e2 )
        {
            this->e1 = e1;
            this->e2 = e2;
        };

        Vector getForce( Entity *e, float dt );


    private:
        Entity *e1, *e2;
        


};

class AirDragForce : public Force
{
    public:
        AirDragForce( void ) {};
        AirDragForce( Entity *e, float c, float p, float a )
        {
            set( e, c, p, a );
        };
        ~AirDragForce( void ) {};

        void set( Entity *e, float c, float p, float a )
        {
            this->c = c;
            this->p = p;
            this->a = a;
        };

        Vector getForce( Entity *e, float dt );


    private:
        float c, p, a;



};

typedef struct Triangle
{
    Triangle( void );
    Triangle( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3 );
    Triangle( Point p1, Point p2, Point p3 );

    Point p[ 3 ];
} Triangle;


typedef struct Point2f
{
    Point2f( void )
    {
        x = 0;
        y = 0;
    };

    Point2f( float x, float y )
    {
        this->x = x;
        this->y = y;
    };

    float x, y;
} Point2f;

std::vector< Triangle > loadModel( char *fileName );

const unsigned __int32 UI_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
typedef struct UIPoint
{
    UIPoint( void )
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        //rhw = 1.0f;
        color = 0;
    };
    UIPoint( float x, float y, float z, DWORD color )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        //rhw = 1.0f;
        this->color = color;
    };
    UIPoint( float x, float y, float z, float r, float g, float b, float a )
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        //rhw = 1.0f;
        color = D3DXCOLOR( r, g, b, a );
    };

    float x, y, z;//, rhw;
    DWORD color;
} UIPoint;

const unsigned __int32 SQUARE_FVF = D3DFVF_XYZRHW;
extern UIPoint uiSquare[ 6 ];

class Renderer;
class Drawable
{
    public:
        Drawable( void ) {};
        virtual ~Drawable( void ) {};

        virtual void draw( Renderer *r ) = 0;

    protected:


    private:

    
};
/*
class Value
{
    public:
        Value( void );
        virtual ~Value( void );

        virtual operator int() = 0;
        virtual operator float() = 0;

    private:


};

class StaticValue : public Value
{
    public:
        StaticValue( void );
        virtual ~StaticValue( void );

        virtual void set( void ) = 0;
};

class StaticFloat : public StaticValue
{
    public:
        StaticFloat( void );
        ~StaticFloat( void );

        StaticFloat &operator = ( float other );
        StaticFloat &operator = ( StaticFloat &other );

        StaticFloat operator + ( float other );
        StaticFloat operator + ( Value &other );
        StaticFloat operator - ( float other );
        StaticFloat operator - ( Value &other );
        StaticFloat operator * ( float other );
        StaticFloat operator * ( Value &other );
        StaticFloat operator / ( float other );
        StaticFloat operator / ( Value &other );

        StaticFloat &operator += ( float other );
        StaticFloat &operator += ( Value &other );
        StaticFloat &operator -= ( float other );
        StaticFloat &operator -= ( Value &other );
        StaticFloat &operator *= ( float other );
        StaticFloat &operator *= ( Value &other );
        StaticFloat &operator /= ( float other );
        StaticFloat &operator /= ( Value &other );

        operator float() { return value; };
        operator int() { return ( int ) value; };

    private:
        float value;

};*/

class DynamicValue
{
    public:
        DynamicValue( void )
        {
            arg = NULL;
            type = NONE;
            data = NULL;
        };
        virtual ~DynamicValue( void )
        {
            type = NONE;
            arg = NULL;
            data = NULL;
        };

        // tracking values are to be used for values that occur in the simulation
        void trackData( float *value ) { type = NONE; data = value; this->value = *value; arg = NULL; };
        void trackFunc( float (*function)( void * ), void *arg ) { type = FUNCTION; this->function = function; this->arg = arg; };

        // static values are to be used for non-simulated values
        void staticData( float *value ) { type = STATIC; data = value; this->value = *value; arg = NULL; };
        void staticFunc( float (*function)( void * ), void *arg ) { type = STATIC | FUNCTION; this->function = function; this->arg = arg; };

        // computes the value with the current parameters (if necessary), then return the result
        float get( void )
        {
            if ( ( type & STATIC ) && !( type & FUNCTION ) )
            {
                return value;
            } else if ( type & FUNCTION )
            {
                value = function( arg );
                return value;
            } else
            {
                value = *data;
                return value;
            }
        }

        void set( float val )
        {
            if ( !( type & FUNCTION ) )
            {
                value = val;
            }
        };

    private:
        typedef enum
        {
            NONE = 0,
            STATIC = 1,
            FUNCTION = 2
        } Type;
        Type type;

        float value;
        void *arg;

        union
        {
            float *data;
            float ( *function )( void * );
        };
};

//---------------------------------------------------------------------------
#endif
