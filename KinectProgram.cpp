#include "KinectProgram.h"

void printFormat(int number) {
    using std::cout;

    if (number > 999) {
        cout << "\b\b\b\b" << number;
    }
    else if (number > 99) {
        cout << "\b\b\b" << number;
    }
    else {
        cout << "\b\b\b\b" << "0000";
    }
}

/******************************************************
 * Function: KinectProgram::Init()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A program to initialize the app and whatever else you
 * want to before the camera starts running
 ******************************************************/
void KinectProgram::Init()
{
    using std::cout;
    using std::endl;

    App::Init();

    // Add your initialization code here


    cout << "Initialization successful." << endl;
}
/******************************************************
 * Function: KinectProgram::Run()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 *  The color and depth buffer is 217,088 elements long.
 *  The height (# of rows) in the array is 424 and the
 *  width (# of cols) is 512. The top left corner
 *  is 0, the top right corner is 512, the bottom
 *  left corner is 512*424-512, and the bottom
 *  right is 512*424. The center row STARTS at
 *  512*424/2.
 * 
 *  To set a pixel to a color use
 *  m_pixelBuffer[i] = RGB(255, 255, 255);
 *  (for example)
 *
 *  !!!!!!
 *  NOTE: RGB IS REVERSED - ACTUALLY BGR!
 *  !!!!!!
 *
 *  DO NOT move to a 2D array - too much lag.
 *
 *  This method is called in each frame.
 ******************************************************/
void KinectProgram::Run(float deltaTime)
{
    using std::cout;
    using std::endl;

    App::Run(deltaTime);

    // Add your vision loop code here

    // Display pixels and depths

    for (int i = 0; i < depthWidth * depthHeight; ++i)
    {
        uint16 distance = m_depthBuffer[i];

        // Check to see if the kinect has detected a distance in range
        if (distance != 0)
        {
            // Label image


            // Check to see if the pixel is part of the floor - label green if so
            if (IsPartOfFloor(distance, PixelYAngle(GetYValue1D(i))))
            {
                m_pixelBuffer[i] = RGB(0, 200, 0);
            }

            // Check to see if the pixel is part of the floor - label green if so
            else if (IsPartOfWall(distance, PixelYAngle(GetYValue1D(i))))
            {
                m_pixelBuffer[i] = RGB(0, 0, 200);
            }

            // If not report the color as a grayscale value depending on depth
            else
            {
                m_pixelBuffer[i] = RGB(distance / 19, distance / 19, distance / 19);

            }






        }

        // The kinect cannot detect the distance at this pixel and sets it to salmon color
        else
        {
            m_pixelBuffer[i] = RGB(220, 220, 255);
        }
    }

}

/******************************************************
 * Function: KinectProgram::IsPartOfWall()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * Returns a boolean of whether or not a specific
 * distance at a specific y angle is part of the
 * floor or not.
 ******************************************************/
bool KinectProgram::IsPartOfFloor(int distance, float yAngleInFrame)
{
    /*
        Do an immediate check here and make an assumption that the
        floor will not extend past halfway in the image. This might
        need to change depending on the robot. Important for removing
        "wall bands" of detected floor.
    */

    if (-yAngleInFrame * 180 / 3.14159 > 0)
        return false;

    // Total angle the depth is below the horizontal - must initially convert to degrees
    const float yAngle = cameraAngle + (180 * -yAngleInFrame / 3.14159);

    /*
        The upperand lower bound for what is an acceptable depth
        measurement to be considered part of the floor
    */
    const float cameraHeightUpperBound = cameraHeight * (1 + (marginOfError / 100.0));
    const float cameraHeightLowerBound = cameraHeight * (1 - (marginOfError / 100.0));


    // Compute the y distance from the camera the depth is
    const float yValueFromDepth = std::abs(distance*std::sin(3.15159 * yAngle / 180));


    // Check to see if the y distance is about how high the camera is off of the ground
    if (yValueFromDepth >= cameraHeightLowerBound && yValueFromDepth <= cameraHeightUpperBound)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/******************************************************
 * Function: KinectProgram::IsPartOfWall()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * Returns a boolean of whether or not a specific
 * distance at a specific y angle is part of the
 * floor or not.
 ******************************************************/
bool KinectProgram::IsPartOfWall(int distance, float yAngleInFrame)
{
    // Total angle the depth is below the horizontal
    float yAngle = cameraAngle + yAngleInFrame;

    /* 
        The upperand lower bound for what is an acceptable depth
        measurement to be considered part of the floor
    */
    const float cameraHeightUpperBound = cameraHeight * (1 + (marginOfError / 100.0));
    const float cameraHeightLowerBound = cameraHeight * (1 - (marginOfError / 100.0));

    // Compute the y distance from the camera the depth is
    const float yValueFromDepth = std::abs(distance * std::sin(3.15159 * yAngle / 180));

    //std::cout << yValueFromDepth << std::endl;

    // Check to see if the y distance is about how high the camera is off of the ground
    if (yValueFromDepth >= cameraHeightLowerBound && yValueFromDepth <= cameraHeightUpperBound)
    {
        return true;
    }
    else
    {
        return false;
    }
}