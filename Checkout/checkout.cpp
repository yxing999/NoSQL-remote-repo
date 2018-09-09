/////////////////////////////////////////////////////////////////////
// Browse.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// YUXUAN XING CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*
* Required Files:
* ---------------
*
*
* Maintenance History:
* --------------------
*
*/

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <clocale>
#include <locale>
#include "checkout.h"
#include "../Version/Version.h"
#include "../CppNoSqlDb/DbCore/Definitions.h"
#include "../CppNoSqlDb/DbCore/DbCore.h"


using namespace NoSqlDb;

#ifdef TEST_CHECKOUT
int main() {
	Version ver;
	DbCore<std::string> repo;

	Checkout<std::string> checkout;
	DbElement<std::string> elem;
	elem.name() = "DbCore";
	elem.payLoad() = "open";
	elem.descrip() = "yuxuan";
	repo.addRecord("DbCore.h", elem);

	checkout.clearversion(ver, "DbCore");
	//std::vector<std::string> s=checkout.getname(repo, "DbCore");
	/*for (size_t i = 0; i < s.size(); i++) {
	std::cout << s[i] << "\n";
	}*/

	showDb(repo);

	getchar();
	return 0;
}
#endif