#ifndef ENVIRONMENTMAP
#define ENVIRONMENTMAP


#include "crtVector.hpp"
#include "../headers/ray.hpp"
#include "texture.hpp"
class EnvironmentMap {
    public:
        EnvironmentMap() {}
        void buildEnvironmentMap(std::vector<std::string> filepaths);
        CRTVector sampleEnvironmentMap(const CRTRay& ray);

    private:
    Texture cubeMap[6]; //order: right-left-top-bottom-back-front
};
#endif