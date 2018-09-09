#pragma once

/////////////////////////////////////////////////////////////////////
// Checkout.h -     Code Repository                                //
// ver 1.0                                                         //
// YUXUAN XING CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package
* Required Files:
* ---------------
* Definitions.h
* RepositoryCore.h
* Version.h
* Maintenance History:
* --------------------
* 3/6/2018 first test
* Due to time and knowledge limitation, functions are not completed....
*/

#include "../CppNoSqlDb/DbCore/Definitions.h"
#include "../CppNoSqlDb/DbCore/DbCore.h"
#include "../Version/Version.h"
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <windows.h>

using namespace NoSqlDb;

template<typename P>
class Checkout {
public:
	using Vertype = std::unordered_map<std::string, char>;
	using iterator = typename Vertype::iterator;
	using DbStore = std::unordered_map<Key, DbElement<P>>;
	using iterat = typename DbStore::iterator;
	//Checkout(RepositoryCore<P>& Repo) : Repo_(Repo) {}
	void clearversion(Version & version, const std::string pacname);
	//private:
	//RepositoryCore<P>& Repo_;
};

template<typename P>
void Checkout<P>::clearversion(Version & version, const std::string pacname) {
	iterator iter = version.vercore().find(pacname);
	if (iter == version.vercore().end()) {
		return;
	}
	else {
		version.vercore().erase(iter);
	}
}
