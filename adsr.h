//
//  ADRS.h
//
//  Originally Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//  C Port by Francesco D'Este
//
//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code for your own purposes, free or commercial.
//

#ifndef ADRS_h
#define ADRS_h

    enum envState {
        env_idle = 0,
        env_attack,
        env_decay,
        env_sustain,
        env_release
    };
	
	 typedef struct {
    int state;
	float output;
	float attackRate;
	float decayRate;
	float releaseRate;
	float attackCoef;
	float decayCoef;
	float releaseCoef;
	float sustainLevel;
    float targetRatioA;
    float targetRatioDR;
    float attackBase;
    float decayBase;
    float releaseBase;
	int gate;

	} ADSR;

 
    float calcCoef(float rate, float targetRatio);
	
	float process(ADSR* ptr);
    float getOutput(ADSR* ptr);
    int getState(ADSR* ptr);
	void gate(ADSR* ptr);
    void setAttackRate(ADSR* ptr, float rate);
    void setDecayRate(ADSR* ptr, float rate);
    void setReleaseRate(ADSR* ptr, float rate);
	void setSustainLevel(ADSR* ptr, float level);
    void setTargetRatioA(ADSR* ptr, float targetRatio);
    void setTargetRatioDR(ADSR* ptr, float targetRatio);
    void reset(ADSR* ptr);





#endif
