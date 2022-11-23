#pragma once


class I3Dui;
class I3Dinput;
class I3Doutput;

class I3Deditor {
public:

  EditorWindow *win;        // parent window
  Ix *ix;                   // ix engine of the parent window


  // I3D editor 
  I3Dui ui;                 // [I3Dui.cpp] user interface
  I3Dinput input;           // [I3Dinput.cpp] input panel
  I3Doutput output;         // [I3Doutput.cpp] output panel


  void init();
  void update();
  void draw();

  //vec3 camPos;            // camera position in world
  //vec3 camFront;          // normalized front pointing vector

  vec3 defCamPos;       // maybe
  vec3 defAngle;        // maybe

  //vec3 angle;             // this won't do i think

  /*
  // to be deleted - old method with angles and distance
  float dist, defDist;
  vec3 angle, defAngle;
  */

  vec4 backgroundColor;

  vec3 sunPos, sunColor;
  float sunAmbientStr;
  float sunSpecularStr;


  


  void computePerspective();

  // constr / destr

  I3Deditor(EditorWindow *in_win);
  ~I3Deditor();
  void delData();




private:

  
  struct PConsts {
    mat4 model;
    alignas(4) int32 map0, map1, map2, map3;
    alignas(4) ixFlags32 flags;
  } push;

  //VkoShader *sl;
  ixShader *sl;

  VkoCommandBuffer *cmd[2];
  VkoCommandPool *pool[2];

};










