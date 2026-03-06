/* ------------------------------------------------------------
author: "SpiralCloudOmega"
license: "MIT"
name: "Vocoder"
Code generated with Faust 2.70.3 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -cn Vocoder -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __Vocoder_H__
#define  __Vocoder_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#include <faust/dsp/dsp.h>
#include <faust/gui/UI.h>
#include <faust/gui/meta.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Vocoder
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

static float Vocoder_faustpower2_f(float value) {
	return value * value;
}
static float Vocoder_faustpower3_f(float value) {
	return value * value * value;
}
static float Vocoder_faustpower4_f(float value) {
	return value * value * value * value;
}

class Vocoder : public dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	float fConst0;
	float fConst5;
	float fConst17;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst25;
	float fConst28;
	float fConst31;
	float fConst32;
	float fConst34;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec2[2];
	float fRec4[2];
	float fRec1[5];
	float fConst36;
	float fRec0[5];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fRec9[5];
	float fRec8[5];
	float fConst37;
	float fRec7[2];
	float fRec6[2];
	FAUSTFLOAT fHslider5;
	float fConst38;
	float fConst49;
	float fConst52;
	float fConst53;
	float fConst54;
	float fConst56;
	float fConst58;
	float fConst60;
	float fConst61;
	float fConst63;
	float fRec11[5];
	float fConst65;
	float fRec10[5];
	float fRec15[5];
	float fRec14[5];
	float fConst66;
	float fRec13[2];
	float fRec12[2];
	FAUSTFLOAT fHslider6;
	float fConst67;
	float fConst78;
	float fConst81;
	float fConst82;
	float fConst83;
	float fConst85;
	float fConst87;
	float fConst89;
	float fConst90;
	float fConst92;
	float fRec17[5];
	float fConst94;
	float fRec16[5];
	float fRec21[5];
	float fRec20[5];
	float fConst95;
	float fRec19[2];
	float fRec18[2];
	FAUSTFLOAT fHslider7;
	float fConst96;
	float fConst107;
	float fConst110;
	float fConst111;
	float fConst112;
	float fConst114;
	float fConst116;
	float fConst118;
	float fConst119;
	float fConst121;
	float fRec23[5];
	float fConst123;
	float fRec22[5];
	float fRec27[5];
	float fRec26[5];
	float fConst124;
	float fRec25[2];
	float fRec24[2];
	FAUSTFLOAT fHslider8;
	float fConst125;
	float fConst136;
	float fConst139;
	float fConst140;
	float fConst141;
	float fConst143;
	float fConst145;
	float fConst147;
	float fConst148;
	float fConst150;
	float fRec29[5];
	float fConst152;
	float fRec28[5];
	float fRec33[5];
	float fRec32[5];
	float fConst153;
	float fRec31[2];
	float fRec30[2];
	FAUSTFLOAT fHslider9;
	float fConst154;
	float fConst165;
	float fConst168;
	float fConst169;
	float fConst170;
	float fConst172;
	float fConst174;
	float fConst176;
	float fConst177;
	float fConst179;
	float fRec35[5];
	float fConst181;
	float fRec34[5];
	float fRec39[5];
	float fRec38[5];
	float fConst182;
	float fRec37[2];
	float fRec36[2];
	FAUSTFLOAT fHslider10;
	float fConst183;
	float fConst194;
	float fConst197;
	float fConst198;
	float fConst199;
	float fConst201;
	float fConst203;
	float fConst205;
	float fConst206;
	float fConst208;
	float fRec41[5];
	float fConst210;
	float fRec40[5];
	float fRec45[5];
	float fRec44[5];
	float fConst211;
	float fRec43[2];
	float fRec42[2];
	FAUSTFLOAT fHslider11;
	float fConst212;
	float fConst223;
	float fConst226;
	float fConst227;
	float fConst228;
	float fConst230;
	float fConst232;
	float fConst234;
	float fConst235;
	float fConst237;
	float fRec47[5];
	float fConst239;
	float fRec46[5];
	float fRec51[5];
	float fRec50[5];
	float fConst240;
	float fRec49[2];
	float fRec48[2];
	FAUSTFLOAT fHslider12;
	float fConst241;
	float fConst252;
	float fConst255;
	float fConst256;
	float fConst257;
	float fConst259;
	float fConst261;
	float fConst263;
	float fConst264;
	float fConst266;
	float fRec53[5];
	float fConst268;
	float fRec52[5];
	float fRec57[5];
	float fRec56[5];
	float fConst269;
	float fRec55[2];
	float fRec54[2];
	FAUSTFLOAT fHslider13;
	float fConst270;
	float fConst281;
	float fConst284;
	float fConst285;
	float fConst286;
	float fConst288;
	float fConst290;
	float fConst292;
	float fConst293;
	float fConst295;
	float fRec59[5];
	float fConst297;
	float fRec58[5];
	float fRec63[5];
	float fRec62[5];
	float fConst298;
	float fRec61[2];
	float fRec60[2];
	FAUSTFLOAT fHslider14;
	float fConst299;
	float fConst310;
	float fConst313;
	float fConst314;
	float fConst315;
	float fConst317;
	float fConst319;
	float fConst321;
	float fConst322;
	float fConst324;
	float fRec65[5];
	float fConst326;
	float fRec64[5];
	float fRec69[5];
	float fRec68[5];
	float fConst327;
	float fRec67[2];
	float fRec66[2];
	FAUSTFLOAT fHslider15;
	float fConst328;
	float fConst339;
	float fConst342;
	float fConst343;
	float fConst344;
	float fConst346;
	float fConst348;
	float fConst350;
	float fConst351;
	float fConst353;
	float fRec71[5];
	float fConst355;
	float fRec70[5];
	float fRec75[5];
	float fRec74[5];
	float fConst356;
	float fRec73[2];
	float fRec72[2];
	FAUSTFLOAT fHslider16;
	float fConst357;
	float fConst368;
	float fConst371;
	float fConst372;
	float fConst373;
	float fConst375;
	float fConst377;
	float fConst379;
	float fConst380;
	float fConst382;
	float fRec77[5];
	float fConst384;
	float fRec76[5];
	float fRec81[5];
	float fRec80[5];
	float fConst385;
	float fRec79[2];
	float fRec78[2];
	FAUSTFLOAT fHslider17;
	float fConst386;
	float fConst397;
	float fConst400;
	float fConst401;
	float fConst402;
	float fConst404;
	float fConst406;
	float fConst408;
	float fConst409;
	float fConst411;
	float fRec83[5];
	float fConst413;
	float fRec82[5];
	float fRec87[5];
	float fRec86[5];
	float fConst414;
	float fRec85[2];
	float fRec84[2];
	FAUSTFLOAT fHslider18;
	float fConst415;
	float fConst426;
	float fConst429;
	float fConst430;
	float fConst431;
	float fConst433;
	float fConst435;
	float fConst437;
	float fConst438;
	float fConst440;
	float fRec89[5];
	float fConst442;
	float fRec88[5];
	float fRec93[5];
	float fRec92[5];
	float fConst443;
	float fRec91[2];
	float fRec90[2];
	FAUSTFLOAT fHslider19;
	float fConst444;
	float fConst455;
	float fConst458;
	float fConst459;
	float fConst460;
	float fConst462;
	float fConst464;
	float fConst466;
	float fConst467;
	float fConst469;
	float fRec95[5];
	float fConst471;
	float fRec94[5];
	float fRec99[5];
	float fRec98[5];
	float fConst472;
	float fRec97[2];
	float fRec96[2];
	FAUSTFLOAT fHslider20;
	float fConst473;
	FAUSTFLOAT fHslider21;
	FAUSTFLOAT fHslider22;
	float fConst474;
	float fRec101[5];
	float fRec100[5];
	float fRec105[5];
	float fRec104[5];
	float fRec103[2];
	float fRec102[2];
	float fRec107[5];
	float fRec106[5];
	float fRec111[5];
	float fRec110[5];
	float fRec109[2];
	float fRec108[2];
	float fRec113[5];
	float fRec112[5];
	float fRec117[5];
	float fRec116[5];
	float fRec115[2];
	float fRec114[2];
	float fRec119[5];
	float fRec118[5];
	float fRec123[5];
	float fRec122[5];
	float fRec121[2];
	float fRec120[2];
	float fRec125[5];
	float fRec124[5];
	float fRec129[5];
	float fRec128[5];
	float fRec127[2];
	float fRec126[2];
	float fRec131[5];
	float fRec130[5];
	float fRec135[5];
	float fRec134[5];
	float fRec133[2];
	float fRec132[2];
	float fRec137[5];
	float fRec136[5];
	float fRec141[5];
	float fRec140[5];
	float fRec139[2];
	float fRec138[2];
	float fRec143[5];
	float fRec142[5];
	float fRec147[5];
	float fRec146[5];
	float fRec145[2];
	float fRec144[2];
	float fRec149[5];
	float fRec148[5];
	float fRec153[5];
	float fRec152[5];
	float fRec151[2];
	float fRec150[2];
	float fRec155[5];
	float fRec154[5];
	float fRec159[5];
	float fRec158[5];
	float fRec157[2];
	float fRec156[2];
	float fRec161[5];
	float fRec160[5];
	float fRec165[5];
	float fRec164[5];
	float fRec163[2];
	float fRec162[2];
	float fRec167[5];
	float fRec166[5];
	float fRec171[5];
	float fRec170[5];
	float fRec169[2];
	float fRec168[2];
	float fRec173[5];
	float fRec172[5];
	float fRec177[5];
	float fRec176[5];
	float fRec175[2];
	float fRec174[2];
	float fRec179[5];
	float fRec178[5];
	float fRec183[5];
	float fRec182[5];
	float fRec181[2];
	float fRec180[2];
	float fRec185[5];
	float fRec184[5];
	float fRec189[5];
	float fRec188[5];
	float fRec187[2];
	float fRec186[2];
	float fRec191[5];
	float fRec190[5];
	float fRec195[5];
	float fRec194[5];
	float fRec193[2];
	float fRec192[2];
	
 public:
	Vocoder() {}

	void metadata(Meta* m) { 
		m->declare("author", "SpiralCloudOmega");
		m->declare("basics.lib/bypass2:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.12.0");
		m->declare("compile_options", "-lang cpp -ct 1 -cn Vocoder -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("description", "16-band channel vocoder with internal saw carrier and band levels");
		m->declare("filename", "vocoder.dsp");
		m->declare("filters.lib/bandpass0_bandstop1:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass0_bandstop1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass0_bandstop1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("license", "MIT");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.7.0");
		m->declare("name", "Vocoder");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/saw2ptr:author", "Julius O. Smith III");
		m->declare("oscillators.lib/saw2ptr:license", "STK-4.3");
		m->declare("oscillators.lib/version", "1.5.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.5.0");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::tan(51163.082f / fConst0);
		float fConst2 = Vocoder_faustpower2_f(fConst0);
		float fConst3 = std::sqrt(4.0f * fConst2 * std::tan(24235.143f / fConst0) * fConst1);
		float fConst4 = Vocoder_faustpower2_f(fConst3);
		fConst5 = 1.0f / fConst0;
		float fConst6 = Vocoder_faustpower3_f(fConst5);
		float fConst7 = fConst6 * fConst4;
		float fConst8 = 6.122935f / fConst0;
		float fConst9 = fConst0 * fConst1;
		float fConst10 = 2.0f * fConst9 - 0.5f * (fConst4 / fConst9);
		float fConst11 = fConst10 * (fConst8 + 1.5307337f * fConst7);
		float fConst12 = Vocoder_faustpower2_f(fConst10);
		float fConst13 = Vocoder_faustpower2_f(fConst5);
		float fConst14 = Vocoder_faustpower4_f(fConst5);
		float fConst15 = fConst14 * Vocoder_faustpower4_f(fConst3);
		float fConst16 = fConst15 + fConst13 * (4.0f * fConst12 + 8.0f * fConst4);
		fConst17 = fConst16 + (16.0f - fConst11);
		float fConst18 = 3.0614674f * fConst7;
		float fConst19 = 12.24587f / fConst0;
		float fConst20 = 4.0f * fConst15;
		fConst21 = fConst20 + fConst10 * (fConst19 - fConst18) + -64.0f;
		fConst22 = 6.0f * fConst15 + (96.0f - fConst13 * (8.0f * fConst12 + 16.0f * fConst4));
		fConst23 = fConst20 + fConst10 * (fConst18 - fConst19) + -64.0f;
		float fConst24 = fConst16 + fConst11 + 16.0f;
		fConst25 = 1.0f / fConst24;
		float fConst26 = 14.782072f / fConst0;
		float fConst27 = fConst10 * (fConst26 + 3.695518f * fConst7);
		fConst28 = fConst16 + (16.0f - fConst27);
		float fConst29 = 7.391036f * fConst7;
		float fConst30 = 29.564144f / fConst0;
		fConst31 = fConst20 + fConst10 * (fConst30 - fConst29) + -64.0f;
		fConst32 = fConst20 + fConst10 * (fConst29 - fConst30) + -64.0f;
		float fConst33 = fConst27 + fConst16 + 16.0f;
		fConst34 = 1.0f / fConst33;
		float fConst35 = fConst13 * fConst12;
		fConst36 = fConst35 / fConst33;
		fConst37 = fConst35 / fConst24;
		fConst38 = fConst12 / fConst24;
		float fConst39 = std::tan(40504.105f / fConst0);
		float fConst40 = std::sqrt(4.0f * fConst2 * std::tan(19186.154f / fConst0) * fConst39);
		float fConst41 = Vocoder_faustpower2_f(fConst40);
		float fConst42 = fConst6 * fConst41;
		float fConst43 = fConst0 * fConst39;
		float fConst44 = 2.0f * fConst43 - 0.5f * (fConst41 / fConst43);
		float fConst45 = fConst44 * (fConst8 + 1.5307337f * fConst42);
		float fConst46 = Vocoder_faustpower2_f(fConst44);
		float fConst47 = fConst14 * Vocoder_faustpower4_f(fConst40);
		float fConst48 = fConst47 + fConst13 * (4.0f * fConst46 + 8.0f * fConst41);
		fConst49 = fConst48 + (16.0f - fConst45);
		float fConst50 = 3.0614674f * fConst42;
		float fConst51 = 4.0f * fConst47;
		fConst52 = fConst51 + fConst44 * (fConst19 - fConst50) + -64.0f;
		fConst53 = 6.0f * fConst47 + (96.0f - fConst13 * (8.0f * fConst46 + 16.0f * fConst41));
		fConst54 = fConst51 + fConst44 * (fConst50 - fConst19) + -64.0f;
		float fConst55 = fConst48 + fConst45 + 16.0f;
		fConst56 = 1.0f / fConst55;
		float fConst57 = fConst44 * (fConst26 + 3.695518f * fConst42);
		fConst58 = fConst48 + (16.0f - fConst57);
		float fConst59 = 7.391036f * fConst42;
		fConst60 = fConst51 + fConst44 * (fConst30 - fConst59) + -64.0f;
		fConst61 = fConst51 + fConst44 * (fConst59 - fConst30) + -64.0f;
		float fConst62 = fConst57 + fConst48 + 16.0f;
		fConst63 = 1.0f / fConst62;
		float fConst64 = fConst13 * fConst46;
		fConst65 = fConst64 / fConst62;
		fConst66 = fConst64 / fConst55;
		fConst67 = fConst46 / fConst55;
		float fConst68 = std::tan(31976.926f / fConst0);
		float fConst69 = std::sqrt(4.0f * fConst2 * std::tan(15146.965f / fConst0) * fConst68);
		float fConst70 = Vocoder_faustpower2_f(fConst69);
		float fConst71 = fConst6 * fConst70;
		float fConst72 = fConst0 * fConst68;
		float fConst73 = 2.0f * fConst72 - 0.5f * (fConst70 / fConst72);
		float fConst74 = fConst73 * (fConst8 + 1.5307337f * fConst71);
		float fConst75 = Vocoder_faustpower2_f(fConst73);
		float fConst76 = fConst14 * Vocoder_faustpower4_f(fConst69);
		float fConst77 = fConst76 + fConst13 * (4.0f * fConst75 + 8.0f * fConst70);
		fConst78 = fConst77 + (16.0f - fConst74);
		float fConst79 = 3.0614674f * fConst71;
		float fConst80 = 4.0f * fConst76;
		fConst81 = fConst80 + fConst73 * (fConst19 - fConst79) + -64.0f;
		fConst82 = 6.0f * fConst76 + (96.0f - fConst13 * (8.0f * fConst75 + 16.0f * fConst70));
		fConst83 = fConst80 + fConst73 * (fConst79 - fConst19) + -64.0f;
		float fConst84 = fConst77 + fConst74 + 16.0f;
		fConst85 = 1.0f / fConst84;
		float fConst86 = fConst73 * (fConst26 + 3.695518f * fConst71);
		fConst87 = fConst77 + (16.0f - fConst86);
		float fConst88 = 7.391036f * fConst71;
		fConst89 = fConst80 + fConst73 * (fConst30 - fConst88) + -64.0f;
		fConst90 = fConst80 + fConst73 * (fConst88 - fConst30) + -64.0f;
		float fConst91 = fConst86 + fConst77 + 16.0f;
		fConst92 = 1.0f / fConst91;
		float fConst93 = fConst13 * fConst75;
		fConst94 = fConst93 / fConst91;
		fConst95 = fConst93 / fConst84;
		fConst96 = fConst75 / fConst84;
		float fConst97 = std::tan(23876.104f / fConst0);
		float fConst98 = std::sqrt(4.0f * fConst2 * std::tan(11309.733f / fConst0) * fConst97);
		float fConst99 = Vocoder_faustpower2_f(fConst98);
		float fConst100 = fConst6 * fConst99;
		float fConst101 = fConst0 * fConst97;
		float fConst102 = 2.0f * fConst101 - 0.5f * (fConst99 / fConst101);
		float fConst103 = fConst102 * (fConst8 + 1.5307337f * fConst100);
		float fConst104 = Vocoder_faustpower2_f(fConst102);
		float fConst105 = fConst14 * Vocoder_faustpower4_f(fConst98);
		float fConst106 = fConst105 + fConst13 * (4.0f * fConst104 + 8.0f * fConst99);
		fConst107 = fConst106 + (16.0f - fConst103);
		float fConst108 = 3.0614674f * fConst100;
		float fConst109 = 4.0f * fConst105;
		fConst110 = fConst109 + fConst102 * (fConst19 - fConst108) + -64.0f;
		fConst111 = 6.0f * fConst105 + (96.0f - fConst13 * (8.0f * fConst104 + 16.0f * fConst99));
		fConst112 = fConst109 + fConst102 * (fConst108 - fConst19) + -64.0f;
		float fConst113 = fConst106 + fConst103 + 16.0f;
		fConst114 = 1.0f / fConst113;
		float fConst115 = fConst102 * (fConst26 + 3.695518f * fConst100);
		fConst116 = fConst106 + (16.0f - fConst115);
		float fConst117 = 7.391036f * fConst100;
		fConst118 = fConst109 + fConst102 * (fConst30 - fConst117) + -64.0f;
		fConst119 = fConst109 + fConst102 * (fConst117 - fConst30) + -64.0f;
		float fConst120 = fConst115 + fConst106 + 16.0f;
		fConst121 = 1.0f / fConst120;
		float fConst122 = fConst13 * fConst104;
		fConst123 = fConst122 / fConst120;
		fConst124 = fConst122 / fConst113;
		fConst125 = fConst104 / fConst113;
		float fConst126 = std::tan(17054.36f / fConst0);
		float fConst127 = std::sqrt(4.0f * fConst2 * std::tan(8078.3813f / fConst0) * fConst126);
		float fConst128 = Vocoder_faustpower2_f(fConst127);
		float fConst129 = fConst6 * fConst128;
		float fConst130 = fConst0 * fConst126;
		float fConst131 = 2.0f * fConst130 - 0.5f * (fConst128 / fConst130);
		float fConst132 = fConst131 * (fConst8 + 1.5307337f * fConst129);
		float fConst133 = Vocoder_faustpower2_f(fConst131);
		float fConst134 = fConst14 * Vocoder_faustpower4_f(fConst127);
		float fConst135 = fConst134 + fConst13 * (4.0f * fConst133 + 8.0f * fConst128);
		fConst136 = fConst135 + (16.0f - fConst132);
		float fConst137 = 3.0614674f * fConst129;
		float fConst138 = 4.0f * fConst134;
		fConst139 = fConst138 + fConst131 * (fConst19 - fConst137) + -64.0f;
		fConst140 = 6.0f * fConst134 + (96.0f - fConst13 * (8.0f * fConst133 + 16.0f * fConst128));
		fConst141 = fConst138 + fConst131 * (fConst137 - fConst19) + -64.0f;
		float fConst142 = fConst135 + fConst132 + 16.0f;
		fConst143 = 1.0f / fConst142;
		float fConst144 = fConst131 * (fConst26 + 3.695518f * fConst129);
		fConst145 = fConst135 + (16.0f - fConst144);
		float fConst146 = 7.391036f * fConst129;
		fConst147 = fConst138 + fConst131 * (fConst30 - fConst146) + -64.0f;
		fConst148 = fConst138 + fConst131 * (fConst146 - fConst30) + -64.0f;
		float fConst149 = fConst144 + fConst135 + 16.0f;
		fConst150 = 1.0f / fConst149;
		float fConst151 = fConst13 * fConst133;
		fConst152 = fConst151 / fConst149;
		fConst153 = fConst151 / fConst142;
		fConst154 = fConst133 / fConst142;
		float fConst155 = std::tan(11938.052f / fConst0);
		float fConst156 = std::sqrt(4.0f * fConst2 * std::tan(5654.8667f / fConst0) * fConst155);
		float fConst157 = Vocoder_faustpower2_f(fConst156);
		float fConst158 = fConst6 * fConst157;
		float fConst159 = fConst0 * fConst155;
		float fConst160 = 2.0f * fConst159 - 0.5f * (fConst157 / fConst159);
		float fConst161 = fConst160 * (fConst8 + 1.5307337f * fConst158);
		float fConst162 = Vocoder_faustpower2_f(fConst160);
		float fConst163 = fConst14 * Vocoder_faustpower4_f(fConst156);
		float fConst164 = fConst163 + fConst13 * (4.0f * fConst162 + 8.0f * fConst157);
		fConst165 = fConst164 + (16.0f - fConst161);
		float fConst166 = 3.0614674f * fConst158;
		float fConst167 = 4.0f * fConst163;
		fConst168 = fConst167 + fConst160 * (fConst19 - fConst166) + -64.0f;
		fConst169 = 6.0f * fConst163 + (96.0f - fConst13 * (8.0f * fConst162 + 16.0f * fConst157));
		fConst170 = fConst167 + fConst160 * (fConst166 - fConst19) + -64.0f;
		float fConst171 = fConst164 + fConst161 + 16.0f;
		fConst172 = 1.0f / fConst171;
		float fConst173 = fConst160 * (fConst26 + 3.695518f * fConst158);
		fConst174 = fConst164 + (16.0f - fConst173);
		float fConst175 = 7.391036f * fConst158;
		fConst176 = fConst167 + fConst160 * (fConst30 - fConst175) + -64.0f;
		fConst177 = fConst167 + fConst160 * (fConst175 - fConst30) + -64.0f;
		float fConst178 = fConst173 + fConst164 + 16.0f;
		fConst179 = 1.0f / fConst178;
		float fConst180 = fConst13 * fConst162;
		fConst181 = fConst180 / fConst178;
		fConst182 = fConst180 / fConst171;
		fConst183 = fConst162 / fConst171;
		float fConst184 = std::tan(8527.18f / fConst0);
		float fConst185 = std::sqrt(4.0f * fConst2 * std::tan(4039.1907f / fConst0) * fConst184);
		float fConst186 = Vocoder_faustpower2_f(fConst185);
		float fConst187 = fConst6 * fConst186;
		float fConst188 = fConst0 * fConst184;
		float fConst189 = 2.0f * fConst188 - 0.5f * (fConst186 / fConst188);
		float fConst190 = fConst189 * (fConst8 + 1.5307337f * fConst187);
		float fConst191 = Vocoder_faustpower2_f(fConst189);
		float fConst192 = fConst14 * Vocoder_faustpower4_f(fConst185);
		float fConst193 = fConst192 + fConst13 * (4.0f * fConst191 + 8.0f * fConst186);
		fConst194 = fConst193 + (16.0f - fConst190);
		float fConst195 = 3.0614674f * fConst187;
		float fConst196 = 4.0f * fConst192;
		fConst197 = fConst196 + fConst189 * (fConst19 - fConst195) + -64.0f;
		fConst198 = 6.0f * fConst192 + (96.0f - fConst13 * (8.0f * fConst191 + 16.0f * fConst186));
		fConst199 = fConst196 + fConst189 * (fConst195 - fConst19) + -64.0f;
		float fConst200 = fConst193 + fConst190 + 16.0f;
		fConst201 = 1.0f / fConst200;
		float fConst202 = fConst189 * (fConst26 + 3.695518f * fConst187);
		fConst203 = fConst193 + (16.0f - fConst202);
		float fConst204 = 7.391036f * fConst187;
		fConst205 = fConst196 + fConst189 * (fConst30 - fConst204) + -64.0f;
		fConst206 = fConst196 + fConst189 * (fConst204 - fConst30) + -64.0f;
		float fConst207 = fConst202 + fConst193 + 16.0f;
		fConst208 = 1.0f / fConst207;
		float fConst209 = fConst13 * fConst191;
		fConst210 = fConst209 / fConst207;
		fConst211 = fConst209 / fConst200;
		fConst212 = fConst191 / fConst200;
		float fConst213 = std::tan(5969.026f / fConst0);
		float fConst214 = std::sqrt(4.0f * fConst2 * std::tan(2827.4333f / fConst0) * fConst213);
		float fConst215 = Vocoder_faustpower2_f(fConst214);
		float fConst216 = fConst6 * fConst215;
		float fConst217 = fConst0 * fConst213;
		float fConst218 = 2.0f * fConst217 - 0.5f * (fConst215 / fConst217);
		float fConst219 = fConst218 * (fConst8 + 1.5307337f * fConst216);
		float fConst220 = Vocoder_faustpower2_f(fConst218);
		float fConst221 = fConst14 * Vocoder_faustpower4_f(fConst214);
		float fConst222 = fConst221 + fConst13 * (4.0f * fConst220 + 8.0f * fConst215);
		fConst223 = fConst222 + (16.0f - fConst219);
		float fConst224 = 3.0614674f * fConst216;
		float fConst225 = 4.0f * fConst221;
		fConst226 = fConst225 + fConst218 * (fConst19 - fConst224) + -64.0f;
		fConst227 = 6.0f * fConst221 + (96.0f - fConst13 * (8.0f * fConst220 + 16.0f * fConst215));
		fConst228 = fConst225 + fConst218 * (fConst224 - fConst19) + -64.0f;
		float fConst229 = fConst222 + fConst219 + 16.0f;
		fConst230 = 1.0f / fConst229;
		float fConst231 = fConst218 * (fConst26 + 3.695518f * fConst216);
		fConst232 = fConst222 + (16.0f - fConst231);
		float fConst233 = 7.391036f * fConst216;
		fConst234 = fConst225 + fConst218 * (fConst30 - fConst233) + -64.0f;
		fConst235 = fConst225 + fConst218 * (fConst233 - fConst30) + -64.0f;
		float fConst236 = fConst231 + fConst222 + 16.0f;
		fConst237 = 1.0f / fConst236;
		float fConst238 = fConst13 * fConst220;
		fConst239 = fConst238 / fConst236;
		fConst240 = fConst238 / fConst229;
		fConst241 = fConst220 / fConst229;
		float fConst242 = std::tan(4263.59f / fConst0);
		float fConst243 = std::sqrt(4.0f * fConst2 * std::tan(2019.5953f / fConst0) * fConst242);
		float fConst244 = Vocoder_faustpower2_f(fConst243);
		float fConst245 = fConst6 * fConst244;
		float fConst246 = fConst0 * fConst242;
		float fConst247 = 2.0f * fConst246 - 0.5f * (fConst244 / fConst246);
		float fConst248 = fConst247 * (fConst8 + 1.5307337f * fConst245);
		float fConst249 = Vocoder_faustpower2_f(fConst247);
		float fConst250 = fConst14 * Vocoder_faustpower4_f(fConst243);
		float fConst251 = fConst250 + fConst13 * (4.0f * fConst249 + 8.0f * fConst244);
		fConst252 = fConst251 + (16.0f - fConst248);
		float fConst253 = 3.0614674f * fConst245;
		float fConst254 = 4.0f * fConst250;
		fConst255 = fConst254 + fConst247 * (fConst19 - fConst253) + -64.0f;
		fConst256 = 6.0f * fConst250 + (96.0f - fConst13 * (8.0f * fConst249 + 16.0f * fConst244));
		fConst257 = fConst254 + fConst247 * (fConst253 - fConst19) + -64.0f;
		float fConst258 = fConst251 + fConst248 + 16.0f;
		fConst259 = 1.0f / fConst258;
		float fConst260 = fConst247 * (fConst26 + 3.695518f * fConst245);
		fConst261 = fConst251 + (16.0f - fConst260);
		float fConst262 = 7.391036f * fConst245;
		fConst263 = fConst254 + fConst247 * (fConst30 - fConst262) + -64.0f;
		fConst264 = fConst254 + fConst247 * (fConst262 - fConst30) + -64.0f;
		float fConst265 = fConst260 + fConst251 + 16.0f;
		fConst266 = 1.0f / fConst265;
		float fConst267 = fConst13 * fConst249;
		fConst268 = fConst267 / fConst265;
		fConst269 = fConst267 / fConst258;
		fConst270 = fConst249 / fConst258;
		float fConst271 = std::tan(3069.785f / fConst0);
		float fConst272 = std::sqrt(4.0f * fConst2 * std::tan(1454.1086f / fConst0) * fConst271);
		float fConst273 = Vocoder_faustpower2_f(fConst272);
		float fConst274 = fConst6 * fConst273;
		float fConst275 = fConst0 * fConst271;
		float fConst276 = 2.0f * fConst275 - 0.5f * (fConst273 / fConst275);
		float fConst277 = fConst276 * (fConst8 + 1.5307337f * fConst274);
		float fConst278 = Vocoder_faustpower2_f(fConst276);
		float fConst279 = fConst14 * Vocoder_faustpower4_f(fConst272);
		float fConst280 = fConst279 + fConst13 * (4.0f * fConst278 + 8.0f * fConst273);
		fConst281 = fConst280 + (16.0f - fConst277);
		float fConst282 = 3.0614674f * fConst274;
		float fConst283 = 4.0f * fConst279;
		fConst284 = fConst283 + fConst276 * (fConst19 - fConst282) + -64.0f;
		fConst285 = 6.0f * fConst279 + (96.0f - fConst13 * (8.0f * fConst278 + 16.0f * fConst273));
		fConst286 = fConst283 + fConst276 * (fConst282 - fConst19) + -64.0f;
		float fConst287 = fConst280 + fConst277 + 16.0f;
		fConst288 = 1.0f / fConst287;
		float fConst289 = fConst276 * (fConst26 + 3.695518f * fConst274);
		fConst290 = fConst280 + (16.0f - fConst289);
		float fConst291 = 7.391036f * fConst274;
		fConst292 = fConst283 + fConst276 * (fConst30 - fConst291) + -64.0f;
		fConst293 = fConst283 + fConst276 * (fConst291 - fConst30) + -64.0f;
		float fConst294 = fConst289 + fConst280 + 16.0f;
		fConst295 = 1.0f / fConst294;
		float fConst296 = fConst13 * fConst278;
		fConst297 = fConst296 / fConst294;
		fConst298 = fConst296 / fConst287;
		fConst299 = fConst278 / fConst287;
		float fConst300 = std::tan(2174.431f / fConst0);
		float fConst301 = std::sqrt(4.0f * fConst2 * std::tan(1029.9937f / fConst0) * fConst300);
		float fConst302 = Vocoder_faustpower2_f(fConst301);
		float fConst303 = fConst6 * fConst302;
		float fConst304 = fConst0 * fConst300;
		float fConst305 = 2.0f * fConst304 - 0.5f * (fConst302 / fConst304);
		float fConst306 = fConst305 * (fConst8 + 1.5307337f * fConst303);
		float fConst307 = Vocoder_faustpower2_f(fConst305);
		float fConst308 = fConst14 * Vocoder_faustpower4_f(fConst301);
		float fConst309 = fConst308 + fConst13 * (4.0f * fConst307 + 8.0f * fConst302);
		fConst310 = fConst309 + (16.0f - fConst306);
		float fConst311 = 3.0614674f * fConst303;
		float fConst312 = 4.0f * fConst308;
		fConst313 = fConst312 + fConst305 * (fConst19 - fConst311) + -64.0f;
		fConst314 = 6.0f * fConst308 + (96.0f - fConst13 * (8.0f * fConst307 + 16.0f * fConst302));
		fConst315 = fConst312 + fConst305 * (fConst311 - fConst19) + -64.0f;
		float fConst316 = fConst309 + fConst306 + 16.0f;
		fConst317 = 1.0f / fConst316;
		float fConst318 = fConst305 * (fConst26 + 3.695518f * fConst303);
		fConst319 = fConst309 + (16.0f - fConst318);
		float fConst320 = 7.391036f * fConst303;
		fConst321 = fConst312 + fConst305 * (fConst30 - fConst320) + -64.0f;
		fConst322 = fConst312 + fConst305 * (fConst320 - fConst30) + -64.0f;
		float fConst323 = fConst318 + fConst309 + 16.0f;
		fConst324 = 1.0f / fConst323;
		float fConst325 = fConst13 * fConst307;
		fConst326 = fConst325 / fConst323;
		fConst327 = fConst325 / fConst316;
		fConst328 = fConst307 / fConst316;
		float fConst329 = std::tan(1534.8925f / fConst0);
		float fConst330 = std::sqrt(4.0f * fConst2 * std::tan(727.0543f / fConst0) * fConst329);
		float fConst331 = Vocoder_faustpower2_f(fConst330);
		float fConst332 = fConst6 * fConst331;
		float fConst333 = fConst0 * fConst329;
		float fConst334 = 2.0f * fConst333 - 0.5f * (fConst331 / fConst333);
		float fConst335 = fConst334 * (fConst8 + 1.5307337f * fConst332);
		float fConst336 = Vocoder_faustpower2_f(fConst334);
		float fConst337 = fConst14 * Vocoder_faustpower4_f(fConst330);
		float fConst338 = fConst337 + fConst13 * (4.0f * fConst336 + 8.0f * fConst331);
		fConst339 = fConst338 + (16.0f - fConst335);
		float fConst340 = 3.0614674f * fConst332;
		float fConst341 = 4.0f * fConst337;
		fConst342 = fConst341 + fConst334 * (fConst19 - fConst340) + -64.0f;
		fConst343 = 6.0f * fConst337 + (96.0f - fConst13 * (8.0f * fConst336 + 16.0f * fConst331));
		fConst344 = fConst341 + fConst334 * (fConst340 - fConst19) + -64.0f;
		float fConst345 = fConst338 + fConst335 + 16.0f;
		fConst346 = 1.0f / fConst345;
		float fConst347 = fConst334 * (fConst26 + 3.695518f * fConst332);
		fConst348 = fConst338 + (16.0f - fConst347);
		float fConst349 = 7.391036f * fConst332;
		fConst350 = fConst341 + fConst334 * (fConst30 - fConst349) + -64.0f;
		fConst351 = fConst341 + fConst334 * (fConst349 - fConst30) + -64.0f;
		float fConst352 = fConst347 + fConst338 + 16.0f;
		fConst353 = 1.0f / fConst352;
		float fConst354 = fConst13 * fConst336;
		fConst355 = fConst354 / fConst352;
		fConst356 = fConst354 / fConst345;
		fConst357 = fConst336 / fConst345;
		float fConst358 = std::tan(1065.8975f / fConst0);
		float fConst359 = std::sqrt(4.0f * fConst2 * std::tan(504.89883f / fConst0) * fConst358);
		float fConst360 = Vocoder_faustpower2_f(fConst359);
		float fConst361 = fConst6 * fConst360;
		float fConst362 = fConst0 * fConst358;
		float fConst363 = 2.0f * fConst362 - 0.5f * (fConst360 / fConst362);
		float fConst364 = fConst363 * (fConst8 + 1.5307337f * fConst361);
		float fConst365 = Vocoder_faustpower2_f(fConst363);
		float fConst366 = fConst14 * Vocoder_faustpower4_f(fConst359);
		float fConst367 = fConst366 + fConst13 * (4.0f * fConst365 + 8.0f * fConst360);
		fConst368 = fConst367 + (16.0f - fConst364);
		float fConst369 = 3.0614674f * fConst361;
		float fConst370 = 4.0f * fConst366;
		fConst371 = fConst370 + fConst363 * (fConst19 - fConst369) + -64.0f;
		fConst372 = 6.0f * fConst366 + (96.0f - fConst13 * (8.0f * fConst365 + 16.0f * fConst360));
		fConst373 = fConst370 + fConst363 * (fConst369 - fConst19) + -64.0f;
		float fConst374 = fConst367 + fConst364 + 16.0f;
		fConst375 = 1.0f / fConst374;
		float fConst376 = fConst363 * (fConst26 + 3.695518f * fConst361);
		fConst377 = fConst367 + (16.0f - fConst376);
		float fConst378 = 7.391036f * fConst361;
		fConst379 = fConst370 + fConst363 * (fConst30 - fConst378) + -64.0f;
		fConst380 = fConst370 + fConst363 * (fConst378 - fConst30) + -64.0f;
		float fConst381 = fConst376 + fConst367 + 16.0f;
		fConst382 = 1.0f / fConst381;
		float fConst383 = fConst13 * fConst365;
		fConst384 = fConst383 / fConst381;
		fConst385 = fConst383 / fConst374;
		fConst386 = fConst365 / fConst374;
		float fConst387 = std::tan(746.12823f / fConst0);
		float fConst388 = std::sqrt(4.0f * fConst2 * std::tan(353.42917f / fConst0) * fConst387);
		float fConst389 = Vocoder_faustpower2_f(fConst388);
		float fConst390 = fConst6 * fConst389;
		float fConst391 = fConst0 * fConst387;
		float fConst392 = 2.0f * fConst391 - 0.5f * (fConst389 / fConst391);
		float fConst393 = fConst392 * (fConst8 + 1.5307337f * fConst390);
		float fConst394 = Vocoder_faustpower2_f(fConst392);
		float fConst395 = fConst14 * Vocoder_faustpower4_f(fConst388);
		float fConst396 = fConst395 + fConst13 * (4.0f * fConst394 + 8.0f * fConst389);
		fConst397 = fConst396 + (16.0f - fConst393);
		float fConst398 = 3.0614674f * fConst390;
		float fConst399 = 4.0f * fConst395;
		fConst400 = fConst399 + fConst392 * (fConst19 - fConst398) + -64.0f;
		fConst401 = 6.0f * fConst395 + (96.0f - fConst13 * (8.0f * fConst394 + 16.0f * fConst389));
		fConst402 = fConst399 + fConst392 * (fConst398 - fConst19) + -64.0f;
		float fConst403 = fConst396 + fConst393 + 16.0f;
		fConst404 = 1.0f / fConst403;
		float fConst405 = fConst392 * (fConst26 + 3.695518f * fConst390);
		fConst406 = fConst396 + (16.0f - fConst405);
		float fConst407 = 7.391036f * fConst390;
		fConst408 = fConst399 + fConst392 * (fConst30 - fConst407) + -64.0f;
		fConst409 = fConst399 + fConst392 * (fConst407 - fConst30) + -64.0f;
		float fConst410 = fConst405 + fConst396 + 16.0f;
		fConst411 = 1.0f / fConst410;
		float fConst412 = fConst13 * fConst394;
		fConst413 = fConst412 / fConst410;
		fConst414 = fConst412 / fConst403;
		fConst415 = fConst394 / fConst403;
		float fConst416 = std::tan(511.6308f / fConst0);
		float fConst417 = std::sqrt(4.0f * fConst2 * std::tan(242.35144f / fConst0) * fConst416);
		float fConst418 = Vocoder_faustpower2_f(fConst417);
		float fConst419 = fConst6 * fConst418;
		float fConst420 = fConst0 * fConst416;
		float fConst421 = 2.0f * fConst420 - 0.5f * (fConst418 / fConst420);
		float fConst422 = fConst421 * (fConst8 + 1.5307337f * fConst419);
		float fConst423 = Vocoder_faustpower2_f(fConst421);
		float fConst424 = fConst14 * Vocoder_faustpower4_f(fConst417);
		float fConst425 = fConst424 + fConst13 * (4.0f * fConst423 + 8.0f * fConst418);
		fConst426 = fConst425 + (16.0f - fConst422);
		float fConst427 = 3.0614674f * fConst419;
		float fConst428 = 4.0f * fConst424;
		fConst429 = fConst428 + fConst421 * (fConst19 - fConst427) + -64.0f;
		fConst430 = 6.0f * fConst424 + (96.0f - fConst13 * (8.0f * fConst423 + 16.0f * fConst418));
		fConst431 = fConst428 + fConst421 * (fConst427 - fConst19) + -64.0f;
		float fConst432 = fConst425 + fConst422 + 16.0f;
		fConst433 = 1.0f / fConst432;
		float fConst434 = fConst421 * (fConst26 + 3.695518f * fConst419);
		fConst435 = fConst425 + (16.0f - fConst434);
		float fConst436 = 7.391036f * fConst419;
		fConst437 = fConst428 + fConst421 * (fConst30 - fConst436) + -64.0f;
		fConst438 = fConst428 + fConst421 * (fConst436 - fConst30) + -64.0f;
		float fConst439 = fConst434 + fConst425 + 16.0f;
		fConst440 = 1.0f / fConst439;
		float fConst441 = fConst13 * fConst423;
		fConst442 = fConst441 / fConst439;
		fConst443 = fConst441 / fConst432;
		fConst444 = fConst423 / fConst432;
		float fConst445 = std::tan(341.0872f / fConst0);
		float fConst446 = std::sqrt(4.0f * fConst2 * std::tan(161.56763f / fConst0) * fConst445);
		float fConst447 = Vocoder_faustpower2_f(fConst446);
		float fConst448 = fConst6 * fConst447;
		float fConst449 = fConst0 * fConst445;
		float fConst450 = 2.0f * fConst449 - 0.5f * (fConst447 / fConst449);
		float fConst451 = fConst450 * (fConst8 + 1.5307337f * fConst448);
		float fConst452 = Vocoder_faustpower2_f(fConst450);
		float fConst453 = fConst14 * Vocoder_faustpower4_f(fConst446);
		float fConst454 = fConst453 + fConst13 * (4.0f * fConst452 + 8.0f * fConst447);
		fConst455 = fConst454 + (16.0f - fConst451);
		float fConst456 = 3.0614674f * fConst448;
		float fConst457 = 4.0f * fConst453;
		fConst458 = fConst457 + fConst450 * (fConst19 - fConst456) + -64.0f;
		fConst459 = 6.0f * fConst453 + (96.0f - fConst13 * (8.0f * fConst452 + 16.0f * fConst447));
		fConst460 = fConst457 + fConst450 * (fConst456 - fConst19) + -64.0f;
		float fConst461 = fConst454 + fConst451 + 16.0f;
		fConst462 = 1.0f / fConst461;
		float fConst463 = fConst450 * (fConst26 + 3.695518f * fConst448);
		fConst464 = fConst454 + (16.0f - fConst463);
		float fConst465 = 7.391036f * fConst448;
		fConst466 = fConst457 + fConst450 * (fConst30 - fConst465) + -64.0f;
		fConst467 = fConst457 + fConst450 * (fConst465 - fConst30) + -64.0f;
		float fConst468 = fConst463 + fConst454 + 16.0f;
		fConst469 = 1.0f / fConst468;
		float fConst470 = fConst13 * fConst452;
		fConst471 = fConst470 / fConst468;
		fConst472 = fConst470 / fConst461;
		fConst473 = fConst452 / fConst461;
		fConst474 = 0.01f * fConst13;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(1.1e+02f);
		fHslider3 = FAUSTFLOAT(5.0f);
		fHslider4 = FAUSTFLOAT(5e+01f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
		fHslider10 = FAUSTFLOAT(0.0f);
		fHslider11 = FAUSTFLOAT(0.0f);
		fHslider12 = FAUSTFLOAT(0.0f);
		fHslider13 = FAUSTFLOAT(0.0f);
		fHslider14 = FAUSTFLOAT(0.0f);
		fHslider15 = FAUSTFLOAT(0.0f);
		fHslider16 = FAUSTFLOAT(0.0f);
		fHslider17 = FAUSTFLOAT(0.0f);
		fHslider18 = FAUSTFLOAT(0.0f);
		fHslider19 = FAUSTFLOAT(0.0f);
		fHslider20 = FAUSTFLOAT(0.0f);
		fHslider21 = FAUSTFLOAT(0.0f);
		fHslider22 = FAUSTFLOAT(1e+02f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec2[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec4[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 5; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 5; l3 = l3 + 1) {
			fRec0[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 5; l4 = l4 + 1) {
			fRec9[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 5; l5 = l5 + 1) {
			fRec8[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec7[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 5; l8 = l8 + 1) {
			fRec11[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 5; l9 = l9 + 1) {
			fRec10[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 5; l10 = l10 + 1) {
			fRec15[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 5; l11 = l11 + 1) {
			fRec14[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec13[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 5; l14 = l14 + 1) {
			fRec17[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 5; l15 = l15 + 1) {
			fRec16[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 5; l16 = l16 + 1) {
			fRec21[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 5; l17 = l17 + 1) {
			fRec20[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec19[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 5; l20 = l20 + 1) {
			fRec23[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 5; l21 = l21 + 1) {
			fRec22[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 5; l22 = l22 + 1) {
			fRec27[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 5; l23 = l23 + 1) {
			fRec26[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec25[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec24[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 5; l26 = l26 + 1) {
			fRec29[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 5; l27 = l27 + 1) {
			fRec28[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 5; l28 = l28 + 1) {
			fRec33[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 5; l29 = l29 + 1) {
			fRec32[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec31[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec30[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 5; l32 = l32 + 1) {
			fRec35[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 5; l33 = l33 + 1) {
			fRec34[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 5; l34 = l34 + 1) {
			fRec39[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 5; l35 = l35 + 1) {
			fRec38[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec37[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec36[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 5; l38 = l38 + 1) {
			fRec41[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 5; l39 = l39 + 1) {
			fRec40[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 5; l40 = l40 + 1) {
			fRec45[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 5; l41 = l41 + 1) {
			fRec44[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec43[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec42[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 5; l44 = l44 + 1) {
			fRec47[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 5; l45 = l45 + 1) {
			fRec46[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 5; l46 = l46 + 1) {
			fRec51[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 5; l47 = l47 + 1) {
			fRec50[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec49[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec48[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 5; l50 = l50 + 1) {
			fRec53[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 5; l51 = l51 + 1) {
			fRec52[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 5; l52 = l52 + 1) {
			fRec57[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 5; l53 = l53 + 1) {
			fRec56[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec55[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec54[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 5; l56 = l56 + 1) {
			fRec59[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 5; l57 = l57 + 1) {
			fRec58[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 5; l58 = l58 + 1) {
			fRec63[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 5; l59 = l59 + 1) {
			fRec62[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec61[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec60[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 5; l62 = l62 + 1) {
			fRec65[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 5; l63 = l63 + 1) {
			fRec64[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 5; l64 = l64 + 1) {
			fRec69[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 5; l65 = l65 + 1) {
			fRec68[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec67[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec66[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 5; l68 = l68 + 1) {
			fRec71[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 5; l69 = l69 + 1) {
			fRec70[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 5; l70 = l70 + 1) {
			fRec75[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 5; l71 = l71 + 1) {
			fRec74[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec73[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec72[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 5; l74 = l74 + 1) {
			fRec77[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 5; l75 = l75 + 1) {
			fRec76[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 5; l76 = l76 + 1) {
			fRec81[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 5; l77 = l77 + 1) {
			fRec80[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec79[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec78[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 5; l80 = l80 + 1) {
			fRec83[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 5; l81 = l81 + 1) {
			fRec82[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 5; l82 = l82 + 1) {
			fRec87[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 5; l83 = l83 + 1) {
			fRec86[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec85[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec84[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 5; l86 = l86 + 1) {
			fRec89[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 5; l87 = l87 + 1) {
			fRec88[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 5; l88 = l88 + 1) {
			fRec93[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 5; l89 = l89 + 1) {
			fRec92[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec91[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fRec90[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 5; l92 = l92 + 1) {
			fRec95[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 5; l93 = l93 + 1) {
			fRec94[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 5; l94 = l94 + 1) {
			fRec99[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 5; l95 = l95 + 1) {
			fRec98[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec97[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec96[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 5; l98 = l98 + 1) {
			fRec101[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 5; l99 = l99 + 1) {
			fRec100[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 5; l100 = l100 + 1) {
			fRec105[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 5; l101 = l101 + 1) {
			fRec104[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fRec103[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec102[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 5; l104 = l104 + 1) {
			fRec107[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 5; l105 = l105 + 1) {
			fRec106[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 5; l106 = l106 + 1) {
			fRec111[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 5; l107 = l107 + 1) {
			fRec110[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec109[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec108[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 5; l110 = l110 + 1) {
			fRec113[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 5; l111 = l111 + 1) {
			fRec112[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 5; l112 = l112 + 1) {
			fRec117[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 5; l113 = l113 + 1) {
			fRec116[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fRec115[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec114[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 5; l116 = l116 + 1) {
			fRec119[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 5; l117 = l117 + 1) {
			fRec118[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 5; l118 = l118 + 1) {
			fRec123[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 5; l119 = l119 + 1) {
			fRec122[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec121[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec120[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 5; l122 = l122 + 1) {
			fRec125[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 5; l123 = l123 + 1) {
			fRec124[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 5; l124 = l124 + 1) {
			fRec129[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 5; l125 = l125 + 1) {
			fRec128[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec127[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec126[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 5; l128 = l128 + 1) {
			fRec131[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 5; l129 = l129 + 1) {
			fRec130[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 5; l130 = l130 + 1) {
			fRec135[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 5; l131 = l131 + 1) {
			fRec134[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec133[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec132[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 5; l134 = l134 + 1) {
			fRec137[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 5; l135 = l135 + 1) {
			fRec136[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 5; l136 = l136 + 1) {
			fRec141[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 5; l137 = l137 + 1) {
			fRec140[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 2; l138 = l138 + 1) {
			fRec139[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec138[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 5; l140 = l140 + 1) {
			fRec143[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 5; l141 = l141 + 1) {
			fRec142[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 5; l142 = l142 + 1) {
			fRec147[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 5; l143 = l143 + 1) {
			fRec146[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec145[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec144[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 5; l146 = l146 + 1) {
			fRec149[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 5; l147 = l147 + 1) {
			fRec148[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 5; l148 = l148 + 1) {
			fRec153[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 5; l149 = l149 + 1) {
			fRec152[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 2; l150 = l150 + 1) {
			fRec151[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec150[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 5; l152 = l152 + 1) {
			fRec155[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 5; l153 = l153 + 1) {
			fRec154[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 5; l154 = l154 + 1) {
			fRec159[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 5; l155 = l155 + 1) {
			fRec158[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec157[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fRec156[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 5; l158 = l158 + 1) {
			fRec161[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 5; l159 = l159 + 1) {
			fRec160[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 5; l160 = l160 + 1) {
			fRec165[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 5; l161 = l161 + 1) {
			fRec164[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fRec163[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec162[l163] = 0.0f;
		}
		for (int l164 = 0; l164 < 5; l164 = l164 + 1) {
			fRec167[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 5; l165 = l165 + 1) {
			fRec166[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 5; l166 = l166 + 1) {
			fRec171[l166] = 0.0f;
		}
		for (int l167 = 0; l167 < 5; l167 = l167 + 1) {
			fRec170[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec169[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec168[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 5; l170 = l170 + 1) {
			fRec173[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 5; l171 = l171 + 1) {
			fRec172[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 5; l172 = l172 + 1) {
			fRec177[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 5; l173 = l173 + 1) {
			fRec176[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 2; l174 = l174 + 1) {
			fRec175[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 2; l175 = l175 + 1) {
			fRec174[l175] = 0.0f;
		}
		for (int l176 = 0; l176 < 5; l176 = l176 + 1) {
			fRec179[l176] = 0.0f;
		}
		for (int l177 = 0; l177 < 5; l177 = l177 + 1) {
			fRec178[l177] = 0.0f;
		}
		for (int l178 = 0; l178 < 5; l178 = l178 + 1) {
			fRec183[l178] = 0.0f;
		}
		for (int l179 = 0; l179 < 5; l179 = l179 + 1) {
			fRec182[l179] = 0.0f;
		}
		for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
			fRec181[l180] = 0.0f;
		}
		for (int l181 = 0; l181 < 2; l181 = l181 + 1) {
			fRec180[l181] = 0.0f;
		}
		for (int l182 = 0; l182 < 5; l182 = l182 + 1) {
			fRec185[l182] = 0.0f;
		}
		for (int l183 = 0; l183 < 5; l183 = l183 + 1) {
			fRec184[l183] = 0.0f;
		}
		for (int l184 = 0; l184 < 5; l184 = l184 + 1) {
			fRec189[l184] = 0.0f;
		}
		for (int l185 = 0; l185 < 5; l185 = l185 + 1) {
			fRec188[l185] = 0.0f;
		}
		for (int l186 = 0; l186 < 2; l186 = l186 + 1) {
			fRec187[l186] = 0.0f;
		}
		for (int l187 = 0; l187 < 2; l187 = l187 + 1) {
			fRec186[l187] = 0.0f;
		}
		for (int l188 = 0; l188 < 5; l188 = l188 + 1) {
			fRec191[l188] = 0.0f;
		}
		for (int l189 = 0; l189 < 5; l189 = l189 + 1) {
			fRec190[l189] = 0.0f;
		}
		for (int l190 = 0; l190 < 5; l190 = l190 + 1) {
			fRec195[l190] = 0.0f;
		}
		for (int l191 = 0; l191 < 5; l191 = l191 + 1) {
			fRec194[l191] = 0.0f;
		}
		for (int l192 = 0; l192 < 2; l192 = l192 + 1) {
			fRec193[l192] = 0.0f;
		}
		for (int l193 = 0; l193 < 2; l193 = l193 + 1) {
			fRec192[l193] = 0.0f;
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
	
	virtual Vocoder* clone() {
		return new Vocoder();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openHorizontalBox("Vocoder");
		ui_interface->openHorizontalBox("Band Levels");
		ui_interface->declare(&fHslider20, "0", "");
		ui_interface->declare(&fHslider20, "style", "knob");
		ui_interface->addHorizontalSlider("Band  0", &fHslider20, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider19, "1", "");
		ui_interface->declare(&fHslider19, "style", "knob");
		ui_interface->addHorizontalSlider("Band  1", &fHslider19, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider18, "2", "");
		ui_interface->declare(&fHslider18, "style", "knob");
		ui_interface->addHorizontalSlider("Band  2", &fHslider18, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider17, "3", "");
		ui_interface->declare(&fHslider17, "style", "knob");
		ui_interface->addHorizontalSlider("Band  3", &fHslider17, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider16, "4", "");
		ui_interface->declare(&fHslider16, "style", "knob");
		ui_interface->addHorizontalSlider("Band  4", &fHslider16, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider15, "5", "");
		ui_interface->declare(&fHslider15, "style", "knob");
		ui_interface->addHorizontalSlider("Band  5", &fHslider15, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider14, "6", "");
		ui_interface->declare(&fHslider14, "style", "knob");
		ui_interface->addHorizontalSlider("Band  6", &fHslider14, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider13, "7", "");
		ui_interface->declare(&fHslider13, "style", "knob");
		ui_interface->addHorizontalSlider("Band  7", &fHslider13, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider12, "8", "");
		ui_interface->declare(&fHslider12, "style", "knob");
		ui_interface->addHorizontalSlider("Band  8", &fHslider12, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider11, "9", "");
		ui_interface->declare(&fHslider11, "style", "knob");
		ui_interface->addHorizontalSlider("Band  9", &fHslider11, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider10, "10", "");
		ui_interface->declare(&fHslider10, "style", "knob");
		ui_interface->addHorizontalSlider("Band 10", &fHslider10, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider9, "11", "");
		ui_interface->declare(&fHslider9, "style", "knob");
		ui_interface->addHorizontalSlider("Band 11", &fHslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider8, "12", "");
		ui_interface->declare(&fHslider8, "style", "knob");
		ui_interface->addHorizontalSlider("Band 12", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider7, "13", "");
		ui_interface->declare(&fHslider7, "style", "knob");
		ui_interface->addHorizontalSlider("Band 13", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider6, "14", "");
		ui_interface->declare(&fHslider6, "style", "knob");
		ui_interface->addHorizontalSlider("Band 14", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider5, "15", "");
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->addHorizontalSlider("Band 15", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Carrier");
		ui_interface->declare(&fHslider2, "0", "");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->declare(&fHslider2, "unit", "Hz");
		ui_interface->addHorizontalSlider("Frequency", &fHslider2, FAUSTFLOAT(1.1e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+03f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider1, "1", "");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "cents");
		ui_interface->addHorizontalSlider("Fine Tune", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1e+02f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider0, "2", "");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Ext/Int", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Envelope");
		ui_interface->declare(&fHslider3, "0", "");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->declare(&fHslider3, "unit", "ms");
		ui_interface->addHorizontalSlider("Attack", &fHslider3, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider4, "1", "");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->declare(&fHslider4, "unit", "ms");
		ui_interface->addHorizontalSlider("Release", &fHslider4, FAUSTFLOAT(5e+01f), FAUSTFLOAT(5.0f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
		ui_interface->declare(&fCheckbox0, "0", "");
		ui_interface->addCheckButton("Bypass", &fCheckbox0);
		ui_interface->declare(&fHslider21, "8", "");
		ui_interface->declare(&fHslider21, "style", "knob");
		ui_interface->declare(&fHslider21, "unit", "dB");
		ui_interface->addHorizontalSlider("Output Gain", &fHslider21, FAUSTFLOAT(0.0f), FAUSTFLOAT(-2e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider22, "9", "");
		ui_interface->declare(&fHslider22, "style", "knob");
		ui_interface->addHorizontalSlider("Mix", &fHslider22, FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = 0.01f * float(fHslider0);
		float fSlow2 = float(fHslider2) * std::pow(2.0f, 0.00083333335f * float(fHslider1));
		float fSlow3 = std::max<float>(1.1920929e-07f, std::fabs(1.005f * fSlow2));
		float fSlow4 = fConst5 * fSlow3;
		float fSlow5 = 1.0f - fConst0 / fSlow3;
		float fSlow6 = std::max<float>(1.1920929e-07f, std::fabs(fSlow2));
		float fSlow7 = fConst5 * fSlow6;
		float fSlow8 = 1.0f - fConst0 / fSlow6;
		float fSlow9 = 1.0f - fSlow1;
		float fSlow10 = 0.001f * float(fHslider3);
		int iSlow11 = std::fabs(fSlow10) < 1.1920929e-07f;
		float fSlow12 = ((iSlow11) ? 0.0f : std::exp(-(fConst5 / ((iSlow11) ? 1.0f : fSlow10))));
		float fSlow13 = 1.0f - fSlow12;
		float fSlow14 = 0.001f * float(fHslider4);
		int iSlow15 = std::fabs(fSlow14) < 1.1920929e-07f;
		float fSlow16 = ((iSlow15) ? 0.0f : std::exp(-(fConst5 / ((iSlow15) ? 1.0f : fSlow14))));
		float fSlow17 = 1.0f - fSlow16;
		float fSlow18 = fConst38 * std::pow(1e+01f, 0.05f * float(fHslider5));
		float fSlow19 = fConst67 * std::pow(1e+01f, 0.05f * float(fHslider6));
		float fSlow20 = fConst96 * std::pow(1e+01f, 0.05f * float(fHslider7));
		float fSlow21 = fConst125 * std::pow(1e+01f, 0.05f * float(fHslider8));
		float fSlow22 = fConst154 * std::pow(1e+01f, 0.05f * float(fHslider9));
		float fSlow23 = fConst183 * std::pow(1e+01f, 0.05f * float(fHslider10));
		float fSlow24 = fConst212 * std::pow(1e+01f, 0.05f * float(fHslider11));
		float fSlow25 = fConst241 * std::pow(1e+01f, 0.05f * float(fHslider12));
		float fSlow26 = fConst270 * std::pow(1e+01f, 0.05f * float(fHslider13));
		float fSlow27 = fConst299 * std::pow(1e+01f, 0.05f * float(fHslider14));
		float fSlow28 = fConst328 * std::pow(1e+01f, 0.05f * float(fHslider15));
		float fSlow29 = fConst357 * std::pow(1e+01f, 0.05f * float(fHslider16));
		float fSlow30 = fConst386 * std::pow(1e+01f, 0.05f * float(fHslider17));
		float fSlow31 = fConst415 * std::pow(1e+01f, 0.05f * float(fHslider18));
		float fSlow32 = fConst444 * std::pow(1e+01f, 0.05f * float(fHslider19));
		float fSlow33 = fConst473 * std::pow(1e+01f, 0.05f * float(fHslider20));
		float fSlow34 = float(fHslider22);
		float fSlow35 = fConst474 * fSlow34 * std::pow(1e+01f, 0.05f * float(fHslider21));
		float fSlow36 = 1.0f - 0.01f * fSlow34;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = ((iSlow0) ? 0.0f : fTemp0);
			float fTemp2 = fSlow4 + fRec2[1] + -1.0f;
			int iTemp3 = fTemp2 < 0.0f;
			float fTemp4 = fSlow4 + fRec2[1];
			fRec2[0] = ((iTemp3) ? fTemp4 : fTemp2);
			float fRec3 = ((iTemp3) ? fTemp4 : fSlow4 + fRec2[1] + fSlow5 * fTemp2);
			float fTemp5 = fSlow7 + fRec4[1] + -1.0f;
			int iTemp6 = fTemp5 < 0.0f;
			float fTemp7 = fSlow7 + fRec4[1];
			fRec4[0] = ((iTemp6) ? fTemp7 : fTemp5);
			float fRec5 = ((iTemp6) ? fTemp7 : fSlow7 + fRec4[1] + fSlow8 * fTemp5);
			float fTemp8 = fSlow9 * (2.0f * fRec5 + 0.5f * (2.0f * fRec3 + -1.0f) + -1.0f);
			float fTemp9 = fTemp8 + fSlow1 * fTemp1;
			fRec1[0] = fTemp9 - fConst34 * (fConst32 * fRec1[1] + fConst22 * fRec1[2] + fConst31 * fRec1[3] + fConst28 * fRec1[4]);
			fRec0[0] = fConst36 * (4.0f * fRec1[0] - 8.0f * fRec1[2] + 4.0f * fRec1[4]) - fConst25 * (fConst23 * fRec0[1] + fConst22 * fRec0[2] + fConst21 * fRec0[3] + fConst17 * fRec0[4]);
			fRec9[0] = fTemp1 - fConst34 * (fConst32 * fRec9[1] + fConst22 * fRec9[2] + fConst31 * fRec9[3] + fConst28 * fRec9[4]);
			fRec8[0] = fConst36 * (4.0f * fRec9[0] - 8.0f * fRec9[2] + 4.0f * fRec9[4]) - fConst25 * (fConst23 * fRec8[1] + fConst22 * fRec8[2] + fConst21 * fRec8[3] + fConst17 * fRec8[4]);
			fRec7[0] = fSlow17 * std::fabs(fConst37 * (4.0f * fRec8[0] - 8.0f * fRec8[2] + 4.0f * fRec8[4])) + fSlow16 * fRec7[1];
			fRec6[0] = fRec7[0] * fSlow13 + fSlow12 * fRec6[1];
			fRec11[0] = fTemp9 - fConst63 * (fConst61 * fRec11[1] + fConst53 * fRec11[2] + fConst60 * fRec11[3] + fConst58 * fRec11[4]);
			fRec10[0] = fConst65 * (4.0f * fRec11[0] - 8.0f * fRec11[2] + 4.0f * fRec11[4]) - fConst56 * (fConst54 * fRec10[1] + fConst53 * fRec10[2] + fConst52 * fRec10[3] + fConst49 * fRec10[4]);
			fRec15[0] = fTemp1 - fConst63 * (fConst61 * fRec15[1] + fConst53 * fRec15[2] + fConst60 * fRec15[3] + fConst58 * fRec15[4]);
			fRec14[0] = fConst65 * (4.0f * fRec15[0] - 8.0f * fRec15[2] + 4.0f * fRec15[4]) - fConst56 * (fConst54 * fRec14[1] + fConst53 * fRec14[2] + fConst52 * fRec14[3] + fConst49 * fRec14[4]);
			fRec13[0] = fSlow17 * std::fabs(fConst66 * (4.0f * fRec14[0] - 8.0f * fRec14[2] + 4.0f * fRec14[4])) + fSlow16 * fRec13[1];
			fRec12[0] = fRec13[0] * fSlow13 + fSlow12 * fRec12[1];
			fRec17[0] = fTemp9 - fConst92 * (fConst90 * fRec17[1] + fConst82 * fRec17[2] + fConst89 * fRec17[3] + fConst87 * fRec17[4]);
			fRec16[0] = fConst94 * (4.0f * fRec17[0] - 8.0f * fRec17[2] + 4.0f * fRec17[4]) - fConst85 * (fConst83 * fRec16[1] + fConst82 * fRec16[2] + fConst81 * fRec16[3] + fConst78 * fRec16[4]);
			fRec21[0] = fTemp1 - fConst92 * (fConst90 * fRec21[1] + fConst82 * fRec21[2] + fConst89 * fRec21[3] + fConst87 * fRec21[4]);
			fRec20[0] = fConst94 * (4.0f * fRec21[0] - 8.0f * fRec21[2] + 4.0f * fRec21[4]) - fConst85 * (fConst83 * fRec20[1] + fConst82 * fRec20[2] + fConst81 * fRec20[3] + fConst78 * fRec20[4]);
			fRec19[0] = fSlow17 * std::fabs(fConst95 * (4.0f * fRec20[0] - 8.0f * fRec20[2] + 4.0f * fRec20[4])) + fSlow16 * fRec19[1];
			fRec18[0] = fRec19[0] * fSlow13 + fSlow12 * fRec18[1];
			fRec23[0] = fTemp9 - fConst121 * (fConst119 * fRec23[1] + fConst111 * fRec23[2] + fConst118 * fRec23[3] + fConst116 * fRec23[4]);
			fRec22[0] = fConst123 * (4.0f * fRec23[0] - 8.0f * fRec23[2] + 4.0f * fRec23[4]) - fConst114 * (fConst112 * fRec22[1] + fConst111 * fRec22[2] + fConst110 * fRec22[3] + fConst107 * fRec22[4]);
			fRec27[0] = fTemp1 - fConst121 * (fConst119 * fRec27[1] + fConst111 * fRec27[2] + fConst118 * fRec27[3] + fConst116 * fRec27[4]);
			fRec26[0] = fConst123 * (4.0f * fRec27[0] - 8.0f * fRec27[2] + 4.0f * fRec27[4]) - fConst114 * (fConst112 * fRec26[1] + fConst111 * fRec26[2] + fConst110 * fRec26[3] + fConst107 * fRec26[4]);
			fRec25[0] = fSlow17 * std::fabs(fConst124 * (4.0f * fRec26[0] - 8.0f * fRec26[2] + 4.0f * fRec26[4])) + fSlow16 * fRec25[1];
			fRec24[0] = fRec25[0] * fSlow13 + fSlow12 * fRec24[1];
			fRec29[0] = fTemp9 - fConst150 * (fConst148 * fRec29[1] + fConst140 * fRec29[2] + fConst147 * fRec29[3] + fConst145 * fRec29[4]);
			fRec28[0] = fConst152 * (4.0f * fRec29[0] - 8.0f * fRec29[2] + 4.0f * fRec29[4]) - fConst143 * (fConst141 * fRec28[1] + fConst140 * fRec28[2] + fConst139 * fRec28[3] + fConst136 * fRec28[4]);
			fRec33[0] = fTemp1 - fConst150 * (fConst148 * fRec33[1] + fConst140 * fRec33[2] + fConst147 * fRec33[3] + fConst145 * fRec33[4]);
			fRec32[0] = fConst152 * (4.0f * fRec33[0] - 8.0f * fRec33[2] + 4.0f * fRec33[4]) - fConst143 * (fConst140 * fRec32[2] + fConst141 * fRec32[1] + fConst139 * fRec32[3] + fConst136 * fRec32[4]);
			fRec31[0] = fSlow17 * std::fabs(fConst153 * (4.0f * fRec32[0] - 8.0f * fRec32[2] + 4.0f * fRec32[4])) + fSlow16 * fRec31[1];
			fRec30[0] = fRec31[0] * fSlow13 + fSlow12 * fRec30[1];
			fRec35[0] = fTemp9 - fConst179 * (fConst177 * fRec35[1] + fConst169 * fRec35[2] + fConst176 * fRec35[3] + fConst174 * fRec35[4]);
			fRec34[0] = fConst181 * (4.0f * fRec35[0] - 8.0f * fRec35[2] + 4.0f * fRec35[4]) - fConst172 * (fConst170 * fRec34[1] + fConst169 * fRec34[2] + fConst168 * fRec34[3] + fConst165 * fRec34[4]);
			fRec39[0] = fTemp1 - fConst179 * (fConst177 * fRec39[1] + fConst169 * fRec39[2] + fConst176 * fRec39[3] + fConst174 * fRec39[4]);
			fRec38[0] = fConst181 * (4.0f * fRec39[0] - 8.0f * fRec39[2] + 4.0f * fRec39[4]) - fConst172 * (fConst170 * fRec38[1] + fConst169 * fRec38[2] + fConst168 * fRec38[3] + fConst165 * fRec38[4]);
			fRec37[0] = fSlow17 * std::fabs(fConst182 * (4.0f * fRec38[0] - 8.0f * fRec38[2] + 4.0f * fRec38[4])) + fSlow16 * fRec37[1];
			fRec36[0] = fRec37[0] * fSlow13 + fSlow12 * fRec36[1];
			fRec41[0] = fTemp9 - fConst208 * (fConst206 * fRec41[1] + fConst198 * fRec41[2] + fConst205 * fRec41[3] + fConst203 * fRec41[4]);
			fRec40[0] = fConst210 * (4.0f * fRec41[0] - 8.0f * fRec41[2] + 4.0f * fRec41[4]) - fConst201 * (fConst199 * fRec40[1] + fConst198 * fRec40[2] + fConst197 * fRec40[3] + fConst194 * fRec40[4]);
			fRec45[0] = fTemp1 - fConst208 * (fConst206 * fRec45[1] + fConst198 * fRec45[2] + fConst205 * fRec45[3] + fConst203 * fRec45[4]);
			fRec44[0] = fConst210 * (4.0f * fRec45[0] - 8.0f * fRec45[2] + 4.0f * fRec45[4]) - fConst201 * (fConst199 * fRec44[1] + fConst198 * fRec44[2] + fConst197 * fRec44[3] + fConst194 * fRec44[4]);
			fRec43[0] = fSlow17 * std::fabs(fConst211 * (4.0f * fRec44[0] - 8.0f * fRec44[2] + 4.0f * fRec44[4])) + fSlow16 * fRec43[1];
			fRec42[0] = fRec43[0] * fSlow13 + fSlow12 * fRec42[1];
			fRec47[0] = fTemp9 - fConst237 * (fConst235 * fRec47[1] + fConst227 * fRec47[2] + fConst234 * fRec47[3] + fConst232 * fRec47[4]);
			fRec46[0] = fConst239 * (4.0f * fRec47[0] - 8.0f * fRec47[2] + 4.0f * fRec47[4]) - fConst230 * (fConst228 * fRec46[1] + fConst227 * fRec46[2] + fConst226 * fRec46[3] + fConst223 * fRec46[4]);
			fRec51[0] = fTemp1 - fConst237 * (fConst235 * fRec51[1] + fConst227 * fRec51[2] + fConst234 * fRec51[3] + fConst232 * fRec51[4]);
			fRec50[0] = fConst239 * (4.0f * fRec51[0] - 8.0f * fRec51[2] + 4.0f * fRec51[4]) - fConst230 * (fConst228 * fRec50[1] + fConst227 * fRec50[2] + fConst226 * fRec50[3] + fConst223 * fRec50[4]);
			fRec49[0] = fSlow17 * std::fabs(fConst240 * (4.0f * fRec50[0] - 8.0f * fRec50[2] + 4.0f * fRec50[4])) + fSlow16 * fRec49[1];
			fRec48[0] = fRec49[0] * fSlow13 + fSlow12 * fRec48[1];
			fRec53[0] = fTemp9 - fConst266 * (fConst264 * fRec53[1] + fConst256 * fRec53[2] + fConst263 * fRec53[3] + fConst261 * fRec53[4]);
			fRec52[0] = fConst268 * (4.0f * fRec53[0] - 8.0f * fRec53[2] + 4.0f * fRec53[4]) - fConst259 * (fConst257 * fRec52[1] + fConst256 * fRec52[2] + fConst255 * fRec52[3] + fConst252 * fRec52[4]);
			fRec57[0] = fTemp1 - fConst266 * (fConst264 * fRec57[1] + fConst256 * fRec57[2] + fConst263 * fRec57[3] + fConst261 * fRec57[4]);
			fRec56[0] = fConst268 * (4.0f * fRec57[0] - 8.0f * fRec57[2] + 4.0f * fRec57[4]) - fConst259 * (fConst257 * fRec56[1] + fConst256 * fRec56[2] + fConst255 * fRec56[3] + fConst252 * fRec56[4]);
			fRec55[0] = fSlow17 * std::fabs(fConst269 * (4.0f * fRec56[0] - 8.0f * fRec56[2] + 4.0f * fRec56[4])) + fSlow16 * fRec55[1];
			fRec54[0] = fRec55[0] * fSlow13 + fSlow12 * fRec54[1];
			fRec59[0] = fTemp9 - fConst295 * (fConst293 * fRec59[1] + fConst285 * fRec59[2] + fConst292 * fRec59[3] + fConst290 * fRec59[4]);
			fRec58[0] = fConst297 * (4.0f * fRec59[0] - 8.0f * fRec59[2] + 4.0f * fRec59[4]) - fConst288 * (fConst286 * fRec58[1] + fConst285 * fRec58[2] + fConst284 * fRec58[3] + fConst281 * fRec58[4]);
			fRec63[0] = fTemp1 - fConst295 * (fConst293 * fRec63[1] + fConst285 * fRec63[2] + fConst292 * fRec63[3] + fConst290 * fRec63[4]);
			fRec62[0] = fConst297 * (4.0f * fRec63[0] - 8.0f * fRec63[2] + 4.0f * fRec63[4]) - fConst288 * (fConst286 * fRec62[1] + fConst285 * fRec62[2] + fConst284 * fRec62[3] + fConst281 * fRec62[4]);
			fRec61[0] = fSlow17 * std::fabs(fConst298 * (4.0f * fRec62[0] - 8.0f * fRec62[2] + 4.0f * fRec62[4])) + fSlow16 * fRec61[1];
			fRec60[0] = fRec61[0] * fSlow13 + fSlow12 * fRec60[1];
			fRec65[0] = fTemp9 - fConst324 * (fConst322 * fRec65[1] + fConst314 * fRec65[2] + fConst321 * fRec65[3] + fConst319 * fRec65[4]);
			fRec64[0] = fConst326 * (4.0f * fRec65[0] - 8.0f * fRec65[2] + 4.0f * fRec65[4]) - fConst317 * (fConst315 * fRec64[1] + fConst314 * fRec64[2] + fConst313 * fRec64[3] + fConst310 * fRec64[4]);
			fRec69[0] = fTemp1 - fConst324 * (fConst322 * fRec69[1] + fConst314 * fRec69[2] + fConst321 * fRec69[3] + fConst319 * fRec69[4]);
			fRec68[0] = fConst326 * (4.0f * fRec69[0] - 8.0f * fRec69[2] + 4.0f * fRec69[4]) - fConst317 * (fConst315 * fRec68[1] + fConst314 * fRec68[2] + fConst313 * fRec68[3] + fConst310 * fRec68[4]);
			fRec67[0] = fSlow17 * std::fabs(fConst327 * (4.0f * fRec68[0] - 8.0f * fRec68[2] + 4.0f * fRec68[4])) + fSlow16 * fRec67[1];
			fRec66[0] = fRec67[0] * fSlow13 + fSlow12 * fRec66[1];
			fRec71[0] = fTemp9 - fConst353 * (fConst351 * fRec71[1] + fConst343 * fRec71[2] + fConst350 * fRec71[3] + fConst348 * fRec71[4]);
			fRec70[0] = fConst355 * (4.0f * fRec71[0] - 8.0f * fRec71[2] + 4.0f * fRec71[4]) - fConst346 * (fConst344 * fRec70[1] + fConst343 * fRec70[2] + fConst342 * fRec70[3] + fConst339 * fRec70[4]);
			fRec75[0] = fTemp1 - fConst353 * (fConst351 * fRec75[1] + fConst343 * fRec75[2] + fConst350 * fRec75[3] + fConst348 * fRec75[4]);
			fRec74[0] = fConst355 * (4.0f * fRec75[0] - 8.0f * fRec75[2] + 4.0f * fRec75[4]) - fConst346 * (fConst344 * fRec74[1] + fConst343 * fRec74[2] + fConst342 * fRec74[3] + fConst339 * fRec74[4]);
			fRec73[0] = fSlow17 * std::fabs(fConst356 * (4.0f * fRec74[0] - 8.0f * fRec74[2] + 4.0f * fRec74[4])) + fSlow16 * fRec73[1];
			fRec72[0] = fRec73[0] * fSlow13 + fSlow12 * fRec72[1];
			fRec77[0] = fTemp9 - fConst382 * (fConst380 * fRec77[1] + fConst372 * fRec77[2] + fConst379 * fRec77[3] + fConst377 * fRec77[4]);
			fRec76[0] = fConst384 * (4.0f * fRec77[0] - 8.0f * fRec77[2] + 4.0f * fRec77[4]) - fConst375 * (fConst373 * fRec76[1] + fConst372 * fRec76[2] + fConst371 * fRec76[3] + fConst368 * fRec76[4]);
			fRec81[0] = fTemp1 - fConst382 * (fConst380 * fRec81[1] + fConst372 * fRec81[2] + fConst379 * fRec81[3] + fConst377 * fRec81[4]);
			fRec80[0] = fConst384 * (4.0f * fRec81[0] - 8.0f * fRec81[2] + 4.0f * fRec81[4]) - fConst375 * (fConst373 * fRec80[1] + fConst372 * fRec80[2] + fConst371 * fRec80[3] + fConst368 * fRec80[4]);
			fRec79[0] = fSlow17 * std::fabs(fConst385 * (4.0f * fRec80[0] - 8.0f * fRec80[2] + 4.0f * fRec80[4])) + fSlow16 * fRec79[1];
			fRec78[0] = fRec79[0] * fSlow13 + fSlow12 * fRec78[1];
			fRec83[0] = fTemp9 - fConst411 * (fConst409 * fRec83[1] + fConst401 * fRec83[2] + fConst408 * fRec83[3] + fConst406 * fRec83[4]);
			fRec82[0] = fConst413 * (4.0f * fRec83[0] - 8.0f * fRec83[2] + 4.0f * fRec83[4]) - fConst404 * (fConst402 * fRec82[1] + fConst401 * fRec82[2] + fConst400 * fRec82[3] + fConst397 * fRec82[4]);
			fRec87[0] = fTemp1 - fConst411 * (fConst409 * fRec87[1] + fConst401 * fRec87[2] + fConst408 * fRec87[3] + fConst406 * fRec87[4]);
			fRec86[0] = fConst413 * (4.0f * fRec87[0] - 8.0f * fRec87[2] + 4.0f * fRec87[4]) - fConst404 * (fConst402 * fRec86[1] + fConst401 * fRec86[2] + fConst400 * fRec86[3] + fConst397 * fRec86[4]);
			fRec85[0] = fSlow17 * std::fabs(fConst414 * (4.0f * fRec86[0] - 8.0f * fRec86[2] + 4.0f * fRec86[4])) + fSlow16 * fRec85[1];
			fRec84[0] = fRec85[0] * fSlow13 + fSlow12 * fRec84[1];
			fRec89[0] = fTemp9 - fConst440 * (fConst438 * fRec89[1] + fConst430 * fRec89[2] + fConst437 * fRec89[3] + fConst435 * fRec89[4]);
			fRec88[0] = fConst442 * (4.0f * fRec89[0] - 8.0f * fRec89[2] + 4.0f * fRec89[4]) - fConst433 * (fConst431 * fRec88[1] + fConst430 * fRec88[2] + fConst429 * fRec88[3] + fConst426 * fRec88[4]);
			fRec93[0] = fTemp1 - fConst440 * (fConst438 * fRec93[1] + fConst430 * fRec93[2] + fConst437 * fRec93[3] + fConst435 * fRec93[4]);
			fRec92[0] = fConst442 * (4.0f * fRec93[0] - 8.0f * fRec93[2] + 4.0f * fRec93[4]) - fConst433 * (fConst431 * fRec92[1] + fConst430 * fRec92[2] + fConst429 * fRec92[3] + fConst426 * fRec92[4]);
			fRec91[0] = fSlow17 * std::fabs(fConst443 * (4.0f * fRec92[0] - 8.0f * fRec92[2] + 4.0f * fRec92[4])) + fSlow16 * fRec91[1];
			fRec90[0] = fRec91[0] * fSlow13 + fSlow12 * fRec90[1];
			fRec95[0] = fTemp9 - fConst469 * (fConst467 * fRec95[1] + fConst459 * fRec95[2] + fConst466 * fRec95[3] + fConst464 * fRec95[4]);
			fRec94[0] = fConst471 * (4.0f * fRec95[0] - 8.0f * fRec95[2] + 4.0f * fRec95[4]) - fConst462 * (fConst460 * fRec94[1] + fConst459 * fRec94[2] + fConst458 * fRec94[3] + fConst455 * fRec94[4]);
			fRec99[0] = fTemp1 - fConst469 * (fConst467 * fRec99[1] + fConst459 * fRec99[2] + fConst466 * fRec99[3] + fConst464 * fRec99[4]);
			fRec98[0] = fConst471 * (4.0f * fRec99[0] - 8.0f * fRec99[2] + 4.0f * fRec99[4]) - fConst462 * (fConst460 * fRec98[1] + fConst459 * fRec98[2] + fConst458 * fRec98[3] + fConst455 * fRec98[4]);
			fRec97[0] = fSlow17 * std::fabs(fConst472 * (4.0f * fRec98[0] - 8.0f * fRec98[2] + 4.0f * fRec98[4])) + fSlow16 * fRec97[1];
			fRec96[0] = fRec97[0] * fSlow13 + fSlow12 * fRec96[1];
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp0 : fSlow36 * fTemp1 + fSlow35 * (fSlow33 * fRec96[0] * (4.0f * (fRec94[0] + fRec94[4]) - 8.0f * fRec94[2]) + fSlow32 * fRec90[0] * (4.0f * (fRec88[0] + fRec88[4]) - 8.0f * fRec88[2]) + fSlow31 * fRec84[0] * (4.0f * (fRec82[0] + fRec82[4]) - 8.0f * fRec82[2]) + fSlow30 * fRec78[0] * (4.0f * (fRec76[0] + fRec76[4]) - 8.0f * fRec76[2]) + fSlow29 * fRec72[0] * (4.0f * (fRec70[0] + fRec70[4]) - 8.0f * fRec70[2]) + fSlow28 * fRec66[0] * (4.0f * (fRec64[0] + fRec64[4]) - 8.0f * fRec64[2]) + fSlow27 * fRec60[0] * (4.0f * (fRec58[0] + fRec58[4]) - 8.0f * fRec58[2]) + fSlow26 * fRec54[0] * (4.0f * (fRec52[0] + fRec52[4]) - 8.0f * fRec52[2]) + fSlow25 * fRec48[0] * (4.0f * (fRec46[0] + fRec46[4]) - 8.0f * fRec46[2]) + fSlow24 * fRec42[0] * (4.0f * (fRec40[0] + fRec40[4]) - 8.0f * fRec40[2]) + fSlow23 * fRec36[0] * (4.0f * (fRec34[0] + fRec34[4]) - 8.0f * fRec34[2]) + fSlow22 * fRec30[0] * (4.0f * (fRec28[0] + fRec28[4]) - 8.0f * fRec28[2]) + fSlow21 * fRec24[0] * (4.0f * (fRec22[0] + fRec22[4]) - 8.0f * fRec22[2]) + fSlow20 * fRec18[0] * (4.0f * (fRec16[0] + fRec16[4]) - 8.0f * fRec16[2]) + fSlow19 * fRec12[0] * (4.0f * (fRec10[0] + fRec10[4]) - 8.0f * fRec10[2]) + fSlow18 * fRec6[0] * (4.0f * (fRec0[0] + fRec0[4]) - 8.0f * fRec0[2]))));
			float fTemp10 = float(input1[i0]);
			float fTemp11 = ((iSlow0) ? 0.0f : fTemp10);
			float fTemp12 = fTemp8 + fSlow1 * fTemp11;
			fRec101[0] = fTemp12 - fConst34 * (fConst32 * fRec101[1] + fConst22 * fRec101[2] + fConst31 * fRec101[3] + fConst28 * fRec101[4]);
			fRec100[0] = fConst36 * (4.0f * fRec101[0] - 8.0f * fRec101[2] + 4.0f * fRec101[4]) - fConst25 * (fConst23 * fRec100[1] + fConst22 * fRec100[2] + fConst21 * fRec100[3] + fConst17 * fRec100[4]);
			fRec105[0] = fTemp11 - fConst34 * (fConst32 * fRec105[1] + fConst22 * fRec105[2] + fConst31 * fRec105[3] + fConst28 * fRec105[4]);
			fRec104[0] = fConst36 * (4.0f * fRec105[0] - 8.0f * fRec105[2] + 4.0f * fRec105[4]) - fConst25 * (fConst23 * fRec104[1] + fConst22 * fRec104[2] + fConst21 * fRec104[3] + fConst17 * fRec104[4]);
			fRec103[0] = fSlow17 * std::fabs(fConst37 * (4.0f * fRec104[0] - 8.0f * fRec104[2] + 4.0f * fRec104[4])) + fSlow16 * fRec103[1];
			fRec102[0] = fRec103[0] * fSlow13 + fSlow12 * fRec102[1];
			fRec107[0] = fTemp12 - fConst63 * (fConst61 * fRec107[1] + fConst53 * fRec107[2] + fConst60 * fRec107[3] + fConst58 * fRec107[4]);
			fRec106[0] = fConst65 * (4.0f * fRec107[0] - 8.0f * fRec107[2] + 4.0f * fRec107[4]) - fConst56 * (fConst54 * fRec106[1] + fConst53 * fRec106[2] + fConst52 * fRec106[3] + fConst49 * fRec106[4]);
			fRec111[0] = fTemp11 - fConst63 * (fConst61 * fRec111[1] + fConst53 * fRec111[2] + fConst60 * fRec111[3] + fConst58 * fRec111[4]);
			fRec110[0] = fConst65 * (4.0f * fRec111[0] - 8.0f * fRec111[2] + 4.0f * fRec111[4]) - fConst56 * (fConst54 * fRec110[1] + fConst53 * fRec110[2] + fConst52 * fRec110[3] + fConst49 * fRec110[4]);
			fRec109[0] = fSlow17 * std::fabs(fConst66 * (4.0f * fRec110[0] - 8.0f * fRec110[2] + 4.0f * fRec110[4])) + fSlow16 * fRec109[1];
			fRec108[0] = fRec109[0] * fSlow13 + fSlow12 * fRec108[1];
			fRec113[0] = fTemp12 - fConst150 * (fConst140 * fRec113[2] + fConst148 * fRec113[1] + fConst147 * fRec113[3] + fConst145 * fRec113[4]);
			fRec112[0] = fConst152 * (4.0f * fRec113[0] - 8.0f * fRec113[2] + 4.0f * fRec113[4]) - fConst143 * (fConst141 * fRec112[1] + fConst140 * fRec112[2] + fConst139 * fRec112[3] + fConst136 * fRec112[4]);
			fRec117[0] = fTemp11 - fConst150 * (fConst148 * fRec117[1] + fConst140 * fRec117[2] + fConst147 * fRec117[3] + fConst145 * fRec117[4]);
			fRec116[0] = fConst152 * (4.0f * fRec117[0] - 8.0f * fRec117[2] + 4.0f * fRec117[4]) - fConst143 * (fConst140 * fRec116[2] + fConst141 * fRec116[1] + fConst139 * fRec116[3] + fConst136 * fRec116[4]);
			fRec115[0] = fSlow17 * std::fabs(fConst153 * (4.0f * fRec116[0] - 8.0f * fRec116[2] + 4.0f * fRec116[4])) + fSlow16 * fRec115[1];
			fRec114[0] = fRec115[0] * fSlow13 + fSlow12 * fRec114[1];
			fRec119[0] = fTemp12 - fConst179 * (fConst177 * fRec119[1] + fConst169 * fRec119[2] + fConst176 * fRec119[3] + fConst174 * fRec119[4]);
			fRec118[0] = fConst181 * (4.0f * fRec119[0] - 8.0f * fRec119[2] + 4.0f * fRec119[4]) - fConst172 * (fConst170 * fRec118[1] + fConst169 * fRec118[2] + fConst168 * fRec118[3] + fConst165 * fRec118[4]);
			fRec123[0] = fTemp11 - fConst179 * (fConst177 * fRec123[1] + fConst169 * fRec123[2] + fConst176 * fRec123[3] + fConst174 * fRec123[4]);
			fRec122[0] = fConst181 * (4.0f * fRec123[0] - 8.0f * fRec123[2] + 4.0f * fRec123[4]) - fConst172 * (fConst170 * fRec122[1] + fConst169 * fRec122[2] + fConst168 * fRec122[3] + fConst165 * fRec122[4]);
			fRec121[0] = fSlow17 * std::fabs(fConst182 * (4.0f * fRec122[0] - 8.0f * fRec122[2] + 4.0f * fRec122[4])) + fSlow16 * fRec121[1];
			fRec120[0] = fRec121[0] * fSlow13 + fSlow12 * fRec120[1];
			fRec125[0] = fTemp12 - fConst208 * (fConst206 * fRec125[1] + fConst198 * fRec125[2] + fConst205 * fRec125[3] + fConst203 * fRec125[4]);
			fRec124[0] = fConst210 * (4.0f * fRec125[0] - 8.0f * fRec125[2] + 4.0f * fRec125[4]) - fConst201 * (fConst199 * fRec124[1] + fConst198 * fRec124[2] + fConst197 * fRec124[3] + fConst194 * fRec124[4]);
			fRec129[0] = fTemp11 - fConst208 * (fConst206 * fRec129[1] + fConst198 * fRec129[2] + fConst205 * fRec129[3] + fConst203 * fRec129[4]);
			fRec128[0] = fConst210 * (4.0f * fRec129[0] - 8.0f * fRec129[2] + 4.0f * fRec129[4]) - fConst201 * (fConst199 * fRec128[1] + fConst198 * fRec128[2] + fConst197 * fRec128[3] + fConst194 * fRec128[4]);
			fRec127[0] = fSlow17 * std::fabs(fConst211 * (4.0f * fRec128[0] - 8.0f * fRec128[2] + 4.0f * fRec128[4])) + fSlow16 * fRec127[1];
			fRec126[0] = fRec127[0] * fSlow13 + fSlow12 * fRec126[1];
			fRec131[0] = fTemp12 - fConst237 * (fConst235 * fRec131[1] + fConst227 * fRec131[2] + fConst234 * fRec131[3] + fConst232 * fRec131[4]);
			fRec130[0] = fConst239 * (4.0f * fRec131[0] - 8.0f * fRec131[2] + 4.0f * fRec131[4]) - fConst230 * (fConst228 * fRec130[1] + fConst227 * fRec130[2] + fConst226 * fRec130[3] + fConst223 * fRec130[4]);
			fRec135[0] = fTemp11 - fConst237 * (fConst235 * fRec135[1] + fConst227 * fRec135[2] + fConst234 * fRec135[3] + fConst232 * fRec135[4]);
			fRec134[0] = fConst239 * (4.0f * fRec135[0] - 8.0f * fRec135[2] + 4.0f * fRec135[4]) - fConst230 * (fConst228 * fRec134[1] + fConst227 * fRec134[2] + fConst226 * fRec134[3] + fConst223 * fRec134[4]);
			fRec133[0] = fSlow17 * std::fabs(fConst240 * (4.0f * fRec134[0] - 8.0f * fRec134[2] + 4.0f * fRec134[4])) + fSlow16 * fRec133[1];
			fRec132[0] = fRec133[0] * fSlow13 + fSlow12 * fRec132[1];
			fRec137[0] = fTemp12 - fConst266 * (fConst264 * fRec137[1] + fConst256 * fRec137[2] + fConst263 * fRec137[3] + fConst261 * fRec137[4]);
			fRec136[0] = fConst268 * (4.0f * fRec137[0] - 8.0f * fRec137[2] + 4.0f * fRec137[4]) - fConst259 * (fConst257 * fRec136[1] + fConst256 * fRec136[2] + fConst255 * fRec136[3] + fConst252 * fRec136[4]);
			fRec141[0] = fTemp11 - fConst266 * (fConst264 * fRec141[1] + fConst256 * fRec141[2] + fConst263 * fRec141[3] + fConst261 * fRec141[4]);
			fRec140[0] = fConst268 * (4.0f * fRec141[0] - 8.0f * fRec141[2] + 4.0f * fRec141[4]) - fConst259 * (fConst257 * fRec140[1] + fConst256 * fRec140[2] + fConst255 * fRec140[3] + fConst252 * fRec140[4]);
			fRec139[0] = fSlow17 * std::fabs(fConst269 * (4.0f * fRec140[0] - 8.0f * fRec140[2] + 4.0f * fRec140[4])) + fSlow16 * fRec139[1];
			fRec138[0] = fRec139[0] * fSlow13 + fSlow12 * fRec138[1];
			fRec143[0] = fTemp12 - fConst295 * (fConst293 * fRec143[1] + fConst285 * fRec143[2] + fConst292 * fRec143[3] + fConst290 * fRec143[4]);
			fRec142[0] = fConst297 * (4.0f * fRec143[0] - 8.0f * fRec143[2] + 4.0f * fRec143[4]) - fConst288 * (fConst286 * fRec142[1] + fConst285 * fRec142[2] + fConst284 * fRec142[3] + fConst281 * fRec142[4]);
			fRec147[0] = fTemp11 - fConst295 * (fConst293 * fRec147[1] + fConst285 * fRec147[2] + fConst292 * fRec147[3] + fConst290 * fRec147[4]);
			fRec146[0] = fConst297 * (4.0f * fRec147[0] - 8.0f * fRec147[2] + 4.0f * fRec147[4]) - fConst288 * (fConst286 * fRec146[1] + fConst285 * fRec146[2] + fConst284 * fRec146[3] + fConst281 * fRec146[4]);
			fRec145[0] = fSlow17 * std::fabs(fConst298 * (4.0f * fRec146[0] - 8.0f * fRec146[2] + 4.0f * fRec146[4])) + fSlow16 * fRec145[1];
			fRec144[0] = fRec145[0] * fSlow13 + fSlow12 * fRec144[1];
			fRec149[0] = fTemp12 - fConst324 * (fConst314 * fRec149[2] + fConst322 * fRec149[1] + fConst321 * fRec149[3] + fConst319 * fRec149[4]);
			fRec148[0] = fConst326 * (4.0f * fRec149[0] - 8.0f * fRec149[2] + 4.0f * fRec149[4]) - fConst317 * (fConst315 * fRec148[1] + fConst314 * fRec148[2] + fConst313 * fRec148[3] + fConst310 * fRec148[4]);
			fRec153[0] = fTemp11 - fConst324 * (fConst322 * fRec153[1] + fConst314 * fRec153[2] + fConst321 * fRec153[3] + fConst319 * fRec153[4]);
			fRec152[0] = fConst326 * (4.0f * fRec153[0] - 8.0f * fRec153[2] + 4.0f * fRec153[4]) - fConst317 * (fConst315 * fRec152[1] + fConst314 * fRec152[2] + fConst313 * fRec152[3] + fConst310 * fRec152[4]);
			fRec151[0] = fSlow17 * std::fabs(fConst327 * (4.0f * fRec152[0] - 8.0f * fRec152[2] + 4.0f * fRec152[4])) + fSlow16 * fRec151[1];
			fRec150[0] = fRec151[0] * fSlow13 + fSlow12 * fRec150[1];
			fRec155[0] = fTemp12 - fConst353 * (fConst351 * fRec155[1] + fConst343 * fRec155[2] + fConst350 * fRec155[3] + fConst348 * fRec155[4]);
			fRec154[0] = fConst355 * (4.0f * fRec155[0] - 8.0f * fRec155[2] + 4.0f * fRec155[4]) - fConst346 * (fConst344 * fRec154[1] + fConst343 * fRec154[2] + fConst342 * fRec154[3] + fConst339 * fRec154[4]);
			fRec159[0] = fTemp11 - fConst353 * (fConst351 * fRec159[1] + fConst343 * fRec159[2] + fConst350 * fRec159[3] + fConst348 * fRec159[4]);
			fRec158[0] = fConst355 * (4.0f * fRec159[0] - 8.0f * fRec159[2] + 4.0f * fRec159[4]) - fConst346 * (fConst344 * fRec158[1] + fConst343 * fRec158[2] + fConst342 * fRec158[3] + fConst339 * fRec158[4]);
			fRec157[0] = fSlow17 * std::fabs(fConst356 * (4.0f * fRec158[0] - 8.0f * fRec158[2] + 4.0f * fRec158[4])) + fSlow16 * fRec157[1];
			fRec156[0] = fRec157[0] * fSlow13 + fSlow12 * fRec156[1];
			fRec161[0] = fTemp12 - fConst382 * (fConst380 * fRec161[1] + fConst372 * fRec161[2] + fConst379 * fRec161[3] + fConst377 * fRec161[4]);
			fRec160[0] = fConst384 * (4.0f * fRec161[0] - 8.0f * fRec161[2] + 4.0f * fRec161[4]) - fConst375 * (fConst373 * fRec160[1] + fConst372 * fRec160[2] + fConst371 * fRec160[3] + fConst368 * fRec160[4]);
			fRec165[0] = fTemp11 - fConst382 * (fConst380 * fRec165[1] + fConst372 * fRec165[2] + fConst379 * fRec165[3] + fConst377 * fRec165[4]);
			fRec164[0] = fConst384 * (4.0f * fRec165[0] - 8.0f * fRec165[2] + 4.0f * fRec165[4]) - fConst375 * (fConst373 * fRec164[1] + fConst372 * fRec164[2] + fConst371 * fRec164[3] + fConst368 * fRec164[4]);
			fRec163[0] = fSlow17 * std::fabs(fConst385 * (4.0f * fRec164[0] - 8.0f * fRec164[2] + 4.0f * fRec164[4])) + fSlow16 * fRec163[1];
			fRec162[0] = fRec163[0] * fSlow13 + fSlow12 * fRec162[1];
			fRec167[0] = fTemp12 - fConst411 * (fConst409 * fRec167[1] + fConst401 * fRec167[2] + fConst408 * fRec167[3] + fConst406 * fRec167[4]);
			fRec166[0] = fConst413 * (4.0f * fRec167[0] - 8.0f * fRec167[2] + 4.0f * fRec167[4]) - fConst404 * (fConst402 * fRec166[1] + fConst401 * fRec166[2] + fConst400 * fRec166[3] + fConst397 * fRec166[4]);
			fRec171[0] = fTemp11 - fConst411 * (fConst409 * fRec171[1] + fConst401 * fRec171[2] + fConst408 * fRec171[3] + fConst406 * fRec171[4]);
			fRec170[0] = fConst413 * (4.0f * fRec171[0] - 8.0f * fRec171[2] + 4.0f * fRec171[4]) - fConst404 * (fConst402 * fRec170[1] + fConst401 * fRec170[2] + fConst400 * fRec170[3] + fConst397 * fRec170[4]);
			fRec169[0] = fSlow17 * std::fabs(fConst414 * (4.0f * fRec170[0] - 8.0f * fRec170[2] + 4.0f * fRec170[4])) + fSlow16 * fRec169[1];
			fRec168[0] = fRec169[0] * fSlow13 + fSlow12 * fRec168[1];
			fRec173[0] = fTemp12 - fConst440 * (fConst438 * fRec173[1] + fConst430 * fRec173[2] + fConst437 * fRec173[3] + fConst435 * fRec173[4]);
			fRec172[0] = fConst442 * (4.0f * fRec173[0] - 8.0f * fRec173[2] + 4.0f * fRec173[4]) - fConst433 * (fConst431 * fRec172[1] + fConst430 * fRec172[2] + fConst429 * fRec172[3] + fConst426 * fRec172[4]);
			fRec177[0] = fTemp11 - fConst440 * (fConst438 * fRec177[1] + fConst430 * fRec177[2] + fConst437 * fRec177[3] + fConst435 * fRec177[4]);
			fRec176[0] = fConst442 * (4.0f * fRec177[0] - 8.0f * fRec177[2] + 4.0f * fRec177[4]) - fConst433 * (fConst431 * fRec176[1] + fConst430 * fRec176[2] + fConst429 * fRec176[3] + fConst426 * fRec176[4]);
			fRec175[0] = fSlow17 * std::fabs(fConst443 * (4.0f * fRec176[0] - 8.0f * fRec176[2] + 4.0f * fRec176[4])) + fSlow16 * fRec175[1];
			fRec174[0] = fRec175[0] * fSlow13 + fSlow12 * fRec174[1];
			fRec179[0] = fTemp12 - fConst469 * (fConst467 * fRec179[1] + fConst459 * fRec179[2] + fConst466 * fRec179[3] + fConst464 * fRec179[4]);
			fRec178[0] = fConst471 * (4.0f * fRec179[0] - 8.0f * fRec179[2] + 4.0f * fRec179[4]) - fConst462 * (fConst459 * fRec178[2] + fConst460 * fRec178[1] + fConst458 * fRec178[3] + fConst455 * fRec178[4]);
			fRec183[0] = fTemp11 - fConst469 * (fConst467 * fRec183[1] + fConst459 * fRec183[2] + fConst466 * fRec183[3] + fConst464 * fRec183[4]);
			fRec182[0] = fConst471 * (4.0f * fRec183[0] - 8.0f * fRec183[2] + 4.0f * fRec183[4]) - fConst462 * (fConst460 * fRec182[1] + fConst459 * fRec182[2] + fConst458 * fRec182[3] + fConst455 * fRec182[4]);
			fRec181[0] = fSlow17 * std::fabs(fConst472 * (4.0f * fRec182[0] - 8.0f * fRec182[2] + 4.0f * fRec182[4])) + fSlow16 * fRec181[1];
			fRec180[0] = fRec181[0] * fSlow13 + fSlow12 * fRec180[1];
			fRec185[0] = fTemp12 - fConst121 * (fConst119 * fRec185[1] + fConst111 * fRec185[2] + fConst118 * fRec185[3] + fConst116 * fRec185[4]);
			fRec184[0] = fConst123 * (4.0f * fRec185[0] - 8.0f * fRec185[2] + 4.0f * fRec185[4]) - fConst114 * (fConst112 * fRec184[1] + fConst111 * fRec184[2] + fConst110 * fRec184[3] + fConst107 * fRec184[4]);
			fRec189[0] = fTemp11 - fConst121 * (fConst119 * fRec189[1] + fConst111 * fRec189[2] + fConst118 * fRec189[3] + fConst116 * fRec189[4]);
			fRec188[0] = fConst123 * (4.0f * fRec189[0] - 8.0f * fRec189[2] + 4.0f * fRec189[4]) - fConst114 * (fConst112 * fRec188[1] + fConst111 * fRec188[2] + fConst110 * fRec188[3] + fConst107 * fRec188[4]);
			fRec187[0] = fSlow17 * std::fabs(fConst124 * (4.0f * fRec188[0] - 8.0f * fRec188[2] + 4.0f * fRec188[4])) + fSlow16 * fRec187[1];
			fRec186[0] = fRec187[0] * fSlow13 + fSlow12 * fRec186[1];
			fRec191[0] = fTemp12 - fConst92 * (fConst90 * fRec191[1] + fConst82 * fRec191[2] + fConst89 * fRec191[3] + fConst87 * fRec191[4]);
			fRec190[0] = fConst94 * (4.0f * fRec191[0] - 8.0f * fRec191[2] + 4.0f * fRec191[4]) - fConst85 * (fConst83 * fRec190[1] + fConst82 * fRec190[2] + fConst81 * fRec190[3] + fConst78 * fRec190[4]);
			fRec195[0] = fTemp11 - fConst92 * (fConst90 * fRec195[1] + fConst82 * fRec195[2] + fConst89 * fRec195[3] + fConst87 * fRec195[4]);
			fRec194[0] = fConst94 * (4.0f * fRec195[0] - 8.0f * fRec195[2] + 4.0f * fRec195[4]) - fConst85 * (fConst83 * fRec194[1] + fConst82 * fRec194[2] + fConst81 * fRec194[3] + fConst78 * fRec194[4]);
			fRec193[0] = fSlow17 * std::fabs(fConst95 * (4.0f * fRec194[0] - 8.0f * fRec194[2] + 4.0f * fRec194[4])) + fSlow16 * fRec193[1];
			fRec192[0] = fRec193[0] * fSlow13 + fSlow12 * fRec192[1];
			output1[i0] = FAUSTFLOAT(((iSlow0) ? fTemp10 : fSlow36 * fTemp11 + fSlow35 * (fSlow20 * fRec192[0] * (4.0f * (fRec190[0] + fRec190[4]) - 8.0f * fRec190[2]) + fSlow21 * fRec186[0] * (4.0f * (fRec184[0] + fRec184[4]) - 8.0f * fRec184[2]) + fSlow33 * fRec180[0] * (4.0f * (fRec178[0] + fRec178[4]) - 8.0f * fRec178[2]) + fSlow32 * fRec174[0] * (4.0f * (fRec172[0] + fRec172[4]) - 8.0f * fRec172[2]) + fSlow31 * fRec168[0] * (4.0f * (fRec166[0] + fRec166[4]) - 8.0f * fRec166[2]) + fSlow30 * fRec162[0] * (4.0f * (fRec160[0] + fRec160[4]) - 8.0f * fRec160[2]) + fSlow29 * fRec156[0] * (4.0f * (fRec154[0] + fRec154[4]) - 8.0f * fRec154[2]) + fSlow28 * fRec150[0] * (4.0f * (fRec148[0] + fRec148[4]) - 8.0f * fRec148[2]) + fSlow27 * fRec144[0] * (4.0f * (fRec142[0] + fRec142[4]) - 8.0f * fRec142[2]) + fSlow26 * fRec138[0] * (4.0f * (fRec136[0] + fRec136[4]) - 8.0f * fRec136[2]) + fSlow25 * fRec132[0] * (4.0f * (fRec130[0] + fRec130[4]) - 8.0f * fRec130[2]) + fSlow24 * fRec126[0] * (4.0f * (fRec124[0] + fRec124[4]) - 8.0f * fRec124[2]) + fSlow23 * fRec120[0] * (4.0f * (fRec118[0] + fRec118[4]) - 8.0f * fRec118[2]) + fSlow22 * fRec114[0] * (4.0f * (fRec112[0] + fRec112[4]) - 8.0f * fRec112[2]) + fSlow19 * fRec108[0] * (4.0f * (fRec106[0] + fRec106[4]) - 8.0f * fRec106[2]) + fSlow18 * fRec102[0] * (4.0f * (fRec100[0] + fRec100[4]) - 8.0f * fRec100[2]))));
			fRec2[1] = fRec2[0];
			fRec4[1] = fRec4[0];
			for (int j0 = 4; j0 > 0; j0 = j0 - 1) {
				fRec1[j0] = fRec1[j0 - 1];
			}
			for (int j1 = 4; j1 > 0; j1 = j1 - 1) {
				fRec0[j1] = fRec0[j1 - 1];
			}
			for (int j2 = 4; j2 > 0; j2 = j2 - 1) {
				fRec9[j2] = fRec9[j2 - 1];
			}
			for (int j3 = 4; j3 > 0; j3 = j3 - 1) {
				fRec8[j3] = fRec8[j3 - 1];
			}
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			for (int j4 = 4; j4 > 0; j4 = j4 - 1) {
				fRec11[j4] = fRec11[j4 - 1];
			}
			for (int j5 = 4; j5 > 0; j5 = j5 - 1) {
				fRec10[j5] = fRec10[j5 - 1];
			}
			for (int j6 = 4; j6 > 0; j6 = j6 - 1) {
				fRec15[j6] = fRec15[j6 - 1];
			}
			for (int j7 = 4; j7 > 0; j7 = j7 - 1) {
				fRec14[j7] = fRec14[j7 - 1];
			}
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			for (int j8 = 4; j8 > 0; j8 = j8 - 1) {
				fRec17[j8] = fRec17[j8 - 1];
			}
			for (int j9 = 4; j9 > 0; j9 = j9 - 1) {
				fRec16[j9] = fRec16[j9 - 1];
			}
			for (int j10 = 4; j10 > 0; j10 = j10 - 1) {
				fRec21[j10] = fRec21[j10 - 1];
			}
			for (int j11 = 4; j11 > 0; j11 = j11 - 1) {
				fRec20[j11] = fRec20[j11 - 1];
			}
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			for (int j12 = 4; j12 > 0; j12 = j12 - 1) {
				fRec23[j12] = fRec23[j12 - 1];
			}
			for (int j13 = 4; j13 > 0; j13 = j13 - 1) {
				fRec22[j13] = fRec22[j13 - 1];
			}
			for (int j14 = 4; j14 > 0; j14 = j14 - 1) {
				fRec27[j14] = fRec27[j14 - 1];
			}
			for (int j15 = 4; j15 > 0; j15 = j15 - 1) {
				fRec26[j15] = fRec26[j15 - 1];
			}
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			for (int j16 = 4; j16 > 0; j16 = j16 - 1) {
				fRec29[j16] = fRec29[j16 - 1];
			}
			for (int j17 = 4; j17 > 0; j17 = j17 - 1) {
				fRec28[j17] = fRec28[j17 - 1];
			}
			for (int j18 = 4; j18 > 0; j18 = j18 - 1) {
				fRec33[j18] = fRec33[j18 - 1];
			}
			for (int j19 = 4; j19 > 0; j19 = j19 - 1) {
				fRec32[j19] = fRec32[j19 - 1];
			}
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			for (int j20 = 4; j20 > 0; j20 = j20 - 1) {
				fRec35[j20] = fRec35[j20 - 1];
			}
			for (int j21 = 4; j21 > 0; j21 = j21 - 1) {
				fRec34[j21] = fRec34[j21 - 1];
			}
			for (int j22 = 4; j22 > 0; j22 = j22 - 1) {
				fRec39[j22] = fRec39[j22 - 1];
			}
			for (int j23 = 4; j23 > 0; j23 = j23 - 1) {
				fRec38[j23] = fRec38[j23 - 1];
			}
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			for (int j24 = 4; j24 > 0; j24 = j24 - 1) {
				fRec41[j24] = fRec41[j24 - 1];
			}
			for (int j25 = 4; j25 > 0; j25 = j25 - 1) {
				fRec40[j25] = fRec40[j25 - 1];
			}
			for (int j26 = 4; j26 > 0; j26 = j26 - 1) {
				fRec45[j26] = fRec45[j26 - 1];
			}
			for (int j27 = 4; j27 > 0; j27 = j27 - 1) {
				fRec44[j27] = fRec44[j27 - 1];
			}
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			for (int j28 = 4; j28 > 0; j28 = j28 - 1) {
				fRec47[j28] = fRec47[j28 - 1];
			}
			for (int j29 = 4; j29 > 0; j29 = j29 - 1) {
				fRec46[j29] = fRec46[j29 - 1];
			}
			for (int j30 = 4; j30 > 0; j30 = j30 - 1) {
				fRec51[j30] = fRec51[j30 - 1];
			}
			for (int j31 = 4; j31 > 0; j31 = j31 - 1) {
				fRec50[j31] = fRec50[j31 - 1];
			}
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			for (int j32 = 4; j32 > 0; j32 = j32 - 1) {
				fRec53[j32] = fRec53[j32 - 1];
			}
			for (int j33 = 4; j33 > 0; j33 = j33 - 1) {
				fRec52[j33] = fRec52[j33 - 1];
			}
			for (int j34 = 4; j34 > 0; j34 = j34 - 1) {
				fRec57[j34] = fRec57[j34 - 1];
			}
			for (int j35 = 4; j35 > 0; j35 = j35 - 1) {
				fRec56[j35] = fRec56[j35 - 1];
			}
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			for (int j36 = 4; j36 > 0; j36 = j36 - 1) {
				fRec59[j36] = fRec59[j36 - 1];
			}
			for (int j37 = 4; j37 > 0; j37 = j37 - 1) {
				fRec58[j37] = fRec58[j37 - 1];
			}
			for (int j38 = 4; j38 > 0; j38 = j38 - 1) {
				fRec63[j38] = fRec63[j38 - 1];
			}
			for (int j39 = 4; j39 > 0; j39 = j39 - 1) {
				fRec62[j39] = fRec62[j39 - 1];
			}
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			for (int j40 = 4; j40 > 0; j40 = j40 - 1) {
				fRec65[j40] = fRec65[j40 - 1];
			}
			for (int j41 = 4; j41 > 0; j41 = j41 - 1) {
				fRec64[j41] = fRec64[j41 - 1];
			}
			for (int j42 = 4; j42 > 0; j42 = j42 - 1) {
				fRec69[j42] = fRec69[j42 - 1];
			}
			for (int j43 = 4; j43 > 0; j43 = j43 - 1) {
				fRec68[j43] = fRec68[j43 - 1];
			}
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			for (int j44 = 4; j44 > 0; j44 = j44 - 1) {
				fRec71[j44] = fRec71[j44 - 1];
			}
			for (int j45 = 4; j45 > 0; j45 = j45 - 1) {
				fRec70[j45] = fRec70[j45 - 1];
			}
			for (int j46 = 4; j46 > 0; j46 = j46 - 1) {
				fRec75[j46] = fRec75[j46 - 1];
			}
			for (int j47 = 4; j47 > 0; j47 = j47 - 1) {
				fRec74[j47] = fRec74[j47 - 1];
			}
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			for (int j48 = 4; j48 > 0; j48 = j48 - 1) {
				fRec77[j48] = fRec77[j48 - 1];
			}
			for (int j49 = 4; j49 > 0; j49 = j49 - 1) {
				fRec76[j49] = fRec76[j49 - 1];
			}
			for (int j50 = 4; j50 > 0; j50 = j50 - 1) {
				fRec81[j50] = fRec81[j50 - 1];
			}
			for (int j51 = 4; j51 > 0; j51 = j51 - 1) {
				fRec80[j51] = fRec80[j51 - 1];
			}
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			for (int j52 = 4; j52 > 0; j52 = j52 - 1) {
				fRec83[j52] = fRec83[j52 - 1];
			}
			for (int j53 = 4; j53 > 0; j53 = j53 - 1) {
				fRec82[j53] = fRec82[j53 - 1];
			}
			for (int j54 = 4; j54 > 0; j54 = j54 - 1) {
				fRec87[j54] = fRec87[j54 - 1];
			}
			for (int j55 = 4; j55 > 0; j55 = j55 - 1) {
				fRec86[j55] = fRec86[j55 - 1];
			}
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			for (int j56 = 4; j56 > 0; j56 = j56 - 1) {
				fRec89[j56] = fRec89[j56 - 1];
			}
			for (int j57 = 4; j57 > 0; j57 = j57 - 1) {
				fRec88[j57] = fRec88[j57 - 1];
			}
			for (int j58 = 4; j58 > 0; j58 = j58 - 1) {
				fRec93[j58] = fRec93[j58 - 1];
			}
			for (int j59 = 4; j59 > 0; j59 = j59 - 1) {
				fRec92[j59] = fRec92[j59 - 1];
			}
			fRec91[1] = fRec91[0];
			fRec90[1] = fRec90[0];
			for (int j60 = 4; j60 > 0; j60 = j60 - 1) {
				fRec95[j60] = fRec95[j60 - 1];
			}
			for (int j61 = 4; j61 > 0; j61 = j61 - 1) {
				fRec94[j61] = fRec94[j61 - 1];
			}
			for (int j62 = 4; j62 > 0; j62 = j62 - 1) {
				fRec99[j62] = fRec99[j62 - 1];
			}
			for (int j63 = 4; j63 > 0; j63 = j63 - 1) {
				fRec98[j63] = fRec98[j63 - 1];
			}
			fRec97[1] = fRec97[0];
			fRec96[1] = fRec96[0];
			for (int j64 = 4; j64 > 0; j64 = j64 - 1) {
				fRec101[j64] = fRec101[j64 - 1];
			}
			for (int j65 = 4; j65 > 0; j65 = j65 - 1) {
				fRec100[j65] = fRec100[j65 - 1];
			}
			for (int j66 = 4; j66 > 0; j66 = j66 - 1) {
				fRec105[j66] = fRec105[j66 - 1];
			}
			for (int j67 = 4; j67 > 0; j67 = j67 - 1) {
				fRec104[j67] = fRec104[j67 - 1];
			}
			fRec103[1] = fRec103[0];
			fRec102[1] = fRec102[0];
			for (int j68 = 4; j68 > 0; j68 = j68 - 1) {
				fRec107[j68] = fRec107[j68 - 1];
			}
			for (int j69 = 4; j69 > 0; j69 = j69 - 1) {
				fRec106[j69] = fRec106[j69 - 1];
			}
			for (int j70 = 4; j70 > 0; j70 = j70 - 1) {
				fRec111[j70] = fRec111[j70 - 1];
			}
			for (int j71 = 4; j71 > 0; j71 = j71 - 1) {
				fRec110[j71] = fRec110[j71 - 1];
			}
			fRec109[1] = fRec109[0];
			fRec108[1] = fRec108[0];
			for (int j72 = 4; j72 > 0; j72 = j72 - 1) {
				fRec113[j72] = fRec113[j72 - 1];
			}
			for (int j73 = 4; j73 > 0; j73 = j73 - 1) {
				fRec112[j73] = fRec112[j73 - 1];
			}
			for (int j74 = 4; j74 > 0; j74 = j74 - 1) {
				fRec117[j74] = fRec117[j74 - 1];
			}
			for (int j75 = 4; j75 > 0; j75 = j75 - 1) {
				fRec116[j75] = fRec116[j75 - 1];
			}
			fRec115[1] = fRec115[0];
			fRec114[1] = fRec114[0];
			for (int j76 = 4; j76 > 0; j76 = j76 - 1) {
				fRec119[j76] = fRec119[j76 - 1];
			}
			for (int j77 = 4; j77 > 0; j77 = j77 - 1) {
				fRec118[j77] = fRec118[j77 - 1];
			}
			for (int j78 = 4; j78 > 0; j78 = j78 - 1) {
				fRec123[j78] = fRec123[j78 - 1];
			}
			for (int j79 = 4; j79 > 0; j79 = j79 - 1) {
				fRec122[j79] = fRec122[j79 - 1];
			}
			fRec121[1] = fRec121[0];
			fRec120[1] = fRec120[0];
			for (int j80 = 4; j80 > 0; j80 = j80 - 1) {
				fRec125[j80] = fRec125[j80 - 1];
			}
			for (int j81 = 4; j81 > 0; j81 = j81 - 1) {
				fRec124[j81] = fRec124[j81 - 1];
			}
			for (int j82 = 4; j82 > 0; j82 = j82 - 1) {
				fRec129[j82] = fRec129[j82 - 1];
			}
			for (int j83 = 4; j83 > 0; j83 = j83 - 1) {
				fRec128[j83] = fRec128[j83 - 1];
			}
			fRec127[1] = fRec127[0];
			fRec126[1] = fRec126[0];
			for (int j84 = 4; j84 > 0; j84 = j84 - 1) {
				fRec131[j84] = fRec131[j84 - 1];
			}
			for (int j85 = 4; j85 > 0; j85 = j85 - 1) {
				fRec130[j85] = fRec130[j85 - 1];
			}
			for (int j86 = 4; j86 > 0; j86 = j86 - 1) {
				fRec135[j86] = fRec135[j86 - 1];
			}
			for (int j87 = 4; j87 > 0; j87 = j87 - 1) {
				fRec134[j87] = fRec134[j87 - 1];
			}
			fRec133[1] = fRec133[0];
			fRec132[1] = fRec132[0];
			for (int j88 = 4; j88 > 0; j88 = j88 - 1) {
				fRec137[j88] = fRec137[j88 - 1];
			}
			for (int j89 = 4; j89 > 0; j89 = j89 - 1) {
				fRec136[j89] = fRec136[j89 - 1];
			}
			for (int j90 = 4; j90 > 0; j90 = j90 - 1) {
				fRec141[j90] = fRec141[j90 - 1];
			}
			for (int j91 = 4; j91 > 0; j91 = j91 - 1) {
				fRec140[j91] = fRec140[j91 - 1];
			}
			fRec139[1] = fRec139[0];
			fRec138[1] = fRec138[0];
			for (int j92 = 4; j92 > 0; j92 = j92 - 1) {
				fRec143[j92] = fRec143[j92 - 1];
			}
			for (int j93 = 4; j93 > 0; j93 = j93 - 1) {
				fRec142[j93] = fRec142[j93 - 1];
			}
			for (int j94 = 4; j94 > 0; j94 = j94 - 1) {
				fRec147[j94] = fRec147[j94 - 1];
			}
			for (int j95 = 4; j95 > 0; j95 = j95 - 1) {
				fRec146[j95] = fRec146[j95 - 1];
			}
			fRec145[1] = fRec145[0];
			fRec144[1] = fRec144[0];
			for (int j96 = 4; j96 > 0; j96 = j96 - 1) {
				fRec149[j96] = fRec149[j96 - 1];
			}
			for (int j97 = 4; j97 > 0; j97 = j97 - 1) {
				fRec148[j97] = fRec148[j97 - 1];
			}
			for (int j98 = 4; j98 > 0; j98 = j98 - 1) {
				fRec153[j98] = fRec153[j98 - 1];
			}
			for (int j99 = 4; j99 > 0; j99 = j99 - 1) {
				fRec152[j99] = fRec152[j99 - 1];
			}
			fRec151[1] = fRec151[0];
			fRec150[1] = fRec150[0];
			for (int j100 = 4; j100 > 0; j100 = j100 - 1) {
				fRec155[j100] = fRec155[j100 - 1];
			}
			for (int j101 = 4; j101 > 0; j101 = j101 - 1) {
				fRec154[j101] = fRec154[j101 - 1];
			}
			for (int j102 = 4; j102 > 0; j102 = j102 - 1) {
				fRec159[j102] = fRec159[j102 - 1];
			}
			for (int j103 = 4; j103 > 0; j103 = j103 - 1) {
				fRec158[j103] = fRec158[j103 - 1];
			}
			fRec157[1] = fRec157[0];
			fRec156[1] = fRec156[0];
			for (int j104 = 4; j104 > 0; j104 = j104 - 1) {
				fRec161[j104] = fRec161[j104 - 1];
			}
			for (int j105 = 4; j105 > 0; j105 = j105 - 1) {
				fRec160[j105] = fRec160[j105 - 1];
			}
			for (int j106 = 4; j106 > 0; j106 = j106 - 1) {
				fRec165[j106] = fRec165[j106 - 1];
			}
			for (int j107 = 4; j107 > 0; j107 = j107 - 1) {
				fRec164[j107] = fRec164[j107 - 1];
			}
			fRec163[1] = fRec163[0];
			fRec162[1] = fRec162[0];
			for (int j108 = 4; j108 > 0; j108 = j108 - 1) {
				fRec167[j108] = fRec167[j108 - 1];
			}
			for (int j109 = 4; j109 > 0; j109 = j109 - 1) {
				fRec166[j109] = fRec166[j109 - 1];
			}
			for (int j110 = 4; j110 > 0; j110 = j110 - 1) {
				fRec171[j110] = fRec171[j110 - 1];
			}
			for (int j111 = 4; j111 > 0; j111 = j111 - 1) {
				fRec170[j111] = fRec170[j111 - 1];
			}
			fRec169[1] = fRec169[0];
			fRec168[1] = fRec168[0];
			for (int j112 = 4; j112 > 0; j112 = j112 - 1) {
				fRec173[j112] = fRec173[j112 - 1];
			}
			for (int j113 = 4; j113 > 0; j113 = j113 - 1) {
				fRec172[j113] = fRec172[j113 - 1];
			}
			for (int j114 = 4; j114 > 0; j114 = j114 - 1) {
				fRec177[j114] = fRec177[j114 - 1];
			}
			for (int j115 = 4; j115 > 0; j115 = j115 - 1) {
				fRec176[j115] = fRec176[j115 - 1];
			}
			fRec175[1] = fRec175[0];
			fRec174[1] = fRec174[0];
			for (int j116 = 4; j116 > 0; j116 = j116 - 1) {
				fRec179[j116] = fRec179[j116 - 1];
			}
			for (int j117 = 4; j117 > 0; j117 = j117 - 1) {
				fRec178[j117] = fRec178[j117 - 1];
			}
			for (int j118 = 4; j118 > 0; j118 = j118 - 1) {
				fRec183[j118] = fRec183[j118 - 1];
			}
			for (int j119 = 4; j119 > 0; j119 = j119 - 1) {
				fRec182[j119] = fRec182[j119 - 1];
			}
			fRec181[1] = fRec181[0];
			fRec180[1] = fRec180[0];
			for (int j120 = 4; j120 > 0; j120 = j120 - 1) {
				fRec185[j120] = fRec185[j120 - 1];
			}
			for (int j121 = 4; j121 > 0; j121 = j121 - 1) {
				fRec184[j121] = fRec184[j121 - 1];
			}
			for (int j122 = 4; j122 > 0; j122 = j122 - 1) {
				fRec189[j122] = fRec189[j122 - 1];
			}
			for (int j123 = 4; j123 > 0; j123 = j123 - 1) {
				fRec188[j123] = fRec188[j123 - 1];
			}
			fRec187[1] = fRec187[0];
			fRec186[1] = fRec186[0];
			for (int j124 = 4; j124 > 0; j124 = j124 - 1) {
				fRec191[j124] = fRec191[j124 - 1];
			}
			for (int j125 = 4; j125 > 0; j125 = j125 - 1) {
				fRec190[j125] = fRec190[j125 - 1];
			}
			for (int j126 = 4; j126 > 0; j126 = j126 - 1) {
				fRec195[j126] = fRec195[j126 - 1];
			}
			for (int j127 = 4; j127 > 0; j127 = j127 - 1) {
				fRec194[j127] = fRec194[j127 - 1];
			}
			fRec193[1] = fRec193[0];
			fRec192[1] = fRec192[0];
		}
	}

};

#endif
