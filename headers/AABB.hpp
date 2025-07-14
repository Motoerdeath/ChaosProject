#ifndef CRTAABB
#define CRTAABB

#include "crtVector.hpp"
#include <limits>
class AABB {
    public:
        AABB(){}
    private:
        CRTVector min{std::numeric_limits<float>::max()};
        CRTVector max{std::numeric_limits<float>::min()};
        AABB build();
};
#endif