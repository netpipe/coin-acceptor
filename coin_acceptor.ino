//credits for code
//feel free to use however you see fit
//https://github.com/olavxxx/Arduino-Sound-Sensor/blob/master/Soundsensor/Soundsensor.ino
//http://timewitharduino.blogspot.com/2014/01/isr-based-sketch-for-adafruit-coin.html

// i2c extender code for lcd



const int COINPIN = 8;
const int threshold1 = 620;
const int sampleWindow = 100; // Sample window width in mS (50 mS = 20Hz)

unsigned int sample;
int samplebuffer = 0;
int count=0; //idle loop counter
float money = 0.0;
volatile int pulses = 0;
volatile long timeLastPulse = 0;


void setup() {
  Serial.begin(9600);
  pinMode(COINPIN, INPUT);
  pinMode(COINPIN,INPUT_PULLUP);
}


void loop() {
  
   unsigned long startMillis= millis();  // Start of sample window
   
   while (millis() - startMillis < sampleWindow)
   {
      sample = digitalRead(COINPIN);
      if (sample < 1)
      {
       // Serial.println(samplebuffer);
            samplebuffer++;
      }
   }
   
    if (samplebuffer >= threshold1) 
   {
        Serial.println(samplebuffer);
        pulses++;
        timeLastPulse = millis();
        count=0;
   }
   else{count++; //idle counter}

  if (startMillis > sampleWindow) {
    samplebuffer = 0;
  }
  
  
  long timeFromLastPulse = millis() - timeLastPulse;
      
  if (pulses > 0 && timeFromLastPulse > 180)
  {
    if (pulses == 3)
    {
      Serial.println("Received quarter (3 pulses)");
      money += .25;
    }
    else if (pulses == 6)
    {
      Serial.println("Received 50 cents (6 pulses)");
      money += .50;
    }
    else if (pulses == 9)
    {
      Serial.println("Received 75 cents (9 pulses)");
      money += .75;
    }
    else if (pulses == 4) //10 pulses
    {
      Serial.println("Received looney (4 pulses)");
      money += 1.0;
    }
    else if (pulses == 8) 
    {
      Serial.println("Received 2looney (8 pulses)");
      money += 2.0;
    }
    else if (pulses == 7)
    {
      Serial.println("Received 1.25 (7 pulses)");
      money += 1.25;
    }
    else if (pulses == 8)
    {
      Serial.println("Received tooney (8 pulses)");
      money += 2.0;
    }
    else if (pulses == 16) //15 pulses
    {
      Serial.println("Received 2tooney (16 pulses)");
      money += 4.0;
    } else  { Serial.print("Unknown coin: "); Serial.print(pulses); Serial.println(" pulses"); }
    
    pulses = 0;
    timeFromLastPulse=0;
    timeLastPulse=0;

  }else{ if (count > 100) { delay(5);  Serial.println(" sleep");}}
  //idle detector
   //delay(2);
          
          
}



