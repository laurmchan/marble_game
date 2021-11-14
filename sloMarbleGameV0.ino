#include <LiquidCrystal.h>

// LCD Inputs
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Rotary Encoder Inputs
#define CLK 10
#define DT 9
#define SW 8

int tilt = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
int target = 9999999;

int posCount = 7;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

void setup() {

  //LCD init
  lcd.begin(16, 2);
  lcd.setCursor(7,1); //first num LR position, second num row position (0 first row)
  lcd.print("O");

  //Encoder init
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  startMillis = millis();
}

void loop() {
  int prevPosCount = posCount;
    // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      tilt --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      tilt ++;
      currentDir ="CW";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(tilt);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= 500/abs(tilt))  //test whether the period has elapsed
  {
      if(tilt>0){ //ball rolls right
        posCount+=1;
      }

      if(tilt<0){ //ball rolls left
        posCount-=1;
      }  //if so, change the state of the LED.  Uses a neat trick to change the state
      
      startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }

  if(prevPosCount != posCount){
    lcd.clear();
    lcd.setCursor(posCount,1); //first num LR position, second num row position (0 first row)
    lcd.print("O");
    lcd.setCursor(0,0);
    lcd.print("Pos: ");
    lcd.print(posCount-7);

  }
  Serial.println(posCount);

}
