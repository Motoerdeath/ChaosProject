#include <cassert>
#include <cstdio>

#include <string>
#include "../headers/scene.hpp"
#include "../headers/renderer.hpp"
#include <iostream>
#include <chrono>

int main() {

//set scenefile that you want to render
const std::string filename = "../inputs/Homework11_Shading3/scene1.crtscene";

  CRTScene scene(filename);
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
 

/*
  const std::string inputFilePath = "../inputs/Homework15_Conclusion/scene0.crtscene";
  const std::string outputFilePath = "../outputs/15_Conclusion/Scene1/Frame_";

  int nFrames = 300;
  CRTScene scene(inputFilePath);
  scene.parseSceneFile(inputFilePath);
  CRTRenderer renderer(&scene);
  CRTVector truck{0.2f,0.f,0.f};
  CRTVector center{0.f,0.f,-3.f};
  scene.fullObjects[1].offset =scene.fullObjects[1].offset+ CRTVector(-1.f,0.f,0.f);
  for(int i = 0; i < nFrames; i++) {
    renderer.setupTriangleAccessStructure();
    renderer.render();
    std::string outputPath = outputFilePath + std::to_string(i) + ".ppm";
    renderer.storeImage(outputPath);

    if(i >= 270) {
      renderer.debug = HeatMap;
      renderer.useAccelerationStructure = true;
      renderer.useMultiThreading = true;
    }else if(i >= 240) {
      renderer.debug = HeatMap;
      renderer.useAccelerationStructure = true;
      renderer.useMultiThreading = false;
    }else if(i >= 210) {
      renderer.debug = HeatMap;
      renderer.useAccelerationStructure = false;
      renderer.useMultiThreading = true;
    }else if(i >= 180) {
      renderer.debug = HeatMap;
      renderer.useAccelerationStructure = false;
      renderer.useMultiThreading = false;
    } else if( i >= 150) {
      renderer.debug = TriangleView;
    } else if( i >= 120) {
      renderer.debug = BarycentricCoordinates;
    } else if( i >= 90) {
      renderer.debug = ShadingNormals;
    } else if( i >= 60) {
      renderer.debug = GeometricNormals;
    }
    

    if(i % 20 <10) {
      scene.fullObjects[1].offset =scene.fullObjects[1].offset+ truck;
    } else {
      scene.fullObjects[1].offset =scene.fullObjects[1].offset- truck;
    }
    //update Ball position
    // 

  }
*/

  return 0;
}