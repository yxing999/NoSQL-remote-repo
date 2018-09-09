///////////////////////////////////////////////////////////////////////
// Query.cpp - Returns information from db metadata and payload      //
// ver 1.1                                                           //
// Author :Yuxuna Xing                                               //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018 //
///////////////////////////////////////////////////////////////////////

/*----< test stub >--------------------------------------------------*/

#ifdef TEST_QUERY

#include <chrono>
#include "Query.h"
#include "../PayLoad/PayLoad.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace NoSqlDb;

//----< demo first part of requirement #3 >----------------------------

bool testR3a(DbCore<PayLoad>& db)
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

  showHeader();
  showElem(demoElem);

  db["Fawcett"] = demoElem;
  Utilities::putline();
  return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b(DbCore<PayLoad>& db)
{
  Utilities::title("Demonstrating Requirement #3b - creating DbCore");

  DbElement<PayLoad> demoElem = db["Fawcett"];

  demoElem.name("Ammar");
  demoElem.descrip("TA for CSE687");
  demoElem.payLoad(PayLoad("You should try ..."));
  db["Salman"] = demoElem;
  if (!db.contains("Salman"))
    return false;

  demoElem.name("Jianan");
  demoElem.payLoad(PayLoad("Dr. Fawcett said ..."));
  db["Sun"] = demoElem;

  demoElem.payLoad(PayLoad("You didn't demonstrate Requirement #4"));
  demoElem.name("Nikhil");
  db["Prashar"] = demoElem;

  demoElem.payLoad(PayLoad("You didn't demonstrate Requirement #5"));
  demoElem.name("Pranjul");
  db["Arora"] = demoElem;

  if (db.size() != 5)
    return false;

  std::cout << "\n  after adding elements with keys: ";
  Keys keys = db.keys();
  showKeys(db);
  Utilities::putline();

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

  Utilities::putline();
  std::cout << "\n  showing all the database elements:";
  showDb(db);
  Utilities::putline();

  std::cout << "\n  database keys are: ";
  showKeys(db);

  Utilities::putline();
  return true;
}
//----< demo Requirement #6 >------------------------------------------

bool testR6(DbCore<PayLoad>& db)
{
  Utilities::title("Demonstrating Requirement #6 - Metadata Queries");

  std::cout << "\n  creating default query, q1, and displaying:\n";
  Query<PayLoad> q1(db);
  q1.show();  // complete db
  Utilities::putline();

  Keys saveKeys = q1.keys();
  std::cout << "\n  select on child keys { \"Prashar\" }\n";
  Keys keys{ "Prashar" };
  Conditions<PayLoad> conds0;
  conds0.children(keys);
  q1.select(conds0).show();
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
  DbElement<PayLoad>& elem = db["Fawcett"];
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
  Utilities::putline();
  return true;
}

bool testR7(DbCore<PayLoad>& db)
{
  Utilities::title("Demonstrating Requirement #7 - Compound Queries");
  Utilities::putline();

  Utilities::title("q1 selecting on name = \"Jim\"");
  Conditions<PayLoad> conds1;
  conds1.name("Jim");
  Query<PayLoad> q1(db);
  q1.select(conds1).show();
  Utilities::putline();

  Utilities::title("q2 - selecting on description containing \"TA\"");
  Conditions<PayLoad> conds2;
  conds2.description("TA");
  Query<PayLoad> q2(db);
  q2.select(conds2).show();
  Utilities::putline();

  Utilities::title("q2 = q1 or q2");
  q2.query_or(q1).show();
  Utilities::putline();

  Utilities::title("q2 = q2 and q1");
  q2.select(conds1).show();
  Utilities::putline();
  return true;
}
bool testR9(DbCore<PayLoad>& db)
{
  Utilities::title("Demonstrating Requirement #9 - PayLoad Queries");

  PayLoad pl;
  pl.categories().push_back("firstCategory");
  pl.categories().push_back("secondCategory");
  pl.value() = "Test Payload #1";
  db["Fawcett"].payLoad() = pl;
  pl.categories().clear();
  pl.categories().push_back("firstCategory");
  pl.categories().push_back("thirdCategory");
  pl.value() = "Test Payload #2";
  db["Salman"].payLoad() = pl;

  std::cout << "\n  db revised for payload tests:\n";
  PayLoad::showDb(db);
  Utilities::putline();

  Query<PayLoad> q1(db);

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
  q1.from(db.keys());
  q1.select(hasValue).show();
  Utilities::putline();

  std::cout << "\n  asynchronous select on payload value \"" << value << "\"\n";
  q1.from(db.keys());
  std::future<Query<PayLoad>> fut = q1.asyncSelect(hasValue);
  // do some useful work here
  q1 = fut.get();
  q1.show();
  return true;
}

int main()
{
  Utilities::Title("Demonstrating Query Package");

  DbCore<PayLoad> db;

  testR3a(db);
  testR3b(db);
  //DbCore<PayLoad> dbSave = db;
  testR6(db);
  testR7(db);
  testR9(db);

  std::cout << "\n\n";
  getchar();
  return 0;
}
#endif

