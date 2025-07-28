#ifndef CRTAABB
#define CRTAABB

#include "crtVector.hpp"
#include "triangle.hpp"
#include <limits>
class AABB {
    public:
        AABB(){}
        AABB(CRTTriangle triangle){ include(triangle);}
        void include(CRTTriangle triangle);
        void include(AABB boundingBox);


        CRTVector min{std::numeric_limits<float>::max()};
        CRTVector max{std::numeric_limits<float>::lowest()};

    private:

};
#endif