//---------------------------------------------------------------------------

#pragma hdrstop

#include "Renderer.h"
#include "TrackVar.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

Renderer::Renderer( void )
{
    d3d = NULL;
    device = NULL;

    screenWidth = 0;
    screenHeight = 0;

    cam = new Camera();

    D3DXMatrixIdentity( &ident );
};

Renderer::Renderer( HWND hWnd, int screenWidth, int screenHeight )
{
    d3d = NULL;
    device = NULL;

    this->screenWidth = 0;
    this->screenHeight = 0;

    cam = new Camera();

    D3DXMatrixIdentity( &ident );

    create( hWnd, screenWidth, screenHeight );
};

Renderer::~Renderer( void )
{
    destroy();
};

void Renderer::addEntity( Entity *e )
{
    for ( size_t i = 0; i < models.size(); ++i )
    {
        if ( models[ i ].mainMem == e->getModelInfo() )
        {
            models[ i ].entities.push_back( e );
        }
    }

    Model m;
    m.fvf = e->getFVF();
    m.mainMem = e->getModelInfo();
    m.entities.push_back( e );

    device->CreateVertexBuffer(sizeof( Triangle ) * m.mainMem->size(),
                               0,                               m.fvf,
                               D3DPOOL_MANAGED,
                               &m.vBuffer,
                               NULL);


    VOID* pVoid;

    m.vBuffer->Lock( 0, 0, &pVoid, 0 );    // locks v_buffer, the buffer we made earlier

    memcpy( pVoid, &( ( *( m.mainMem ) )[ 0 ] ), sizeof( Triangle ) * m.mainMem->size() );

    m.vBuffer->Unlock();

    models.push_back( m );
};

void Renderer::draw( float msPassed )
{
    cam->updatePos( msPassed );

    clearScreen();

    setupProjection( 45.0f );
    setupViewTransform( cam );
    device->SetRenderState( D3DRS_ZENABLE, TRUE );    // turn on the z-buffer

    device->BeginScene();

    for ( size_t m = 0; m < models.size(); ++m )
    {
        device->SetFVF( models[ m ].fvf );

        device->SetStreamSource( 0, models[ m ].vBuffer, 0, sizeof( Point ) );

        for ( size_t e = 0; e < models[ m ].entities.size(); ++e )
        {
            setupWorldTransform( models[ m ].entities[ e ] );

            device->SetTexture( 0, NULL );
            device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, models[ m ].mainMem->size() );
        }
    }

    setupOrthogonal();
    device->SetRenderState( D3DRS_ZENABLE, FALSE );    // turn on the z-buffer
    device->SetTransform( D3DTS_VIEW, &ident );


    for ( size_t u = 0; u < uiItems.size(); ++u )
    {
        uiItems[ u ]->draw( this );
    }

    TrackVar::drawAll( this );

    device->EndScene();

    updateScreen();
};


/**
 * D3DContext::init() creates the Direct3D device object in memory and sets
 * it up so the DirectX device can be drawn to by the main application.
 * After this, the device is ready to use for rendering
 */
void Renderer::create( HWND hWnd, int width, int height ) {

    // Create the Direct3D structure
    d3d = Direct3DCreate9( D3D_SDK_VERSION );

    // Set the window handle and the screen dimensions
    screenWidth = width;
    screenHeight = height;

    // clear out direct3d's parameter list, then set all of the necessary attributes
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;

    // Set up the format of how DirectX draws to the screen
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = screenWidth;
    d3dpp.BackBufferHeight = screenHeight;

    // Setup depth buffering
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


    // allow for fullscreen antialiasing
    //d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;


    // create a device using this information and the info from the d3dpp struct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &device);

    Graph::setup( this );
    TextBox::setup( this );

    reset();

};

/**
 * reset() simply resets all of DirectX's states and behaviours to the
 * default values for this application.
 */
void Renderer::reset( void )
{
    device->ShowCursor( 0 );

    device->SetRenderState( D3DRS_ZENABLE, TRUE );    // turn on the z-buffer

    // Tell D3D to combine the lighting and the texture colours
    device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );


    // Setup anisotropic filtering
    device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
    device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
    device->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, 16 );

    // Change a few of Direct3D's render states - for lighting and backface culling
    device->SetRenderState( D3DRS_LIGHTING, FALSE );    device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    //device->SetRenderState( D3DRS_NORMALIZENORMALS, FALSE );

    device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

    device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );


    //device->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 0, 255, 0 ) );
};


/**
 * setupProjection() sets up Direct3D's projection matrix.
 * The projection matrix is just a way of transforming a point in 3d space
 * onto the user's screen, creating an illusion of a 3D world.
 */
void Renderer::setupProjection( float fov ) {
    // The projection matrix
    D3DXMATRIX proj;

    // Setup the projection matrix
    D3DXMatrixPerspectiveFovLH(&proj,
                               D3DXToRadian( fov ),    // the horizontal field of view
                               ( FLOAT ) screenWidth / ( FLOAT ) screenHeight,    // aspect ratio
                               0.1f,    // the near view-plane
                               100.0f);    // the far view-plane

    // Set the projection matrix to the perspective matrix that we just made
    device->SetTransform( D3DTS_PROJECTION, &proj );

};

void Renderer::setupOrthogonal( void )
{
    D3DXMATRIX ortho;

    D3DXMatrixOrthoLH( &ortho, SCREEN_WIDTH, SCREEN_HEIGHT, 0.001, 10.0 );

    device->SetTransform( D3DTS_PROJECTION, &ortho );
};


/**
 * setupWorldProjection() modifies Direct3D's world transformation matrix.
 * The world matrix modifies how objects in the world are transformed. This
 * could include where the object is, how it is rotated, and how big it is.
 */
void Renderer::setupWorldTransform( Entity *e )
{
    // The matrix variables for rotation, translation, and scale
    D3DXMATRIX entityMat, rotX, rotY, rotZ, trans, scale;

    Point pos, rot, sca;
    pos = e->getPos();
    rot = e->getRot();
    sca = e->getScale();


    // setup the rotations
    D3DXMatrixRotationX( &rotX, D3DXToRadian( rot.v[ 0 ] ) );
    D3DXMatrixRotationY( &rotY, D3DXToRadian( rot.v[ 1 ] ) );
    D3DXMatrixRotationZ( &rotZ, D3DXToRadian( rot.v[ 2 ] ) );

    // Setup the scale
    D3DXMatrixScaling( &scale, sca.v[ 0 ], sca.v[ 1 ], sca.v[ 2 ] );

    // setup the translation
    D3DXMatrixTranslation( &trans, pos.v[ 0 ], pos.v[ 1 ], pos.v[ 2 ] );

    // Setup the entity's final transform
    entityMat = scale * rotX * rotY * rotZ * trans;

    // Set the world matrix to the entity's matrix
    device->SetTransform( D3DTS_WORLD, &entityMat );
};


/**
 * updateScreen() Sends Direct3D a message that tells it to update the
 * screen by sending the image that was rendered earlier by the game engine.
 */
void Renderer::updateScreen( void )
{
    device->Present( NULL, NULL, NULL, NULL );
};

/**
 * clearScreen() refreshes DirectX's Depth buffer. Clearing the depth
 * buffer prevents objects that are behind other objects from being
 * rendered in front of the object that should be in the foreground.
 */
void Renderer::clearScreen( void )
{
    device->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
    device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 64, 64, 64 ), 1.0f, 0 );
};

void Renderer::setInputSource( InputSource *input )
{
    this->input = input;
    cam->init( input );
};

void Renderer::forget( void )
{
    for ( size_t i = 0; i < models.size(); ++i )
    {
        models[ i ].entities.resize( 0 );
    }

    uiItems.resize( 0 );
};

void Renderer::destroy( void )
{
    if ( d3d != NULL )
    {
        d3d->Release();
        d3d = NULL;
    }

    if ( device != NULL )
    {
        device->Release();
        device = NULL;
    }

    for ( size_t i = 0; i < models.size(); ++i )
    {
        if ( models[ i ].vBuffer != NULL )
        {
            models[ i ].vBuffer->Release();
            models[ i ].vBuffer = NULL;
        }
    }

    delete cam;
    cam = NULL;

    input = NULL;
};

void Renderer::setupViewTransform( Camera *c )
{
    Point *pos, *rot;
    pos = c->getPos();
    rot = c->getRot();

    // The transformation matrices
    D3DXMATRIX rotX, rotY, trans, eyeTrans;

    // Rotate the world around the camera
    D3DXMatrixRotationX( &rotX, -D3DXToRadian( rot->v[ 0 ] ) );
    D3DXMatrixRotationY( &rotY, -D3DXToRadian( rot->v[ 1 ] ) );

    // move the world around the camera
    D3DXMatrixTranslation( &trans, pos->v[ 0 ], pos->v[ 1 ], pos->v[ 2 ] );

    // send the viewing transformation to Direct3D
    device->SetTransform( D3DTS_VIEW, &( trans * rotY * rotX ) );
};

void Renderer::setupWorldTransform( float x, float y, float z,
                                    float rx, float ry, float rz,
                                    float sx, float sy, float sz )
{
    // The matrix variables for rotation, translation, and scale
    D3DXMATRIX entityMat, rotX, rotY, rotZ, trans, scale;

    // setup the rotations
    D3DXMatrixRotationX( &rotX, D3DXToRadian( rx ) );
    D3DXMatrixRotationY( &rotY, D3DXToRadian( ry ) );
    D3DXMatrixRotationZ( &rotZ, D3DXToRadian( rz ) );

    // Setup the scale
    D3DXMatrixScaling( &scale, sx, sy, sz );

    // setup the translation
    D3DXMatrixTranslation( &trans, x, y, z );

    // Setup the entity's final transform
    entityMat = rotX * rotY * rotZ * scale * trans;

    // Set the world matrix to the entity's matrix
    device->SetTransform( D3DTS_WORLD, &entityMat );
};


//---------------------------------------------------------------------------
#pragma package(smart_init)

