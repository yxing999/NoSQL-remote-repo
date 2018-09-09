#pragma once
///////////////////////////////////////////////////////////////////////
// TestClass.h - defines all testing for NoSqlDb                     //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a TestClass class that implements all testing
*  for the NoSqlDb.  It is an alternative to the TestUtilities demonstrated
*  in class for the NoSqlDb prototype.
*
*  Required Files:
*  ---------------
*  TestClass.h, TestClass.cpp
*  DbCore.h, DbCore,cpp
*  DateTime.h, DateTime.cpp
*  PayLoad.h
*  Query.h, Query.cpp
*  StringUtilities.h, StringUtilities.cpp
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 10 Feb 2018
*  - first release
*/

#include <string>
#include <iostream>
#include <vector>
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"

class TestDb
{
public:
  using MPtr = bool(TestDb::*)();

  TestDb(NoSqlDb::DbCore<NoSqlDb::PayLoad>& db) : db_(db) {}

  static void identity(std::ostream& out = std::cout);
  bool invoke(MPtr mPtr);
  bool testR1();
  bool testR2();
  bool testR3a();
  bool testR3b();
  bool testR4();
  bool testR5a();
  bool testR5b();
  bool testR6();
  bool testR7();
  bool testR8();
  bool testR9();
  bool testR10();
  bool testR11();
  bool testR12();
  bool testR13();
private:
  NoSqlDb::DbCore<NoSqlDb::PayLoad>& db_;
};

inline void TestDb::identity(std::ostream& out)
{
  out << "\n  \"" << __FILE__ << "\"";
}

inline bool TestDb::invoke(TestDb::MPtr mPtr)
{
  try {
    bool result = ((*this).*mPtr)();
    if (result)
    {
      std::cout << "\npassed <<<<\n";
    }
    else
    {
      std::cout << "\nfailed <<<<\n";
    }
    return result;
  }
  catch (std::exception& ex)
  {
    std::cout << "\n    exception thrown:";
    std::cout << "\n    " << ex.what();
    std::cout << "\nfailed <<<<\n";
    return false;
  }
}