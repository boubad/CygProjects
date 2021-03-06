#pragma once
#ifndef __CLUSTERIZE_KMEANS_H__
#define __CLUSTERIZE_KMEANS_H__
///////////////////////////////////////
#include "indivcluster.h"
//////////////////////////////////////////
namespace info {
	///////////////////////////////////////
	class ClusterizeKMeans {
	public:
		typedef std::vector<DbValueMap> valuemaps_vector;
	private:
		size_t m_nbclusters;
		IIndivProvider *m_provider;
		indivclusters_vector m_clusters;
		ints_size_t_map m_map;
		DbValueMap m_center;
	public:
		ClusterizeKMeans(IIndivProvider *pProvider = nullptr, size_t nbClusters = 5);
		ClusterizeKMeans(const ClusterizeKMeans &other);
		ClusterizeKMeans & operator=(const ClusterizeKMeans &other);
		~ClusterizeKMeans();
	public:
		void clear(void);
		bool is_empty(void) const;
		bool is_valid(void) const;
		size_t clusters_count(void) const;
		void clusters_count(const size_t n);
		IIndivProvider *provider(void) const;
		void provider(IIndivProvider *pProvider);
		const indivclusters_vector & clusters(void) const;
		const ints_size_t_map get_map(void) const;
		const DbValueMap & center(void) const;
		bool compute(valuemaps_vector &oSeeds, const size_t nbMaxIterations = 100);
		bool compute(const size_t nbMaxIterations = 100);
		bool intra_inertia(double &dRes) const;
		bool inter_inertia(double &dRes) const;
		bool criteria(double &dRes) const;
		void add_results(InfoCritItems &oInfos) const;
	protected:
		void update_center(void);
	};// class ClusterizeKMeans
	//////////////////////////////////////////////
	class ClusterizeKMeansCollector : public boost::noncopyable {
	private:
		IIndivProvider *m_provider;
		size_t m_nbclusters;
		size_t m_nbmaxiterations;
		size_t  m_nbruns;
		double m_criteria;
		InfoCritItems m_crititems;
		ClusterizeKMeans m_object;
	public:
		ClusterizeKMeansCollector();
		~ClusterizeKMeansCollector();
	public:
		double get_criteria(void) const;
		size_t get_clusters_count(void) const;
		size_t get_nbmaxiters(void) const;
		size_t get_nbruns(void) const;
		const InfoCritItems &get_crititems(void) const;
		const indivclusters_vector &get_clusters(void) const;
		const ints_size_t_map  get_map(void) const;
		bool is_empty(void) const {
			return (this->m_object.is_empty());
		}
		const ClusterizeKMeans &object(void) const {
			return (this->m_object);
		}
		bool intra_inertia(double &dRes) const {
			return (this->m_object.intra_inertia(dRes));
		}
		bool inter_inertia(double &dRes) const {
			return (this->m_object.inter_inertia(dRes));
		}
		bool criteria(double &dRes) const {
			return (this->m_object.criteria(dRes));
		}
	public:
		void process(IIndivProvider *pProvider, const size_t nbClusters = 5, const size_t nbMaxIterations = 100, const size_t nbRuns = 10);
	};// class ClusterizeKMeansCollector
	////////////////////////////////////////////
}// namespace info
///////////////////////////////////////////
#endif // !__CLUSTERIZE_KMEANS_H__
