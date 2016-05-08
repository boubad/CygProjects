//dbstatitems.cpp
/////////////////////////////
#include "../include/dbstatitems.h"
#include "../include/stringconvert.h"
//////////////////////////////////
namespace info {
	///////////////////////////////
	DBStatValue::DBStatValue(const DBStatVariable &oVar, const DBStatIndiv &oInd) :m_varid(oVar.id()), m_indid(oInd.id()) {
	}// DBStatValue
	DBStatValue::DBStatValue(const DBStatVariable &oVar, const DBStatIndiv &oInd, const DbValue &v) :
		m_varid(oVar.id()), m_indid(oInd.id()), m_val(v) {
	}// DBStatValue
	DBStatValue::DBStatValue(const IntType nId) : StatBaseItem(nId) {}
	DBStatValue::DBStatValue(const IntType nId, const IntType nVersion, const std::wstring &status,
		const IntType nVarId, const IntType nIndId, const DbValue &val) : StatBaseItem(nId, nVersion, status),
		m_varid(nVarId), m_indid(nIndId), m_val(val) {}
	DBStatValue::DBStatValue(const IntType nId, const IntType nVersion, const std::string &status,
		const IntType nVarId, const IntType nIndId, const DbValue &val) : StatBaseItem(nId, nVersion, status),
		m_varid(nVarId), m_indid(nIndId), m_val(val) {}
	DBStatValue::DBStatValue() {
	}
	DBStatValue::DBStatValue(const DBStatValue &other) : StatBaseItem(other), m_varid(other.m_varid),
		m_indid(other.m_indid), m_val(other.m_val) {

	}
	DBStatValue & DBStatValue::operator=(const DBStatValue &other) {
		if (this != &other) {
			StatBaseItem::operator=(other);
			this->m_varid = other.m_varid;
			this->m_indid = other.m_indid;
			this->m_val = other.m_val;
		}
		return (*this);
	}
	DBStatValue::~DBStatValue() {}
	IntType DBStatValue::get_variable_id(void) const {
		return (this->m_varid);
	}
	void DBStatValue::set_variable_id(const IntType n) {
		this->m_varid = n;
	}
	IntType DBStatValue::get_indiv_id(void) const {
		return (this->m_indid);
	}
	void DBStatValue::set_indiv_id(const IntType n) {
		this->m_indid = n;
	}
	const DbValue & DBStatValue::get_value(void) const {
		return (this->m_val);
	}
	void DBStatValue::set_value(const DbValue &v) {
		this->m_val = v;
	}
	bool DBStatValue::is_writeable(void) const {
		return ((this->m_varid != 0) && (this->m_indid != 0));
	}
	void DBStatValue::swap(DBStatValue &other) {
		DBStatValue t(*this);
		*this = other;
		other = t;
	}// swap
	void DBStatValue::clear(void) {
		StatBaseItem::clear();
		this->m_indid = 0;
		this->m_varid = 0;
		this->m_val = DbValue();
	}
	/////////////////////////////////////////////////////
	DBStatIndiv::DBStatIndiv(const IntType nId, const IntType nVersion, const std::wstring &status,
		const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc, const IntType nDatasetId, const double f) :
		DBStatDatasetChild(nId, nVersion, status, sSigle, sName, sDesc, nDatasetId,f) {
	}
	DBStatIndiv::DBStatIndiv(const IntType nId, const IntType nVersion, const std::string &status,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nDatasetId, const double f) :
		DBStatDatasetChild(nId, nVersion, status, sSigle, sName, sDesc, nDatasetId,f) {
	}
	DBStatIndiv::DBStatIndiv(const IntType n) : DBStatDatasetChild(n) {}
	DBStatIndiv::DBStatIndiv() {}
	DBStatIndiv::DBStatIndiv(const DBStatDataset &oSet, const std::string &sSigle) {
		this->set_dataset_id(oSet.id());
		this->set_sigle(sSigle);
		this->set_name(sSigle);
		this->set_desc(sSigle);
	}
	DBStatIndiv::DBStatIndiv(const DBStatDataset &oSet, const std::wstring &sSigle) {
		this->set_dataset_id(oSet.id());
		this->set_sigle(sSigle);
		this->set_name(sSigle);
		this->set_desc(sSigle);
	}
	DBStatIndiv::DBStatIndiv(const DBStatIndiv &other) : DBStatDatasetChild(other) {}
	DBStatIndiv & DBStatIndiv::operator=(const DBStatIndiv &other) {
		if (this != &other) {
			DBStatDatasetChild::operator=(other);
		}
		return (*this);
	}
	DBStatIndiv::~DBStatIndiv() {

	}
	void DBStatIndiv::swap(DBStatIndiv &other) {
		DBStatIndiv t(*this);
		*this = other;
		other = t;
	}// swap
	/////////////////////////////////////////////////
	DBStatVariable::DBStatVariable(const IntType nId, const IntType nVersion, const std::wstring &status,
		const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc, const IntType nDatasetId, const double f,
		const bool bCateg, const std::wstring &sType, const std::wstring &sGenre) :
		DBStatDatasetChild(nId, nVersion, status, sSigle, sName, sDesc, nDatasetId,f), m_categ(bCateg) {
		this->set_vartype(sType);
		this->set_genre(sGenre);
	}
	DBStatVariable::DBStatVariable(const IntType nId, const IntType nVersion, const std::string &status,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nDatasetId, const double f,
		const bool bCateg, const std::string &sType, const std::string &sGenre) :
		DBStatDatasetChild(nId, nVersion, status, sSigle, sName, sDesc, nDatasetId,f), m_categ(bCateg) {
		this->set_vartype(sType);
		this->set_genre(sGenre);
	}
	DBStatVariable::DBStatVariable(const IntType nId) : DBStatDatasetChild(nId), m_categ(false) {}
	DBStatVariable::DBStatVariable() : m_categ(false) {}
	DBStatVariable::DBStatVariable(const DBStatDataset &oSet, const std::string &sSigle) : m_categ(false) {
		this->set_dataset_id(oSet.id());
		this->set_sigle(sSigle);
		this->set_name(sSigle);
		this->set_desc(sSigle);
		this->set_vartype("float");
		this->set_genre("initial");
		this->is_categ(false);
	}
	DBStatVariable::DBStatVariable(const DBStatDataset &oSet, const std::wstring &sSigle) : m_categ(false) {
		this->set_dataset_id(oSet.id());
		this->set_sigle(sSigle);
		this->set_name(sSigle);
		this->set_desc(sSigle);
		this->set_vartype("float");
		this->set_genre("initial");
		this->is_categ(false);
	}
	void DBStatVariable::clear(void) {
		DBStatDatasetChild::clear();
		this->set_vartype("float");
		this->set_genre("initial");
		this->is_categ(false);
	}
	DBStatVariable::DBStatVariable(const DBStatVariable &other) : DBStatDatasetChild(other),
		m_categ(other.m_categ), m_type(other.m_type), m_genre(other.m_genre) {}
	DBStatVariable & DBStatVariable::operator=(const DBStatVariable &other) {
		if (this != &other) {
			DBStatDatasetChild::operator=(other);
			this->m_categ = other.m_categ;
			this->m_type = other.m_type;
			this->m_genre = other.m_genre;
		}
		return (*this);
	}
	DBStatVariable::~DBStatVariable() {

	}
	bool DBStatVariable::is_writeable(void) const {
		return ((!this->m_type.empty()) && DBStatDatasetChild::is_writeable());
	}
	bool DBStatVariable::is_categ(void) const {
		return (this->m_categ);
	}
	void DBStatVariable::is_categ(const bool b) {
		this->m_categ = b;
	}
	void DBStatVariable::get_vartype(std::string &s) const {
		s = this->m_type;
	}
	void DBStatVariable::get_vartype(std::wstring &s) const {
		s = info_trim_lower(StringConvert::s2ws(this->m_type));
	}
	void DBStatVariable::set_vartype(const std::wstring &s) {
		this->m_type = info_trim_lower(StringConvert::ws2s(s));
	}
	void DBStatVariable::set_vartype(const std::string &s) {
		this->m_type = info_trim_lower(s);
	}
	void DBStatVariable::get_genre(std::wstring &s) const {
		s = StringConvert::s2ws(this->m_genre);
	}
	void DBStatVariable::get_genre(std::string &s) const {
		s = this->m_genre;
	}
	void DBStatVariable::set_genre(const std::wstring &s) {
		this->m_genre = info_trim_upper(StringConvert::ws2s(s));
	}
	void DBStatVariable::set_genre(const std::string &s) {
		this->m_genre = info_trim_upper(s);
	}
	void DBStatVariable::swap(DBStatVariable &other) {
		DBStatVariable t(*this);
		*this = other;
		other = t;
	}// swap
	bool DBStatVariable::is_numeric(void) const {
		if (this->is_categ()) {
			return (false);
		}
		std::string s = this->m_type;
		return ((s == "double") || (s == "float") || (s == "real") ||
			(s == "int") || (s == "integer") || (s == "long"));
	}// is_numeric
	///////////////////////////////////////////
	DBStatDatasetChild::DBStatDatasetChild() :m_datasetid(0), m_weight(1.0) {
	}
	DBStatDatasetChild::DBStatDatasetChild(const IntType nId) : StatNamedItem(nId), m_datasetid(0), m_weight(1.0) {
	}
	DBStatDatasetChild::DBStatDatasetChild(const IntType nId, const IntType nVersion, const std::wstring &status,
		const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc
		, const IntType nDatasetId, const double f) :
		StatNamedItem(nId, nVersion, status, sSigle, sName, sDesc), m_datasetid(nDatasetId), m_weight(f) {}
	DBStatDatasetChild::DBStatDatasetChild(const IntType nId, const IntType nVersion, const std::string &status,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc,
		const IntType nDatasetId, const double f) :
		StatNamedItem(nId, nVersion, status, sSigle, sName, sDesc), m_datasetid(nDatasetId), m_weight(f) {}
	DBStatDatasetChild::DBStatDatasetChild(const DBStatDatasetChild &other) : StatNamedItem(other),
		m_datasetid(other.m_datasetid), m_weight(other.m_weight) {}
	DBStatDatasetChild & DBStatDatasetChild::operator=(const DBStatDatasetChild &other) {
		if (this != &other) {
			StatNamedItem::operator=(other);
			this->m_datasetid = other.m_datasetid;
			this->m_weight = other.m_weight;
		}
		return (*this);
	}
	DBStatDatasetChild::~DBStatDatasetChild() {}
	IntType DBStatDatasetChild::get_dataset_id(void) const {
		return (this->m_datasetid);
	}
	void DBStatDatasetChild::clear(void) {
		StatNamedItem::clear();
		this->m_datasetid = 0;
		this->m_weight = 1.0;
	}
	void  DBStatDatasetChild::set_dataset_id(const IntType n) {
		this->m_datasetid = n;
	}
	bool DBStatDatasetChild::is_writeable(void) const {
		return ((this->m_datasetid != 0) && StatNamedItem::is_writeable());
	}
	double DBStatDatasetChild::weight(void) const {
		return this->m_weight;
	}
	void DBStatDatasetChild::weight(double d) {
		if (d >= 0.0) {
			this->m_weight = d;
		}
	}
	//////////////////////////////////////////
	DBStatDataset::DBStatDataset() {
	}
	DBStatDataset::DBStatDataset(const std::string &sigle) {
		this->set_sigle(sigle);
		this->set_name(sigle);
		this->set_desc(sigle);
	}
	DBStatDataset::DBStatDataset(const std::wstring &sigle) {
		this->set_sigle(sigle);
		this->set_name(sigle);
		this->set_desc(sigle);
	}
	DBStatDataset::DBStatDataset(const IntType nId) :StatNamedItem(nId) {
	}
	DBStatDataset::DBStatDataset(const IntType nId, const IntType nVersion, const std::wstring &status,
		const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc) :
		StatNamedItem(nId, nVersion, status, sSigle, sName, sDesc) {}
	DBStatDataset::DBStatDataset(const IntType nId, const IntType nVersion, const std::string &status,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc) :
		StatNamedItem(nId, nVersion, status, sSigle, sName, sDesc) {}
	DBStatDataset::DBStatDataset(const DBStatDataset &other) : StatNamedItem(other) {}
	DBStatDataset & DBStatDataset::operator=(const DBStatDataset &other) {
		if (this != &other) {
			StatNamedItem::operator=(other);
		}
		return (*this);
	}
	DBStatDataset::~DBStatDataset() {
	}
	void DBStatDataset::swap(DBStatDataset &other) {
		DBStatDataset t(*this);
		*this = other;
		other = t;
	}// swap
	///////////////////////////////////////////
	StatNamedItem::StatNamedItem() {

	}
	StatNamedItem::StatNamedItem(const IntType nId) :StatBaseItem(nId) {

	}
	StatNamedItem::StatNamedItem(const IntType nId, const IntType nVersion, const std::wstring &status,
		const std::wstring &sSigle, const std::wstring &sName, const std::wstring &sDesc) :
		StatBaseItem(nId, nVersion, status) {
		this->set_sigle(sSigle);
		this->set_name(sName);
		this->set_desc(sDesc);
	}
	StatNamedItem::StatNamedItem(const IntType nId, const IntType nVersion, const std::string &status,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc) :
		StatBaseItem(nId, nVersion, status) {
		this->set_sigle(sSigle);
		this->set_name(sName);
		this->set_desc(sDesc);
	}
	StatNamedItem::StatNamedItem(const StatNamedItem &other) :StatBaseItem(other),
		m_sigle(other.m_sigle), m_name(other.m_name), m_desc(other.m_desc) {

	}
	StatNamedItem & StatNamedItem::operator=(const StatNamedItem &other) {
		if (this != &other) {
			StatBaseItem::operator=(other);
			this->m_sigle = other.m_sigle;
			this->m_name = other.m_name;
			this->m_desc = other.m_desc;
		}
		return (*this);
	}
	StatNamedItem::~StatNamedItem() {

	}
	void StatNamedItem::clear(void) {
		StatBaseItem::clear();
		this->m_desc.clear();
		this->m_name.clear();
		this->m_sigle.clear();
	}
	void StatNamedItem::get_sigle(std::wstring &s) const {
		s = StringConvert::s2ws(this->m_sigle);
	}
	void StatNamedItem::get_sigle(std::string &s) const {
		s = this->m_sigle;
	}
	void StatNamedItem::set_sigle(const std::string &s) {
		this->m_sigle = info_trim_upper(s);
	}
	void StatNamedItem::set_sigle(const std::wstring &s) {
		this->m_sigle = info_trim_upper(StringConvert::ws2s(s));
	}
	//
	void StatNamedItem::get_name(std::string &s) const {
		s = this->m_name;
	}
	void StatNamedItem::get_name(std::wstring &s) const {
		s = StringConvert::s2ws(this->m_name);
	}
	void StatNamedItem::set_name(const std::wstring &s) {
		this->m_name = info_trim(StringConvert::ws2s(s));
	}
	void StatNamedItem::set_name(const std::string &s) {
		this->m_name = info_trim(s);
	}
	//
	void StatNamedItem::get_desc(std::wstring &s) const {
		s = StringConvert::s2ws(this->m_desc);
	}
	void StatNamedItem::get_desc(std::string &s) const {
		s = this->m_desc;
	}
	void StatNamedItem::set_desc(const std::wstring &s) {
		this->m_desc = info_trim(StringConvert::ws2s(s));
	}
	void StatNamedItem::set_desc(const std::string &s) {
		this->m_desc = info_trim(s);
	}
	bool StatNamedItem::is_writeable(void) const {
		return (!(this->m_sigle.empty()));
	}
	//////////////////////////////////////////
	StatBaseItem::StatBaseItem() :m_id(0), m_version(0) {
	}
	StatBaseItem::StatBaseItem(const IntType nId) : m_id(nId), m_version(0) {
	}
	StatBaseItem::StatBaseItem(const IntType nId, const IntType nVersion, const std::string &status) : m_id(nId), m_version(nVersion) {
		this->set_status(status);
	}
	StatBaseItem::StatBaseItem(const IntType nId, const IntType nVersion, const std::wstring &status) : m_id(nId), m_version(nVersion) {
		this->set_status(status);
	}
	StatBaseItem::StatBaseItem(const StatBaseItem &other) : m_id(other.m_id), m_version(other.m_version), m_status(other.m_status) {
	}
	StatBaseItem & StatBaseItem::operator=(const StatBaseItem &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_version = other.m_version;
			this->m_status = other.m_status;
		}
		return (*this);
	}
	StatBaseItem::~StatBaseItem() {

	}
	void StatBaseItem::clear(void) {
		this->m_status.clear();
		this->m_id = 0;
		this->m_version = 0;
	}
	void StatBaseItem::get_status(std::string &s) const {
		s = this->m_status;
	}
	void StatBaseItem::get_status(std::wstring &s) const {
		s = StringConvert::s2ws(this->m_status);
	}
	void StatBaseItem::set_status(const std::string &s) {
		this->m_status = info_trim_upper(s);
	}
	void StatBaseItem::set_status(const std::wstring &s) {
		this->m_status = StringConvert::ws2s(info_trim_upper(s));
	}
	IntType StatBaseItem::id(void) const {
		return (this->m_id);
	}
	IntType StatBaseItem::version(void) const {
		return (this->m_version);
	}
	bool StatBaseItem::is_writeable(void) const {
		return (false);
	}
	bool StatBaseItem::is_updateable(void) const {
		return ((this->id() != 0) && (this->is_writeable()));
	}
	bool StatBaseItem::is_removeable(void) const {
		return (this->id() != 0);
	}
	void StatBaseItem::id(const IntType n) {
		this->m_id = n;
	}
	void StatBaseItem::version(const IntType n) {
		this->m_version = n;
	}
	bool StatBaseItem::operator==(const StatBaseItem &other) const {
		return (this->id() == other.id());
	}
	bool StatBaseItem::operator<(const StatBaseItem &other) const {
		return (this->id() < other.id());
	}
	IntType StatBaseItem::operator()(void) const {
		return (this->id());
	}
	///////////////////////////////////////////
}// namespace info_sqlite