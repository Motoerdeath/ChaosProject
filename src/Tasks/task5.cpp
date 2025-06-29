#include "../headers/task.hpp"
#include "../headers/ppmImage.hpp"
#include <time.h>
#include <random>
#include <string>
class Task1 : public Task {
    public:
        Task1() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    std::string resultFilePath = "../outputs/02_Fundamentals/output.ppm";

    void setup() override {

    }
    void run() override {
    }
    void cleanup() override {
    }
};