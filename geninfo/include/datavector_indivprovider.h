#pragma once
#ifndef __DATAVECTOR_INDIVPROVIDER_H__
#define __DATAVECTOR_INDIVPROVIDER_H__
//////////////////////////////////////////////
#include "indiv.h"
////////////////////////////////////////////////
namespace info {
	/////////////////////////////
	template<typename U, typename STRINGTYPE>
	class DataVectorIndivSource : public IIndivSource<U, STRINGTYPE>, private boost::noncopyable {
		using mutex_type = std::mutex;
		using lock_type = std::lock_guard<mutex_type>;
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
	private:
		std::atomic<size_t> m_current;
		std::atomic<size_t> m_nrows;
		size_t m_ncols;
		doubles_vector m_data;
		ints_vector m_ids;
		ints_vector m_varids;
		strings_vector m_names;
		indivptrs_vector m_inds;
		ints_doubles_map m_weights;
		//
		mutex_type _mutex;
	public:
		template <typename T>
		DataVectorIndivSource(const size_t nRows, const size_t nCols, const std::vector<T> &data,
			const ints_vector &indIds, const ints_vector &varIds, const strings_vector &names, 
			const ints_doubles_map &oWeights, bool bComputeWeights = false) :m_current(0), m_nrows(nRows), m_ncols(nCols),
			m_data(nRows * nCols), m_ids(nRows), m_varids(nCols), m_names(nRows), m_inds(nRows), m_weights(oWeights) {
			assert(nRows > 0);
			assert(nCols > 0);
#ifndef NDEBUG
			size_t nn = (size_t)(nRows * nCols);
			assert(data.size() >= nn);
#endif
			assert(indIds.size() >= nRows);
			assert(varIds.size() >= nCols);
			assert(names.size() >= nRows);
			ints_doubles_map &ww = this->m_weights;
			ints_doubles_map temp;
			if (bComputeWeights) {
				ww.clear();
				double somme = 0.0;
				bool bOk = true;
				for (size_t i = 0; i < nCols; ++i) {
					double s1 = 0.0;
					double s2 = 0.0;
					for (size_t j = 0; j < nRows; ++j) {
						double x = (double)data[j * nCols + i];
						s1 += x;
						s2 += x * x;
					}// j
					s1 /= nRows;
					s2 = (s2 / nRows) - (s1 * s1);
					if (s2 <= 0.0) {
						bOk = false;
						break;
					}
					U key = varIds[i];
					double ff = 1.0 / s2;
					temp[key] = ff;
					somme += ff;
				}// i
				if (!bOk) {
					ww.clear();
				}
				else {
					for (auto &p : temp) {
						ww[p.first] = p.second / somme;
					}// it
				}
			}//
			doubles_vector &vv = this->m_data;
			for (size_t i = 0; i < nRows; ++i) {
				(this->m_ids)[i] = indIds[i];
				(this->m_names)[i] = names[i];
				for (size_t j = 0; j < nCols; ++j) {
					size_t pos = (size_t)(i * nCols + j);
					vv[pos] = (double)data[pos];
				}// j
			}// i
			for (size_t i = 0; i < nCols; ++i) {
				(this->m_varids)[i] = varIds[i];
			}
		}
		virtual ~DataVectorIndivSource(){}
	public:
		bool recode(double vMax = 1000.0, double vMin = 0.0) {
			if (vMax <= vMin) {
				return (false);
			}
			doubles_vector &vv = this->m_data;
			double v1 = 0, v2 = 0;
			auto it = std::min_element(vv.begin(), vv.end());
			if (it != vv.end()) {
				v1 = *it;
			}
			auto jt = std::max_element(vv.begin(), vv.end());
			if (jt != vv.end()) {
				v2 = *jt;
			}
			if (v1 >= v2) {
				return (false);
			}
			double delta = (vMax - vMin) / (v2 - v1);
			std::transform(vv.begin(), vv.end(), vv.begin(), [vMin, v1, delta](const double &v)->double {
				return ((v - v1) * delta + vMin);
			});
			return (true);
		}// recode
		virtual void set_weights(const ints_doubles_map &oWeights) {
			lock_type oLock(this->_mutex);
			this->m_weights = oWeights;
		}// weights
		virtual void weights(ints_doubles_map &oWeights) {
			oWeights = this->m_weights;
		}
		virtual size_t count(void) {
			return (this->m_nrows.load());
		}
		virtual IndivTypePtr get(const size_t pos) {
			IndivTypePtr oRet;
			if (pos >= this->m_nrows.load()) {
				return (oRet);
			}
			indivptrs_vector &vv = this->m_inds;
			oRet = vv[pos];
			if (oRet.get() != nullptr) {
				return (oRet);
			}
			DataMap oMap;
			const size_t nCols = this->m_ncols;
			const size_t base_pos = (size_t)(pos * nCols);
			ints_vector &varids = this->m_varids;
			doubles_vector &data = this->m_data;
			for (size_t i = 0; i < nCols; ++i) {
				double x = data[base_pos + i];
				InfoValue v(x);
				U key = varids[i];
				oMap[key] = v;
			}
			U aIndex = (this->m_ids)[pos];
			STRINGTYPE sSigle = (this->m_names)[pos];
			oRet.reset(new IndivType(aIndex, oMap, sSigle));
			{
				lock_type oLock(this->_mutex);
				vv[pos] = oRet;
			}
			return (oRet);
		}
		virtual void reset(void) {
			lock_type oLock(this->_mutex);
			m_current = 0;
		}
		virtual IndivTypePtr next(void) {
			size_t n = this->m_current.load();
			if (n < this->m_ids.size()) {
				this->m_current.store((size_t)(n + 1));
				return (this->get(n));
			}
			return (IndivTypePtr());
		}
		virtual IndivTypePtr find(const IndexType aIndex) {
			ints_vector &vv = this->m_ids;
			const size_t n = vv.size();
			for (size_t i = 0; i < n; ++i) {
				if (vv[i] == aIndex) {
					return (this->get(i));
				}
			}// i
			return (IndivTypePtr());
		}
	};
	///////////////////////////////////////
}// namespace info
//////////////////////////////////////////////
#endif // !__DATAVECTOR_INDIVPROVIDER_H__
