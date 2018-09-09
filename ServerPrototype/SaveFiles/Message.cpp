///////////////////////////////////////////////////////////////////////////
// Message.cpp - defines message structure used in communication channel //
// ver 1.2                                                               //
// Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017          //
///////////////////////////////////////////////////////////////////////////

#include "Message.h"
#include <iostream>

using namespace MsgPassingCommunication;
using SUtils = Utilities::StringHelper;

//----< default constructor results in Message with no attributes >----

Message::Message() {}

//----< constructor accepting dst and src addresses >------------------

Message::Message(EndPoint to, EndPoint from)
{
  attributes_["to"] = to.toString();
  attributes_["from"] = from.toString();
}
//----< returns reference to Message attributes >----------------------

Message::Attributes& Message::attributes()
{
  return attributes_;
}
//----< adds or modifies an existing attribute >-----------------------

void Message::attribute(const Key& key, const Value& value)
{
  attributes_[key] = value;
}
//----< clears all attributes >----------------------------------------

void Message::clear()
{
  attributes_.clear();
}
//----< returns vector of attribute keys >-----------------------------

Message::Keys Message::keys()
{
  Keys keys;
  keys.reserve(attributes_.size());
  for (auto kv : attributes_)
  {
    keys.push_back(kv.first);
  }
  return keys;
}
//---< does this message have key? >-----------------------------------

bool Message::containsKey(const Key& key)
{
  if (attributes_.find(key) != attributes_.end())
    return true;
  return false;
}
//----< remove attribute with this key >-------------------------------

bool Message::remove(const Key& key)
{
  if (containsKey(key))
  {
    attributes_.erase(key);
    return true;
  }
  return false;
}
//----< return value of specified key >--------------------------------

Message::Value Message::value(const Key& key)
{
  for (auto item : attributes_)
  {
    if (item.first == key)
      return item.second;
  }
  return "";
}
//----< get to attribute >---------------------------------------------

EndPoint Message::to()
{
  if (containsKey("to"))
  {
    return EndPoint::fromString(attributes_["to"]);
  }
  return EndPoint();
}
//----< set to attribute >---------------------------------------------

void Message::to(EndPoint ep)
{
  attributes_["to"] = ep.toString();
}
//----< get from attribute >-------------------------------------------

EndPoint Message::from()
{
  if (containsKey("from"))
  {
    return EndPoint::fromString(attributes_["from"]);
  }
  return EndPoint();
}
//----< set from attribute >-------------------------------------------

void Message::from(EndPoint ep)
{
  attributes_["from"] = ep.toString();
}
//----< get name attribute >-------------------------------------------

std::string Message::name()
{
  if (containsKey("name"))
  {
    return attributes_["name"];
  }
  return "";
}
//----< set name attribute >-------------------------------------------

void Message::name(const std::string& nm)
{
  attributes_["name"] = nm;
}
//----< get command attribute >----------------------------------------

std::string Message::command()
{
  if (containsKey("command"))
  {
    return attributes_["command"];
  }
  return "";
}
//----< set command attribute >----------------------------------------

void Message::command(const std::string& cmd)
{
  attributes_["command"] = cmd;
}
//----< get file name attribute >--------------------------------------

std::string Message::file()
{
  if (containsKey("file"))
  {
    return attributes_["file"];
  }
  return "";
}
//----< set file name attribute >--------------------------------------

void Message::file(const std::string& fl)
{
  attributes_["file"] = fl;
}
//----< get body length >----------------------------------------------

size_t Message::contentLength()
{
  if (containsKey("content-length"))
  {
    std::string lenStr = attributes_["content-length"];
    return Utilities::Converter<size_t>::toValue(lenStr);
  }
  return 0;
}
//----< set body length >----------------------------------------------

void Message::contentLength(size_t ln)
{
  attributes_["content-length"] = Utilities::Converter<size_t>::toString(ln);
}
//----< convert message to string representation >---------------------

std::string Message::toString()
{
  std::string temp;
  for (auto kv : attributes_)
  {
    temp += kv.first + ":" + kv.second + "\n";
  }
  return temp + "\n";
}
//----< extracts name from attribute string >--------------------------

Message::Key Message::attribName(const Attribute& attrib)
{
  size_t pos = attrib.find_first_of(':');
  if (pos == attrib.length())
    return "";
  return attrib.substr(0, pos);
}
//----< extracts value from attribute string >-------------------------

Message::Value Message::attribValue(const Attribute& attrib)
{
  size_t pos = attrib.find_first_of(':');
  if (pos == attrib.length())
    return "";
  return attrib.substr(pos + 1, attrib.length() - pos);
}
//----< creates message from message representation string >-----------

Message Message::fromString(const std::string& src)
{
  Message msg;
  std::vector<std::string> splits = Utilities::StringHelper::split(src);
  for (Attribute attr : splits)
  {
    if(attribName(attr) != "")
      msg.attributes()[attribName(attr)] = attribValue(attr);
  }
  return msg;
}
//----< displays message on std::ostream >-----------------------------
/*
*  - adds beginning newline and removes trailing newline
*  - by default stream is std::cout
*  - can be replaced by std::ostringstream to get display string
*/
std::ostream& Message::show(std::ostream& out)
{
  std::string temp = toString();  // convert this message to string
  size_t pos = temp.find_last_of('\n');
  if (pos < temp.size())
  {
    temp[pos] = '\0';  // remove last newline
  }
  out << "\n" << temp; // prepend newline
  return out;
}
//----< test stub >----------------------------------------------------

#ifdef TEST_MESSAGE

int main()
{
  SUtils::Title("Testing Message Class");

  SUtils::title("testing endpoints");
  EndPoint ep("localhost", 8080);
  std::cout << "\n  address = " << ep.address;
  std::cout << "\n  port = " << ep.port;
  std::string epStr = ep.toString();
  std::cout << "\n  " << epStr;

  EndPoint newEp = EndPoint::fromString(epStr);
  std::cout << "\n  " << newEp.toString();
  Utilities::putline();

  SUtils::title("testing messages");
  Utilities::putline();

  SUtils::title("creating message from Message::methods");
  Message msg;
  msg.name("msg#1");
  msg.to(EndPoint("localhost", 8080));
  msg.from(EndPoint("localhost", 8081));
  msg.command("doIt");
  msg.bodyLength(42);
  msg.file("someFile");
  msg.show();

  SUtils::title("testing Message msg = fromString(msg.toString())");
  Message newMsg = Message::fromString(msg.toString());
  newMsg.show();

  SUtils::title("retrieving attributes from message");
  std::cout << "\n  msg name          : " << newMsg.name();
  std::cout << "\n  msg command       : " << newMsg.command();
  std::cout << "\n  msg to            : " << newMsg.to().toString();
  std::cout << "\n  msg from          : " << newMsg.from().toString();
  std::cout << "\n  msg file          : " << newMsg.file();
  std::cout << "\n  msg content-Length: " << newMsg.contentLength();
  Utilities::putline();

  SUtils::title("adding custom attribute");
  newMsg.attribute("customName", "customValue");
  newMsg.show();

  SUtils::title("testing assignment");
  Message srcMsg;
  srcMsg.name("srcMsg");
  srcMsg.attribute("foobar", "feebar");
  srcMsg.show();
  std::cout << "\n  assigning srcMsg to msg #1";
  newMsg = srcMsg;
  newMsg.show();

  std::cout << "\n\n";
  return 0;
}
#endif
