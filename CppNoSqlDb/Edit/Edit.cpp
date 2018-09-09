///////////////////////////////////////////////////////////////////////
// Edit.h - Edit metadata values                                     //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#ifdef TEST_EDIT

#include "Edit.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
using namespace NoSqlDb;

int main()
{
  Utilities::Title("Demonstrating DbElement Edits");

  DbElement<std::string> elem;
  elem.name("original name");
  elem.descrip("test element");
  elem.payLoad("elem payload");
  showElem(elem);

  Edit<std::string> edit(elem);
  edit.name("new name");
  edit.description("edited test element");
  DateTime& edt = edit.dateTime();
  edt -= DateTime::makeDuration(48, 0);
  edit.payLoad("edited elem payload");
  showElem(edit.DbElement());
  std::cout << "\n\n";
}
#endif