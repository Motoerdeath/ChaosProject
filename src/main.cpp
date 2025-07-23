#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"


#include <cassert>
#include <cstdio>

#include <mutex>
#include <queue>
#include <stack>
#include <string>
#include <thread>
#include "../headers/scene.hpp"
#include "../headers/renderer.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include "../headers/bucket.hpp"
#include "../headers/accelerationStructure.hpp"


#define MULTITHREADING 0;
#define DEBUGLEVEL 0;
std::vector<int> a{0,1,2,3,4,5,6,7,8,9};


int main() {

    
const std::string filename = "../inputs/Homework11_Shading3/scene1.crtscene";

CRTScene scene(filename);

std::printf("Begin importing scene.\n");
scene.parseSceneFile(filename);
CRTRenderer renderer(&scene);
renderer.setupTriangleAccessStructure();
std::printf("finished importing scene.\n");
std::printf("Begin rendering scene.\n");
renderer.render();
std::printf("finished rendering scene.\n");
renderer.storeImage("../output.ppm");
std::printf("finished storing output.\n");




  return 0;
}