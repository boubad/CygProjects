#pragma once
#ifndef __DBSTATITEMS_H__
#define __DBSTATITEMS_H__
/////////////////////////////////////////
#include "dbvalue.h"
/////////////////////////////////////////
namespace info {
	class Indiv;
	class SQLiteStatHelper;
	class StoreIndivProvider;
	//////////////////////////////////////////
	class StatBaseItem {
		friend class SQLiteStatHelper;
		friend class StoreIndivProvider;
	protected:
		StatBaseItem();
		StatBaseItem(const IntType nId);
		StatBaseItem(const IntType nId, const IntType nVersion, const std::string &status);
		StatBaseItem(const IntType nId, const IntType nVersion, const std::wstring &status);
		StatBaseItem(const StatBaseItem &other);
		StatBaseItem & operator=(const StatBaseItem &other);
		void id(const IntType n);
		void version(const IntType n);
	private:
		IntType m_id;
		IntType m_version;
		std::string m_status;
	public:
		virtual ~StatBaseItem();
	public:
		bool operator==(const StatBaseItem &other) const;
		bool operator<(const StatBaseItem &other) const;
		IntType operator()(void) const;
		IntType id(void) const;
		IntType version(void) const;
		void get_status(std::string &s) const;
		void get_status(std::wstring &s) const;
		void set_status(const std::string &s);
		void set_status(const std::wstring &s);
	public:
		virtual bool is_writeable(void) const;
		virtual bool is_updateable(void) const;
		virtual bool is_removeable(void) const;
		virtual void clear(void);
	};// class StatBaseItem
	///////////////////////////////////////////////
	class StatNamedItem : public StatBaseItem {
		friend class SQLiteStatHelper;
		friend class StoreIndivProvider;
	protected:
		StatNamedItem();
		StatNamedItem(const IntType nId);
		StatNamedItem(const IntType nId, const IntType nVersion, const std::string &status,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc);
		StatNamedItem(const IntType nId, const IntType nVersion, const std::wstring &status,
			const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc);
		StatNamedItem(const StatNamedItem &other);
		StatNamedItem & operator=(const StatNamedItem &other);
	private:
		std::string m_sigle;
		std::string m_name;
		std::string m_desc;
	public:
		virtual ~StatNamedItem();
	public:
		void get_sigle(std::wstring &s) const;
		void set_sigle(const std::wstring &s);
		void get_sigle(std::string &s) const;
		void set_sigle(const std::string &s);
		//
		void get_name(std::wstring &s) const;
		void get_name(std::string &s) const;
		void set_name(const std::wstring &s);
		void set_name(const std::string &s);
		//
		void get_desc(std::wstring &s) const;
		void get_desc(std::string &s) const;
		void set_desc(const std::wstring &s);
		void set_desc(const std::string &s);
	public:
		virtual bool is_writeable(void) const;
		virtual void clear(void);
	};
	////////////////////////////////////////////////
	class DBStatDataset : public StatNamedItem {
		friend class SQLiteStatHelper;
		friend class StoreIndivProvider;
	public:
		DBStatDataset(const IntType nId);
		DBStatDataset(const std::wstring &sigle);
		DBStatDataset(const std::string &sigle);
		DBStatDataset(const IntType nId, const IntType nVersion, const std::wstring &status,
			const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc);
		DBStatDataset(const IntType nId, const IntType nVersion, const std::string &status,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc);
		DBStatDataset();
		DBStatDataset(const DBStatDataset &other);
		DBStatDataset & operator=(const DBStatDataset &other);
		virtual ~DBStatDataset();
		void swap(DBStatDataset &other);
	}; // class DBStatDataset
	///////////////////////////////////////////////
	class DBStatDatasetChild : public StatNamedItem {
		friend class SQLiteStatHelper;
		friend class StoreIndivProvider;
	private:
		int m_datasetid;
		double m_weight;
	protected:
		DBStatDatasetChild();
		DBStatDatasetChild(const IntType nId);
		DBStatDatasetChild(const IntType nId, const IntType nVersion, const std::wstring &status,
			const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc,
			const IntType nDatasetId,const double f);
		DBStatDatasetChild(const IntType nId,const IntType nVersion, const std::string &status,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc,
			const IntType nDatasetId,const double f);
		DBStatDatasetChild(const DBStatDatasetChild &other);
		DBStatDatasetChild & operator=(const DBStatDatasetChild &other);
		void set_dataset_id(const IntType n);
	public:
		virtual ~DBStatDatasetChild();
		IntType get_dataset_id(void) const;
		double weight(void) const;
		void weight(double d);
		virtual bool is_writeable(void) const;
		virtual void clear(void);
	}; // class DBStatDatasetChild
	 ///////////////////////////////////////////////
	class DBStatVariable : public DBStatDatasetChild {
		friend class SQLiteStatHelper;
		friend class StoreIndivProvider;
	private:
		bool m_categ;
		std::string m_type;
		std::string m_genre;
	public:
		DBStatVariable(const IntType nId, const IntType nVersion, const std::wstring &status,
			const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc, const IntType nDatasetId,const double f,
			const bool bCateg, const std::wstring &sType, const std::wstring &sGenre);
		DBStatVariable(const IntType nId, const IntType nVersion, const std::string &status,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nDatasetId, const double f,
			const bool bCateg, const std::string &sType, const std::string &sGenre);
		DBStatVariable();
		DBStatVariable(const IntType nId);
		DBStatVariable(const DBStatDataset &oSet, const std::string &sSigle = std::string());
		DBStatVariable(const DBStatDataset &oSet, const std::wstring &sSigle = std::wstring());
		DBStatVariable(const DBStatVariable &other);
		DBStatVariable & operator=(const DBStatVariable &other);
		virtual ~DBStatVariable();
	public:
		bool is_numeric(void) const;
		bool is_categ(void) const;
		void is_categ(const bool b);
		void get_vartype(std::wstring &s) const;
		void get_vartype(std::string &s) const;
		void set_vartype(const std::wstring &s);
		void set_vartype(const std::string &s);
		void get_genre(std::wstring &s) const;
		void get_genre(std::string &s) const;
		void set_genre(const std::wstring &s);
		void set_genre(const std::string &s);
		void swap(DBStatVariable &other);
		//
		virtual bool is_writeable(void) const;
		virtual void clear(void);
	}; // class DBStatVariable
	///////////////////////////////////////////////////
	class DBStatIndiv : public DBStatDatasetChild {
		friend class SQLiteStatHelper;
		friend class StoreIndivProvider;
		friend class Indiv;
	public:
		DBStatIndiv(const IntType nId, const IntType nVersion, const std::wstring &status,
			const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc, const IntType nDatasetId, const double f);
		DBStatIndiv(const IntType nId, const IntType nVersion, const std::string &status,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nDatasetId, const double f);
		DBStatIndiv();
		DBStatIndiv(const IntType nId);
		DBStatIndiv(const DBStatDataset &oSet, const std::string &sSigle = std::string());
		DBStatIndiv(const DBStatDataset &oSet, const std::wstring &sSigle = std::wstring());
		DBStatIndiv(const DBStatIndiv &other);
		DBStatIndiv & operator=(const DBStatIndiv &other);
		virtual ~DBStatIndiv();
		void swap(DBStatIndiv &other);
	}; // class DBStatIndiv
	///////////////////////////////////////////////////
	class DBStatValue : public StatBaseItem {
		friend class SQLiteStatHelper;
		friend class StoreIndivProvider;
	private:
		IntType m_varid;
		IntType m_indid;
		DbValue m_val;
	public:
		DBStatValue(const IntType nId, const IntType nVersion, const std::wstring &status,
			const IntType nVarId, const IntType nIndId, const DbValue &val);
		DBStatValue(const IntType nId, const IntType nVersion, const std::string &status,
			const IntType nVarId, const IntType nIndId, const DbValue &val);
		DBStatValue();
		DBStatValue(const IntType nId);
		DBStatValue(const DBStatVariable &oVar, const DBStatIndiv &oInd);
		DBStatValue(const DBStatVariable &oVar, const DBStatIndiv &oInd, const DbValue &v);
		DBStatValue(const DBStatValue &other);
		DBStatValue & operator=(const DBStatValue &other);
		virtual ~DBStatValue();
	public:
		IntType get_variable_id(void) const;
		IntType get_indiv_id(void) const;
		void set_variable_id(const IntType n);
		void set_indiv_id(const IntType n);
		const DbValue & get_value(void) const;
		void set_value(const DbValue &v);
		virtual bool is_writeable(void) const;
		void swap(DBStatValue &other);
		virtual void clear(void);
	}; // class DBStatValue
	////////////////////////////////////////////////////
	typedef boost::container::vector<DBStatDataset> datasets_vector;
	typedef boost::container::vector<DBStatVariable> variables_vector;
	typedef boost::container::vector<DBStatIndiv> indivs_vector;
	typedef boost::container::vector<DBStatValue> values_vector;
	typedef boost::container::flat_map<IntType, DBStatVariable> variables_map;
	////////////////////////////////////////////////////
}// namespace info
//////////////////////////////////////////
inline void swap(info::DBStatDataset &v1, info::DBStatDataset &v2) {
	v1.swap(v2);
}
inline void swap(info::DBStatVariable &v1, info::DBStatVariable &v2) {
	v1.swap(v2);
}
inline void swap(info::DBStatIndiv &v1, info::DBStatIndiv &v2) {
	v1.swap(v2);
}
inline void swap(info::DBStatValue &v1, info::DBStatValue &v2) {
	v1.swap(v2);
}
/////////////////////////////////////////
#endif // !__DBSTATITEMS_H__
