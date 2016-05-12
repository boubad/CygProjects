// Copyright (C) 2008  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.

#include <dlib/test/tester.h>
#include <dlib/logger.h>
/////////////////////////////
#include "infotestdata.h"
//////////////////////////
#include <sqlitestathelper.h>
////////////////////////////////////////////
// This is called an unnamed-namespace and it has the effect of making everything 
// inside this file "private" so that everything you declare will have static linkage.  
// Thus we won't have any multiply defined symbol errors coming out of the linker when 
// we try to compile the test suite.
using namespace std;
using namespace test;
using namespace info;
using namespace dlib;
namespace
{
	// Declare the logger we will use in this test.  The name of the logger 
	// should start with "test."
	static dlib::logger dlog("test.sqlitestathelper");
	//
	class sqlitestatmanager_tester : public tester
	{
		/*!
		WHAT THIS OBJECT REPRESENTS
		This object represents a unit test.  When it is constructed
		it adds itself into the testing framework.
		!*/
	public:
		sqlitestatmanager_tester() :
			tester(
				"sqlitestatmanager",       // the command line argument name for this test
				"Run sqlitestatmanager tests.", // the command line argument description
				0                     // the number of command line arguments for this test
			)
		{
		}


		void perform_test()
		{
			// This message gets logged to the file debug.txt if the user has enabled logging by
			// supplying the -d option on the command line (and they haven't set the logging level
			// to something higher than LINFO).
			dlog << LINFO << "ENTERING sqlitestatmanage tests...";
			//
			std::string dbName;
			InfoTestData::get_database_filename(dbName);
			DLIB_TEST_MSG(!dbName.empty(), "Database filename cannot be empty");
			SQLiteStatHelper oMan(dbName);
			IStoreHelper *pHelper = &oMan;
			DLIB_TEST_MSG(pHelper->is_valid(), "StatDataManager is not valid");
			//
			std::string setName;
			InfoTestData::get_conso_name(setName);
			DLIB_TEST_MSG(!setName.empty(), "set name cannot be empty");
			
		}// perform_test
	};

	// Create an instance of this object.  Doing this causes this test
	// to be automatically inserted into the testing framework whenever this cpp file
	// is linked into the project.  Note that since we are inside an unnamed-namespace 
	// we won't get any linker errors about the symbol a being defined multiple times. 
	sqlitestatmanager_tester a;

}




