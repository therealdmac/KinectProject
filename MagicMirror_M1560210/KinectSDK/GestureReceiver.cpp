#include "stdafx.h"
// evh_native.cpp
#include <stdio.h>
#include <iostream>

#include "GestureReceiver.h"
#include "stdafx.h"

//constructor
GestureReceiver::GestureReceiver()
{
}

//destructor
GestureReceiver::~GestureReceiver(void)
{
}

//these methods do the event handling and carry out the actions inside the methods

void GestureReceiver::MyHandler1(int nValue) {
	if (nValue = 123) {
		printf_s("MyHandler1 was called with value %d.\n", nValue);
	}
	else {
		printf_s("MyHandler1 was called with other value %d.\n");
	}
}

void GestureReceiver::MyHandler2(int nValue) {
	if (nValue = 123) {
		printf_s("MyHandler2 was called with value %d.\n", nValue);
	}
	else {
		printf_s("MyHandler2 was called with other value %d.\n");
	}
}

void GestureReceiver::ExecutionAfterDetection(int gestureVal) {
	//run through the values input to detect what gesture it is
	//
	if (gestureVal == 1) {
		//testing only
		//glPushMatrix();
		//glRotatef(3.0f,0.0f,0.0f,1.0f);
		//glPopMatrix();
		printf("I got a 1 %d. \n", gestureVal);
		//printf("frame rate is %lg\n");
		//system("pause");
	} else {
		printf_s("for testing other gestures %d. \n", gestureVal);
		//system("pause");
	}
}

void GestureReceiver::MyHandler3(char val) {
	printf_s("MyHandler3 was called with value %d.\n", val);
	system("pause");
}

void GestureReceiver::hookEvent(GestureSource* pSource) {
      __hook(&GestureSource::MyEvent, pSource, &GestureReceiver::MyHandler1);
      __hook(&GestureSource::MyEvent, pSource, &GestureReceiver::MyHandler2);
	  __hook(&GestureSource::MyEvent2, pSource, &GestureReceiver::MyHandler3);
	  __hook(&GestureSource::GestureRecognised, pSource, &GestureReceiver::ExecutionAfterDetection);
}

void GestureReceiver::unhookEvent(GestureSource* pSource) {
      __unhook(&GestureSource::MyEvent, pSource, &GestureReceiver::MyHandler1);
      __unhook(&GestureSource::MyEvent, pSource, &GestureReceiver::MyHandler2);
	  __unhook(&GestureSource::MyEvent2, pSource, &GestureReceiver::MyHandler3);
	  __unhook(&GestureSource::GestureRecognised, pSource, &GestureReceiver::ExecutionAfterDetection);
}

