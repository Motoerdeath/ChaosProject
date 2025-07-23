#include "../headers/homework.hpp"
#include "../headers/ppmImage.hpp"
#include <time.h>
#include <random>
#include <string>
class Homework7 : public Homework {
    public:
        Homework7() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    std::string resultFilePath = "../outputs/07_Scene/output.ppm";

    void setup() override {

    }
    void run() override {
    }
    void cleanup() override {
    }
};