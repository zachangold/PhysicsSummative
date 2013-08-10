//---------------------------------------------------------------------------

#ifndef RendererH
#define RendererH

#include "Entity.h"
#include "Camera.h"
#include "Util.h"
#include "UIElement.h"
#include "Graph.h"

class Renderer
{
    public:
        Renderer( void );
        Renderer( HWND hWnd, int width, int height );
        ~Renderer( void );

        void create( HWND hWnd, int width, int height );
        void destroy( void );

        void reset( void );

        void addEntity( Entity *e );

        void draw( float msPassed );

        void setInputSource( InputSource *input );

        LPDIRECT3DDEVICE9 getDevice( void )
        {
            return device;
        };

        void addUIElement( UIElement *uie )
        {
            uiItems.push_back( uie );
        };

        void forget( void );

        void setupProjection( float fov );
        void setupOrthogonal( void );
        void setupViewTransform( Camera *c );
        void setupWorldTransform( Entity *e );
        void setupWorldTransform( float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz );

    private:
        typedef struct
        {
            std::vector< Triangle > *mainMem;
            LPDIRECT3DVERTEXBUFFER9 vBuffer;
            unsigned __int32 fvf;

            std::vector< Entity * > entities;
        } Model;

        std::vector< Model > models;
        std::vector< UIElement * > uiItems;

        LPDIRECT3D9 d3d;
        D3DPRESENT_PARAMETERS d3dpp;
        LPDIRECT3DDEVICE9 device;

        int screenWidth, screenHeight;

        Camera *cam;
        InputSource *input;

        void clearScreen( void );
        void updateScreen( void );

        D3DXMATRIX ident;

        bool deviceLost;
        void recoverD3D( void );
};


//---------------------------------------------------------------------------
#endif
 