#pragma once



class TEXeditor;
class EditorWindow;

class TEXui {
  TEXeditor    *_tex;
  EditorWindow *_win;
  Ix           *_ix;
public:
  // FONTS MUST BE LOADED IN A CENTRAL LOCATION
  void *fnt1_10, *fnt1_12, *fnt1_15, *fnt1_20;      // main things, titles
  void *fnt2_12, *fnt2_13, *fnt2_14, *fnt2_15;      // large bodies of text, mass of text
  void *fnt2_16, *fnt2_17, *fnt2_18, *fnt2_20;      // large bodies of text, mass of text
  
  ixWSstyle winStyle;

  ixFontStyle fntStyle1, fntStyle2;
  ixWindow *w1, *w2;  // w1[input/left] w2[output/right/3do]  left and right windows

  vec4 col, colHover, colBRD, colFocus, colBRDfocus;

  // input panel window
  struct {
    // MUST BE DONE

    /* I3D ui input
    ixButton *butLoad;

    void *nMeshes, *nMats, *nTex, *nAnims, *nLights, *nCameras; // print text handles

    ixDropList *lMesh, *lMats;
    ixButton *b1visualize, *b2visualize;  // left/right buttons to visualize currently selected mesh

    void *meshVert, *meshNorm, *meshTexCoords, *meshMat;

    //int32 meshLastSel, matLastSel;

    void *matDiffuse, *matAmbient, *matSpecular, *matEmissive, *matTransparent, *matReflective;
    ixButton *matButDiffuse, *matButAmbient, *matButSpecular, *matButEmissive, *matButTransparent, *matButReflective;
    */
  } i;

  // output panel window
  struct {

    // TEX output ui must be done

    /* I3D one
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
    */
  } o;



  // funcs

  //rgba col, colHover, colBRD, colFocus, colBRDfocus;
  
  void init();
  void update();
  void draw();

  void updateControls();
  //void updateInputMeshDetails();
  //void updateInputMatDetails();

  //void updateOutputMeshDetails();

  //void updateI3OfromControls();

  //void updateControlsNull();

  // constr / destr

  TEXui(TEXeditor *in_parent, EditorWindow *in_win);
  ~TEXui();
  void delData();

private:



};


