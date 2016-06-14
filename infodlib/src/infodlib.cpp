//============================================================================
// Name        : infodlib.cpp
// Author      : Boubacar Diarra
// Version     :
// Copyright   : boubadiarra@gmail.com
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <dlib/gui_widgets.h>
#include <sstream>
#include <string>
/////////////////////////
#include <infodlib/dlib_matrice.h>
//////////////////////////////
#include <infotestdata.h>
//////////////////////////////
#include <global_defs.h>
/////////////////////////////
using namespace std;
using namespace dlib;
using namespace info;
//////////////////////////////
using DrawItemsType = DrawItems<STRINGTYPE, FLOATTYPE>;
using ints_vector = std::vector<IDTYPE>;
using doubles_vector = std::vector<double>;
using strings_vector = std::vector<STRINGTYPE>;
using ints_doubles_map = std::map<IDTYPE, double>;
////////////////////////////////////////
using IntraMatElemType = IntraMatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
using SourceType = IIndivSource<IDTYPE, STRINGTYPE>;
using RescritType = std::atomic<DISTANCETYPE>;
using IntraMatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
using IntraMatElemResultPtr = std::shared_ptr<IntraMatElemResultType>;
using queue_type = SharedQueue<IntraMatElemResultPtr>;
////////////////////////
using MatOrdType = IntraMatOrd<IDTYPE, DISTANCETYPE, STRINGTYPE>;
using backgrounder = MatElemResultBackgounder<IDTYPE, DISTANCETYPE, STRINGTYPE>;
////////////////////////////////
using MatriceDataType = MatriceData<IDTYPE, STRINGTYPE, DATATYPE>;
using MatriceWindowType = MatriceDisplayWindow<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE, DATATYPE>;
//////////////////////////////////////////
class matrice_win : public drawable_window
{
	MatriceWindowType mat_win;
	label lblVar;
	label lblInd;
	label lblStatus;
	MatriceDataType m_data;
protected:
	void process_result(IntraMatElemResultPtr oRes) {
		STRINGSTREAM os;
		label *pLabel = nullptr;
		IntraMatElemResultType *p = oRes.get();
		if (p != nullptr) {
			StageType stage = p->stage;
			if (stage == StageType::started) {
				os << "STARTED...\t";
			}
			else if (stage == StageType::finished) {
				os << "FINISHED!!!\t";
			}
			STRINGTYPE sx = p->sigle;
			if (!sx.empty()) {
				os << sx << "\t";
			}
			if (p->disposition == DispositionType::indiv) {
				os << "INDS\t";
				pLabel = &(lblInd);
			}
			else if (p->disposition == DispositionType::variable) {
				os << "VARS\t";
				pLabel = &(lblVar);
			}
			DISTANCETYPE crit = p->first;
			os << "Crit: " << crit << std::endl;
		}// p
		STRINGTYPE ss = os.str();
		if (pLabel != nullptr) {
			pLabel->set_text(ss);
		}
	}// process_result
	void my_redim(void) {
		unsigned long h = 0, w = 0;
		this->get_size(w, h);
		long x2 = w / 2;
		x2 += 5;
		lblVar.set_pos(5, 5);
		lblInd.set_pos(x2, 5);
		unsigned long hh = (5 + lblVar.height() + 5);
		unsigned long hy =  h - hh;
		if (hy > 0) {
			this->mat_win.set_pos(0, hh);
			this->mat_win.set_size(w, hy);
			this->mat_win.window_resized();
		}

	}// my_redim
	virtual void on_window_resized() {
		this->my_redim();
	}
public:
	matrice_win() :mat_win(*this),lblVar(*this),lblInd(*this),lblStatus(*this)
	{
		lblVar.set_pos(5, 5);
		size_t nRows = 0, nCols = 0;
		STRINGTYPE name;
		std::vector<STRINGTYPE> rowNames, colNames;
		std::vector<DATATYPE> data;
		InfoTestData::get_test_data(name, nRows, nCols, data, rowNames, colNames);
		this->m_data.initialize(name, nRows, nCols, data, rowNames, colNames);
		this->mat_win.set_callback([this](IntraMatElemResultPtr oRes) {
			this->process_result(oRes);
		});
		this->mat_win.arrange(&this->m_data);
		this->mat_win.show();
		show();
	}
	~matrice_win() {
	}

};
//////////////////////////
int main()
{
	// create our window
	matrice_win my_window;


	// wait until the user closes this window before we let the program
	// terminate.
	my_window.wait_until_closed();

	return 0;
}
////////////////////////////
