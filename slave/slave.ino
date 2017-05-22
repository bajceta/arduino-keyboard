
byte ROWS = 7;
byte COLS = 5;
const byte max = 10;
const char EMPTY = 126;

int rows[] = {8,7,6,5,4,3,2};
int cols[] = {A0,A1, A2,A3, A4};

char pressed[max]={};
char pressedNow[max]={};
byte i;
void emptyArray(char array[],int max){
    for (i=0;i<max;i++){
        array[i]=EMPTY;
    }
}

void insert(char array[],char val, byte max){
    int i;
    for (i=0;i<max;i++){
        if(array[i]==EMPTY){
            array[i]=val;
            break;
        }
    }
}

boolean contains(char array[], char element, byte size){
    for (int i = 0; i < size; i++) {
        if (array[i] == element) {
            return true;
        }
    }
    return false;
}

void press_release(){
    int i;
    char key;

    String relpres="PressedNow: " ;
    /* Serial.println(relpres + key); */
    String rel="Press ";

    /* Release keys */
    for (i=0;i<max;i++){
        key = pressed[i];
        if(key!=EMPTY&&!contains(pressedNow, key, max)){
            String rel="Release :";
            Serial.write(-key);
            /* Serial.println(rel + key); */
        }
    }

    /* Press keys  */
    for (i=0;i<max;i++){
        key=pressedNow[i];
        if(key!=EMPTY&&!contains(pressed, key, max)){
            /* String pre="press:  "; */
            String rel="Press :";
            Serial.write(key);
            /* Serial.println(rel + key); */
        }
    }

    /* Save currently pressed keys */
    for (i=0;i<max;i++){
        key = pressedNow[i];
        /* rel="PressedNow: " ; */
        /* Serial.print(rel + key); */
        pressed[i]=key;
    }
}

char key(int col, int row){
    return 10*col + row+1;
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
    pinMode(colPin, OUTPUT);
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
    /* Serial1.begin(9600);           // set up Serial library at 9600 bps */
    Serial.begin(9600);           // set up Serial library at 9600 bps

    /* Serial.println("Start keyboard scan"); */
    emptyArray(pressed, max);
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
