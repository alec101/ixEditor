#pragma once




class I3Deditor;
class EditorWindow;


class I3Dinput {
  I3Deditor    *_i3d;
  EditorWindow *_win;
  Ix           *_ix;

public:

  int32 meshSel, matSel;
  str8 fileName;

  Assimp::Importer aiImp;
  const aiScene *scene;

  bool loadFile();
  bool loadFileName(cchar *in_file, bool useIxConsole= true);

  void update(); // updates contents after a load? might not be useful



  // constructor / destructor

  I3Dinput(I3Deditor *in_parent, EditorWindow *in_win);
  ~I3Dinput();
  void delData();


private:

};








