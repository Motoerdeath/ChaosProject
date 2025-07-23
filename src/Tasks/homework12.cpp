#include "../headers/homework.hpp"
#include "../headers/ppmImage.hpp"
#include <time.h>
#include <random>
#include <string>
#include "../headers/scene.hpp"
#include "../headers/renderer.hpp"
class Homework12 : public Homework {
    public:
        Homework12() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    std::string inputFolderPath = "../inputs/Homework12_Textures/";
    std::string resultFolderPath = "../outputs/12_Textures/";
    std::vector<std::string> inputs = {"scene0.crtscene",
                                        "scene1.crtscene",
                                        "scene2.crtscene",
                                        "scene3.crtscene",
                                        "scene4.crtscene"};
    //std::vector<std::string> inputs ) 

    void setup() override {

    }
    void run() override {
        for( int i = 0; i < inputs.size();i++) {
            std::string input = inputs[i];
            std::string fullInput = inputFolderPath + input;
            std::printf(fullInput.c_str());
            
            CRTScene scene(fullInput);
            
            std::printf("Begin importing scene.\n");
            
            scene.parseSceneFile(fullInput);
            CRTRenderer renderer(&scene);
            renderer.setupTriangleAccessStructure();
            std::printf("finished importing scene.\n");
            /*
            std::printf("Begin rendering scene.\n");
            renderer.render();
            std::printf("finished rendering scene.\n");
            renderer.storeImage(resultFolderPath + input + ".ppm");
            std::printf("finished storing output.\n");
            */
        }
    }
    void cleanup() override {
    }
};