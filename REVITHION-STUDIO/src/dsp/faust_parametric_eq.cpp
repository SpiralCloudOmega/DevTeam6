/* ------------------------------------------------------------
author: "SpiralCloudOmega"
license: "MIT"
name: "Parametric EQ"
Code generated with Faust 2.70.3 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -cn ParametricEQ -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __ParametricEQ_H__
#define  __ParametricEQ_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS ParametricEQ
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float ParametricEQ_faustpower2_f(float value) {
	return value * value;
}

class ParametricEQ : public dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fConst2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	float fVec0[2];
	float fRec5[2];
	float fRec4[3];
	FAUSTFLOAT fHslider8;
	float fRec7[2];
	float fRec6[3];
	float fRec3[3];
	float fRec2[3];
	float fVec1[2];
	float fRec1[2];
	float fRec0[3];
	FAUSTFLOAT fHslider9;
	float fRec9[2];
	float fRec8[3];
	float fVec2[2];
	float fRec15[2];
	float fRec14[3];
	float fRec17[2];
	float fRec16[3];
	float fRec13[3];
	float fRec12[3];
	float fVec3[2];
	float fRec11[2];
	float fRec10[3];
	float fRec19[2];
	float fRec18[3];
	
 public:
	ParametricEQ() {}

	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.2.0");
		m->declare("author", "SpiralCloudOmega");
		m->declare("basics.lib/bypass2:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.12.0");
		m->declare("compile_options", "-lang cpp -ct 1 -cn ParametricEQ -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("description", "4-band fully parametric EQ (low shelf, 2 peaking, high shelf)");
		m->declare("filename", "parametric_eq.dsp");
		m->declare("filters.lib/filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/filterbank:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/highshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/low_shelf:author", "Julius O. Smith III");
		m->declare("filters.lib/low_shelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/low_shelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/lowshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowshelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("license", "MIT");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.7.0");
		m->declare("name", "Parametric EQ");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 3.1415927f / fConst0;
		fConst2 = 6.2831855f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(8e+03f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(2e+03f);
		fHslider3 = FAUSTFLOAT(1.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(5e+02f);
		fHslider6 = FAUSTFLOAT(1.0f);
		fHslider7 = FAUSTFLOAT(1e+02f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec5[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
			fRec4[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec7[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fRec6[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = l5 + 1) {
			fRec3[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec2[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec1[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec0[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec8[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fVec2[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec15[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec14[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec16[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec13[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec12[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fVec3[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec11[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec10[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec19[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec18[l23] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual ParametricEQ* clone() {
		return new ParametricEQ();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openHorizontalBox("Parametric EQ");
		ui_interface->openHorizontalBox("Band 1 Low Shelf");
		ui_interface->declare(&fHslider7, "0", "");
		ui_interface->declare(&fHslider7, "style", "knob");
		ui_interface->declare(&fHslider7, "unit", "Hz");
		ui_interface->addHorizontalSlider("Freq", &fHslider7, FAUSTFLOAT(1e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider8, "1", "");
		ui_interface->declare(&fHslider8, "style", "knob");
		ui_interface->declare(&fHslider8, "unit", "dB");
		ui_interface->addHorizontalSlider("Gain", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(-15.0f), FAUSTFLOAT(15.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Band 2 Peak");
		ui_interface->declare(&fHslider5, "0", "");
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->declare(&fHslider5, "unit", "Hz");
		ui_interface->addHorizontalSlider("Freq", &fHslider5, FAUSTFLOAT(5e+02f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider4, "1", "");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->declare(&fHslider4, "unit", "dB");
		ui_interface->addHorizontalSlider("Gain", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(-15.0f), FAUSTFLOAT(15.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider6, "2", "");
		ui_interface->declare(&fHslider6, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider6, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Band 3 Peak");
		ui_interface->declare(&fHslider2, "0", "");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->declare(&fHslider2, "unit", "Hz");
		ui_interface->addHorizontalSlider("Freq", &fHslider2, FAUSTFLOAT(2e+03f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(1.5e+04f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider1, "1", "");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "dB");
		ui_interface->addHorizontalSlider("Gain", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(-15.0f), FAUSTFLOAT(15.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider3, "2", "");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Q", &fHslider3, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Band 4 High Shelf");
		ui_interface->declare(&fHslider0, "0", "");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->declare(&fHslider0, "unit", "Hz");
		ui_interface->addHorizontalSlider("Freq", &fHslider0, FAUSTFLOAT(8e+03f), FAUSTFLOAT(2e+03f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider9, "1", "");
		ui_interface->declare(&fHslider9, "style", "knob");
		ui_interface->declare(&fHslider9, "unit", "dB");
		ui_interface->addHorizontalSlider("Gain", &fHslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(-15.0f), FAUSTFLOAT(15.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
		ui_interface->declare(&fCheckbox0, "0", "");
		ui_interface->addCheckButton("Bypass", &fCheckbox0);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = std::tan(fConst1 * float(fHslider0));
		float fSlow2 = ParametricEQ_faustpower2_f(fSlow1);
		float fSlow3 = 2.0f * (1.0f - 1.0f / fSlow2);
		float fSlow4 = 1.0f / fSlow1;
		float fSlow5 = (fSlow4 + -1.0f) / fSlow1 + 1.0f;
		float fSlow6 = 1.0f / ((fSlow4 + 1.0f) / fSlow1 + 1.0f);
		float fSlow7 = float(fHslider1);
		int iSlow8 = fSlow7 > 0.0f;
		float fSlow9 = float(fHslider2);
		float fSlow10 = float(fHslider3) * std::sin(fConst2 * fSlow9);
		float fSlow11 = fConst1 * (fSlow9 * std::pow(1e+01f, 0.05f * std::fabs(fSlow7)) / fSlow10);
		float fSlow12 = fConst1 * (fSlow9 / fSlow10);
		float fSlow13 = ((iSlow8) ? fSlow12 : fSlow11);
		float fSlow14 = std::tan(fConst1 * fSlow9);
		float fSlow15 = 1.0f / fSlow14;
		float fSlow16 = fSlow15 * (fSlow15 + fSlow13) + 1.0f;
		float fSlow17 = ((iSlow8) ? fSlow11 : fSlow12);
		float fSlow18 = fSlow15 * (fSlow15 - fSlow17) + 1.0f;
		float fSlow19 = 2.0f * (1.0f - 1.0f / ParametricEQ_faustpower2_f(fSlow14));
		float fSlow20 = fSlow15 * (fSlow15 - fSlow13) + 1.0f;
		float fSlow21 = float(fHslider4);
		int iSlow22 = fSlow21 > 0.0f;
		float fSlow23 = float(fHslider5);
		float fSlow24 = float(fHslider6) * std::sin(fConst2 * fSlow23);
		float fSlow25 = fConst1 * (fSlow23 * std::pow(1e+01f, 0.05f * std::fabs(fSlow21)) / fSlow24);
		float fSlow26 = fConst1 * (fSlow23 / fSlow24);
		float fSlow27 = ((iSlow22) ? fSlow26 : fSlow25);
		float fSlow28 = std::tan(fConst1 * fSlow23);
		float fSlow29 = 1.0f / fSlow28;
		float fSlow30 = fSlow29 * (fSlow29 + fSlow27) + 1.0f;
		float fSlow31 = ((iSlow22) ? fSlow25 : fSlow26);
		float fSlow32 = fSlow29 * (fSlow29 - fSlow31) + 1.0f;
		float fSlow33 = 2.0f * (1.0f - 1.0f / ParametricEQ_faustpower2_f(fSlow28));
		float fSlow34 = fSlow29 * (fSlow29 - fSlow27) + 1.0f;
		float fSlow35 = std::tan(fConst1 * float(fHslider7));
		float fSlow36 = 1.0f / ParametricEQ_faustpower2_f(fSlow35);
		float fSlow37 = 2.0f * (1.0f - fSlow36);
		float fSlow38 = 1.0f / fSlow35;
		float fSlow39 = (fSlow38 + -1.0f) / fSlow35 + 1.0f;
		float fSlow40 = 1.0f / ((fSlow38 + 1.0f) / fSlow35 + 1.0f);
		float fSlow41 = 1.0f - fSlow38;
		float fSlow42 = 1.0f / (fSlow38 + 1.0f);
		float fSlow43 = std::pow(1e+01f, 0.05f * float(fHslider8));
		float fSlow44 = fSlow29 * (fSlow29 + fSlow31) + 1.0f;
		float fSlow45 = fSlow15 * (fSlow15 + fSlow17) + 1.0f;
		float fSlow46 = 1.0f - fSlow4;
		float fSlow47 = 1.0f / (fSlow4 + 1.0f);
		float fSlow48 = std::pow(1e+01f, 0.05f * float(fHslider9)) / fSlow2;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = fSlow19 * fRec2[1];
			float fTemp1 = fSlow33 * fRec3[1];
			float fTemp2 = float(input0[i0]);
			float fTemp3 = ((iSlow0) ? 0.0f : fTemp2);
			fVec0[0] = fTemp3;
			fRec5[0] = -(fSlow42 * (fSlow41 * fRec5[1] - (fTemp3 + fVec0[1])));
			fRec4[0] = fRec5[0] - fSlow40 * (fSlow39 * fRec4[2] + fSlow37 * fRec4[1]);
			fRec7[0] = -(fSlow42 * (fSlow41 * fRec7[1] - fSlow38 * (fTemp3 - fVec0[1])));
			fRec6[0] = fRec7[0] - fSlow40 * (fSlow39 * fRec6[2] + fSlow37 * fRec6[1]);
			fRec3[0] = fSlow40 * (fSlow36 * (fRec6[2] + (fRec6[0] - 2.0f * fRec6[1])) + fSlow43 * (fRec4[2] + fRec4[0] + 2.0f * fRec4[1])) - (fRec3[2] * fSlow34 + fTemp1) / fSlow30;
			fRec2[0] = (fTemp1 + fRec3[0] * fSlow44 + fRec3[2] * fSlow32) / fSlow30 - (fRec2[2] * fSlow20 + fTemp0) / fSlow16;
			float fTemp4 = (fTemp0 + fRec2[0] * fSlow45 + fRec2[2] * fSlow18) / fSlow16;
			fVec1[0] = fTemp4;
			fRec1[0] = -(fSlow47 * (fSlow46 * fRec1[1] - fSlow4 * (fTemp4 - fVec1[1])));
			fRec0[0] = fRec1[0] - fSlow6 * (fSlow5 * fRec0[2] + fSlow3 * fRec0[1]);
			fRec9[0] = -(fSlow47 * (fSlow46 * fRec9[1] - (fTemp4 + fVec1[1])));
			fRec8[0] = fRec9[0] - fSlow6 * (fSlow5 * fRec8[2] + fSlow3 * fRec8[1]);
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp2 : fSlow6 * (fRec8[2] + fRec8[0] + 2.0f * fRec8[1] + fSlow48 * (fRec0[0] + fRec0[2] - 2.0f * fRec0[1]))));
			float fTemp5 = fSlow19 * fRec12[1];
			float fTemp6 = fSlow33 * fRec13[1];
			float fTemp7 = float(input1[i0]);
			float fTemp8 = ((iSlow0) ? 0.0f : fTemp7);
			fVec2[0] = fTemp8;
			fRec15[0] = -(fSlow42 * (fSlow41 * fRec15[1] - (fTemp8 + fVec2[1])));
			fRec14[0] = fRec15[0] - fSlow40 * (fSlow39 * fRec14[2] + fSlow37 * fRec14[1]);
			fRec17[0] = -(fSlow42 * (fSlow41 * fRec17[1] - fSlow38 * (fTemp8 - fVec2[1])));
			fRec16[0] = fRec17[0] - fSlow40 * (fSlow39 * fRec16[2] + fSlow37 * fRec16[1]);
			fRec13[0] = fSlow40 * (fSlow36 * (fRec16[2] + (fRec16[0] - 2.0f * fRec16[1])) + fSlow43 * (fRec14[2] + fRec14[0] + 2.0f * fRec14[1])) - (fSlow34 * fRec13[2] + fTemp6) / fSlow30;
			fRec12[0] = (fTemp6 + fRec13[0] * fSlow44 + fSlow32 * fRec13[2]) / fSlow30 - (fSlow20 * fRec12[2] + fTemp5) / fSlow16;
			float fTemp9 = (fTemp5 + fRec12[0] * fSlow45 + fSlow18 * fRec12[2]) / fSlow16;
			fVec3[0] = fTemp9;
			fRec11[0] = -(fSlow47 * (fSlow46 * fRec11[1] - fSlow4 * (fTemp9 - fVec3[1])));
			fRec10[0] = fRec11[0] - fSlow6 * (fSlow5 * fRec10[2] + fSlow3 * fRec10[1]);
			fRec19[0] = -(fSlow47 * (fSlow46 * fRec19[1] - (fTemp9 + fVec3[1])));
			fRec18[0] = fRec19[0] - fSlow6 * (fSlow5 * fRec18[2] + fSlow3 * fRec18[1]);
			output1[i0] = FAUSTFLOAT(((iSlow0) ? fTemp7 : fSlow6 * (fRec18[2] + fRec18[0] + 2.0f * fRec18[1] + fSlow48 * (fRec10[0] + fRec10[2] - 2.0f * fRec10[1]))));
			fVec0[1] = fVec0[0];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fVec1[1] = fVec1[0];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fVec2[1] = fVec2[0];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fVec3[1] = fVec3[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
		}
	}

};

#endif
