#include "WiiCursorSmoother.h"

namespace MazeCrisis
{
    WiiCursorSmoother::WiiCursorSmoother() : WiiCursorSmoother(10)
    {

    }

    WiiCursorSmoother::WiiCursorSmoother(unsigned int maxElements)
    {
        this->numMaxElements = maxElements;
    }

    vec2
    WiiCursorSmoother::addPointAndGetAverage(vec2 point)
    {
        if (pointsToAverage.size() >= numMaxElements)
        {
            runningAverage -= pointsToAverage.front();
            pointsToAverage.pop_front();
        }

        pointsToAverage.push_back(point);
        runningAverage += point;
        return vec2(runningAverage.x / pointsToAverage.size(),
            runningAverage.y / pointsToAverage.size());
    }
}
