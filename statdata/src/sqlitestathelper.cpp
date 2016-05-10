#include "../include/sqlitestathelper.h"
#include "../include/sqlite_database.h"
#include "../include/stringconvert.h"
#include "../include/sqlite_statement.h"
/////////////////////////////////////
namespace info {
	//////////////////////////////////////
	const std::string SQLiteStatHelper::DEFAULT_DATABASE_NAME("info_sets.sqlite");
	/////////////////////////////////////
	static const char *SQL_CREATE_DATASET =
		"CREATE TABLE IF NOT EXISTS dbdataset("
		" datasetid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" sigle TEXT NOT NULL UNIQUE,"
		" nom TEXT DEFAULT NULL,"
		" description TEXT DEFAULT NULL,"
		" status TEXT DEFAULT NULL"
		" )";
	static const char *SQL_CREATE_VARIABLE =
		"CREATE TABLE IF NOT EXISTS dbvariable("
		" variableid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" datasetid INTEGER NOT NULL,"
		" sigle TEXT NOT NULL,"
		" weight REAL NOT NULL DEFAULT 1,"
		" vartype TEXT NOT NULL,"
		" categvar INTEGER NOT NULL DEFAULT 0,"
		" nom TEXT DEFAULT NULL,"
		" description TEXT DEFAULT NULL,"
		" genre TEXT DEFAULT NULL,"
		" status TEXT DEFAULT NULL,"
		" CONSTRAINT uc_variable UNIQUE (datasetid, sigle),"
		" CONSTRAINT fk_variable_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
		" )";
	static const char *SQL_CREATE_INDIV =
		"CREATE TABLE IF NOT EXISTS dbindiv("
		" individ INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" datasetid INTEGER NOT NULL,"
		" sigle TEXT NOT NULL,"
		" weight REAL NOT NULL DEFAULT 1,"
		" nom TEXT DEFAULT NULL,"
		" description TEXT DEFAULT NULL,"
		" status TEXT DEFAULT NULL,"
		" CONSTRAINT uc_indiv UNIQUE (datasetid, sigle),"
		" CONSTRAINT fk_indiv_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
		" )";
	static const char *SQL_CREATE_VALUE =
		"CREATE TABLE IF NOT EXISTS dbvalue("
		" valueid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" variableid INTEGER NOT NULL,"
		" individ INTEGER NOT NULL,"
		" stringval TEXT NULL,"
		" status TEXT DEFAULT NULL,"
		" CONSTRAINT uc_value UNIQUE (variableid, individ),"
		" CONSTRAINT fk_value_variable FOREIGN KEY (variableid) REFERENCES dbvariable (variableid) ON DELETE CASCADE,"
		" CONSTRAINT fk_value_indiv FOREIGN KEY (individ) REFERENCES dbindiv (individ) ON DELETE CASCADE"
		" )";
	static const char *CREATE_SQL[] = {
		SQL_CREATE_DATASET,
		SQL_CREATE_VARIABLE,
		SQL_CREATE_INDIV,
		SQL_CREATE_VALUE,
		nullptr };
	/////////////////////////////////////
	static const char *SQL_FIND_ALL_DATASETS_IDS =
		"SELECT datasetid"
		" FROM dbdataset ORDER BY datasetid"
		" LIMIT :taken OFFSET ?";
	static const char *SQL_FIND_ALL_DATASETS =
		"SELECT datasetid,optlock,sigle,nom,description,status"
		" FROM dbdataset ORDER BY sigle"
		" LIMIT ? OFFSET ?";
	static const char *SQL_FIND_DATASET_BY_ID =
		"SELECT datasetid,optlock,sigle,nom,description,status"
		" FROM dbdataset WHERE datasetid = ?";
	static const char *SQL_FIND_DATASET_BY_SIGLE =
		"SELECT datasetid,optlock,sigle,nom,description,status"
		" FROM dbdataset WHERE UPPER(LTRIM(RTRIM(sigle))) = ?";
	static const char *SQL_INSERT_DATASET =
		"INSERT INTO dbdataset (sigle,nom,description,status)"
		" VALUES (?,?,?,?)";
	static const char *SQL_UPDATE_DATASET =
		"UPDATE dbdataset SET optlock = optlock + 1,"
		" sigle = ?, nom = ?, description = ?, status = ? WHERE datasetid = ?";
	static const char *SQL_REMOVE_DATASET =
		"DELETE FROM dbdataset WHERE datasetid = ?";
	static const char *SQL_FIND_DATASETS_COUNT =
		"SELECT COUNT(*) FROM dbdataset";
	//
	static const char *SQL_FIND_DATASET_VARIABLES =
		"SELECT variableid, optlock, datasetid , sigle, vartype, categvar, nom, description, genre, status,weight"
		" FROM dbvariable WHERE datasetid = ?"
		" ORDER BY categvar DESC, sigle ASC"
		" LIMIT ? OFFSET ?";
	static const char *SQL_FIND_DATASET_VARIABLES_IDS =
		"SELECT variableid FROM dbvariable WHERE datasetid = ?"
		" ORDER BY variableid"
		" LIMIT ? OFFSET ?";
	static const char *SQL_VARIABLE_BY_ID =
		"SELECT variableid,optlock,datasetid,sigle,vartype,categvar,nom,description,genre,status,weight"
		" FROM dbvariable WHERE variableid = ?";
	static const char *SQL_VARIABLE_BY_DATASET_AND_SIGLE =
		"SELECT variableid,optlock,datasetid,sigle,vartype,categvar,nom,description,genre,status,weight"
		" FROM dbvariable WHERE datasetid = ? AND UPPER(LTRIM(RTRIM(sigle))) = ?";
	static const char *SQL_INSERT_VARIABLE =
		"INSERT INTO dbvariable (datasetid,sigle,vartype,categvar,nom,description,genre,status,weight)"
		" VALUES (?,?,?,?,?,?,?,?,?)";
	static const char *SQL_UPDATE_VARIABLE =
		"UPDATE dbvariable SET optlock = optlock + 1,"
		" sigle = ?, vartype = ?, categvar = ?, nom = ?, description = ?, genre = ?, status = ?,weight = ? WHERE variableid = ?";
	static const char *SQL_REMOVE_VARIABLE =
		"DELETE FROM dbvariable WHERE variableid = ?";
	static const char *SQL_FIND_DATASET_VARIABLES_COUNT =
		"SELECT COUNT(*) FROM dbvariable WHERE datasetid = ?";
	//
	static const char *SQL_FIND_DATASET_INDIVS_COUNT =
		"SELECT COUNT(*) FROM dbindiv"
		" WHERE datasetid = ?";
	static const char *SQL_GET_DATASET_INDIV_IDS =
		"SELECT individ FROM dbindiv"
		" WHERE datasetid = ? ORDER BY individ ASC"
		" LIMIT ? OFFSET ?";
	static const char *SQL_FIND_DATASET_INDIVS =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status,weight"
		" FROM dbindiv WHERE datasetid = ? ORDER BY sigle"
		" LIMIT ? OFFSET ?";
	static const char *SQL_INDIV_BY_ID =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status,weight"
		" FROM dbindiv WHERE individ = ?";
	static const char *SQL_INDIV_BY_DATASET_AND_SIGLE =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status,weight"
		" FROM dbindiv WHERE datasetid = ? AND UPPER(LTRIM(RTRIM(sigle))) = ?";
	static const char *SQL_INSERT_INDIV =
		"INSERT INTO dbindiv (datasetid,sigle,nom,description,status,weight)"
		" VALUES(?,?,?,?,?,?)";
	static const char *SQL_UPDATE_INDIV =
		"UPDATE dbindiv SET optlock = OPTLOCK + 1,"
		" sigle = ?, nom = ?, description = ?, status = ?,weight = ? WHERE individ = ?";
	static const char *SQL_REMOVE_INDIV =
		"DELETE FROM dbindiv WHERE individ = ?";
	//
	static const char *SQL_VALUE_BY_ID =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE valueid = ?";
	static const char *SQL_VALUES_BY_VARIABLE_INDIV =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE variableid = ? AND individ = ?";
	static const char *SQL_INSERT_VALUE =
		"INSERT INTO dbvalue (variableid,individ,stringval,status)"
		" VALUES(?,?,?,?)";
	static const char *SQL_UPDATE_VALUE =
		"UPDATE dbvalue SET optlock = optlock + 1,"
		" stringval = ?, status = ? WHERE valueid = ? ";
	static const char *SQL_REMOVE_VALUE =
		"DELETE from dbvalue WHERE valueid = ?";
	static const char *SQL_FIND_DATASET_VALUES_COUNT = "SELECT COUNT(*)"
		" FROM dbvalue a, dbvariable b"
		" WHERE a.variableid = b.variableid AND b.datasetid = ?";
	static const char *SQL_FIND_DATASET_VALUES =
		"SELECT a.valueid,a.optlock,a.variableid,a.individ,a.stringval,a.status"
		" FROM dbvalue a, dbvariable b"
		" WHERE a.variableid = b.variableid AND b.datasetid = ?"
		" ORDER BY a.variableid ASC, a.individ ASC"
		" LIMIT ? OFFSET ?";
	static const char *SQL_VALUES_BY_VARIABLEID =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE variableid = ?"
		" LIMIT ? OFFSET ?";
	static const char *SQL_VALUES_BY_INDIVID =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE individ = ?"
		" LIMIT ? OFFSET ?";
	static const char *SQL_VARIABLE_VALUES_DISTINCT =
		"SELECT DISTINCT stringval FROM dbvalue WHERE variableid = ?"
		" LIMIT ? OFFSET ?";
	static const char *SQL_DELETE_VARIABLE_VALUES =
		"DELETE FROM dbvalue where variableid = ?";
	static const char *SQL_DELETE_INDIV_VALUES =
		"DELETE FROM dbvalue where individ = ?";
	static const char *SQL_FIND_DATASET_VARIABLES_TYPES =
		"SELECT variableid,vartype FROM dbvariable WHERE datasetid = ?";
	///////////////////////////////////////
	static void log_error(const std::exception &err) {
		std::cerr << "ERROR: " << err.what() << std::endl;
	}
	static void log_error(sqlite_error &err) {
		std::cerr << "ERROR: SQLite error: " << err.code() << " , " << err.what() << std::endl;
	}
	void SQLiteStatHelper::begin_transaction(void) {
		BOOST_ASSERT(this->is_valid());
		if (this->m_intransaction) {
			return;
		}
		this->m_base->begin_transaction();
		this->m_intransaction = true;
	} // begin_transaction
	void SQLiteStatHelper::commit_transaction(void) {
		BOOST_ASSERT(this->is_valid());
		if (!this->m_intransaction) {
			return;
		}
		this->m_base->commit_transaction();
		this->m_intransaction = false;
	} // commit_transaction
	void SQLiteStatHelper::rollback_transaction(void) {
		BOOST_ASSERT(this->is_valid());
		if (!this->m_intransaction) {
			return;
		}
		try {
			this->m_base->rollback_transaction();
			this->m_intransaction = false;
		}
		catch (...) {}
		this->m_intransaction = false;
	} // rollback_transaction
	///////////////////////////////////////
	bool SQLiteStatHelper::find_dataset_variables_types(const DBStatDataset &oSet, inttype_string_map &oMap) {
		BOOST_ASSERT(this->is_valid());
		DBStatDataset xSet(oSet);
		oMap.clear();
		if (!this->find_dataset(xSet)) {
			return (false);
		}
		try {
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES_TYPES);
			BOOST_ASSERT(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			BOOST_ASSERT(q.get_num_columns() == 2);
			while (q.move_next()) {
				IntType key = 0;
				q.get_column(0, key);
				std::string val;
				q.get_column(1, val);
				oMap[key] = val;
			}
			return (true);
		}
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_variables_types

	bool SQLiteStatHelper::remove_indiv(const DBStatIndiv &oInd, bool bCommit /*= true*/) {
		BOOST_ASSERT(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			DBStatIndiv xind(oInd);
			if (!this->find_indiv(xind)) {
				return (false);
			}
			IntType nId = xind.id();
			SQLite_Statement q1(*(this->m_base), SQL_DELETE_INDIV_VALUES);
			BOOST_ASSERT(q1.get_parameters_count() == 1);
			q1.bind(1, nId);
			q1.exec();
			SQLite_Statement q2(*(this->m_base), SQL_REMOVE_INDIV);
			BOOST_ASSERT(q2.get_parameters_count() == 1);
			q2.bind(1, nId);
			q2.exec();
			if (bInTrans  && bCommit) {
				this->commit_transaction();
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		catch (std::exception &ex) {
			log_error(ex);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		return (false);
	}//remove_indiv
	bool SQLiteStatHelper::remove_variable(const DBStatVariable &oVar, bool bCommit /*= true*/) {
		BOOST_ASSERT(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			DBStatVariable xind(oVar);
			if (!this->find_variable(xind)) {
				return (false);
			}
			IntType nId = xind.id();
			SQLite_Statement q1(*(this->m_base), SQL_DELETE_VARIABLE_VALUES);
			BOOST_ASSERT(q1.get_parameters_count() == 1);
			q1.bind(1, nId);
			q1.exec();
			SQLite_Statement q2(*(this->m_base), SQL_REMOVE_VARIABLE);
			BOOST_ASSERT(q2.get_parameters_count() == 1);
			q2.bind(1, nId);
			q2.exec();
			if (bInTrans  && bCommit) {
				this->commit_transaction();
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		catch (std::exception &ex) {
			log_error(ex);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		return (false);
	}//remove_variable
	 //////////////////// VALUES /////////////////////
	bool  SQLiteStatHelper::find_dataset_values_count(const DBStatDataset &oSet, int &nCount) {
		BOOST_ASSERT(this->is_valid());
		try {
			DBStatDataset xSet(oSet);
			nCount = 0;
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VALUES_COUNT);
			BOOST_ASSERT(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			BOOST_ASSERT(q.get_num_columns() == 1);
			if (q.move_next()) {
				q.get_column(0, nCount);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_values_count

	bool  SQLiteStatHelper::find_dataset_values(const DBStatDataset &oSet,
		values_vector &oList,
		int skip /*= 0*/, int count /*= 100*/) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DBStatDataset xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VALUES);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			while (q.move_next()) {
				DBStatValue cur;
				this->read_value(q, cur);
				oList.push_back(cur);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_values

	bool  SQLiteStatHelper::find_variable_values(DBStatVariable &oVar, values_vector &oList,
		int skip /*= 0*/, int count /*= 100*/) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			if (!this->find_variable(oVar)) {
				return (false);
			}
			IntType nId = oVar.id();
			SQLite_Statement q(*(this->m_base), SQL_VALUES_BY_VARIABLEID);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			while (q.move_next()) {
				DBStatValue cur;
				this->read_value(q, cur);
				oList.push_back(cur);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}

	bool  SQLiteStatHelper::find_variable_distinct_values(DBStatVariable &oVar,
		strings_vector &oList,
		int skip /*= 0*/, int count /*= 100*/) {
		BOOST_ASSERT(this->is_valid());
		oList.clear();
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			if (!this->find_variable(oVar)) {
				return (false);
			}
			IntType nId = oVar.id();
			SQLite_Statement q(*(this->m_base), SQL_VARIABLE_VALUES_DISTINCT);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			while (q.move_next()) {
				std::string s;
				q.get_column(0, s);
				if (!s.empty()) {
					oList.push_back(s);
				}
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_variable_distinct_values

	bool  SQLiteStatHelper::find_indiv_values(DBStatIndiv &oInd, values_vector &oList,
		int skip /*= 0*/, int count /*= 100*/) {
		BOOST_ASSERT(this->is_valid());
		oList.clear();
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			if (!this->find_indiv(oInd)) {
				return (false);
			}
			IntType nId = oInd.id();
			SQLite_Statement q(*(this->m_base), SQL_VALUES_BY_INDIVID);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			while (q.move_next()) {
				DBStatValue cur;
				this->read_value(q, cur);
				oList.push_back(cur);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_indiv_values

	 ///
	bool SQLiteStatHelper::maintains_values(const values_vector &oVals,
		bool bCommit /*= true*/,
		bool bRemove /*= false*/) {
		BOOST_ASSERT(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			SQLite_Statement qInsert(*(this->m_base), SQL_INSERT_VALUE);
			BOOST_ASSERT(qInsert.get_parameters_count() == 4);
			SQLite_Statement qUpdate(*(this->m_base), SQL_UPDATE_VALUE);
			BOOST_ASSERT(qUpdate.get_parameters_count() == 3);
			SQLite_Statement qRemove(*(this->m_base), SQL_REMOVE_VALUE);
			BOOST_ASSERT(qRemove.get_parameters_count() == 1);
			//
			BOOST_FOREACH(const DBStatValue &oVal, oVals)
			{
				bool mustRemove = bRemove;
				DBStatValue xVal(oVal);
				this->find_value(xVal);
				IntType nId = xVal.id();
				std::string status;
				oVal.get_status(status);
				DbValue v = oVal.get_value();
				std::string sval;
				v.string_value(sval);
				if (nId != 0) {
					if (sval.empty()) {
						mustRemove = true;
					}
				}
				if (mustRemove) {
					if (nId != 0) {
						qRemove.bind(1, nId);
						qRemove.exec();
					}// nId
				}
				else if (oVal.is_writeable()) {
					if (nId != 0) {
						qUpdate.bind(1, sval);
						qUpdate.bind(2, status);
						qUpdate.bind(3, nId);
						qUpdate.exec();
					}
					else {
						IntType nVarId = oVal.get_variable_id();
						IntType nIndId = oVal.get_indiv_id();
						qInsert.bind(1, nVarId);
						qInsert.bind(2, nIndId);
						qInsert.bind(3, sval);
						qInsert.bind(4, status);
						qInsert.exec();
					}
				} // writeable
			}// oVal
			//
			if (bCommit && bInTrans) {
				this->commit_transaction();
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		catch (std::exception &ex) {
			log_error(ex);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		return (false);
	}//maintains_values

	bool SQLiteStatHelper::find_value(DBStatValue &cur) {
		BOOST_ASSERT(this->is_valid());
		try {
			IntType nId = cur.id();
			SQLite_Statement q(*(this->m_base), SQL_VALUE_BY_ID);
			BOOST_ASSERT(q.get_parameters_count() == 1);
			q.bind(1, nId);
			q.exec();
			if (q.move_next()) {
				DBStatValue cur;
				this->read_value(q, cur);
				return (true);
			}
			IntType nVarId = cur.get_variable_id();
			IntType nIndId = cur.get_indiv_id();
			SQLite_Statement q2(*(this->m_base), SQL_VALUES_BY_VARIABLE_INDIV);
			BOOST_ASSERT(q2.get_parameters_count() == 2);
			q2.bind(1, nVarId);
			q2.bind(2, nIndId);
			q2.exec();
			if (q2.move_next()) {
				DBStatValue cur;
				this->read_value(q2, cur);
				return (true);
			}
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_value
	 /////////////////////////////// INDIVS ///////////////
	bool SQLiteStatHelper::find_dataset_indivs_count(const DBStatDataset &oSet, int &nCount) {
		BOOST_ASSERT(this->is_valid());
		try {
			DBStatDataset xSet(oSet);
			nCount = 0;
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_INDIVS_COUNT);
			BOOST_ASSERT(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			if (q.move_next()) {
				q.get_column(0, nCount);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_indivs_count

	bool SQLiteStatHelper::maintains_indivs(const indivs_vector &oInds,
		bool bCommit /*= true*/,
		bool bRemove /*= false*/) {
		BOOST_ASSERT(this->is_valid());
		bool bInTrans = false;
		//
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			SQLite_Statement qInsert(*(this->m_base), SQL_INSERT_INDIV);
			BOOST_ASSERT(qInsert.get_parameters_count() == 6);
			SQLite_Statement qUpdate(*(this->m_base), SQL_UPDATE_INDIV);
			BOOST_ASSERT(qUpdate.get_parameters_count() == 6);
			//
			BOOST_FOREACH(const DBStatIndiv &oInd, oInds)
			{
				DBStatIndiv xInd(oInd);
				this->find_indiv(xInd);
				IntType nId = xInd.id();
				if (bRemove) {
					if (nId != 0) {
						this->remove_indiv(xInd, false);
					}// nId
				}
				else if (oInd.is_writeable()) {
					std::string sigle, name, status, desc;
					oInd.get_sigle(sigle);
					oInd.get_name(name);
					oInd.get_status(status);
					oInd.get_desc(desc);
					double w = oInd.weight();
					if (w < 0.0) {
						w = 0;
					}
					IntType nDatasetId = oInd.get_dataset_id();
					if (nId != 0) {
						qUpdate.bind(1, sigle);
						qUpdate.bind(2, name);
						qUpdate.bind(3, desc);
						qUpdate.bind(4, status);
						qUpdate.bind(5, w);
						qUpdate.bind(6, nId);
						qUpdate.exec();
					}
					else {
						qInsert.bind(1, nDatasetId);
						qInsert.bind(2, sigle);
						qInsert.bind(3, name);
						qInsert.bind(4, desc);
						qInsert.bind(5, status);
						qInsert.bind(6, w);
						qInsert.exec();
					}
				}
			}// oInd
			if (bCommit && bInTrans) {
				this->commit_transaction();
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		catch (std::exception &ex) {
			log_error(ex);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		return (false);
	}//maintains_indivs

	bool SQLiteStatHelper::find_dataset_indivs_ids(const DBStatDataset &oSet, ints_vector &oList,
		int skip /*=0*/, int count /*=100*/) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DBStatDataset xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_GET_DATASET_INDIV_IDS);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			BOOST_ASSERT(q.get_num_columns() == 1);
			while (q.move_next()) {
				IntType nId = 0;
				q.get_column(0, nId);
				if (nId != 0) {
					oList.push_back(nId);
				}
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_indivs_ids
	bool SQLiteStatHelper::find_dataset_indivs(const DBStatDataset &oSet,
		indivs_vector &oList,
		int skip /*=0*/, int count /*=100*/) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DBStatDataset xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_INDIVS);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			while (q.move_next()) {
				DBStatIndiv cur;
				this->read_indiv(q, cur);
				oList.push_back(cur);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_indivs
	 ////////////////////////////////////
	bool SQLiteStatHelper::find_indiv(DBStatIndiv &cur) {
		BOOST_ASSERT(this->is_valid());
		try {
			IntType nId = cur.id();
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_INDIV_BY_ID);
				BOOST_ASSERT(q.get_parameters_count() == 1);
				q.bind(1, nId);
				q.exec();
				if (q.move_next()) {
					this->read_indiv(q, cur);
					return (true);
				}
			}
			std::string sigle;
			cur.get_sigle(sigle);
			IntType nDatasetId = cur.get_dataset_id();
			if ((!sigle.empty()) && (nDatasetId != 0)) {
				SQLite_Statement q(*(this->m_base), SQL_INDIV_BY_DATASET_AND_SIGLE);
				BOOST_ASSERT(q.get_parameters_count() == 2);
				q.bind(1, nDatasetId);
				q.bind(2, sigle);
				q.exec();
				if (q.move_next()) {
					this->read_indiv(q, cur);
					return (true);
				}
			}
			return (false);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_indiv
	 ////////////////////////////// VARIABLES /////
	bool SQLiteStatHelper::find_dataset_variables_count(const DBStatDataset &oSet, int &nCount) {
		BOOST_ASSERT(this->is_valid());
		try {
			DBStatDataset xSet(oSet);
			nCount = 0;
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES_COUNT);
			BOOST_ASSERT(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			BOOST_ASSERT(q.get_num_columns() == 1);
			if (q.move_next()) {
				q.get_column(0, nCount);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_variables_count

	bool SQLiteStatHelper::maintains_variables(const variables_vector &oVars,
		bool bCommit /*= true*/,
		bool bRemove /*= false*/) {
		BOOST_ASSERT(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			SQLite_Statement qInsert(*(this->m_base), SQL_INSERT_VARIABLE);
			BOOST_ASSERT(qInsert.get_parameters_count() == 9);
			SQLite_Statement qUpdate(*(this->m_base), SQL_UPDATE_VARIABLE);
			BOOST_ASSERT(qUpdate.get_parameters_count() == 9);
			//
			BOOST_FOREACH(const DBStatVariable &oVar, oVars)
			{
				DBStatVariable xVar(oVar);
				this->find_variable(xVar);
				IntType nId = xVar.id();
				if (bRemove) {
					if (nId != 0) {
						this->remove_variable(xVar, false);
					}// nId
				}
				else if (oVar.is_writeable()) {
					std::string sigle, name, status, desc, vartype, genre;
					int nCateg = (oVar.is_categ()) ? 1 : 0;
					oVar.get_sigle(sigle);
					oVar.get_name(name);
					oVar.get_status(status);
					oVar.get_desc(desc);
					oVar.get_genre(genre);
					oVar.get_vartype(vartype);
					IntType nDatasetId = oVar.get_dataset_id();
					double w = oVar.weight();
					if (w < 0) {
						w = 0;
					}
					if (nId != 0) {
						qUpdate.bind(1, sigle);
						qUpdate.bind(2, vartype);
						qUpdate.bind(3, nCateg);
						qUpdate.bind(4, name);
						qUpdate.bind(5, desc);
						qUpdate.bind(6, genre);
						qUpdate.bind(7, status);
						qUpdate.bind(8, w);
						qUpdate.bind(9, nId);
						qUpdate.exec();
					}
					else {
						qInsert.bind(1, nDatasetId);
						qInsert.bind(2, sigle);
						qInsert.bind(3, vartype);
						qInsert.bind(4, nCateg);
						qInsert.bind(5, name);
						qInsert.bind(6, desc);
						qInsert.bind(7, genre);
						qInsert.bind(8, status);
						qInsert.bind(9, w);
						qInsert.exec();
					}
				}// writeable
			}//oVar
			if (bCommit && bInTrans) {
				this->commit_transaction();
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		catch (std::exception &ex) {
			log_error(ex);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		return (false);
	}//maintains_variables

	bool SQLiteStatHelper::find_dataset_variables_ids(const DBStatDataset &oSet, ints_vector &oList,
		int skip /*=0*/, int count /*=100*/) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DBStatDataset xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES_IDS);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			BOOST_ASSERT(q.get_num_columns() == 1);
			while (q.move_next()) {
				IntType nId = 0;
				q.get_column(0, nId);
				if (nId != 0) {
					oList.push_back(nId);
				}
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_variables_ids

	bool SQLiteStatHelper::find_dataset_variables(const DBStatDataset &oSet, variables_vector &oList,
		int skip /*=0*/, int count /*=100*/) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DBStatDataset xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES);
			BOOST_ASSERT(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, count);
			q.bind(3, skip);
			q.exec();
			while (q.move_next()) {
				DBStatVariable cur;
				this->read_variable(q, cur);
				oList.push_back(cur);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_variables

	 ////////////////////////////////////
	bool SQLiteStatHelper::find_variable(DBStatVariable &cur) {
		BOOST_ASSERT(this->is_valid());
		try {
			IntType nId = cur.id();
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_VARIABLE_BY_ID);
				BOOST_ASSERT(q.get_parameters_count() == 1);
				q.bind(1, nId);
				q.exec();
				if (q.move_next()) {
					this->read_variable(q, cur);
					return (true);
				}
			}
			std::string sigle;
			cur.get_sigle(sigle);
			IntType nDatasetId = cur.get_dataset_id();
			if ((!sigle.empty()) && (nDatasetId != 0)) {
				SQLite_Statement q(*(this->m_base), SQL_VARIABLE_BY_DATASET_AND_SIGLE);
				BOOST_ASSERT(q.get_parameters_count() == 2);
				q.bind(1, nDatasetId);
				q.bind(2, sigle);
				q.exec();
				if (q.move_next()) {
					this->read_variable(q, cur);
					return (true);
				}
			}
			return (false);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_variable
	 /////////////////////////////////////////////////////
	bool SQLiteStatHelper::remove_dataset(const DBStatDataset &cur,
		bool bCommit /*= true*/) {
		BOOST_ASSERT(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			DBStatDataset xSet(cur);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IntType nId = xSet.id();
			{
				int nCount = 0;
				this->find_dataset_indivs_count(xSet, nCount);
				if (nCount > 0) {
					ints_vector oIds;
					this->find_dataset_indivs_ids(xSet, oIds, 0, nCount);
					std::for_each(oIds.begin(), oIds.end(), [&](const IntType &aId) {
						DBStatIndiv xInd(aId);
						this->remove_indiv(xInd, false);
					});
				}
			}
			{
				int nCount = 0;
				this->find_dataset_variables_count(xSet, nCount);
				if (nCount > 0) {
					ints_vector oIds;
					this->find_dataset_variables_ids(xSet, oIds, 0, nCount);
					std::for_each(oIds.begin(), oIds.end(), [&](const IntType &aId) {
						DBStatVariable xInd(aId);
						this->remove_variable(xInd, false);
					});
				}
			}
			SQLite_Statement qq(*(this->m_base), SQL_REMOVE_DATASET);
			BOOST_ASSERT(qq.get_parameters_count() == 1);
			qq.bind(1, nId);
			qq.exec();
			if (bCommit && bInTrans) {
				this->commit_transaction();
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		catch (std::exception &ex) {
			log_error(ex);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		return (false);
	}// remove_dataset

	bool SQLiteStatHelper::maintains_dataset(DBStatDataset &cur, bool bCommit) {
		BOOST_ASSERT(this->is_valid());
		if (!cur.is_writeable()) {
			return (false);
		}
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			DBStatDataset xSet(cur);
			this->find_dataset(xSet);
			IntType nId = xSet.id();
			std::string sigle, name, desc, status;
			cur.get_sigle(sigle);
			cur.get_name(name);
			cur.get_desc(desc);
			cur.get_status(status);
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_UPDATE_DATASET);
				BOOST_ASSERT(q.get_parameters_count() == 5);
				q.bind(1, sigle);
				q.bind(2, name);
				q.bind(3, desc);
				q.bind(4, status);
				q.bind(5, nId);
				q.exec();
			}
			else {
				SQLite_Statement q(*(this->m_base), SQL_INSERT_DATASET);
				BOOST_ASSERT(q.get_parameters_count() == 4);
				q.bind(1, sigle);
				q.bind(2, name);
				q.bind(3, desc);
				q.bind(4, status);
				q.exec();
			}
			if (bCommit && bInTrans) {
				this->commit_transaction();
			}
			return (this->find_dataset(cur));
		}// try
		catch (sqlite_error &err) {
			log_error(err);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		catch (std::exception &ex) {
			log_error(ex);
			if (bInTrans) {
				this->rollback_transaction();
			}
		}
		return (false);

	}// maintains_datase
	bool SQLiteStatHelper::find_dataset(DBStatDataset &cur) {
		BOOST_ASSERT(this->is_valid());
		try {
			IntType nId = cur.id();
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_BY_ID);
				BOOST_ASSERT(q.get_parameters_count() == 1);
				q.bind(1, nId);
				q.exec();
				if (q.move_next()) {
					this->read_dataset(q, cur);
					return (true);
				}
			}
			std::string sigle;
			cur.get_sigle(sigle);
			if (!sigle.empty()) {
				SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_BY_SIGLE);
				BOOST_ASSERT(q.get_parameters_count() == 1);
				q.bind(1, sigle);
				q.exec();
				if (q.move_next()) {
					this->read_dataset(q, cur);
					return (true);
				}
			}
			return (false);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}// find_dataset
	bool  SQLiteStatHelper::find_all_datasets(datasets_vector &oList,
		int skip /* = 0*/,
		int count /* = 100 */) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			SQLite_Statement q(*(this->m_base), SQL_FIND_ALL_DATASETS);
			BOOST_ASSERT(q.get_parameters_count() == 2);
			q.bind(1, count);
			q.bind(2, skip);
			q.exec();
			while (q.move_next()) {
				DBStatDataset cur;
				this->read_dataset(q, cur);
				oList.push_back(cur);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_all_datasets
	bool  SQLiteStatHelper::find_all_datasets_ids(ints_vector &oList,
		int skip /* = 0*/,
		int count /* = 100 */) {
		BOOST_ASSERT(this->is_valid());
		try {
			oList.clear();
			if (skip < 0) {
				skip = 0;
			}
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			SQLite_Statement q(*(this->m_base), SQL_FIND_ALL_DATASETS_IDS);
			BOOST_ASSERT(q.get_parameters_count() == 2);
			q.bind(1, count);
			q.bind(2, skip);
			q.exec();
			while (q.move_next()) {
				IntType cur;
				q.get_column(0, cur);
				if (cur != 0) {
					oList.push_back(cur);
				}
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_all_datasets_ids
	bool SQLiteStatHelper::find_all_datasets_count(int &nCount) {
		BOOST_ASSERT(this->is_valid());
		try {
			nCount = 0;
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASETS_COUNT);
			q.exec();
			BOOST_ASSERT(q.get_num_columns() == 1);
			if (q.move_next()) {
				q.get_column(0, nCount);
			}
			return (true);
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		return (false);
	}//find_dataset_variables_count
	 ////////////////////////////////////
	void SQLiteStatHelper::read_dataset(SQLite_Statement &q, DBStatDataset &cur) {
		BOOST_ASSERT(q.get_num_columns() == 6);
		std::string sSigle, sName, status, sDesc;
		IntType nId, nVersion;
		q.get_column(0, nId);
		q.get_column(1, nVersion);
		q.get_column(2, sSigle);
		q.get_column(3, sName);
		q.get_column(4, sDesc);
		q.get_column(5, status);
		cur = DBStatDataset(nId, nVersion, status, sSigle, sName, sDesc);
	}

	void SQLiteStatHelper::read_variable(SQLite_Statement &q, DBStatVariable &cur) {
		BOOST_ASSERT(q.get_num_columns() == 11);
		std::string sSigle, sName, status, sDesc, sGenre, sType;
		IntType nId, nVersion, nCateg, nDatasetId;
		double w = 1.0;
		q.get_column(0, nId);
		q.get_column(1, nVersion);
		q.get_column(2, nDatasetId);
		q.get_column(3, sSigle);
		q.get_column(4, sType);
		q.get_column(5, nCateg);
		q.get_column(6, sName);
		q.get_column(7, sDesc);
		q.get_column(8, sGenre);
		q.get_column(9, status);
		q.get_column(10, w);
		bool bCateg = (nCateg != 0) ? true : false;
		cur = DBStatVariable(nId, nVersion, status, sSigle, sName, sDesc, nDatasetId,w,
			bCateg, sType, sGenre);
	}

	void SQLiteStatHelper::read_indiv(SQLite_Statement &q, DBStatIndiv &cur) {
		BOOST_ASSERT(q.get_num_columns() == 8);
		std::string sSigle, sName, status, sDesc;
		IntType nId, nVersion, nDatasetId;
		double w = 1.0;
		q.get_column(0, nId);
		q.get_column(1, nVersion);
		q.get_column(2, nDatasetId);
		q.get_column(3, sSigle);
		q.get_column(4, sName);
		q.get_column(5, sDesc);
		q.get_column(6, status);
		q.get_column(7, w);
		//
		cur = DBStatIndiv(nId, nVersion, status, sSigle, sName, sDesc, nDatasetId,w);
	}

	void SQLiteStatHelper::read_value(SQLite_Statement &q, DBStatValue &cur) {
		BOOST_ASSERT(q.get_num_columns() == 6);
		std::string status, sval;
		IntType nId, nVersion, nVarId, nIndId;
		q.get_column(0, nId);
		q.get_column(1, nVersion);
		q.get_column(2, nVarId);
		q.get_column(3, nIndId);
		q.get_column(4, sval);
		q.get_column(5, status);
		DbValue v(sval);
		cur = DBStatValue(nId, nVersion, status, nVarId, nIndId, v);
	}
	////////////////////////////////
	SQLiteStatHelper::SQLiteStatHelper(const std::string &sDatabaseName /*= DEFAULT_DATABASE_NAME*/) {
		try {
			this->m_intransaction = false;
			this->m_base = std::make_unique<SQLite_Database>(sDatabaseName);
			BOOST_ASSERT(this->m_base.get() != nullptr);
			if (this->m_base->is_open()) {
				this->check_schema();
			}
		}
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}
		catch (...) {
			std::cerr << "ERROR : Unknown exception in open..." << std::endl;
		}
	}
	SQLiteStatHelper::SQLiteStatHelper(const std::wstring &sName /*= DEFAULT_DATABASE_NAME*/) {
		try {
			this->m_intransaction = false;
			std::string file = StringConvert::ws2s(sName);
			this->m_base = std::make_unique<SQLite_Database>(file);
			BOOST_ASSERT(this->m_base.get() != nullptr);
			if (this->m_base->is_open()) {
				this->check_schema();
			}
		}
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		} catch(...){
			std::cerr << "ERROR : Unknown exception in open..." << std::endl;
		}
	}
	void SQLiteStatHelper::check_schema(void) {
		BOOST_ASSERT(this->is_valid());
		try {
			int i = 0;
			SQLite_Database &base = *(this->m_base);
			while (CREATE_SQL[i] != nullptr) {
				const char *pszSQL = CREATE_SQL[i];
				std::string sql(pszSQL);
				base.exec(sql);
				i++;
			}// i
		}// try
		catch (sqlite_error &err) {
			log_error(err);
		}
		catch (std::exception &ex) {
			log_error(ex);
		}

	}// check_schema
	bool SQLiteStatHelper::is_valid(void) {
		SQLite_Database *p = this->m_base.get();
		return ((p != nullptr) && p->is_open());
	}
	SQLiteStatHelper::~SQLiteStatHelper() {

	}
	///////////////////////////////////////
}// namespace info
