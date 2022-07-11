#pragma once

//some often used C standard library header files
#include <cstdlib>
#include <cstdio>
#include <cstring>

//some often used STL header files
#include <iostream>
#include <memory>
#include <vector>

#include <Kinect.h>
#include <SDL_stdinc.h>

//size of the window
#define SCRWIDTH 512
#define SCRHEIGHT 424

//some useful typedefs for explicit type sizes
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

//safe way of deleting a COM object
template<typename T>
void SafeRelease(T& ptr) { if (ptr) { ptr->Release(); ptr = nullptr; } }

/******************************************************
 * Class: App
 *
 * Author: Tiernan Lindauer
 * Date: 7/9/2022
 *
 * Description:
 * A program to run the kinect sensor and interface
 * directly with its data return.
 ******************************************************/
class App
{
public:
    void Init();
    void Run(float deltaTime);
    void Shutdown();

    void const SetPixelBuffer(uint32* pixel_buffer)
    {
        this->m_pixel_buffer = pixel_buffer;
    }

    //safe way of plotting a pixel
    const void Plot(int x, int y, uint32 color)
    {
        if (x < 0 || x >= SCRWIDTH || y < 0 || y >= SCRHEIGHT)
            return;
        m_pixel_buffer[x + y * SCRWIDTH] = color;
    }

    static const double ToRadians(double degrees)
    {
        return degrees * M_PI / 180;
    }
    static const double ToDegrees(double radians)
    {
        return radians * 180 / M_PI;
    }

protected:
    //pointer to buffer that contains pixels that get pushed to the screen
    //size of this buffer is SCRWIDTH * SCRWIDTH * sizeof(uint32)
    uint32* m_pixel_buffer = nullptr;

    IKinectSensor* m_sensor = nullptr;
    IDepthFrameReader* m_depth_frame_reader = nullptr;
    IColorFrameReader* m_color_frame_reader = nullptr;
    ICoordinateMapper* m_coordinate_mapper = nullptr;

    uint32* m_color_buffer = nullptr;

    ColorSpacePoint* m_color_space_points = nullptr;
    uint16* m_depth_buffer = nullptr;

    int m_depth_width = 0, m_depth_height = 0;


    /**************************
        MATH CONSTANTS AND CONVERSIONS
   ***************************/

    const float SQRT_3 = 1.73205;

};