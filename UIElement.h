//---------------------------------------------------------------------------

#ifndef UIElementH
#define UIElementH

#include "Util.h"

class UIElement : public Drawable
{
    public:
        UIElement( void )
        {
            created.push_back( this );
            fixedPos = false;
            fixedSize = false;
        };
        virtual ~UIElement( void )
        {
            for ( size_t i = 0; i < created.size(); ++i )
            {
                if ( created[ i ] == this )
                {
                    created[ i ] = NULL;
                }
            }
        };

        // automatically detects when two click events happen in rapid succession
        // on mouse down, press = true. on mouse up, press = false. 
        void mouseClickEvent( int mx, int my, int button, bool press );
        void mouseMoveEvent( int mx, int my );

        void keyEvent( char key, bool press );

        // fixedPos determines whether the user can click and drag the UIElement.
        // position can still be changed using setPos( float, float )
        bool fixedPos;
        bool fixedSize;

        virtual void setSize( float sx, float sy )
        {
            scale[ 0 ] = sx;
            scale[ 1 ] = sy;
        };

        virtual void setPos( float x, float y )
        {
            pos[ 0 ] = x;
            pos[ 1 ] = y;
        };

        virtual void addToRenderer( Renderer *r );


    protected:
        Point pos;
        Point scale;

        //virtual void onClickEvent( int mx, int my, int button, bool press ) = 0;
        //virtual void onClickDragEvent( int mx, int my, int nx, int ny, int button, bool press ) = 0;
        //virtual void onDblClickEvent( int mx, int my, int button, bool press ) = 0;
        //virtual void onKeyEvent( char key, bool press ) = 0;

    private:
        static UIElement *focus;
        static std::vector< UIElement * > created;
        

};


//---------------------------------------------------------------------------
#endif
