#pragma once
/////////////////////////////////////////////////////////////////////
// CommLibWrapper.h - Comm object factory                          //
// ver 1.0
// YUXUAN XING
// Source:Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017//
/////////////////////////////////////////////////////////////////////
/*
*  Provides export and import declarations for Comm
*
*  Required Files:
* -----------------
*  CommLibWrapper.h, CommLibWrapper.cpp
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 27 Mar 2018
*  - first release
*/
#include <string>
#include "../CppCommWithFileXfer/MsgPassingComm/IComm.h"

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

namespace MsgPassingCommunication
{
  extern "C" {
    struct CommFactory {
      static DLL_DECL IComm* create(const std::string& machineAddress, size_t port);
    };
  }
}