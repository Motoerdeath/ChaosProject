#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"


#include <cassert>
#include <cstdio>

#include <string>
#include "../headers/scene.hpp"
#include "../headers/renderer.hpp"


bool FUNDAMENTALS = true;
bool RAYS = true;



int main() {
  /*
    Homework2 homework2;
    homework2.execute();

    Homework3 homework3;
    homework3.execute();

    Homework4 homework4;
    homework4.execute();

    Homework5 homework5;
    homework5.execute();
 */

    
const std::string filename = "../inputs/Homework11_Shading3/scene0.crtscene";



CRTScene scene(filename);
CRTRenderer renderer(&scene);
std::printf("Begin importing scene.\n");
scene.parseSceneFile(filename);
std::printf("finished importing scene.\n");
std::printf("Begin rendering scene.\n");
renderer.render();
std::printf("finished rendering scene.\n");
renderer.storeImage("../output.ppm");
std::printf("finished storing output.\n");


    return 0;
}