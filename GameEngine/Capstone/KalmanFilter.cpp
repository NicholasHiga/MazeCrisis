#include "KalmanFilter.h"
#include <iostream>

namespace MazeCrisis
{
    //const mat3&
    void
    KalmanFilter::predict()
    {
        // Mat statePre;           //!< predicted state x(k)=A*x(k-1)+B*u(k)
        // Mat errorCovPre;        //!< priori error estimate covariance matrix P'(k)=A*P(k-1)*At + Q)*/

        statePre = transitionMatrix * statePost;
        errorCovPre = transitionMatrix * errorCovPost
            * transpose(transitionMatrix) + processNoiseCov;
    }

    const mat3&
    KalmanFilter::correct(const mat3& measurement)
    {
        // Mat gain;               //!< Kalman gain matrix K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)
        // Mat statePost;          //!< corrected state x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
        // Mat errorCovPost;       //!< posteriori error estimate covariance matrix P(k)=(I-K(k)*H)*P'(k)
        /*gain = errorCovPre * transpose(measurementMatrix)
            * inverse(measurementMatrix * errorCovPre * 
          transpose(measurementMatrix) + measurementNoiseCov);
        statePost = statePre + gain * (measurement - measurementMatrix * statePre);
        errorCovPost = errorCovPre - gain * measurementMatrix * errorCovPre;*/
    
        gain = errorCovPre * measurementMatrix
            * inverse(measurementMatrix * errorCovPre *
                measurementMatrix + measurementNoiseCov);
        statePost = statePre + gain * (measurement - measurementMatrix * statePre);
        errorCovPost = errorCovPre - gain * measurementMatrix * errorCovPre;
        return statePost;
        // gain = 3 x 3 * 3 x 1 + inverse(1 x 3 * 3 x 3 * 3 x 1 + 3 x 3)
        //      = 3 x 1 + inverse(1 x 3 * 3 x 1 + 3 x 3)
        //      = 3 x 1 + inverse( 
    }

    void
    KalmanFilter::printMatrix(mat3 mat, std::string matName = "")
    {
        if (matName != "")
            std::cout << matName.c_str() << std::endl;
        std::cout << "["
            << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ";" << std::endl
            << "  " << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ";" << std::endl
            << "  " << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << "]" << std::endl;
    }

    void
    KalmanFilter::printAllMatrices()
    {
        printMatrix(statePre, "statePre");
        std::cout << std::endl;

        printMatrix(statePost, "statePost");
        std::cout << std::endl;

        printMatrix(transitionMatrix, "transitionMatrix");
        std::cout << std::endl;

        printMatrix(measurementMatrix, "measurementMatrix");
        std::cout << std::endl;

        printMatrix(processNoiseCov, "processNoiseCov");
        std::cout << std::endl;

        printMatrix(measurementNoiseCov, "measurementNoiseCov");
        std::cout << std::endl;

        printMatrix(errorCovPre, "errorCovPre");
        std::cout << std::endl;

        printMatrix(gain, "gain");
        std::cout << std::endl;

        printMatrix(errorCovPost, "errorCovPost");
        std::cout << std::endl;
    }
}