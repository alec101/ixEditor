#pragma once




class TEXeditor;
class EditorWindow;


class TEXinput {
  TEXeditor    *_tex;
  EditorWindow *_win;
  Ix           *_ix;

public:

  //int32 meshSel, matSel;
  //str8 fileName;

  //Assimp::Importer aiImp;
  //const aiScene *scene;

  bool loadFile();
  bool loadFileName(cchar *in_file, bool useIxConsole= true);

  void update(); // updates contents after a load? might not be useful

  // constructor / destructor

  TEXinput(TEXeditor *in_parent, EditorWindow *in_win);
  ~TEXinput();
  void delData();


private:

};








