#include "app.h"

/******************************************************
 * Function: App::Init()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A program to initialize the app
 * before the camera starts running
 ******************************************************/
void App::Init()
{
    //put initialization stuff here

    HRESULT hr;


    //get the kinect sensor
    hr = GetDefaultKinectSensor(&m_sensor);
    if (FAILED(hr))
    {
        printf("Failed to find the kinect sensor!\n");
        exit(10);
    }
    m_sensor->Open();


    //get the depth frame source
    IDepthFrameSource* depthFrameSource;
    hr = m_sensor->get_DepthFrameSource(&depthFrameSource);

    if (FAILED(hr))
    {
        printf("Failed to get the depth frame source.\n");
        exit(10);
    }
    //get depth frame description
    IFrameDescription* frameDesc;
    depthFrameSource->get_FrameDescription(&frameDesc);
    frameDesc->get_Width(&m_depth_width);
    frameDesc->get_Height(&m_depth_height);

    //get the depth frame reader
    hr = depthFrameSource->OpenReader(&m_depth_frame_reader);
    if (FAILED(hr))
    {
        printf("Failed to open the depth frame reader!\n");
        exit(10);
    }
    //release depth frame source
    SafeRelease(depthFrameSource);

    //allocate depth buffer
    m_depth_buffer = new uint16[m_depth_width * m_depth_height];

    //get color frame source
    IColorFrameSource* colorFrameSource;
    hr = m_sensor->get_ColorFrameSource(&colorFrameSource);
    if (FAILED(hr))
    {
        printf("Failed to get color frame source!\n");
        exit(10);
    }

    //get color frame reader
    hr = colorFrameSource->OpenReader(&m_color_frame_reader);
    if (FAILED(hr))
    {
        printf("Failed to open color frame reader!\n");
        exit(10);
    }

    //release the color frame source
    SafeRelease(colorFrameSource);

    //allocate color buffer
    m_color_buffer = new uint32[1920 * 1080];

    //get the coordinate mapper
    hr = m_sensor->get_CoordinateMapper(&m_coordinate_mapper);
    if (FAILED(hr))
    {
        printf("Failed to get coordinate mapper!\n");
        exit(10);
    }

    //allocate a buffer of color space points
    m_color_space_points = new ColorSpacePoint[m_depth_width * m_depth_height];
}

/******************************************************
 * Function: App::Run()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A method to run the camera and check it is working
 ******************************************************/
void App::Run(float deltaTime)
{
    using std::cout;
    //put update and drawing stuff here

    HRESULT hr;

    //depth stuff
    IDepthFrame* depthFrame;
    hr = m_depth_frame_reader->AcquireLatestFrame(&depthFrame);


    if (FAILED(hr)) return;

    hr = depthFrame->CopyFrameDataToArray(
        m_depth_width * m_depth_height, m_depth_buffer);


    if (FAILED(hr))
    {
        SafeRelease(depthFrame);
        printf("oh no, something went wrong while copying!\n");
        return;
    }

    SafeRelease(depthFrame);



    //color stuff
    IColorFrame* colorFrame;
    hr = m_color_frame_reader->AcquireLatestFrame(&colorFrame);
    if (FAILED(hr))
        return;

    hr = colorFrame->CopyConvertedFrameDataToArray(
        1920 * 1080 * 4, (BYTE*)m_color_buffer, ColorImageFormat_Bgra);
    if (FAILED(hr))
        return;

    SafeRelease(colorFrame);

    hr = m_coordinate_mapper->MapDepthFrameToColorSpace(
        m_depth_width * m_depth_height, m_depth_buffer,
        m_depth_width * m_depth_height, m_color_space_points);
    if (FAILED(hr))
    {
        printf("Oh no! Failed map the depth frame to color space!\n");
        return;
    }

}

/******************************************************
 * Function: App::Shutdown()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A method to destroy and release all app's objects
 ******************************************************/
void App::Shutdown()
{
    //put cleaning up stuff here

    delete[] m_color_buffer;
    SafeRelease(m_color_frame_reader);

    delete[] m_depth_buffer;
    SafeRelease(m_depth_frame_reader);
    SafeRelease(m_sensor);
}
