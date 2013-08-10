//---------------------------------------------------------------------------

#ifndef GraphH
#define GraphH

#include "UIElement.h"
#include "DataSet.h"
#include "TextBox.h"

class Renderer;

class Graph : public UIElement
{
    public:
        Graph( void )
        {
            pos = Point( 0.0f, 0.0f, 1.0f );
            scale = Point( 1.0f, 1.0f, 1.0f );

            numPoints = 0;
            graphs.push_back( this );

            titleText = new TextBox();
            xText = new TextBox();
            yText = new TextBox();

            topValue = new TextBox();
            bottomValue = new TextBox();

            titleText->flags = DT_CENTER | DT_WORDBREAK;
            xText->flags = DT_LEFT | DT_WORDBREAK;
            yText->flags = DT_RIGHT | DT_WORDBREAK;

            topValue->flags = DT_RIGHT | DT_WORDBREAK;
            bottomValue->flags = DT_RIGHT | DT_WORDBREAK;

            xMin = 0.0f;
            xMax = 0.0f;
            yMin = 0.0f;
            yMax = 0.0f;

            labelOnLeft = true;
            draws = true;
        };
        virtual ~Graph( void )
        {
            if ( titleText != NULL )
            {
                delete titleText;
                titleText = NULL;
            }

            if ( xText != NULL )
            {
                delete xText;
                xText = NULL;
            }

            if ( yText != NULL )
            {
                delete yText;
                yText = NULL;
            }

            if ( topValue != NULL )
            {
                delete topValue;
                topValue = NULL;
            }

            if ( bottomValue != NULL )
            {
                delete bottomValue;
                bottomValue = NULL;
            }

            for ( size_t i = 0; i < graphs.size(); ++i )
            {
                if ( graphs[ i ] == this )
                {
                    graphs[ i ] = NULL;
                    break;
                }
            }
        };

        void setTitle( std::string title )
        {
            this->title = title;
            titleText->setText( title );
        };
        void setUnits( std::string xUnits, std::string yUnits )
        {
            this->xUnits = xUnits;
            this->yUnits = yUnits;

            xText->setText( xUnits );
            yText->setText( yUnits );
        };

        void addDataSet( DataSet *data );

        void update( void );
        void draw( Renderer *r );

        void setDepth( float z )
        {
            pos[ 2 ] = z;
        };

        static void updateAll( void )
        {
            for ( size_t i = 0; i < graphs.size(); ++i )
            {
                if ( graphs[ i ] != NULL )
                {
                    graphs[ i ]->update();
                }
            }
        };

        static void deleteAll( void )
        {
            if ( vBuffer != NULL )
            {
                vBuffer->Release();
                vBuffer = NULL;
            }

            for ( size_t i = 0; i < graphs.size(); ++i )
            {
                if ( graphs[ i ] != NULL )
                {
                    graphs[ i ] = NULL;
                }
            }
        };

        void setSize( float sx, float sy )
        {
            scale[ 0 ] = sx;
            scale[ 1 ] = sy;
            titleText->setSize( sx, D3DFont::CHAR_HEIGHT );

            xText->setSize( sx / 2.0, D3DFont::CHAR_HEIGHT );
            yText->setSize( sx / 2.0, D3DFont::CHAR_HEIGHT );

            topValue->setSize( D3DFont::CHAR_HEIGHT * 5, D3DFont::CHAR_HEIGHT );
            bottomValue->setSize( D3DFont::CHAR_HEIGHT * 5, D3DFont::CHAR_HEIGHT );
        };

        void setPos( float x, float y )
        {
            pos[ 0 ] = x;
            pos[ 1 ] = y;
        };

        static void setup( Renderer *r );

        void addToRenderer( Renderer *r );

        // Takes a data point and returns a screen point
        // Assumes that the argument is within the graph
        Point getScreenPos( float x, float y )
        {
            float xScale = xMax - xMin;
            float yScale = yMax - yMin;

            if ( xScale == 0.0f )
            {
                xScale = 1.0f;
            }

            if ( yScale == 0.0f )
            {
                yScale = 1.0f;
            }

            // First, find the position on the graph
            Point rtn;
            rtn[ 0 ] = ( ( ( x - xMin ) / xScale - 0.5 ) ) * ( scale[ 0 ] * ( 1.0 - 0.2 ) );
            rtn[ 1 ] = ( ( ( y - yMin ) / yScale - 0.5 ) ) * ( scale[ 1 ] * ( 1.0 - 0.2 ) );
            rtn[ 2 ] = 0.0f;

            // Then translate that point to where the graph is on the screen
            rtn[ 0 ] += pos[ 0 ];
            rtn[ 1 ] += pos[ 1 ];

            //rtn[ 0 ] += 2;
            //rtn[ 1 ] += 2;

            return rtn;
        };

        Point getScale( void )
        {
            Point rtn;
            rtn[ 0 ] = ( scale[ 0 ] ) / ( xMax - xMin );
            rtn[ 1 ] = ( scale[ 1 ] ) / ( yMax - yMin );
            rtn[ 2 ] = 1.0f;

            rtn = rtn * 0.8f;
            rtn[ 2 ] += 0.4f;

            return rtn; 
        };

        bool labelOnLeft;
        bool draws;

    protected:
        void onClickEvent( int mx, int my, int button, bool press );
        void onClickDragEvent( int mx, int my, int nx, int ny, int button, bool press );
        void onDblClickEvent( int mx, int my, int button, bool press );
        void onKeyEvent( char key, bool press );

    private:
        int numPoints;
        std::vector< DataSet * > dataSets;
        std::string xUnits, yUnits, title;
        TextBox *titleText, *xText, *yText;
        TextBox *topValue, *bottomValue;

        float xMin, xMax;
        float yMin, yMax;

        static std::vector< Graph * > graphs;

        static std::vector< UIPoint > setupPoints( void )
        {
            std::vector< UIPoint > rtn;
            float alpha = 0.2;
            const int NUM_LINES = 3;

            DWORD color = D3DXCOLOR( 0.2, 0.2, 0.2, alpha );
            DWORD lineColor[ NUM_LINES ] =
            {
                D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ),
                D3DXCOLOR( 0.5, 0.5, 0.5, 1.0 ),
                D3DXCOLOR( 0.2, 0.2, 0.2, 1.0 )
            };

            // top-left triangle
            rtn.push_back( UIPoint(-0.5,-0.5, 0.0, color ) );
            rtn.push_back( UIPoint( 0.5,-0.5, 0.0, color ) );
            rtn.push_back( UIPoint(-0.5, 0.5, 0.0, color ) );

            // bottom-right triangle
            rtn.push_back( UIPoint( 0.5, 0.5, 0.0, color ) );
            rtn.push_back( UIPoint(-0.5, 0.5, 0.0, color ) );
            rtn.push_back( UIPoint( 0.5,-0.5, 0.0, color ) );

            // Give a 0.1 "bezel"
            const float BEZEL = 0.1f;
            // Main box
            rtn.push_back( UIPoint( BEZEL - 0.5, BEZEL - 0.5, 0.0, lineColor[ 0 ] ) );
            rtn.push_back( UIPoint( BEZEL - 0.5,-BEZEL + 0.5, 0.0, lineColor[ 0 ] ) );

            rtn.push_back( UIPoint( BEZEL - 0.5,-BEZEL + 0.5, 0.0, lineColor[ 0 ] ) );
            rtn.push_back( UIPoint(-BEZEL + 0.5,-BEZEL + 0.5, 0.0, lineColor[ 0 ] ) );

            rtn.push_back( UIPoint(-BEZEL + 0.5,-BEZEL + 0.5, 0.0, lineColor[ 0 ] ) );
            rtn.push_back( UIPoint(-BEZEL + 0.5, BEZEL - 0.5, 0.0, lineColor[ 0 ] ) );

            rtn.push_back( UIPoint(-BEZEL + 0.5, BEZEL - 0.5, 0.0, lineColor[ 0 ] ) );
            rtn.push_back( UIPoint( BEZEL - 0.5, BEZEL - 0.5, 0.0, lineColor[ 0 ] ) );

            // The "+" shape through the centre, in a slightly gray-er color
            rtn.push_back( UIPoint( 0.0, BEZEL - 0.5, 0.0, lineColor[ 1 ] ) );
            rtn.push_back( UIPoint( 0.0,-BEZEL + 0.5, 0.0, lineColor[ 1 ] ) );

            rtn.push_back( UIPoint( BEZEL - 0.5, 0.0, 0.0, lineColor[ 1 ] ) );
            rtn.push_back( UIPoint(-BEZEL + 0.5, 0.0, 0.0, lineColor[ 1 ] ) );

            // The "#" shape, in an even grayer color
            const float DISP = ( 0.5f - BEZEL ) / 2.0f;

            rtn.push_back( UIPoint( DISP, BEZEL - 0.5, 0.0, lineColor[ 2 ] ) );
            rtn.push_back( UIPoint( DISP,-BEZEL + 0.5, 0.0, lineColor[ 2 ] ) );

            rtn.push_back( UIPoint(-DISP, BEZEL - 0.5, 0.0, lineColor[ 2 ] ) );
            rtn.push_back( UIPoint(-DISP,-BEZEL + 0.5, 0.0, lineColor[ 2 ] ) );

            rtn.push_back( UIPoint( BEZEL - 0.5, DISP, 0.0, lineColor[ 2 ] ) );
            rtn.push_back( UIPoint(-BEZEL + 0.5, DISP, 0.0, lineColor[ 2 ] ) );

            rtn.push_back( UIPoint( BEZEL - 0.5,-DISP, 0.0, lineColor[ 2 ] ) );
            rtn.push_back( UIPoint(-BEZEL + 0.5,-DISP, 0.0, lineColor[ 2 ] ) );



            return rtn;
        };
        static std::vector< UIPoint > points;
        static LPDIRECT3DVERTEXBUFFER9 vBuffer;
};


//---------------------------------------------------------------------------
#endif
