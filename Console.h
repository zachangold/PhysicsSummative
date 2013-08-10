//---------------------------------------------------------------------------

#ifndef ConsoleH
#define ConsoleH

#include "UIElement.h"

class Console : public UIElement
{
    public:
        Console( void );
        ~Console( void );



    protected:
        void onClickEvent( int mx, int my, int button, bool press );
        void onClickDragEvent( int mx, int my, int nx, int ny, int button, bool press );
        void onDblClickEvent( int mx, int my, int button, bool press );
        void onKeyEvent( char key, bool press );

    private:
        

};

//---------------------------------------------------------------------------
#endif
