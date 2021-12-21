#pragma once






class Input {
public:

  int32 meshSel, matSel;
  str8 fileName;

  Assimp::Importer aiImp;
  const aiScene *scene;

  bool loadFile();
  bool loadFileName(cchar *in_file, bool useIxConsole= true);

  void update(); // updates contents after a load? might not be useful



  // constructor / destructor

  Input();
  ~Input();
  void delData();


private:

};



extern Input input;













