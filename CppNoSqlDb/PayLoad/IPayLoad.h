#pragma once
///////////////////////////////////////////////////////////////////////
// IPayLoad.h - declare language for serializing a PayLoad instance  //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
*  Note:
*  - It is common practice to define C++ interfaces as structs with
*    pure virtual methods, including a virtual destructor.
*  - If a class derives from IPayLoad and fails to implement the
*    pure virtual method toXmlElement, the class will be abstract
*    and compilation will fail for statements that declare the PayLoad.
*  - static methods cannot be virtual, and we need fromXmlElement to
*    be static, because we may not have a PayLoad instance to invoke
*    it when deserializing.
*  - IPayLoad defers implementation to the PayLoad class.  If that
*    class fails to implement the method, builds will fail to link.
*/
namespace NoSqlDb
{
#include "../XmlDocument/XmlElement/XmlElement.h"

  using Xml = std::string;
  using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

  template<typename P>
  struct IPayLoad
  {
    virtual Sptr toXmlElement() = 0;
    static P fromXmlElement(Sptr elem);
    virtual ~IPayLoad() {};
  };
}