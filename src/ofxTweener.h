/*
 *  ofxTweener.h
 *  openFrameworks
 *
 *  Created by Sander ter Braak on 26-08-10.
 *
 */

 // Взято из openFrameworks
 // Автор Sander ter Braak
 // Модифицированно мною, под этот движок

//#include "ofMain.h"
#include "stdafx.h"
#include <vector>
#include "ofxTransitions.h"
#include "Timestamp.h"

#ifndef _OFXTWEEN
#define _OFXTWEEN

#define TWEENMODE_OVERRIDE 0x01
#define TWEENMODE_SEQUENCE 0x02

class Tween {
public:
	typedef float(ofxTransitions::* easeFunction)(float,float,float,float);
	float* _var;
	float _from, _to, _duration,_by, _useBezier;
	easeFunction _easeFunction;
	Timestamp _timestamp;
};


class ofxTweener /*: public ofBaseApp */{

public:
	
	ofxTweener();
	
	void addTween(float &var, float to, float time);
	void addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float));
	void addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay);
	void addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, float bezierPoint);
	
	void removeTween(float &var);	
	void setTimeScale(float scale);
	void update();
	void removeAllTweens();	
	void setMode(int mode);
	
	int getTweenCount();	
	
	
private:
	float				_scale;
	ofxTransitions		a;
	bool				_override;
	void				addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, float bezierPoint, bool useBezier);
	float				bezier(float b, float e, float t, float p);
	std::vector<Tween>		tweens;
};


extern ofxTweener Tweener;
#endif