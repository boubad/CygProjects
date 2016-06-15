/*
 * datafixture.h
 *
 *  Created on: 15 juin 2016
 *      Author: boubad
 */

#ifndef DATAFIXTURE_H_
#define DATAFIXTURE_H_
/////////////////////////////////
#include <matricedata.h>
#include <inforunner.h>
/////////////////////////
#include "infotestdata.h"
////////////////////////////
namespace info {
///////////////////////////////////
template<typename U, typename STRINGTYPE, typename DATATYPE>
class InfoDataFixture {
public:
	using MatriceDataType = MatriceData<U,STRINGTYPE,DATATYPE>;
	using SourceType = IIndivSource<U, STRINGTYPE>;
	using DataVectorIndivSourceType = DataVectorIndivSource<U, STRINGTYPE>;
	using strings_vector = std::vector<STRINGTYPE>;
	using cancelflag = std::atomic<bool>;
	using pcancelflag = cancelflag *;
	using PBackgrounder = Backgrounder *;
private:
	std::unique_ptr<MatriceDataType> m_mortal;
	std::unique_ptr<MatriceDataType> m_conso;
	std::unique_ptr<MatriceDataType> m_test;
	std::unique_ptr<InfoRunner> m_runner;
private:
	MatriceDataType *init_data(const STRINGTYPE &name) {
		size_t nRows = 0, nCols = 0;
		std::vector<DATATYPE> data;
		strings_vector rowNames, colNames;
		InfoTestData::get_data(name, nRows, nCols, data, rowNames, colNames);
		MatriceDataType *pRet = new MatriceDataType(name, nRows, nCols, data,
				rowNames, colNames);
		return (pRet);
	} // init_data
public:
	InfoDataFixture() {
	}
	virtual ~InfoDataFixture() {
	}
public:
	InfoRunner *get_runner(void) {
		InfoRunner *p = this->m_runner.get();
		if (p == nullptr) {
			this->m_runner.reset(new InfoRunner());
			p = this->m_runner.get();
			assert(p != nullptr);
		}
		return (p);
	}
	PBackgrounder get_backgrounder(void) {
		InfoRunner *p = this->get_runner();
		return (p->get_backgrounder());
	}
	pcancelflag get_cancelflag(void) {
		InfoRunner *p = this->get_runner();
		return (p->get_cancelflag());
	}
	void reset_cancel(void) {
		InfoRunner *p = this->get_runner();
		p->reset_cancel();
	}
	void cancel(void) {
		InfoRunner *p = this->get_runner();
		p->cancel();
	}
	MatriceDataType *get_mortal_data(void) {
		MatriceDataType *pMat = this->m_mortal.get();
		if (pMat == nullptr) {
			STRINGTYPE name;
			InfoTestData::get_mortal_name(name);
			pMat = this->init_data(name);
			assert(pMat != nullptr);
			this->m_mortal.reset(pMat);
		}
		return (pMat);
	} // get_mortal_data
	MatriceDataType *get_conso_data(void) {
		MatriceDataType *pMat = this->m_conso.get();
		if (pMat == nullptr) {
			STRINGTYPE name;
			InfoTestData::get_conso_name(name);
			pMat = this->init_data(name);
			assert(pMat != nullptr);
			this->m_conso.reset(pMat);
		}
		return (pMat);
	} // get_mortal_data
	MatriceDataType *get_test_data(void) {
		MatriceDataType *pMat = this->m_test.get();
		if (pMat == nullptr) {
			STRINGTYPE name;
			InfoTestData::get_test_name(name);
			pMat = this->init_data(name);
			assert(pMat != nullptr);
			this->m_test.reset(pMat);
		}
		return (pMat);
	} // get_mortal_data
	SourceType *mortal_indiv_provider(void) {
		MatriceDataType *pMat = this->get_mortal_data();
		return (pMat->indiv_provider());
	} // get_mortal_indivs_source
	SourceType *mortal_variable_provider(void) {
		MatriceDataType *pMat = this->get_mortal_data();
		return (pMat->variable_provider());
	} // get_mortal_indivs_source
	SourceType *conso_indiv_provider(void) {
		MatriceDataType *pMat = this->get_conso_data();
		return (pMat->indiv_provider());
	} // get_mortal_indivs_source
	SourceType *conso_variable_provider(void) {
		MatriceDataType *pMat = this->get_conso_data();
		return (pMat->variable_provider());
	} // get_mortal_indivs_source
	SourceType *test_indiv_provider(void) {
		MatriceDataType *pMat = this->get_test_data();
		return (pMat->indiv_provider());
	} // get_mortal_indivs_source
	SourceType *test_variable_provider(void) {
		MatriceDataType *pMat = this->get_test_data();
		return (pMat->variable_provider());
	} // get_mortal_indivs_source
};
// class InfoDataFixture<U,STRINGTYPE,DATATYPE>
///////////////////////////////////////
}// namespace info
///////////////////////////////
#endif /* DATAFIXTURE_H_ */
