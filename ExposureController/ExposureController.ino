#include <stdio.h>
#include <Wire.h>                        //libraries needed
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "MenuBackend.h"    //MenuBackend library - copyright by Alexander Brevig


//I2C Controller
#define I2C_ADDR    0x20  // Define I2C Address for controller
#define BACKLIGHT_PIN  7
#define En_pin  4
#define Rw_pin  5
#define Rs_pin  6
#define D4_pin  0
#define D5_pin  1
#define D6_pin  2
#define D7_pin  3
#define  LED_OFF  0
#define  LED_ON  1

//LEDs
#define RED_LED 11
#define UV_BOTTOM_LED 10
#define UV_TOP_LED 9
 
//LiquidCrystal_I2C  lcd(I2C_ADDR);//,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


const int button1 = A0;           // start/green/right
const int button2 = A1;           // seconds/white/up
const int button3 = A2;           // minutes/white/down
const int button4 = A3;           // clear/red/left
//const int lightBoardPin = 9;
const int buzzerPin = 8;
const int ledPin = 13;             

int minutes = 0;   
int seconds = 0; 
int countdown = 0;       
boolean startCountdown = false;
int logic1, logic2, logic3, logic4;
int currentMenu;
boolean redLedState = false;
boolean bottomUVEnabled = true;
boolean topUVEnabled = true;
boolean beepEnabled = true;

// LCDMENU
const int buttonPinLeft = button2;      // pin for the Up button
const int buttonPinRight = button3;    // pin for the Down button
const int buttonPinEsc = button4;     // pin for the Esc button
const int buttonPinEnter = button1;   // pin for the Enter button

int lastButtonPushed = 0;
int lastButtonUsed = 99;

int lastButtonState = 1;   // the previous reading from the button input pin
int lastButtonEnterState = LOW;   // the previous reading from the Enter input pin
int lastButtonEscState = LOW;   // the previous reading from the Esc input pin
int lastButtonLeftState = LOW;   // the previous reading from the Left input pin
int lastButtonRightState = LOW;   // the previous reading from the Right input pin


long lastDebounceTime = 0;  // the last time the output pin was toggled
long lastEnterDebounceTime = 0;  // the last time the output pin was toggled
long lastEscDebounceTime = 0;  // the last time the output pin was toggled
long lastLeftDebounceTime = 0;  // the last time the output pin was toggled
long lastRightDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 500;    // the debounce time

//Menu variables
MenuBackend menu = MenuBackend(menuUsed,menuChanged);
//initialize menuitems
    MenuItem mi1 = MenuItem("RedLeds",                  "Toggle Red Leds ");
    MenuItem mi2 = MenuItem("DefaultExpose",            "Default Exposure");
    MenuItem mi3 = MenuItem("CustomExpose",             "Custom Exposure ");
    MenuItem mi4 = MenuItem("ProfileExpose",            "Profile Exposure");
    MenuItem mi4_i1 = MenuItem("Profile1",              "Profile 1: 1 min");
    MenuItem mi4_i2 = MenuItem("Profile2",              "Profile 2: 10min");
    MenuItem mi4_i3 = MenuItem("Profile3",              "Profile 3: 30sec");
    MenuItem mi5 = MenuItem("Setup",                    "Setup           ");
    MenuItem mi5_i1 = MenuItem("Beep",                  "Toggle Beep     ");
    MenuItem mi5_i2 = MenuItem("UVMode",                "UV Led Mode     ");
    MenuItem mi5_i2_i1 = MenuItem("UVBoth",             "Top and Bottom  ");
    MenuItem mi5_i2_i2 = MenuItem("UVBottom",           "Bottom Only     ");
    MenuItem mi5_i2_i3 = MenuItem("UVTop",              "Top Only        ");



void setup()
{ 
  pinMode(ledPin, OUTPUT);
  pinMode(UV_TOP_LED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(UV_BOTTOM_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(UV_TOP_LED, LOW);
  digitalWrite(UV_BOTTOM_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(ledPin, LOW);

  lcd.begin(16, 2); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-UV Exposure v1-");
//  lcd.print("-Exposure Time-"); 
  lcd.setCursor(0, 1);
  lcd.print("- Press SETUP -");
  Serial.begin(9600);
  
  //configure menu

  // Version 2 menu
  menu.getRoot().addFirstChild(mi1).addNextSibling(mi2).addNextSibling(mi3).addNextSibling(mi4).addNextSibling(mi5);
  mi4.addFirstChild(mi4_i1).addNextSibling(mi4_i2).addNextSibling(mi4_i3);
  mi5.addFirstChild(mi5_i1).addNextSibling(mi5_i2);
  mi5_i2.addFirstChild(mi5_i2_i1).addNextSibling(mi5_i2_i2).addNextSibling(mi5_i2_i3);



  menu.toRoot();
  
  buzzer(1,0);
}

void UVLedsOn(boolean switchOn)
{
  if (switchOn == true)
  {
    digitalWrite(ledPin, HIGH);
    if (bottomUVEnabled == true) digitalWrite(UV_BOTTOM_LED, HIGH);
    if (topUVEnabled == true) digitalWrite(UV_TOP_LED, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(UV_BOTTOM_LED, LOW);
    digitalWrite(UV_TOP_LED, LOW);
  }
}


void ToggleRedLeds()
{
  RedLedsOn(not(redLedState));
}

void ToggleBeep()
{
//  beepEnabled=not(beepEnabled);
  if (beepEnabled) { beepEnabled = false; } else {beepEnabled = true; };
}

void RedLedsOn(boolean switchOn)
{
  if (switchOn == true)
  {
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(RED_LED, LOW);
  }
  redLedState=switchOn;
}

void splashscreen()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" To begin"); 
    lcd.setCursor(0, 1);
    lcd.print("* Press SETUP *"); 
}

void buzzer(int numBeep, int repeatCount)
{
  if (beepEnabled) 
  {
    for(int x = 0; x <= repeatCount; x++)
    {
      for(int y = 0; y < numBeep; y++)
      {
         tone(buzzerPin, 1500, 100);
         delay(200);
      }
    }
  } else {
         tone(buzzerPin, 1500, 1);
         delay(200);
  }
}

void Expose()
{
  boolean prevBeepEnabledState = beepEnabled;
  beepEnabled=true;
  buzzer(1,1);  
//  RedLedsOn(false);
  countdown = ((minutes * 60) + seconds);
  UVLedsOn(true);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Exposing Board");
  lcd.setCursor(0, 1);
  lcd.print("Time:");
  lcd.setCursor(11, 1);
  lcd.print(" Secs");
  for (int timer = countdown; timer >= 0; timer --)
  {
    char buffer[6]; 
    
    sprintf(buffer, "%4d", countdown);
    lcd.setCursor(7, 1);
    lcd.print(buffer);
    delay(1000);
    countdown = countdown - 1;
  }
  
  UVLedsOn(false);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ** ALL DONE **");
  lcd.setCursor(0, 1);
  lcd.print("  Press START");
  buzzer(3,3);
  
  logic1 = analogRead(button1);
  while(logic1 != 0)
  {
    logic1 = analogRead(button1);
  }
  
  startCountdown = false;
  minutes = 0;
  seconds = 0;
  countdown = 0;
  beepEnabled = prevBeepEnabledState;
  delay(200);
}

void mainMenu()
{
//  string menuItem="Default Profile";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  - Main Menu -  "); 
  lcd.setCursor(0, 1);
  lcd.print("Default Profile");
  while(startCountdown == false) 
  { 
    logic2 = analogRead(button2);   
    if (logic2 == 0)                  // SECONDS ADJUST
    { 
      seconds ++;
    }
    logic3 = analogRead(button2);   
  }
}

void setupTimer()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  - Set Time -  "); 
  lcd.setCursor(0, 1);
//  lcd.print("Then Press Start");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Set Expose Time"); 
  lcd.setCursor(0, 1);
  lcd.print("Min: ");
  lcd.setCursor(9, 1);
  lcd.print("Sec: ");
  
  while(startCountdown == false) 
  { 
    logic2 = analogRead(button2);   
    if (logic2 == 0)                  // SECONDS ADJUST
    { 
      seconds ++;
      delay(300);
      
       if (seconds >= 60)
       {
         seconds = 0;
         minutes ++;
       }   
    }
   char buffer[3];
   sprintf(buffer, "%2d", seconds); 
   lcd.setCursor(14, 1);
   lcd.print(buffer);
  
   logic3 = analogRead(button3);
   if (logic3 == 0)                  // MINUTES ADJUST
   {
     minutes ++;
     delay(500);
   }
   
   lcd.setCursor(6, 1);
   lcd.print(minutes);
   
   logic4 = analogRead(button4);
   if(logic4 == 0)                   // clear timer
   {
     seconds = 0;
     minutes = 0;
   }
   
   logic1 = analogRead(button1);
   if (logic1 == 0)                  // START             
   {
     startCountdown = true;
   }
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Exposure Start"); 
  lcd.setCursor(0, 1);
  lcd.print("Timer = "); 
  lcd.setCursor(8, 1);
  lcd.print(countdown);
  lcd.setCursor(12, 1);
  lcd.print("Secs");
  
  Expose();
}

void loop()
{
  readButtons();  //I splitted button reading and navigation in two procedures because
  navigateMenus();  //in some situations I want to use the button for other purpose (eg. to change some settings)
/*  RedLedsOn(true);

  splashscreen();
  logic1 = analogRead(button1);
  logic2 = analogRead(button2);
  logic3 = analogRead(button3);

  if (logic1 == 0)                 // SETUP & START BUTTON
  {
    setupTimer();
  }

    Serial.print("button1 = ");
    Serial.print(analogRead(button1));
    Serial.print("   button2 = ");
    Serial.print(analogRead(button2));
    Serial.print("   button3 = ");
    Serial.println(analogRead(button3));
    delay(100);
*/
}

void menuChanged(MenuChangeEvent changed){
 
  buzzer(1,0);  
  MenuItem newMenuItem=changed.to; //get the destination menu
 
  //MenuItem testMenu=newMenuItem.getBack(); 
  lcd.setCursor(0,1); //set the start position for lcd printing to the second row

  if(newMenuItem.getName()==menu.getRoot()){
      lcd.setCursor(0,0); //set the start position for lcd printing to the second row
      lcd.print("-UV Exposure v1-");
      lcd.setCursor(0,1); //set the start position for lcd printing to the second row
      lcd.print("Main Menu       ");
  } else if(not(newMenuItem.getParent()->getName()==menu.getRoot())) {
      lcd.setCursor(0,0); //set the start position for lcd printing to the second row
      lcd.print(newMenuItem.getParent()->getDescription());
      lcd.setCursor(0,1); //set the start position for lcd printing to the second row
      lcd.print(newMenuItem.getDescription());
      lcd.setCursor(15,1); //set the start position for lcd printing to the second row
      //lcd.print("+");
  }else{
      lcd.setCursor(0,0); //set the start position for lcd printing to the second row
      lcd.print("Main Menu       ");
      lcd.setCursor(0,1); //set the start position for lcd printing to the second row
      lcd.print(newMenuItem.getDescription());
      lcd.setCursor(15,1); //set the start position for lcd printing to the second row
      //lcd.print("=");
  }
}

void menuUsed(MenuUseEvent used){
   boolean prevTopUVEnableState = topUVEnabled;
   boolean prevBottomUVEnableState = bottomUVEnabled;
  buzzer(1,0);  
//  lcd.setCursor(0,0); 
//  lcd.print("You used        ");
//  lcd.setCursor(0,1);
//  lcd.print(used.item.getName());

//    MenuItem mi1 = MenuItem("RedLeds",                  "Toggle Red Leds ");
//    MenuItem mi2 = MenuItem("DefaultExpose",            "Default Exposure");
//    MenuItem mi3 = MenuItem("CustomExpose",             "Custom Exposure ");
//    MenuItem mi4 = MenuItem("ProfileExpose",            "Profile Exposure");
//    MenuItem mi4_i1 = MenuItem("Profile1",              "Profile 1       ");
//    MenuItem mi4_i2 = MenuItem("Profile2",              "Profile 2       ");
//    MenuItem mi4_i3 = MenuItem("Profile3",              "Profile 3       ");
//    MenuItem mi5 = MenuItem("Setup",                    "Setup           ");
//    MenuItem mi5_i1 = MenuItem("Beep",                  "Toggle Beep     ");

//Menu version 2 events
  if (used.item==mi1) {
    //RedLedsOn(true);
    ToggleRedLeds();
    delay(debounceDelay);
  } else if (used.item==mi2) {
    minutes=2;
    seconds=15;
    bottomUVEnabled=true;
    topUVEnabled=true;
    Expose();
    topUVEnabled = prevTopUVEnableState;
    bottomUVEnabled = prevBottomUVEnableState;
  } else if (used.item==mi3) {
    setupTimer();
  } else if (used.item==mi4_i1) {
    minutes=1;
    seconds=0;
    Expose();
    menu.toRoot();
  } else if (used.item==mi4_i2) {
    minutes=10;
    seconds=0;
    Expose();
    menu.toRoot();
  } else if (used.item==mi4_i3) {
    minutes=0;
    seconds=30;
    Expose();
    menu.toRoot();
  } else if (used.item==mi5_i1) {
    ToggleBeep();
    delay(debounceDelay);
    //menu.moveBack();
  } else if (used.item==mi5_i2_i1) {
    bottomUVEnabled=true;
    topUVEnabled=true;
    menu.moveParent();
  } else if (used.item==mi5_i2_i2) {
    bottomUVEnabled=true;
    topUVEnabled=false;
    menu.moveParent();
  } else if (used.item==mi5_i2_i3) {
    bottomUVEnabled=false;
    topUVEnabled=true;
    menu.moveParent();
  }



//Menu version 1 events
//  if (used.item==mi1_i1) {
//    //RedLedsOn(true);
//    ToggleRedLeds();
//  } else if (used.item==mi1_i2) {
//    RedLedsOn(false);
//  } else if (used.item==mi1_i2) {
//    RedLedsOn(false);
//  } else if (used.item==mi3_mi2) {
//    setupTimer();
//  }
//  delay(3000);  //delay to allow message reading

//  lcd.setCursor(0,0); 
//  lcd.print("-UV Exposure v1-");
//  lcd.setCursor(0,1);
//  lcd.print("                ");
//  menu.toRoot();  //back to Main
}

int readButtonState(int Button){
  int reading;
  int buttonState;             // the current reading from the input pin
    
  reading = analogRead(Button);  // read the state of the switch into a local variable:
  if (reading>0) reading=1;      

  // check to see if you just pressed the enter button
  // (i.e. the input went from HIGH to LOW),  and you've waited
  // long enough since the last press to ignore any noise: 
  if (reading == 0) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
      buttonState=reading;
      // reset the debouncing timer
      lastDebounceTime=millis();
      // save the reading.  Next time through the loop,
      // it'll be the lastButtonState:
      lastButtonState = reading;
      lastButtonUsed = Button;
    }
  }
 
  return reading;
}

void  readButtons(){  //read buttons status
  int buttonEnterState=1;             // the current reading from the Enter input pin
  int buttonEscState=1;             // the current reading from the input pin
  int buttonLeftState=1;             // the current reading from the input pin
  int buttonRightState=1;             // the current reading from the input pin
  
  buttonEnterState=readButtonState(buttonPinEnter);
  buttonEscState=readButtonState(buttonPinEsc);
  buttonLeftState=readButtonState(buttonPinLeft);
  buttonRightState=readButtonState(buttonPinRight);

lcd.setCursor(0, 10);

  //records which button has been pressed
  if (buttonEnterState==0){
    lastButtonPushed=buttonPinEnter;
//    lcd.print("ENTER ");

  }else if(buttonEscState==0){
    lastButtonPushed=buttonPinEsc;
//    lcd.print("ESC   ");

  }else if(buttonRightState==0){
    lastButtonPushed=buttonPinRight;
//    lcd.print("DOWN  ");

  }else if(buttonLeftState==0){
    lastButtonPushed=buttonPinLeft;
//    lcd.print("UP    ");

  }else{
    lastButtonPushed=0;
  }                 

//delay(2000);  
}



void navigateMenus() {
  MenuItem currentMenu=menu.getCurrent();
  int buttonUsed = lastButtonPushed;
  lastButtonPushed=0; //reset the lastButtonPushed variable
 
  switch (buttonUsed){
    case buttonPinEnter:
      if(!(currentMenu.moveFirstChild())){  //if the current menu has a child and has been pressed enter then menu navigate to item below
        menu.use();
      }else{  //otherwise, if menu has no child and has been pressed enter the current menu is used
        menu.moveFirstChild();
       }
      break;
    case buttonPinEsc:
//      buzzer(1,2);
      //menu.toRoot();  //back to main
      if (currentMenu.getParent()->getName()==menu.getRoot()) {
        lcd.setCursor(0,1); //set the start position for lcd printing to the second row
        lcd.print("Main Menu       ");
        menu.toRoot();  //back to main
      } else {
        menu.moveParent();
      }
      break;
    case buttonPinRight:
//      buzzer(1,1);
      menu.moveNextSibling();
//      menu.moveDown();
      break;     
    case buttonPinLeft:
//      buzzer(1,0);
      menu.movePreviousSibling();
//      menu.moveUp();
      break;     
  }
 
}



