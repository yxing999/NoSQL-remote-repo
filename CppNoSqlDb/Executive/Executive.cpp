///////////////////////////////////////////////////////////////////////
// Executive.cpp - used to set up deployment process                 //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
* - This project is compiled as a static library after setting its property
*   C/C++ > Proprocessor > Preprocessor Definitions to noTEST_EXECUTIVE
* - Clients will include NoSqlDb.h, their own PayLoad.h, and link with
*   Executive.lib by making a reference to the Executive project.
*/
#ifdef TEST_EXECUTIVE

//#include "NoSqlDb.h"
#include "Executive.h"
#include "../PayLoad/PayLoad.h"

using namespace NoSqlDb;

/*
* - This code is an example of how a client will use the NoSqlDb library.
*/
int main()
{
  Utilities::Title("Demonstrating Executive - designed to support deployment");
  Utilities::putline();

  Utilities::title("making demo database");
  DbCore<PayLoad> db;

  DbElement<PayLoad> elem = makeElement<PayLoad>("a name", "a description");
  PayLoad pl;
  pl.value("a payload value");
  pl.categories().push_back("some category");
  elem.payLoad(pl);
  db["first"] = elem;

  std::string name2 = "another name";
  elem.name(name2);
  pl.value("another payload value");
  pl.categories().push_back("some other category");
  elem.payLoad(pl);
  db["second"] = elem;

  showDb(db);
  Utilities::putline();

  elem.payLoad().showDb(db);
  Utilities::putline();

  Utilities::title("demonstrating query");
  Query<PayLoad> q(db);
  q.select(
    [=](DbElement<PayLoad>& elem) { 
      if (elem.name() == name2)
      {
        std::cout << "\n  " << elem.name();
        return true;
      }
      return false;
    }
  ).show();
  Utilities::putline();

  Utilities::title("demonstrating persistance - to XML string");
  Persist<PayLoad> persist(db);
  std::string xml = persist.toXml();
  std::cout << xml;
  Utilities::putline();

  Utilities::title("demonstrating persistance - db from XML string");
  persist.fromXml(xml);
  showDb(db);
  
  std::cout << "\n\n";
  return 0;
}

#endif
