#ifndef GLCD_H
#define GLCD_H

#include <Arduino.h>
#include <string.h>

class LCD{
public:
	void init();
	//void ~INIT();
	void clear();
	void print(int row, int col, String data);
};

#endif
