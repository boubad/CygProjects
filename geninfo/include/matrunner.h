#pragma once
#ifndef __MATRUNNER_H__
#define __MATRUNNER_H__
////////////////////////////////
#include "inforunner.h"
#include "clusterize.h"
#include "info_treeindivs.h"
#include "info_matord.h"
#include "anacompo_indivprovider.h"
////////////////////////////////////////
namespace info {
	////////////////////////////////////////
	template <typename IDTYPE, typename STRINGTYPE, typename DISTANCETYPE, typename INTTYPE, typename WEIGHTYPE>
	class MatRunner : public InfoRunner {
	public:
		using cancelflag = std::atomic<bool>;
		using pcancelflag = cancelflag *;
		using PBackgrounder = Backgrounder *;
		using strings_vector = std::vector<STRINGTYPE>;
		using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using SourceType = typename MatElemType::SourceType;
		using DistanceMapType = typename MatElemType::DistanceMapType;
		using IndivMapType = typename MatElemType::IndivMapType;
		using IndivMapTypePtr = std::shared_ptr<IndivMapType>;
		using IndivMapTuple = std::tuple<IndivMapTypePtr, IndivMapTypePtr>;
		using IndivMapPair = std::pair<IndivMapTypePtr, IndivMapTypePtr>;
		using IndivMapPairPtr = std::shared_ptr<IndivMapPair>;
		using indivmappair_promise = std::promise<IndivMapPairPtr>;
		using indivmappair_future = std::future<IndivMapPairPtr>;
		using indivmappair_promise_ptr = std::shared_ptr<indivmappair_promise>;
		using MatElemResultType = typename MatElemType::MatElemResultType;
		using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
		using matelem_promise = std::promise<MatElemResultPtr>;
		using matelem_promise_ptr = std::shared_ptr<matelem_promise>;
		using matelem_future = std::future<MatElemResultPtr>;
		using matelem_function = std::function<void(MatElemResultPtr)>;
		//
		using ints_vector = std::vector<IDTYPE>;
		using sizet_intsvector_map = std::map<size_t, ints_vector>;
		using ints_sizet_map = std::map<IDTYPE, size_t>;
		using ClusterizeResultType = ClusterizeResult<IDTYPE>;
		using ClusterizeResultPtr = std::shared_ptr< ClusterizeResultType>;
		using RESULT = std::shared_ptr< ClusterizeResultType>;
		using clusterize_function = std::function<void(RESULT)>;
		using clusterize_promise = std::promise<RESULT>;
		using clusterize_promise_ptr = std::shared_ptr<clusterize_promise>;
		using clusterize_future = std::future<RESULT>;
		//
		using StoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		using IndivsTreeType = IndivsTree<IDTYPE, STRINGTYPE, DISTANCETYPE>;
		using ClusterizeKMeansType = ClusterizeKMeans<IDTYPE, STRINGTYPE, DISTANCETYPE>;
		using AnaCompoIndivSourceCreatorType = AnaCompoIndivSourceCreator<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE, DISTANCETYPE>;
		//
		using InfoMatriceResultPair = std::pair<MatElemResultPtr, MatElemResultPtr>;
		using InfoMatriceResultPairPtr = std::shared_ptr<InfoMatriceResultPair>;
		using InfoMatriceType = InfoMatrice<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using matrice_promise = std::promise<InfoMatriceResultPairPtr>;
		using matrice_future = std::future<InfoMatriceResultPairPtr>;
		using matrice_promise_ptr = std::shared_ptr<matrice_promise>;
		//
	private:
	public:
		MatRunner() {}
		virtual ~MatRunner() {}
	public:
		//
		template <typename T>
		matrice_future arrange_matrice(matrice_promise_ptr oPromise,
			size_t nRows, size_t nCols, const std::vector<T> &oData,
			const strings_vector &indsNames, const strings_vector &colsNames,
			bool bComputeWeights = false,
			matelem_function f = [](MatElemResultPtr o) {},
			const STRINGTYPE &sSigle = STRINGTYPE(),bool bNotify = true) {
			this->send_dispatch([this, oPromise, nRows, nCols, oData, indsNames, colsNames, bComputeWeights, f, sSigle,bNotify]() {
				try {
					InfoMatriceResultPairPtr oRes = InfoMatriceType::perform_arrange(nRows, nCols, oData,
						indsNames, colsNames, bComputeWeights, this->get_cancelflag(), this->get_backgrounder(), f, sSigle,bNotify);
					matrice_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						matrice_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			matrice_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}//arrange_matrice
		//
		matrice_future arrange_matrice(matrice_promise_ptr oPromise,
			SourceType *pIndsSource, SourceType *pVarsSource,
			const STRINGTYPE &datasetSigle,
			matelem_function ff = [](MatElemResultPtr arg) {},bool bNotify = true) {
			this->send_dispatch([this, oPromise, pIndsSource, pVarsSource, datasetSigle, ff,bNotify]() {
				try {
					InfoMatriceResultPairPtr oRes = InfoMatriceType::perform_arrange(pIndsSource, pVarsSource, this->get_cancelflag(),
						this->get_backgrounder(), ff, datasetSigle,bNotify);
					matrice_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						matrice_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			matrice_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}//arrange_matrice
		//
		indivmappair_future compute_anacompo(indivmappair_promise_ptr oPromise,
			StoreType *pStore,
			const STRINGTYPE &datasetSigle, const double fLimit = 0.95) {
			this->send_dispatch([this, oPromise, pStore, datasetSigle, fLimit]() {
				try {
					IndivMapTuple t = AnaCompoIndivSourceCreatorType::create_indivmaps(pStore, datasetSigle, fLimit);
					IndivMapPairPtr oRes = std::make_shared<IndivMapPair>();
					IndivMapPair *pPair = oRes.get();
					assert(pPair != nullptr);
					pPair->first = std::get<0>(t);
					pPair->second = std::get<1>(t);
					indivmappair_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						indivmappair_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			indivmappair_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}//compute_anacompo
		//
		clusterize_future kmeans_aggreg(clusterize_promise_ptr oPromise,
			SourceType *pProvider, size_t nbClusters, size_t nbMaxIters = 100,
			clusterize_function ff = [](RESULT) {}) {
			this->send_dispatch([this, oPromise, ff, nbClusters, nbMaxIters, pProvider]() {
				try {
					ClusterizeKMeansType oMean(this->get_cancelflag(), this->get_backgrounder(), ff);
					oMean.process(pProvider, nbClusters, nbMaxIters);
					ClusterizeResultPtr oRes = oMean.getResult(StageType::finished);
					clusterize_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						clusterize_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			clusterize_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}// kmeans_aggreg
		 //
		//
		clusterize_future hierar_aggreg(clusterize_promise_ptr oPromise,
			SourceType *pProvider, size_t nbClusters, LinkMode mode = LinkMode::linkMean,
			clusterize_function ff = [](RESULT) {}) {
			this->send_dispatch([this, oPromise, ff, nbClusters, mode, pProvider]() {
				try {
					IndivsTreeType oTree(this->get_cancelflag(), this->get_backgrounder(), ff, mode);
					oTree.process(pProvider, nbClusters);
					ClusterizeResultPtr oRes = oTree.getResult(StageType::finished);
					clusterize_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						clusterize_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			clusterize_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}// hierar_aggreg
		//
		matelem_future arrange_elem(matelem_promise_ptr oPromise, IndivMapType *pMap,
			matelem_function ff = [](MatElemResultPtr arg) {}) {
			this->send_dispatch([this, oPromise, ff, pMap]() {
				try {
					MatElemType oMat(DispositionType::indiv, this->get_cancelflag(), this->get_backgrounder(), ff);
					oMat.arrange(pMap);
					MatElemResultPtr oRes = oMat.getResult(StageType::finished);
					matelem_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						matelem_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			matelem_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}// arrange_elem
		matelem_future arrange_elem(matelem_promise_ptr oPromise, DistanceMapType *pDist,
			matelem_function ff = [](MatElemResultPtr arg) {}) {
			this->send_dispatch([this, oPromise, ff, pDist]() {
				try {
					MatElemType oMat(DispositionType::indiv, this->get_cancelflag(), this->get_backgrounder(), ff);
					oMat.arrange(pDist);
					MatElemResultPtr oRes = oMat.getResult(StageType::finished);
					matelem_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						matelem_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			matelem_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}// arrange_elem
		matelem_future arrange_elem(matelem_promise_ptr oPromise, SourceType *pProvider,
			matelem_function ff = [](MatElemResultPtr arg) {}) {
			this->send_dispatch([this, oPromise, ff, pProvider]() {
				try {
					MatElemType oMat(DispositionType::indiv, this->get_cancelflag(), this->get_backgrounder(), ff);
					oMat.arrange(pProvider);
					MatElemResultPtr oRes = oMat.getResult(StageType::finished);
					matelem_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						matelem_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			matelem_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}// arrange_elem
		template<typename T, typename F>
		matelem_future arrange_elem(matelem_promise_ptr oPromise,
			size_t nRows, size_t nCols, const std::vector<T> &oData,
			const strings_vector &names, const std::vector<F> &weights,
			bool bComputeWeights = false, matelem_function ff = [](MatElemResultPtr arg) {}) {
			this->send_dispatch([this, oPromise, nRows, nCols, oData, names, weights, bComputeWeights, ff]() {
				try {
					MatElemType oMat(DispositionType::indiv, this->get_cancelflag(), this->get_backgrounder(), ff);
					oMat.arrange(nRows, nCols, oData, names, weights, bComputeWeights);
					MatElemResultPtr oRes = oMat.getResult(StageType::finished);
					matelem_promise *pPromise = oPromise.get();
					pPromise->set_value(oRes);
				}
				catch (...) {
					try {
						matelem_promise *pPromise = oPromise.get();
						pPromise->set_exception(std::current_exception());
					}
					catch (...) {}
				}
			});
			matelem_promise *pPromise = oPromise.get();
			return (pPromise->get_future());
		}// arrange_elem
	};// class MatRunner<IDTYPE,STRINGTYPE,DISTANCETYPE>
	///////////////////////////////////////////
}// info
///////////////////////////////////
#endif // !__MATRUNNER_H__
