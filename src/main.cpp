#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"

#include "../headers/triangle.hpp"
#include "../headers/camera.hpp"
#include "../headers/settings.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cassert>
#include <cfloat>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include "../headers/matrix.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include <iostream>
//#include "../src/jsonUtilities.cpp"
#include "../headers/mesh.hpp"
#include "../headers/scene.hpp"
#include "glm/ext/matrix_transform.hpp"
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

    
const std::string filename = "../inputs/Homework11_Shading3/scene5.crtscene";



CRTScene scene(filename);
CRTRenderer renderer(&scene);
std::printf("Begin importing scene.\n");
scene.parseSceneFile(filename);
std::printf("finished importing scene.\n");
std::printf("Begin rendering scene.\n");
renderer.render();
//scene.render();
std::printf("finished rendering scene.\n");
renderer.storeImage("../output.ppm");
std::printf("finished storing output.\n");


    return 0;
}