#pragma once

class UI {
public:
  void *fnt1_10, *fnt1_12, *fnt1_15, *fnt1_20;      // main things, titles
  void *fnt2_12, *fnt2_13, *fnt2_14, *fnt2_15;      // large bodies of text, mass of text
  void *fnt2_16, *fnt2_17, *fnt2_18, *fnt2_20;      // large bodies of text, mass of text
  
  ixWSstyle winStyle;

  ixFontStyle fntStyle1, fntStyle2;
  ixWindow *w1, *w2;  // w1[input/left] w2[output/right/3do]  left and right windows

  vec4 col, colHover, colBRD, colFocus, colBRDfocus;

  // input window
  struct {
    ixButton *butLoad;

    void *nMeshes, *nMats, *nTex, *nAnims, *nLights, *nCameras; // print text handles

    ixDropList *lMesh, *lMats;
    ixButton *b1visualize, *b2visualize;  // left/right buttons to visualize currently selected mesh

    void *meshVert, *meshNorm, *meshTexCoords, *meshMat;

    int32 meshLastSel, matLastSel;

    void *matDiffuse, *matAmbient, *matSpecular, *matEmissive, *matTransparent, *matReflective;
    ixButton *matButDiffuse, *matButAmbient, *matButSpecular, *matButEmissive, *matButTransparent, *matButReflective;
  } i;

  // output window
  struct {
    void *fileName;
    void *fileIndex;

    void *inputFileName;
    void *inputMeshName;

    void *flagsKeepHostData;
    void *dataType;
    void *affinity;
    void *nrVert;
    void *dataSize;
      
  } o;



  // funcs

  //rgba col, colHover, colBRD, colFocus, colBRDfocus;
  
  void init();
  void update();
  void draw();

  void updateControls();
  void updateInputMeshDetails();
  void updateInputMatDetails();

  void updateOutputMeshDetails();

  //void updateControlsNull();

  // constr / destr

  UI();
  ~UI();
  void delData();

private:



};



extern UI ui;










