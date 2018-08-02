#pragma once
#include <glm/glm.hpp>
#include <string>

// Using my previously done Mat4x4 and updated it to a Mat5x5 since any of the
// current libraries I'm using don't have a Mat5x5. This KalmanFilter will be
// hard coded (size wise) so it will only be using a Mat5x5. This is a very
// crude implementation of a Kalman Filter, so it is not very robust.

// This class is basically a modified version of OpenCV's KalmanFilter. This is
// because importing OpenCV just for the KalmanFilter class would be too much 
// overhead for my liking.

using namespace glm;

namespace MazeCrisis
{
    // Helper class used specifically to make glm compatible for this specific
    // application.
    class inverseVec3
    {

    };

    // This is a simple version of the Kalman Filter, which isn't very flexible.
    // in the equation x(k) = Ax(k - 1) + Bu(k) + w(k - 1), this version assumes
    // that there is no u(k) or in other words no control signal.
    class KalmanFilter
    {
        // x(k) = Ax(k - 1) + Bu(k) + w(k - 1)
        // z(k) = Hx(k) + v(k)

        //Mat statePre;           //!< predicted state (x'(k)): x(k)=A*x(k-1)+B*u(k)
        //Mat statePost;          //!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
        //Mat transitionMatrix;   //!< state transition matrix (A)
        //Mat measurementMatrix;  //!< measurement matrix (H)
        //Mat processNoiseCov;    //!< process noise covariance matrix (Q)
        //Mat measurementNoiseCov;//!< measurement noise covariance matrix (R)
        //Mat errorCovPre;        //!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/
        //Mat gain;               //!< Kalman gain matrix (K(k)): K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)
        //Mat errorCovPost;       //!< posteriori error estimate covariance matrix (P(k)): P(k)=(I-K(k)*H)*P'(k)

        // Time Update (Prediction)
        // Mat statePre;           //!< predicted state (x'(k)): x(k)=A*x(k-1)+B*u(k)
        // Mat errorCovPre;        //!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/

        // Measurement Update (Correction)
        // Mat gain;               //!< Kalman gain matrix (K(k)): K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)
        // Mat statePost;          //!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
        // Mat errorCovPost;       //!< posteriori error estimate covariance matrix (P(k)): P(k)=(I-K(k)*H)*P'(k)

        // ProcessNoise or (Q) is additional uncertainty from environment (set to standard dev of sensor)
        // MeasurementNoise (R) Sensor noise, covariance of uncertainty
        // 
        // X(k) = [x(k), y(k)]'   (Position)
        // V(k) = [vx(k), vy(k)]' (Velocity)
        // X(k + 1) = X(k) + dt*V(k) + 0.5*dt*dt*a(k)
        // V(k + 1) = V(k) + t*a(k)    
        // a(k + 1) = a(k)
        // X =  [ x, y, v_x, v_y, a]
        // M =    { 1, 0, dt, 0, 0.5 dt dt } {  x  }
        //        { 0, 1, 0, dt, 0.5 dt dt } {  y  }
        //        { 0, 0, 1, 0, dt         } { v_x }
        //        { 0, 0, 0, 1, dt         } { v_y }
        //        { 0, 0, 0, 0, 1   } {  a  }

        // X = [ x, v_x, a ]
        // M = { 1, dt, 0.5 dt dt } {  x  }
        //     { 0, 1,  dt           } { v_x }
        //       { 0, 0,  1         } {  a  }

        // X(k + 1) = X(k) + dt *  V(k) + 0.5 * dt * dt * a(k)
        // V(k + 1) = V(k) + a(k)

        // X = [ y, v_y, a ]
        // M = { 1, dt, 0.5 dt dt } {  y  }
        //     { 0, 1,  dt           } { v_y }
        //       { 0, 0,  1         } {  a  }
    public:
        //! computes predicted state
        void predict();
        //! updates the predicted state from the measurement
        const mat3& correct(const mat3& measurement);

        void printMatrix(mat3 mat, std::string matName);
        void printAllMatrices();

        mat3 statePre;           //!< predicted state (x'(k)): x(k)=A*x(k-1)+B*u(k)
        mat3 statePost;          //!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
        mat3 transitionMatrix;   //!< state transition matrix (A)
        //mat3 controlMatrix;       //!< control matrix (B) (not used if there is no control)
        mat3 measurementMatrix;  //!< measurement matrix (H) 1 row, 3 cols
        mat3 processNoiseCov;    //!< process noise covariance matrix (Q)
        mat3 measurementNoiseCov;//!< measurement noise covariance matrix (R)
        mat3 errorCovPre;        //!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/
        mat3 gain;               //!< Kalman gain matrix (K(k)): K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)
        mat3 errorCovPost;       //!< posteriori error estimate covariance matrix (P(k)): P(k)=(I-K(k)*H)*P'(k)
    };
}