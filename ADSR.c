
//  ADSR.c
//
//  Originally Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//  C Port 2013 Francesco D'Este

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

#include "ADSR.h"
#include <math.h>

 

void initADSR(ADSR* ptr){
    reset(ptr);
  
    setAttackRate(ptr,0);
    setDecayRate(ptr,0);
    setReleaseRate(ptr,0);
    setSustainLevel(ptr,1.0);
    setTargetRatioA(ptr,0.3);
    setTargetRatioDR(ptr,0.0001);
	
}

void destroyADSR(){

}


void setAttackRate(ADSR* ptr, float rate) {
    ptr->attackRate = rate;
    ptr->attackCoef = calcCoef(rate, ptr->targetRatioA);
    ptr->attackBase = (1.0 + ptr->targetRatioA) * (1.0 - ptr->attackCoef);
}

void setDecayRate(ADSR* ptr,float rate) {
    ptr->decayRate = rate;
    ptr->decayCoef = calcCoef(rate, ptr->targetRatioDR);
    ptr->decayBase = (ptr->sustainLevel - ptr->targetRatioDR) * (1.0 - ptr->decayCoef);
}

void setReleaseRate(ADSR* ptr, float rate) {
    ptr->releaseRate = rate;
    ptr->releaseCoef = calcCoef(rate, ptr->targetRatioDR);
    ptr->releaseBase = -ptr->targetRatioDR * (1.0 - ptr->releaseCoef);
}

float calcCoef(float rate, float targetRatio) {
    return exp(-log((1.0 + targetRatio) / targetRatio) / rate);
}

void setSustainLevel(ADSR* ptr, float level) {
    ptr->sustainLevel = level;
    ptr->decayBase = (ptr->sustainLevel - ptr->targetRatioDR) * (1.0 - ptr->decayCoef);
}

void setTargetRatioA(ADSR* ptr, float targetRatio) {
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    ptr->targetRatioA = targetRatio;
    ptr->attackBase = (1.0 + ptr->targetRatioA) * (1.0 - ptr->attackCoef);
}

void setTargetRatioDR(ADSR* ptr, float targetRatio) {
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    ptr->targetRatioDR = targetRatio;
    ptr->decayBase = (ptr->sustainLevel - ptr->targetRatioDR) * (1.0 - ptr->decayCoef);
    ptr->releaseBase = -ptr->targetRatioDR * (1.0 - ptr->releaseCoef);
}

float process(ADSR* ptr) {
	switch (ptr->state) {
        case env_idle:
            break;
        case env_attack:
            ptr->output = ptr->attackBase + ptr->output * ptr->attackCoef;
            if (ptr->output >= 1.0) {
                ptr->output = 1.0;
                ptr->state = env_decay;
            }
            break;
        case env_decay:
            ptr->output = ptr->decayBase + ptr->output * ptr->decayCoef;
            if (ptr->output <= ptr->sustainLevel) {
                ptr->output = ptr->sustainLevel;
                ptr->state = env_sustain;
            }
            break;
        case env_sustain:
            break;
        case env_release:
            ptr->output = ptr->releaseBase + ptr->output * ptr->releaseCoef;
            if (ptr->output <= 0.0) {
                ptr->output = 0.0;
                ptr->state = env_idle;
            }
	}
	return ptr->output;
}

void gate(ADSR* ptr) {
	if (ptr->gate == 0){
		ptr->state = env_attack;
		ptr->gate = 1;
	}
	else if (ptr->state != env_idle){
        ptr->state = env_release;
		ptr->gate = 0;
		}
}

int getState(ADSR* ptr) {
    return ptr->state;
}

void reset(ADSR* ptr) {
	ptr->gate = 0;
    ptr->state = env_idle;
    ptr->output = 0.0;
}

float getOutput(ADSR* ptr) {
	return ptr->output;
}

