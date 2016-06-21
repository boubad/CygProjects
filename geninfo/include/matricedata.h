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
	template<typename U, typename STRINGTYPE>
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
	public:
		MatriceData() :m_ok(false), m_done(false), m_nrows(0), m_ncols(0) {
		}// MatriceData
		template <typename T>
		MatriceData(const STRINGTYPE sName, size_t nRows, size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames) : m_ok(false), m_done(false), m_nrows(0), m_ncols(0) {
			this->m_future = this->initializeAsync(sName, nRows, nCols, data, rowNames, colNames);
		}// MatriceData
		virtual ~MatriceData() {}
		template <typename T>
		std::future<bool> initializeAsync(const STRINGTYPE sName, size_t nRows, size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames) {
			return std::async(std::launch::async, [this, sName, nRows, nCols, data, rowNames, colNames]() {
				return this->initialize(sName, nRows, nCols, data, rowNames, colNames);
			});
		}// initializeAsync
	public:
		const STRINGTYPE &name(void) {
			this->check();
			return (this->m_name);
		}
		size_t rows_count(void)  {
			this->check();
			return (this->m_nrows);
		}
		size_t cols_count(void)  {
			this->check();
			return (this->m_ncols);
		}
		const doubles_vector &data(void)  {
			this->check();
			return (this->m_fdata);
		}
		const strings_vector & cols_names(void)  {
			this->check();
			return (this->m_colnames);
		}
		const strings_vector & rows_names(void)  {
			this->check();
			return (this->m_rownames);
		}
		const doubles_vector & variables_summary(void)  {
			this->check();
			return (this->m_fvarsum);
		}
		const doubles_vector & indivs_summary(void)  {
			this->check();
			return (this->m_findsum);
		}
		DataVectorIndivSourceType *indiv_provider(void)  {
			this->check();
			return (this->m_inds.get());
		}
		DataVectorIndivSourceType *variable_provider(void)  {
			this->check();
			return (this->m_vars.get());
		}
	private:
		std::atomic<bool> m_ok;
		std::atomic<bool> m_done;
		size_t m_nrows;
		size_t m_ncols;
		strings_vector m_rownames;
		strings_vector m_colnames;
		doubles_vector m_fdata;
		doubles_vector m_fvarsum;
		doubles_vector m_findsum;
		ints_vector m_varids;
		ints_vector m_indids;
		STRINGTYPE m_name;
		std::unique_ptr<DataVectorIndivSourceType> m_inds;
		std::unique_ptr<DataVectorIndivSourceType> m_vars;
		std::future<bool> m_future;
	private:
		template <typename T>
		bool initialize(const STRINGTYPE sName, size_t nRows, size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames) {
			this->clear();
			if (sName.empty() || (nRows < 1) || (nCols < 1) || (data.size() < (size_t)(nCols *nRows)) ||
				(rowNames.size() < nRows) || (colNames.size() < nCols)) {
				return (false);
			}
			this->m_ok.store(true);
			this->m_name = sName;
			this->m_nrows = nRows;
			this->m_ncols = nCols;
			this->m_rownames = rowNames;
			this->m_colnames = colNames;
			ints_vector varids(nCols), indids(nRows);
			const size_t nTotal = (size_t)(nCols * nRows);
			doubles_vector &vdata = this->m_fdata;
			doubles_vector &variances = this->m_fvarsum;
			variances.resize(nCols);
			this->m_findsum.resize(nRows);
			vdata.resize(nTotal);
			ints_doubles_map varWeights, indWeights;
			double fsum = 0.0;
			for (size_t i = 0; i < nCols; ++i) {
				U key = (U)(i + 1);
				double s1 = 0, s2 = 0;
				varids[i] = key;
				double fmin = 0, fmax = 0;
				for (size_t j = 0; j < nRows; ++j) {
					const size_t pos = (size_t)(j * nCols + i);
					T xx = data[pos];
					double x = (double)xx;
					s1 += x;
					s2 += x *x;
					if (j == 0) {
						fmin = x;
						fmax = x;
					}
					else if (x < fmin) {
						fmin = x;
					}
					else if (x > fmax) {
						fmax = x;
					}
				}// j
				if (fmin >= fmax) {
					this->clear();
					return (false);
				}
				s1 /= (double)nRows;
				s2 /= (double)nRows;
				s2 = s2 - (s1 * s1);
				if (s2 <= 0) {
					this->clear();
					return (false);
				}
				double f = 1.0 / s2;
				variances[i] = f;
				fsum += f;
				double delta = fmax - fmin;
				for (size_t j = 0; j < nRows; ++j) {
					const size_t pos = (size_t)(j * nCols + i);
					double xx = ((double)data[pos] - fmin) / delta;
					vdata[pos] = xx;
				}// j
			}// i
			if (fsum <= 0) {
				this->clear();
				return (false);
			}
			for (size_t i = 0; i < nCols; ++i) {
				variances[i] = variances[i] / fsum;
				U key = (U)(i + 1);
				varWeights[key] = variances[i];
			}
			double ff = 1.0 / (double)nRows;
			doubles_vector ftemp(nTotal);
			for (size_t i = 0; i < nRows; ++i) {
				U key = (U)(i + 1);
				indids[i] = key;
				indWeights[key] = ff;
				double ftotal = 0;
				for (size_t j = 0; j < nCols; ++j) {
					const size_t pos = (size_t)(i * nCols + j);
					double xx = vdata[pos];
					ftotal += xx;
					ftemp[j * nRows + i] = xx;
				}// j
				this->m_findsum[i] = ftotal / nCols;
			}// i
			this->m_inds.reset(new DataVectorIndivSourceType(nRows, nCols, vdata, indids, varids, rowNames, varWeights));
			DataVectorIndivSourceType *pi = this->m_inds.get();
			if (pi == nullptr) {
				this->clear();
				return (false);
			}
			pi->recode(1000);
			this->m_vars.reset(new  DataVectorIndivSourceType(nCols, nRows, ftemp, varids, indids, colNames, indWeights));
			DataVectorIndivSourceType *pv = this->m_vars.get();
			if (pv == nullptr) {
				this->clear();
				return (false);
			}
			pv->recode(1000);
			this->m_done.store(true);
			return (true);
		}// initialize
		void clear(void) {
			this->m_ok.store(false);
			this->m_done.store(false);
			this->m_nrows = 0;
			this->m_ncols = 0;
			this->m_rownames.clear();
			this->m_colnames.clear();
			this->m_fdata.clear();
			this->m_fvarsum.clear();
			this->m_findsum.clear();
			this->m_varids.clear();
			this->m_indids.clear();
		}// clear
		void check(void) {
			if (!this->m_done.load()) {
				if (this->m_ok.load()) {
					this->m_future.wait();
					this->m_done.store(true);
				}
			}// not done
		}// check
	};// class MatriceData<U,STRINGTYPE>
	///////////////////////////////
}// namespace info
//////////////////////////////////////////////
/////////////////////////////////////
#endif /* MATRICEDATA_H_ */
