#include "KinectScreenRunner.h"

/******************************************************
 * Function: KinectScreenRunner::DrawPixelBuffer()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A function to draw whatever's in the pixel buffer to
 * the window.
 ******************************************************/
void KinectScreenRunner::DrawPixelBuffer(SDL_Texture* texture, SDL_Renderer* renderer,
    const uint32* pixelBuffer)
{
    // Upload the pixel buffer to a texture
    void* pixels;
    int pitch;
    SDL_LockTexture(texture, nullptr, &pixels, &pitch);
    if (pitch == SCRWIDTH * 4)
        memcpy(pixels, pixelBuffer, SCRWIDTH * SCRHEIGHT * 4);
    else
    {
        const uint32* src = pixelBuffer;
        char* dst = (char*)pixels;
        for (int y = 0; y < SCRHEIGHT; ++y)
        {
            memcpy(dst, src, SCRWIDTH * 4);
            src += SCRWIDTH;
            dst += pitch;
        }
    }
    SDL_UnlockTexture(texture);

    // Copy the texture to the frame buffer
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // Present the frame buffer on the screen
    SDL_RenderPresent(renderer);
}

/******************************************************
 * Function: KinectScreenRunner::RunScreen()
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A function to instantiate all of the window and screen
 * objects, like the renderer.
 ******************************************************/
int KinectScreenRunner::RunScreen() {
    //initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window for the depth map
    SDL_Window* window = SDL_CreateWindow(
        "Kinect Floor Map - Top View", 100, 100, SCRWIDTH, SCRHEIGHT, 0);
    if (window == nullptr)
        return 1;

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
        return 2;


    //create a texture
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        SCRWIDTH, SCRHEIGHT);
    if (texture == nullptr)
        return 3;

    if (texture == nullptr)
        return 3;

    if (m_pixel_buffer_1 == nullptr)
        return 4;

    //clear the pixel buffer
    memset(m_pixel_buffer_1, 0, SCRWIDTH * SCRHEIGHT * 4);

    //draw pixel buffer to the screen
    DrawPixelBuffer(texture, renderer, m_pixel_buffer_1);

    KinectProgram program;
    program.SetPixelBuffer(m_pixel_buffer_1);
    program.Init();

    auto lastTime = Clock::now();

    bool running = true;
    while (running)
    {
        //get events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                //pressing the cross or pressing escape will quit the application
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running = false;
                break;

            default: //ignore other events for this demo
                break;
            }
        }

        //calculate delta time
        const auto now = Clock::now();
        const auto duration = duration_cast<microseconds>(now - lastTime);
        const float deltaTime = duration.count() / 1000000.0f;
        lastTime = now;

        //update the application
        program.Run(deltaTime);

        //draw pixel buffer to the screen
        DrawPixelBuffer(texture, renderer, m_pixel_buffer_1);

        memset(m_pixel_buffer_1, 0, SCRWIDTH * SCRHEIGHT * 4);
    }


    //clean up
    program.Shutdown();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}