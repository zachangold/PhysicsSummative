//---------------------------------------------------------------------------

#ifndef ButtonH
#define ButtonH

#include "UIElement.h"
#include "TextBox.h"

// The Button class doesn't actually have its own ui model - it just has a TextBox
class Button : public UIElement
{
    public:
        Button( void )
        {
            func = NULL;
            arg = NULL;
            text = NULL;
        };
        Button( std::string text )
        {
            ( *this ) = Button();
            setText( text );
        };
        ~Button( void )
        {
            if ( text != NULL )
            {
                delete text;
                text = NULL;
            }
        };

        void setFunc( void ( *func )( void * ), void *arg )
        {
            this->func = func;
            this->arg = arg;
        };

        void setText( std::string text )
        {
            if ( this->text != NULL )
            {
                delete this->text;
            }

            this->text = new TextBox( text );
        };

        //static void setup( Renderer *r );


    protected:
        void onClickEvent( int mx, int my, int button, bool press )
        {
            if ( button == MOUSE_LEFT )
            {
                func( arg );
            }
        };
        void onClickDragEvent( int mx, int my, int nx, int ny, int button, bool press ) {};
        void onDblClickEvent( int mx, int my, int button, bool press ) {};
        void onKeyEvent( char key, bool press )
        {
            // TODO: find which key code the enter key gives
            //if ( key == ENTER && press )
            //{
            //    func( arg );
            //{
        };

        // Does nothing - the TextBox will be drawn anyway
        void draw( Renderer *r )
        {
        };

    private:
        TextBox *text;

        void ( *func )( void * );
        void *arg;
        

};

//---------------------------------------------------------------------------
#endif
