#include "Mouse.h"

#include <Keyboard.h>

int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

int EMPTY=0;
char EMPTY_SERIAL=101;

int NOC=0;

const byte LEFT_ROWS=6;
const byte LEFT_COLS=5;

byte ROWS = 6;
byte COLS = 5;
const byte max = 10;

const byte MAX_LAYER = 2;
const char MBLEFT  = 'u';
const char MBRIGHT  = 'o';
const char MBMID  = 'i';
const char MLEFT  = 'h';
const char MRIGHT  = 'l';
const char MUP  = 'k';
const char MDOWN  = 'j';

const char L1=1;
const char L2=2;
const char L1code=' ';
const char L2code=KEY_BACKSPACE;

int rows[] = {7,6,5,4,3,2};
int cols[] = {A0,A1,A2,A3,A4};
const byte MOUSE_LAYER = 2;
char matrix[][5][12]= {
	{
		{KEY_ESC,'1','2','3','4','5','6','7','8','9','0',KEY_BACKSPACE},
		{KEY_TAB,'q','w','e','r','t','y','u','i','o','p',KEY_ESC},
		{KEY_LEFT_CTRL,'a','s','d','f','g','h','j','k','l',';',KEY_RETURN},
		{KEY_LEFT_SHIFT,'z','x','c','v','b','n','m',',','.','/',KEY_LEFT_SHIFT},
		{L1,L2,KEY_RETURN,KEY_LEFT_ALT,KEY_LEFT_GUI,L1,L1,L2,KEY_RETURN,KEY_ESC,'/',KEY_LEFT_SHIFT}
	},{
		{KEY_F12,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11},
		{NOC,NOC,NOC,NOC,'[',']','[',']', '{','}', NOC, NOC},
		{NOC,NOC,NOC,NOC, '{','}',KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW,'\'', '\\'},
		{NOC,NOC,NOC,NOC,'(',')','=','-','+',NOC,NOC,NOC},
		{NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC}
	},{
		{NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC},
		{NOC,NOC,NOC,NOC,NOC,NOC,NOC, MBLEFT, MBMID, MBRIGHT,NOC, NOC},
		{NOC,NOC,NOC,NOC,NOC,NOC,MLEFT, MDOWN, MUP, MRIGHT, NOC},
		{NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC},
		{NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC,NOC}
	}
};
/* char matrix[][5][6]= { */
/* { */
/* {KEY_ESC,'1','2','3','4','5'}, */
/* {KEY_TAB,'q','w','e','r','t'}, */
/* {KEY_LEFT_CTRL,'a','s','d','f','g'}, */
/* {KEY_LEFT_SHIFT,'z','x','c','v','b'}, */
/* {L1,L2,KEY_RETURN,KEY_LEFT_ALT,KEY_LEFT_GUI,L1} */
/* },{ */
/* {KEY_F6,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5}, */
/* {'[',']', NOC, NOC, NOC, NOC},  */
/* {KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW,'\'', '\\'}, */
/* {'=','-','+',NOC,NOC,NOC}, */
/* {NOC,NOC,NOC,NOC,NOC,NOC} */
/* },{ */
/* {NOC,NOC,NOC,NOC,NOC,NOC}, */
/* {NOC, MBLEFT, MBMID, MBRIGHT,NOC, NOC}, */
/* {MLEFT, MDOWN, MUP, MRIGHT, NOC}, */
/* {NOC,NOC,NOC,NOC,NOC,NOC}, */
/* {NOC,NOC,NOC,NOC,NOC,NOC} */
/* } */
/* }; */

char up=KEY_RIGHT_ALT;
char down=KEY_RIGHT_CTRL;
byte layer=0;

char key(byte col,byte row){
	byte currentLayer=layer;
	char k = matrix[currentLayer][col][row];
	while (k==NOC&&currentLayer!=0){
		currentLayer--;
		k = matrix[currentLayer][col][row];
	}
	return k;
}

char pressed[max]={};
char pressedNow[max]={};
byte i;

void fillArray(char array[],int max, char val){
	for (i=0;i<max;i++){
		array[i]=val;
	}
}


void replace(char array[],char oldVal,char newVal, byte max){
	int i;
	for (i=0;i<max;i++){
		if(array[i]==oldVal){
			array[i]=newVal;
			break;
		}
	}
}

void insert(char array[],char val, byte max){
	replace(array,EMPTY,val,max);
}

void remove(char array[],char val, byte max){
	replace(array,val,EMPTY,max);
}

boolean contains(char array[], char element, byte size){
	for (int i = 0; i < size; i++) {
		if (array[i] == element) {
			return true;
		}
	}
	return false;
}

byte countPressed(char array[], byte size){
	byte count=0;
    
	for (int i = 0; i < size; i++) {
		if (array[i] != EMPTY) {
			count=count+1;
		}
	}
	return count;
}

char serialPressed[max] = {};

void handleSerial() {
	if (Serial1.available()) {
		char inByte = (char)Serial1.read();
		Serial.println(inByte,DEC);
		if (inByte<0) {
			/* remove  */
			replace(serialPressed, -inByte, EMPTY_SERIAL, max);
		} else {
			/* insert */
			replace(serialPressed, EMPTY_SERIAL, inByte,  max);
		}
	}
}

char serialKey(char val){
	char realVal = val-1;
	byte row=0;
	byte col=0;
	if (realVal!=0) {
		row = realVal % 10;
		col = realVal / 10;
	}
	return key(col,row);
}

void layerUp(){
	if (layer!=MAX_LAYER) {
		layer++;
		Serial.println("layer up");
	}
}

void layerDown(){
	if (layer>0){
		layer--;
		Serial.println("layer down");
	}
}
byte move=1;
bool isMouseMove(char key){
	if (layer!=MOUSE_LAYER) return false;
	switch (key){
		case MLEFT:
			return true;
		case MRIGHT:
			return true;
		case MUP:
			return true;
		case MDOWN:
			return true;
		default:
			return false;
	}
}
bool handleMouseMove(char key){
	if (layer!=MOUSE_LAYER) return false;
	switch (key){
		case MLEFT:
			Mouse.move(-move,0,0);
			return true;
		case MRIGHT:
			Mouse.move(move,0,0);
			return true;
		case MUP:
			Mouse.move(0,-move,0);
			return true;
		case MDOWN:
			Mouse.move(0,move,0);
			return true;
		default:
			return false;
	}
}
bool handleMousePress(char key){
	if (layer!=MOUSE_LAYER) return false;
	switch (key) {
		case MBLEFT:
			Mouse.press(MOUSE_LEFT);
			return true;
		case MBMID:
			Mouse.press(MOUSE_MIDDLE);
			return true;
		case MBRIGHT:
			Mouse.press(MOUSE_RIGHT);
			return true;
		default:
			return false;
	}
}
bool handleMouseRelease(char key){
	if (layer!=MOUSE_LAYER) return false;
	switch (key) {
		case MBLEFT:
			Mouse.release(MOUSE_LEFT);
			return true;
		case MBMID:
			Mouse.release(MOUSE_MIDDLE);
			return true;
		case MBRIGHT:
			Mouse.release(MOUSE_RIGHT);
			return true;
		default:
			return false;
	}
}

boolean single;

void handlePress(char key){
    String msg="count=  ";
    Serial.println(msg+countPressed(pressedNow, max));
	if (countPressed(pressedNow, max)==1){
		single=true;
	}  else single=false;
	String rel="Press: ";
	if (handleMousePress(key)||isMouseMove(key)) {
		Serial.println("mouse");
	} else {
		if (key!=L1&&key!=L2) {
			Serial.println(rel + key);
			Keyboard.press(key);
		}
	}
	if (key==L1) {
		Serial.println("Level 1");
		layer=1;
	} else if (key==L2){
		Serial.println("Level 2");
		layer=2;
	}
}

void handleHold(char key){
	handleMouseMove(key);
}

void handleRelease(char key){
	String dep="depress: ";
	/* Serial.println(dep + kk); */
	if (handleMouseRelease(key)||isMouseMove(key)) {
		Serial.println("mouse");
	} else {
		if(key==L1||key==L2){
			Serial.println("Back to level 0");
			layer=0;
			char code;
			if (single) {
				if (key==L1) {code=L1code;  }
				if (key==L2) {code=L2code; }
				String msg="Write ";
				Serial.println(msg + code);
				Keyboard.write(code);
			}
		} else {
			String rel="Release: ";
			Serial.println(rel + key);
			Keyboard.release(key);
		}
	}
}

void press_release(){
	int i;
	char key;

	for (i=0;i<max;i++){
		key = pressed[i];
		if (key!=EMPTY) {
			if (contains(pressedNow, key, max)){
				handleHold(key);
			} else {
				handleRelease(key);
			}
		}
	}
	for (i=0;i<max;i++){
		key = pressedNow[i];
		if(key!=EMPTY&&!contains(pressed, key, max)){
			handlePress(key);
		}
	}
	/* String rel="Single: "; */
	/* Serial.println(rel + single); */

	for (i=0;i<max;i++){
		key = pressedNow[i];
		pressed[i]=key;
	}
}

void pressSerial(){
	int i;
	for (i=0; i<max; i++){
		char input = serialPressed[i];
		if (input!=EMPTY_SERIAL) {
			insert(pressedNow, serialKey(input), max);
		}
	}
}

void scan(){
	byte col;
	byte i;
	fillArray(pressedNow,max,EMPTY);
	for (i = 0; i < COLS; i = i + 1) {
		scan_col(i);
	}
	/* Serial.println("scan"); */
	for(i=0;i<max;i++){
		/* Serial.print(pressedNow[i]); */
	}
	handleSerial();
	pressSerial();
	press_release();
}


void scan_col(byte col){
	byte row;
	byte i;
	byte colPin=cols[col];
	byte rowPin;
	pinMode(colPin, OUTPUT);      // sets the digital pin 13 as output
	digitalWrite(colPin, 0);
	for (row = 0; row < ROWS; row = row + 1) {
		rowPin=rows[row];
		bool pressed = !digitalRead(rowPin);
		if (pressed) {
			/* Serial.println(matrix[col][row]); */
			String msg = "col:";
			/* Serial.println(msg + col ); */
			msg = "row:";
			/* Serial.println(msg + row ); */
			insert(pressedNow, key(col,LEFT_ROWS+row), max);
		}
	}
	pinMode(colPin, INPUT);      // sets the digital pin 7 as input
	/* digitalWrite(colPin, 1); */

}

void setup()
{

	int i;
	byte pin;
	for (i = 0; i < COLS; i = i + 1) {
		pin=cols[i];
		pinMode(pin, OUTPUT);      // sets the digital pin 13 as output
	}
	for (i = 0; i < ROWS; i = i + 1) {
		pin=rows[i];
		pinMode(pin, INPUT_PULLUP);      // sets the digital pin 7 as input
	}
	/* pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output */
	Serial.begin(9600);           // set up Serial library at 9600 bps
	Serial1.begin(9600);           // set up Serial library at 9600 bps
	Serial.println("Start keyboard scan");
	fillArray(pressed, max, EMPTY);
	fillArray(serialPressed, max, EMPTY_SERIAL);
	Keyboard.begin();
	Mouse.begin();
}


void loop()
{
	/* boolean led = false; */
	/* led = !led; */
	/* digitalWrite(ledPin, led);    // sets the LED to the button's value */
	scan();
	delay(3);
	/* Seial.println("still testing"); */
}

