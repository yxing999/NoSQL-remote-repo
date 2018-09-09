#pragma once
/////////////////////////////////////////////////////////////////////////
// Message.h - defines message structure used in communication channel //
// ver 1.2                                                             //
// Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017        //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines an EndPoint struct and a Message class.  
*  - Endpoints define a message source or destination with an address and port number.
*  - Messages have an HTTP style structure with a set of attribute lines containing
*    name:value pairs.
*  - Message have a number of getter, setter methods for common attributes, and allow
*    definition of other "custom" attributes.
*
*  Required Files:
*  ---------------
*  Message.h, Message.cpp, Utilities.h, Utilities.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.2 : 27 Mar 2018
*  - added remove method to remove a message attribute, based on its key
*  ver 1.1 : 25 Mar 2018
*  - added method value(Key key) that returns value of attribute with key
*  ver 1.0 : 03 Oct 2017
*  - first release
*
*/
#include "../Utilities/Utilities.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace MsgPassingCommunication
{
  ///////////////////////////////////////////////////////////////////
  // EndPoint struct

  struct EndPoint
  {
    using Address = std::string;
    using Port = size_t;
    Address address;
    Port port;
    EndPoint(Address anAddress = "", Port aPort = 0);
    std::string toString();
    static EndPoint fromString(const std::string& str);
  };

  inline EndPoint::EndPoint(Address anAddress, Port aPort) : address(anAddress), port(aPort) {}

  inline std::string EndPoint::toString()
  {
    return address + ":" + Utilities::Converter<size_t>::toString(port);
  }

  inline EndPoint EndPoint::fromString(const std::string& str)
  {
    EndPoint ep;
    size_t pos = str.find_first_of(':');
    if (pos == str.length())
      return ep;
    ep.address = str.substr(0, pos);
    std::string portStr = str.substr(pos + 1);
    ep.port = Utilities::Converter<size_t>::toValue(portStr);
    return ep;
  }
  ///////////////////////////////////////////////////////////////////
  // Message class
  // - follows the style, but not the implementation details of
  //   HTTP messages

  class Message
  {
  public:
    using Key = std::string;
    using Value = std::string;
    using Attribute = std::string;
    using Attributes = std::unordered_map<Key, Value>;
    using Keys = std::vector<Key>;

    Message();
    Message(EndPoint to, EndPoint from);

    Attributes& attributes();
    void attribute(const Key& key, const Value& value);
    Keys keys();
    static Key attribName(const Attribute& attr);
    static Value attribValue(const Attribute& attr);
    bool containsKey(const Key& key);
    Value value(const Key& key);
    bool remove(const Key& key);

    EndPoint to();
    void to(EndPoint ep);
    EndPoint from();
    void from(EndPoint ep);
    std::string name();
    void name(const std::string& nm);
    std::string command();
    void command(const std::string& cmd);
    std::string file();
    void file(const std::string& fl);
    size_t contentLength();
    void contentLength(size_t ln);
    void clear();
    std::string toString();
    static Message fromString(const std::string& src);
    std::ostream& show(std::ostream& out = std::cout);

  private:
    Attributes attributes_;
    // name            : msgName
    // command         : msg Command
    // to              : dst EndPoint
    // from            : src EndPoint
    // file            : file name
    // content-length  : body length in bytes
    // custom attributes
  };
}