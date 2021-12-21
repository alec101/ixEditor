#include "ix3do.h"


Output::Output(): outMesh(&ix) {

}



Output::~Output() {
  //mat->delData();
}



void Output::delData() {
}



void Output::init() {

  mat= ix.res.mat.add();

}


bool Output::_inportMesh(ixMesh *out_m, const aiScene *in_scene, uint32 in_nr) {
  //input.meshSel; input.matSel;
  //input.scene->mMeshes[input.meshSel];
  aiMesh *iMesh= in_scene->mMeshes[in_nr];    /// shortcut

  ix.vk.DeviceWaitIdle(ix.vk.device);

  
  if(out_m->data) delete[] out_m->data;
  if(out_m->buf) out_m->unload();

  /// mesh name
  if(iMesh->mName.length) {
    int32 size= iMesh->mName.length< 64? iMesh->mName.length: 63;
    int32 a;
    for(a= 0; a< size; a++)
      out_m->name[a]= iMesh->mName.data[a];
    out_m->name[a]= 0; // str terminator

  } else
    out_m->name[0]= 0;
  

  out_m->nrVert= iMesh->mNumVertices;

  out_m->fileInputName= input.fileName;
  

  if(out_m->dataType== 0) {
    out_m->size= ixMesh::Data0::size()* out_m->nrVert;
    out_m->data= new uint8[out_m->size];
    ixMesh::Data0 d(out_m);

    for(uint32 a= 0; a< out_m->nrVert; a++) {
      d.pos[a].x= iMesh->mVertices[a].x,
      d.pos[a].y= iMesh->mVertices[a].y,
      d.pos[a].z= iMesh->mVertices[a].z;

      if(iMesh->mNormals)
        d.nrm[a].x= iMesh->mNormals[a].x,
        d.nrm[a].y= iMesh->mNormals[a].y,
        d.nrm[a].z= iMesh->mNormals[a].z;
      else
        d.nrm[a].set(0.0f, 0.0f, 0.0f);

      if(iMesh->mTextureCoords[0])
        d.tex1[a].x= iMesh->mTextureCoords[0]->x,
        d.tex1[a].y= iMesh->mTextureCoords[0]->y;
      else
        d.tex1[a].x= d.tex1[a].y= 0.0f;
    }

  } else if(out_m->dataType== 1) {
    out_m->size= ixMesh::Data1i::size()* out_m->nrVert;
    out_m->data= new uint8[out_m->size];
    ixMesh::Data1i d(out_m);

    for(uint32 a= 0; a< out_m->nrVert; a++) {
      d.vert[a].pos.x= iMesh->mVertices[a].x,
      d.vert[a].pos.y= iMesh->mVertices[a].y,
      d.vert[a].pos.z= iMesh->mVertices[a].z;

      if(iMesh->mNormals)
        d.vert[a].nrm.x= iMesh->mNormals[a].x,
        d.vert[a].nrm.y= iMesh->mNormals[a].y,
        d.vert[a].nrm.z= iMesh->mNormals[a].z;
      else
        d.vert[a].nrm.set(0.0f, 0.0f, 0.0f);

      if(iMesh->mTextureCoords[0])
        d.vert[a].tex1.x= iMesh->mTextureCoords[0]->x,
        d.vert[a].tex1.y= iMesh->mTextureCoords[0]->y;
      else
        d.vert[a].tex1.x= d.vert[a].tex1.y= 0.0f;
    }

  } /// all dataTypes


  // vvvvvvvvvvvvvvv
  // MATERIAL MAKEME <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // ^^^^^^^^^^^^^^^
  // MATERIAL MUST BE LOADED IN ui.cpp, same as calling this func
  //mat->map[0]= ix.vki.noTexture;
  return true;
}


bool Output::inportMesh_n(ixMesh *out_m, const aiScene *in_scene, uint32 in_n) {
  return _inportMesh(out_m, in_scene, in_n);
}


bool Output::inportMesh_name(ixMesh *out_m, const aiScene *in_scene, const char *in_name) {
  aiString in_aiName;
  in_aiName= in_name;

  for(uint a= 0; a< in_scene->mNumMeshes; a++)
    if(in_scene->mMeshes[a]->mName== in_aiName) {
      // found it
      _inportMesh(out_m, in_scene, a);
      return true;
    }
  printf("Mesh name not found\n");
  return false;
}


bool Output::saveAs() {
  const int bufferSize= 512;
  char fname[bufferSize]= "";

  OPENFILENAME ofn;                 // open file name dialog
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize=  sizeof(ofn);
  ofn.hwndOwner=    (HWND)ix.win->_hWnd;
  ofn.lpstrFilter=  "Ix I3D Files (*.i3d)\0*.i3d\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile=    fname;
  ofn.nMaxFile=     bufferSize;
  ofn.Flags=        OFN_EXPLORER| OFN_FILEMUSTEXIST| OFN_HIDEREADONLY| OFN_NOCHANGEDIR;
  ofn.lpstrDefExt=  "i3d";

  if(GetSaveFileName(&ofn)) {
    std::filesystem::path base(std::filesystem::current_path());
    std::filesystem::path fsPath(fname);
    std::filesystem::path relative(std::filesystem::relative(fsPath, base));
    /*
    the dae filepath must be updated to be relative
    must check if you can actually save something
      but things are working, i got all i need, relative paths and separators are good, everything.;
    so i should finish this week, if all goes well;
    */

    str8 relPath((char *)relative.generic_u8string().c_str());
    ixUtil::changePathSeparator(&relPath);

    ix.console().printf("FILE SAVE: [%s]", relPath.d);

    return ix.res.mesh.save(relPath.d, &outMesh);

  }
  return false;
}



bool Output::save() {
  if(outMesh.fileName.len<= 1)
    return saveAs();

  return ix.res.mesh.save(outMesh.fileName, &outMesh);
}








