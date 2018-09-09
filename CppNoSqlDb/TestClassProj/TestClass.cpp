///////////////////////////////////////////////////////////////////////
// TestClass.cpp - defines all testing for NoSqlDb                   //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include "TestClass.h"
#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Persist/Persist.h"
#include "../Edit/Edit.h"
#include "../Executive/Executive.h"
#include <functional>

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
  Utilities::putline(n, out);
};
//----< make PayLoad Test database >-----------------------------------

DbCore<PayLoad> makeTestDb()
{
  DbCore<PayLoad> db;
  PayLoad pl;
  // first record
  pl.value() = "test value1";
  pl.categories().push_back("cat1");
  pl.categories().push_back("cat2");
  DbElement<PayLoad> elem;
  elem.name("elem1");
  elem.descrip("descrip1");
  elem.children().push_back("elem2");
  elem.children().push_back("elem3");
  elem.payLoad(pl);
  db["one"] = elem;

  // second record
  pl.value() = "test value2";
  pl.categories().clear();
  pl.categories().push_back("cat1");
  pl.categories().push_back("cat3");
  elem.name("elem2");
  elem.descrip("descrip2 - a very long and boring description of virtually nothing");
  elem.children().clear();
  elem.children().push_back("elem1");
  elem.children().push_back("elem3");
  elem.payLoad(pl);
  db["two"] = elem;

  // third record
  pl.value() = "test value3 with some extra not very interesting trivia";
  pl.categories().clear();
  pl.categories().push_back("cat2");
  elem.name("elem3");
  elem.descrip("descrip3");
  elem.children().clear();
  elem.children().push_back("elem2");
  elem.payLoad(pl);
  db["three"] = elem;

  return db;
}
//----< R1 >-----------------------------------------------------------
/*
*  - Demonstrate that solution uses C++11
*/
bool TestDb::testR1()
{
  Utilities::title("Demonstrating Requirement #1");
  std::cout << "\n  " << typeid(std::function<bool()>).name()
    << ", declared in this function, "
    << "\n  is only valid for C++11 and later versions.";
  return true; // would not compile unless C++11
}
//----< R2 >-----------------------------------------------------------
/*
*  - Cite use of streams and operators new and delete
*/
bool TestDb::testR2()
{
  Utilities::title("Demonstrating Requirement #2");
  std::cout << "\n  A visual examination of all the submitted code "
    << "will show only\n  use of streams and operators new and delete.";
  return true;
}
//----< R3a >----------------------------------------------------------
/*
*  - Demonstrate creation of DbElement
*/
bool TestDb::testR3a()
{
  Utilities::title("Demonstrating Requirement #3a - creating DbElement");
  std::cout << "\n  Creating a db element with key \"Fawcett\":";

  // create some demo elements and insert into db

  DbElement<PayLoad> demoElem;

  demoElem.name("Jim");
  demoElem.descrip("Instructor for CSE687");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad(PayLoad("The good news is ..."));

  if (demoElem.name() != "Jim")
    return false;
  if (demoElem.descrip() != "Instructor for CSE687")
    return false;
  if (demoElem.dateTime().now() != DateTime().now())
    return false;
  if (std::string(demoElem.payLoad()) != std::string("The good news is ..."))
    return false;

  showHeader(false);
  showElem(demoElem);

  db_["Fawcett"] = demoElem;
  return true;
}
//----< R3b >----------------------------------------------------------
/*
*  - Demonstrate creation of Database
*/
bool TestDb::testR3b()
{
  Utilities::title("Demonstrating Requirement #3b - creating DbCore");

  DbElement<PayLoad> demoElem = db_["Fawcett"];

  demoElem.name("Ammar");
  demoElem.descrip("TA for CSE687");
  demoElem.payLoad(std::string("You should try ..."));
  db_["Salman"] = demoElem;
  if (!db_.contains("Salman"))
    return false;

  demoElem.name("Jianan");
  demoElem.payLoad(std::string("Dr. Fawcett said ..."));
  db_["Sun"] = demoElem;

  demoElem.payLoad(std::string("You didn't demonstrate Requirement #4"));
  demoElem.name("Nikhil");
  db_["Prashar"] = demoElem;

  demoElem.payLoad(std::string("You didn't demonstrate Requirement #5"));
  demoElem.name("Pranjul");
  db_["Arora"] = demoElem;

  if (db_.size() != 5)
    return false;

  std::cout << "\n  after adding elements with keys: ";
  Keys keys = db_.keys();
  showKeys(db_);
  putLine();

  std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
  db_["Fawcett"].children().push_back("Salman");
  db_["Fawcett"].children().push_back("Sun");
  db_["Fawcett"].children().push_back("Prashar");
  db_["Fawcett"].children().push_back("Arora");

  showHeader(doNotShowKey);
  showElem(db_["Fawcett"]);

  db_["Salman"].children().push_back("Sun");
  db_["Salman"].children().push_back("Prashar");
  db_["Salman"].children().push_back("Arora");

  // display the results

  putLine();
  std::cout << "\n  showing all the database elements:";
  showDb(db_);
  putLine();

  std::cout << "\n  database keys are: ";
  showKeys(db_);

  return true;
}
//----< R4 >-----------------------------------------------------------
/*
*  - Demonstrate adding and deleting key-value pairs
*/
bool TestDb::testR4()
{
  Utilities::title("Demonstrating Requirement #4 - adding and deleteing records");

  showDb(db_);
  Utilities::putline();

  Utilities::title("deleting \"Fawcett\" record");
  size_t dbCount = db_.size();
  DbElement<PayLoad> save = db_["Fawcett"];
  db_.removeRecord("Fawcett");
  showDb(db_);
  Utilities::putline();
  if (db_.size() != dbCount - 1)
    return false;

  Utilities::title("adding back \"Fawcett\" record");
  db_["Fawcett"] = save;
  showDb(db_);

  return db_.size() == dbCount;
}
//----< R5a >----------------------------------------------------------
/*
*  - Demonstrate relationship editing
*/
bool TestDb::testR5a()
{
  Utilities::title("Demonstrating Requirement #5a - editing child relationships");

  showDb(db_);
  Utilities::putline();
  DbCore<PayLoad> save = db_;
  db_.throwOnIndexNotFound(true);

  Key testKey = "foobar";
  Utilities::title("adding " + testKey + " child relationship to db[\"Fawcett\"]");
  db_["Fawcett"].addChildKey(testKey);
  showDb(db_);
  Utilities::putline();
  if (!db_["Fawcett"].containsChildKey(testKey))
  {
    db_ = save;
    return false;
  }

  Utilities::title("removing " + testKey + "child relationship from db[\"Fawcett\"]");
  db_["Fawcett"].removeChildKey(testKey);
  Utilities::putline();
  showDb(db_);
  if (db_["Fawcett"].containsChildKey(testKey))
  {
    db_ = save;
    return false;
  }
  Utilities::putline();

  testKey = "Prashar";
  Utilities::title("finding parents of " + testKey);
  Parents parents = db_.parents(testKey);
  showKeys(parents);
  Utilities::putline();

  Utilities::title("removing db[\"" + testKey + "\"] whose key is a child of other db elements");
  db_.removeRecord(testKey);
  showDb(db_);
  Keys keys = db_.parents(testKey);
  db_ = save;
  return 0 == keys.size();
}
//----< R5b >----------------------------------------------------------
/*
*  - Demonstrate metadata editing
*/
bool TestDb::testR5b()
{
  Utilities::title("Demonstrating Requirement #5b - editing metadata");

  showDb(db_);
  Utilities::putline();

  Key key = "Fawcett";
  Utilities::title("editing \"" + key + "\" element");
  DbElement<PayLoad> elem = db_[key];
  showElem(elem);
  DbElement<PayLoad> save = elem;

  Edit<PayLoad> edit(elem);
  edit.name("James");
  edit.description("Instructor for OOD");
  edit.dateTime() -= DateTime::makeDuration(24 * 365 * 26, 0);
  edit.payLoad() = "The bad news is ...";
  edit.clearChildKeys();
  edit.addChildKey("Shafiq");
  edit.addChildKey("Santosh");
  showElem(edit.DbElement());
  if (save.name() == elem.name())
    return false;
  if (save.descrip() == elem.descrip())
    return false;
  if (save.dateTime() == elem.dateTime())
    return false;
  if (save.payLoad().value() == elem.payLoad().value())
    return false;
  if (save.children().size() == elem.children().size())
    return false;
  return true;
}
//----< R6 >-----------------------------------------------------------
/*
*  - Demonstrate metadata queries
*/
bool TestDb::testR6()
{
  Utilities::title("Demonstrating Requirement #6 - Metadata Queries");

  std::cout << "\n  creating default query, q1, and displaying:\n";
  Query<PayLoad> q1(db_);
  q1.show();  // complete db
  Utilities::putline();

  Keys saveKeys = q1.keys();
  std::cout << "\n  select on child keys { \"Prashar\" }\n";
  Keys keys{ "Prashar" };
  Conditions<PayLoad> conds0;
  conds0.children(keys);
  q1.select(conds0).show();
  Utilities::putline();

  Key aChild = "Arora";
  std::cout << "\n  select on child key \"" << aChild << "\"\n";

  /* need reference capture since we will change value in next use, below */
  
  auto findChild = [&aChild](DbElement<PayLoad>& elem) {
    for (auto child : elem.children())
    {
      if (child == aChild)
        return true;
    }
    return false;
  };
  q1.from(db_.keys()).select(findChild).show();
  Utilities::putline();

  aChild = "Salman";
  std::cout << "\n  selecting asynchronously on child key \"" << aChild << "\"";
  Utilities::putline();

  auto f = q1.from(db_.keys()).asyncSelect(findChild);

  /* It doesn't make sense to make blocking call to f.get() */
  /* unless we had some other work to do before calling.    */

  Query<PayLoad> qas = f.get();
  qas.show();
  Utilities::putline();

  std::cout << "\n  select on name containing \"Ji\" and description containing \"In\" or \"TA\"\n";
  q1.from(saveKeys);
  Conditions<PayLoad> conds1;
  conds1.name("Ji");
  conds1.description("(.*In)|(.*TA)");
  q1.select(conds1).show();
  Utilities::putline();

  DateTime dtlb, dtub;       // default time is now
  std::chrono::hours day(24);
  dtlb -= (365 * day);  // one year ago
  DbElement<PayLoad>& elem = db_["Fawcett"];
  DateTime newDt;
  newDt -= (2 * 365 * day);
  //newDt -= (10 * day);
  elem.dateTime(newDt);
  std::cout << "\n  select on time lowerbound = " << dtlb.time()
    << " and upperbound = " << dtub.time()
    << "\n  after changing time of \"Fawcett\" to " << newDt.time() << "\n";
  conds1.lowerBound(dtlb);
  conds1.upperBound(dtub);
  q1.select(conds1).show();
  return true;
}
//----< R7 >-----------------------------------------------------------
/*
*  - Demonstrate compound queries
*/
bool TestDb::testR7()
{
  Utilities::title("Demonstrating Requirement #7 - Compound Queries");

  Utilities::title("q1 selecting on name = \"Jim\"");
  Conditions<PayLoad> conds1;
  conds1.name("Jim");
  Query<PayLoad> q1(db_);
  q1.select(conds1).show();
  Utilities::putline();

  Utilities::title("q2 - selecting on description containing \"TA\"");
  Conditions<PayLoad> conds2;
  conds2.description("TA");
  Query<PayLoad> q2(db_);
  q2.select(conds2).show();
  Utilities::putline();

  Utilities::title("q2 = q1 or q2");
  q2.query_or(q1).show();
  Utilities::putline();

  Utilities::title("q2 = q2 and q1");
  q2.select(conds1).show();
  return true;
}
//----< R8 >-----------------------------------------------------------
/*
*  - Demonstrate persistance with sharding
*/
bool TestDb::testR8()
{
  Utilities::title("Demonstrating Requirement #8 - Persistance");
  Utilities::putline();

  Utilities::title("test database");
  DbCore<PayLoad> db = makeTestDb();
  showDb(db);
  Utilities::putline();

  Utilities::title("test database payload details");
  PayLoad::showDb(db);
  Utilities::putline();

  Persist<PayLoad> persist(db);
  Keys shard;
  shard.push_back("one");
  shard.push_back("two");
  persist.shard(shard);
  Utilities::title("shard keys are:");
  showKeys(shard);
  Utilities::putline();

  Xml xml = persist.toXml();
  Utilities::title("XML string representation of test database:");
  std::cout << xml;
  Utilities::putline();

  DbCore<PayLoad> save = db;
  Utilities::title("after rebuilding db from xml string");
  persist.fromXml(xml, rebuild);
  showDb(db);
  Utilities::putline();
  PayLoad::showDb(db);
  Utilities::putline();
  if (db.size() != 2)
    return false;

  db = save;
  Persist<PayLoad> persistWithAugment(db);
  Utilities::title("after augmenting db from xml string");
  persist.fromXml(xml, augment);
  showDb(db);
  Utilities::putline();
  PayLoad::showDb(db);
  if (db.size() != 3)
    return false;

  return true;
}
//----< R9 >-----------------------------------------------------------
/*
*  - Demonstrate payload queries
*/
bool TestDb::testR9()
{
  Utilities::title("Demonstrating Requirement #9 - PayLoad Queries");

  PayLoad pl;
  pl.categories().push_back("firstCategory");
  pl.categories().push_back("secondCategory");
  pl.value() = "Test Payload #1";
  db_["Fawcett"].payLoad() = pl;
  pl.categories().clear();
  pl.categories().push_back("firstCategory");
  pl.categories().push_back("thirdCategory");
  pl.value() = "Test Payload #2";
  db_["Salman"].payLoad() = pl;

  std::cout << "\n  db revised for payload tests:\n";
  PayLoad::showDb(db_);
  Utilities::putline();

  Query<PayLoad> q1(db_);

  std::string category = "thirdCategory";
  std::cout << "\n  select on payload categories for \"" << category << "\"\n";

  auto hasCategory = [&category](DbElement<PayLoad>& elem) {
    return (elem.payLoad()).hasCategory(category);
  };

  q1.select(hasCategory).show();

  std::string value = "Test Payload #1";
  auto hasValue = [&value](DbElement<PayLoad>& elem) {
    return (elem.payLoad()).value() == value;
  };
  Utilities::putline();

  std::cout << "\n  select on payload value \"" << value << "\"\n";
  q1.from(db_.keys());
  q1.select(hasValue).show();
  return true;
}
//----< R10 >----------------------------------------------------------
/*
*  - Demonstrate use of required packages
*/
bool TestDb::testR10()
{
  Utilities::Title("Demonstrating Requirement #10 - required packages");
  DbCore<PayLoad>::identify();
  Edit<PayLoad>::identify();
  Executive::identify();
  PayLoad::identify();
  Persist<PayLoad>::identify();
  Query<PayLoad>::identify();
  return true;
}
//----< R11 >----------------------------------------------------------
/*
*  - Demonstrate db holding solution structure
*/
DbCore<PayLoad> makePackageDb()
{
  DbCore<PayLoad> db;
  DbElement<PayLoad> elem = makeElement<PayLoad>("Exec", "deployer");
  elem.addChildKey("DateTime");
  elem.addChildKey("DbCore");
  elem.addChildKey("Edit");
  elem.addChildKey("PayLoad");
  elem.addChildKey("Persist");
  elem.addChildKey("Query");
  elem.addChildKey("TestClass");
  elem.addChildKey("StringUtilities");
  db.addRecord("Exec", elem);

  elem = makeElement<PayLoad>("DbCore", "base db functionality");
  elem.clearChildKeys();
  elem.addChildKey("DateTime");
  elem.addChildKey("PayLoad");
  elem.addChildKey("StringUtilities");
  db.addRecord("DbCore", elem);

  elem = makeElement<PayLoad>("Edit", "edit db elements");
  elem.clearChildKeys();
  elem.addChildKey("DateTime");
  elem.addChildKey("DbCore");
  elem.addChildKey("PayLoad");
  elem.addChildKey("Query");
  elem.addChildKey("StringUtilities");
  db.addRecord("Edit", elem);

  elem = makeElement<PayLoad>("PayLoad", "application specific data");
  elem.clearChildKeys();
  elem.addChildKey("DbCore");
  elem.addChildKey("StringUtilities");
  db.addRecord("PayLoad", elem);

  elem = makeElement<PayLoad>("Persist", "save and restore db");
  elem.clearChildKeys();
  elem.addChildKey("DateTime");
  elem.addChildKey("DbCore");
  elem.addChildKey("PayLoad");
  elem.addChildKey("Query");
  elem.addChildKey("StringUtilities");
  elem.addChildKey("XmlDocument");
  elem.addChildKey("XmlElement");
  db.addRecord("Persist", elem);

  elem = makeElement<PayLoad>("Query", "search for db information");
  elem.clearChildKeys();
  elem.addChildKey("DbCore");
  elem.addChildKey("PayLoad");
  elem.addChildKey("StringUtilities");
  db.addRecord("Query", elem);

  elem = makeElement<PayLoad>("TestClass", "test db functionality");
  elem.clearChildKeys();
  elem.addChildKey("DateTime");
  elem.addChildKey("DbCore");
  elem.addChildKey("Edit");
  elem.addChildKey("PayLoad");
  elem.addChildKey("Persist");
  elem.addChildKey("Query");
  elem.addChildKey("TestClass");
  elem.addChildKey("StringUtilities");
  db.addRecord("TestClass", elem);

  elem = makeElement<PayLoad>("StringUtilities", "string helpers");
  elem.clearChildKeys();
  db.addRecord("StringUtilities", elem);

  return db;
}

bool TestDb::testR11()
{
  Utilities::Title("Demonstrate Requirement #11 -  Solution Structure");
  DbCore<PayLoad> db = makePackageDb();
  showDb(db);
  return true;
}
//----< R12 >----------------------------------------------------------
/*
*  - Demonstrate use of Executive package
*  - This should support deployment
*/
bool TestDb::testR12()
{
  Utilities::Title("Demonstrate Requirement #12 - Executive Package");
  
  std::cout << "\n  You will find the executive package here:";
  Executive::identify();
  Utilities::putline();

  std::cout << "\n  Executive is used to demonstrate a deployable structure.";
  std::cout << "\n  Demonstration of requirements is delegated to:";
  TestDb::identity();

  return true;
}
//----< R13 >----------------------------------------------------------
/*
*  - Display pdf of package structure
*/
#include <sstream>

bool TestDb::testR13()
{
  Utilities::Title("Demonstrate Requirement #13 - Solution Structure PDF");

  std::cout << "\n  You will find a package diagram for the NoSqlDb here:";
  std::ostringstream out;
  Executive::identify(out);
  std::string path = out.str();
  size_t pos = path.find_last_of("\\");
  path = path.substr(3, pos-3);
  std::cout << "\n    " << path << std::endl;

  return true;
}
//----< implement requirements testing >-------------------------------

int main()
{
  std::cout << "\n  Testing DbCore<PayLoad>";
  std::cout << "\n =========================";

  NoSqlDb::DbCore<PayLoad> db;
  TestDb tdb(db);
  tdb.invoke(&TestDb::testR1);
  tdb.invoke(&TestDb::testR2);
  tdb.invoke(&TestDb::testR3a);
  tdb.invoke(&TestDb::testR3b);
  tdb.invoke(&TestDb::testR4);
  tdb.invoke(&TestDb::testR5a);
  tdb.invoke(&TestDb::testR5b);
  tdb.invoke(&TestDb::testR6);
  tdb.invoke(&TestDb::testR7);
  tdb.invoke(&TestDb::testR8);
  tdb.invoke(&TestDb::testR9);
  tdb.invoke(&TestDb::testR10);
  tdb.invoke(&TestDb::testR11);
  tdb.invoke(&TestDb::testR12);
  tdb.invoke(&TestDb::testR13);
  std::cout << "\n\n";
}