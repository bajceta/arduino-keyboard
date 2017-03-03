#include "Keyboard.h"
#include "Mouse.h"

/* #include <Keyboard.h> */

int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

int EMPTY=0;
int NOC=0;
byte ROWS = 6;
byte COLS = 5;
const byte max = 10;

const byte MAX_LAYER = 2;
const byte MBLEFT  = 1;
const byte MBRIGHT  = 2;
const byte MBMID  = 3;
const byte MLEFT  = 4;
const byte MRIGHT  = 5;
const byte MUP  = 6;
const byte MDOWN  = 7;

int rows[] = {5,4,3,2,1,0};
int cols[] = {A0,A1, A2,A3, A4};
const byte MOUSE_LAYER = 2;
char matrix[][5][6]= {
    {
        {'6','7','8','9','0',KEY_BACKSPACE},
        {'y','u','i','o','p',KEY_ESC},
        {'h','j','k','l',';',KEY_RETURN},
        {'n','m',',','.','/',KEY_LEFT_SHIFT},
        {KEY_RIGHT_ALT,KEY_RIGHT_CTRL,KEY_RETURN,KEY_ESC,'/',KEY_LEFT_SHIFT}
    },{
        {KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11},
        {'[',']', NOC, NOC, NOC, NOC},
        {KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW,'\'', '\\'},
        {'=','-','+',NOC,NOC,NOC},
        {NOC,NOC,NOC,NOC,NOC,NOC}
    },{
        {NOC,NOC,NOC,NOC,NOC,NOC},
        {NOC, MBLEFT, MBMID, MBRIGHT,NOC, NOC},
        {MLEFT, MDOWN, MUP, MRIGHT, NOC},
        {NOC,NOC,NOC,NOC,NOC,NOC},
        {NOC,NOC,NOC,NOC,NOC,NOC}
    }

};

char up=KEY_RIGHT_ALT;
char down=KEY_RIGHT_CTRL;
char l1=KEY_RIGHT_ALT;
char l2=KEY_RIGHT_CTRL;
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

boolean countPressed(int array[], byte size){
    byte count=0;

    for (int i = 0; i < size; i++) {
        if (array[i] != EMPTY) {
            count++;
        }
    }
    return count;
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
void handleMouseMove(char key){
    switch (key){
        case MLEFT:
            Mouse.move(-move,0,0);
            break;
        case MRIGHT:
            Mouse.move(move,0,0);
            break;
        case MUP:
            Mouse.move(0,-move,0);
            break;
        case MDOWN:
            Mouse.move(0,move,0);
            break;
    }
}
bool handleMousePress(char key){
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
void press_release(){
    int i;
    int key;

    String relpres="PressedNow: " ;
    /* Serial.println(relpres + key); */
    String rel="Press ";
    for (i=0;i<max;i++){
        key = pressed[i];
        if(key!=EMPTY){
            if (layer==MOUSE_LAYER) {
                handleMouseMove(key);
            }
            /* Serial.print(key); */
        }
        if(key!=EMPTY&&!contains(pressedNow, key, max)){
            if (layer==MOUSE_LAYER&&!handleMouseRelease(key)) {
            } else {
                String rel="Release ";
                Serial.println(rel + key);
                Keyboard.release(key);
            }
            if(key==l1||key==l2){
                layer=0;
            }
            /* if (single) { */
            /* if (key==up){ */
            /* layerUp(); */
            /* } else if (key==down){ */
            /* layerDown(); */
            /* } */
            /* } */
        }
    }
    for (i=0;i<max;i++){
        key = pressedNow[i];
        if(key!=EMPTY){
            /* Serial.print(key); */
        }
        if(key!=EMPTY&&!contains(pressed, key, max)){
            if (layer==MOUSE_LAYER&&!handleMousePress(key)) {
            } else {
                Serial.println(rel + key);
                if (key!=l1&&key!=l2) {
                    Keyboard.press(key);
                }
            }
            single = (i==0 && countPressed(pressedNow, max)==1) ;
            if (key==l1) {
                layer=1;
            } else if (key==l2){
                layer=2;
            }

        }
        pressed[i]=key;
    }
    /* Serial.println("pressed"); */

    for (i=0;i<max;i++){
        key = pressedNow[i];
        rel="PressedNow: " ;
        /* Serial.print(rel + key); */
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
            insert(pressedNow, key(col,row), max);
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
    Serial.println("Start keyboard scan");
    emptyArray(pressed, max);
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
    /* Serial.println("still testing"); */
}
