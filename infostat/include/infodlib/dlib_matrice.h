#pragma once
#ifndef __DLIB_MATRICE__H__
#define __DLIB_MATRICE__H__
/////////////////////////////////
#include <dlib/threads.h>
#include <dlib/gui_widgets.h>
///////////////////////////////////
#include "../intramat.h"
#include "../base_drawitem.h"
#include "../datavector_indivprovider.h"
//////////////////////////////////////
namespace info {
	////////////////////////////////////////////
	template <typename STRINGTYPE, typename FLOATTYPE>
	class DLibDrawContext : public info::DrawContext<STRINGTYPE, FLOATTYPE> {
		using DrawItem = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
		const dlib::canvas &m_canvas;
	public:
		DLibDrawContext(const dlib::canvas &m, const DrawContextParams *params) :info::DrawContext<STRINGTYPE, FLOATTYPE>(params), m_canvas(m) {
		}
		virtual ~DLibDrawContext() {}
	public:
		virtual void draw(DrawItem *pItem, coord_type x0 = 0, coord_type y0 = 0) const {
			if (pItem == nullptr) {
				return;
			}
			const DrawContextParams *pParams = this->draw_params();
			assert(pParams != nullptr);
			dist_type w = pParams->dx;
			dist_type  h = pParams->dy;
			if ((w <= 0) || (h <= 0)) {
				return;
			}
			dlib::rectangle r(x0, y0, x0 + w, y0 + h);
			dlib::rectangle area = m_canvas.intersect(r);
			if (area.is_empty() == true) {
				return;
			}
			MatCellType aType = pItem->type();
			switch (aType)
			{
			case MatCellType::summaryIndCell:
			{
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					dist_type hx = (h < w) ? h : w;
					double rf = hx * std::sqrt(v);
					int r = (int)rf;
					dlib::point center(x0 + (w / 2), y0 + (h / 2));
					dlib::rgb_pixel color(pParams->sumindcolor.red, pParams->sumindcolor.green, pParams->sumindcolor.blue);
					dlib::draw_solid_circle(m_canvas, center, r, color);
				}
			}
			break;
			case MatCellType::summaryVarCell:
			{
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					dist_type hx = (h < w) ? h : w;
					double rf = hx * std::sqrt(v);
					int r = (int)rf;
					dlib::point center(x0 + (w / 2), y0 + (h / 2));
					dlib::rgb_pixel color(pParams->sumvarcolor.red, pParams->sumvarcolor.green, pParams->sumvarcolor.blue);
					dlib::draw_solid_circle(m_canvas, center, r, color);
				}
			}
			break;
			case MatCellType::plainCell:
			{
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					double f = 256.0 * v;
					int n = (int)f;
					if (n > 255) {
						n = 255;
					}
					dlib::rectangle rr(x0, y0, x0 + w, y0 + h);
					dlib::rgb_pixel color((unsigned char)n, (unsigned char)n, (unsigned char)n);
					dlib::fill_rect(m_canvas, rr, color);
				}
			}
			break;
			case MatCellType::histogCell:
			{
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					double f = h * v;
					int hx = (int)f;
					dlib::rectangle rr(x0, y0 + h - hx, x0 + w, y0 + h);
					unsigned char c = pParams->downcolor.red;
					dlib::rgb_pixel color(c, c, c);
					dlib::fill_rect(m_canvas, rr, color);
				}
			}
			break;
			default:
				break;
			}
		}// draw
	};// class DLibDrawContext
	/////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
	class MatOrdAgent : public dlib::threaded_object {
	public:
		using MatElemType = IntraMatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using DistanceMapType = typename MatElemType::DistanceMapType;
		using MatElemResultType = typename MatElemType::IntraMatElemResultType;
		using MatElemResultPtr = typename MatElemType::IntraMatElemResultPtr;
		using SourceType = typename MatElemType::SourceType;
		using MatOrdType = IntraMatOrd<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using queue_type = MatElemResultClient<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	private:
		queue_type *m_pdest;
		std::atomic<bool> m_cancel;
		SourceType *m_pIndSource;
		SourceType *m_pVarSource;
		std::unique_ptr<MatOrdType> m_matord;
	private:
		virtual void thread() {
			if ((this->m_pIndSource != nullptr) && (this->m_pVarSource != nullptr)) {
				MatOrdType *pMat = m_matord.get();
				if (pMat != nullptr) {
					this->m_cancel.store(false);
					pMat->arrange(this->m_pIndSource, this->m_pVarSource, this->m_pdest);
				}// pMat
			}// soucres
		}// thread
	public:
		MatOrdAgent(queue_type *pDest = nullptr) :m_pdest(pDest), m_cancel(false), m_pIndSource(nullptr), m_pVarSource(nullptr) {
			m_matord.reset(new MatOrdType(m_pdest, &m_cancel));
			MatOrdType *pMat = m_matord.get();
			assert(pMat != nullptr);
		}
		MatOrdAgent(SourceType *pInd, SourceType *pVar, queue_type *pDest = nullptr) : m_pdest(pDest),
			m_cancel(false), m_pIndSource(pInd), m_pVarSource(pVar) {
			m_matord.reset(new MatOrdType(m_pdest, &m_cancel));
			MatOrdType *pMat = m_matord.get();
			assert(pMat != nullptr);
			this->start();
		}
		virtual ~MatOrdAgent() {}
	public:
		bool arrange(SourceType *pInd, SourceType *pVar, queue_type *pDest = nullptr) {
			assert(pInd != nullptr);
			assert(pVar != nullptr);
			if (!this->is_running()) {
				this->m_pIndSource = pInd;
				this->m_pVarSource = pVar;
				if (pDest != nullptr) {
					this->m_pdest = pDest;
				}
				this->start();
				return (true);
			}
			return (false);
		}// arrange
		void cancel(void) {
			if (this->is_running()) {
				this->m_cancel.store(true);
				this->stop();
			}
		}// cancel
	};// class MatOrdAgent<IDTYPE,DISTANCETYPE,STRINGTYPE>
	//////////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE, typename FLOATTYPE, typename DATATYPE>
	class MatriceDisplayWindow : public dlib::button {
		using MatElemType = IntraMatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using MatElemResultType = typename MatElemType::IntraMatElemResultType;
		using MatElemResultPtr = typename MatElemType::IntraMatElemResultPtr;
		using MatOrdAgentType = MatOrdAgent<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using MatriceDataType = MatriceData<IDTYPE, STRINGTYPE, DATATYPE>;
		using ContextType = DLibDrawContext<STRINGTYPE, FLOATTYPE>;
		using DrawItemsType = DrawItems<STRINGTYPE, FLOATTYPE>;
		using MatElemFunctionType = std::function<void(MatElemResultPtr)>;
		using queue_type = MatElemResultClient<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using strings_vector = std::vector<STRINGTYPE>;
	protected:
		
		virtual void draw(const dlib::canvas& c) const
		{
			DrawItemsType *p = this->m_items.get();
			if (p != nullptr) {
				unsigned long ww = 0, hh = 0;
				ww = this->width();
				hh = this->height();
				dlib::rectangle r(0, 0, ww, hh);
				dlib::fill_rect(c, r, dlib::rgb_pixel(255, 255, 255));
				ContextType oContext(c, &oDrawContextParams);
				p->draw(&oContext);
			}// p
		}// draw
	private:
		void my_draw(void)  {
			this->show();
			/*
			unsigned long ww = 0, hh = 0;
			base_window& pp = this->parent_window();
			pp.get_size(ww, hh);
			dlib::rectangle r(0, 0, ww, hh);
			pp.invalidate_rectangle(r);
			*/
		}

		void init_data(void) {
			MatriceDataType *pMat = this->m_pMatData;
			if (pMat == nullptr) {
				return;
			}
			//
			size_t nRows = pMat->rows_count();
			size_t nCols = pMat->cols_count();
			const std::vector<DATATYPE> & data = pMat->data();
			std::vector<double> varsSum, indsSum;
			varsSum.resize(nCols);
			indsSum.resize(nRows);
			for (size_t i = 0; i < nCols; ++i) {
				double s1 = 0;
				double s2 = 0;
				for (size_t j = 0; j < nRows; ++j) {
					double x = data[j * nCols + i];
					s1 += x;
					s2 += x * x;
				}// j
				s1 /= nRows;
				s2 /= nRows;
				s2 = std::sqrt(s2 - (s1 * s1));
				varsSum[i] = s2 / s1;
			}// i
			for (size_t i = 0; i < nRows; ++i) {
				indsSum[i] = data[i * nCols];
			}
			DrawItemsType *p = new DrawItemsType();
			assert(p != nullptr);
			//MatCellType aType(MatCellType::histogCell);
			MatCellType aType(MatCellType::plainCell);
			const strings_vector &rowNames = pMat->rows_names();
			const strings_vector &colNames = pMat->cols_names();
			bool bRet = p->initialize(aType, nRows, nCols, data, rowNames, colNames, indsSum, varsSum);
			assert(bRet);
			this->m_items.reset(p);
		}// init_data
		void run_matrice(void) {
			MatriceDataType *pData = this->m_pMatData;
			if (pData == nullptr) {
				return;
			}
			//
			queue_type *pf =  new queue_type([this](MatElemResultPtr oRes) {
				MatElemResultType *p = oRes.get();
				if (p != nullptr) {
					if (p->stage == StageType::finished) {
						if (p->disposition == DispositionType::indiv) {
							this->m_doneind.store(true);
						}
						else if (p->disposition == DispositionType::variable) {
							this->m_donevar.store(true);
						}
						if (this->m_donevar.load() && this->m_doneind.load()) {
							DrawContextParams &ctx = this->oDrawContextParams;
							ctx.downcolor = ctx.donecolor;
							ctx.bIndsSum = true;
							ctx.bVarsSum = true;
							this->my_draw();
						}
					}// finished
					DrawItemsType *pItems = this->m_items.get();
					pItems->set_result(oRes);
					this->my_draw();
					(this->m_dest)(oRes);
				}
			});
			this->m_callback.reset(pf);
			//
			this->m_matord.reset(new MatOrdAgentType(pf));
			MatOrdAgentType *pMat = this->m_matord.get();
			DrawContextParams &ctx = this->oDrawContextParams;
			ctx.downcolor = InfoColor(0, 0, 0);
			ctx.bIndsSum = false;
			ctx.bVarsSum = false;
			pMat->arrange(pData->indiv_provider(), pData->variable_provider());
		}// run_matrice
	public:
		MatriceDisplayWindow(dlib::drawable_window& w) : dlib::button(w),
			m_dest([](MatElemResultPtr oRes) {}),
			m_donevar(false), m_doneind(false), m_pMatData(nullptr) {
			this->show();
		}//MatriceDisplayWindow
		~MatriceDisplayWindow() {}
		void set_callback(MatElemFunctionType f) {
			this->m_dest = f;
		}
		void arrange(MatriceDataType *pData) {
			this->m_pMatData = pData;
			this->init_data();
			this->run_matrice();
			this->show();
		}
		void window_resized() {
			unsigned long ww = 0, hh = 0;
			ww = this->width();
			hh = this->height();
			if ((ww > 0) && (hh > 0)) {
				MatriceDataType *pMat = this->m_pMatData;
				if (pMat != nullptr) {
					size_t nRows = pMat->rows_count();
					size_t nCols = pMat->cols_count();
					unsigned long w = ww / (nCols + 2);
					unsigned long h = hh / (nRows + 2);
					if (w < 8) {
						w = 8;
					}
					if (h < 8) {
						h = 8;
					}
					oDrawContextParams.dx = w;
					oDrawContextParams.dy = h;
					my_draw();
				}// pMat
			}
		}
	private:
		MatElemFunctionType m_dest;
		std::unique_ptr<queue_type> m_callback;
		std::atomic<bool> m_donevar;
		std::atomic<bool> m_doneind;
		MatriceDataType *m_pMatData;
		DrawContextParams oDrawContextParams;
		std::unique_ptr<MatOrdAgentType> m_matord;
		std::unique_ptr<DrawItemsType> m_items;
	};// class MatriceDisplayWindow
	////////////////////////////////////////
}// namespace info
////////////////////////////////////
#endif // !__DLIB_MATRICE__H__

