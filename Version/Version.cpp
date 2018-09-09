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
* -------------------
*
*
* Maintenance History:
* --------------------
*
*/

#include <iostream>
#include <iomanip>
#include <functional>

#include"Version.h"
#include "../CppNoSqlDb/Utilities/StringUtilities/StringUtilities.h"
#include "../CppNoSqlDb/Utilities/TestUtilities/TestUtilities.h"
using namespace NoSqlDb;

#ifdef TEST_VERSION
typedef char num;

class DbProvider
{
public:
	Version& db() { return db_; }
private:
	static Version db_;
};

Version DbProvider::db_;

//void Version::identify(std::ostream& out)
//{
//	out << "\n  \"" << __FILE__ << "\"";
//}
//
//bool Version::contains(const std::string& name) {
//	iterator iter = VerCore_.find(name);
//	return iter != VerCore_.end();
//}
//
//size_t Version::size()
//{
//	return VerCore_.size();
//}
//std::vector<std::string> Version::names() {
//	std::vector<std::string> Vernames;
//	Vertype& vers = vercore();
//	size_t size = vers.size();
//	Vernames.reserve(size);
//	for (auto item : vers)
//	{
//		Vernames.push_back(item.first);
//	}
//	return Vernames;
//}
//
//
//void Version::addnum(const std::string& name, const num& number) {
//	if (contains(name))
//		return ;
//	VerCore_[name] = number;
//}
//void Version::removenum(const std::string& name) {
//	size_t numErased = VerCore_.erase(name);
//}
//
//void Version::editnum(const std::string& name, const num& number) {
//	if (!contains(name))
//		return;
//	VerCore_[name] = number;
//}

//void Version::ShowVer(const Version& Ver, std::ostream& out = std::cout) {
//	out << "show the Version \n";
//	out << "==============================================";
//	typename Version::Vertype vers = Ver.vercore();
//	for (auto item : vers)
//	{
//		out << item.first << "  " << item.second << "\n";
//	}
//}

int main() {
	Utilities::Title("Testing Version function :  ");
	std::cout << "\n";
	Version ver;
	DbProvider dbp;
	dbp.db() = ver;

	ver.addnum("Dbcore", '1');
	ShowVer(ver);

	ver.editnum("Dbcore", '2');
	ShowVer(ver);

	ver.removenum("Dbcore");
	ShowVer(ver);
	getchar();
	return 0;
}
#endif