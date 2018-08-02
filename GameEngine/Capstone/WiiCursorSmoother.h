#pragma once
#include <list>
#include <glm/glm.hpp>

namespace MazeCrisis
{
    using namespace glm;
    using namespace std;

    class WiiCursorSmoother
    {
    public:
        WiiCursorSmoother();
        WiiCursorSmoother(unsigned int maxElements);
        vec2 addPointAndGetAverage(vec2 point);

    private:
        unsigned int numMaxElements;
        list<vec2> pointsToAverage;
        vec2 runningAverage;
    };
}