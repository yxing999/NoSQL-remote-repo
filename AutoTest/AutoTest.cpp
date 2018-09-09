#include "AutoTest.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>

using namespace NoSqlDb;

void Version::identify(std::ostream& out)
{
	out << "\n  \"" << __FILE__ << "\"";
}

bool Version::contains(const std::string& name) {
	iterator iter = VerCore_.find(name);
	return iter != VerCore_.end();
}

size_t Version::size()
{
	return VerCore_.size();
}
std::vector<std::string> Version::names() {
	std::vector<std::string> Vernames;
	Vertype& vers = vercore();
	size_t size = vers.size();
	Vernames.reserve(size);
	for (auto item : vers)
	{
		Vernames.push_back(item.first);
	}
	return Vernames;
}


void Version::addnum(const std::string& name, const num& number) {
	if (contains(name))
		return;
	VerCore_[name] = number;
}
void Version::removenum(const std::string& name) {
	size_t numErased = VerCore_.erase(name);
}

void Version::editnum(const std::string& name, const num& number) {
	if (!contains(name))
		return;
	VerCore_[name] = number;
}

class repoProvider
{
public:
	DbCore<std::string>& db() { return db_; }
private:
	static DbCore<std::string> db_;
};

DbCore<std::string> repoProvider::db_;

class verProvider
{
public:
	Version& db() { return db_; }
private:
	static Version db_;
};

Version verProvider::db_;

bool test1() {
	std::cout << "Demonstrate Version management:    \n";

	std::cout << "\n";

	Version ver;
	verProvider v;
	v.db() = ver;

	std::cout << "put DbCore in:\n";
	ver.addnum("Dbcore", '1');
	ShowVer(ver);

	std::cout << "put DbCore in again:\n";
	ver.editnum("Dbcore", '2');
	ShowVer(ver);

	std::cout << "Remove DbCore :\n";
	ver.removenum("Dbcore");
	ShowVer(ver);
	return true;
}

bool test2() {
	std::cout << "Demonstrate Checkin process:           \n";

	DbCore<std::string> repo;
	repoProvider dbp;
	dbp.db() = repo;


	verProvider v;
	Version ver = v.db();

	Checkin<std::string> checkin(repo);

	std::cout << "accept a package \n";
	std::vector<std::string> currfiles = checkin.getFiles("../Translater", "*.*");
	std::cout << "the package content is :    \n";
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << currfiles[i] << "\n";
	char c = checkin.doversion(ver, "Translater");
	for (size_t k = 0; k<currfiles.size(); k++) {
		currfiles[k] = currfiles[k] + "." + c;
	}
	for (size_t t = 0; t < currfiles.size(); t++) {
		DbElement<std::string> elem;
		elem.name() = "Translater";
		elem.payLoad() = "open";
		elem.descrip() = "yuxuan";
		checkin.PushBack(currfiles[t], elem);
	}
	showDb(repo);
	std::cout << "accept a package \n";
	std::vector<std::string> currfiles1 = checkin.getFiles("../Translater", "*.*");
	std::cout << "the package content is :    \n";
	for (size_t i = 0; i<currfiles1.size(); ++i)
		std::cout << "\n    " << currfiles1[i] << "\n";
	char b = checkin.doversion(ver, "Translater");
	for (size_t k = 0; k<currfiles1.size(); k++) {
		currfiles1[k] = currfiles1[k] + "." + b;
	}
	for (size_t t = 0; t < currfiles1.size(); t++) {
		DbElement<std::string> elem;
		elem.name() = "Translater";
		elem.payLoad() = "open";
		elem.descrip() = "yuxuan";
		checkin.PushBack(currfiles1[t], elem);
	}
	showDb(repo);
	std::cout << "accept a package \n";
	std::vector<std::string> currfiles2 = checkin.getFiles("../ServerPrototype", "*.*");
	std::cout << "the package content is :    \n";
	for (size_t i = 0; i<currfiles2.size(); ++i)
		std::cout << "\n    " << currfiles2[i] << "\n";
	char a = checkin.doversion(ver, "ServerPrototype");
	for (size_t k = 0; k<currfiles2.size(); k++) {
		currfiles2[k] = currfiles2[k] + "." + a;
	}
	for (size_t t = 0; t < currfiles2.size(); t++) {
		DbElement<std::string> elem;
		elem.name() = "ServerPrototype";
		elem.payLoad() = "close";
		elem.descrip() = "yuxuan";
		checkin.PushBack(currfiles2[t], elem);
	}
	showDb(repo);
	return true;
}

bool test4() {
	std::cout << "Test checkout function:        \n";

	repoProvider dbp;
	DbCore<std::string> repo = dbp.db();


	verProvider v;
	Version ver = v.db();

	Checkout<std::string> checkout;
	DbElement<std::string> elem;
	elem.name() = "DbCore";
	elem.payLoad() = "open";
	elem.descrip() = "yuxuan";
	repo.addRecord("DbCore.h", elem);

	checkout.clearversion(ver, "DbCore");
	//std::vector<std::string> s = checkout.getname(repo, "DbCore");
	//for (size_t i = 0; i < s.size(); i++) {
	//	std::cout << s[i] << "\n";
	//}

	showDb(repo);
	return true;
}

int main() {


	TestExecutive ex;


	TestExecutive::TestStr ts1{ test1, "version:" };
	TestExecutive::TestStr ts2{ test2, "checkin" };
	//TestExecutive::TestStr ts3{ test3, "browse" };
	TestExecutive::TestStr ts4{ test4, "checkout" };



	ex.registerTest(ts1);
	ex.registerTest(ts2);
	//ex.registerTest(ts3);
	ex.registerTest(ts4);

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	getchar();
	return 0;
}