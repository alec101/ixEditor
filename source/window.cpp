#include "ixEditor.h"

/*

- osi vulkan init requires ix.vk, but if you want other osiWindows, what then?
  atm, doing the init for every window seems ok, no problems, but if there will be a problem, maybe a "switch vk renderer" func has to happen in osi



*/


EditorWindow::EditorWindow(): i3d(this), tex(this) {

  isMain= (&ix== Ix::getMain());
  
  osiWin= null;
}


EditorWindow::~EditorWindow() {
}


void EditorWindow::delData() {
}


bool EditorWindow::init() {
  ix.cfg.vk.enableValidationLayers= enableValidationLayers;

  //ix.cfg.vk.derivedData[0]= &game.slGlbData[0];
  //ix.cfg.vk.derivedData[1]= &game.slGlbData[1];
  //ix.cfg.vk.derivedDataSize= sizeof(PRJ_NAME::GlbData);

  ix.init();
  //if(isMain)
  osi.vkInit(&ix.vk);   // just init for every window, i don't see a drawback atm
  
  // window creation
  int32 dx, dy;
  str8 winName;

  if(isMain) {
    dx= 1500;
    dy= 900;
    winName= "Ix Editor Main Window";
  } else {
    dx= 1500;
    dy= 900;
    winName.f("Ix Editor window %d", nWindows()+ 1);
  }
  osiWin= &osi.win[nWindows()];

  if(osi.vkCreateWindow(osiWin, osi.display.primary, winName.d, dx, dy, 1)) {
    nWindows()++;
  } else {
    osiWin= null;
    return false;
  }

  // 150MB right in here... 50MB vulkan device (nothing to do about it)
  // well, memory for clusters is like 185, by default
  ix.initWindow(osiWin);



  
  fnt1_10= ix.pr.loadFont("montserrat.fnt", 10);
  fnt1_12= ix.pr.loadFont("montserrat.fnt", 12);
  fnt1_15= ix.pr.loadFont("montserrat.fnt", 15);
  fnt1_20= ix.pr.loadFont("montserrat.fnt", 20);


  fnt2_12= ix.pr.loadFont("montserratSB.fnt", 12);
  fnt2_13= ix.pr.loadFont("montserratSB.fnt", 13);
  fnt2_14= ix.pr.loadFont("montserratSB.fnt", 14);
  fnt2_15= ix.pr.loadFont("montserratSB.fnt", 15);
  fnt2_16= ix.pr.loadFont("montserratSB.fnt", 16);
  fnt2_17= ix.pr.loadFont("montserratSB.fnt", 17);
  fnt2_18= ix.pr.loadFont("montserratSB.fnt", 18);
  fnt2_20= ix.pr.loadFont("montserratSB.fnt", 20);


  fntStyle1.selFont= fnt2_12;
  fntStyle1.color1.set(1, 1, 1, 1);
  fntStyle1.drawMode= 0x02; //0x01;
  fntStyle1.outlineSize= 1;
  fntStyle1.shadowPos= vec2(1.0, 1.0);

  fntStyle2.selFont= fnt2_17;
  fntStyle2.color1.set(1, 1, 1, 1);
  fntStyle2.drawMode= 0x01; //| 0x02;
  fntStyle2.outlineSize= 1;
  fntStyle2.shadowPos= vec2(1.5, 1.5);
  
  


  i3d.init();
  return true;
}



void EditorWindow::update() {
  // i can't remember RN, but i think every window will have it's own ix, so the ix update should happen there, NOT SURE atm
  ix.update();

  // ix windowing system update, happens only on the main window update
  if(isMain)
    wsysUpdate()= ix.wsys().update();   // ix window system update

  i3d.update();
}

void EditorWindow::draw() {
  if(ix.startRender()) {
    i3d.draw();
    i3d.ui.draw();
    //viewer.draw();      /// has startPerspective() / endPersp
    //ui.draw();          /// has startOrtho() / endOrtho
    ix.endRender();
  }
}