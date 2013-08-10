//---------------------------------------------------------------------------

#ifndef CameraH
#define CameraH

#include "Util.h"
#include "InputSource.h"

class Camera
{
    public:
        Camera( void );
        Camera( InputSource *input );
        ~Camera( void );

        void init( InputSource *input );
        void updatePos( float msPassed );

        Point *getPos( void );
        Point *getRot( void );
        float getFOV( void );

    private:
        Point pos;
        Point rot;
        float fov;

        InputSource *input;
};


//---------------------------------------------------------------------------
#endif
