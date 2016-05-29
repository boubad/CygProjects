/*
 * matrice.h
 *
 *  Created on: 29 mai 2016
 *      Author: boubad
 */

#ifndef MATRICE_H_
#define MATRICE_H_
/////////////////////////////////
#include "matelem.h"
//////////////////////////////////
namespace info {
///////////////////////////////////
template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
class MatOrd: public InterruptObject, private boost::noncopyable {
public:
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	//
	using DistanceMapType = typename MatElemType::DistanceMapType;
	using MatElemResultType = typename MatElemType::MatElemResultType;
	using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
	using SignalType = typename boost::signals2::signal<void (MatElemResultPtr,MatElemResultPtr)>;
	using SlotType = typename SignalType::slot_type;
	using ConnectionType =typename MatElemType::ConnectionType;
	using SourceType = typename MatElemType::SourceType;
	using ints_vector = typename MatElemType::ints_vector;
	using sizets_vector = typename MatElemType::sizets_vector;
	using RescritType = typename MatElemType::RescritType;
	using IndivType = typename MatElemType::IndivType;
	using IndivTypePtr = typename MatElemType::IndivTypePtr;
	using MatOrdType = MatOrd<IDTYPE,DISTANCETYPE,STRINGTYPE>;
private:
	std::unique_ptr<MatElemType> m_vars;
	std::unique_ptr<MatElemType> m_inds;
	ConnectionType m_connectVars;
	ConnectionType m_connectInds;
	SignalType m_signal;
	MatElemResultPtr m_varsResult;
	MatElemResultPtr m_indsResults;
	ints_vector m_varids;
	ints_vector m_indids;
	std::unique_ptr<DistanceMapType> m_indsmap;
	std::unique_ptr<DistanceMapType> m_varsmap;
	//
	std::mutex _mutex;
protected:
	bool prep_vars(SourceType *pProvider) {
		this->m_varsmap.reset(new DistanceMapType(pProvider));
		DistanceMapType *pMap = this->m_varsmap.get();
		assert(pMap != nullptr);
		pMap->indexes(this->m_varids);
		this->m_vars.reset(
				new MatElemType(pMap, &(this->m_varids), nullptr,
						this->get_cancelleable_flag()));
		MatElemType *pMat = this->m_vars.get();
		assert(pMat != nullptr);
		this->m_connectVars = pMat->connect([&](MatElemResultPtr oCrit) {
			std::lock_guard<std::mutex> oLock(this->_mutex);
			this->m_varsResult = oCrit;
			MatElemResultPtr  o = this->m_indsResults;
			this->m_signal(o, oCrit);
		});
		return (pMat->process_signal());
	} // prep_vars
	bool prep_inds(SourceType *pProvider) {
		this->m_indsmap.reset(new DistanceMapType(pProvider));
		DistanceMapType *pMap = this->m_indsmap.get();
		assert(pMap != nullptr);
		pMap->indexes(this->m_indids);
		this->m_inds.reset(
				new MatElemType(pMap, &(this->m_indids), nullptr,
						this->get_cancelleable_flag()));
		MatElemType *pMat = this->m_inds.get();
		assert(pMat != nullptr);
		this->m_connectInds = pMat->connect([&](MatElemResultPtr oCrit) {
			std::lock_guard<std::mutex> oLock(this->_mutex);
			this->m_indsResults = oCrit;
			MatElemResultPtr o = this->m_varsResult;
			this->m_signal(oCrit, o);
		});
		return (pMat->process_signal());
	} // prep_inds
public:
	MatOrd(std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel) {
	} // MatOrd
	virtual ~MatOrd() {
	}
	ConnectionType connect(const SlotType &subscriber) {
		return m_signal.connect(subscriber);
	}
	bool process(SourceType *pIndsSource, SourceType *pVarsSource,
			std::atomic_bool *pCancel = nullptr) {
		assert(pIndsSource != nullptr);
		assert(pVarsSource != nullptr);
		if (pCancel != nullptr) {
			this->set_cancelleable_flag(pCancel);
		}
		std::future<bool> bRow = std::async([&]()->bool {
			return (this->prep_inds(pIndsSource));
		});
		std::future<bool> bCol = std::async([&]()->bool {
			return (this->prep_vars(pVarsSource));
		});
		bool bRet = bCol.get() && bRow.get();
		return  (bRet && ((this->check_interrupt()) ? false : true));
	} // process
	void get_ids(ints_vector &indids, ints_vector &varids) {
		indids.clear();
		varids.clear();
		{
			std::lock_guard<std::mutex> oLock(_mutex);
			MatElemResultType *p1 = this->m_indsResults.get();
			if (p1 != nullptr) {
				const sizets_vector &src = p1->second;
				const ints_vector & dest = this->m_indids;
				for (auto it = src.begin(); it != src.end(); ++it) {
					size_t pos = *it;
					assert(pos < dest.size());
					indids.push_back(dest[pos]);
				}
			}
			MatElemResultType *p2 = this->m_varsResult.get();
			if (p2 != nullptr) {
				const sizets_vector &src = p2->second;
				const ints_vector & dest = this->m_varids;
				for (auto it = src.begin(); it != src.end(); ++it) {
					size_t pos = *it;
					assert(pos < dest.size());
					varids.push_back(dest[pos]);
				}
			}
		}
	} // getIds
};
////////////////////////////////////
}// namespace info
//////////////////////////////
#endif /* MATRICE_H_ */
