#include "app.h"

#pragma once

/******************************************************
 * Class: KinectProgram
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A program to allow use the color and depth buffer in
 * an Init() and Run() method.
 ******************************************************/
class KinectProgram : public App
{
public:
    // Code to run at initialization
    void Init();

    // Code to run during the camera's stream
    void Run(float deltaTime);


private:
    /**************************
        CAMERA POSITION INFORMATION
    **************************/

    // The height the camera is from the ground in mm
    static const int CAMERA_HEIGHT = 760;

    // The angle the camera is below the horizontal, in degrees
    static const int CAMERA_ANGLE = -10;    



    // An acceptable margin of error for eliminating depth measurement noise, in percent
    static const int MARGIN_OF_ERROR = 5;

    static const int SIZE_OF_TOP_VIEW = 5000; // In millimeters

    /**************************
        KINECT CHARACTERISTICS
        WILL NOT CHANGE!!
    **************************/

    // Height of the depth frame
    static const int DEPTH_FRAME_HEIGHT = 424;

    // Width of the depth frame
    static const int DEPTH_FRAME_WIDTH = 512;

    // Upper limit of what is considered the camera height - used for floor detection
    const float CAMERA_HEIGHT_UPPER_BOUND = CAMERA_HEIGHT * (1 + (MARGIN_OF_ERROR / 100.0));

    // Lower limit of what is considered the camera height - used for floor detection
    const float CAMERA_HEIGHT_LOWER_BOUND = CAMERA_HEIGHT * (1 - (MARGIN_OF_ERROR / 100.0));


    /**************************
        DETECTION FUNCTIONS
    **************************/

    // Determine if a certain angle and distance is part of the floor
    bool IsPartOfFloor(int distance, double yAngle);
    // Determine if a certain angle and distance is part of the wall
    bool IsPartOfWall(int distance, double yAngle);

    int TopViewPlotDepthX(int i, int distance);
    int TopViewPlotDepthY(int i, int distance);



    /**************************
        PIXEL AND BUFFER OPERATIONS
    **************************/

    // Get the x angle from a pixel position
    const double PixelXAngle(int x_position)
    {
        return std::atan((7 * (x_position - (DEPTH_FRAME_WIDTH / 2))) / (DEPTH_FRAME_WIDTH / 2.0 * 10));
    }

    // Get the y angle from a pixel position
    const double PixelYAngle(int y_position)
    {
        return -std::atan((SQRT_3 * (((DEPTH_FRAME_HEIGHT / 2)-y_position)))/(DEPTH_FRAME_HEIGHT / 2 * 3));
    }

    /*
        Convert an x - y coordinate to a single index for
        the pixel buffer and depth buffer.
    */
    const int ConvertCoordinate(int x, int y)
    {
        return y * DEPTH_FRAME_WIDTH + x;
    }

    // Uses integer division to calculate the y index value of an item in a list
    const int GetYValue1D(int index_in_1D_list)
    {
        return index_in_1D_list / DEPTH_FRAME_WIDTH;
    }

    // Uses integer modulus to calculate the x index value of an item in a list
    const int GetXValue1D(int index_in_1D_list)
    {
        return index_in_1D_list % DEPTH_FRAME_WIDTH;
    }
};