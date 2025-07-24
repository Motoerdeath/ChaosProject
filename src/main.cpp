#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"
#include "../src/Tasks/homework12.cpp"


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


int main() {


  //Homework12  homework12;
  //homework12.execute();
      
  const std::string filename = "../inputs/Homework9_Shading1/scene3.crtscene";

  CRTScene scene(filename);

  std::cout << glm::tan(45*M_PI/180) << std::endl;
  std::printf("Begin importing scene.\n");
  scene.parseSceneFile(filename);
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