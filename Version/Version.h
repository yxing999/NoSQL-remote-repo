#pragma once
/////////////////////////////////////////////////////////////////////
// Version.h -  Code Repository                                    //
// ver 1.0                                                         //
// YUXUAN XING CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* this package define the version management,
* every step of checkin and checkout will check this version management
* to make sure the version number is correct
* Required Files:
* ---------------
* Definitions.h
* RepositoryCore.h
* Maintenance History:
* --------------------
* 3/6/2018 first released
*/
#include "../CppNoSqlDb/DbCore/Definitions.h"
#include "../CppNoSqlDb/DbCore/DbCore.h"
#include<unordered_map>
#include<iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <windows.h>

namespace NoSqlDb {

	class Version {
	public:
		//Version() = default;
		static void identify(std::ostream& out = std::cout);
		typedef char num;
		using Vertype = std::unordered_map<std::string, num>;
		using iterator = typename Vertype::iterator;
		bool contains(const std::string& name);
		//operator std::unordered_map<std::string, num>() { return VerCore_; };

		std::vector<std::string> names();
		size_t size();
		void addnum(const std::string& name, const num& number);
		void removenum(const std::string& name);
		void editnum(const std::string& name, const num& number);
		typename iterator begin() { return VerCore_.begin(); }
		typename iterator end() { return VerCore_.end(); }

		Vertype& vercore() { return VerCore_; }
		Vertype vercore() const { return VerCore_; }
		void vercore(const Vertype& Vercore) { VerCore_ = Vercore; }

	private:
		Vertype VerCore_;
	};
	inline	void ShowVer(const Version& Ver, std::ostream& out = std::cout) {
		out << "show the Version \n";
		out << "name    number \n";
		out << "============================================== \n";
		typename Version::Vertype vers = Ver.vercore();
		for (auto item : vers)
		{
			out << item.first << "  " << item.second << "\n";
		}
		out << "\n";
	}
}