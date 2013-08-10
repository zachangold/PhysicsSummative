//---------------------------------------------------------------------------

#ifndef TrackVarH
#define TrackVarH

#include "Util.h"
#include "TextBox.h"

class TrackVar
{
    public:
        TrackVar( void )
        {
            trackVars.push_back( this );
            text = new TextBox();
        };

        ~TrackVar( void )
        {
            if ( text != NULL )
            {
                delete text;
                text = NULL;
            }

            for ( size_t i = 0; i < trackVars.size(); ++i )
            {
                if ( trackVars[ i ] == this )
                {
                    trackVars[ i ] = NULL;
                }
            }
        };

        void setFormat( std::string format )
        {
            this->format = format;
        };

        void setTracker( DynamicValue *value )
        {
            this->value = value;
        };

        void addToRenderer( Renderer *r )
        {
            text->addToRenderer( r );
            text->flags = DT_LEFT | DT_WORDBREAK;
            text->setSize( WIDTH, D3DFont::CHAR_HEIGHT );
        };

        static void drawAll( Renderer *r )
        {
            int lineNum = 0;

            for ( size_t i = 0; i < trackVars.size(); ++i )
            {
                if ( trackVars[ i ] != NULL )
                {
                    trackVars[ i ]->updateText();
                    trackVars[ i ]->text->setPos( -SCREEN_WIDTH_HALF + WIDTH_HALF + 10, SCREEN_HEIGHT_HALF - 10 - lineNum * D3DFont::CHAR_HEIGHT );
                    trackVars[ i ]->text->draw( r );

                    lineNum++;
                }
            }
        };

        static void deleteAll( void )
        {
            for ( size_t i = 0; i < trackVars.size(); ++i )
            {
                if ( trackVars[ i ] != NULL )
                {
                    delete trackVars[ i ];
                    trackVars[ i ] = NULL;
                }
            }
        };

        static const int WIDTH = 500;
        static const int WIDTH_HALF = WIDTH / 2;

    private:
        void updateText( void )
        {
            char buf[ 256 ];
            if ( value != NULL )
            {
                sprintf( buf, format.c_str(), value->get() );
            } else
            {
                sprintf( buf, "%s", format.c_str() );
            }

            text->setText( buf );
        };

        TextBox *text;
        DynamicValue *value;
        std::string format;

        static std::vector< TrackVar * > trackVars;

};

//---------------------------------------------------------------------------
#endif
 