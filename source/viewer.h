#pragma once


class Viewer {
public:


  



  void init();
  void update();
  void draw();



  // constr / destr

  Viewer();
  ~Viewer();
  void delData();




private:

  
  struct PConsts {
    //int32 blockID;                    // terrain block index
    //int view;                         // one of the 4 possible camera positions
    //float x, y, z;                    // position
    int32 map0, map1, map2, map3;
    ixFlags32 flags;
  } push;
  //VkoShader *sl;
  ixShader *sl;

  VkoCommandBuffer *cmd[2];
  VkoCommandPool *pool[2];

};


extern Viewer viewer;










