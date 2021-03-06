//---------------------------------------------------------------------------

#ifndef TextBoxH
#define TextBoxH

#include "UIElement.h"
#include "D3DFont.h"

class TextBox : public UIElement
{
    public:
        TextBox( void )
        {
            setText( "" );
            setColor( 1.0, 1.0, 1.0, 1.0 );
            setFont( &defaultFont );
            alignment = -1;
            scale = Point( 100.0, 100.0, 1.0 );
            pos = Point( 0.0, 0.0, 1.0 );
        };

        TextBox( std::string text )
        {
            setText( "" );
            setColor( 1.0, 1.0, 1.0, 1.0 );
            setFont( &defaultFont );
            alignment = -1;
            scale = Point( 100.0, 100.0, 1.0 );
            pos = Point( 0.0, 0.0, 1.0 );

            setText( text );
            setColor( 1.0f, 1.0f, 1.0f, 1.0f );
            setFont( &defaultFont );
            alignment = -1;
        };

        TextBox( std::string text, DWORD color )
        {
            ( *this ) = TextBox();
            setText( text );
            setColor( color );
            setFont( &defaultFont );
            alignment = -1;
        };

        TextBox( std::string text, float r, float g, float b, float a )
        {
            ( *this ) = TextBox();
            setText( text );
            setColor( r, g, b, a );
            setFont( &defaultFont );
            alignment = -1;
        };
        ~TextBox( void )
        {

        };

        void setText( std::string text )
        {
            this->text = text;
        };
        void setColor( DWORD color )
        {
            this->color = color;
        };
        void setColor( float r, float g, float b, float a )
        {
            color = D3DXCOLOR( r, g, b, a );
        };
        void setFont( D3DFont *font )
        {
            this->font = font;
        };

        void draw( Renderer *r );
        static void setup( Renderer *r );
        int alignment;
        int flags;

    private:
        std::string text;
        DWORD color;
        D3DFont *font;

        static D3DFont defaultFont;
        static std::vector< UIPoint > points;
        static LPDIRECT3DVERTEXBUFFER9 vBuffer;

        static std::vector< UIPoint > setupPoints( void )
        {
            std::vector< UIPoint > rtn;
            float alpha = 0.2;

            DWORD color = D3DXCOLOR( 0.2, 0.2, 0.2, alpha );

            // top-left triangle
            rtn.push_back( UIPoint(-0.5,-0.5, 0.0, color ) );
            rtn.push_back( UIPoint( 0.5,-0.5, 0.0, color ) );
            rtn.push_back( UIPoint(-0.5, 0.5, 0.0, color ) );

            // bottom-right triangle
            rtn.push_back( UIPoint( 0.5, 0.5, 0.0, color ) );
            rtn.push_back( UIPoint(-0.5, 0.5, 0.0, color ) );
            rtn.push_back( UIPoint( 0.5,-0.5, 0.0, color ) );

            return rtn;
        };

};

//---------------------------------------------------------------------------
#endif
