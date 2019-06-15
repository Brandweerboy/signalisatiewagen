

//libary
#include <FastLED.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BT(10, 11); // RX, TX

//auto pinnen
#define in4 7
#define in3 6
#define in2 5
#define in1 4
#define ENB 2
#define ENA 3

int tijd = 10000 / 100;

int ledPins[8] = { 52, 50, 48, 46, 44, 42, 40, 38, };
//   index         0    1   2   3   4   5   6   7


//trage flicker
unsigned long startTime = 0;      // ALWAYS use unsigned long for timers, not int
unsigned long currentTime = 0;    // every loop currentTime is measured
unsigned long delayTime = 600;    // time to delay
boolean FLICKER = LOW;

//snelle pinker
unsigned long pinkerstartTime = 0;      // ALWAYS use unsigned long for timers, not int
unsigned long pinkercurrentTime = 0;    // every loop currentTime is measured
unsigned long pinkerdelayTime = 30;
boolean PINKER = LOW;

// kleuren
long BLAUW  = 0x0004FF;
long ORANJE = 0xFE7000;
long ZWART  = 0x000000; // = uit

//Ledstrip declaratie
#define DATA_PIN 12
#define BRIGHTNESS 60
#define LED_TYPE WS2812

// state op 0 zetten = false
bool state_LINKS = false;
bool state_RECHTS = false;
bool state_KRUIS = false;
bool state_SERVO = false;
bool state_lichten = false;
bool state_handrem = false;
bool state_licht_links = false;
bool state_licht_rechts = false;
bool state_nood = false;
bool state_achterlicht = false;
bool state_seinlicht_1 = false;
int stateBT;

// maximum x - y rooster
#define  MAXX  5
#define  MAXY  5

//declareren leds
int licht_1 = 47;
int licht_2 = 45;
int licht_links_1 = 51;
int licht_links_2 = 43;
int licht_rechts_1 = 49;
int licht_rechts_2 = 41;
int achterled_1 = 39;
int achterled_2 = 37;


int licht_links [2] = { 51, 43};

//declareren van peil rechts ; patroon rechts maken
int patroonPeil_RECHTS[5][2] = { {1, 0 } , {2, 1 } , {3, 2 } , {2, 3 } , {1, 4 } , };
//                    0      1           partoon[0][0] = x-waarde
//                     0 1    0 1        partoon[0][1] = y-waarde

//declareren van peil links ; patroon links maken
int patroonPeil_LINKS[5][2] = { {3, 0 } , {2, 1 } , {1, 2 } , {2, 3 } , {3, 4 } , };
//                    0      1           partoon[0][0] = x-waarde
//                     0 1    0 1        partoon[0][1] = y-waarde

//declareren van kruis ; patroon kruis maken
int patroonKruis[9][2] = { {0, 0 } , {2, 2 } , {1, 3 } ,   {3, 1 } , {3, 3 }, {4, 4 } , {1, 1 }, {4, 0 } , {0, 4 } };
//                    0      1           partoon[0][0] = x-waarde
//                     0 1    0 1        partoon[0][1] = y-waarde

//declareren buitenste lichten
int BLAUWLICHT[4][2] = { {0, 0 } ,  {4, 4 }, {4, 0 } , {0, 4 } };
//                    0      1           partoon[0][0] = x-waarde
//                     0 1    0 1        partoon[0][1] = y-waarde


// formule voor matrix te veranderen in ledstrip
CRGB leds [MAXX * MAXY];
// lednr = MAXY * y + x

Servo myservo;  // create servo object to control a servo

void setup() {
  startTime = millis(); // place this before e.g. in setup()

  // put your setup code here, to run once:
  for (int i = 0; i <= 5; i++) {
    pinMode( ledPins[i] , OUTPUT);
  } // end for


  //ledjes
  pinMode(licht_1, OUTPUT);
  pinMode(licht_2, OUTPUT);
  pinMode(licht_links_1, OUTPUT);
  pinMode(licht_rechts_1, OUTPUT);
  pinMode(licht_links_2, OUTPUT);
  pinMode(licht_rechts_2, OUTPUT);
  pinMode(achterled_1, OUTPUT);
  pinMode(achterled_2, OUTPUT);
  //
  Serial.begin(9600);

  // kijken of monitor werkt en start teken zeggen nu kan hij waarden in lezen
  Serial.println("start");

  // set the data rate for the SoftwareSerial port
  BT.begin(9600);

  // auto pinnen
  pinMode(in4, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // initialiseren FastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, MAXX * MAXY);
  FastLED.setBrightness( BRIGHTNESS );

  myservo.attach(53);  // attaches the servo on pin 6 to the servo object

  // starten geav delay
  startTime = millis();
  pinkerstartTime = millis();
} //end setup

void loop() {



  // Update the Bounce instances :
  if ( BT.available() ) {
    stateBT = BT.read();
    Serial.print("data BT = ");
    Serial.println(stateBT);
  } // end if

  //BEGIN AUTO BESTUREN

  
  if (stateBT == 45) {
    state_seinlicht_1 = !state_seinlicht_1;
  }//end if

  if(state_seinlicht_1 == 1) {
       for (int i = 0; i <= 4; i++){
      digitalWrite(ledPins[4+i],HIGH);
      digitalWrite(ledPins[4-i],HIGH);
      delay(100);
    } // end for
    
    for (int i = 0; i <= 4; i++) {
      digitalWrite(ledPins[4+i],LOW);
      digitalWrite(ledPins[4-i],LOW);
      delay(100);
    } // end for
   for (int i = 4; i >= 0; i--){
      digitalWrite(ledPins[4-i],HIGH);
      digitalWrite(ledPins[4+i],HIGH);
      delay(100);
   }//end for
   for (int i = 4; i >= 0; i--){
    digitalWrite(ledPins[4-i],LOW);
    digitalWrite(ledPins[4+i],LOW);
    delay(100);
   }// end for
  }


  if (stateBT == 11) {
    state_lichten = !state_lichten;
  }//end if

  if (state_lichten == 1 ) {
    digitalWrite(licht_1, HIGH);
    digitalWrite(licht_2, HIGH);
  } else {
    digitalWrite(licht_1, LOW);
    digitalWrite(licht_2, LOW);
  }// end if - else

  if (stateBT == 1) {
    state_achterlicht = !state_achterlicht;
  }//end if

  if (state_achterlicht == 0 ) {
    digitalWrite(achterled_1, HIGH);
    digitalWrite(achterled_2, HIGH);
  } else {
    digitalWrite(achterled_1, LOW);
    digitalWrite(achterled_2, LOW);
  }// end if - else

  if (stateBT == 5) {
    state_licht_links = !state_licht_links;
  }//end if

  if (state_licht_links == 1) {

    for (int i = 0; i < 5; i++) {
      blinkLed(licht_links, tijd);
    }



  }//end if

  if (stateBT == 4) {
    state_licht_rechts = !state_licht_rechts;
  }//end if

  if (state_licht_rechts == 1) {
    blinkLed(licht_rechts_1 , tijd);
    blinkLed(licht_rechts_2 , tijd);
  }//end if

  if (stateBT == 3) {
    state_nood = !state_nood;
  }//end if

  if (state_nood == 1) {
    blinkLed(licht_links_1 , tijd);
    blinkLed(licht_links_2 , tijd);
    blinkLed(licht_rechts_1 , tijd);
    blinkLed(licht_rechts_2 , tijd);
  }//end if else

  if (stateBT == 21) {
    rechts();
  }

  if (stateBT == 20) {
    links();
  }

  if (stateBT == 22) {
    vooruit();
  }

  if (stateBT == 10) {
    handrem();
  }

  if (stateBT == 23) {

    achteruit();
  }//end if-else



  // BEGIN TRAFFIC ADVISOR
  // making 1 second (time-variables must be of type ’long’)
  //               |<------------------- delayTime ------------->|
  //     |---------|=================================|-----------|-----> t(s)
  //  millis() startTime                     currentTime
  //               |<---- currentTime-startTime ---->|
  currentTime = millis();
  if ( (currentTime - startTime) > delayTime) {
    FLICKER = !FLICKER;
    startTime = currentTime; // reset startTime
  } // end if


  // making 1 second (time-variables must be of type ’long’)
  //               |<------------------- delayTime ------------->|
  //     |---------|=================================|-----------|-----> t(s)
  //  millis() startTime                     currentTime
  //               |<---- currentTime-startTime ---->|
  pinkercurrentTime = millis();
  if ( (pinkercurrentTime - pinkerstartTime) > pinkerdelayTime) {
    PINKER = !PINKER;
    pinkerstartTime = pinkercurrentTime; // reset startTime
  } // end if

  /*
    Serial.print("FLICKER =");
    Serial.print(FLICKER);
    Serial.print("  PINKER =");
    Serial.println(PINKER);
  */
  // verander state waarde false= 0 naar 1
  if (stateBT == 50) {
    state_SERVO = !state_SERVO;
  }//end if

  //
  if (state_SERVO) {
    myservo.write(90);    // tell servo to go to position in variable 'pos'
  } else {
    //alle andere 3 states op false zetten
    state_LINKS = false; state_RECHTS = false; state_KRUIS = false;
    myservo.write(0);              // tell servo to go to position in variable 'pos'
  } //end if

  if (stateBT == 102) {
    //alle andere 3 state op 0 zetten
    state_LINKS = false; state_KRUIS = false;
    state_RECHTS = !state_RECHTS;
  }//end if

  if (stateBT == 103) {
    //alle andere 2 state op false zetten
    state_RECHTS = false; state_KRUIS = false;
    state_LINKS = !state_LINKS;
  }//end if

  // verander state waarde false= 0 naar 1
  if (stateBT == 104) {
    //alle andere 3 states op false zetten
    state_LINKS = false; state_RECHTS = false;
    state_KRUIS = !state_KRUIS;
  }//end if


  // blauwlicht flikkert snel als pinker niet aan is
  if (!FLICKER && PINKER && ( state_LINKS == 1 || state_RECHTS == 1 || state_KRUIS == 1)) {
    //FastLED.clear();
    for (int i = 0; i < 4; i++) {
      leds[MAXY * BLAUWLICHT[i][1] + BLAUWLICHT[i][0]] = BLAUW;
    } //end for
    FastLED.show();
  } //end if


  // RECHTS
  if (FLICKER && state_RECHTS == 1) {
    //peil maken naar rechts
    for (int i = 0; i < 5; i++) {
      leds[MAXY * patroonPeil_RECHTS[i][1] + patroonPeil_RECHTS[i][0]] = ORANJE;
    } //end for
    FastLED.show();
  } else {
    FastLED.clear();
    FastLED.show();
  } //end if


  // LINKS
  if (FLICKER && state_LINKS == 1) {
    //peil maken naar rechts
    for (int i = 0; i < 5; i++) {
      leds[MAXY * patroonPeil_LINKS[i][1] + patroonPeil_LINKS[i][0]] = ORANJE;
    } //end for
    FastLED.show();
  } else {
    FastLED.clear();
    FastLED.show();
  } //end if


  // KRUIS
  if (FLICKER && state_KRUIS == 1) {
    //Kruis maken
    for (int i = 0; i < 9; i++) {
      leds[MAXY * patroonKruis[i][1] + patroonKruis[i][0]] = ORANJE;
    } //end for
    FastLED.show();
  } else {
    FastLED.clear();
    FastLED.show();
  } //end if






  stateBT = !stateBT;

} //end loop

void vooruit() {
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  analogWrite(ENB, 225);
  analogWrite(ENA, 225);
} //end vooruit

void handrem() {
  digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
  analogWrite(ENB, 0);
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
  analogWrite(ENA, 0);
} //end handrem

void links() {
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  analogWrite(ENB, 190);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  analogWrite(ENA, 130);
} //end links

void rechts() {
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  analogWrite(ENB, 130);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  analogWrite(ENA, 190);
} //end rechts

void achteruit() {
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  analogWrite(ENB, 225);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  analogWrite(ENA, 225);

} //end achteruit

void blinkLed(int pin, int tijd) {
  digitalWrite(pin, HIGH);
  delay(tijd);
  digitalWrite(pin, LOW);
  delay(tijd);
} // end blinkled

