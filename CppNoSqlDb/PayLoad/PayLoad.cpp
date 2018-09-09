///////////////////////////////////////////////////////////////////////
// PayLoad.h - application defined payload                           //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include "PayLoad.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace NoSqlDb;
using namespace XmlProcessing;

#ifdef TEST_PAYLOAD

int main()
{
  Utilities::Title("Demonstrating Application Specific PayLoad class");
  Utilities::putline();

  using Sptr = std::shared_ptr<AbstractXmlElement>;

  Utilities::title("creating xml string from payload instance");
  PayLoad pl;
  pl.value("demo payload value");
  pl.categories().push_back("cat1");
  pl.categories().push_back("cat2");
  Sptr sPpl = pl.toXmlElement();
  XmlDocument doc(sPpl);
  std::cout << doc.toString();
  Utilities::putline();

  Utilities::title("creating payload instance from an XmlElement");
  PayLoad newPl = pl.fromXmlElement(sPpl);
  std::cout << "\n  payload value = " << pl.value();
  std::cout << "\n  payload categories:\n    ";
  for (auto cat : newPl.categories())
  {
    std::cout << cat << " ";
  }
  std::cout << "\n\n";
}
#endif
