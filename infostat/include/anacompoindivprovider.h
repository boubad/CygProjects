#pragma once
#ifndef __ANACOMPOINDIVPROVIDER_H__
#define __ANACOMPOINDIVPROVIDER_H__
/////////////////////////////////////
#include "transformed_storeindivsource.h"
#include "anacompo.h"
#include "datavector_indivprovider.h"
#include "indivmap.h"
//////////////////////////////////////////
namespace info {
	////////////////////////////////////
	template<typename U, typename INTTYPE,
		typename STRINGTYPE, typename WEIGHTYPE, typename DISTANCETYPE>
		class AnaCompoIndivSourceCreator {
		public:
			using DataVectorIndivSourceType = DataVectorIndivSource<U, STRINGTYPE>;
			using DataVectorIndivSourcePtr = std::shared_ptr<DataVectorIndivSourceType>;
			using DataVectorIndivSourceTuple = std::tuple<DataVectorIndivSourcePtr, DataVectorIndivSourcePtr>;
			//
			using ints_vector = std::vector<U>;
			using strings_vector = std::vector<STRINGTYPE>;
			using IndivType = Indiv<U, STRINGTYPE>;
			using DataMap = std::map<U, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using indivptrs_vector = std::vector<IndivTypePtr>;
			using StatSummatorType = StatSummator<U, STRINGTYPE>;
			using StatInfoType = StatInfo<U, STRINGTYPE>;
			using TranformedStoreIndivSourceType = TranformedStoreIndivSource<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using SourceType = IIndivSource<U, STRINGTYPE>;
			using StoreType = IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = StatDataset<U, INTTYPE, STRINGTYPE>;
			using VariableType = StatVariable<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using variables_vector = std::vector<VariableType>;
			using ints_doubles_map = std::map<U, double>;
			using IndivMapType = IndivMap<U, STRINGTYPE, DISTANCETYPE>;
			using IndivMapTypePtr = std::shared_ptr<IndivMapType>;
			using IndivMapTuple = std::tuple<IndivMapTypePtr, IndivMapTypePtr>;
		public:
			static IndivMapTuple create_indivmaps(StoreType *pStore,
				const STRINGTYPE &datasetSigle, const double fLimit = 0.95) {
				//
				assert(fLimit > 0.0);
				assert(fLimit <= 1.0);
				//
				IndivMapTypePtr sInd, sVar;
				//
				assert(pStore != nullptr);
				DatasetType oSet(datasetSigle);
				if (!pStore->find_dataset(oSet)) {
					return (std::make_tuple(sInd, sVar));
				}
				//
				TranformedStoreIndivSourceType src(pStore, datasetSigle);
				size_t nRows = 0, nCols = 0;
				std::vector<double> oSrcData;
				if (!src.get_data_array(nRows, nCols, oSrcData)) {
					return (std::make_tuple(sInd, sVar));
				}
				std::vector<double> oVars, oInds, oFreq;
				size_t nFacts = 0;
				if (!AnaCompo<double>::compute_anacompo(nRows, nCols, oSrcData, nFacts, oVars, oInds, oFreq)) {
					return (std::make_tuple(sInd, sVar));
				}
				size_t nf = 0;
				double ss = 0;
				for (size_t i = 0; i < nFacts; ++i) {
					ss += oFreq[i];
					++nf;
					if (ss >= fLimit) {
						break;
					}
				}
				if (nf > nFacts) {
					nf = nFacts;
				}
				ints_doubles_map weights;
				ints_vector factIds(nf);
				for (size_t i = 0; i < nf; ++i) {
					U aIndex = (U)(i + 1);
					factIds[i] = aIndex;
					double f = (oFreq[i] / ss);
					weights[aIndex] = f;
				}
#if defined(_MSC_VER)
				concurrency::parallel_invoke([&]() {
					ints_vector varIds;
					src.get_variables_ids(varIds);
					strings_vector varsNames(nCols);
					std::map<U, STRINGTYPE> xMap;
					size_t nv = 0;
					if (pStore->find_dataset_variables_count(oSet, nv)) {
						variables_vector vvx;
						if (pStore->find_dataset_variables(oSet, vvx, 0, nv)) {
							for (size_t i = 0; i < nCols; ++i) {
								U aIndex = varIds[i];
								auto it = std::find_if(vvx.begin(), vvx.end(), [aIndex](const VariableType &v)->bool {
									return (v.id() == aIndex);
								});
								if (it != vvx.end()) {
									auto v = *it;
									varsNames[i] = v.sigle();
								}
							}// i
							DataVectorIndivSourceType oProvider(nCols, nf, oVars, varIds, factIds, varsNames, weights);
							sVar = std::make_shared<IndivMapType>(&oProvider);
						}// ok
					}// var
				},
					[&]() {
					ints_vector indIds;
					src.get_indivs_ids(indIds);
					strings_vector indsNames(nRows);
					src.get_indivs_names(indsNames);
					DataVectorIndivSourceType oProvider(nRows, nf, oInds, indIds, factIds, indsNames, weights);
					sInd = std::make_shared<IndivMapType>(&oProvider);
				});
#else
				std::future<IndivMapTypePtr> fVar = std::async(std::launch::async, [&]()->IndivMapTypePtr {
					ints_vector varIds;
					src.get_variables_ids(varIds);
					strings_vector varsNames(nCols);
					std::map<U, STRINGTYPE> xMap;
					size_t nv = 0;
					if (!pStore->find_dataset_variables_count(oSet, nv)) {
						return (IndivMapTypePtr());
					}
					variables_vector vvx;
					if (!pStore->find_dataset_variables(oSet, vvx, 0, nv)) {
						return (IndivMapTypePtr());
					}
					for (size_t i = 0; i < nCols; ++i) {
						U aIndex = varIds[i];
						auto it = std::find_if(vvx.begin(), vvx.end(), [aIndex](const VariableType &v)->bool {
							return (v.id() == aIndex);
						});
						if (it != vvx.end()) {
							auto v = *it;
							varsNames[i] = v.sigle();
						}
					}// i
					DataVectorIndivSourceType oProvider(nCols, nf, oVars, varIds, factIds, varsNames, weights);
					return (std::make_shared<IndivMapType>(&oProvider));
				}); // fTaskVar
				ints_vector indIds;
				src.get_indivs_ids(indIds);
				strings_vector indsNames(nRows);
				src.get_indivs_names(indsNames);
				DataVectorIndivSourceType oProvider(nRows, nf, oInds, indIds, factIds, indsNames, weights);
				sInd = std::make_shared<IndivMapType>(&oProvider);
				sVar = fVar.get();
#endif // _MSC_VER
				return (std::make_tuple(sInd, sVar));
			}//create_indivmaps
	};
	/////////////////////////////////////
}// namespace info
///////////////////////////////////////
#endif // !__ANACOMPOINDIVPROVIDER_H__
