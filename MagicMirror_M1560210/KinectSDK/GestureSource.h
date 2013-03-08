#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <iostream>


//[event_source(native)]
class GestureSource {
public:
	//constructors & destructors
	//GestureSource();
	//~GestureSource(void);

   __event void MyEvent(int nValue);
   __event void MyEvent2(char val);
   __event void GestureRecognised(int gestureVal);
};