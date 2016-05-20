#include "../include/infosqlitestore.h"
#include "../include/sqlite_database.h"
#include "../include/sqlite_statement.h"
#include <infovalue.h>
#include <stringconvert.h>
////////////////////////////////////////
//#include <iostream>
#include <boost/log/trivial.hpp>
/////////////////////////////////////
namespace info {
	using namespace info_sqlite;
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
		" nbmodals INTEGER NOT NULL DEFAULT 0,"
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
		"SELECT variableid, optlock, datasetid , sigle, vartype, categvar, nom, description, genre, status,weight,nbmodals"
		" FROM dbvariable WHERE datasetid = ?"
		" ORDER BY categvar DESC, sigle ASC"
		" LIMIT ? OFFSET ?";
	static const char *SQL_FIND_DATASET_VARIABLES_IDS =
		"SELECT variableid FROM dbvariable WHERE datasetid = ?"
		" ORDER BY variableid"
		" LIMIT ? OFFSET ?";
	static const char *SQL_VARIABLE_BY_ID =
		"SELECT variableid,optlock,datasetid,sigle,vartype,categvar,nom,description,genre,status,weight,nbmodals"
		" FROM dbvariable WHERE variableid = ?";
	static const char *SQL_VARIABLE_BY_DATASET_AND_SIGLE =
		"SELECT variableid,optlock,datasetid,sigle,vartype,categvar,nom,description,genre,status,weight,nbmodals"
		" FROM dbvariable WHERE datasetid = ? AND UPPER(LTRIM(RTRIM(sigle))) = ?";
	static const char *SQL_INSERT_VARIABLE =
		"INSERT INTO dbvariable (datasetid,sigle,vartype,categvar,nom,description,genre,status,weight,nbmodals)"
		" VALUES (?,?,?,?,?,?,?,?,?,?)";
	static const char *SQL_UPDATE_VARIABLE =
		"UPDATE dbvariable SET optlock = optlock + 1,"
		" sigle = ?, vartype = ?, categvar = ?, nom = ?, description = ?, genre = ?, status = ?,weight = ?,nbmodals = ? WHERE variableid = ?";
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
	static const char *SQL_INDIV_VALUES_COUNT =
		"SELECT COUNT(*)"
		" FROM dbvalue WHERE individ = ?";
	static const char *SQL_VARIABLE_VALUES_COUNT =
		"SELECT COUNT(*)"
		" FROM dbvalue WHERE variableid = ?";
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
	static void log_error(const std::exception & /*err*/) {
		//BOOST_LOG_TRIVIAL(error) << err.what();
	}
	static void log_error(sqlite_error &/*err*/) {
		//BOOST_LOG_TRIVIAL(error) << "SQLite error: " << err.code() << " , " << err.what();
	}
	static void convert_value(const std::string &stype, const boost::any &vsrc, boost::any &vRes) {
		InfoValue v0(vsrc);
		if ((stype == "bool") || (stype == "boolean") || (stype == "logical")) {
			vRes = v0.bool_value();
		}
		else if (stype == "short") {
			vRes = v0.short_value();
		}
		else if (stype == "unsigned short") {
			vRes = v0.unsigned_short_value();
		}
		else if (stype == "int") {
			vRes = v0.int_value();
		}
		else if (stype == "unsigned int") {
			vRes = v0.unsigned_int_value();
		}
		else if (stype == "long") {
			vRes = v0.long_value();
		}
		else if (stype == "unsigned long") {
			vRes = v0.unsigned_long_value();
		}
		else if ((stype == "double") || (stype == "real")) {
			vRes = v0.double_value();
		}
		else if (stype == "string") {
			std::string s;
			v0.string_value(s);
			vRes = s;
		}
		else if (stype == "wstring") {
			std::wstring s;
			v0.string_value(s);
			vRes = s;
		}
		else {
			std::string s;
			v0.string_value(s);
			vRes = s;
		}

	}// convert_value
	//////////////////////////////////////////////
	void SQLiteStatHelper::begin_transaction(void) {
		assert(this->is_valid());
		if (this->m_intransaction) {
			return;
		}
		this->m_base->begin_transaction();
		this->m_intransaction = true;
	} // begin_transaction
	void SQLiteStatHelper::commit_transaction(void) {
		assert(this->is_valid());
		if (!this->m_intransaction) {
			return;
		}
		this->m_base->commit_transaction();
		this->m_intransaction = false;
	} // commit_transaction
	void SQLiteStatHelper::rollback_transaction(void) {
		assert(this->is_valid());
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
	bool SQLiteStatHelper::find_dataset_variables_types(const DatasetType &oSet, ints_string_map &oMap) {
		assert(this->is_valid());
		DatasetType xSet(oSet);
		oMap.clear();
		if (!this->find_dataset(xSet)) {
			return (false);
		}
		try {
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES_TYPES);
			assert(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			assert(q.get_num_columns() == 2);
			while (q.move_next()) {
				IDTYPE key = 0;
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

	bool SQLiteStatHelper::remove_indiv(const IndivType &oInd, bool bCommit /*= true*/) {
		assert(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			IndivType xind(oInd);
			if (!this->find_indiv(xind)) {
				return (false);
			}
			IDTYPE nId = xind.id();
			SQLite_Statement q1(*(this->m_base), SQL_DELETE_INDIV_VALUES);
			assert(q1.get_parameters_count() == 1);
			q1.bind(1, nId);
			q1.exec();
			SQLite_Statement q2(*(this->m_base), SQL_REMOVE_INDIV);
			assert(q2.get_parameters_count() == 1);
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
	bool SQLiteStatHelper::remove_variable(const VariableType &oVar, bool bCommit /*= true*/) {
		assert(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			VariableType xind(oVar);
			if (!this->find_variable(xind)) {
				return (false);
			}
			IDTYPE nId = xind.id();
			SQLite_Statement q1(*(this->m_base), SQL_DELETE_VARIABLE_VALUES);
			assert(q1.get_parameters_count() == 1);
			q1.bind(1, nId);
			q1.exec();
			SQLite_Statement q2(*(this->m_base), SQL_REMOVE_VARIABLE);
			assert(q2.get_parameters_count() == 1);
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
	bool  SQLiteStatHelper::find_dataset_values_count(const DatasetType &oSet, size_t &nCount) {
		assert(this->is_valid());
		try {
			DatasetType xSet(oSet);
			nCount = 0;
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VALUES_COUNT);
			assert(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			assert(q.get_num_columns() == 1);
			if (q.move_next()) {
				int nx = 0;
				q.get_column(0, nx);
				nCount = (size_t)nx;
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

	bool  SQLiteStatHelper::find_dataset_values(const DatasetType &oSet,
		values_vector &oList,
		size_t skip /*= 0*/, size_t count /*= 100*/) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DatasetType xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			ints_string_map oMap;
			if (!this->find_dataset_variables_types(xSet, oMap)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VALUES);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
			q.exec();
			while (q.move_next()) {
				ValueType cur;
				this->read_value(q, cur);
				IDTYPE key = cur.variable_id();
				if (oMap.find(key) != oMap.end()) {
					boost::any h;
					convert_value(oMap[key], cur.value(), h);
					cur.value(InfoValue(h));
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
	}//find_dataset_values

	bool  SQLiteStatHelper::find_variable_values(VariableType &oVar, values_vector &oList,
		size_t skip /*= 0*/, size_t count /*= 100*/) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			if (!this->find_variable(oVar)) {
				return (false);
			}
			STRINGTYPE stype = oVar.vartype();
			IDTYPE nId = oVar.id();
			SQLite_Statement q(*(this->m_base), SQL_VALUES_BY_VARIABLEID);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
			q.exec();
			while (q.move_next()) {
				ValueType cur;
				this->read_value(q, cur);
				boost::any h;
				convert_value(stype, cur.value(), h);
				cur.value(InfoValue(h));
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

	bool  SQLiteStatHelper::find_variable_distinct_values(VariableType &oVar,
		strings_vector &oList,
		size_t skip /*= 0*/, size_t count /*= 100*/) {
		assert(this->is_valid());
		oList.clear();
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			if (!this->find_variable(oVar)) {
				return (false);
			}
			IDTYPE nId = oVar.id();
			SQLite_Statement q(*(this->m_base), SQL_VARIABLE_VALUES_DISTINCT);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
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
	bool SQLiteStatHelper::find_indiv_values_count(IndivType &oInd, size_t &nc) {
		assert(this->is_valid());
		try {
			nc = 0;
			if (!this->find_indiv(oInd)) {
				return (false);
			}
			IDTYPE nId = oInd.id();
			SQLite_Statement q(*(this->m_base), SQL_INDIV_VALUES_COUNT);
			assert(q.get_parameters_count() == 1);
			q.bind(1, nId);
			if (q.move_next()) {
				int nx = 0;
				q.get_column(0, nx);
				nc = (size_t)nx;
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
	}//find_indiv_values_count
	bool SQLiteStatHelper::find_variable_values_count(VariableType &oVar, size_t &nc) {
		assert(this->is_valid());
		try {
			nc = 0;
			if (!this->find_variable(oVar)) {
				return (false);
			}
			IDTYPE nId = oVar.id();
			SQLite_Statement q(*(this->m_base), SQL_VARIABLE_VALUES_COUNT);
			assert(q.get_parameters_count() == 1);
			q.bind(1, nId);
			if (q.move_next()) {
				int nx = 0;
				q.get_column(0, nx);
				nc = (size_t)nx;
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
	}//find_variable_values_count
	bool  SQLiteStatHelper::find_indiv_values(IndivType &oInd, values_vector &oList,
		size_t skip /*= 0*/, size_t count /*= 100*/) {
		assert(this->is_valid());
		oList.clear();
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			if (!this->find_indiv(oInd)) {
				return (false);
			}
			DatasetType xSet;
			xSet.id(oInd.dataset_id());
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			ints_string_map oMap;
			if (!this->find_dataset_variables_types(xSet, oMap)) {
				return (false);
			}
			IDTYPE nId = oInd.id();
			SQLite_Statement q(*(this->m_base), SQL_VALUES_BY_INDIVID);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
			q.exec();
			while (q.move_next()) {
				ValueType cur;
				this->read_value(q, cur);
				IDTYPE key = cur.variable_id();
				if (oMap.find(key) != oMap.end()) {
					boost::any h;
					convert_value(oMap[key], cur.value(), h);
					cur.value(InfoValue(h));
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
	}//find_indiv_values

	 ///
	bool SQLiteStatHelper::maintains_values(const values_vector &oVals,
		bool bCommit /*= true*/,
		bool bRemove /*= false*/) {
		assert(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			SQLite_Statement qInsert(*(this->m_base), SQL_INSERT_VALUE);
			assert(qInsert.get_parameters_count() == 4);
			SQLite_Statement qUpdate(*(this->m_base), SQL_UPDATE_VALUE);
			assert(qUpdate.get_parameters_count() == 3);
			SQLite_Statement qRemove(*(this->m_base), SQL_REMOVE_VALUE);
			assert(qRemove.get_parameters_count() == 1);
			//
			std::for_each(oVals.begin(), oVals.end(), [&](const ValueType &oVal) {
				bool mustRemove = bRemove;
				ValueType xVal(oVal);
				this->find_value(xVal);
				IDTYPE nId = xVal.id();
				STRINGTYPE status = oVal.status();
				boost::any v0 = oVal.value();
				InfoValue v(v0);
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
						IDTYPE nVarId = oVal.variable_id();
						IDTYPE nIndId = oVal.indiv_id();
						qInsert.bind(1, nVarId);
						qInsert.bind(2, nIndId);
						qInsert.bind(3, sval);
						qInsert.bind(4, status);
						qInsert.exec();
					}
				} // writeable
			});
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

	bool SQLiteStatHelper::find_value(ValueType &cur) {
		assert(this->is_valid());
		try {
			IDTYPE nId = cur.id();
			SQLite_Statement q(*(this->m_base), SQL_VALUE_BY_ID);
			assert(q.get_parameters_count() == 1);
			q.bind(1, nId);
			q.exec();
			if (q.move_next()) {
				ValueType cur;
				this->read_value(q, cur);
				return (true);
			}
			IDTYPE nVarId = cur.variable_id();
			IDTYPE nIndId = cur.indiv_id();
			SQLite_Statement q2(*(this->m_base), SQL_VALUES_BY_VARIABLE_INDIV);
			assert(q2.get_parameters_count() == 2);
			q2.bind(1, nVarId);
			q2.bind(2, nIndId);
			q2.exec();
			if (q2.move_next()) {
				ValueType cur;
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
	bool SQLiteStatHelper::find_dataset_indivs_count(const DatasetType &oSet, size_t &nCount) {
		assert(this->is_valid());
		try {
			DatasetType xSet(oSet);
			nCount = 0;
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_INDIVS_COUNT);
			assert(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			int nx = 0;
			if (q.move_next()) {
				q.get_column(0, nx);
				nCount = (size_t)nx;
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
		assert(this->is_valid());
		bool bInTrans = false;
		//
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			SQLite_Statement qInsert(*(this->m_base), SQL_INSERT_INDIV);
			assert(qInsert.get_parameters_count() == 6);
			SQLite_Statement qUpdate(*(this->m_base), SQL_UPDATE_INDIV);
			assert(qUpdate.get_parameters_count() == 6);
			//
			std::for_each(oInds.begin(), oInds.end(), [&](const IndivType &oInd) {
				IndivType xInd(oInd);
				this->find_indiv(xInd);
				IDTYPE nId = xInd.id();
				if (bRemove) {
					if (nId != 0) {
						this->remove_indiv(xInd, false);
					}// nId
				}
				else if (oInd.is_writeable()) {
					STRINGTYPE sigle, name, status, desc;
					sigle = oInd.sigle();
					name = oInd.name();
					status = oInd.status();
					desc = oInd.description();
					double w = oInd.weight();
					if (w < 0.0) {
						w = 0;
					}
					IDTYPE nDatasetId = oInd.dataset_id();
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
			});
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

	bool SQLiteStatHelper::find_dataset_indivs_ids(const DatasetType &oSet, ints_vector &oList,
		size_t skip /*=0*/, size_t count /*=100*/) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DatasetType xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_GET_DATASET_INDIV_IDS);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
			q.exec();
			assert(q.get_num_columns() == 1);
			while (q.move_next()) {
				IDTYPE nId = 0;
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
	bool SQLiteStatHelper::find_dataset_indivs(const DatasetType &oSet,
		indivs_vector &oList,
		size_t skip /*=0*/, size_t count /*=100*/) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DatasetType xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_INDIVS);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
			q.exec();
			while (q.move_next()) {
				IndivType cur;
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
	bool SQLiteStatHelper::find_indiv(IndivType &cur) {
		assert(this->is_valid());
		try {
			IDTYPE nId = cur.id();
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_INDIV_BY_ID);
				assert(q.get_parameters_count() == 1);
				q.bind(1, nId);
				q.exec();
				if (q.move_next()) {
					this->read_indiv(q, cur);
					return (true);
				}
			}
			STRINGTYPE sigle = cur.sigle();
			IDTYPE nDatasetId = cur.dataset_id();
			if ((!sigle.empty()) && (nDatasetId != 0)) {
				SQLite_Statement q(*(this->m_base), SQL_INDIV_BY_DATASET_AND_SIGLE);
				assert(q.get_parameters_count() == 2);
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
	bool SQLiteStatHelper::find_dataset_variables_count(const DatasetType &oSet, size_t &nCount) {
		assert(this->is_valid());
		try {
			DatasetType xSet(oSet);
			nCount = 0;
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES_COUNT);
			assert(q.get_parameters_count() == 1);
			q.bind(1, nDatasetId);
			q.exec();
			assert(q.get_num_columns() == 1);
			if (q.move_next()) {
				int nx = 0;
				q.get_column(0, nx);
				nCount = nx;
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
		assert(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			SQLite_Statement qInsert(*(this->m_base), SQL_INSERT_VARIABLE);
			assert(qInsert.get_parameters_count() == 10);
			SQLite_Statement qUpdate(*(this->m_base), SQL_UPDATE_VARIABLE);
			assert(qUpdate.get_parameters_count() == 10);
			//
			std::for_each(oVars.begin(), oVars.end(), [&](const VariableType &oVar) {
				VariableType xVar(oVar);
				this->find_variable(xVar);
				IDTYPE nId = xVar.id();
				if (bRemove) {
					if (nId != 0) {
						this->remove_variable(xVar, false);
					}// nId
				}
				else if (oVar.is_writeable()) {
					STRINGTYPE sigle, name, status, desc, vartype, genre;
					sigle = oVar.sigle();
					name = oVar.name();
					status = oVar.status();
					desc = oVar.description();
					vartype = oVar.vartype();
					genre = oVar.genre();
					int nCateg = (oVar.is_categ()) ? 1 : 0;
					int nbModal = oVar.modalites_count();
					IDTYPE nDatasetId = oVar.dataset_id();
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
						qUpdate.bind(9, nbModal);
						qUpdate.bind(10, nId);
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
						qInsert.bind(10, nbModal);
						qInsert.exec();
					}
				}// writeable
			});
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

	bool SQLiteStatHelper::find_dataset_variables_ids(const DatasetType &oSet, ints_vector &oList,
		size_t skip /*=0*/, size_t count /*=100*/) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DatasetType xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES_IDS);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
			q.exec();
			assert(q.get_num_columns() == 1);
			while (q.move_next()) {
				IDTYPE nId = 0;
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

	bool SQLiteStatHelper::find_dataset_variables(const DatasetType &oSet, variables_vector &oList,
		size_t skip /*=0*/, size_t count /*=100*/) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			DatasetType xSet(oSet);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nDatasetId = xSet.id();
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_VARIABLES);
			assert(q.get_parameters_count() == 3);
			q.bind(1, nDatasetId);
			q.bind(2, (int)count);
			q.bind(3, (int)skip);
			q.exec();
			while (q.move_next()) {
				VariableType cur;
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
	bool SQLiteStatHelper::find_variable(VariableType &cur) {
		assert(this->is_valid());
		try {
			IDTYPE nId = cur.id();
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_VARIABLE_BY_ID);
				assert(q.get_parameters_count() == 1);
				q.bind(1, nId);
				q.exec();
				if (q.move_next()) {
					this->read_variable(q, cur);
					return (true);
				}
			}
			STRINGTYPE sigle = cur.sigle();
			IDTYPE nDatasetId = cur.dataset_id();
			if ((!sigle.empty()) && (nDatasetId != 0)) {
				SQLite_Statement q(*(this->m_base), SQL_VARIABLE_BY_DATASET_AND_SIGLE);
				assert(q.get_parameters_count() == 2);
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
	bool SQLiteStatHelper::remove_dataset(const DatasetType &cur,
		bool bCommit /*= true*/) {
		assert(this->is_valid());
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			DatasetType xSet(cur);
			if (!this->find_dataset(xSet)) {
				return (false);
			}
			IDTYPE nId = xSet.id();
			{
				size_t nCount = 0;
				this->find_dataset_indivs_count(xSet, nCount);
				if (nCount > 0) {
					ints_vector oIds;
					this->find_dataset_indivs_ids(xSet, oIds, 0, nCount);
					std::for_each(oIds.begin(), oIds.end(), [&](const IDTYPE &aId) {
						IndivType xInd(aId);
						this->remove_indiv(xInd, false);
					});
				}
			}
			{
				size_t nCount = 0;
				this->find_dataset_variables_count(xSet, nCount);
				if (nCount > 0) {
					ints_vector oIds;
					this->find_dataset_variables_ids(xSet, oIds, 0, nCount);
					std::for_each(oIds.begin(), oIds.end(), [&](const IDTYPE &aId) {
						VariableType xInd(aId);
						this->remove_variable(xInd, false);
					});
				}
			}
			SQLite_Statement qq(*(this->m_base), SQL_REMOVE_DATASET);
			assert(qq.get_parameters_count() == 1);
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

	bool SQLiteStatHelper::maintains_dataset(DatasetType &cur, bool bCommit) {
		assert(this->is_valid());
		if (!cur.is_writeable()) {
			return (false);
		}
		bool bInTrans = false;
		try {
			if (bCommit) {
				this->begin_transaction();
				bInTrans = true;
			}
			DatasetType xSet(cur);
			this->find_dataset(xSet);
			IDTYPE nId = xSet.id();
			STRINGTYPE sigle, name, desc, status;
			sigle = cur.sigle();
			name = cur.name();
			status = cur.status();
			desc = cur.description();
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_UPDATE_DATASET);
				assert(q.get_parameters_count() == 5);
				q.bind(1, sigle);
				q.bind(2, name);
				q.bind(3, desc);
				q.bind(4, status);
				q.bind(5, nId);
				q.exec();
			}
			else {
				SQLite_Statement q(*(this->m_base), SQL_INSERT_DATASET);
				assert(q.get_parameters_count() == 4);
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
	bool SQLiteStatHelper::find_dataset(DatasetType &cur) {
		assert(this->is_valid());
		try {
			IDTYPE nId = cur.id();
			if (nId != 0) {
				SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_BY_ID);
				assert(q.get_parameters_count() == 1);
				q.bind(1, nId);
				q.exec();
				if (q.move_next()) {
					this->read_dataset(q, cur);
					return (true);
				}
			}
			STRINGTYPE sigle = cur.sigle();
			if (!sigle.empty()) {
				SQLite_Statement q(*(this->m_base), SQL_FIND_DATASET_BY_SIGLE);
				assert(q.get_parameters_count() == 1);
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
		size_t skip /* = 0*/,
		size_t count /* = 100 */) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			SQLite_Statement q(*(this->m_base), SQL_FIND_ALL_DATASETS);
			assert(q.get_parameters_count() == 2);
			q.bind(1, (int)count);
			q.bind(2, (int)skip);
			q.exec();
			while (q.move_next()) {
				DatasetType cur;
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
		size_t skip /* = 0*/,
		size_t count /* = 100 */) {
		assert(this->is_valid());
		try {
			oList.clear();
			if (count < 1) {
				count = DATATRANSFER_CHUNK_SIZE;
			}
			SQLite_Statement q(*(this->m_base), SQL_FIND_ALL_DATASETS_IDS);
			assert(q.get_parameters_count() == 2);
			q.bind(1, (int)count);
			q.bind(2, (int)skip);
			q.exec();
			while (q.move_next()) {
				IDTYPE cur;
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
	bool SQLiteStatHelper::find_all_datasets_count(size_t &nCount) {
		assert(this->is_valid());
		try {
			nCount = 0;
			SQLite_Statement q(*(this->m_base), SQL_FIND_DATASETS_COUNT);
			q.exec();
			assert(q.get_num_columns() == 1);
			if (q.move_next()) {
				int nx = 0;
				q.get_column(0, nx);
				nCount = (size_t)nx;
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
	void SQLiteStatHelper::read_dataset(SQLite_Statement &q, DatasetType &cur) {
		assert(q.get_num_columns() == 6);
		STRINGTYPE sSigle, sName, status, sDesc;
		IDTYPE nId;
		INTTYPE nVersion;
		q.get_column(0, nId);
		q.get_column(1, nVersion);
		q.get_column(2, sSigle);
		q.get_column(3, sName);
		q.get_column(4, sDesc);
		q.get_column(5, status);
		cur.id(nId);
		cur.version(nVersion);
		cur.sigle(sSigle);
		cur.name(sName);
		cur.status(status);
		cur.description(sDesc);
	}

	void SQLiteStatHelper::read_variable(SQLite_Statement &q, VariableType &cur) {
		assert(q.get_num_columns() == 12);
		STRINGTYPE sSigle, sName, status, sDesc, sGenre, sType;
		IDTYPE nId, nDatasetId;
		INTTYPE nVersion, nCateg;
		int nbModal = 0;
		WEIGHTYPE w = 1.0;
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
		q.get_column(11, nbModal);
		bool bCateg = (nCateg != 0) ? true : false;
		cur.id(nId);
		cur.version(nVersion);
		cur.sigle(sSigle);
		cur.name(sName);
		cur.status(status);
		cur.description(sDesc);
		cur.is_categ(bCateg);
		cur.vartype(sType);
		cur.genre(sGenre);
		cur.dataset_id(nDatasetId);
		cur.weight(w);
		cur.modalites_count(nbModal);
	}

	void SQLiteStatHelper::read_indiv(SQLite_Statement &q, IndivType &cur) {
		assert(q.get_num_columns() == 8);
		STRINGTYPE sSigle, sName, status, sDesc;
		IDTYPE nId, nDatasetId;
		INTTYPE nVersion;
		WEIGHTYPE w = 1.0;
		q.get_column(0, nId);
		q.get_column(1, nVersion);
		q.get_column(2, nDatasetId);
		q.get_column(3, sSigle);
		q.get_column(4, sName);
		q.get_column(5, sDesc);
		q.get_column(6, status);
		q.get_column(7, w);
		//
		cur.id(nId);
		cur.version(nVersion);
		cur.sigle(sSigle);
		cur.name(sName);
		cur.status(status);
		cur.description(sDesc);
		cur.dataset_id(nDatasetId);
		cur.weight(w);
	}

	void SQLiteStatHelper::read_value(SQLite_Statement &q, ValueType &cur) {
		assert(q.get_num_columns() == 6);
		STRINGTYPE status, sval;
		IDTYPE nId, nVarId, nIndId;
		INTTYPE nVersion;
		InfoValue v;
		q.get_column(0, nId);
		q.get_column(1, nVersion);
		q.get_column(2, nVarId);
		q.get_column(3, nIndId);
		q.get_column(4, v);
		q.get_column(5, status);
		cur.id(nId);
		cur.version(nVersion);
		cur.status(status);
		cur.variable_id(nVarId);
		cur.indiv_id(nIndId);
		cur.value(v);
	}
	////////////////////////////////
	SQLiteStatHelper::SQLiteStatHelper(const STRINGTYPE &sDatabaseName /*= DEFAULT_DATABASE_NAME*/) {
		try {
			this->m_intransaction = false;
			this->m_base = std::make_unique<SQLite_Database>(sDatabaseName);
			assert(this->m_base.get() != nullptr);
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
			//BOOST_LOG_TRIVIAL(error) << "Unknown exception in open..." << std::endl;
		}
	}
	void SQLiteStatHelper::check_schema(void) {
		assert(this->is_valid());
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
