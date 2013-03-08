#pragma once

#include <stdio.h>
#include <iostream>
//#include <glut.h>
#include "GestureSource.h"

//[event_receiver(native)]
class GestureReceiver {
public:
	//constructors & destructors
	GestureReceiver();
	~GestureReceiver(void);

	//the handlers are supposed to do something when they are hooked to the event sources	
	void MyHandler1(int nValue);

	void MyHandler2(int nValue);

	void MyHandler3(char val);

	void ExecutionAfterDetection(int nValue);

	void hookEvent(GestureSource* pSource);

	void unhookEvent(GestureSource* pSource) ;
};