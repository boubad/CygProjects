/*
 * indivcluster.cpp
 *
 *  Created on: 9 mai 2016
 *      Author: boubad
 */
#include "../include/indivcluster.h"
#include "../include/crititem.h"
 ////////////////////////////////
namespace info {
	////////////////////////////////////
	extern size_t info_global_clusterize_kmeans(IIndivProvider *pProvider,
		const size_t nbClusters, indivclusters_vector &oRes,
		const size_t nbMaxIterations /*= 100*/) {
		assert(pProvider != nullptr);
		assert(pProvider->is_valid());
		assert(nbClusters > 1);
		assert(nbMaxIterations > 0);
		//
		typedef std::map<IntType, size_t> sizet_map;
		typedef std::pair<IntType, size_t> int_sizet_pair;
		//
		size_t nbIndivs = 0;
		if (!pProvider->indivs_count(nbIndivs)) {
			return (0);
		}
		if (nbClusters > nbIndivs) {
			return (false);
		}
		oRes.resize(nbClusters);
		info_indivs_vector oInitial;
		if (!info::info_global_get_random_indivs(nbClusters, pProvider, oInitial)) {
			return (0);
		}
		for (size_t i = 0; i < nbClusters; ++i) {
			IndivCluster c(pProvider, i);
			c.add(oInitial[i]);
			c.update_center();
			oRes[i] = c;
		} // i
		size_t iter = 0;
		int ni = (int)nbIndivs;
		ints_size_t_map oldMap;
		while (iter < nbMaxIterations) {
			//
			std::for_each(oRes.begin(), oRes.end(), [](IndivCluster &c) {
				c.clear_members();
			});
			for (int i = 0; i < ni; ++i) {
				CritItem oCritRes;
				Indiv oInd;
				if (pProvider->find_indiv_at(i, oInd)) {
					for (size_t j = 0; j < nbClusters; ++j) {
						IndivCluster &c = oRes[j];
						double d = c.distance(oInd);
						CritItem cur(j, j + 1, d);
						oCritRes += cur;
					} // j
					std::pair<size_t, size_t> oPair;
					oCritRes.get(oPair);
					size_t iRes = oPair.first;
					if ((iRes >= 0) && (iRes < nbClusters)) {
						IndivCluster &cc = oRes[iRes];
						cc.add(oInd);
					}
				} // ind
			} // i
			ints_size_t_map curMap;
			std::for_each(oRes.begin(), oRes.end(), [&](IndivCluster &c) {
				c.update_center();
				c.get_map(curMap);
			});
			if (oldMap.empty()) {
				oldMap = curMap;
				continue;
			}
			//
			bool done = true;
			for (auto it = oldMap.begin(); it != oldMap.end(); ++it) {
				const IntType key = (*it).first;
				const size_t val = (*it).second;
				if (curMap.find(key) == curMap.end()) {
					return (iter);
				}
				if (curMap[key] != val) {
					done = false;
					break;
				}
			} // it
			++iter;
			oldMap = curMap;
			if (done) {
				break;
			}
		} // iter
		return (iter);
	}		//info_global_clusterize_kmeans
	extern void info_global_add_clusterize_result(const indivclusters_vector &oClusters, InfoCritItems &oInfos) {
		ints_size_t_map curMap;
		std::for_each(oClusters.begin(), oClusters.end(), [&curMap](const IndivCluster &c) {
			c.get_map(curMap);
		});
		ints_set oSet;
		for (auto it = curMap.begin(); it != curMap.end(); ++it) {
			const IntType aIndex = (*it).first;
			oSet.insert(aIndex);
		}// it
		for (auto it = oSet.begin(); it != oSet.end(); ++it) {
			const IntType aIndex1 = *it;
			const size_t cluster1 = curMap[aIndex1];
			for (auto jt = oSet.begin(); jt != it; ++jt) {
				const IntType aIndex2 = *jt;
				const size_t cluster2 = curMap[aIndex2];
				if (cluster1 == cluster2) {
					double v = 1;
					if (oInfos.get(aIndex2, aIndex1, v)) {
						v = v + 1.0;
					}
					oInfos.add(aIndex2, aIndex1, v);
				}// equal
			}// jt
		}// it
	}//info_global_add_clusterize_result
	///////////////////////////////////
	IndivCluster::IndivCluster() :
		m_mustdelete(false), m_index(0), m_provider(nullptr), m_pdist(nullptr) {

	}
	IndivCluster::IndivCluster(IndivDistanceMap *pDist, const size_t aIndex) :
		m_mustdelete(false), m_index(aIndex), m_provider(nullptr), m_pdist(
			pDist) {

	}
	IndivCluster::IndivCluster(IIndivProvider *pProvider, const size_t aIndex) :
		m_mustdelete(false), m_index(aIndex), m_provider(pProvider), m_pdist(
			nullptr) {
	}
	IndivCluster::IndivCluster(IIndivProvider *pProvider, const size_t aIndex, const DbValueMap &oCenter):
		m_mustdelete(false), m_index(aIndex), m_provider(pProvider), m_pdist(nullptr) {
		this->m_center = oCenter;
	}
	IndivCluster::IndivCluster(const IndivCluster &other) :
		m_mustdelete(false), m_index(other.m_index), m_provider(
			other.m_provider), m_pdist(nullptr), m_individs(
				other.m_individs), m_center(other.m_center), cache_dist(other.cache_dist), m_sommes(other.m_sommes), m_counts(other.m_counts) {
		if (!other.m_mustdelete) {
			this->m_pdist = other.m_pdist;
		}
	}
	IndivCluster & IndivCluster::operator=(const IndivCluster &other) {
		if (this != &other) {
			if (this->m_mustdelete) {
				delete this->m_pdist;
				this->m_pdist = nullptr;
				this->m_mustdelete = false;
			}
			this->m_index = other.m_index;
			this->m_provider = other.m_provider;
			if (!other.m_mustdelete) {
				this->m_pdist = other.m_pdist;
			}
			this->m_individs = other.m_individs;
			this->m_center = other.m_center;
			this->cache_dist = other.cache_dist;
			this->m_sommes = other.m_sommes;
			this->m_counts = other.m_counts;
		}
		return (*this);
	}
	void IndivCluster::swap(IndivCluster &other) {
		IndivCluster t(*this);
		*this = other;
		other = t;
	} // swap
	IndivCluster::~IndivCluster() {
		if (this->m_mustdelete) {
			delete this->m_pdist;
			this->m_pdist = nullptr;
			this->m_mustdelete = false;
		}
	}
	void IndivCluster::get_map(ints_size_t_map &oMap) const {
		const ints_deque &vv = this->m_individs;
		const size_t n = this->m_index;
		std::for_each(vv.begin(), vv.end(), [n, &oMap](const IntType &key) {
			oMap[key] = n;
		});
	}//get_map
	bool IndivCluster::is_valid(void) const {
		if (m_pdist != nullptr) {
			return (true);
		}
		return ((this->m_provider != nullptr) && (this->m_provider->is_valid()));
	}
	bool IndivCluster::get_distance(const IntType aIndex1, const IntType &aIndex2,
		double &dRes) const {
		if (this->m_pdist != nullptr) {
			if (this->m_pdist->get(aIndex1, aIndex2, dRes)) {
				return (true);
			}
		}
		IIndivProvider *pProvider = this->m_provider;
		if (pProvider == nullptr) {
			return (false);
		}
		Indiv oInd1;
		if (!pProvider->find_indiv(aIndex1, oInd1, VariableMode::modeNumeric)) {
			return (false);
		}
		Indiv oInd2;
		if (!pProvider->find_indiv(aIndex2, oInd2, VariableMode::modeNumeric)) {
			return (false);
		}
		dRes = oInd1.distance(oInd2);
		IndivCluster &o = const_cast<IndivCluster &>(*this);
		if (this->m_pdist == nullptr) {
			o.m_pdist = new IndivDistanceMap();
			assert(o.m_pdist != nullptr);
			o.m_mustdelete = true;
		}
		o.m_pdist->add(aIndex1, aIndex2, dRes);
		return (true);
	} //get_distanc
	IndivDistanceMap *IndivCluster::distance_map(void) const {
		return (this->m_pdist);
	}
	void IndivCluster::distance_map(IndivDistanceMap *pMap) {
		this->m_pdist = pMap;
		this->m_mustdelete = false;
	}
	bool IndivCluster::is_empty(void) const {
		return (this->m_individs.empty());
	}
	IIndivProvider *IndivCluster::provider(void) const {
		return (this->m_provider);
	}
	void IndivCluster::provider(IIndivProvider *pProvider) {
		this->m_provider = pProvider;
	}
	void IndivCluster::index(const size_t n) {
		this->m_index = n;
	}
	size_t IndivCluster::index(void) const {
		return (this->m_index);
	}
	const ints_deque & IndivCluster::members(void) const {
		return (this->m_individs);
	}
	const DbValueMap &IndivCluster::center(void) const {
		return (this->m_center);
	}
	double IndivCluster::distance(const Indiv &oInd) const {
		const IntType aIndex = oInd.id();
		auto it = this->cache_dist.find(aIndex);
		if (it != this->cache_dist.end()) {
			double dRet = (*it).second;
			return dRet;
		}
		const DbValueMap & curData = this->m_center;
		const DbValueMap &m2 = oInd.data();
		double dRet = 0;
		size_t nc = 0;
		typedef std::pair<IntType, DbValue> MyPair;
		std::for_each(curData.begin(), curData.end(), [&](const MyPair &oPair) {
			const DbValue &v1 = oPair.second;
			if (!v1.empty()) {
				const IntType key = oPair.first;
				auto jt = m2.find(key);
				if (jt != m2.end()) {
					const DbValue &v2 = (*jt).second;
					if (!v2.empty()) {
						double t = v1.double_value() - v2.double_value();
						dRet += t * t;
						++nc;
					}// ok
				}// jt
			}// v1
		});
		if (nc > 1) {
			dRet /= nc;
		}
		IndivCluster &o = const_cast<IndivCluster &>(*this);
		ints_double_map &mm = o.cache_dist;
		mm[aIndex] = dRet;
		return dRet;
	}
	void IndivCluster::add(const Indiv &oInd) {
		const IntType aIndex = oInd.id();
		ints_deque &vv = this->m_individs;
		for (auto it = vv.begin(); it != vv.end(); ++it) {
			if ((*it) == aIndex) {
				return;
			}
		} // it
		if (!this->cache_dist.empty()) {
			this->cache_dist.clear();
		}
		vv.push_back(aIndex);
		const DbValueMap &oMap = oInd.data();
		ints_double_map &sommes = this->m_sommes;
		ints_size_t_map &counts = this->m_counts;
		std::for_each(oMap.begin(), oMap.end(), [&](const std::pair<IntType, DbValue> &oPair) {
			const DbValue &vv = oPair.second;
			if (!vv.empty()) {
				const double v = vv.double_value();
				const IntType key = oPair.first;
				size_t nCount = 1;
				auto it = counts.find(key);
				if (it == counts.end()) {
					counts[key] = nCount;
					sommes[key] = v;
				}
				else {
					nCount = (*it).second;
					++nCount;
					counts[key] = nCount;
					double ss = sommes[key];
					ss += v;
					sommes[key] = ss;
				}
			}// not empy
		});
	} // add
	double IndivCluster::distance(const IntType aIndex, ClusterDistanceMode &mode) {
		assert(this->is_valid());
		IIndivProvider *pProvider = this->m_provider;
		Indiv oInd;
		if (!pProvider->find_indiv(aIndex, oInd, VariableMode::modeNumeric)) {
			assert(false);
			return (false);
		}
		return this->distance(oInd, mode);
	} // distance
	double IndivCluster::distance(const Indiv &oInd, ClusterDistanceMode &mode) {
		assert(this->is_valid());
		IIndivProvider *pProvider = this->m_provider;
		ints_deque &vv = this->m_individs;
		const size_t n = vv.size();
		if (n < 1) {
			return (this->distance(oInd));
		}
		if (n == 1) {
			Indiv oInd2;
			if (!pProvider->find_indiv(vv[0], oInd2, VariableMode::modeNumeric)) {
				mode = ClusterDistanceMode::modeInvalid;
				assert(false);
				return (false);
			}
			return (oInd.distance(oInd2));
		}
		if (mode == ClusterDistanceMode::modeTop) {
			Indiv oInd2;
			if (!pProvider->find_indiv(vv[0], oInd2, VariableMode::modeNumeric)) {
				mode = ClusterDistanceMode::modeInvalid;
				assert(false);
				return (false);
			}
			return (oInd.distance(oInd2));
		}
		else if (mode == ClusterDistanceMode::modeBottom) {
			Indiv oInd2;
			if (!pProvider->find_indiv(vv[n - 1], oInd2,
				VariableMode::modeNumeric)) {
				mode = ClusterDistanceMode::modeInvalid;
				assert(false);
				return (false);
			}
			return (oInd.distance(oInd2));
		}
		mode = ClusterDistanceMode::modeInvalid;
		return (0);
	} // distance
	bool IndivCluster::min_distance(const IndivCluster &other, double &dRes,
		ClusterAppendMode &mode) const {
		const ints_deque &vv1 = this->m_individs;
		const ints_deque &vv2 = other.m_individs;
		const size_t n1 = vv1.size();
		const size_t n2 = vv2.size();
		if ((n1 < 1) || (n2 < 1)) {
			return (false);
		}
		else if ((n1 == 1) && (n2 == 1)) {
			mode = ClusterAppendMode::modeTopBottom;
			return (this->get_distance(vv1[0], vv2[0], dRes));
		}
		else if ((n1 == 1) && (n2 > 1)) {
			double d1, d2;
			if (!this->get_distance(vv1[0], vv2[0], d1)) {
				return (false);
			}
			if (!this->get_distance(vv1[0], vv2[n2 - 1], d2)) {
				return (false);
			}
			if (d1 < d2) {
				mode = ClusterAppendMode::modeTopTop;
				dRes = d1;
			}
			else {
				mode = ClusterAppendMode::modeTopBottom;
				dRes = d2;
			}
			return (true);
		}
		else if ((n1 > 1) && (n2 == 1)) {
			double d1, d2;
			if (!this->get_distance(vv1[0], vv2[0], d1)) {
				return (false);
			}
			if (!this->get_distance(vv1[n1 - 1], vv2[0], d2)) {
				return (false);
			}
			if (d1 < d2) {
				mode = ClusterAppendMode::modeTopTop;
				dRes = d1;
			}
			else {
				mode = ClusterAppendMode::modeBottomTop;
				dRes = d2;
			}
			return (true);
		}
		else {
			if (!this->get_distance(vv1[0], vv2[0], dRes)) {
				return (false);
			}
			mode = ClusterAppendMode::modeTopTop;
			double d;
			if (!this->get_distance(vv1[0], vv2[n2 - 1], d)) {
				return (false);
			}
			if (d < dRes) {
				dRes = d;
				mode = ClusterAppendMode::modeTopBottom;
			}
			if (!this->get_distance(vv1[n1 - 1], vv2[0], d)) {
				return (false);
			}
			if (d < dRes) {
				dRes = d;
				mode = ClusterAppendMode::modeBottomTop;
			}
			if (!this->get_distance(vv1[n1 - 1], vv2[n2 - 1], d)) {
				return (false);
			}
			if (d < dRes) {
				dRes = d;
				mode = ClusterAppendMode::modeBottomBottom;
			}
			return (true);
		}
	} // min_distance
	void IndivCluster::add(const IndivCluster &other,
		const ClusterAppendMode mode) {
		ints_deque &dest = this->m_individs;
		const size_t n1 = dest.size();
		const ints_deque &src = other.m_individs;
		const size_t n2 = src.size();
		if ((n1 < 1) || (n2 < 1)) {
			return;
		}
		switch (mode) {
		case ClusterAppendMode::modeTopBottom: {
			for (size_t i = (size_t)(n2 - 1); i >= 0; i--) {
				dest.push_front(src[i]);
				if (i == 0) {
					break;
				}
			}
		}
											   break;
		case ClusterAppendMode::modeTopTop: {
			for (size_t i = 0; i < n2; ++i) {
				dest.push_front(src[i]);
			}
		}
											break;
		case ClusterAppendMode::modeBottomTop: {
			for (size_t i = 0; i < n2; ++i) {
				dest.push_back(src[i]);
			}
		}
											   break;
		case ClusterAppendMode::modeBottomBottom: {
			for (size_t i = (size_t)(n2 - 1); i >= 0; i--) {
				dest.push_back(src[i]);
				if (i == 0) {
					break;
				}
			}
		}
												  break;
		default:
			break;
		} //mode
		if (!this->cache_dist.empty()) {
			this->cache_dist.clear();
		}
		ints_double_map &sommes1 = this->m_sommes;
		const ints_double_map &sommes2 = other.m_sommes;
		ints_size_t_map &counts1 = this->m_counts;
		const ints_size_t_map &counts2 = other.m_counts;
		std::for_each(counts2.begin(), counts2.end(), [&](const std::pair<IntType, size_t> &p) {
			const IntType key = p.first;
			size_t n = p.second;
			auto jt = sommes2.find(key);
			assert(jt != sommes2.end());
			double v = (*jt).second;
			auto it = counts1.find(key);
			if (it == counts1.end()) {
				counts1[key] = n;
				sommes1[key] = v;
			}
			else {
				size_t n1 = (*it).second;
				n1 += n;
				counts1[key] = n1;
				double ss = sommes1[key];
				ss += v;
				sommes1[key] = ss;
			}
		});
	} //add
	void IndivCluster::clear_members(void) {
		this->m_individs.clear();
		this->cache_dist.clear();
		this->m_sommes.clear();
		this->m_counts.clear();
	} //clear_members
	void IndivCluster::update_center(void) {
		DbValueMap &oRes = this->m_center;
		oRes.clear();
		ints_double_map &sommes = this->m_sommes;
		ints_size_t_map &counts = this->m_counts;
		std::for_each(counts.begin(), counts.end(), [&](const std::pair<IntType, size_t> &p) {
			const IntType key = p.first;
			const size_t n = p.second;
			auto jt = sommes.find(key);
			std::pair<IntType, double> p2 = *jt;
			const double x = (p2.second) / n;
			oRes[key] = x;
		});
		this->cache_dist.clear();
	} //update_center
	bool IndivCluster::inter_inertia(const IndivCluster &other, double &dRes) const {
		const DbValueMap &m1 = this->m_center;
		const DbValueMap &m2 = other.m_center;
		size_t nc = 0;
		dRes = 0;
		std::for_each(m1.begin(), m1.end(), [m2, &nc, &dRes](const std::pair<IntType, DbValue> &oPair) {
			const IntType key = oPair.first;
			const DbValue &v1 = oPair.second;
			if (!v1.empty()) {
				auto jt = m2.find(key);
				if (jt != m2.end()) {
					const DbValue &v2 = (*jt).second;
					if (!v2.empty()) {
						double t = v1.double_value() - v2.double_value();
						dRes += t * t;
						++nc;
					}// ok
				}// found
			}// v1
		});
		if (nc > 1) {
			dRes /= nc;
		}
		return (nc > 0);
	}// inter_inertia
	bool IndivCluster::intra_inertia(double &dRes) const {
		IIndivProvider *pProvider = this->m_provider;
		if (pProvider == nullptr) {
			return (false);
		}
		dRes = 0;
		size_t nbinds = 0;
		typedef std::pair<IntType, DbValue> MyPair;
		const ints_deque &vv = this->m_individs;
		const DbValueMap &oCenter = this->m_center;
		std::for_each(vv.begin(), vv.end(), [&](const IntType &aIndex) {
			Indiv oInd;
			if (pProvider->find_indiv(aIndex, oInd)) {
				const DbValueMap &oMap = oInd.data();
				double somme = 0;
				size_t nc = 0;
				std::for_each(oMap.begin(), oMap.end(), [&](const MyPair &p) {
					const DbValue &v = p.second;
					if (!v.empty()) {
						double x = v.double_value();
						const IntType key = p.first;
						auto it = oCenter.find(key);
						if (it != oCenter.end()) {
							const DbValue &v2 = (*it).second;
							if (!v2.empty()) {
								double t = v.double_value() - v2.double_value();
								somme += t * t;
								++nc;
							}
						}//it

					}// not empty
					if (nc > 0) {
						if (nc > 1) {
							somme /= nc;
						}
						dRes += somme;
					}
				});
				++nbinds;
			}// ind
		});
		if (nbinds > 1) {
			dRes /= nbinds;
		}
		return (nbinds > 0);
	}// intra_inertia
	///////////////////////////////////////
} // namespace info
