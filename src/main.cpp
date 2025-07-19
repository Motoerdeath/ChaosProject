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

std::mutex bucketMutex;
std::vector<int> a{0,1,2,3,4,5,6,7,8,9};


void func2(int threadIndex,std::queue<Bucket>* buckets, CRTRenderer* renderer) {
  //
  while(true) {
    bucketMutex.lock();
    if(buckets->size() > 0) {
      Bucket temp = buckets->front();
      buckets->pop();
      //std::cout << "Thread " << threadIndex<<" acquired lock on value " << temp.width<<std::endl;
      bucketMutex.unlock();
      //std::this_thread::sleep_for(std::chrono::milliseconds(100));
      renderer->renderRegion(temp.startX, temp.startY, temp.width, temp.height);
      //std::cout << "Bucket " << temp.bucketIDx<<" finished!"<<std::endl;
    } else {
      bucketMutex.unlock();
      return;
    }

  }

  //

  return;
}

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

    
const std::string filename = "../inputs/Homework11_Shading3/scene1.crtscene";

CRTScene scene(filename);
CRTRenderer renderer(&scene);
std::printf("Begin importing scene.\n");
scene.parseSceneFile(filename);

AccelerationStructure as;
as.createTriangleSoup(scene.sceneObjects);
as.buildAS();

std::printf("finished importing scene.\n");
std::printf("Begin rendering scene.\n");
//renderer.render();
std::printf("finished rendering scene.\n");
//renderer.storeImage("../output.ppm");
std::printf("finished storing output.\n");


  BucketQueue queue;
  queue.generateBucketQueue(1920, 1080, 24);

const auto nThreads = std::thread::hardware_concurrency();

std::cout << nThreads << std::endl;;
  std::vector<std::thread> threads;

  for(int i = 0; i < nThreads;i++) {
    //threads.push_back(std::thread(&func2,i,&(queue.buckets),&renderer));
  }
    

  for(std::thread& t : threads) {
    //t.join();
  }
renderer.storeImage("../output.ppm");



  return 0;
}