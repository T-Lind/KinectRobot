#pragma once

#include <iostream>
#include <chrono>

#include "SDL.h"

#include "KinectProgram.h"


using namespace std::chrono;
typedef steady_clock Clock;

/******************************************************
 * Class: KinectScreenRunner
 *
 * Author: Tiernan Lindauer
 * Date: 7/8/2022
 *
 * Description:
 * A program to display a pixel buffer as an image in
 * a window on a screen.
 ******************************************************/
class KinectScreenRunner {
public:
	// Function to draw the pixels to a window in the current buffer
	void DrawPixelBuffer(SDL_Texture* texture, SDL_Renderer* renderer,
		const uint32* pixelBuffer);

	// Function to start the screen running
	int RunScreen();

	// Constant function to get the address of the pixel buffer
	const uint32* GetPixelBufferOneAddress() {
		return m_pixel_buffer_1;
	}

private:
	uint32* m_pixel_buffer_1 = new uint32[SCRWIDTH * SCRHEIGHT];
};
