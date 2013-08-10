//---------------------------------------------------------------------------

#pragma hdrstop

#include "Camera.h"

Camera::Camera( void )
{
    pos = 0.0f;
    rot = 0.0f;
    fov = 0.0f;

    input = NULL;
};

Camera::Camera( InputSource *input )
{
    pos = 0.0f;
    rot = 0.0f;
    fov = 0.0f;
    this->input = NULL;

    init( input );
};

Camera::~Camera( void )
{
    this->input = NULL;
};

void Camera::init( InputSource *input )
{
    this->input = input;
};

void Camera::updatePos( float msPassed )
{
    const float MOVE_SPEED = 4.0f;
    const float PI_OVER_180 = 3.1415926 / 180.0;

    rot.v[ 1 ] = input->getInputState()->getMouseState()->x;
    rot.v[ 0 ] = input->getInputState()->getMouseState()->y;


    if ( input->getInputState()->getKey( 'W' ) ) {
        // If the 'w' key is being pressed, move forward
        pos.v[ 0 ] -= sin( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
        pos.v[ 2 ] -= cos( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
    }
    if ( input->getInputState()->getKey( 'S' ) ) {
        // If the 's' key is being pressed, move backward
        pos.v[ 0 ] += sin( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
        pos.v[ 2 ] += cos( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
    }
    if ( input->getInputState()->getKey( 'A' ) ) {
        // If the 'a' key is being pressed, move to the left
        pos.v[ 0 ] += cos( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
        pos.v[ 2 ] -= sin( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
    }
    if ( input->getInputState()->getKey( 'D' ) ) {
        // If the 'd' key is being pressed, move to the right
        pos.v[ 0 ] -= cos( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
        pos.v[ 2 ] += sin( rot.v[ 1 ] * PI_OVER_180 ) * MOVE_SPEED * msPassed;
    }
    if ( input->getInputState()->getKey( 'C' ) ) {
        // if the 'c' key is being pressed, move downwards
        pos.v[ 1 ] += MOVE_SPEED * msPassed;
    }
    if ( input->getInputState()->getKey( ' ' ) ) {
        // if the space bar is being pressed, move upwards
        pos.v[ 1 ] -= MOVE_SPEED * msPassed;
    }
};

Point *Camera::getPos( void )
{
    return &pos;
};

Point *Camera::getRot( void )
{
    return &rot;
};

float Camera::getFOV( void )
{
    return 65.0f;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
