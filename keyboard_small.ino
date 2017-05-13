#include "Mouse.h"

#include <Keyboard.h>

int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

int EMPTY = 126;
char EMPTY_SERIAL = 101;

int NOC = 0;

uint8_t lastPosPress;

const byte max = 10;

const byte MAX_LAYER = 3;
const char MBLEFT  = 'u';
const char MBRIGHT  = 'o';
const char MBMID  = 'i';
const char MLEFT  = 'h';
const char MRIGHT  = 'l';
const char MUP  = 'k';
const char MDOWN = 'j';
const char MFAST = ';';
const char MLATCH = '/';

const char L1 = 1;
const char L2 = 2;
const char L3 = 3;
const char MUTE = 121;
const char VDOWN = 122;
const char VUP = 123;
const char MICMUTE = 198;
const byte EMPTY_POS = 254;
uint8_t pressedKey[] = {};
const byte LEFT_ROWS = 6;
const byte LEFT_COLS = 5;
const byte ROWS = 6;
const byte COLS = 4;
const byte ROWS_TOTAL=12;
int rows[] = {7, 6, 5, 4, 3, 2};
int cols[] = {A1, A2, A3, A4};
const byte MOUSE_LAYER = 2;

uint8_t momentary[] = {
    NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC,
    KEY_ESC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC,
    NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC,
    NOC, NOC, NOC, NOC, KEY_ESC, ' ', ' ', KEY_TAB, NOC, NOC, NOC, NOC
};
uint8_t matrix[][48] = {
    {
        KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', KEY_BACKSPACE,
        KEY_LEFT_CTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', KEY_RETURN,
        KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', KEY_LEFT_SHIFT,
        L2, L2, L2, KEY_LEFT_ALT, KEY_LEFT_GUI, L1, L2, KEY_LEFT_SHIFT, KEY_RETURN, KEY_ESC, L3, L3
    }, {
        NOC, '!', '"', '\'', '[', ']', KEY_HOME, KEY_PAGE_DOWN, KEY_PAGE_UP, KEY_END, '#', NOC,
        NOC, '@', '\\', '%', '{', '}', KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW, '$', NOC,
        NOC, '^', '&', '|', '(', ')', '=', '-', '+', '_', '~', NOC,
        NOC, NOC, NOC, NOC, NOC,  NOC, NOC, NOC, NOC, NOC, NOC, NOC
    },{
        NOC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', KEY_DELETE,
        NOC, '4', '5', '6', '-', '+', MLEFT, MDOWN, MUP, MRIGHT, MFAST, NOC,
        NOC, '7', '8', '9', '0', '*', MBLEFT, MBMID, MBRIGHT, NOC, NOC, NOC,
        NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC
    }, {
        KEY_F12,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,
        NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC,
        NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC,
        NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC, NOC
    }
};

byte layer = 0;

uint8_t keycode(char pos) {
    byte currentLayer = layer;
    char k = matrix[currentLayer][pos];
    while (k == NOC && currentLayer != 0) {
        currentLayer--;
        k = matrix[currentLayer][pos];
    }
    return k;
}

// keep track of keycode for position mapping
uint8_t pressedKeycode[max] = {};
uint8_t pressedPos[max] = {};


// track pressed and released keys
uint8_t pressed[max] = {};
uint8_t pressedNow[max] = {};
byte i;



void fillArray(char array[], int max, char val) {
    for (i = 0; i < max; i++) {
        array[i] = val;
    }
}


int replace(char array[], char oldVal, char newVal, byte max) {
    int i;
    for (i = 0; i < max; i++) {
        if (array[i] == oldVal) {
            array[i] = newVal;
            return i;
        }
    }
}

int insert(char array[], char val, byte max) {
    return replace(array, EMPTY, val, max);
}

int remove(char array[], char val, byte max) {
    return replace(array, val, EMPTY, max);
}

int indexOf(char array[], char element, byte size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == element) {
            return i;
        }
    }
    return -1;
}
boolean contains(char array[], char element, byte size) {
    return indexOf(array,element,size)>-1;
}

byte countPressed(char array[], byte size) {
    byte count = 0;

    for (int i = 0; i < size; i++) {
        if (array[i] != EMPTY) {
            count = count + 1;
        }
    }
    return count;
}

char serialPressed[max] = {};

void handleSerial() {
    if (Serial1.available()) {
        char inByte = (char)Serial1.read();
        Serial.println(inByte, DEC);
        if (inByte < 0) {
            /* remove  */
            replace(serialPressed, -inByte, EMPTY_SERIAL, max);
        } else {
            /* insert */
            replace(serialPressed, EMPTY_SERIAL, inByte,  max);
        }
    }
}


char serialPos(char val) {
    char realVal = val - 1;
    byte row = 0;
    byte col = 0;
    if (realVal != 0) {
        row = realVal % 10;
        col = realVal / 10;
    }
    return col*ROWS_TOTAL+row;
}

void layerUp() {
    if (layer != MAX_LAYER) {
        layer++;
        Serial.println("layer up");
    }
}

void layerDown() {
    if (layer > 0) {
        layer--;
        Serial.println("layer down");
    }
}
byte move = 1;
bool isMouseMove(char key) {
    if (layer != MOUSE_LAYER) return false;
    switch (key) {
        case MLEFT:
            return true;
        case MRIGHT:
            return true;
        case MUP:
            return true;
        case MDOWN:
            return true;
        case MFAST:
            return true;
        case MLATCH:
            return true;
        default:
            return false;
    }
}
char lastkey=EMPTY;
int presslength=0;
byte skipcount=0;
byte skip;
bool mouseLatch=false;

void toggleFast(){
    if (skip==9) {
        skip=1;
    } else {
        skip=9;
    }
}

bool handleMouseMove(char key) {
    if (layer != MOUSE_LAYER) return false;
    if (key!=MLEFT&&key!=MDOWN&&key!=MRIGHT&&key!=MUP&&key!=MFAST&&key!=MLATCH) return false;
    if (skipcount<skip) {
        skipcount++;
        return false;
    } else  {
        skipcount=0;
    }

    switch (key) {
        case MLEFT:
            Mouse.move(-move, 0, 0);
            return true;
        case MRIGHT:
            Mouse.move(move, 0, 0);
            return true;
        case MUP:
            Mouse.move(0, -move, 0);
            return true;
        case MDOWN:
            Mouse.move(0, move, 0);
            return true;
        default:
            return false;
    }
}

bool ml=false;
bool mm=false;
bool mr=false;
void toggleMouseLatch(){
    mouseLatch=!mouseLatch;
    ml=false;
    mm=false;
    mr=false;
}

bool handleMousePress(char key) {
    if (layer != MOUSE_LAYER) return false;
    switch (key) {
        case MBLEFT:
            if (!ml) {
                Mouse.press(MOUSE_LEFT);
                ml=true;
            }
            return true;
        case MBMID:
            if (!mm) {
                Mouse.press(MOUSE_MIDDLE);
                mm=true;
            }
            return true;
        case MBRIGHT:
            if (!mr) {
                Mouse.press(MOUSE_RIGHT);
                mr=true;
            }
            return true;
        default:
            return false;
    }
}

bool handleMouseRelease(char key) {
    if (layer != MOUSE_LAYER) return false;
    switch (key) {
        case MBLEFT:
            if (ml){
                Mouse.release(MOUSE_LEFT);
                ml=false;
            }
            return true;
        case MBMID:
            if (mm){
                Mouse.release(MOUSE_MIDDLE);
                mm=false;
            }
            return true;
        case MBRIGHT:
            if (mr) {
                Mouse.release(MOUSE_RIGHT);
                mr=false;
            }
            return true;
        case MLATCH:
            toggleMouseLatch();
            return true;
        case MFAST:
            toggleFast();
            return true;
        default:
            return false;
    }
}

boolean single;
void handlePress(char pos) {
    lastPosPress=pos;
    String msg = "count=  ";
    Serial.println(msg + countPressed(pressedNow, max));
    if (countPressed(pressedNow, max) == 1) {
        single = true;
    }  else single = false;
    String rel = "Press: ";
    uint8_t key = keycode(pos);
    if (handleMousePress(key) || isMouseMove(key)) {
        Serial.println("mouse");
    } else {
        if (key != L1 && key != L2 && key != L3) {
            Serial.println(rel + key);
            if (!contains(pressedKeycode, key, max)){
                int index = insert(pressedPos, pos, max);
                pressedKeycode[index]=key;
                Keyboard.press(key);
            }
        }
    }
    if (key == L1) {
        Serial.println("Level 1");
        if (layer==2) {
            layer = 3;
        } else {
            layer = 1;
        }
    } else if (key == L2) {
        Serial.println("Level 2");
        if (layer==1) {
            layer = 3;
        } else {
            layer = 2;
        }
    } else if (key == L3) {
        Serial.println("Level 3");
        layer = 3;
    }

}

void handleHold(char key) {
    handleMouseMove(keycode(key));
}

void handleSinglePress(char pos){
    if (pos==lastPosPress){
        uint8_t key = momentary[pos];
        if (key!=NOC) {
            Keyboard.write(key);
        }
    }
}

void handleRelease(char pos) {
    String dep = "depress: ";
    uint8_t key;
    /* Serial.println(dep + kk); */
    int index = indexOf(pressedPos, pos, max);
    if (index>-1){
        key = pressedKeycode[index];
        pressedPos[index]=EMPTY;
        pressedKeycode[index]=NOC;
        if(key!=NOC){
            String rel = "Release: ";
            Serial.println(rel + key);
            Keyboard.release(key);
        }
    }
    if (key==NOC||index==-1){
        key=keycode(pos);
    }
    if (handleMouseRelease(key) || isMouseMove(key)) {
        Serial.println("mouse");
    } else {
        if (key == L1 || key == L2 || key == L3) {
            Serial.println("Back to level 0");
            layer = 0;
        }
        handleSinglePress(pos);
    }
}

char debounce[max][2];
byte DEBOUNCE_CYCLES=4;

void addDebounce(char key){
    byte i;
    bool found=false;
    for(i=0;i<max;i++){
        if (debounce[i][0]==key) {
            found=true;
            break;
        }
    }
    if (!found) {
        for(i=0;i<max;i++){
            if (debounce[i][0]==EMPTY) {
                debounce[i][0]=key;
                debounce[i][1]=0;
                break;
            }
        }

    }
}

void debounced(){
    byte i;
    char key;

    for(i=0;i<max;i++){
        if (debounce[i][0]!=EMPTY) {
            debounce[i][1]=debounce[i][1]+1;
            if (debounce[i][1]==DEBOUNCE_CYCLES) {
                key=debounce[i][0];
                handleRelease(key);
                debounce[i][0]=EMPTY;
            }
        }
    }
}

bool isInDebounce(char key){
    byte i;

    for(i=0;i<max;i++){
        if (debounce[i][0]==key) {
            return true;
        }
    }
    return false;
}


void debounceInit(){
    byte i;

    for(i=0;i<max;i++){
        debounce[i][0]=EMPTY;
    }
}

void press_release() {
    int i;
    char key;

    for (i = 0; i < max; i++) {
        key = pressed[i];
        if (key != EMPTY) {
            if (contains(pressedNow, key, max)) {
                handleHold(key);
            } else {
                addDebounce(key);
            }
        }
    }
    for (i = 0; i < max; i++) {
        key = pressedNow[i];
        if (key != EMPTY && !contains(pressed, key, max)) {
            if (!isInDebounce(key))
                handlePress(key);
        }
    }
    debounced();
    /* String rel="Single: "; */
    /* Serial.println(rel + single); */

    for (i = 0; i < max; i++) {
        key = pressedNow[i];
        pressed[i] = key;
    }
}

void pressSerial() {
    int i;
    for (i = 0; i < max; i++) {
        char input = serialPressed[i];
        if (input != EMPTY_SERIAL) {
            insert(pressedNow, serialPos(input), max);
        }
    }
}

void scan() {
    byte col;
    byte i;
    fillArray(pressedNow, max, EMPTY);
    for (i = 0; i < COLS; i = i + 1) {
        scan_col(i);
    }
    /* Serial.println("scan"); */
    for (i = 0; i < max; i++) {
        /* Serial.print(pressedNow[i]); */
    }
    handleSerial();
    pressSerial();
    press_release();
}


void scan_col(byte col) {
    byte row;
    byte i;
    byte colPin = cols[col];
    byte rowPin;
    pinMode(colPin, OUTPUT);      // sets the digital pin 13 as output
    digitalWrite(colPin, 0);
    for (row = 0; row < ROWS; row = row + 1) {
        rowPin = rows[row];
        bool pressed = !digitalRead(rowPin);
        if (pressed) {
            /* Serial.println(matrix[col][row]); */
            String msg = "col:";
            /* Serial.println(msg + col ); */
            msg = "row:";
            /* Serial.println(msg + row ); */
            insert(pressedNow, col*ROWS_TOTAL + LEFT_ROWS + row, max);
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
        pin = cols[i];
        pinMode(pin, OUTPUT);      // sets the digital pin 13 as output
    }
    for (i = 0; i < ROWS; i = i + 1) {
        pin = rows[i];
        pinMode(pin, INPUT_PULLUP);      // sets the digital pin 7 as input
    }
    /* pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output */
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial1.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Start keyboard scan");
    fillArray(pressed, max, EMPTY);
    fillArray(serialPressed, max, EMPTY_SERIAL);
    fillArray(pressedKeycode, max, NOC);
    fillArray(pressedPos, max, EMPTY);
    Keyboard.begin();
    Mouse.begin();
    debounceInit();
}


void loop()
{
    /* boolean led = false; */
    /* led = !led; */
    /*digitalWrite(ledPin, led);    // sets the LED to the button's value */
    scan();
    delay(1);
    /* Seial.println("still testing"); */
}

