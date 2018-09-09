///////////////////////////////////////////////////////////////////////
// XmlHelpDemo.cpp - Demonstrates how to persist a simple db to XML  //
// Ver 1.1                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
*  - Creates XmlDocument from MockDb database
*  - Extracts Xml string representing database
*  - Restores db, e.g., builds new MockDb from Xml String
*  - You will need to persist to file and restore from file
* 
*  - To make this as simple as possible I modified XmlParser::createTextElement()
*    by trimming trailing whitespace from text.
*
*  Maintenance history:
*  --------------------
*  ver 1.1 : 02 Feb 18
*  - added demonstration of attribute handling
*  - attribute handling was completed in version 1.8 of XmlElement, today
*  ver 1.0 : 30 Jan 18
*  - first release
*/


#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"

///////////////////////////////////////////////////////////////////////
// MockDbElem class 
// - a simple element with fewer parts than those required for Project #1

class MockDbElem
{
public:
  std::string name() const 
  { 
    return name_; 
  }
  void name(const std::string& name) 
  { 
    name_ = name; 
  }
  std::string description() const 
  { 
    return description_; 
  }
  void description(const std::string& description) 
  { 
    description_ = description; 
  }
private:
  std::string name_;
  std::string description_;
};

///////////////////////////////////////////////////////////////////////
// MockDb class
// - a simple NoSql database - simpler than you need for Project #1

class MockDb
{
public:
  using Key = std::string;
  using DbStore = std::unordered_map<Key, MockDbElem>;
  using iterator = DbStore::iterator;

  bool contains(Key key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  MockDbElem& operator[](Key key) { return dbStore_[key]; }
  iterator begin() { return dbStore_.begin(); }
  iterator end() { return dbStore_.end(); }
private:
  DbStore dbStore_;
};

//----< display a single Db element >----------------------------------

void showElem(const MockDbElem& elem)
{
  std::cout << std::left;
  std::cout << std::setw(20) << elem.name();
  std::cout << std::setw(30) << elem.description();
}
//----< display an instance of a MockDb >------------------------------

void showDb(MockDb& db)
{
  std::cout << std::left << "\n";
  std::cout << std::setw(15) << "  key";
  std::cout << std::setw(20) << "  name";
  std::cout << std::setw(30) << "  description" << "\n";
  std::cout << std::setw(15) << " -------------";
  std::cout << std::setw(20) << " ------------------";
  std::cout << std::setw(30) << " ----------------------------";
  for (auto item : db)
  {
    std::cout << "\n  " << std::setw(15) << item.first;
    showElem(item.second);
  }
}

///////////////////////////////////////////////////////////////////////
// Demonstration of prototype persistance processing
// - You will need to build a Persistance package with a Persistance class.
// - You will break up the processing below into small, well-named methods.

using namespace XmlProcessing;

int main()
{
  std::cout << "\n  Demonstrating Mock Persistance with XmlDocument";
  std::cout << "\n =================================================\n";
  std::cout << "\n    version 1.1 adds attribute handling\n";

  std::cout << "\n  Building Mock Database";
  std::cout << "\n ------------------------";

  MockDb db;
  MockDbElem elem;
  elem.name("first elem");
  elem.description("test elem");
  db["first"] = elem;
  elem.name("second elem");
  db["second"] = elem;
  showDb(db);
  std::cout << "\n";

  std::cout << "\n  Creating XML representation of Mock Database using XmlDocument";
  std::cout << "\n ----------------------------------------------------------------";

  using Sptr = std::shared_ptr<AbstractXmlElement>;
  Sptr pDb = makeTaggedElement("db");
  pDb->addAttrib("type", "testDb");
  Sptr pDocElem = makeDocElement(pDb);
  XmlDocument xDoc(pDocElem);

  for (auto item : db)
  {
    Sptr pRecord = makeTaggedElement("dbRecord");
    pDb->addChild(pRecord);
    Sptr pKey = makeTaggedElement("key", item.first);
    pRecord->addChild(pKey);

    Sptr pValue = makeTaggedElement("value");
    pRecord->addChild(pValue);
    Sptr pName = makeTaggedElement("name", item.second.name());
    pValue->addChild(pName);
    Sptr pDescrip = makeTaggedElement("description", item.second.description());
    pValue->addChild(pDescrip);
  }
  std::string Xml = xDoc.toString();
  std::cout << Xml << std::endl;

  std::cout << "\n  Creating XmlDocument from XML string";
  std::cout << "\n --------------------------------------";

  MockDb newDb;
  XmlDocument newXDoc(Xml, XmlDocument::str);
  std::cout << newXDoc.toString();
  std::cout << "\n";
  std::vector<Sptr> found = newXDoc.element("db").select();
  if (found.size() > 0)
  {
    AbstractXmlElement::Attributes attribs = found[0]->attributes();
    if (attribs.size() > 0)
      std::cout << "\n  database type = " << "\"" << attribs[0].second << "\"\n";
  }
  std::cout << "\n  Building MockDb from XmlDocument";
  std::cout << "\n ----------------------------------";

  using Key = std::string;

  std::vector<Sptr> records = newXDoc.descendents("dbRecord").select();
  for (auto pRecord : records)
  {
    Key key;
    MockDbElem elem;
    std::vector<Sptr> pChildren = pRecord->children();
    for (auto pChild : pChildren)
    {
      // record's children are key and value

      if (pChild->tag() == "key")
      {
        // get value of TextElement child
        key = pChild->children()[0]->value();
      }
      else
      {
        // value represents a MockDbElem with children name and description

        std::vector<Sptr> pValueChildren = pChild->children();

        for (auto pValueChild : pValueChildren)
        {
          if (pValueChild->tag() == "name")
          {
            // get value of TextElement child
            elem.name(pValueChild->children()[0]->value());
          }
          if (pValueChild->tag() == "description")
          {
            // get value of TextElement child
            elem.description(pValueChild->children()[0]->value());
          }
        }
      }
    }
    newDb[key] = elem;
  }
  showDb(newDb);
  
  std::cout << "\n\n";
  return 0;
}

