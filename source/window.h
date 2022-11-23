#pragma once


class EditorWindow {
  
public:

  Ix ix;
  osiWindow *osiWin;

  I3Deditor i3d;
  TEXeditor tex;

  inline static uint &nWindows()   { static uint _nWindows= 0;       return _nWindows; }   // number of osiWindows created
  inline static bool &wsysUpdate() { static bool _wsysUpdate= false; return _wsysUpdate; } // holds ret val of wsys.update()

  // main objects that can be used with this window

  

  // main funcs

  bool init();
  void update();
  void draw();

  // constr / destr

  EditorWindow();           // first window created will be the main window, always
  ~EditorWindow();
  void delData();
private:
  
  bool isMain;              // this is the main window of ixEditor;
  
};





