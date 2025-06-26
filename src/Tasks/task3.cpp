#include "../headers/task.hpp"
#include "../headers/ppmImage.hpp"
#include <time.h>
#include <random>
#include <string>
#include <fstream>
class Task3 : public Task {
    public:
        Task3() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    std::string resultFilePath = "../outputs/04_Triangle/output.txt";

    void setup() override {
    }
    void run() override {

    std::ofstream ostream;
    ostream.open(resultFilePath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
        /*
        Exercise 1
        a)Calculate the cross product (AxB) between two vectors:
         A = (3.5, 0, 0) and B = (1.75, 3.5, 0)
        b)Calculate the cross product (AxB) between two vectors:
         A = (3, -3, 1) and B = (4, 9, 3)
        c)Calculate the area of the parallelogram formed by vectors:
         A = (3, -3, 1) and B = (4, 9, 3)
        d)Calculate the area of the parallelogram formed by vectors:
         A = (3, -3, 1) and B = (-12, 12, -4)
        */

        //a)
        CRTVector a(3.5,0.f,0.f);
        CRTVector b(1.75f,3.5f,0.f);
        CRTVector resultA = a.cross(b);
        ostream << "a): (" << resultA.x << ";" << resultA.y << ";" << resultA.z << ")\n";

        //b)
        a = CRTVector(3.f,-3.f,1.f);
        b = CRTVector(4.f,9.f,3.f);
        CRTVector resultB = a.cross(b);
        ostream << "b): (" << resultB.x << ";" << resultB.y << ";" << resultB.z << ")\n";
        //c)
        a = CRTVector(3.f,-3.f,1.f);
        b = CRTVector(4.f,9.f,3.f);
        float resultC = resultB.length();
        ostream << "c): (" << resultC << ")\n";
        //d)
        a = CRTVector(3.f,-3.f,1.f);
        b = CRTVector(-12.f,12.f,-4.f);
        float resultD = a.cross(b).length();
        ostream << "d): (" << resultD << ")\n";
    /*
        Exercise2:
        e)Find the normal vector for a triangle with the following vertices:
         A = (-1.75, -1.75, -3)
         B = (1.75, -1.75, -3)
         C = (0, 1.75, -3)
        f)Find the normal vector for a triangle with the following vertices:
         A = (0, 0, -1)
         B = (1, 0, 1)
         C = (-1, 0, 1)
        g)Find the normal vector for a triangle with the following vertices:
         A = (0.56, 1.11, 1.23)
         B = (0.44, -2.368, -0.54)
         C = (-1.56, 0.15, -1.92)
        h)Calculate the areas of these triangles.
    */

    //e)
        a = CRTVector(-1.75f,-1.75f,-3.f);
        b = CRTVector(1.75f,-1.75f,-3.f);
        CRTVector c(0.f,1.75f,-3.f);
        CRTVector ab= b-a;
        CRTVector ac= c-a;
        CRTVector resultE = ab.cross(ac);
        resultE = resultE.normalize();
        ostream << "e): (" << resultE.x << ";" << resultE.y << ";" << resultE.z << ")\n";
        float areaE = ab.length() * ac.length()/2.f;
    //f)
        a = CRTVector(0.f,0.f,-1.f);
        b = CRTVector(1.f,0.f,1.f);
        c = CRTVector(-1.f,0.f,1.f);
        ab= b-a;
        ac= c-a;
        CRTVector resultF = ab.cross(ac);
        resultF = resultF.normalize();
        ostream << "f): (" << resultF.x << ";" << resultF.y << ";" << resultF.z << ")\n";
        float areaF = ab.length() * ac.length()/2.f;
    //g)
        a = CRTVector(0.56f,1.11f,1.23f);
        b = CRTVector(.44f,-2.368f,-0.54f);
        c = CRTVector(-1.56f,0.15f,-1.92f);
        ab= b-a;
        ac= c-a;
        CRTVector resultG = ab.cross(ac);
        resultG = resultG.normalize();
        ostream << "g): (" << resultG.x << ";" << resultG.y << ";" << resultG.z << ")\n";
        float areaG = ab.length() * ac.length()/2.f;

    //h)
    ostream << "h): (" << areaE << ";" << areaF << ";" << areaG << ")\n";

    ostream.close();
    }
    void cleanup() override {
    }
};