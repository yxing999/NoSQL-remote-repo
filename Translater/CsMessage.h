#pragma once
///////////////////////////////////////////////////////////////////////
// CsMessage.h - Defines Message type for C# GUI                     //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  This C++\Cli Package contains three classes:
*  - Sutils:      defines methods to convert between managed strings and native strings
*  - CsEndPoint:  defines endpoint structurer, e.g., machineName and port
*  - CsMessage:   defines messages as a collection of attributes.
*
*  Required Files:
* -----------------
*  CsMessage.h, CsMessage.cpp
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 3/27/2018
*  - first release
*/
#include <string>
#include <iostream>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

namespace MsgPassingCommunication
{
  using Key = String;
  using Value = String;
  using Attributes = Dictionary<String^, String^>;

  public ref class Sutils
  {
  public:
    static std::string MtoNstr(String^ s);
    static String^ NtoMstr(const std::string& s);
  };
  //----< convert from managed to native string >----------------------

  std::string Sutils::MtoNstr(System::String^ str)
  {
    std::string temp;
    for (int i = 0; i < str->Length; ++i)
    {
      temp.push_back((char)str[i]);
    }
    return temp;
  }
  //----< convert from native to managed string >----------------------

  String^ Sutils::NtoMstr(const std::string& str)
  {
    StringBuilder^ sb = gcnew StringBuilder;
    for (auto ch : str)
    {
      sb->Append((wchar_t)ch);
    }
    return sb->ToString();
  }

  public ref class CsEndPoint
  {
  public:
    CsEndPoint()
    {
      machineAddress = gcnew String("");
      port = 0;
    }
    property String^ machineAddress;
    property int port;
    static String^ toString(CsEndPoint^ ep);
    static CsEndPoint^ fromString(String^ epStr);
  };
  //----< convert endpoint to string >---------------------------------

  String^ CsEndPoint::toString(CsEndPoint^ ep)
  {
    String^ epStr = ep->machineAddress + ":" + ep->port.ToString();
    return epStr;
  }
  //----< convert from string representing endpoint to endpoint >------

  CsEndPoint^ CsEndPoint::fromString(String^ epStr)
  {
    CsEndPoint^ ep = gcnew CsEndPoint;
    int pos = epStr->IndexOf(":");
    ep->machineAddress = epStr->Substring(0, pos);
    String^ portStr = epStr->Substring(pos + 1, epStr->Length - pos - 1);
    ep->port = System::Convert::ToInt32(portStr);
    return ep;
  }

  public ref class CsMessage
  {
  public:
    CsMessage() 
    {
      attributes = gcnew Attributes;
    }
    CsMessage(CsEndPoint to, CsEndPoint from)
    {
      attributes = gcnew Attributes;
      attributes->Add("to", to.machineAddress + ":" + to.port.ToString());
      attributes->Add("from", from.machineAddress + ":" + from.port.ToString());
    }
    void add(Key^ key, Value^ value)
    {
      attributes->Add(key, value);
    }
    Value^ value(Key^ key)
    {
      return attributes[key];
    }
    bool remove(Key^ key)
    {
      if (attributes->ContainsKey(key))
      {
        attributes->Remove(key);
        return true;
      }
      return false;
    }
    property Attributes^ attributes;

    void show()
    {
      Console::Write("\n  CsMessage:");
      auto enumer = attributes->GetEnumerator();
      while (enumer.MoveNext())
      {
        String^ key = enumer.Current.Key;
        String^ value = enumer.Current.Value;
        std::cout << "\n  { " << Sutils::MtoNstr(key) << ", " << Sutils::MtoNstr(value) << " }";
      }
      Console::Write("\n");
    }
  };
}