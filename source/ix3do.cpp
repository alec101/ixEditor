#include "ix/ix.h"

#include "ix3do.h"


Ix ix;

#ifdef _DEBUG
bool enableValidationLayers= true;
#else
bool enableValidationLayers= false;
#endif

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

  // window creation
  osi.vkCreateWindow(&osi.win[0], &osi.display.monitor[0], "Ix 3DO file manager", 1500, 900, 1);
  
  // after window init
  in.init();

  // 150MB right in here... 50MB vulkan device (nothing to do about it)
  // well, memory for clusters is like 185, by default
  ix.initWindow(&osi.win[0]);

  // start program conf

  Ix::console().saveBuffers();

  while(1) {
    // osi & ix update
    osi.checkMSG();
    in.update();

    // esc key press / osi exit - quick quit
    if(in.k.key[in.Kv.esc] || osi.flags.exit)
      break;

    // window resized
    //if(osi.flags.windowResized)
    //  ui.updateOnWindowResize();

    ix.update();
    // ix.wsys().update(); // this is moved to ui.update 

  } // infinite loop
    
    
  // exit
  ix.shutdown();
  osi.exit(0);
  return 0;
}