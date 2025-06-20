#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"


struct CRTRay {
    glm::vec3 rayOrigin;
    glm::vec3 rayDirection;
};


class CRTImage {
    
    public:
    CRTImage(int width, int height);
    int width;
    int height;
    float aspectRatio;

    private:
};