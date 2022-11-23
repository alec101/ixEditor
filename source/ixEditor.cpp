#include "ixEditor.h"


/* ASSIMP install / info #################################################################

    1. git it up
    2. install cmake (reboot so path works)
    3. cmake cmakelists.txt - always re-cmake it, if you move your shit, as it creates direct paths, not relatives, unfortunately
    4. open solution assimp, build all you need (debug/release)
    5. put your include to point to assimp/include, lib to assimp/lib/[release/debug]

    -bin/[dbg/rel]/assimp dll is there, you might wanna place it in the output
    -not sure if you need the dll, but it would be nice NOT to need it. There is a lib, therefore the dll should not be really needed,
     unless some crap is needed from it for ixEdit, one dll would not be the end, i guess
    -also there's some nice libs inside assimp, to check, not sure

    there's all the damn compressors in the world in there, unfortunately, it's kinda a mess, but for the editor this will do, nothing you can do about it, tons of garbage


    TODO:
    -maybe a UI texture editor? you'd select where each control is in the texture, instead of doing it with photoshop/text editor

*/

#ifdef _DEBUG
bool enableValidationLayers= true;
#else
bool enableValidationLayers= false;
#endif

EditorWindow mainWin;
Ix *ix= &mainWin.ix;

ixFontStyle fntStyle1, fntStyle2;

void *fnt1_10, *fnt1_12, *fnt1_15, *fnt1_20;      // main things, titles
void *fnt2_12, *fnt2_13, *fnt2_14, *fnt2_15;      // large bodies of text, mass of text
void *fnt2_16, *fnt2_17, *fnt2_18, *fnt2_20;      // large bodies of text, mass of text

void updateWork();

// 3DO's will have to have a type of shader at least to be tied to, maybe? possible...


#if defined OS_WIN && defined NDEBUG
int WinMain(_In_  HINSTANCE hInstance, _In_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_  int nCmdShow) {
#else
int main(int argc, char *argv[], char *envp[]) {
#endif

  // pre window init
  error.useConsoleFlag= true;
  Ix::Config::shaderDIR()= "../../ix/shaders/";
  Ix::console().print("Console start line ------------------");

  mainWin.init();
  
  // after window init
  in.init();

  // start program conf

  Ix::console().saveBuffers();

  if(osi.argc> 1)
    updateWork();   // if there is a updateDir.txt and "update" is passed as a starting prog argument, an update work happens


  // >>> skeleton alloc / dealloc test has to happen;
  // >>> must see what happens with the bulk alloc, if the mem is properly deallocated with a delete[];

  while(1) {
    // osi & ix update
    osi.checkMSG();
    in.update();

    // esc key press / osi exit - quick quit
    if(in.k.key[in.Kv.esc] || osi.flags.exit)
      break;

    // console
    if((in.k.lastKey[0].code== in.Kv.grave) && (!in.k.lastKey[0].checked)) {
      in.k.clearTypedBuffer();
      Ix::console().toggle(), in.k.lastKey[0].checked= true;
    }
    // window resized
    //if(osi.flags.windowResized)
    //  ui.updateOnWindowResize();

    

    mainWin.update();
    // other windows must update here <<<<<<<<<<<<<<<<<<<<
    //ui.update();
    //viewer.update();  /// has camera computed

    // RENDERING
    mainWin.draw();
    // other windows must be drawn here <<<<<<<<<<<<<<<<


  } // infinite loop
    
    
  // exit

  mainWin.ix.shutdown();
  osi.exit(0);
  return 0;
}





























// ##    ##  ######    ######      ####    ########  ########
// ##    ##  ##    ##  ##    ##  ##    ##     ##     ##
// ##    ##  ##    ##  ##    ##  ##    ##     ##     ######
// ##    ##  ######    ##    ##  ########     ##     ##
//   ####    ##        ######    ##    ##     ##     ########

void update(str8 *in_file, str8 *in_workDir);

void updateWork() {
  char buf[1024]; buf[0]= 0;
  str8 arg1, line, cmd, a1, a2, a3, a4;
  str8 workingDir, file;
  uint32 filesProccessed= 0;

  file.wrap(buf, 1023);


  if(osi.argc> 1)
    arg1= osi.argv[1];
  arg1.lower();

  // do the update work
  if(arg1== "update") {
    
    ix->win->hide();
    printf("Update command started\n");
    printf(" -attempting to read update.txt\n");

    FILE *f= null;
    f= fopen("update.txt", "rb");
    if(f== null) {
      printf("update.txt not present, update command aborting\n");
      return;
    } else 
      printf("update.txt found, started proccessing\n");

      while(readLine8(f, &line)) {
        ixUtil::parseGenericTxtCommand(&line, &cmd, &a1, &a2, &a3, &a4);
        cmd.lower(); a1.lower(); // a2.lower(); a3.lower(); a4.lower();

        if(cmd== "updatedir" || cmd== "update directory" || cmd== "workdir" || cmd== "work directory")
          workingDir= a1;
        else if(cmd== "file") {
          file= a1;
          update(&file, &workingDir);
          filesProccessed++;
        }
      }


    fclose(f);
    printf("proccessed %d files\n", filesProccessed);
    osi.exit(0);    // success, then exit, no need to proceed
  }
  // return and start the program as if update command did not do anything
}



void update(str8 *in_file, str8 *in_workDir) {
  static ixMesh m(ix);
  char buf1[1024], buf2[1024]; buf1[0]= 0; buf2[0]= 0;
  str8 inputDirFile;
  str8 filePath;
  inputDirFile.wrap(buf1, 1023);
  filePath.wrap(buf2, 1023);
  
  printf(" -loading [%s] ...", in_file->d);
  if(!m.load(*in_file, 0x0001)) {       // load ix mesh
    printf("File load error\n");
    return;
  }
  if(in_file->d== null || in_file->nrUnicodes== 0) {
    printf("file name error\n");
    return;
  }

  // input directory + filename
  getFilePath(in_file->d, &filePath);
  inputDirFile= *in_workDir;
  inputDirFile+= filePath;
  inputDirFile+= '/';
  inputDirFile+= m.fileInputName;

  // load the inport scene and inport the data
  if(mainWin.i3d.input.fileName!= m.fileInputName)
    if(!mainWin.i3d.input.loadFileName(inputDirFile.d, false)) {
      printf("\n");
      return;
    }
  if(!mainWin.i3d.output.importMesh_name(&m, mainWin.i3d.input.scene, m.name)) {
    printf("\n");
    return;
  }

  if(m.save(*in_file)) {       // save the updated data in ix file
    printf("success\n");
  } else {
    printf("save failed\n");
  }

}








