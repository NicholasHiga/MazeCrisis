#pragma once
#include <glm/glm.hpp>
#include "KalmanFilter.h"

namespace MazeCrisis
{
	using namespace glm;

	class WiiCursorSmoother2
	{
	public: 
		WiiCursorSmoother2(vec2 initialMouseCoords);
		vec2 update(vec2 input);

	private:
		// Helper methods
		mat3 matrixDiagonalInit(float value);
		mat3 matrixDiagonalInit(float value1, float value2, float value3);

		KalmanFilter kfX, kfY; // One filter for each x, y mouse positions.

		float processNoiseCov = 0.1f, measurementNoiseCov = 5.0f, 
			stateEstimationErrorCov = 50;

		float dt = 50.0f / 1000.0f; // dt / T
	};
}