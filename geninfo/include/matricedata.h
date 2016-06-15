/*
 * matricedata.h
 *
 *  Created on: 15 juin 2016
 *      Author: boubad
 */

#ifndef MATRICEDATA_H_
#define MATRICEDATA_H_
/////////////////////////
#include "datavector_indivprovider.h"
/////////////////////////////////////
namespace info {
	/////////////////////////////
	template<typename U, typename STRINGTYPE, typename DATATYPE>
	class MatriceData {
	public:
		using IndexType = U;
		using IndivType = Indiv<U, STRINGTYPE>;
		using DataMap = std::map<U, InfoValue>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using SourceType = IIndivSource<U, STRINGTYPE>;
		using ints_vector = std::vector<U>;
		using doubles_vector = std::vector<double>;
		using strings_vector = std::vector<STRINGTYPE>;
		using indivptrs_vector = std::vector<IndivTypePtr>;
		using ints_doubles_map = std::map<U, double>;
		using DataVectorIndivSourceType = DataVectorIndivSource<U, STRINGTYPE>;
	private:
		size_t m_nrows;
		size_t m_ncols;
		strings_vector m_rownames;
		strings_vector m_colnames;
		std::vector<DATATYPE> m_data;
		STRINGTYPE m_name;
		std::unique_ptr<DataVectorIndivSourceType> m_inds;
		std::unique_ptr<DataVectorIndivSourceType> m_vars;
	public:
		MatriceData() :m_nrows(0), m_ncols(0) {
		}// MatriceData
		template <typename T>
		MatriceData(const STRINGTYPE sName, size_t nRows, size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames) : m_nrows(0), m_ncols(0) {
			this->initialize(sName, nRows, nCols, data, rowNames, colNames);
		}// MatriceData
		virtual ~MatriceData() {}
		template <typename T>
		void initialize(const STRINGTYPE sName, size_t nRows, size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames) {
			assert(nRows > 0);
			assert(nCols > 0);
			assert(data.size() >= (size_t)(nRows * nCols));
			assert(rowNames.size() >= nRows);
			assert(colNames.size() >= nCols);
			//
			this->m_name = sName;
			this->m_nrows = nRows;
			this->m_ncols = nCols;
			this->m_rownames = rowNames;
			this->m_colnames = colNames;
			std::vector<DATATYPE> &vv = this->m_data;
			vv.resize(nRows * nCols);
			doubles_vector vdata(nCols * nRows), variances(nCols);
			ints_vector indIds(nRows), varIds(nCols);
			ints_doubles_map weights, varWeights;
			double fsum = 0.0;
			for (size_t i = 0; i < nCols; ++i) {
				U key = (U)(i + 1);
				double s1 = 0, s2 = 0;
				varIds[i] = key;
				for (size_t j = 0; j < nRows; ++j) {
					T xx = data[j * nCols + i];
					vv[j * nCols + i] = (DATATYPE)xx;
					double x = (double)xx;
					vdata[i * nRows + j] = x;
					s1 += x;
					s2 += x *x;
				}// j
				s1 /= nRows;
				s2 /= nRows;
				s2 = s2 - (s1 * s1);
				assert(s2 > 0.0);
				double f = 1.0 / s2;
				weights[key] = f;
				fsum += f;
			}// i
			for (auto &&p : weights) {
				//U key = p.first;
				double f = p.second;
				p.second = f / fsum;
			}// p
			double ff = 1.0 / (double)nRows;
			for (size_t i = 0; i < nRows; ++i) {
				U key = (U)(i + 1);
				indIds[i] = key;
				varWeights[key] = ff;
			}// i
			this->m_inds.reset(new DataVectorIndivSourceType(nRows, nCols, data, indIds, varIds, rowNames, weights));
			DataVectorIndivSourceType *pi = this->m_inds.get();
			assert(pi != nullptr);
			pi->recode(1000);
			this->m_vars.reset(new  DataVectorIndivSourceType(nCols, nRows, vdata, varIds, indIds, colNames, varWeights));
			DataVectorIndivSourceType *pv = this->m_vars.get();
			assert(pv != nullptr);
			pv->recode(1000);
		}// initialize
	public:
		const STRINGTYPE &name(void) const {
			return (this->m_name);
		}
		size_t rows_count(void) const {
			return (this->m_nrows);
		}
		size_t cols_count(void) const {
			return (this->m_ncols);
		}
		const std::vector<DATATYPE> &data(void) const {
			return (this->m_data);
		}
		const strings_vector & cols_names(void) const {
			return (this->m_colnames);
		}
		const strings_vector & rows_names(void) const {
			return (this->m_rownames);
		}
		DataVectorIndivSourceType *indiv_provider(void) const {
			return (this->m_inds.get());
		}
		DataVectorIndivSourceType *variable_provider(void) const {
			return (this->m_vars.get());
		}
	};// class MatriceData<U,STRINGTYPE>
	///////////////////////////////
}// namespace info
//////////////////////////////////////////////
/////////////////////////////////////
#endif /* MATRICEDATA_H_ */
