#pragma once


class Viewer {
public:

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

  Viewer();
  ~Viewer();
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


extern Viewer viewer;










