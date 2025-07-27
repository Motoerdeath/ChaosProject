#ifndef GLOBALSETTINGS
#define GLOBALSETTINGS

//Rendering Settings
#define MULTITHREADING 1  // 0: no multithreading; 1: multithreading on
#define DEBUGLEVEL 0 //0: no debug view aka normal rendering
#define SAMPLESPERPIXEL 2056
#define CAMERAJITTER 1
#define MAXPATHDEPTH 5
#define MOVABLEOBJECTS 0

//Settings for Acceleration structures
#define ACCELERATION 1 //
#define TRIPERLEAF 2
#define MAXTREEDEPTH 15

//Camera Settings
#define FOV 90

//Global illumination Settings
#define GLOBALILLUMINATION 1
#define DIFFUSEREFLECTIONSCOUNT 1
#define ENVIRONMENTMAP 0
#endif