#pragma once






class Input {
public:

  Assimp::Importer aiImp;
  const aiScene *scene;

  bool loadFile();

  void update(); // updates contents after a load? might not be useful



  // constructor / destructor

  Input();
  ~Input();
  void delData();


private:

};



extern Input input;













