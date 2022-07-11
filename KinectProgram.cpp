#include "KinectProgram.h"

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

    for (int i = 0; i < DEPTH_FRAME_WIDTH * DEPTH_FRAME_HEIGHT; ++i)
    {
        uint16 distance = m_depth_buffer[i];

        // Check to see if the kinect has detected a distance in range
        //if (distance != 0)
        //{
            // Label image


        if (IsPartOfFloor(distance, PixelYAngle(GetYValue1D(i)))) {
            int plot_x = TopViewPlotDepthX(i, distance);
            int plot_y = TopViewPlotDepthY(i, distance);
            m_pixel_buffer[ConvertCoordinate(plot_x, plot_y)] = RGB(0, 255, 0);
        }


            //// Check to see if the pixel is part of the floor - label green if so
            //if (IsPartOfFloor(distance, PixelYAngle(GetYValue1D(i))))
            //{
            //    m_pixel_buffer[i] = RGB(0, 200, 0);
            //}


            //// If not report the color as a grayscale value depending on depth
            //else
            //{
            //    m_pixel_buffer[i] = RGB(distance / 19, distance / 19, distance / 19);

            //}


         //}

        // The kinect cannot detect the distance at this pixel and sets it to salmon color
        //else
        //{
        //    m_pixel_buffer[i] = RGB(220, 220, 255);
        //}
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
bool KinectProgram::IsPartOfFloor(int distance, double yAngleInFrame)
{
    /*
        Do an immediate check here and make an assumption that the
        floor will not extend past halfway in the image. This might
        need to change depending on the robot. Important for removing
        "wall bands" of detected floor. Here it is okay to check in
        radians because 0 degrees and 0 rads is equal
    */
    if (-yAngleInFrame > 0)
        return false;

    // Total angle the depth is below the horizontal - must initially convert to degrees
    const float yAngle = CAMERA_ANGLE + ToDegrees(-yAngleInFrame);


    // Compute the y distance from the camera the depth is
    const float yValueFromDepth = std::abs(distance*std::sin(ToRadians(yAngle)));


    // Check to see if the y distance is about how high the camera is off of the ground
    if (yValueFromDepth >= CAMERA_HEIGHT_LOWER_BOUND && yValueFromDepth <= CAMERA_HEIGHT_UPPER_BOUND)
    {
        return true;
    }
    else
    {
        return false;
    }
}


int KinectProgram::TopViewPlotDepthX(int i, int distance)
{

    using namespace std;


    double x_angle = PixelXAngle(GetXValue1D(i));
    double y_angle = PixelYAngle(GetYValue1D(i))+CAMERA_ANGLE;


    double x_distance = sin(x_angle)*cos(y_angle) * distance;

    // Convert x distance to a pixel value

    double distance_percent = x_distance / (SIZE_OF_TOP_VIEW);

    return distance_percent * (DEPTH_FRAME_WIDTH / 2) + (DEPTH_FRAME_WIDTH / 2);
}


int KinectProgram::TopViewPlotDepthY(int i, int distance)
{
    using namespace std;

    double x_angle = PixelXAngle(GetXValue1D(i));
    double y_angle = PixelYAngle(GetYValue1D(i))+CAMERA_ANGLE;

    double y_distance = cos(ToRadians(x_angle)) * cos(ToRadians(y_angle)) * distance;

    // Convert x distance to a pixel value

    double distance_percent = y_distance / (SIZE_OF_TOP_VIEW);


    return (DEPTH_FRAME_HEIGHT - distance_percent * DEPTH_FRAME_HEIGHT);
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
bool KinectProgram::IsPartOfWall(int distance, double yAngleInFrame)
{
    // Total angle the depth is below the horizontal - must initially convert to degrees
    const float yAngle = CAMERA_ANGLE + (ToDegrees(-yAngleInFrame));

    // Compute the y distance from the camera the depth is
    const float yValueFromDepth = std::abs(distance * std::sin(ToRadians(yAngle)));

    // Check to see if the y distance is about how high the camera is off of the ground
    if (yValueFromDepth >= CAMERA_HEIGHT_LOWER_BOUND && yValueFromDepth <= CAMERA_HEIGHT_UPPER_BOUND)
    {
        return true;
    }
    else
    {
        return false;
    }
}