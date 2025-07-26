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
#include "../headers/environmentMap.hpp"

int main() {


   /*  
  const std::string filename = "../inputs/Homework15_Conclusion/scene2.crtscene";

  CRTScene scene(filename);
  std::printf("Begin importing scene.\n");
  scene.parseSceneFile(filename);
  CRTVector center{0.f,0.f,-3.f};
  CRTVector dolly{0.f,0.f,-1.f};
  //scene.sceneCamera.move(dolly);
  //scene.sceneCamera.rotateCameraHorizontalAroundPoint(center, -119);
  //scene.sceneCamera.lookAt(center);
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
  
*/

  const std::string inputFilePath = "../inputs/Homework15_Conclusion/scene2.crtscene";
  const std::string outputFilePath = "../outputs/15_Conclusion/Scene3/Frame_";

  int nFrames = 240;
  CRTScene scene(inputFilePath);
  scene.parseSceneFile(inputFilePath);
  CRTRenderer renderer(&scene);
  CRTVector center{0.f,0.f,-3.f};
  for(int i = 0; i < nFrames; i++) {
    renderer.setupTriangleAccessStructure();
    renderer.render();
    std::string outputPath = outputFilePath + std::to_string(i) + ".ppm";
    renderer.storeImage(outputPath);

    //update Scene
    CRTVector truck(0.2f,0.f,0.f);
    scene.sceneCamera.move(truck);
    scene.sceneCamera.lookAt(center);


    
    
  }
  

  return 0;
}