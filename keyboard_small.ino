#include "Keyboard.h"

/* #include <Keyboard.h> */

int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

byte ROWS = 4;
byte COLS = 4;
const byte max = 10;

int rows[] = {2, 3, 4, 5};
int cols[] = {6, 7, 8, 9};
char matrix[][4]= {
    {'a','b','c',KEY_LEFT_ALT },
    {'1','2','3','4' },
    {'5','6','7','8' },
    {'9','0','+','=' }
};



int EMPTY=0;
int pressed[max]={};
int pressedNow[max]={};
int i;
void emptyArray(int array[],int max){
    for (i=0;i<max;i++){
        array[i]=EMPTY;
    }
}

void insert(int array[],int val, byte max){
    int i;
    for (i=0;i<max;i++){
        if(array[i]==EMPTY){
            array[i]=val;
            break;
        }
    }
}

boolean contains(int array[], int element, byte size){
    for (int i = 0; i < size; i++) {
        if (array[i] == element) {
            return true;
        }
    }
    return false;
}


void press_release(){
    int i;
    int key;

    for (i=0;i<max;i++){
        key = pressed[i];
        if(key!=EMPTY){
            Serial.print(key);
        }
        if(key!=EMPTY&&!contains(pressedNow, key, max)){
            Keyboard.release(key);
            String rel="Release ";
            Serial.println(rel + key);
        }
    }
    for (i=0;i<max;i++){
        key = pressedNow[i];
        if(key!=EMPTY){
            Serial.print(key);
        }
        if(key!=EMPTY&&!contains(pressed, key, max)){
            String rel="Press ";
            Serial.println(rel + key);
            Keyboard.press(key);
        }
        pressed[i]=key;
    }
    for (i=0;i<max;i++){
        key = pressedNow[i];
        pressed[i]=key;
    }
}

void scan(){
    int col;
    int i;
    emptyArray(pressedNow,max);
    for (i = 0; i < COLS; i = i + 1) {
        scan_col(i);
    }
    /* Serial.println("scan"); */
    for(i=0;i<max;i++){
        /* Serial.print(pressedNow[i]); */
    }
    press_release();
}

void scan_col(int col){
    int row;
    int i;
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
            insert(pressedNow, matrix[row][col],max);
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
    pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Start keyboard scan");
    emptyArray(pressed, max);
    Keyboard.begin();

}

void loop()
{
    boolean led = false;
    led = !led;
    digitalWrite(ledPin, led);    // sets the LED to the button's value
    scan();
    delay(100);
    /* Serial.println("still testing"); */
}
