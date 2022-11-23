#pragma once


class I3Deditor;
class EditorWindow;


class I3Doutput {
  I3Deditor    *_i3d;
  EditorWindow *_win;
  Ix           *_ix;
public:

  ixMesh outMesh;

  bool defineFormatFromImport;      // [interweave will NOT be touched but used] if mesh.format is to be changed on import, to fit the file, or import with current mesh.format and fill in what is possible

  // the material that will be used for the rendering
  // there should be only one, i don't see a need for a list
  ixMaterial *mat;

  // funcs

  bool importMesh_n(ixMesh *out_m, const aiScene *in_scene, uint32 in_nr);
  bool importMesh_name(ixMesh *out_m, const aiScene *in_scene, const char *in_name);
  bool importMat_n(ixMesh *out_m, const aiScene *in_scene, uint32 in_nr);
  
  void init();


  bool save();
  bool saveAs();

  // constructor / destructor

  I3Doutput(I3Deditor *in_parent, EditorWindow *in_win);
  ~I3Doutput();
  void delData();
  
private:


  void _computeRelativePath();

  bool _importMesh(ixMesh *out_m, const aiScene *in_Scene, uint32 in_nr);

  //void _aiSkelly2ixSkelly(
};











// ASSIMP to IX bone data helper class ============--------------

class AI2IXskeletron {
public:
  struct Data {
    uint16 ixID;
    uint32 aiID;
    aiNode *nodeP;
  } *data;
  uint32 nrBones;

  void doImport(aiBone **in_aiBone, uint32 in_nrBones) {
    delData();

    aiBone **impBone= in_aiBone;
    nrBones= in_nrBones;

    // find the root of the armature
    aiNode *root= impBone[0]->mNode;
    while(root->mParent!= nullptr)
      root= root->mParent;
    
    data= new Data[nrBones];

    // populate ixID, nodeP
    uint16 ixID= 0;
    writeList(data, &ixID, root);

    // populate aiID
    for(uint a= 0; a< nrBones; a++) {
      uint32 n= findNode(impBone[a]->mNode);
      data[n].aiID= a;
    }
  }


  uint16 ai2ix(uint32 in_ai) {
    for(uint a= 0; a< nrBones; a++)
      if(data[a].aiID== in_ai) return a;  // found
    return ~0;                            // not found
  }

  uint32 ix2ai(uint16 in_ix) { return data[in_ix].aiID; }


  void delData() { if(data) { delete[] data; data= null; } nrBones= 0; }
  AI2IXskeletron(): data(null), nrBones(0) {}
  ~AI2IXskeletron() { delData(); }

private:
  uint32 findNode(aiNode *in_node) {
    for(uint a= 0; a< nrBones; a++)
      if(data[a].nodeP== in_node)
        return a;
    return ~0;
  }


  void writeList(Data *in_data, uint16 *in_ixID, aiNode *in_root) {
    // root writen first, i don't see a simpler logic atm
    in_data->ixID= *in_ixID;
    in_data->nodeP= in_root;
    in_data++;
    (*in_ixID)++;
    _writeChildren(in_data, in_ixID, in_root);
  }
  // to be used ONLY with writeList
  void _writeChildren(Data *in_data, uint16 *in_ixID, aiNode *in_node) {
    for(uint a= 0; a< in_node->mNumChildren; a++) {
      in_data->ixID= *in_ixID;
      in_data->nodeP= in_node->mChildren[a];
      in_data++;                // advance pointer
      (*in_ixID)++;             // update ix id
    }

    for(uint a= 0; a< in_node->mNumChildren; a++)
      _writeChildren(in_data+ a, in_ixID, in_node->mChildren[a]);
  }

  //void countYourself(uint32 *out_n, aiNode *in_node) {
  //  (*out_n)++;
  //  for(uint a= 0; a< in_node->mNumChildren; a++)
  //    countYourself(out_n, in_node->mChildren[a]);
  //}
};



