#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <clocale>
#include <locale>
#include "Checkin.h"
#include "../Version/Version.h"
#include "../CppNoSqlDb/DbCore/DbCore.h"
#include "../CppNoSqlDb/DbCore/Definitions.h"
#include "../CppNoSqlDb/PayLoad/PayLoad.h"

using namespace NoSqlDb;

#ifdef TEST_CHECKIN
int main() {
	Version ver;
	DbCore<std::string> repo;

	Checkin<std::string> checkin(repo);

	std::cout << "accept a package \n";
	std::vector<std::string> currfiles = checkin.getFiles("../DbCore", "*.*");
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << currfiles[i] << "\n";
	char c = checkin.doversion(ver, "DbCore");
	for (size_t k = 0; k<currfiles.size(); k++) {
		currfiles[k] = currfiles[k] + "." + c;
	}
	for (size_t t = 0; t < currfiles.size(); t++) {
		DbElement<std::string> elem;
		elem.name() = "DbCore";
		elem.payLoad() = "open";
		elem.descrip() = "yuxuan";
		checkin.PushBack(currfiles[t], elem);
	}

	std::vector<std::string> currfiles1 = checkin.getFiles("../DbCore", "*.*");
	for (size_t i = 0; i<currfiles1.size(); ++i)
		std::cout << "\n    " << currfiles1[i] << "\n";
	char b = checkin.doversion(ver, "DbCore");
	for (size_t k = 0; k<currfiles1.size(); k++) {
		currfiles1[k] = currfiles1[k] + "." + b;
	}
	for (size_t t = 0; t < currfiles1.size(); t++) {
		DbElement<std::string> elem;
		elem.name() = "DbCore";
		elem.payLoad() = "open";
		elem.descrip() = "yuxuan";
		checkin.PushBack(currfiles1[t], elem);
	}
	showDb(repo);
	getchar();
	return 0;
}
#endif