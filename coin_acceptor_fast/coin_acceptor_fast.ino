//credits for code
//feel free to use however you see fit
//https://github.com/olavxxx/Arduino-Sound-Sensor/blob/master/Soundsensor/Soundsensor.ino
//http://timewitharduino.blogspot.com/2014/01/isr-based-sketch-for-adafruit-coin.html

// i2c extender code for lcd
#define IRSENSOR   //TCRT5000 IR SENSOR module from ebay hotglued to bottom of unit on left side and connected to D0

//20 , 40,70 ms with 100 ms between

//#define SLOW 70
//#define MEDIUM 40
#define FAST 20

const int IRPIN = 0;
const int COINPIN = 8;

#ifdef MEDIUM 
const int threshold1 = 350;    // higher value for larger sample window
const int sampleWindow = 40; // Sample window width in mS (50 mS = 20Hz)
#endif

#ifdef FAST
const int threshold1 = 100;    // higher value for larger sample window
const int sampleWindow = 15; // Sample window width in mS (50 mS = 20Hz)
#endif

int oldpulses=0;
int counter=0;
unsigned int sample;
int samplebuffer = 0;
int count=0; //idle loop counter
float money = 0.0;
volatile int pulses = 0;
volatile long timeLastPulse = 0;
//string pulsestring;
unsigned long startMillis;
long elapsedmilis=0;
long halfmilis=0;
long timeFromLastPulse;
int bpulse=0;
int oldcounter=0;



void setup() {
  Serial.begin(9600);
  pinMode(COINPIN,INPUT_PULLUP);
}


void loop() {

  startMillis= millis();  // Start of sample window
  //halfmilis=startMillis/2-40;

  while (elapsedmilis < sampleWindow)
  {
    
  #ifdef IRSENSOR
    if (bpulse != 1 && digitalRead(IRPIN) < 1 ){
      bpulse++;
    delay(10);      
    }

  #endif


    if (digitalRead(COINPIN) < 1)
    {
      // Serial.println(samplebuffer);
      samplebuffer++;
    }

    elapsedmilis = millis() - startMillis;       
  }
  
  if (samplebuffer >= threshold1) 
  {   
    Serial.println(samplebuffer);
    pulses++;
    // pulsestring+=1;
    timeLastPulse = millis();
    count=0;
    delay (100);
  }

  //if (elapsedmilis >= sampleWindow) {
    samplebuffer = 0;
    elapsedmilis=0;
    count++;
    // oldcounter=0;
 // }
 #ifdef IRSENSOR
    if (count > 6 && bpulse != 0){
    //  Serial.println("IRPULSE2"); 
      Serial.println(counter); 
    counter++;
    bpulse=0;
    count=0;
    delay(15); 
  }
#endif

  timeFromLastPulse = millis() - timeLastPulse;

  if (pulses > 0 && timeFromLastPulse > 150) //set higher to be more accurate or if you are getting single and double pulse
  {
    // single and double pulse available aswell but sometimes are triggered accidently when putting coins in fast
    // might be ok to use with larger 100ms sample window now.
    if (pulses == 3)
    {
      Serial.println("Received quarter (3 pulses)");
      Serial.println(counter);
      money += .25;
    }
    else if (pulses == 6)
    {
      Serial.println("Received 50 cents (6 pulses)");
      Serial.println(counter);
      money += .50;
    }
    else if (pulses == 9)
    {
      Serial.println("Received 75 cents (9 pulses)");
      Serial.println(counter);
      money += .75;
    }
    else if (pulses == 4) //10 pulses
    {
      Serial.println("Received looney (4 pulses)");
      Serial.println(counter);
      money += 1.0;
    }
    else if (pulses == 8) 
    {
      Serial.println("Received toonie (8 pulses)");
      Serial.println(counter);
      money += 2.0;
    }
    else if (pulses == 7)
    {
      Serial.println("Received 1.25 (7 pulses)");
      money += 1.25;
    }
    else if (pulses == 10)
    {
      Serial.println("Received 1.50 (10 pulses)");
      money += 1.50;
    }
    else if (pulses == 11)
    {
      Serial.println("Received 2.25 (11 pulses)");
      money += 2.25;
    }
    else if (pulses == 12)
    {
      Serial.println(counter);
      if (counter == 2){
        Serial.println("Received 3.00 (12 pulses)"); 
        money += 3.00;
      }
      if (counter == 4){
        Serial.println("Received 3 quarters (12 pulses)");
        money += 1.00;
      }
    }
    else if (pulses == 15)
    {
      Serial.println("Received 5quarters (16 pulses)");
      money += 1.25;
    }
    else if (pulses == 16)
    {
      Serial.println("Received 2tooney (16 pulses)");
            Serial.println(counter);
      money += 4.0;
    }
    else if (pulses == 20) 
    {
      Serial.println("Received 2tooney (20 pulses)");
      money += 3.0;
    }
    else if (pulses == 24) 
    {
      Serial.println("Received 3tooney (24 pulses)");
      money += 6.0;
    } 
    else  { 
      Serial.print("Unknown coin: "); 
      Serial.print(pulses); 
      Serial.println(" pulses"); 
    }

    counter=0;
    pulses = 0;
    timeFromLastPulse=0;
    timeLastPulse=0;
    oldpulses=0;
    bpulse=0;
    oldcounter=0; 
    count=0;
  }
  else{  
    if (count > 50) {
     // Serial.println("r");
      oldcounter=0;  
      counter=0; 
      bpulse=0;  
      if (count > 600) {
        delay(4);  
        Serial.println("money"); 
        Serial.println(money);
      }
    }
  }  //idle detector


}

