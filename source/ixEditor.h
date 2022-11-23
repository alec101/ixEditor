#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif


#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <filesystem>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "ix/ix.h"
#include "osi/include/util/fileOp.h"



// I3D editor
#include "I3Dui.h"
#include "I3Dinput.h"
#include "I3Doutput.h"
#include "I3Deditor.h"

#include "TEXui.h"
#include "TEXinput.h"
#include "TEXoutput.h"
#include "TEXeditor.h"

#include "window.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


/// osi required libs
#ifdef _WIN64
  #pragma comment(lib, "XInput_64.lib")
  #pragma comment(lib, "dinput8_64.lib")
  #pragma comment(lib, "dxguid_64.lib")
  #pragma comment(lib, "d3d9_64.lib")

  #ifdef NDEBUG
    #pragma comment(lib, "vko.win64.lib")
    #pragma comment(lib, "osi.vk.win64.lib")
    #pragma comment(lib, "ix.vk.win64.lib")
    #pragma comment(lib, "../../assimp/lib/release/assimp-vc143-mtd.lib")
  #else
    #pragma comment(lib, "vko.win64.dbg.lib")
    #pragma comment(lib, "osi.vk.win64.dbg.lib")
    #pragma comment(lib, "ix.vk.win64.dbg.lib")
    #pragma comment(lib, "../../assimp/lib/debug/assimp-vc143-mtd.lib")

  #endif /// RELEASE / DEBUG
#else
  #pragma comment(lib, "XInput_32.lib")
  #pragma comment(lib, "dinput8_32.lib")
  #pragma comment(lib, "dxguid_32.lib")
  #pragma comment(lib, "d3d9_32.lib")

  #ifdef NDEBUG
    #pragma comment(lib, "vko.win32.lib")
    #pragma comment(lib, "osi.vk.win32.lib")
    #pragma comment(lib, "ix.vk.win32.lib")
  #else
    #pragma comment(lib, "vko.win32.dbg.lib")
    #pragma comment(lib, "osi.vk.win32.dbg.lib")
    #pragma comment(lib, "ix.vk.win32.dbg.lib")
  #endif /// RELEASE / DEBUG
#endif


extern bool enableValidationLayers;

extern EditorWindow mainWin;




extern ixFontStyle fntStyle1, fntStyle2;

extern void *fnt1_10, *fnt1_12, *fnt1_15, *fnt1_20;      // main things, titles
extern void *fnt2_12, *fnt2_13, *fnt2_14, *fnt2_15;      // large bodies of text, mass of text
extern void *fnt2_16, *fnt2_17, *fnt2_18, *fnt2_20;      // large bodies of text, mass of text



