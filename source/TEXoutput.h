#pragma once


class TEXeditor;
class EditorWindow;


class TEXoutput {
  TEXeditor    *_tex;
  EditorWindow *_win;
  Ix           *_ix;
public:

  //ixMesh outMesh;

  // the material that will be used for the rendering
  // there should be only one, i don't see a need for a list
  //ixMaterial *mat;

  // funcs

  //bool inportMesh_n(ixMesh *out_m, const aiScene *in_scene, uint32 in_nr);
  //bool inportMesh_name(ixMesh *out_m, const aiScene *in_scene, const char *in_name);
  //bool inportMat_n(ixMesh *out_m, const aiScene *in_scene, uint32 in_nr);
  
  void init();

  bool save();
  bool saveAs();

  // constructor / destructor

  TEXoutput(TEXeditor *in_parent, EditorWindow *in_win);
  ~TEXoutput();
  void delData();
private:
  void _computeRelativePath();
  bool _inportMesh(ixMesh *out_m, const aiScene *in_Scene, uint32 in_nr);
};
