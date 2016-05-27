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
////////////////////////////////////
namespace info {
	///////////////////////////////
	template<typename U, typename STRINGTYPE>
	class ClustersCollection : public InterruptObject {
	public:
		using IndexType = U;
		using IndivType = Indiv<U, STRINGTYPE>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using indivptrs_vector = std::vector<IndivTypePtr>;
		using DataMap = std::map<U, InfoValue>;
		using ints_sizet_map = std::map<U, size_t>;
		using IndivClusterType = IndivCluster<U, STRINGTYPE>;
		using SourceType = IIndivSource<U>;
		using clusters_vector = std::vector<IndivClusterType>;
		using ints_vector = std::vector<U>;
		using sizet_intsvector_map = std::map<size_t, ints_vector>;
		using datamaps_vector = std::vector<DataMap>;
		using ClustersCollectionType = ClustersCollection<U, STRINGTYPE>;
	private:
		SourceType *m_provider;
		size_t m_nbclusters;
		size_t m_nbindivs;
		size_t m_nbmaxiters;
		double m_finter;
		double m_fintra;
		double m_ff;
		clusters_vector m_clusters;
		DataMap m_center;
		ints_sizet_map m_indivsmap;
		sizet_intsvector_map m_classesmap;
	public:
		std::ostream & write_to(std::ostream &os) const {
			os << this->m_finter << ", " << this->m_fintra << ", " << this->m_ff
				<< std::endl;
			std::for_each(this->m_clusters.begin(), this->m_clusters.end(),
				[&](const IndivClusterType &c) {
				os << c << std::endl;
			});
			std::string s;
			info_global_write_map(this->m_center, s);
			os << s << std::endl;
			return (os);
		} // write_to
		std::wostream & write_to(std::wostream &os) const {
			os << this->m_finter << L", " << this->m_fintra << L", " << this->m_ff
				<< std::endl;
			std::for_each(this->m_clusters.begin(), this->m_clusters.end(),
				[&](const IndivClusterType &c) {
				os << c << std::endl;
			});
			std::wstring s;
			info_global_write_map(this->m_center, s);
			os << s << std::endl;
			return (os);
		} // write_to
		void to_string(std::string &s)const {
			std::stringstream os;
			this->write_to(os);
			s = os.str();
		}
		void to_string(std::wstring &s)const {
			std::wstringstream os;
			this->write_to(os);
			s = os.str();
		}
	public:
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
				if (this->check_interrupt()) {
					return (false);
				}
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
			return (this->check_interrupt()) ? false : true;
		}
		const sizet_intsvector_map & classes_map(void) const {
			return (this->m_classesmap);
		}
		const ints_sizet_map & indivs_map(void) const {
			return (this->m_indivsmap);
		}
		std::tuple<double, double, double> criterias(void) const {
			return std::make_tuple(this->m_fintra, this->m_finter, this->m_ff);
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
		virtual bool process(SourceType *pSource, const size_t nbClusters = 5,
			const size_t nbMaxIters = 100,
			std::atomic_bool *pCancel = nullptr) {
			if (!this->initialize_process(pSource, nbClusters, nbMaxIters,
				pCancel)) {
				return (false);
			}
			if (this->check_interrupt()) {
				return (false);
			}
			do {
				if (this->check_interrupt()) {
					return (false);
				}
				if (!this->one_iteration()) {
					break;
				}
			} while (true);
			if (this->check_interrupt()) {
				return (false);
			}
			return (this->post_terminate_process());
		} // process
		DataMap & center(void) {
			return (this->m_center);
		}
		virtual ~ClustersCollection() {
		}
	protected:
		ClustersCollection(std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_provider(nullptr), m_nbclusters(0), m_nbindivs(
				0), m_nbmaxiters(100), m_finter(0), m_fintra(0), m_ff(0) {
		}
		ClustersCollection(const ClustersCollectionType &other) :InterruptObject(other),
			m_ptovider(other.m_provider),
			m_nbclusters(other.m_nbclusters), m_nbindivs(other.m_nbindivs), m_nbmaxiters(other.m_nbmaxiters),
			m_finter(other.m_finter), m_fintra(other.m_fintra), m_ff(other.m_ff),
			m_clusters(other.m_clusters), m_center(other.m_center),
			m_indivsmap(other.m_indivsmap), m_classesmap(other.m_classesmap) {}
		SourceType *source(void) const {
			return (this->m_provider);
		}
		ClustersCollectionType & operator=(const ClustersCollectionType &other) {
			if (this != &other) {
				InterruptObject::operator=(other);
				this->m_provider = other.m_provider;
				this->m_nbclusters = other.m_nbclusters;
				this->m_nbindivs = other.m_nbindivs;
				this->m_nbmaxiters = other.m_nbmaxiters;
				this->m_finter = other.m_finter;
				this->m_fintra = other.m_fintra;
				this->m_ff = other.m_ff;
				this->m_clusters = other.m_clusters;
				this->m_center = other.m_center;
				this->m_indivsmap = other.m_indivsmap;
				this->m_classesmap = other.m_classesmap;
			}
			return (*this);
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
				if (this->check_interrupt()) {
					return;
				}
				const IndivClusterType &oInd = *kt;
				size_t val = (size_t)oInd.id();
				oInd.get_indivs_map(oMap, val);
			} // kt
		} //get_indivs_map
		void get_clusters_ids(sizet_intsvector_map &oMap) const {
			if (this->check_interrupt()) {
				return;
			}
			oMap.clear();
			const clusters_vector &v = this->clusters();
			const size_t n = v.size();
			for (size_t i = 0; i < n; ++i) {
				if (this->check_interrupt()) {
					return;
				}
				const IndivClusterType & p = v[i];
				ints_vector oo;
				p.get_ids(oo);
				size_t key = (size_t)p.id();
				oMap[key] = oo;
			}	  // i
		}	  // get_clusters_ids
		bool get_criterias(double &fIntra, double &fInter, double &ff) const {
			if (this->check_interrupt()) {
				return (false);
			}
			const clusters_vector &v = this->clusters();
			fIntra = 0;
			fInter = 0;
			ff = 0;
			const size_t n = v.size();
			if (n < 1) {
				return (false);
			}
			std::atomic_bool *pCancel = this->get_cancelleable_flag();
			const DataMap &oCenter = this->center();
			for (size_t i = 0; i < n; ++i) {
				if (this->check_interrupt()) {
					return (false);
				}
				const IndivClusterType &c = v[i];
				double d = 0;
				if (info_global_compute_distance(oCenter, c.center(), d, pCancel)) {
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
			this->m_indivsmap.clear();
			this->m_classesmap.clear();
		} // clear
		virtual void update_center(void) {
			if (this->check_interrupt()) {
				return;
			}
			IndivSummator<U> summator(this->get_cancelleable_flag());
			const clusters_vector &v = this->m_clusters;
			for (auto & oInd : v) {
				if (this->check_interrupt()) {
					return;
				}
				const DataMap &oMap = oInd.center();
				summator.add(oMap);
			}
			summator.get_result(this->m_center);
		} // update_center
		virtual bool initialize_process(SourceType *pSource,
			const size_t nbClusters, const size_t nbMaxIters = 100,
			std::atomic_bool *pCancel = nullptr) {
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
			if (this->get_cancelleable_flag() == nullptr) {
				this->set_cancelleable_flag(pCancel);
			}
			this->clear();
			return (this->check_interrupt()) ? false : true;
		} // initalize_process
		virtual bool one_iteration(void) {
			return (this->check_interrupt()) ? false : true;
		} // one_iteration
		virtual bool post_terminate_process(void) {
			this->get_indivs_map(this->m_indivsmap);
			if (this->check_interrupt()) {
				return (false);
			}
			this->get_clusters_ids(this->m_classesmap);
			if (this->check_interrupt()) {
				return (false);
			}
			this->get_criterias(this->m_fintra, this->m_finter, this->m_ff);
			return (this->check_interrupt()) ? false : true;
		} // post_terminate_process
	};
	// class ClustersCoillection<U>
	///////////////////////////////
}// namespace info
/////////////////////////////////////
template<typename U, typename STRINGTYPE>
inline std::ostream & operator<<(std::ostream &os,
	const info::ClustersCollection<U, STRINGTYPE> &d) {
	return d.write_to(os);
}
template<typename U, typename STRINGTYPE>
inline std::wostream & operator<<(std::wostream &os,
	const info::ClustersCollection<U, STRINGTYPE> &d) {
	return d.write_to(os);
}
/////////////////////////
#endif /* CLUSTERSCOLLECTION_H_ */
