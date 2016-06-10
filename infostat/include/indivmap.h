/*
 * indivmap.h
 *
 *  Created on: 4 juin 2016
 *      Author: boubad
 */

#ifndef INDIVMAP_H_
#define INDIVMAP_H_
///////////////////////////
#include "indiv.h"
#include "distancemap.h"
/////////////////////////////
namespace info {
///////////////////////////
template<typename U = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
class IndivMap {
public:
	using IndexType = U;
	using IndivType = Indiv<U, STRINGTYPE>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using indivsmap_type = std::map<U,IndivTypePtr>;
	using IndivMapType = IndivMap<U,STRINGTYPE>;
	using iterator = typename indivsmap_type::iterator;
	using const_iterator = typename indivsmap_type::const_iterator;
	using DistanceMapType = DistanceMap<U, double>;
	using ints_vector = std::vector<U>;
	using ints_doubles_map = std::map<U, double>;
	using SourceType = IIndivSource<U, STRINGTYPE>;
private:
	double m_fmin;
	double m_fmax;
	DISTANCETYPE m_zmax;
	indivsmap_type m_map;
	DistanceMapType m_dist;
	ints_vector m_ids;
	ints_doubles_map m_weights;
public:
	IndivMap() :
			m_fmin(0), m_fmax(0), m_zmax(1000) {
	}
	virtual ~IndivMap() {
	}
public:
	size_t size(void){
		return (this->m_dist.size());
	}
	iterator begin(void) {
		return (this->m_map.begin());
	}
	const_iterator begin(void) const {
		return (this->m_map.begin());
	}
	iterator end(void) {
		return (this->m_map.end());
	}
	const_iterator end(void) const {
		return (this->m_map.end());
	}
	IndivTypePtr get(const U aIndex) {
		IndivTypePtr oRet;
		const indivsmap_type &m = this->m_map;
		auto it = m.find(aIndex);
		if (it != m.end()) {
			oRet = (*it).second;
		}
		return (oRet);
	} // get
	bool put(IndivTypePtr oInd) {
		IndivType *pInd = oInd.get();
		if (pInd == nullptr) {
			return (false);
		}
		U aIndex = pInd->id();
		indivsmap_type &m = this->m_map;
		m[aIndex] = oInd;
		ints_vector &ids = this->m_ids;
		auto it = std::find(ids.begin(), ids.end(), aIndex);
		if (it == ids.end()) {
			ids.push_back(aIndex);
		}
		DistanceMapType &oDist = this->m_dist;
		ints_doubles_map &weights = this->m_weights;
		for (auto &xIndex : ids) {
			if (xIndex != aIndex) {
				IndivTypePtr xind = this->get(xIndex);
				IndivType *p = xind.get();
				double d = 0;
				if (info_global_compute_distance(pInd->center(), p->center(),
						weights, d)) {
					oDist.add(aIndex, xIndex, d);
					if (oDist.size() < 3) {
						this->m_fmin = d;
						this->m_fmax = d;
					} else if (d < this->m_fmin) {
						this->m_fmin = d;
					} else if (d > this->m_fmax) {
						this->m_fmax = d;
					}
				} // distance
			} //compute
		} // xIndex
		return (true);
	} // put
};
/////////////////////////////
}// namespace info
//////////////////////
#endif /* INDIVMAP_H_ */
