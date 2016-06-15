/*
* eigen3_impl.cpp
*
*  Created on: 18 oct. 2012
*      Author: boubad
*/
/////////////////////////////////////////////////////////
#if defined (__INTEL_COMPILER)
#pragma warning (disable:2196)
#endif
/////////////////////////////
#include <Eigen/Eigenvalues>
///////////////////////////////////////////
#include <cassert>
#include <vector>
#include <algorithm>
#include <cmath>
//////////////////////////////////////////////
namespace info {
	//////////////////////////////////////////
	using namespace Eigen;
	///////////////////////////////////////////////
	const double EPSILON = 0.000001;
	////////////////////////////////////////////////
	typedef std::pair<size_t, double> MyPair;
	typedef Matrix<double, Dynamic, Dynamic> MyMatrix;
	//
	struct MyComparePairDescFunc : public std::binary_function<MyPair, MyPair, bool> {
		bool operator()(const MyPair &v1, const MyPair &v2) const {
			return (v1.second > v2.second);
		} // operator()
	};
	// MyComparePairFunc
	///////////////////////////////////////////////////
	extern bool info_compute_eigen_impl(const int n, const double *pData,
		double *pVals, double *pVecs, int *pNbFacts /*= nullptr */) {
		//
		assert(n > 0);
		assert(pData != nullptr);
		assert(pVals != nullptr);
		assert(pVecs != nullptr);
		//
		MyMatrix m(n, n);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j <= i; ++j) {
				double v = pData[n * i + j];
				if (std::abs(v) < EPSILON) {
					v = 0;
				}
				m(i, j) = v;
				if (j != i) {
					m(j, i) = v;
				}
			}	// j
		}	// i
		EigenSolver<MyMatrix> solver(m);
		typename EigenSolver<MyMatrix>::EigenvalueType val = solver.eigenvalues();
		int nx = (int)val.rows();
		int ny = (int)val.cols();
		MyMatrix rval(nx, ny);
		for (int i = 0; i < nx; ++i) {
			for (int j = 0; j < ny; ++j) {
				rval(i, j) = val(i, j).real();
			}	// j
		}	// i
		typename EigenSolver<MyMatrix>::EigenvectorsType vec =
			solver.eigenvectors();
		nx = (int)vec.rows();
		ny = (int)vec.cols();
		MyMatrix rvec(nx, ny);
		for (int i = 0; i < nx; ++i) {
			for (int j = 0; j < ny; ++j) {
				rvec(i, j) = vec(i, j).real();
			}	// j
		}	// i
		std::vector<MyPair> oPairs;
		for (int i = 0; i < n; ++i) {
			double xx = rval(i, 0);
			if (xx > 0) {
				MyPair o((size_t)i, xx);
				oPairs.push_back(o);
			}
		}				// i
		int nFacts = (int)oPairs.size();
		bool bRet = (nFacts > 0);
		if (bRet) {
			std::sort(oPairs.begin(), oPairs.end(), MyComparePairDescFunc());
			for (int i = 0; i < nFacts; ++i) {
				const MyPair &pp = oPairs[i];
				const size_t ii = pp.first;
				double tt = pp.second;
				if (std::abs(tt) < EPSILON) {
					tt = 0;
				}
				pVals[i] = tt;
				for (int j = 0; j < n; ++j) {
					double xx = rvec(j, ii);
					if (std::abs(xx) < EPSILON) {
						xx = 0;
					}
					pVecs[j * nFacts + i] = xx;
				}// j
			}				// i
		}				// bRet
		if (pNbFacts != nullptr) {
			*pNbFacts = nFacts;
		}
		return (bRet);
	}// compute_eigen_impl
	 //////////////////////////////////////////
}	// namespace info
	/////////////////////////////////////////////////
