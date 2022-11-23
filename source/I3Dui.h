#pragma once



class I3Deditor;
class EditorWindow;

class I3Dui {
  I3Deditor    *_i3d;
  EditorWindow *_win;
  Ix           *_ix;
public:
  
  ixWSstyle winStyle;

  
  ixWindow *w1, *w2;  // w1[input/left] w2[output/right/3do]  left and right windows

  vec4 col, colHover, colBRD, colFocus, colBRDfocus;

  // input panel window
  struct {
    ixButton *butLoad;

    void *nMeshes, *nMats, *nTex, *nAnims, *nLights, *nCameras; // print text handles

    ixDropList *lMesh, *lMats;
    ixButton *b1visualize, *b2visualize;  // left/right buttons to visualize currently selected mesh

    void *meshVert, *meshNorm, *meshTexCoords, *meshMat;

    //int32 meshLastSel, matLastSel;

    void *matDiffuse, *matAmbient, *matSpecular, *matEmissive, *matTransparent, *matReflective;
    ixButton *matButDiffuse, *matButAmbient, *matButSpecular, *matButEmissive, *matButTransparent, *matButReflective;
  } i;

  // output panel window
  struct {
    void *fileName;
    void *fileIndex;

    void *inputFileName;
    void *name;

    void *flagsKeepHostData;
    ixDropList *dataType; //void *dataType;
    ixDropList *affinity; //void *affinity;
    void *nrVert;
    void *dataSize;
    
    ixButton *load;
    ixButton *update;
    ixButton *save;
    ixButton *saveAs;
    ixButton *importFromCurrent;
    ixButton *visualize;

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

  void updateI3OfromControls();

  //void updateControlsNull();

  // constr / destr

  I3Dui(I3Deditor *in_parent, EditorWindow *in_win);
  ~I3Dui();
  void delData();

private:



};


