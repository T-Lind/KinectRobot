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

	void DrawPixelBuffer(SDL_Texture* texture, SDL_Renderer* renderer,
		const uint32* pixelBuffer);
	int RunScreen();

	uint32* GetPixelBufferAddress() {
		return pixelBuffer;
	}

private:
	uint32* pixelBuffer = new uint32[SCRWIDTH * SCRHEIGHT];
};
