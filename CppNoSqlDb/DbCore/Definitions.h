#ifndef DEFINITIONS_H
#define DEFINITIONS_H
///////////////////////////////////////////////////////////////////////
// Definitions.h - define aliases used throughout NoSqlDb namespace  //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

namespace NoSqlDb
{
  using Key = std::string;
  using Keys = std::vector<Key>;
  using Children = std::vector<Key>;
  using Parents = std::vector<Key>;

  const bool showKey = true;
  const bool doNotShowKey = false;
}
#endif
