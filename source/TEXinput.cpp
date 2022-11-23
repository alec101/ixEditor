#include "ixEditor.h"





TEXinput::TEXinput(TEXeditor *in_parent, EditorWindow *in_win): _tex(in_parent), _win(in_win) {
  _ix= &in_win->ix;

  //scene= null;
  //meshSel= matSel= 0;
}


TEXinput::~TEXinput() {

}


void TEXinput::delData() {

}






void TEXinput::update() {
  error.makeme(__FUNCTION__);
}


bool TEXinput::loadFile() {
  return false;
  const int bufferSize= 512;
  char fname[bufferSize]= "";
  
  OPENFILENAME ofn;                 // open file name dialog
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

  /*  SCREW PREFF SEPARATOR, USE THAT replace FUNC FROM STD
  std::filesystem::path base("/is/the/speed/of/light/absolute");
  std::filesystem::path p("/is/the/speed/of/light/absolute/or/is/it/relative/to/the/observer");
  std::filesystem::path p2("/little/light/races/in/orbit/of/a/rogue/planet");
  std::cout << "Base is base: " << std::filesystem::relative(p, base).generic_string() << '\n'
            << "Base is deeper: " << std::filesystem::relative(base, p).generic_string() << '\n'
            << "Base is orthogonal: " << std::filesystem::relative(p2, base).generic_string();
  */

  /*
  matSel= meshSel= 0;

  ofn.lStructSize=  sizeof(ofn); // SEE NOTE BELOW
  ofn.hwndOwner=    (HWND)_ix->win->_hWnd;
  ofn.lpstrFilter=  "Collada Files (*.dae)\0*.dae\0Blender Files (*.blend)\0*.blend\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile=    fname;
  ofn.nMaxFile=     bufferSize;
  ofn.Flags=        OFN_EXPLORER| OFN_FILEMUSTEXIST| OFN_HIDEREADONLY| OFN_NOCHANGEDIR;
  ofn.lpstrDefExt=  "dae";

  if(GetOpenFileName(&ofn)) {
    
    std::filesystem::path base(std::filesystem::current_path());
    std::filesystem::path fsPath(fname);
    std::filesystem::path relative(std::filesystem::relative(fsPath, base));

    str8 relPath((char *)relative.generic_u8string().c_str());
    ixUtil::changePathSeparator(&relPath);

    if(loadFileName(relPath.d)) {
      //fileName= (char *)fsPath.filename().generic_u8string().c_str();
      return true;
    } else
      return false;
  }
  else return false;
  */
}



bool TEXinput::loadFileName(cchar *in_name, bool useIxConsole) {
  return false;
  /*
  // Do something useful with the filename stored in szFileName 
  if(useIxConsole)
    _ix->console().printf("FILE OPEN: [%s]", in_name);
  else
    printf("FILE OPEN: [%s]\n", in_name);

  scene= aiImp.ReadFile(in_name, 0);
  if(scene== null) {
    if(useIxConsole)
      _ix->console().printf("assimp ReadFile error: [%s]", aiImp.GetErrorString());
    else
      printf("assimp ReadFile error: [%s]\n", aiImp.GetErrorString());
    return false;
  }

  if(scene->mFlags& AI_SCENE_FLAGS_INCOMPLETE) {
    if(useIxConsole)
      _ix->console().print("assimp scene is incomplete");
    else
      printf("assimp scene is incomplete\n");
    return false;
  }

  std::filesystem::path fsPath(in_name);
  fileName= (char *)fsPath.filename().generic_u8string().c_str();

  return true;
  */
}











