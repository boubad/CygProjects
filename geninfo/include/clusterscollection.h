#pragma once
/*
 * clusterscollection.h
 *
 *  Created on: 21 mai 2016
 *      Author: boubad
 */
#ifndef CLUSTERSCOLLECTION_H_
#define CLUSTERSCOLLECTION_H_
////////////////////////////////////
#include "indivcluster.h"
#include "inforunner.h"
#include "matresult.h"
////////////////////////////////////
namespace info {
/////////////////////////////////////
template<typename U>
class ClusterizeResult {
public:
	using ints_vector = std::vector<U>;
	using sizet_intsvector_map = std::map<size_t, ints_vector>;
	using ints_sizet_map = std::map<U, size_t>;
	using ClusterizeResultType = ClusterizeResult<U>;
	using ClusterizeResultPtr = std::shared_ptr< ClusterizeResultType>;
private:
	StageType m_stage;
	double m_finter;
	double m_fintra;
	double m_ff;
	ints_sizet_map m_indivsmap;
	sizet_intsvector_map m_classesmap;
public:
	ClusterizeResult() :
			m_stage(StageType::current), m_finter(0), m_fintra(0), m_ff(0) {
	}
	ClusterizeResult(const ClusterizeResultType &other) :
			m_stage(other.m_stage), m_finter(other.m_finter), m_fintra(
					other.m_fintra), m_ff(other.m_ff), m_indivsmap(
					other.m_indivsmap), m_classesmap(other.m_classesmap) {
	}
	ClusterizeResultType & operator=(const ClusterizeResultType &other) {
		if (this != &other) {
			this->m_stage = other.m_stage;
			this->m_finter = other.m_finter;
			this->m_fintra = other.m_fintra;
			this->m_ff = other.m_ff;
			this->m_indivsmap = other.m_indivsmap;
			this->m_classesmap = other.m_classesmap;
		}
		return (*this);
	}
	virtual ~ClusterizeResult() {
	}
public:
	void clear(void) {
		this->m_finter = 0;
		this->m_fintra = 0;
		this->m_ff = 0;
		this->m_classesmap.clear();
		this->m_indivsmap.clear();
	}
	StageType stage(void) const {
		return (this->m_stage);
	}
	void stage(StageType s) {
		this->m_stage = s;
	}
	const sizet_intsvector_map & classes_map(void) const {
		return (this->m_classesmap);
	}
	sizet_intsvector_map & classes_map(void) {
		return (this->m_classesmap);
	}
	const ints_sizet_map &indivs_map(void) const {
		return (this->m_indivsmap);
	}
	ints_sizet_map &indivs_map(void) {
		return (this->m_indivsmap);
	}
	double criteria(void) const {
		return (this->m_ff);
	}
	void criteria(double d) {
		this->m_ff = d;
	}
	double intra_inertia(void) const {
		return (this->m_fintra);
	}
	void intra_inertia(double d) {
		this->m_fintra = d;
	}
	double inter_inertia(void) const {
		return (this->m_finter);
	}
	void inter_inertia(double d) {
		this->m_finter = d;
	}
	void to_string(std::string &ss) const {
		std::stringstream os;
		if (this->m_stage == StageType::started) {
			os << "STARTED... ";
		} else if (this->m_stage == StageType::finished) {
			os << "FINISHED!! ";
		} else if (this->m_stage == StageType::aborted) {
			os << "ABORTED. ";
		}
		os << this->m_finter << ", " << this->m_fintra << ", " << this->m_ff
				<< std::endl;
		for (auto &p : this->m_classesmap) {
			size_t key = p.first;
			os << key << "\t";
			std::string sx;
			info_write_vector(p.second, sx);
			os << sx << std::endl;
		} // p
		ss = os.str();
	} //to_string
	void to_string(std::wstring &ss) const {
		std::wstringstream os;
		if (this->m_stage == StageType::started) {
			os << L"STARTED... ";
		} else if (this->m_stage == StageType::finished) {
			os << L"FINISHED!! ";
		} else if (this->m_stage == StageType::aborted) {
			os << L"ABORTED. ";
		}
		os << this->m_finter << L", " << this->m_fintra << L", " << this->m_ff
				<< std::endl;
		for (auto &p : this->m_classesmap) {
			size_t key = p.first;
			os << key << L"\t";
			std::wstring sx;
			info_write_vector(p.second, sx);
			os << sx << std::endl;
		} // p
		ss = os.str();
	} //to_string
};
// class ClusterizeResult<U>
///////////////////////////////
template<typename U, typename STRINGTYPE>
class ClustersCollection: public CancellableObject<
		std::shared_ptr<ClusterizeResult<U>>>{
public:
using BaseType = CancellableObject<std::shared_ptr<ClusterizeResult<U>>>;
using ClusterizeResultType = ClusterizeResult<U>;
using ClusterizeResultPtr = std::shared_ptr< ClusterizeResultType>;
using IndexType = U;
using IndivType = Indiv<U, STRINGTYPE>;
using IndivTypePtr = std::shared_ptr<IndivType>;
using indivptrs_vector = std::vector<IndivTypePtr>;
using DataMap = std::map<U, InfoValue>;
using ints_sizet_map = std::map<U, size_t>;
using IndivClusterType = IndivCluster<U, STRINGTYPE>;
using SourceType = IIndivSource<U,STRINGTYPE>;
using clusters_vector = std::vector<IndivClusterType>;
using ints_vector = std::vector<U>;
using sizet_intsvector_map = std::map<size_t, ints_vector>;
using datamaps_vector = std::vector<DataMap>;
using ClustersCollectionType = ClustersCollection<U, STRINGTYPE>;
using cancelflag = std::atomic<bool>;
using pcancelflag = cancelflag *;
using PBackgrounder = Backgrounder *;
using RESULT = std::shared_ptr< ClusterizeResultType>;
using function_type = std::function<void(RESULT)>;
private:
SourceType *m_provider;
size_t m_nbclusters;
size_t m_nbindivs;
size_t m_nbmaxiters;
DataMap m_center;
clusters_vector m_clusters;
ClusterizeResultType m_res;
private:
ClustersCollection(const ClustersCollectionType &) = delete;
ClustersCollectionType & operator=(const ClustersCollectionType &) = delete;
public:
virtual ~ClustersCollection() {
}

bool get_random_indivs(const size_t nc, indivptrs_vector &oRes) {
	assert(nc > 0);
	SourceType *pProvider = this->m_provider;
	assert(pProvider != nullptr);
	const size_t nbIndivs = this->m_nbindivs;
	assert(nc <= nbIndivs);
	oRes.clear();
	std::vector<size_t> temp(nbIndivs);
	for (size_t i = 0; i < nbIndivs; ++i) {
		temp[i] = i;
	} // i
	std::srand(unsigned(std::time(0)));
	std::random_shuffle(temp.begin(), temp.end());
	datamaps_vector oSeeds;
	size_t icur = 0;
	while ((oRes.size() < nc) && (icur < nbIndivs)) {
		size_t pos = temp[icur++];
		IndivTypePtr oInd = pProvider->get(pos);
		const IndivType *pInd = oInd.get();
		if ((pInd != nullptr) && pInd->has_numeric_fields()) {
			oRes.push_back(oInd);
		} // pInd
	} // while
	if (oRes.size() < nc) {
		return (false);
	}
	return (true);
}
const ClusterizeResultType &get_result(void) const {
	return (this->m_res);
}
size_t clusters_count(void) const {
	return (this->clusters().size());
}
const clusters_vector & clusters(void) const {
	return (this->m_clusters);
}
const DataMap & center(void) const {
	return (this->m_center);
}
DataMap & center(void) {
	return (this->m_center);
}
virtual void notify(StageType s = StageType::current) {
	this->update_result();
	ClusterizeResultPtr oRes = std::make_shared<ClusterizeResultType>(this->m_res);
	ClusterizeResultType *p = oRes.get();
	assert(p != nullptr);
	p->stage(s);
	this->send(oRes);
} // notify
virtual bool process(SourceType *pSource, const size_t nbClusters = 5,
		const size_t nbMaxIters = 100) {
	if (!this->initialize_process(pSource, nbClusters, nbMaxIters)) {
		return (false);
	}
	if (this->is_cancelled()) {
		return (false);
	}
	this->notify(StageType::started);
	do {
		if (!this->one_iteration()) {
			break;
		}
		if (this->is_cancelled()) {
			return (false);
		}
		this->notify(StageType::current);
	}while (true);
	if (this->is_cancelled()) {
		return (false);
	}
	this->notify(StageType::finished);
	return (!this->is_cancelled());
} // process
protected:
ClustersCollection(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
		function_type f = [](ClusterizeResultPtr arg) {}) :
BaseType(pFlag, pq, f), m_provider(nullptr), m_nbclusters(0), m_nbindivs(
		0), m_nbmaxiters(100) {
}
SourceType *source(void) const {
	return (this->m_provider);
}
size_t get_nbIndivs(void) const {
	return (this->m_nbindivs);
}
size_t get_nbClusters(void) const {
	return (this->m_nbclusters);
}
size_t get_nbMaxIters(void) const {
	return (this->m_nbmaxiters);
}
clusters_vector & clusters(void) {
	return (this->m_clusters);
}
void set_clusters(const clusters_vector &v) {
	this->m_clusters = v;
}
void get_indivs_map(ints_sizet_map &oMap) const {
	oMap.clear();
	const clusters_vector &v = this->clusters();
	for (auto kt = v.begin(); kt != v.end(); ++kt) {
		const IndivClusterType &oInd = *kt;
		size_t val = (size_t) oInd.id();
		oInd.get_indivs_map(oMap, val);
	} // kt
} //get_indivs_map
void get_clusters_ids(sizet_intsvector_map &oMap) const {
	oMap.clear();
	const clusters_vector &v = this->clusters();
	const size_t n = v.size();
	for (size_t i = 0; i < n; ++i) {
		const IndivClusterType & p = v[i];
		ints_vector oo;
		p.get_ids(oo);
		size_t key = (size_t) p.id();
		oMap[key] = oo;
	}	  // i
}	  // get_clusters_ids
bool get_criterias(double &fIntra, double &fInter, double &ff) const {
	const clusters_vector &v = this->clusters();
	fIntra = 0;
	fInter = 0;
	ff = 0;
	const size_t n = v.size();
	if (n < 1) {
		return (false);
	}
	const DataMap &oCenter = this->center();
	for (size_t i = 0; i < n; ++i) {
		const IndivClusterType &c = v[i];
		double d = 0;
		if (info_global_compute_distance(oCenter, c.center(), d)) {
			fInter += d;
		}
		d = 0;
		if (c.inertia(d)) {
			fIntra += d;
		}
	} // i
	fInter /= n;
	fIntra /= n;
	if (fInter > 0) {
		ff = fIntra / fInter;
		return (true);
	}
	return (false);
}	  // get_criterias
virtual void clear(void) {
	this->m_clusters.clear();
	this->m_center.clear();
	this->m_res.clear();
} // clear
virtual void update_center(void) {
	IndivSummator<U> summator;
	const clusters_vector &v = this->m_clusters;
	for (auto & oInd : v) {
		const DataMap &oMap = oInd.center();
		summator.add(oMap);
	}
	summator.get_result(this->m_center);
} // update_center
virtual bool initialize_process(SourceType *pSource,
		const size_t nbClusters, const size_t nbMaxIters = 100) {
	assert(pSource != nullptr);
	assert(nbClusters > 0);
	assert(nbMaxIters > 0);
	this->m_provider = pSource;
	this->m_nbclusters = nbClusters;
	this->m_nbmaxiters = nbMaxIters;
	this->m_nbindivs = pSource->count();
	if (this->m_nbindivs < this->m_nbclusters) {
		return (false);
	}
	this->clear();
	return (this->is_cancelled()) ? false : true;
} // initalize_process
virtual bool one_iteration(void) {
	return (this->is_cancelled()) ? false : true;
} // one_iteration
void update_result(void) {
	this->update_center();
	this->get_indivs_map(this->m_res.indivs_map());
	this->get_clusters_ids(this->m_res.classes_map());
	double fintra, finter, ff;
	this->get_criterias(fintra, finter, ff);
	this->m_res.criteria(ff);
	this->m_res.intra_inertia(fintra);
	this->m_res.inter_inertia(finter);
}//update_result
virtual bool post_terminate_process(void) {
	this->update_result();
	return (this->is_cancelled()) ? false : true;
} // post_terminate_process
};
// class ClustersCoillection<U>
///////////////////////////////
}
 // namespace info
/////////////////////////
#endif /* CLUSTERSCOLLECTION_H_ */
