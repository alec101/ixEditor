#include "ix3do.h"







Input::Input() {
  scene= null;
}


Input::~Input() {

}


void Input::delData() {

}



bool Input::loadFile() {
  
  // open file name dialog
  OPENFILENAME ofn;
  char fname[MAX_PATH]= "";

  ZeroMemory(&ofn, sizeof(ofn));

  /*
  'blend files are a memory dump of blender' so it's a chaos to even try to read them... can't fully blame assimp
    im guessing every new iteration of blender would require a new loader for .blend files for assimp
    so reading directly from .blend files is out the picture (at least for now)
    so ... 3ds? any other format... those huge dao or whatever? any are good
    im guessing the fully documented, exact file types won't have issues
    still, the best would've been to just... update directly from blend files... that would be so sweet, but that ain't happening

    mkay, probly dae (collada) , and i must test with that, see how i can see the whole scene
    ;
    */


  ofn.lStructSize=  sizeof(ofn); // SEE NOTE BELOW
  ofn.hwndOwner=    (HWND)ix.win->_hWnd;
  ofn.lpstrFilter=  "Collada Files (*.dae)\0*.dae\0Blender Files (*.blend)\0*.blend\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile=    fname;
  ofn.nMaxFile=     MAX_PATH;
  ofn.Flags=        OFN_EXPLORER| OFN_FILEMUSTEXIST| OFN_HIDEREADONLY;
  ofn.lpstrDefExt=  "dae";

  if(GetOpenFileName(&ofn)) {
    // Do something usefull with the filename stored in szFileName 
    ix.console().printf("FILE OPEN: [%s]", fname);

    scene= aiImp.ReadFile(fname, 0);
    if(scene== null) {
      ix.console().printf("assimp ReadFile error: [%s]", aiImp.GetErrorString());
      return false;
    }

    if(scene->mFlags& AI_SCENE_FLAGS_INCOMPLETE) {
      ix.console().print("assimp scene is incomplete");
      return false;
    }



















    return true;
  }
  else return false;
}















