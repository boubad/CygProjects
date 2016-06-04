#pragma once
#ifndef __ANACOMPOINDIVPROVIDER_H__
#define __ANACOMPOINDIVPROVIDER_H__
/////////////////////////////////////
#include "transformed_storeindivsource.h"
#include "anacompo.h"
#include "datavector_indivprovider.h"
//////////////////////////////////////////
namespace info {
	////////////////////////////////////
	template<typename U = unsigned long, typename INTTYPE = unsigned long,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
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
		public:
			static DataVectorIndivSourceTuple create(StoreType *pStore,
				const STRINGTYPE &datasetSigle,const size_t nbFactsMax = 4) {
				DataVectorIndivSourcePtr sInd, sVar;
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
				if (!AnaCompo<double>::compute_anacompo(nRows, nCols, oSrcData, nFacts, oVars, oInds,oFreq)) {
					return (std::make_tuple(sInd, sVar));
				}
				ints_vector varIds, indIds;
				src.get_indivs_ids(indIds);
				src.get_variables_ids(varIds);
				strings_vector indsNames(nRows), varsNames(nCols);
				src.get_indivs_names(indsNames);
				std::map<U, STRINGTYPE> xMap;
				size_t nv = 0;
				if (!pStore->find_dataset_variables_count(oSet, nv)) {
					return (std::make_tuple(sInd, sVar));
				}
				variables_vector vvx;
				if (!pStore->find_dataset_variables(oSet, vvx, 0, nv)) {
					return (std::make_tuple(sInd, sVar));
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
				//
				size_t nf = nbFactsMax;
				if (nf > nFacts) {
					nf = nFacts;
				}
				ints_doubles_map weights;
				ints_vector factIds(nf);
				for (size_t i = 0; i < nf; ++i) {
					U aIndex = (U)(i + 1);
					factIds[i] = aIndex;
					double f = oFreq[i];
					weights[aIndex] = f;
				}
				//
				sInd.reset(new DataVectorIndivSourceType(nRows, nf, oInds, indIds, factIds, indsNames,weights));
				sVar.reset(new DataVectorIndivSourceType(nCols, nf, oVars, varIds, factIds, varsNames,weights));
				return (std::make_tuple(sInd, sVar));
			}// create
	};
	/////////////////////////////////////
}// namespace info
///////////////////////////////////////
#endif // !__ANACOMPOINDIVPROVIDER_H__
