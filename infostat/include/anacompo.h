#pragma once
#ifndef ANACOMPO_H_
#define ANACOMPO_H_
/////////////////////////////
#include "info_includes.h"
#include "eigen_impl.h"
////////////////////////
namespace info {
	///////////////////////////////////////////////
	template<typename T = double>
	class AnaCompo {
	public:
		using AnaCompoType = AnaCompo<T>;
	public:
		template<typename X, typename Z, class ALLOCX, class ALLOCZ>
		static bool compute_eigen(const size_t nr,
			const std::vector<X, ALLOCX> &oSrc, size_t &nFacts,
			std::vector<Z, ALLOCZ> &oVals, std::vector<Z, ALLOCZ> &oVecs) {
			//
			using EigenSolverType = EigenSolver<X, double>;
			using EigenResultType = typename EigenSolverType::EigenResultType;
			using FloatType = typename EigenSolverType::FloatType;
			using FloatTypePtr = typename EigenSolverType::FloatTypePtr;
			//
			assert(nr > 1);
			const size_t nn = (size_t)(nr * nr);
			assert(oSrc.size() >= nn);
			//
			int nx = (int)nr;
			EigenResultType r = EigenSolverType::compute(nx, oSrc);
			int iFacts = std::get<0>(r);
			nFacts = (size_t)iFacts;
			FloatTypePtr vals = std::get<1>(r);
			FloatTypePtr vecs = std::get<2>(r);
			int nz = std::get<3>(r);
			FloatType *pVals = vals.get();
			FloatType *pVecs = vecs.get();
			if ((iFacts > 0) && (pVals != nullptr) && (pVecs != nullptr)) {
				oVals.resize(iFacts);
				oVecs.resize(iFacts * nz);
				for (int i = 0; i < iFacts; ++i) {
					oVals[i] = (Z)pVals[i];
					for (int j = 0; j < nz; ++j) {
						int pos = (j * iFacts) + i;
						oVecs[pos] = pVecs[pos];
					}// j
				}// i
				return (true);
			}// ok
			return (false);
		}

		template<typename X, class ALLOCX, class ALLOCT>
		static bool compute_anacompo(const size_t nr, const size_t nv,
			const std::vector<X, ALLOCX> &oSrc, std::vector<T, ALLOCT> &oMeans,
			std::vector<T, ALLOCT> &oStds, std::vector<T, ALLOCT> &oCorr,
			size_t &nFacts, std::vector<T, ALLOCT> &oFreq,
			std::vector<T, ALLOCT> &oVals, std::vector<T, ALLOCT> &oVecs,
			std::vector<T, ALLOCT> &oVars, std::vector<T, ALLOCT> &oInds) {
			//
			assert(nv > 1);
			assert(nr > nv);
			//
			const size_t nnv = nv * nv;
			const size_t nnr = nr * nv;
			//
			assert(oSrc.size() >= nnr);
			//
			oMeans = std::vector<T, ALLOCT >(nv);
			oStds = std::vector<T, ALLOCT >(nv);
			oCorr = std::vector<T, ALLOCT >(nnv);
			//
			std::vector<double> oTemp(nnr);
			std::vector<double> xCorr(nnv);
			std::vector<double> xStds(nv);
			bool bRet = true;
			double dnr = (double)nr;
			for (size_t ivar = 0; ivar < nv; ++ivar) {
				xCorr[ivar * nv + ivar] = 1.0;
				oCorr[ivar * nv + ivar] = (T) 1.0;
				double s = 0.0;
				for (size_t i = 0; i < nr; ++i) {
					const size_t k = i * nv + ivar;
					double x = (double)oSrc[k];
					oTemp[k] = x;
					s += x;
				} // i
				double moy = s / dnr;
				oMeans[ivar] = (T)moy;
				s = 0.0;
				for (size_t i = 0; i < nr; ++i) {
					const size_t k = i * nv + ivar;
					double x = oTemp[k] - moy;
					oTemp[k] = x;
					s += x * x;
				} // i
				double dvar = s / dnr;
				if (dvar > 0.0) {
					double ec1 = std::sqrt(dvar);
					xStds[ivar] = ec1;
					oStds[ivar] = (T)ec1;
					for (size_t ivar1 = 0; ivar1 < ivar; ++ivar1) {
						double s = 0.0;
						for (size_t i = 0; i < nr; ++i) {
							double x = oTemp[i * nv + ivar] * oTemp[i * nv + ivar1];
							s += x;
						}
						s /= dnr;
						double r = s / (ec1 * xStds[ivar1]);
						T rt = (T)r;
						xCorr[ivar * nv + ivar1] = r;
						oCorr[ivar * nv + ivar1] = rt;
						xCorr[ivar1 * nv + ivar] = r;
						oCorr[ivar1 * nv + ivar] = rt;
					}
				}
				else {
					bRet = false;
				}
			}
			if (bRet) {
				for (size_t ivar = 0; ivar < nv; ++ivar) {
					double vec = xStds[ivar];
					for (size_t i = 0; i < nr; ++i) {
						const size_t k = i * nv + ivar;
						double x = oTemp[k] / vec;
						oTemp[k] = x;
					}
				}
			}
			nFacts = 0;
			std::vector<double> xVals, xVecs;
			double stotal = 0.0;
			bRet = bRet && AnaCompoType::compute_eigen(nv, xCorr, nFacts, xVals, xVecs);
			if (bRet) {
				oVals.resize(nFacts);
				oVecs.resize(nFacts * nv);
				for (size_t i = 0; i < nFacts; ++i) {
					double x = xVals[i];
					oVals[i] = (T)x;
					stotal += x;
				}
				if (stotal <= 0) {
					bRet = false;
				}
			}
			if (bRet) {
				oFreq.resize(nFacts);
				oVecs.resize(nFacts * nv);
				oVars.resize(nFacts * nv);
				oInds.resize(nFacts * nr);
				double act2 = std::sqrt((double)nv);
				for (size_t iFact = 0; iFact < nFacts; ++iFact) {
					double val = xVals[iFact];
					oFreq[iFact] = (T)(val / stotal);
					double fval = std::sqrt(val);
					for (size_t ivar = 0; ivar < nv; ++ivar) {
						const size_t k = ivar * nFacts + iFact;
						double vx = xVecs[k];
						oVecs[k] = (T)vx;
						oVars[k] = (T)(fval * xVecs[k]);
					}
					for (size_t irow = 0; irow < nr; ++irow) {
						double s = 0.0;
						for (size_t ivar = 0; ivar < nv; ++ivar) {
							double x = oTemp[irow * nv + ivar]
								* xVecs[ivar * nFacts + iFact];
							s += x;
						}
						oInds[irow * nFacts + iFact] = (T)(s / act2);
					}
				}
			}
			return (bRet);
		}// compute_anacompo
		template<typename X, class ALLOCX, class ALLOCT>
		static bool compute_anacompo(const size_t nr, const size_t nv,
			const std::vector<X, ALLOCX> &oSrc, size_t &nFacts,
			std::vector<T, ALLOCT> &oVars, std::vector<T, ALLOCT> &oInds, std::vector<T, ALLOCT> &oFreq) {
			std::vector<T, ALLOCT> oMeans, oStds, oCorr, oVals, oVecs;
			return (AnaCompoType::compute_anacompo(nr, nv, oSrc, oMeans,
				oStds, oCorr, nFacts, oFreq, oVals, oVecs, oVars, oInds));
		}

	};
	// class IntraEigenSolver<T>
	//////////////////////////////////////////
}
////////////////////////////////////////

#endif /* ANACOMPO_H_ */
