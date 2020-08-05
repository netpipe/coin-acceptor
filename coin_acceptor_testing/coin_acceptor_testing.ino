//credits for code
//feel free to use however you see fit
//https://github.com/olavxxx/Arduino-Sound-Sensor/blob/master/Soundsensor/Soundsensor.ino
//http://timewitharduino.blogspot.com/2014/01/isr-based-sketch-for-adafruit-coin.html

// i2c extender code for lcd
#define IRSENSOR   //TCRT5000 IR SENSOR module from ebay hotglued to bottom of unit on left side and connected to D0

const int IRPIN = 0;
const int COINPIN = 8;
const int threshold1 = 620;    // higher value for larger sample window
const int sampleWindow = 150; // Sample window width in mS (50 mS = 20Hz)

int oldpulses=0;
int counter=0;
unsigned int sample;
int samplebuffer = 0;
int count=0; //idle loop counter
float money = 0.0;
volatile int pulses = 0;
volatile long timeLastPulse = 0;
String pulsestring;


void setup() {
  Serial.begin(9600);
  pinMode(COINPIN,INPUT_PULLUP);
}


void loop() {
  
   unsigned long startMillis= millis();  // Start of sample window

   while (millis() - startMillis < sampleWindow)
   {
     #ifdef IRSENSOR
        if (samplebuffer > 2 && oldpulses < pulses + 1){
           if (digitalRead(IRPIN) < 1){
             // counter++;
              oldpulses=pulses+1;
              pulsestring+=0;
         //    Serial.println("IRPULSE");
           }
        }
      #endif
        
      sample = digitalRead(COINPIN);
      if (sample < 1)
      {
       // Serial.println(samplebuffer);
            samplebuffer++;
      }
   }
   
    if (samplebuffer >= threshold1) 
   {
   //  Serial.println("pulse");
    //    Serial.println(samplebuffer);
        pulses++;
        pulsestring+=1;
        timeLastPulse = millis();
        count=0;

   }
   else{count++; }//idle counter

  if (startMillis > sampleWindow) {
    samplebuffer = 0;
  }
  
 // Serial.println(digitalRead(0));

  
  long timeFromLastPulse = millis() - timeLastPulse;
      
  if (pulses > 0 && timeFromLastPulse > 1000) //set higher to be more accurate or if you are getting single and double pulse
  {
     pulses = 0;
    for (int i=0;i < pulsestring.length(); i++){
      Serial.println(pulsestring.c_str());
      //Serial.println(pulsestring.length()-1); // exclude the irrpulse spacer
      
      String pulsestring2;

      pulsestring2 += pulsestring.charAt(i);
      Serial.println(pulsestring2.c_str());
      
      if (pulsestring2 == "0"){
              i++;
              pulsestring2="";
           pulsestring2 += pulsestring.charAt(i);
           Serial.println("nextinstring");
         }
         
        while(pulsestring2 == "1"){
                pulsestring2="";
              //  pulsestring2 += pulsestring.charAt(i);
          pulses++;
          i++; 
          pulsestring2 += pulsestring.charAt(i);
          Serial.println(pulses);
      
        }
      pulsestring2="";
     // Serial.println(pulses);
    
    if (pulses == 3)
    {
      Serial.println("Received quarter (3 pulses)");
      money += .25;
          pulses = 0;
    }
    
    if (pulses == 4)
    {
      Serial.println("Received looney (4 pulses)");
      money += 1.0;
          pulses = 0;
    }
    
    if (pulses == 8)
    {
      Serial.println("Received toonie (8 pulses)");
      money += 2.0;
          pulses = 0;
    }
    pulses = 0;

  }
      //counter=0;
    pulsestring="";
    pulses = 0;
    timeFromLastPulse=0;
    timeLastPulse=0;
    oldpulses=0;
  }

 //Serial.println(money);
 // }}  
 //idle detector
        
          
}




