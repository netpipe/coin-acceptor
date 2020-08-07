//credits for code
//feel free to use however you see fit
//https://github.com/olavxxx/Arduino-Sound-Sensor/blob/master/Soundsensor/Soundsensor.ino
//http://timewitharduino.blogspot.com/2014/01/isr-based-sketch-for-adafruit-coin.html

// i2c extender code for lcd
#define IRSENSOR   //TCRT5000 IR SENSOR module from ebay hotglued to bottom of unit on left side and connected to D0

const int IRPIN = 0;
const int COINPIN = 8;
const int threshold1 = 350;    // higher value for larger sample window
const int sampleWindow = 40; // Sample window width in mS (50 mS = 20Hz)

int oldpulses=0;
int counter=0;
unsigned int sample;
int samplebuffer = 0;
int count=0; //idle loop counter
float money = 0.0;
volatile int pulses = 0;
volatile long timeLastPulse = 0;
String pulsestring;
unsigned long startMillis;
  unsigned long elapsedmilis=0;
  unsigned long halfstart;
  int bpulse=0;
  
void setup() {
  Serial.begin(9600);
  pinMode(COINPIN,INPUT_PULLUP);
}


void loop() {
  
   startMillis= millis();  // Start of sample window

   while ( elapsedmilis < sampleWindow)
   {
           #ifdef IRSENSOR
           if (bpulse != 1 && digitalRead(IRPIN) < 1 ){
              counter++;
              bpulse=1;
                pulsestring+=0;
             Serial.println("IRPULSE1");         
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
   //  Serial.println("pulse");
     //   Serial.println(samplebuffer);
        pulses++;
        pulsestring+=1;
        timeLastPulse = millis();
        count=0;
        delay (100);  
   }
   else{count++; }//idle counter

  if (elapsedmilis >= sampleWindow) {
    samplebuffer = 0;
    elapsedmilis=0;
    count++;      
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
        bpulse=0;
  }else{  if (count > 300) { counter=0; bpulse=0;  if (count > 600) {delay(4);  Serial.println("money"); Serial.println(money);
  }}}  //idle detector

 //Serial.println(money);
 // }}  
 //idle detector
        
          
}





