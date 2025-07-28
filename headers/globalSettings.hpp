#ifndef GLOBALSETTINGS
#define GLOBALSETTINGS

//Rendering Settings
#define MULTITHREADING 1  // 0: no multithreading; 1: multithreading on
#define DEBUGLEVEL 0 //0: no debug view aka normal rendering
#define SAMPLESPERPIXEL 1
#define CAMERAJITTER 0
#define MAXPATHDEPTH 5
#define MOVABLEOBJECTS 1

//Settings for Acceleration structures
#define ACCELERATION 1 //
#define TRIPERLEAF 2
#define MAXTREEDEPTH 15

//Camera Settings
#define FOV 90

//Global illumination Settings
#define GLOBALILLUMINATION 0
#define DIFFUSEREFLECTIONSCOUNT 1
#endif