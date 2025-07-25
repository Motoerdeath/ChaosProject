#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"
#include "../src/Tasks/homework12.cpp"


#include <cassert>
#include <cstdio>

#include <string>
#include "../headers/scene.hpp"
#include "../headers/renderer.hpp"
#include <iostream>
#include <chrono>




int main() {


  //Homework12  homework12;
  //homework12.execute();
      
  const std::string filename = "../inputs/Homework6_Camera/scene1.crtscene";

  CRTScene scene(filename);
  std::printf("Begin importing scene.\n");
  scene.parseSceneFile(filename);
  CRTVector truck(1.f,0.f,0.f);
  CRTVector dolly(0.f,0.f,1.f);
  CRTVector pedestal(0.f,1.f,0.f);
  //scene.sceneCamera.move(pedestal);
  scene.sceneCamera.tilt(20);
  CRTRenderer renderer(&scene);
  renderer.setupTriangleAccessStructure();
  std::printf("finished importing scene.\n");
  std::printf("Begin rendering scene.\n");
  auto start = std::chrono::high_resolution_clock::now();
  
  renderer.render();
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::microseconds dur = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
  const double seconds = dur.count()/1'000'000.0;
  std::cout<< "Execution time: "<<seconds <<" seconds." <<std::endl;
  std::printf("finished rendering scene.\n");
  renderer.storeImage("../output.ppm");
  std::printf("finished storing output.\n");




  return 0;
}