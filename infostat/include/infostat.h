#pragma once
#ifndef __INFOSTAT_H__
#define __INFOSTAT_H__
//////////////////////////////
#include "indiv.h"
#include "stringconvert.h"
//////////////////////////////////
namespace info {
	//////////////////////////////////
	enum class TransformationType {
		noTransf, recode, recode1000, recode10000, normalize
	};
	// enum class TransformationType
	/////////////////////////////
	template<typename U = unsigned long, typename STRINGTYPE = std::string>
	class StatInfo {
	public:
		using IntType = U;
		using strings_sizet_map = std::map<STRINGTYPE, size_t>;
		using StatInfoType = StatInfo<U, STRINGTYPE>;
	private:
		IntType index;
		size_t count;
		double min;
		double max;
		double somme1;
		double somme2;
		strings_sizet_map m_modalsmap;
	public:
		StatInfo() :
			index(0), count(0), min(0), max(0), somme1(0), somme2(0) {
		}
		StatInfo(const U aIndex, double x) :
			index(aIndex), count(1), min(x), max(x), somme1(x), somme2(x * x) {
		}
		StatInfo(const U aIndex, const InfoValue &v) :
			index(aIndex), count(0), min(0), max(0), somme1(0), somme2(0) {
			if (!v.empty()) {
				if (v.is_numerical()) {
					double x = 0;
					v.get_value(x);
					this->count = 1;
					this->min = x;
					this->max = x;
					this->somme1 = x;
					this->somme2 = x * x;
				}
				else if (v.is_text()) {
					STRINGTYPE s;
					v.get_value(s);
					STRINGTYPE ss = info_trim_upper(s);
					if (!ss.empty()) {
						this->count = 1;
						size_t nv = 0;
						this->m_modalsmap[ss] = nv;
					} // not empty
				}
			} // not empty
		}
		StatInfo(const StatInfoType &other) :
			index(other.index), count(other.count), min(other.min), max(
				other.max), somme1(other.somme1), somme2(other.somme2), m_modalsmap(
					other.m_modalsmap) {
		}
		StatInfoType & operator=(const StatInfoType &other) {
			if (this != &other) {
				this->index = other.index;
				this->count = other.count;
				this->min = other.min;
				this->max = other.max;
				this->somme1 = other.somme1;
				this->somme2 = other.somme2;
				this->m_modalsmap = other.m_modalsmap;
			}
			return (*this);
		}
		virtual ~StatInfo() {
		}
		bool operator==(const StatInfoType &other) const {
			return (this->index == other.index);
		}
		bool operator<(const StatInfoType &other) const {
			return (this->index < other.index);
		}
		IntType operator()(void) const {
			return (this->index);
		}
		IntType key(void) const {
			return (this->index);
		}
	public:
		const strings_sizet_map & get_modals_map(void) const {
			return (this->m_modalsmap);
		}
		size_t get_count(void) const {
			return (this->count);
		}
		double get_min(void) const {
			return (this->min);
		}
		double get_max(void) const {
			return (this->max);
		}
		double get_mean(void) const {
			double d = this->somme1;
			const size_t n = this->count;
			if (n > 1) {
				d /= n;
			}
			return (d);
		}
		double get_variance(void) const {
			double d1 = this->somme1;
			double d2 = this->somme2;
			const size_t n = this->count;
			if (n > 1) {
				d1 /= n;
				d2 /= n;
			}
			double d = d2 - (d1 * d1);
			return (d);
		}
		double get_deviation(void) const {
			double d = this->get_variance();
			if (d > 0) {
				d = std::sqrt(d);
			}
			return (d);
		}
		void reset(void) {
			this->count = 0;
			this->min = 0;
			this->max = 0;
			this->somme1 = 0;
			this->somme2 = 0;
			this->m_modalsmap.clear();
		}
		void add(const double x) {
			if (this->count < 1) {
				this->count = 1;
				this->min = x;
				this->max = x;
				this->somme1 = x;
				this->somme2 = x * x;
			}
			else {
				this->count++;
				this->somme1 += x;
				this->somme2 += x * x;
				if (x < this->min) {
					this->min = x;
				}
				if (x > this->max) {
					this->max = x;
				}
			}
		} // add
		void add(const InfoValue &v) {
			if (!v.empty()) {
				if (v.is_numerical()) {
					double x = 0;
					v.get_value(x);
					this->add(x);
				}
				else if (v.is_text()) {
					STRINGTYPE s;
					v.get_value(s);
					STRINGTYPE ss = info_trim_upper(s);
					if (!ss.empty()) {
						this->count++;
						strings_sizet_map &oMap = this->m_modalsmap;
						if (oMap.find(ss) == oMap.end()) {
							size_t nval = oMap.size();
							oMap[ss] = nval;
						}
					} // not empty
				}
			} // not empty
		} // add
		bool transform(const InfoValue &src, InfoValue &dest,
			const TransformationType mode = TransformationType::noTransf) const {
			if (src.empty() || (this->count < 1)) {
				dest = InfoValue();
				return (false);
			}
			if (src.is_text()) {
				STRINGTYPE s;
				src.get_value(s);
				STRINGTYPE ss = info_trim_upper(s);
				const strings_sizet_map &oMap = this->m_modalsmap;
				auto it = oMap.find(ss);
				if (it != oMap.end()) {
					int nv = (int)(*it).second;
					dest = InfoValue(nv);
					return (true);
				}
				dest = InfoValue();
				return (false);
			}
			bool bRet = false;
			if (src.is_numerical()) {
				double x = 0;
				src.get_value(x);
				switch (mode) {
				case TransformationType::recode: {
					if (this->min < this->max) {
						if ((x >= this->min) && (x <= this->max)) {
							double dd = (x - this->min) / (this->max - this->min);
							dest = InfoValue(dd);
							bRet = true;
						}
					} // min
				}
												 break;
				case TransformationType::recode1000: {
					if (this->min < this->max) {
						if ((x >= this->min) && (x <= this->max)) {
							double dd = ((x - this->min) / (this->max - this->min))*1000.0 + 0.5;
							int nx = (int)dd;
							dest = InfoValue(nx);
							bRet = true;
						}
					} // min
				}
													 break;
				case TransformationType::recode10000: {
					if (this->min < this->max) {
						if ((x >= this->min) && (x <= this->max)) {
							double dd = ((x - this->min) / (this->max - this->min))*10000.0 + 0.5;
							int nx = (int)dd;
							dest = InfoValue(nx);
							bRet = true;
						}
					} // min
				}
													  break;
				case TransformationType::normalize: {
					double s = this->get_deviation();
					if (s > 0) {
						double m = this->get_mean();
						double dd = (x - m) / s;
						dest = InfoValue(dd);
						bRet = true;
					}
				}
													break;
				default:
					break;
				} // mode
				if (!bRet) {
					dest = InfoValue();
				}
				return (bRet);
			}
			dest = InfoValue();
			return (false);
		} // transform
	};
	// class StatInfo<U>
	/////////////////////////////////
	template<typename U = unsigned long, typename STRINGTYPE = std::string>
	class StatSummator : public InterruptObject {
	public:
		using StatInfoType = StatInfo<U, STRINGTYPE>;
		using entries_map = std::map<U, StatInfoType>;
		using StatSummatorType = StatSummator<U>;
		using IndivType = Indiv<U>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
	private:
		entries_map m_map;
	public:
		StatSummator(std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel) {
		}
		StatSummator(const StatSummatorType &other) :
			InterruptObject(other), m_map(other.m_map) {
		}
		StatSummatorType & operator=(const StatSummatorType &other) {
			if (this != &other) {
				InterruptObject::operator=(other);
				this->m_map = other.m_map;
			}
			return (*this);
		}
		virtual ~StatSummator() {
		}
	public:
		bool has_key(const U key) const {
			const entries_map &m = this->m_map;
			return (m.find(key) != m.end());
		}
		bool get(const U key, StatInfoType &oInfo) const {
			const entries_map &m = this->m_map;
			auto it = m.find(key);
			if (it != m.end()) {
				oInfo = (*it).second;
				return (true);
			}
			return (false);
		}
		void clear(void) {
			this->m_map.clear();
		}
		template<typename XU>
		void add(const std::map<XU, InfoValue> &oMap) {
			entries_map &m = this->m_map;
			for (auto kt = oMap.begin(); kt != oMap.end(); ++kt) {
				if (this->check_interrupt()) {
					return;
				}
				const std::pair<XU, InfoValue> &p = *kt;
				const InfoValue &v = p.second;
				if (!v.empty()) {
					U key = (U)p.first;
					auto it = m.find(key);
					if (it != m.end()) {
						StatInfoType &e = (*it).second;
						e.add(v);
					}
					else {
						m[key] = StatInfoType(key, v);
					}
				}	// v
			}	// kt
		}	// add
		template<typename XU>
		void add(const Indiv<XU> &oInd) {
			this->add(oInd.center());
		}	// ass
		template<typename XU>
		void add(const std::shared_ptr<Indiv<XU> > &oPtr) {
			const Indiv<XU> *p = oPtr.get();
			if (p != nullptr) {
				this->add(p->center());
			}
		}	// add
		template<typename XU>
		bool transform(const std::map<XU, InfoValue> &src,
			std::map<U, InfoValue> &dest, const TransformationType mode =
			TransformationType::noTransf) const {
			dest.clear();
			const entries_map &m = this->m_map;
			std::for_each(src.begin(), src.end(),
				[&](const std::pair<XU, InfoValue> &p) {
				const InfoValue &v = p.second;
				if (!v.is_empty()) {
					const U key = (U)p.first;
					auto it = m.find(key);
					if (it != m.end()) {
						const StatInfoType &info = (*it).second;
						InfoValue vr;
						if (info.transform(v, vr, mode)) {
							dest[key] = vr;
						}
					}	// it
				}	// not empty
			});
			return (!dest.empty());
		}	// transform
		bool transform(IndivType &oInd, const TransformationType mode =
			TransformationType::noTransf) const {
			std::map<U, InfoValue> dest;
			if (!this->transform(oInd.center(), dest, mode)) {
				return (false);
			}
			oInd.center(dest);
			return (!oInd.empty());
		}	// transform
		IndivTypePtr transform(const IndivTypePtr &oSrc,
			const TransformationType mode = TransformationType::noTransf) const {
			IndivTypePtr oRet;
			IndivType *pInd = oSrc.get();
			if (pInd != nullptr) {
				oRet = std::make_shared<IndivType>(*pInd);
				IndivType *pRet = oRet.get();
				if (pRet != nullptr) {
					if (this->transform(*pRet, mode)) {
						return (oRet);
					}
				}
			}	// pInd
			return (oRet);
		}	// transform
	};
	// class IndivSummaror<U>
	////////////////////////////////
}// namespace info
/////////////////////////////////
#endif // !__INFOSTAT_H__
