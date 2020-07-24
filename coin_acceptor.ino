
// some of the code is copyrighted for now until i can rewrite then it will be lgpl
//https://github.com/BasOnTech/Arduino-Beginners-EN/tree/master/E30-sound-sensor  -- not used anymore
//https://github.com/olavxxx/Arduino-Sound-Sensor/blob/master/Soundsensor/Soundsensor.ino
//http://timewitharduino.blogspot.com/2014/01/isr-based-sketch-for-adafruit-coin.html

//method is opensource feel free to rewrite code, or wait until i can do it.



const int COINPIN = 8;

const int threshold1 = 285;

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

float money = 0.0;

// gets incremented by the ISR;
// gets reset when coin was recognized (after train of pulses ends);
volatile int pulses = 0;
volatile long timeLastPulse = 0;

int samplebuffer = 0;


void setup() {
  Serial.begin(9600);
    pinMode(OUT_PIN, INPUT);
  pinMode(OUT_PIN,INPUT_PULLUP);
  
}

int count=0;
int count2=0;





void loop() {
  
   unsigned long startMillis= millis();  // Start of sample window
   while (millis() - startMillis < sampleWindow)
   {
      sample = digitalRead(COINPIN);
      if (sample < 290)  // toss out spurious readings
      {
            samplebuffer++;
      }
   }
   
    if (samplebuffer >= threshold1) 
   {
        Serial.println(samplebuffer);
        pulses++;
          timeLastPulse = millis();
            Serial.println("Knock1!");
   }


  if (startMillis < sampleWindow) {
    samplebuffer = 0;
  }
  
  
  
  long timeFromLastPulse = millis() - timeLastPulse;
      
  if (pulses > 0 && timeFromLastPulse > 365)
  {
    //check for doubles for fast coins
    
    // sequence of pulses stopped; determine the coin type;
   // if (pulses == 2)
  //  {
  //    Serial.println("Received dime (2 pulses)");
  //    money += .1;
  //  }
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
      Serial.println("Received looney (5 pulses)");
      money += 1.0;
    }
    else if (pulses == 8) 
    {
      Serial.println("Received 2looney (10 pulses)");
      money += 2.0;
    }
    else if (pulses == 5)
    {
      Serial.println("Received tooney (5 pulses)");
      money += 2.0;
    }
        else if (pulses == 10) //15 pulses
    {
      Serial.println("Received 2tooney (10 pulses)");
      money += 4.0;
    }
    else
    {
     Serial.print("Unknown coin: ");
      Serial.print(pulses);
      Serial.println(" pulses");
      
      // add 1 to total to try and make a coin
    }

    pulses = 0;
    timeFromLastPulse=0;
    timeLastPulse=0;

  }
  
  
   delay(2);
          
          
}


