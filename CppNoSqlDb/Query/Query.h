#pragma once
///////////////////////////////////////////////////////////////////////
// Query.h - Returns information from db metadata and payload        //
// ver 1.1                                                           //
// Authoe: Yuxuan Xing                                               //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018 //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides classes:
*  - Conditions holds the conditions that define the data returned from a query
*  - Query provides methods: select, from, keys, and show
*
*  Required Files:
*  ---------------
*    Query.h, Query.cpp
*    PayLoad.h, PayLoad.cpp - application defined package
*    DbCore.h, DbCore.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 11 Mar 2018
*  - discovered while grading midterms that Query<P>::select(Conditions<P>&) and
*    template<typename CallObj> Query<P>::select(CallObj) were iterating over all
*    keys in db, not just the keys in the query.
*    This was a performance issue, not a correctness issue, now fixed.
*  - fixed potential race condition in 
*    template<typename CallObj> Query<P>::asyncSelect(CallObj)
*  Ver 1.0 : 10 Feb 2018
*  - first release
*
*/
#include "../DbCore/Definitions.h"
#include "../DbCore/DbCore.h"
#include <vector>
#include <string>
#include <regex>
#include <thread>
#include <future>

namespace NoSqlDb
{
  //using Key = std::string;
  //using Keys = std::vector<Key>;

  /////////////////////////////////////////////////////////////////////
  // Conditions class
  // - defines data returned by query

  template <typename P>
  class Conditions
  {
  public:
    using RegExp = const std::string&;

    Conditions() = default;
    void value(DbElement<P>& elem) { pDbElem_ = &elem; } // set pointer to element used during query

    bool match();
    void name(RegExp regExp) { nameRegExp_ = regExp; }    
    bool matchName();
    void description(RegExp regExp) { descriptionRegExp_ = regExp; }
    bool matchDescription();
    void lowerBound(const DateTime& lower) { lowerBound_ = lower; }
    void upperBound(const DateTime& upper = DateTime.now()) { upperBound_ = upper; }
    bool matchDateTimeInterval();
    void children(Keys keys) { keys_ = keys; }
    bool matchChildren();

    ///////////////////////////////////////////////////////////////
    // Not currently using the two functions, below.
    // Included, as I may decide to use them later.
    // template<typename CallObj>
    // void payLoadCallableObject(CallObj callObj) { callObj_ = callObj; }
    // template <typename CallObj>
    // bool matchPayLoad() 
    // { 
    //   return callObj_(pDbElem_->payLoad()); 
    // }
    ///////////////////////////////////////////////////////////////
  private:
    // DbElement<P> is referenced through pointer, not C++ reference type,
    // so the element can be changed at any time.
    DbElement<P>* pDbElem_ = nullptr;
    std::string nameRegExp_ = "";
    std::string descriptionRegExp_ = "";
    DateTime lowerBound_ = DateTime().now();
    DateTime upperBound_ = DateTime().now();
    Keys keys_;
    ///////////////////////////////////////////////////////////////
    // Not currently used, but may later.
    // template<typename CallObj>
    // static CallObj callObj_;
  };
  /*----< test element metadata for specified conditions >-----------*/

  template<typename P>
  bool Conditions<P>::match()
  {
    try
    {
      return matchName() && matchDescription() && matchChildren() && matchDateTimeInterval();
    }
    catch (std::exception& ex)
    {
      std::cout << "\n  exception thrown:\n  " << ex.what();
    }
    return false;
  }
  /*----< test element metadata for name match >---------------------*/

  template<typename P>
  bool Conditions<P>::matchName()
  {
    if (nameRegExp_ == "")
      return true;
    std::regex re(nameRegExp_);
    return std::regex_search(pDbElem_->name(), re);
  }
  /*----< test metadata for description match >----------------------*/

  template<typename P>
  bool Conditions<P>::matchDescription()
  {
    if (descriptionRegExp_ == "")
      return true;
    std::regex re(descriptionRegExp_);
    std::string desc = pDbElem_->descrip();
    return std::regex_search(desc, re);
  }
  /*----< test metadata for time interval match >--------------------*/

  template<typename P>
  bool Conditions<P>::matchDateTimeInterval()
  {
    if (lowerBound_ == upperBound_)
      return true;
    DateTime elemDateTime = pDbElem_->dateTime();
    bool aboveLower = lowerBound_ < elemDateTime;
    bool belowUpper = upperBound_ > elemDateTime;
    return aboveLower && belowUpper;
  }
  /*----< test metadata for children matches >-----------------------*/

  template<typename P>
  bool Conditions<P>::matchChildren()
  {
    if (keys_.size() == 0)
      return true;
    Keys::iterator start = pDbElem_->children().begin();
    Keys::iterator end = pDbElem_->children().end();
    for (Key key : keys_)
    {
      if (std::find(start, end, key) == end)
        return false;
    }
    return true;
  }
  
  /////////////////////////////////////////////////////////////////////
  // Query class

  template<typename P>
  class Query
  {
  public:
    Query() {}
    Query(DbCore<P>& db) : pDb_(&db) { keys_ = pDb_->keys(); }
    Query(const Query<P>& q);
    Query(Query<P>&& q);

    Query<P>& operator=(const Query<P>& q);
    Query<P>& operator=(Query<P>&& q);

    static void identify(std::ostream& out = std::cout);

    Query<P>& select(Conditions<P>& conds);

    template<typename CallObj>
    Query<P>& select(CallObj callObj);

    template<typename CallObj>
    std::future<Query<P>> asyncSelect(CallObj& callobj);

    Query<P>& query_or(Query<P>& q);

    Query<P>& from(const Keys& keys) { keys_ = keys; return *this; }

    void show(std::ostream& out = std::cout);

    Keys& keys() { return keys_; }

  private:
    DbCore<P>* pDb_;  // non-owning pointer to db
    Keys keys_;
  };
  //----< copy constructor >-------------------------------------------

  template<typename P>
  Query<P>::Query(const Query<P>& q)
  {
    keys_ = q.keys_;
    pDb_ = q.pDb_;   // non-owning so don't have to make src nullptr
  }
  //----< move constructor >-------------------------------------------

  template<typename P>
  Query<P>::Query(Query<P>&& q)
  {
    keys_ = q.keys_;
    pDb_ = q.pDb_;   // non-owning so don't have to make src nullptr
  }
  //----< copy assignment >--------------------------------------------

  template<typename P>
  Query<P>& Query<P>::operator=(const Query<P>& q)
  {
    if (this == &q) return *this;
    keys_ = q.keys_;
    pDb_ = q.pDb_;   // non-owning so just copy
    return *this;
  }
  //----< move assignment >--------------------------------------------

  template<typename P>
  Query<P>& Query<P>::operator=(Query<P>&& q)
  {
    if (this == &q) return *this;
    keys_ = q.keys_;
    pDb_ = q.pDb_;   // non-owning so don't have to make src nullptr
    return *this;
  }
  //----< show file name >---------------------------------------------

  template<typename P>
  void Query<P>::identify(std::ostream& out)
  {
    out << "\n  \"" << __FILE__ << "\"";
  }
  /*----< returns keys for elements that match conds >---------------*/

  template<typename P>
  Query<P>& Query<P>::select(Conditions<P>& conds)
  {
    Keys newKeys;
    for (auto key : keys_)  // searches current query key set
    {
      conds.value((*pDb_)[key]);
      if (conds.match())
        newKeys.push_back(key);
    }
    /////////////////////////////////////////////////////
    // code from ver 1.0 to show change
    //
    // for (auto item : *pDb_)  // searches entire db
    // {
    //   conds.value(item.second);
    //   if (conds.match())
    //     newKeys.push_back(item.first);
    // }

    keys_ = newKeys;
    return *this;
  }
  /*----< supports application defined queries for payload >---------*/
  /*
  *  - CallObj is defined by the application to return results from
  *    application's payload.  See test stub for example uses.
  */
  template<typename P>
  template<typename CallObj>
  Query<P>& Query<P>::select(CallObj callObj)
  {
    Keys newKeys;
    for (auto key : keys_)  // searches current keyset
    {
      if (callObj((*pDb_)[key]))
        newKeys.push_back(key);
    }
    ///////////////////////////////////////////
    // code from ver 1.0 to show change
    //
    // for (auto item : *pDb_)  // searches entire db
    // {
    //   if (callObj(item.second))
    //     newKeys.push_back(item.first);
    // }

    keys_ = newKeys;
    return *this;
  }
  /*----< supports application defined queries for payload >---------*/
  /*
  *  - CallObj is defined by the application to return results from
  *    application's payload.  This function selects asynchronously.
  */
  template<typename P>
  template<typename CallObj>
  std::future<Query<P>> Query<P>::asyncSelect(CallObj& callObj)
  {
    auto search = [&]()
    {
      Keys newKeys;
      for (Key key : keys_)
      {
        if (callObj((*pDb_)[key]))
          newKeys.push_back(key);
      }
      keys_ = newKeys;
      return *this;
    };
    return std::async(search);

    ////////////////////////////////////////////////////////
    // Original desgin:
    // This code demonstrates concern about race condition.
    //
    // std::future<Query<P>> Query<P>::asyncSelect(CallObj& callObj)
    // {
    //   std::future<Query<P>> f;
    //   std::thread t(
    //     [&]() {
    //       std::promise<Query<P>> p;
    //       std::future<Query<P>> f_thread(p.get_future());
    //       Keys newKeys;
    //       f = std::move(f_thread);  // race condition ??
    //       for (auto item : *pDb_)
    //       {
    //         if (callObj(item.second))
    //           newKeys.push_back(item.first);
    //       }
    //       keys_ = newKeys;
    //       Query<P> q(*pDb_);
    //       p.set_value(q.from(newKeys));
    //     }
    //   );
    //   t.detach();
    //   return f;
    // }
  }
  /*----< form union of keys with q.keys() >-------------------------*/

  template<typename P>
  Query<P>& Query<P>::query_or(Query<P>& q)
  {
    Keys keys = q.keys();
    for (auto key : keys)
    {
      Keys::iterator start = keys_.begin();
      Keys::iterator end = keys_.end();
      if(std::find(start, end, key) == end)
        keys_.push_back(key);
    }
    return *this;
  }
  /*----< displays query results >-----------------------------------*/
  /*
  *  - displays an element for each key in query keys
  */
  template<typename P>
  void Query<P>::show(std::ostream& out)
  {
    showHeader(showKey, out);
    for (auto key : keys_)
    {
      DbElement<P> temp = (*pDb_)[key];
      showRecord<P>(key, temp, out);
    }
  }
}