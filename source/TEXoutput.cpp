#include "ixEditor.h"


TEXoutput::TEXoutput(TEXeditor *in_parent, EditorWindow *in_win): _tex(in_parent), _win(in_win) {
  _ix= &in_win->ix;
}

TEXoutput::~TEXoutput() {
}

void TEXoutput::delData() {
}






void TEXoutput::init() {

}



bool TEXoutput::saveAs() {
  const int bufferSize= 512;
  char fname[bufferSize]= "";

  OPENFILENAME ofn;                 // open file name dialog
  ZeroMemory(&ofn, sizeof(ofn));
  return false;

  /*
  ofn.lStructSize=  sizeof(ofn);
  ofn.hwndOwner=    (HWND)_ix->win->_hWnd;
  ofn.lpstrFilter=  "Ix TEX Files (*.i3d)\0*.i3d\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile=    fname;
  ofn.nMaxFile=     bufferSize;
  ofn.Flags=        OFN_EXPLORER| OFN_FILEMUSTEXIST| OFN_HIDEREADONLY| OFN_NOCHANGEDIR;
  ofn.lpstrDefExt=  "i3d";

  if(GetSaveFileName(&ofn)) {
    std::filesystem::path base(std::filesystem::current_path());
    std::filesystem::path fsPath(fname);
    std::filesystem::path relative(std::filesystem::relative(fsPath, base));
    //the dae filepath must be updated to be relative
    //must check if you can actually save something
    //  but things are working, i got all i need, relative paths and separators are good, everything.;
    //so i should finish this week, if all goes well;

    str8 relPath((char *)relative.generic_u8string().c_str());
    ixUtil::changePathSeparator(&relPath);

    _ix->console().printf("FILE SAVE: [%s]", relPath.d);

    return _ix->res.mesh.save(relPath.d, &outMesh);

  }
  return false;
  */
  
}



bool TEXoutput::save() {
  return false;
  //if(outMesh.fileName.len<= 1)
  //  return saveAs();

  //return _ix->res.mesh.save(outMesh.fileName, &outMesh);
  
}








