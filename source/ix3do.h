#pragma once

#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

//#include "assimp/include/assimp/importerdesc.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <assimp/scene.h>
#include <assimp/Importer.hpp>


#include "ix/ix.h"
#include "ui.h"
#include "viewer.h"
#include "input.h"
#include "output.h"
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
  #else
    #pragma comment(lib, "vko.win64.dbg.lib")
    #pragma comment(lib, "osi.vk.win64.dbg.lib")
    #pragma comment(lib, "ix.vk.win64.dbg.lib")
    #pragma comment(lib, "debug/assimp-vc143-mtd.lib")

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



extern Ix ix;




