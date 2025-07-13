#ifndef CRTTEXTURE
#define CRTTEXTURE

#include <string>

enum TextureType {
    albedoTexture,
    Bitmap
};
class Texture {
    public:
    private:
        std::string name;
        TextureType type;
};

class BitMapTexture : public Texture {

};

class AlbedoTexture : public Texture {

};

class ProceduralTexture : public Texture {

};

#endif