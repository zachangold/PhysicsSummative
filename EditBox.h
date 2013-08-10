//---------------------------------------------------------------------------

#ifndef EditBoxH
#define EditBoxH

#include "UIElement.h"

class EditBox : public UIElement
{
    public:
        EditBox( void );
        ~EditBox( void );


    protected:
        void onClickEvent( int mx, int my, int button, bool press );
        void onClickDragEvent( int mx, int my, int nx, int ny, int button, bool press );
        void onDblClickEvent( int mx, int my, int button, bool press );
        void onKeyEvent( char key, bool press );

    private:
        std::string text;

};

//---------------------------------------------------------------------------
#endif
