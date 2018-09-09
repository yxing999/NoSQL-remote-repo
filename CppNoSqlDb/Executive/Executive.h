#pragma once
///////////////////////////////////////////////////////////////////////
// Executive.h - used to set up deployment process                   //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
* - This package defines a single header file that includes all the NoSqlDb parts.
*   We do this so that an application needs only to include two headers:
*   - NoSqlDb.h : declare NoSqlDb functionality
*   - PayLoad.h : declare application specific payload functionality
*   The application needs to:
*   - include PayLoad.cpp in its project
*   - make a reference to the Executive project's static library
*/

#include <iostream>
#include "NoSqlDb.h"

class Executive
{
public:
  static void identify(std::ostream& out = std::cout)
  {
    out << "\n  \"" << __FILE__ << "\"";
  }
};