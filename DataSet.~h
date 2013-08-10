//---------------------------------------------------------------------------

#ifndef DataSetH
#define DataSetH

#include "Util.h"
#include "UIElement.h"
//#include "Renderer.h"

// the type of a graph input
#define IS_FLOAT    0x01
#define IS_FUNC     0x02
#define USES_ARG    0x04
// Therefore, the progression is:
// 0x00 = Integer Data Pointer
// 0x01 = Floating-Point Data Pointer
// 0x02 = Integer Function Pointer
// 0x03 = Floating-Point Function Pointer
#define INPUT_INT_DATA          0x0
#define INPUT_FLOAT_DATA        IS_FLOAT
#define INPUT_INT_FUNCTION      IS_FUNC
#define INPUT_FLOAT_FUNCTION    IS_FLOAT | IS_FUNC

class Graph;

typedef struct GraphInput
{
    GraphInput( void );

    void create( DynamicValue *value, int type );
    void destroy( void );

    // pushes val onto vec if val is within any restrictions.
    // returns false if the value cannot be pushed; true if it can
    // does not work if T is non-primitive
    template< typename T > bool pushValue( std::vector< T > *vec, T val );


    bool update( void );
    void popLast( void );

    float getLast( void )
    {
        return (*fValues)[ fValues->size() - 1 ];
    };

    union
    {
        std::vector< int > *iValues;
        std::vector< float > *fValues;
    };

    DynamicValue *value;

    unsigned int type;
    bool created;
    bool hasMax, hasMin;
    float maxValue, minValue;
} GraphInput;


/**
 * A DataSet is a set of points on a graph.
 * it is assumed that, unless otherwise noted, the DataSet is unrestricted (i.e., no limitations on the x- or y-values)
 * it is assumed that DataSets free their own memory or are stack-allocated
 */
class DataSet
{
    public:
        DataSet( void )
        {
            reset();
            dataSets.push_back( this );
            numPoints = 0;
            vBuffer = NULL;
            made[ AXIS_X ] = false;
            made[ AXIS_Y ] = false;

            setColor( 1.0, 1.0, 1.0, 1.0 );
        };
        ~DataSet( void )
        {
            axes[ AXIS_X ].destroy();
            axes[ AXIS_Y ].destroy();

            for ( size_t i = 0; i < dataSets.size(); ++i )
            {
                if ( dataSets[ i ] == this )
                {
                    dataSets[ i ] = NULL;
                    break;
                }
            }
        };

        typedef enum
        {
            AXIS_X = 0,
            AXIS_Y = 1
        } Axis;

        void track( DynamicValue *value, Axis axNum )
        {
            axes[ axNum ].create( value, IS_FLOAT );
            made[ axNum ] = true;
        };

        // The limits of this function
        void setDomain( bool hasMin, bool hasMax );
        void setDomain( float min, float max );
        void setRange( bool hasMin, bool hasMax );
        void setRange( float min, float max );


        // draw() assumes that all transformations are already applied
        void draw( Renderer *r );
        void update( void );

        void setup( Renderer *r );

        static void updateAll( void )
        {
            for ( size_t i = 0; i < dataSets.size(); ++i )
            {
                if ( dataSets[ i ] != NULL )
                {
                    dataSets[ i ]->update();
                }
            }
        };

        float getMax( Axis axisNum )
        {
            if ( !made[ 0 ] || !made[ 1 ] )
            {
                return 0.0f;
            }

            if ( numPoints < 1 )
            {
                return 0.0f;
            }

            float max = ( *axes[ axisNum ].fValues )[ 0 ];

            for ( size_t i = 1; i < axes[ axisNum ].fValues->size(); ++i )
            {
                if ( max < ( *axes[ axisNum ].fValues )[ i ] )
                {
                    max = ( *axes[ axisNum ].fValues )[ i ];
                }
            }

            return max;
        };

        float getMin( Axis axisNum )
        {
            if ( !made[ 0 ] || !made[ 1 ] )
            {
                return 0.0f;
            }

            if ( numPoints < 1 )
            {
                return 0.0f;
            }

            float min = ( *(axes[ axisNum ].fValues ) )[ 0 ];

            for ( size_t i = 1; i < axes[ axisNum ].fValues->size(); ++i )
            {
                if ( min > ( *axes[ axisNum ].fValues )[ i ] )
                {
                    min = ( *axes[ axisNum ].fValues )[ i ];
                }
            }

            return min;
        };

        void setOwner( Graph *g );

        void destroy( void )
        {
            if ( vBuffer != NULL )
            {
                vBuffer->Release();
                vBuffer = NULL;
            }

            axes[ 0 ].destroy();
            axes[ 1 ].destroy();

            numPoints = 0;
            owner = NULL;

            made[ 0 ] = false;
            made[ 1 ] = false;
        };

        static void deleteAll( void )
        {
            for ( size_t i = 0; i < dataSets.size(); ++i )
            {
                if ( dataSets[ i ] != NULL )
                {
                    delete dataSets[ i ];
                    dataSets[ i ] = NULL;
                }
            }

            dataSets.resize( 0 );
        };

        void setColor( float r, float g, float b, float a )
        {
            color = D3DXCOLOR( r, g, b, a );
        };

    private:
        DWORD color;
        bool made[ 2 ];

        void reset( void );

        Graph *owner;

        GraphInput axes[ 2 ];
        int numPoints;
        LPDIRECT3DVERTEXBUFFER9 vBuffer;

        static std::vector< DataSet * > dataSets;
        static const int VBUFFER_SIZE;
};


//---------------------------------------------------------------------------
#endif
