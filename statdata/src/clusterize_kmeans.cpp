#include "../include/clusterize_kmeans.h"
////////////////////////////////////////
namespace info {
	//////////////////////////////////
	/*
	IIndivProvider *m_provider;
		size_t m_nbclusters;
		size_t m_nbmaxiterations;
		size_t  m_nbruns;
		double m_criteria;
		InfoCritItems m_crititems;
		indivclusters_vector m_clusters;
		ints_size_t_map m_map;
	*/
	///////////////////////////////////////////
	void ClusterizeKMeansCollector::process(IIndivProvider *pProvider,
		const size_t nbClusters /*= 5*/, const size_t nbMaxIterations /*= 100 */, const size_t nbRuns /*= 100*/)
	{
		assert(pProvider != nullptr);
		assert(pProvider->is_valid());
		assert(nbClusters > 0);
		assert(nbMaxIterations > 0);
		assert(nbRuns > 0);
		//
		this->m_provider = pProvider;
		this->m_nbclusters = nbClusters;
		this->m_nbmaxiterations = nbMaxIterations;
		this->m_nbruns = nbRuns;
		this->m_criteria = 0;
		this->m_crititems.clear();
		this->m_clusters.clear();
		this->m_map.clear();
		//
		int nn = (int)nbRuns;
		InfoCritItems &oInfos = this->m_crititems;
//#pragma omp parallel for
		for (int iRun = 0; iRun < nn; ++iRun) {
			ClusterizeKMeans oCur(pProvider, nbClusters);
			(void)oCur.compute(nbMaxIterations);
			double crit = 0;
			if (oCur.criteria(crit)) {
				if (crit > 0) {
//#pragma omp critical
					{
						oCur.add_results(oInfos);
						if (this->m_map.empty()) {
							this->m_criteria = crit;
							this->m_clusters = oCur.clusters();
							this->m_map = oCur.get_map();
						}
						else if ((this->m_criteria > 0) && (crit < this->m_criteria)) {
							this->m_criteria = crit;
							this->m_clusters = oCur.clusters();
							this->m_map = oCur.get_map();
						}
					}// critical section
				}// can reduce
			}// crit
		}// iRun
	}//process
	ClusterizeKMeansCollector::ClusterizeKMeansCollector() :m_provider(nullptr), m_nbclusters(0), m_nbmaxiterations(0), m_nbruns(0), m_criteria(0)
	{

	}
	ClusterizeKMeansCollector::~ClusterizeKMeansCollector() {

	}
	double ClusterizeKMeansCollector::get_criteria(void) const {
		return (this->m_criteria);
	}
	size_t ClusterizeKMeansCollector::get_clusters_count(void) const {
		return (this->m_clusters.size());
	}
	size_t ClusterizeKMeansCollector::get_nbmaxiters(void) const {
		return (this->m_nbmaxiterations);
	}
	size_t ClusterizeKMeansCollector::get_nbruns(void) const {
		return (this->m_nbruns);
	}
	const InfoCritItems &ClusterizeKMeansCollector::get_crititems(void) const {
		return (this->m_crititems);
	}
	const indivclusters_vector &ClusterizeKMeansCollector::get_clusters(void) const {
		return (this->m_clusters);
	}
	const ints_size_t_map & ClusterizeKMeansCollector::get_map(void) const {
		return (this->m_map);
	}

	///////////////////////////////////////////
	ClusterizeKMeans::ClusterizeKMeans(IIndivProvider *pProvider /* = nullptr*/, size_t nbClusters /*= 5*/) :m_nbclusters(nbClusters), m_provider(pProvider) {

	}
	ClusterizeKMeans::ClusterizeKMeans(const ClusterizeKMeans &other) : m_nbclusters(other.m_nbclusters), m_provider(other.m_provider),
		m_clusters(other.m_clusters), m_map(other.m_map), m_center(other.m_center) {

	}
	ClusterizeKMeans & ClusterizeKMeans::operator=(const ClusterizeKMeans &other) {
		if (this != &other) {
			this->m_nbclusters = other.m_nbclusters;
			this->m_provider = other.m_provider;
			this->m_clusters = other.m_clusters;
			this->m_map = other.m_map;
			this->m_center = other.m_center;
		}
		return (*this);
	}
	ClusterizeKMeans::~ClusterizeKMeans() {

	}
	bool ClusterizeKMeans::is_valid(void) const {
		return (this->m_provider != nullptr) && (this->m_provider->is_valid()) && (this->m_nbclusters > 0);
	}
	size_t ClusterizeKMeans::clusters_count(void) const {
		return (this->m_clusters.size());
	}
	void ClusterizeKMeans::clusters_count(const size_t n) {
		assert(n > 0);
		this->m_nbclusters = n;
	}
	IIndivProvider *ClusterizeKMeans::provider(void) const {
		return (this->m_provider);
	}
	void ClusterizeKMeans::provider(IIndivProvider *pProvider) {
		this->m_provider = pProvider;
	}
	const indivclusters_vector & ClusterizeKMeans::clusters(void) const {
		return (this->m_clusters);
	}
	const ints_size_t_map ClusterizeKMeans::get_map(void) const {
		return (this->m_map);
	}
	const DbValueMap & ClusterizeKMeans::center(void) const {
		return (this->m_center);
	}// center
	void  ClusterizeKMeans::add_results(InfoCritItems &oInfos) const {
		const ints_size_t_map &curMap = this->m_map;
		ints_set oSet;
		for (auto it = curMap.begin(); it != curMap.end(); ++it) {
			const IntType aIndex = (*it).first;
			oSet.insert(aIndex);
		}// it
		for (auto it = oSet.begin(); it != oSet.end(); ++it) {
			const IntType aIndex1 = *it;
			auto kt1 = curMap.find(aIndex1);
			const size_t cluster1 = (*kt1).second;
			for (auto jt = oSet.begin(); jt != it; ++jt) {
				const IntType aIndex2 = *jt;
				auto kt2 = curMap.find(aIndex2);
				const size_t cluster2 = (*kt2).second;
				if (cluster1 == cluster2) {
					double v = 1;
					if (oInfos.get(aIndex2, aIndex1, v)) {
						v = v + 1.0;
					}
					oInfos.add(aIndex2, aIndex1, v);
				}// equal
			}// jt
		}// it
	}//add_results
	void ClusterizeKMeans::update_center(void) {
		DbValueMap &oRes = this->m_center;
		oRes.clear();
		IIndivProvider *pProvider = this->m_provider;
		if (pProvider == nullptr) {
			return;
		}
		typedef std::pair<IntType, DbValue> MyPair;
		typedef std::pair<IntType, size_t> MyPair2;
		std::map<IntType, size_t> counts;
		std::map<IntType, double> sommes;
		indivclusters_vector & vv = this->m_clusters;
		std::for_each(vv.begin(), vv.end(), [&](IndivCluster &oCluster) {
			const DbValueMap &oMap = oCluster.center();
			std::for_each(oMap.begin(), oMap.end(), [&](const MyPair &p) {
				const DbValue &v = p.second;
				if (!v.empty()) {
					double x = v.double_value();
					const IntType key = p.first;
					auto it = counts.find(key);
					if (it == counts.end()) {
						counts[key] = 1;
						sommes[key] = x;
					}
					else {
						size_t nx = counts[key];
						++nx;
						counts[key] = nx;
						double s = sommes[key];
						s += x;
						sommes[key] = s;
					}
				}// not empty
			});
		});
		std::for_each(counts.begin(), counts.end(), [&](MyPair2 p) {
			IntType key = p.first;
			size_t n = p.second;
			auto jt = sommes.find(key);
			std::pair<IntType, double> p2 = *jt;
			double x = (p2.second) / n;
			oRes[key] = x;
		});
	}// update_center
	bool ClusterizeKMeans::compute(const size_t nbMaxIterations /* = 100 */) {
		assert(nbMaxIterations > 0);
		if (!this->is_valid()) {
			return (false);
		}
		const size_t initialNbClusters = this->m_nbclusters;
		assert(initialNbClusters > 0);
		IIndivProvider *pProvider = this->m_provider;
		assert(pProvider != nullptr);
		indivclusters_vector &clusters = this->m_clusters;
		clusters.clear();
		ints_size_t_map &oMap = this->m_map;
		oMap.clear();
		size_t nbIndivs = 0;
		if (!pProvider->indivs_count(nbIndivs)) {
			return (false);
		}
		if (initialNbClusters > nbIndivs) {
			return (false);
		}
		{
			std::srand(unsigned(std::time(0)));
			std::vector<size_t> indexes(nbIndivs);
			for (size_t i = 0; i < nbIndivs; ++i) {
				indexes[i] = i;
			}
			std::random_shuffle(indexes.begin(), indexes.end());
			for (size_t i = 0; i < initialNbClusters; ++i) {
				Indiv oInd;
				size_t pos = indexes[i];
				if (!pProvider->find_indiv(pos, oInd)) {
					return (false);
				}
				IndivCluster c(pProvider, i);
				c.add(oInd);
				c.update_center();
				clusters.push_back(c);
			} // i
		}
		size_t iter = 0;
		while (iter < nbMaxIterations) {
			//
			std::for_each(clusters.begin(), clusters.end(), [&](IndivCluster &c) {
				c.clear_members();
			});
			for (size_t i = 0; i < nbIndivs; ++i) {
				CritItem oCritRes;
				Indiv oInd;
				if (pProvider->find_indiv_at(i, oInd)) {
					for (size_t j = 0; j < initialNbClusters; ++j) {
						IndivCluster &c = clusters[j];
						double d = c.distance(oInd);
						CritItem cur(j, j + 1, d);
						oCritRes += cur;
					} // j
					std::pair<size_t, size_t> oPair;
					oCritRes.get(oPair);
					size_t iRes = oPair.first;
					if ((iRes >= 0) && (iRes < initialNbClusters)) {
						(clusters[iRes]).add(oInd);
					}
				} // ind
			} // i
			ints_size_t_map curMap;
			std::for_each(clusters.begin(), clusters.end(), [&](IndivCluster &c) {
				c.update_center();
				c.get_map(curMap);
			});
			if (oMap.empty()) {
				oMap = curMap;
				continue;
			}
			//
			bool done = true;
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				const IntType key = (*it).first;
				const size_t val = (*it).second;
				if (curMap.find(key) == curMap.end()) {
					return (false);
				}
				if (curMap[key] != val) {
					done = false;
					break;
				}
			} // it
			++iter;
			oMap = curMap;
			if (done) {
				break;
			}
		} // iter
		bool xDone = false;
		while (!xDone) {
			xDone = true;
			for (auto it = clusters.begin(); it != clusters.end(); ++it) {
				IndivCluster &c = *it;
				if (c.is_empty()) {
					clusters.erase(it);
					xDone = true;
					break;
				}
			}// it
		}// xDone
		oMap.clear();
		std::for_each(clusters.begin(), clusters.end(), [&](IndivCluster &c) {
			c.update_center();
			c.get_map(oMap);
		});
		this->update_center();
		return (true);
	}// compute
	bool ClusterizeKMeans::intra_inertia(double &dRes) const {
		dRes = 0;
		size_t nc = 0;
		const indivclusters_vector &v = this->m_clusters;
		std::for_each(v.begin(), v.end(), [&](const IndivCluster &c) {
			double d = 0;
			if (c.intra_inertia(d)) {
				dRes += d;
				++nc;
			}
		});
		if (nc > 0) {
			dRes /= nc;
		}
		return (true);
	}//intra_inertia
	bool ClusterizeKMeans::inter_inertia(double &dRes) const {
		const DbValueMap &m1 = this->m_center;
		const indivclusters_vector &v = this->m_clusters;
		size_t nc = 0;
		dRes = 0;
		std::for_each(v.begin(), v.end(), [&](const IndivCluster &c) {
			const DbValueMap &m2 = c.center();
			size_t nx = 0;
			double somme = 0;
			std::for_each(m1.begin(), m1.end(), [&](const std::pair<IntType, DbValue> &oPair) {
				const IntType key = oPair.first;
				const DbValue &v1 = oPair.second;
				if (!v1.empty()) {
					auto jt = m2.find(key);
					if (jt != m2.end()) {
						const DbValue &v2 = (*jt).second;
						if (!v2.empty()) {
							double t = v1.double_value() - v2.double_value();
							somme += t * t;
							++nx;
						}// ok
					}// found
				}// v1
			});
			if (nx > 1) {
				somme /= nx;
			}
			dRes += somme;
			++nc;
		});
		if (nc > 1) {
			dRes /= nc;
		}
		return (true);
	}//inter_inertia
	bool ClusterizeKMeans::criteria(double &dRes) const {
		dRes = 0;
		double fNum = 0, fDen = 0;
		if (this->inter_inertia(fDen) && this->intra_inertia(fNum)) {
			if (fDen > 0) {
				dRes = fNum / fDen;
				return (true);
			}
		}
		return (false);
	}//criteria
	/////////////////////////////////////////////
}// namespace info