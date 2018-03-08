#include "WiiCursorSmoother2.h"
#include <iostream>

namespace MazeCrisis
{
	WiiCursorSmoother2::WiiCursorSmoother2(int initialX, int initialY)
	{
		kfX.transitionMatrix = mat3(1, dt, 0.5f * dt * dt,
			0, 1, dt,
			0, 0, 1);
		kfY.transitionMatrix = mat3(1, dt, 0.5f * dt * dt,
			0, 1, dt,
			0, 0, 1);

		//kfX.statePost = matrixDiagonalInit(initialMouseCoords.x, 0.1f, 0.1f);
		//kfY.statePost = matrixDiagonalInit(initialMouseCoords.y, 0.1f, 0.1f);

		kfX.statePost = matrixDiagonalInit(initialX, 0.1f, 0.1f);
		kfY.statePost = matrixDiagonalInit(initialY, 0.1f, 0.1f);

		kfX.statePre = kfX.statePost;
		kfY.statePre = kfY.statePost;

		kfX.measurementMatrix = matrixDiagonalInit(1, 0, 0);
		kfY.measurementMatrix = matrixDiagonalInit(1, 0, 0);

		kfX.processNoiseCov = matrixDiagonalInit(processNoiseCov);
		kfY.processNoiseCov = matrixDiagonalInit(processNoiseCov);

		kfX.errorCovPre = matrixDiagonalInit(stateEstimationErrorCov);
		kfY.errorCovPre = matrixDiagonalInit(stateEstimationErrorCov);

		kfX.errorCovPost = matrixDiagonalInit(stateEstimationErrorCov);
		kfY.errorCovPost = matrixDiagonalInit(stateEstimationErrorCov);

		kfX.measurementNoiseCov = matrixDiagonalInit(measurementNoiseCov);
		kfY.measurementNoiseCov = matrixDiagonalInit(measurementNoiseCov);

		kfX.gain = matrixDiagonalInit(0);
		kfY.gain = matrixDiagonalInit(0);

		/*std::cout << "Init -----------------------------------------------------" << std::endl << std::endl;
		kfX.printAllMatrices();*/
	}

	vec2
	WiiCursorSmoother2::update(vec2 input)
	{

		kfX.predict();
		kfY.predict();

		/*std::cout << "After Predict " << numIterations
			<< " --------------------------------------------" << std::endl << std::endl;
		kfX.printAllMatrices();
			
		mat3 xIn = matrixDiagonalInit(50, 0.1f, 0.1f);
		mat3 yIn = matrixDiagonalInit(50, 0.1f, 0.1f);*/

		mat3 xIn = matrixDiagonalInit(input.x, 0.1f, 0.1f);
		mat3 yIn = matrixDiagonalInit(input.y, 0.1f, 0.1f);

		mat3 xEstimated = kfX.correct(xIn);
		mat3 yEstimated = kfY.correct(yIn);

		/*std::cout << "After Correct " << numIterations
			<< " --------------------------------------------" << std::endl << std::endl;

		kfX.printAllMatrices();
		std::cout << "estimatedX" << std::endl << xEstimated[0][0] << std::endl << std::endl;

		numIterations++;*/
		return vec2(xEstimated[0][0], yEstimated[0][0]);
	}

	mat3 
	WiiCursorSmoother2::matrixDiagonalInit(float value)
	{
		return mat3(value, 0, 0,
					0, value, 0,
					0, 0, value);
	}

	mat3
	WiiCursorSmoother2::matrixDiagonalInit(float value1, float value2, 
		float value3)
	{
		return mat3(value1, 0, 0,
					0, value2, 0,
					0, 0, value3);
	}
}