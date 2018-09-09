#pragma once
/////////////////////////////////////////////////////////////////////
// AutoTest.h -                                                    //
// ver 1.0                                                         //
// YUXUAN XING CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provide some test about checkin,checkout automatically.
* Required Files:
* ---------------
* Version.h
* DbCore.h
* Definitions.h
* Checkin.h
* Checkout.h
* Maintenance History:
* --------------------
* 5/1/2018 fisrt release
*/
#include "../CppNoSqlDb/DbCore/DbCore.h"
#include "../Version/Version.h"
#include "../Checkin/checkin.h"
#include "../Checkout/Checkout.h"
#include "../CppNoSqlDb/Utilities/StringUtilities/StringUtilities.h"
#include "../CppNoSqlDb/Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>

using namespace NoSqlDb;

bool test1();
bool test2();
//bool test3();
bool test4();