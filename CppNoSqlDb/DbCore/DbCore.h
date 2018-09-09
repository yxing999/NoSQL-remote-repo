#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.2														   //
// Yuxuan Xing													   //
// Source:Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018 //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.3 : 17 Feb 2018
* - added containsChildKey, addChildKey, and removeChildKey to DbElement<P>
* - added addRecord, removeRecord, and parents to DbCore<P>
* ver 1.2 : 10 Feb 2018
* - moved definition of Key, Keys, and Children to NoSqlDb namespace scope
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Definitions.h"
#include "../DateTime/DateTime.h"

namespace NoSqlDb
{
  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

  template<typename P>
  class DbElement
  {
  public:

    DbElement() {}

    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }
   
    bool containsChildKey(const Key& key);
    bool addChildKey(const Key& key);
    bool removeChildKey(const Key& key);
    void clearChildKeys() { children_.clear(); }

    P& payLoad() { return payLoad_; }
    P payLoad() const { return payLoad_; }
    void payLoad(const P& payLoad) { payLoad_ = payLoad; }

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
    P payLoad_;
  };
  //----< does children collection contain key? >----------------------

  template<typename P>
  bool DbElement<P>::containsChildKey(const Key& key)
  {
    Keys::iterator start = children_.begin();
    Keys::iterator end = children_.end();
    return std::find(start, end, key) != end;
  }
  //----< add key to children collection >-----------------------------

  template<typename P>
  bool DbElement<P>::addChildKey(const Key& key)
  {
    if (!containsChildKey(key))
    {
      children_.push_back(key);
      return true;
    }
    else
    {
      return false;
    }
  }
  //----< remove child key from children collection >------------------

  template<typename P>
  bool DbElement<P>::removeChildKey(const Key& key)
  {
    size_t keyCount = children_.size();
    std::vector<Key>::iterator start = children_.begin();
    std::vector<Key>::iterator end = children_.end();
    std::vector<Key>::iterator found = std::find(start, end, key);
    if (found == end)
      return false;
    found = children_.erase(found);
    return keyCount == children_.size() + 1;
  }
  //----< display key set >--------------------------------------------

  inline void showKeys(Keys keys, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : keys)
    {
      out << key << " ";
    }
  }
  //----< make DbElement >---------------------------------------------

  template <typename P>
  DbElement<P> makeElement(const std::string& name, const std::string& descrip = "")
  {
    DbElement<P> elem;
    elem.name(name);
    elem.descrip(descrip);
    return elem;
  }
  
  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename P>
  class DbCore
  {
  public:
    using DbStore = std::unordered_map<Key,DbElement<P>>;
    using iterator = typename DbStore::iterator;

    static void identify(std::ostream& out = std::cout);

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
    size_t size();
    void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
    DbElement<P>& operator[](const Key& key);
    DbElement<P> operator[](const Key& key) const;
    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }
    bool addRecord(const Key& key, const DbElement<P>& elem);
    bool removeRecord(const Key& key);
    Parents parents(const Key& key);
  private:
    DbStore dbStore_;
    bool doThrow_ = false;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore<P> methods

  //----< show file name >---------------------------------------------

  template<typename P>
  void DbCore<P>::identify(std::ostream& out)
  {
    out << "\n  \"" << __FILE__ << "\"";
  }
  //----< does db contain this key? >----------------------------------

  template<typename P>
  bool DbCore<P>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  //----< returns current key set for db >-----------------------------

  template<typename P>
  typename Keys DbCore<P>::keys()
  {
    Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename P>
  size_t DbCore<P>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  *  - In order to create a key-value pair in the database like this:
  *      db[newKey] = newDbElement
  *    we need to index with the new key and assign a default element. 
  *    That will be replaced by newDbElement when we return from operator[]
  *  - However, if we want to index without creating new elements, we need
  *    to throw an exception if the key does not exist in the database.
  *  - The behavior we get is determined by doThrow_.  If false we create
  *    a new element, if true, we throw. Creating new elements is the default
  *    behavior.
  */
  template<typename P>
  DbElement<P>& DbCore<P>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      if (doThrow_)
        throw(std::exception("key does not exist in db"));
      else
        return (dbStore_[key] = DbElement<P>());
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename P>
  DbElement<P> DbCore<P>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      throw(std::exception("key does not exist in db"));
    }
    return dbStore_[key];
  }
  //----< adds database record if key does not exist >-----------------

  template<typename P>
  bool DbCore<P>::addRecord(const Key& key, const DbElement<P>& elem)
  {
    if (contains(key))
      return false;
    dbStore_[key] = elem;
    return true;
  }
  //----< removes database record if key exists >----------------------

  template<typename P>
  bool DbCore<P>::removeRecord(const Key& key)
  {
    size_t numErased = dbStore_.erase(key);
    Parents parents = this->parents(key);
    for (auto dbKey : parents)
    {
      dbStore_[dbKey].removeChildKey(key);
    }
    return numErased > 0;
  }
  //----< find all parents of record index by key >--------------------

  template<typename P>
  Parents DbCore<P>::parents(const Key& key)
  {
    Parents parents;
    for (auto item : dbStore_)
    {
      if (item.second.containsChildKey(key))
        parents.push_back(item.first);
    }
    return parents;
  }

  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename P>
  void showKeys(DbCore<P>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------
  /*
  * - will use definitions (Definitions.h): 
  *     showKey = true
  *     doNotShowKey = false
  *   for first argument
  */
  inline void showHeader(bool showKey = true, std::ostream& out = std::cout)
  {
    out << "\n  ";
    if(showKey)
      out << std::setw(15) << std::left << "Key";
    out << std::setw(15) << std::left << "Name";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(20) << std::left << "Description";
    out << std::setw(20) << std::left << "Payload";
    out << "\n  ";
    if(showKey)
      out << std::setw(15) << std::left << "-------------";
    out << std::setw(15) << std::left << "-------------";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(20) << std::left << "------------------";
    out << std::setw(20) << std::left << "------------------";
  }
  //----< display DbElements >-----------------------------------------

  template<typename P>
  void showElem(const DbElement<P>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(15) << std::left << el.name().substr(0, 13);
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(20) << std::left << el.descrip().substr(0, 18);
    out << std::setw(20) << std::left << std::string(el.payLoad()).substr(0, 18);
    Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }
  }
  //----< display Db records >-----------------------------------------

  template<typename P>
  void showRecord(const Key& key, const DbElement<P>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(15) << std::left << key.substr(0, 13);
    out << std::setw(15) << std::left << el.name().substr(0, 13);
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(20) << std::left << el.descrip().substr(0, 18);
    out << std::setw(20) << std::left << std::string(el.payLoad()).substr(0, 18);
    Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }
  }
  //----< display all records in database >----------------------------

  template<typename P>
  void showDb(const DbCore<P>& db, std::ostream& out = std::cout)
  {
    showHeader(true, out);
    typename DbCore<P>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showRecord(item.first, item.second, out);
    }
  }
}
