#include "ixEditor.h"


I3Doutput::I3Doutput(I3Deditor *in_parent, EditorWindow *in_win): _i3d(in_parent), _win(in_win), outMesh(&in_win->ix) {
  _ix= &in_win->ix;

  defineFormatFromImport= true;
}



I3Doutput::~I3Doutput() {
  //mat->delData();
}



void I3Doutput::delData() {
}



void I3Doutput::init() {

  mat= _ix->res.mat.add();

}


bool I3Doutput::_importMesh(ixMesh *out_m, const aiScene *in_scene, uint32 in_nr) {
  // flags can be enabled with the respective readFile proc from AI
  // aiProcess_LimitBoneWeights - MUST be enabled; - 4 bones max, AI will re-weight them to 1.0, also
  // aiProcess_PopulateArmatureData - MUST be enabled - need armature data for parent-children-root


  //input.meshSel; input.matSel;
  //input.scene->mMeshes[input.meshSel];
  if(in_scene== null) return false;

  aiMesh *iMesh= in_scene->mMeshes[in_nr];    /// shortcut
  uint32 *p1, *p2;            // walks int32 mem
  uint16 *p16;                // walks int16 mem
  float *pf;                  // walks float mem
  uint32 offset1, offset2;
  uint32 p2size;

  AI2IXskeletron ai2ix;       // helper class to transform from ai bone id to ix bone id

  uint8 *vertBones= null;     // ALLOC INIT 1


  _ix->vk.DeviceWaitIdle(_ix->vk.device);

  
  out_m->dealloc_data();
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

  out_m->fileInputName= _i3d->input.fileName;
  
  
  if(defineFormatFromImport) {
    // interweave will not be touched
    out_m->format.clear();

    if(iMesh->HasPositions())
      out_m->format.setupChannel(ixMesh::channelType::POS, 3);
    
    if(iMesh->HasNormals())
      out_m->format.setupChannel(ixMesh::channelType::NRM, 3);

    if(iMesh->HasVertexColors(0))
      out_m->format.setupChannel(ixMesh::channelType::COL, 4);

    for(uint a= 0; a< IXMESH_MAX_TEX_CHANNELS; a++)
      if(iMesh->HasTextureCoords(a))
        out_m->format.setupChannelNr((uint)ixMesh::channelType::TEX1+ a, iMesh->mNumUVComponents[a]);

    if(iMesh->HasBones()) {                                         // using method 3, compressed, 6components total for 4 bones max
      out_m->format.setupChannel(ixMesh::channelType::BONEID, 2);   // vec2i
      out_m->format.setupChannel(ixMesh::channelType::BONEWG, 4);   // vec4
    }
  }
  


  // OTHER POSSIBLE IMPORTS:
    
  // iMesh->mAABB                       // <<< THE BOUNDING BOX
  // iMesh->GetNumColorChannels();      // <<< MULTIPLE COLOR CHANNELS CAN BE IMPOTED
  // iMesh->HasTextureCoordsName(index) // <<< texture coords NAMES 
  // iMesh->HasTangentsAndBitangents()  // <<< tangents & bitangents (mTangents+mBitangents)
  // iMesh->mAnimMeshes                 // <<< extra meshes tied to this, check mAnimMeshes info, could happen, not sure
  

  out_m->alloc_data(out_m->nrVert);

  for(uint c= 0; c< IXMESH_MAX_CHANNELS; c++) {
    if(out_m->format.ch[c].size== 0)
      continue;

    p1= (uint32 *)out_m->data+ out_m->getChannel(&out_m->format, out_m->nrVert, c),
    offset1= (uint32)out_m->getChannelStride(&out_m->format, c);

    p2= null, offset2= 0, p2size= 0;

    if(c== (uint)ixMesh::channelType::POS && iMesh->HasPositions())
      p2= (uint32 *)iMesh->mVertices, p2size= 3;
    else if(c== (uint)ixMesh::channelType::TEX1 && iMesh->HasTextureCoords(0))
      p2= (uint32 *)iMesh->mTextureCoords[0], p2size= iMesh->mNumUVComponents[0];
    else if(c== (uint)ixMesh::channelType::TEX2 && iMesh->HasTextureCoords(1))
      p2= (uint32 *)iMesh->mTextureCoords[1], p2size= iMesh->mNumUVComponents[1];
    else if(c== (uint)ixMesh::channelType::TEX3 && iMesh->HasTextureCoords(2))
      p2= (uint32 *)iMesh->mTextureCoords[2], p2size= iMesh->mNumUVComponents[2];
    else if(c== (uint)ixMesh::channelType::TEX4 && iMesh->HasTextureCoords(3))
      p2= (uint32 *)iMesh->mTextureCoords[3], p2size= iMesh->mNumUVComponents[3];
    else if(c== (uint)ixMesh::channelType::NRM && iMesh->HasNormals())
      p2= (uint32 *)iMesh->mNormals, p2size= 3;
    else if(c== (uint)ixMesh::channelType::COL && iMesh->HasVertexColors(0))
      p2= (uint32 *)iMesh->mColors[0], p2size= 4;
    /// bones are ignored ATM
    offset2= p2size;


    // copy operation
    for(uint v= 0; v< out_m->nrVert; v++) {                /// for each vertex
      for(uint d= 0; d< out_m->format.ch[c].size; d++)    /// for each component
        if(d< p2size)
          p1[d]= p2[d];
        else
          p1[d]= 0;
      
      if(offset2) p2+= offset2;
      p1+= offset1;
    } /// for each vertex
  } /// for each vertex channel

  

  // bone data populate

  if(out_m->format.boneid.size> 0) {

    // set all bone weight data to zero - if a weight is zero, no computation will happen
    p1= (uint32 *)out_m->data+ out_m->getChannel(&out_m->format, out_m->nrVert, (uint32)ixMesh::channelType::BONEID);
    offset1= (uint32)out_m->getChannelStride(&out_m->format, (uint32)ixMesh::channelType::BONEID);
    p2= (uint32 *)out_m->data+ out_m->getChannel(&out_m->format, out_m->nrVert, (uint32)ixMesh::channelType::BONEWG);
    offset2= (uint32)out_m->getChannelStride(&out_m->format, (uint32)ixMesh::channelType::BONEWG);

    for(uint a= 0; a< out_m->nrVert; a++) {
      for(uint b= 0; b< out_m->format.boneid.size; b++)
        p1[b]= 0;
      for(uint b= 0; b< out_m->format.bonewg.size; b++)
        p2[b]= 0;
      p1+= offset1;
      p2+= offset2;
    }

    // AI bone data
    if(iMesh->HasBones()) {
      /// vertBones this will remember how many times a vertex's bone data was written
      vertBones= new uint8[out_m->nrVert];        // ALLOC 1
      for(uint a= 0; a< out_m->nrVert; a++)
        vertBones[a]= 0;

      ai2ix.doImport(iMesh->mBones, iMesh->mNumBones);

      for(uint bone= 0; bone< iMesh->mNumBones; bone++) {
        for(uint w= 0; w< iMesh->mBones[bone]->mNumWeights; w++) {
          uint v= iMesh->mBones[bone]->mWeights[w].mVertexId;
          /// bone id
          p16= (uint16 *)out_m->getVertexData(out_m->data, &out_m->format, out_m->nrVert, v, (uint32)ixMesh::channelType::BONEID);
          p16[vertBones[v]]= ai2ix.ai2ix(bone);

          if(p16[vertBones[v]]== ~0)
            Ix::console().printf("_importMesh: ai2ix helper class could not find a match");

          /// bone weight
          pf= (float *)out_m->getVertexData(out_m->data, &out_m->format, out_m->nrVert, v, (uint32)ixMesh::channelType::BONEWG);
          pf[vertBones[v]]= iMesh->mBones[bone]->mWeights[w].mWeight;

          vertBones[v]++;
        } /// for each weight
      } /// for each bone
    } /// has ai bone data
  } /// has out mesh has bone data



  if(vertBones) { delete[] vertBones; vertBones= null; }    // DEALLOC 1



  // vvvvvvvvvvvvvvv
  // MATERIAL MAKEME <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // ^^^^^^^^^^^^^^^
  // MATERIAL MUST BE LOADED IN ui.cpp, same as calling this func
  //mat->map[0]= _ix->vki.noTexture;
  return true;
}


bool I3Doutput::importMesh_n(ixMesh *out_m, const aiScene *in_scene, uint32 in_n) {
  return _importMesh(out_m, in_scene, in_n);
}


bool I3Doutput::importMesh_name(ixMesh *out_m, const aiScene *in_scene, const char *in_name) {
  aiString in_aiName;
  in_aiName= in_name;

  for(uint a= 0; a< in_scene->mNumMeshes; a++)
    if(in_scene->mMeshes[a]->mName== in_aiName) {
      // found it
      _importMesh(out_m, in_scene, a);
      return true;
    }
  printf("Mesh name not found\n");
  return false;
}


bool I3Doutput::saveAs() {
  const int bufferSize= 512;      // MAX PATH OF 512, CAN THIS BE BIGGER?
  char fname[bufferSize]= "";

  OPENFILENAME ofn;                 // open file name dialog
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize=  sizeof(ofn);
  ofn.hwndOwner=    (HWND)_ix->win->_hWnd;
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

    _ix->console().printf("FILE SAVE: [%s]", relPath.d);

    return _ix->res.mesh.save(relPath.d, &outMesh);

  }
  return false;
}



bool I3Doutput::save() {
  if(outMesh.fileName.len<= 1)
    return saveAs();

  return _ix->res.mesh.save(outMesh.fileName, &outMesh);
}








