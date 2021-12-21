#include "ix3do.h"


#ifdef _DEBUG
bool enableValidationLayers= true;
#else
bool enableValidationLayers= false;
#endif


Ix ix;
UI ui;
Viewer viewer;
Input input;
Output output;
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
  ix.cfg.vk.enableValidationLayers= enableValidationLayers;

  //ix.cfg.vk.derivedData[0]= &game.slGlbData[0];
  //ix.cfg.vk.derivedData[1]= &game.slGlbData[1];
  //ix.cfg.vk.derivedDataSize= sizeof(PRJ_NAME::GlbData);

  ix.init();
  osi.vkInit(&ix.vk);

  // attemptLoadWorkingDir();

  // window creation
  osi.vkCreateWindow(&osi.win[0], &osi.display.monitor[0], "Ix 3DO file manager", 1500, 900, 1);
  
  // after window init
  in.init();

  // 150MB right in here... 50MB vulkan device (nothing to do about it)
  // well, memory for clusters is like 185, by default
  ix.initWindow(&osi.win[0]);

  // start program conf

  Ix::console().saveBuffers();

  if(osi.argc> 1)
    updateWork();   // if there is a updateDir.txt and "update" is passed as a starting prog argument, an update work happens


  ui.init();
  viewer.init();
  output.init();

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
      ix.console().toggle(), in.k.lastKey[0].checked= true;
    }
    // window resized
    //if(osi.flags.windowResized)
    //  ui.updateOnWindowResize();
    
    ix.update();
    ui.update();      /// has ix.wsys().update
    viewer.update();  /// has camera computed

    // RENDERING
    if(ix.startRender()) {
      viewer.draw();      /// has startPerspective() / endPersp
      ui.draw();          /// has startOrtho() / endOrtho
      ix.endRender();
    }

  } // infinite loop
    
    
  // exit
  ix.shutdown();
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
    
    ix.win->hide();
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
  static ixMesh m(&ix);
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
  if(input.fileName!= m.fileInputName)
    if(!input.loadFileName(inputDirFile.d, false)) {
      printf("\n");
      return;
    }
  if(!output.inportMesh_name(&m, input.scene, m.name)) {
    printf("\n");
    return;
  }

  if(m.save(*in_file)) {       // save the updated data in ix file
    printf("success\n");
  } else {
    printf("save failed\n");
  }

}








