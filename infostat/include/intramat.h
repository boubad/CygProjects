#pragma once
#ifndef INTRAMAT_H_
#define INTRAMAT_H_
/////////////////////////////
#include "matresult.h"
#include "indiv.h"
#include "distancemap.h"
#include "sharedqueue.h"
/////////////////////////////////
#include <boost/signals2/signal.hpp>
//////////////////////////////////
namespace info {
	///////////////////////////////////////////
	template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
		class IntraMatElem {
		public:
			using sizets_pair = std::pair<size_t, size_t>;
			using pairs_list = std::list<sizets_pair>;
			using ints_vector = std::vector<IDTYPE>;
			using sizets_vector = std::vector<size_t>;
			using DistanceMapType = DistanceMap<IDTYPE, DISTANCETYPE>;
			using IndivType = Indiv<IDTYPE, STRINGTYPE>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using SourceType = IIndivSource<IDTYPE, STRINGTYPE>;
			using IntraMatElemType = IntraMatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
			using RescritType = std::atomic<DISTANCETYPE>;
			using IntraMatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE>;
			using IntraMatElemResultPtr = std::shared_ptr<IntraMatElemResultType>;
			using SignalType = typename boost::signals2::signal<void(IntraMatElemResultPtr)>;
			using SlotType = typename SignalType::slot_type;
			using ConnectionType = boost::signals2::connection;
		private:
			bool m_hasconnect;
			DISTANCETYPE m_crit;
			DistanceMapType *m_pdist;
			sizets_vector m_indexes;
			SignalType m_signal;
			std::unique_ptr<DistanceMapType> m_odist;
		public:
			IntraMatElem() : m_hasconnect(false), m_crit(0), m_pdist(nullptr) {
			}
			virtual ~IntraMatElem() {
			}
		public:
			IntraMatElemResultPtr getResult(void) const {
				const DistanceMapType *pDist = this->m_pdist;
				assert(pDist != nullptr);
				IntraMatElemResultPtr oRet(new IntraMatElemResultType());
				IntraMatElemResultType *p = oRet.get();
				p->first = this->m_crit;
				p->second = this->m_indexes;
				ints_vector &dest = p->third;
				const sizets_vector &src = this->m_indexes;
				const size_t n = src.size();
				dest.resize(n);
				for (size_t i = 0; i < n; ++i) {
					IDTYPE aIndex = 0;
					pDist->get_id_from_index(i, aIndex);
					dest[i] = aIndex;
				}// i
				return (oRet);
			}
			ConnectionType connect(const SlotType &subscriber) {
				this->m_hasconnect = true;
				return m_signal.connect(subscriber);
			}
			DISTANCETYPE criteria(void) const {
				return (this->m_crit);
			}
			void indexes(sizets_vector &oIndex) {
				oIndex = this->m_indexes;
			}
			void arrange(SourceType *pProvider) {
				this->initialize(pProvider);
				DISTANCETYPE oCrit(this->m_crit);
				do {
					if (!this->one_iteration(oCrit)) {
						break;
					}
					this->notify();
				} while (true);
			} // arrange
			void arrange(DistanceMapType *pDist) {
				this->initialize(pDist);
				DISTANCETYPE oCrit(this->m_crit);
				do {
					if (!this->one_iteration(oCrit)) {
						break;
					}
					this->notify();
				} while (true);
			} // arrange
			static IntraMatElemResultPtr perform_arrange(SourceType *pProvider) {
				IntraMatElemType oMat;
				oMat.arrange(pProvider);
				IntraMatElemResultPtr oRet = oMat.getResult();
				return (oRet);
			} //perform_arrange
		protected:
			void notify(void) {
				if (this->m_hasconnect) {
					IntraMatElemResultPtr res = this->getResult();
					this->m_signal(res);
				}
			}//notify
			bool one_iteration(DISTANCETYPE &oCrit) {
				pairs_list q;
				bool bRet = this->find_best_try(q, oCrit);
				if (!bRet) {
					return (false);
				}
				const size_t nx = q.size();
				if (nx < 1) {
					return (false);
				}
				size_t i1 = 0, i2 = 0;
				sizets_pair p = q.front();
				q.pop_front();
				i1 = p.first;
				i2 = p.second;
				if (i1 == i2) {
					return (false);
				}
				if (oCrit >= this->m_crit) {
					return (false);
				}
				this->permute_items(i1, i2);
				this->m_crit = oCrit;
				//
				if (!q.empty()) {
					sizets_vector oldIndexes(this->m_indexes);
					DISTANCETYPE oldCrit(oCrit);
					while (!q.empty()) {
						sizets_pair pp = q.front();
						size_t j1 = pp.first;
						size_t j2 = pp.second;
						q.pop_front();
						if (j1 != j2) {
							IntraMatElemType xMat;
							xMat.m_pdist = this->m_pdist;
							xMat.m_indexes = oldIndexes;
							xMat.permute_items(j1, j2);
							xMat.m_crit = xMat.criteria(xMat.m_indexes);
							xMat.one_iteration(oldCrit);
							if (xMat.m_crit < this->m_crit) {
								this->m_crit = xMat.m_crit;
								this->m_indexes = xMat.m_indexes;
							}
						}
					} // more paths
				} // not empty
				return (true);
			} //one_iteration
			bool find_best_try(pairs_list &qq, DISTANCETYPE &oCrit) {
				const sizets_vector &indexes = this->m_indexes;
				const size_t n = this->m_pdist->size();
				DISTANCETYPE oldCrit = oCrit;
				for (size_t i = 0; i < n; ++i) {
					for (size_t j = 0; j < i; ++j) {
						sizets_vector temp(indexes);
						const size_t tt = temp[i];
						temp[i] = temp[j];
						temp[j] = tt;
						DISTANCETYPE c = this->criteria(temp);
						if (c <= oldCrit) {
							auto it =
								std::find_if(qq.begin(), qq.end(),
									[i, j](const sizets_pair &p)->bool {
								if ((p.first == (size_t)i) && (p.second == (size_t)j)) {
									return (true);
								}
								else if ((p.first == (size_t)j) && (p.second == (size_t)i)) {
									return (true);
								}
								else {
									return (false);
								}
							});
							if (it == qq.end()) {
								if ((c == oldCrit) && (!qq.empty())) {
									sizets_pair oPair(std::make_pair(i, j));
									qq.push_back(oPair);
								}
								else if (c < oldCrit) {
									oldCrit = c;
									qq.clear();
									sizets_pair oPair(std::make_pair(i, j));
									qq.push_back(oPair);
								}
							} // may add
						} // candidate
					} // j
				} // i
				oCrit = oldCrit;
				return (!qq.empty());
			} //find_best_try_serial
		private:
			DISTANCETYPE distance(const size_t i1, const size_t i2) const {
				assert(this->m_pdist != nullptr);
				DISTANCETYPE d = 0;
				this->m_pdist->indexed_get(i1, i2, d);
				return (d);
			} // distance
			DISTANCETYPE criteria(const sizets_vector &indexes) {
				const size_t n = indexes.size();
				DISTANCETYPE dRet = 0;
				if (n < 2) {
					return (dRet);
				}
				const size_t nx = (size_t)(n - 1);
				for (size_t i = 0; i < nx; ++i) {
					const size_t i1 = indexes[i];
					const size_t i2 = indexes[i + 1];
					dRet = (DISTANCETYPE)(dRet + this->distance(i1, i2));
				} // i
				return (dRet);
			} // criteria
			void permute_items(const size_t i1, const size_t i2) {
				sizets_vector &vv = this->m_indexes;
				const size_t tt = vv[i1];
				vv[i1] = vv[i2];
				vv[i2] = tt;
			} // permute_items
			void initialize(DistanceMapType *pDist) {
				assert(pDist != nullptr);
				this->m_pdist = pDist;
				const size_t n = pDist->size();
				sizets_vector & indexes = this->m_indexes;
				indexes.resize(n);
				for (size_t i = 0; i < n; ++i) {
					indexes[i] = i;
				} // i
				this->m_crit = this->criteria(indexes);
			} // initialize
			void initialize(SourceType *pProvider) {
				assert(pProvider != nullptr);
				this->m_odist.reset(new DistanceMapType(pProvider));
				DistanceMapType *pDist = this->m_odist.get();
				assert(pDist != nullptr);
				initialize(pDist);
			} // initialize
	};
	////////////////////////////////////////////
	template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
		class IntraMatOrd {
		using mutex_type = std::mutex;
		public:
			using MatElemType = IntraMatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
			//
			using IntraMatOrdResultType = IntraMatOrdResult<IDTYPE, DISTANCETYPE>;
			using IntraMatOrdResultTypePtr = std::shared_ptr<IntraMatOrdResultType>;
			using DistanceMapType = typename MatElemType::DistanceMapType;
			using MatElemResultType = typename MatElemType::IntraMatElemResultType;
			using MatElemResultPtr = typename MatElemType::IntraMatElemResultPtr;
			using SignalType = typename boost::signals2::signal<void(IntraMatOrdResultTypePtr)>;
			using SlotType = typename SignalType::slot_type;
			using ConnectionType = typename MatElemType::ConnectionType;
			using SourceType = typename MatElemType::SourceType;
			using ints_vector = typename MatElemType::ints_vector;
			using sizets_vector = typename MatElemType::sizets_vector;
			using RescritType = typename MatElemType::RescritType;
			using IndivType = typename MatElemType::IndivType;
			using IndivTypePtr = typename MatElemType::IndivTypePtr;
			using MatOrdType = IntraMatOrd<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		private:
			std::atomic_bool m_done;
			std::atomic_bool m_running;
			bool m_hasconnect;
			std::unique_ptr<MatElemType> m_vars;
			std::unique_ptr<MatElemType> m_inds;
			ConnectionType m_connectVars;
			ConnectionType m_connectInds;
			SignalType m_signal;
			MatElemResultPtr m_varsResults;
			MatElemResultPtr m_indsResults;
			//
			mutex_type _mutex;
		protected:
			bool has_clients(void) const {
				return (this->m_hasconnect);
			}
			void notify_variable(MatElemResultPtr oCrit) {
				if (this->m_hasconnect) {
					MatElemResultType *p = oCrit.get();
					if (p != nullptr) {
						p->disposition = DispositionType::variable;
					}
					MatElemResultPtr o;
					{
						std::unique_lock<mutex_type> oLock(this->_mutex);
						this->m_varsResults = oCrit;
						o = this->m_indsResults;
					}
					IntraMatOrdResultTypePtr oRes(new IntraMatOrdResultType(o, oCrit));
					this->m_signal(oRes);
				}
			}// notify_variable
			bool prep_vars(SourceType *pProvider) {
				this->m_vars.reset(new MatElemType());
				MatElemType *pMat = this->m_vars.get();
				assert(pMat != nullptr);
				this->m_connectVars = pMat->connect([this](MatElemResultPtr oCrit) {
					this->notify_variable(oCrit);
				});
				pMat->arrange(pProvider);
				return (true);
			} // prep_vars
			void notify_indiv(MatElemResultPtr oCrit) {
				if (this->m_hasconnect) {
					MatElemResultType *p = oCrit.get();
					if (p != nullptr) {
						p->disposition = DispositionType::indiv;
					}
					MatElemResultPtr o;
					{
						std::unique_lock<mutex_type> oLock(this->_mutex);
						this->m_indsResults = oCrit;
						o = this->m_varsResults;
					}
					IntraMatOrdResultTypePtr oRes(new IntraMatOrdResultType(oCrit, o));
					this->m_signal(oRes);
				}
			}// notify_indiv
			bool prep_inds(SourceType *pProvider) {
				this->m_inds.reset(new MatElemType());
				MatElemType *pMat = this->m_inds.get();
				assert(pMat != nullptr);
				this->m_connectInds = pMat->connect([this](MatElemResultPtr oCrit) {
					this->notify_indiv(oCrit);
				});
				pMat->arrange(pProvider);
				return (true);
			} // prep_inds
		public:
			IntraMatOrd() :m_done(false),m_running(false),m_hasconnect(false) {
			} // MatOrd
			virtual ~IntraMatOrd() {
			}
			bool is_done(void) {
				return (this->m_done.load());
			}
			bool is_running(void) {
				return (this->m_running.load());
			}
			ConnectionType connect(const SlotType &subscriber) {
				this->m_hasconnect = true;
				return m_signal.connect(subscriber);
			}
			void get_variables_results(MatElemResultPtr &oRes) {
				std::unique_lock<mutex_type> oLock(this->_mutex);
				oRes = this->m_varsResults;
			}
			void get_indivs_results(MatElemResultPtr &oRes) {
				std::unique_lock<mutex_type> oLock(this->_mutex);
				oRes = this->m_indsResults;
			}
			IntraMatOrdResultTypePtr getResult(void)  {
				std::unique_lock<mutex_type> oLock(this->_mutex);
				IntraMatOrdResultTypePtr oRes(new IntraMatOrdResultType(this->m_indsResults, this->m_varsResults));
				return (oRes);
			}
			void arrange(SourceType *pIndsSource, SourceType *pVarsSource) {
				assert(pIndsSource != nullptr);
				assert(pVarsSource != nullptr);
				this->m_running.store(true);
				this->m_varsResults.reset();
				this->m_indsResults.reset();
				this->m_done.store(false);
				//MatOrdType *pMat = this;
				std::thread t1([pIndsSource, this]() {
					this->prep_inds(pIndsSource);
				});
				std::thread t2([pVarsSource, this]() {
					this->prep_vars(pVarsSource);
				});
				t2.join();
				t1.join();
				//this->prep_vars(pVarsSource);
				//this->prep_inds(pIndsSource);
				this->m_running.store(false);
				this->m_done.store(true);
			} // process
	};
	////////////////////////////////////
	/////////////////////////////////////////
}// namespace infO
 ///////////////////////////////////
#endif /* INTRAMAT_H_ */
