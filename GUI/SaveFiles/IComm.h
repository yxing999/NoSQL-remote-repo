#pragma once
/////////////////////////////////////////////////////////////////////
// IComm.h - interface for message-passing communication facility  //
// ver 1.0                                                         //
// Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017    //
/////////////////////////////////////////////////////////////////////

#include <string>
#include "../Message/Message.h"

namespace MsgPassingCommunication
{
  class IComm
  {
  public:
    static IComm* create(const std::string& machineAddress, size_t port);
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void postMessage(Message msg) = 0;
    virtual Message getMessage() = 0;
    virtual std::string name() = 0;
    virtual ~IComm() {}
  };
}