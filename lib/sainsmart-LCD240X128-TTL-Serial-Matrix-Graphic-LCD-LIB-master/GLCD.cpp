#include "GLCD.h"

const byte sta = 0x8A;
const byte stb = 0xA8;
const byte ff = 0xff;

void LCD::init(){
	Serial1.begin(9600);
}

//void LCD::~INIT(){}

void LCD::clear(){
	Serial1.write(sta);
	Serial1.write(stb);
	Serial1.write(ff);
	Serial1.write(ff);
}

void LCD::print(int row, int col, String data){
	int length = data.length();
	int modRow = row * 14; //13 as thats the length of the chars
	int modCol = col;
	//delay(20);
	Serial1.write(sta);
	Serial1.write(stb);
	Serial1.write(modRow);//row
	Serial1.write(modCol);//col
	Serial1.write(length);//string length
	Serial1.print(data);
	Serial.println(length);
}
