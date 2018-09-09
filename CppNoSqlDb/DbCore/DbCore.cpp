/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 1.2                                                         //
// Yuxuan Xing, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <functional>

#ifdef TEST_DBCORE

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n=1, std::ostream& out = std::cout)
{
  Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
  DbCore<std::string>& db() { return db_; }
private:
  static DbCore<std::string> db_;
};

DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
  Utilities::title("Demonstrating Requirement #1");
  std::cout << "\n  " << typeid(std::function<bool()>).name()
    << ", declared in this function, "
    << "\n  is only valid for C++11 and later versions.";
  putLine();
  return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
  Utilities::title("Demonstrating Requirement #2");
  std::cout << "\n  A visual examination of all the submitted code "
    << "will show only\n  use of streams and operators new and delete.";
  putLine();
  return true;
}

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
  Utilities::title("Demonstrating Requirement #3a - creating DbElement");
  std::cout << "\n  Creating a db element with key \"Fawcett\":";

  // create database to hold std::string payload

  DbCore<std::string> db;
  DbProvider dbp;
  dbp.db() = db;

  // create some demo elements and insert into db

  DbElement<std::string> demoElem;

  demoElem.name("Jim");
  demoElem.descrip("Instructor for CSE687");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad("The good news is ...");

  if (demoElem.name() != "Jim")
    return false;
  if (demoElem.descrip() != "Instructor for CSE687")
    return false;
  if (demoElem.dateTime().now() != DateTime().now())
    return false;
  if (demoElem.payLoad() != "The good news is ...")
    return false;

  showHeader();
  showElem(demoElem);

  db["Fawcett"] = demoElem;
  dbp.db() = db;
  putLine();
  return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
  Utilities::title("Demonstrating Requirement #3b - creating DbCore");

  DbProvider dbp;
  DbCore<std::string> db = dbp.db();

  DbElement<std::string> demoElem = db["Fawcett"];

  demoElem.name("Ammar");
  demoElem.descrip("TA for CSE687");
  demoElem.payLoad("You should try ...");
  db["Salman"] = demoElem;
  if (!db.contains("Salman"))
    return false;

  demoElem.name("Jianan");
  demoElem.payLoad("Dr. Fawcett said ...");
  db["Sun"] = demoElem;

  demoElem.payLoad("You didn't demonstrate Requirement #4");
  demoElem.name("Nikhil");
  db["Prashar"] = demoElem;

  demoElem.payLoad("You didn't demonstrate Requirement #5");
  demoElem.name("Pranjul");
  db["Arora"] = demoElem;

  if (db.size() != 5)
    return false;

  std::cout << "\n  after adding elements with keys: ";
  Keys keys = db.keys();
  showKeys(db);
  putLine();

  std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
  db["Fawcett"].children().push_back("Salman");
  db["Fawcett"].children().push_back("Sun");
  db["Fawcett"].children().push_back("Prashar");
  db["Fawcett"].children().push_back("Arora");

  showHeader();
  showElem(db["Fawcett"]);

  db["Salman"].children().push_back("Sun");
  db["Salman"].children().push_back("Prashar");
  db["Salman"].children().push_back("Arora");

  // display the results

  putLine();
  std::cout << "\n  showing all the database elements:";
  showDb(db);
  putLine();

  std::cout << "\n  database keys are: ";
  showKeys(db);

  putLine();
  dbp.db() = db;
  return true;
}
//----< test R4 >------------------------------------------------------

bool testR4()
{
  Utilities::title("Demonstrating Requirement #4 - adding and deleteing records");

  DbProvider dbp;
  DbCore<std::string> db = dbp.db();

  showDb(db);
  Utilities::putline();

  Utilities::title("deleting \"Fawcett\" record");
  size_t dbCount = db.size();
  DbElement<std::string> save = db["Fawcett"];
  db.removeRecord("Fawcett");
  showDb(db);
  Utilities::putline();
  if (db.size() != dbCount - 1)
    return false;

  Utilities::title("adding back \"Fawcett\" record");
  db["Fawcett"] = save;
  showDb(db);

  Utilities::putline();
  return db.size() == dbCount;
}
//----< test part of R5 >----------------------------------------------

bool testR5()
{
  Utilities::title("Demonstrating Requirement #4 - adding and deleteing records");

  DbProvider dbp;
  DbCore<std::string> db = dbp.db();

  showDb(db);
  Utilities::putline();

  Key testKey = "foobar";
  Utilities::title("adding " + testKey + " child relationship to db[\"Fawcett\"]");
  db["Fawcett"].addChildKey(testKey);
  showDb(db);
  Utilities::putline();
  if (!db["Fawcett"].containsChildKey(testKey))
    return false;

  Utilities::title("removing " + testKey + "child relationship from db[\"Fawcett\"]");
  db["Fawcett"].removeChildKey(testKey);
  Utilities::putline();
  showDb(db);
  if (db["fawcett"].containsChildKey(testKey))
    return false;
  Utilities::putline();

  testKey = "Prashar";
  Utilities::title("finding parents of " + testKey);
  Parents parents = db.parents(testKey);
  showKeys(parents);
  Utilities::putline();

  Utilities::title("removing db[\"" + testKey + "\"] whose key is a child of other db elements");
  db.removeRecord(testKey);
  showDb(db);
  Utilities::putline();
  Keys keys = db.parents(testKey);
  return 0 == keys.size();
}
//----< test stub >----------------------------------------------------

using namespace Utilities;

int main()
{
  Utilities::Title("Testing DbCore - He said, she said database");
  putLine();

  TestExecutive ex;

  // define test structures with test function and message

  TestExecutive::TestStr ts1{ testR1, "Use C++11" };
  TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
  TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
  TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };
  TestExecutive::TestStr ts4{ testR4, "and and remove records" };
  TestExecutive::TestStr ts5{ testR5, "and and remove child relationships" };

  // register test structures with TestExecutive instance, ex

  ex.registerTest(ts1);
  ex.registerTest(ts2);
  ex.registerTest(ts3a);
  ex.registerTest(ts3b);
  ex.registerTest(ts4);
  ex.registerTest(ts5);

  // run tests

  bool result = ex.doTests();
  if (result == true)
    std::cout << "\n  all tests passed";
  else
    std::cout << "\n  at least one test failed";

  putLine(2);
  return 0;
}
#endif
