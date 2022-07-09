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
    // The height the camera is from the ground in mm
    static const int cameraHeight = 760;

    // The angle the camera is below the horizontal, in degrees
    static const int cameraAngle = -10;    

    // An acceptable margin of error for eliminating depth measurement noise, in percent
    static const int marginOfError = 10;

    // Determine if a certain angle and distance is part of the floor
    bool IsPartOfFloor(int distance, float yAngle);
    // Determine if a certain angle and distance is part of the wall
    bool IsPartOfWall(int distance, float yAngle);





    // Get the x angle from a pixel position
    double PixelXAngle(int xPosition) const
    {
        return std::atan((7 * (xPosition-256)) / 2560.0);
    }

    // Get the y angle from a pixel position
    double PixelYAngle(int yPosition) const
    {
        return -std::atan((1.7321*((212-yPosition)))/636.0);
    }

    /* Convert an x - y coordinate to a single index for
    the pixel buffer and depth buffer. */
    int convertCoordinate(int x, int y) const
    {
        return y * depthWidth + x;
    }

    // Uses integer division to calculate the y index value of an item in a list
    int GetYValue1D(int indexIn1DList) const
    {
        return indexIn1DList / depthWidth;
    }
    // Uses integer division to calculate the y index value of an item in a list
    int GetXValue1D(int indexIn1DList) const
    {
        return indexIn1DList % depthWidth;
    }
};