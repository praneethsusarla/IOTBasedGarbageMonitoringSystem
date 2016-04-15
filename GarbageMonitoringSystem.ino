#include<Wire.h>
#include "rgb_lcd.h"
#include<Ethernet.h>

// Ultrasonic Sensor Pins
const int trigPins[4] ={5,7,9,11};
const int echoPins[4] = {6,8,10,12};

//Buzzer Pin
const int buzzPin= 3;

// Touch Sensor Pin
const int touchPin=4;

// LCD display
rgb_lcd lcd;
int colorR = 255;
int colorG = 0;
int colorB = 0;

long init_duration[4], init_distance[4];
int touchVal=0;
long min_threshold = 5;

//Ethernet Settings
byte mac[] = {0x98, 0x4F, 0xEE, 0x03, 0x00, 0xC8};
char server[] = "api.thingspeak.com";
String writeAPIKey = "4II3WOOXGUPA5ZN7"; // API Key
EthernetClient client;
const int updateInterval = 5*1000; //Time interval in milli seconds to update Thingspeak

//Variable Setup for Cloud Connection
long lastConnectionTime = 0;
boolean lastConnected  = false;
int resetCounter = 0; 

//Function Declarations
void Warning(String data, int index);
void updateThingSpeak(String tsData);

int tries = 0;
void setup() {
  Serial.begin(9600);
  
  //Ultrasonic Sensors Setup
  for(int i=0;i<4;i++){
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  
  //Buzzer Sensor Setup
  pinMode(buzzPin, OUTPUT);

  //Touch Sensor Setup
  pinMode(touchPin, INPUT);
  
  // LCD display setup
  lcd.begin(16,2);
  lcd.setRGB(colorR, colorG, colorB);
  

  lcd.setCursor(0,0);
  lcd.print("Initialising...");
  lcd.setCursor(0,1);
  lcd.print("Press the switch to continue...");
  lcdScroll(15);

  
  /*while(touchVal <= 0)
    touchVal = digitalRead(touchPin);

  touchVal = 0;*/
  //digitalWrite(buzzPin, LOW);

  for(int i=0;i<4;i++){
    
    do{
    digitalWrite(trigPins[i], LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPins[i], LOW);

    /*while(touchVal <= 0)
      touchVal = digitalRead(touchPin);*/
   
    //touchVal = -1;
    //while(init_duration[i] == 0)
    init_duration[i] = pulseIn(echoPins[i], HIGH);
    
    init_distance[i] = (init_duration[i]/2) / 29.1; //speed of sound 340m/s converted into micro seconds
    tries++;
    }while(init_duration[i] == 0 && ++tries < 3);
    tries = 0;
    Serial.println("Initial Distance of "+String(i+1)+" is: "+String(init_distance[i]));
    // buzzer sound before every sensor display
   /* digitalWrite(buzzPin, HIGH);
    delay(200);
    digitalWrite(buzzPin, LOW);
    */
    if (i==0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.setRGB(255, 128, 0); //Setting Orange
      lcd.print("Ultrasonic_1");
        
      lcd.setCursor(0,1);
      lcd.print("Initial Distance: ");
      lcd.print(init_distance[i]);
      lcdScroll(15);
      delay(1000);
    }
    else if (i==1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.setRGB(255, 255, 255); //Setting white
      lcd.print("Ultrasonic_2");
        
      lcd.setCursor(0,1);
      lcd.print("Initial Distance: ");
      lcd.print(init_distance[i]);
      lcdScroll(15);
      delay(1000);
    }

    else if (i==2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.setRGB(0, 255, 0); //Setting Green
      lcd.print("Ultrasonic_3");
        
      lcd.setCursor(0,1);
      lcd.print("Initial Distance: ");
      lcd.print(init_distance[i]);
      lcdScroll(15);
      delay(1000);
    }

    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.setRGB(0, 0, 255); //Setting Blue
      lcd.print("Ultrasonic_4");
        
      lcd.setCursor(0,1);
      lcd.print("Initial Distance: ");
      lcd.print(init_distance[i]);
      lcdScroll(15);
      delay(1000);
    }
    
    touchVal = -1;
  }
 
  //lcdPrint("Initialising.....", 16);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Initialisation done....");
lcdScroll(15);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Tap the switch twice to continue");
lcdScroll(15);
/*while(touchVal <= 0)
      touchVal = digitalRead(touchPin);
touchVal = -1;
digitalWrite(buzzPin, HIGH);
delay(200);
digitalWrite(buzzPin, LOW);*/
      

 //Ethernet Setup with the Server
 client.connect(server, 80);
}


void loop() {
    // put your main code here, to run repeatedly:
  long duration[4]={0,0,0,0}, distance[4]={0,0,0,0};
  String data[4];
  for(int i=0;i<4;i++){
    
      digitalWrite(trigPins[i], LOW);  // Added this line
      delayMicroseconds(2); // Added this line
      digitalWrite(trigPins[i], HIGH);
      delayMicroseconds(10); // Added this line
      digitalWrite(trigPins[i], LOW);
      
      duration[i] = pulseIn(echoPins[i], HIGH);
      distance[i] = (duration[i]/2) / 29.1; //speed of sound 340m/s converted into micro seconds
      data[i] = String((int)distance[i], DEC);
      if((init_distance[i]-distance[i]) >= min_threshold){
            Warning(data[i],i);
            Continue();
      }
      else{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.setRGB(127, 127, 127);
          lcd.print("Monitoring....");
          lcdScroll(15);
      }
   
  }
  
  if(client.connected() && (millis() - lastConnectionTime > updateInterval)){
      Serial.println("Updating.....");
      Serial.println("Connection: "+String(client.connect(server,80), DEC));
      updateThingSpeak("field1="+ data[0]+"&field2="+data[1]+"&field3="+data[2]+"&field4="+data[3]);
      Serial.println("Data Sent!!");
    }
   lastConnected = client.connected();
}


void lcdScroll(int size){
  int i=0;
  for(i=0;i<size;i++){
    lcd.scrollDisplayLeft();
    delay(150);
  }

  for(i=0;i<size+(16-size);i++){
    lcd.scrollDisplayRight();
    delay(150);
  }
  
}

void Warning(String data, int index)
{
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.setRGB(255, 0, 0); //Setting Red
  
  for(int i=0; i<6;i++){
    digitalWrite(buzzPin, HIGH);
    delay(50);
    digitalWrite(buzzPin, LOW);
  }
  lcd.print("Bin_"+String(index+1,DEC)+ " is full");
    
  lcd.setCursor(0,1);
  lcd.print("Distance: ");
  lcd.print(data);
  lcdScroll(15);
  
  lcd.setCursor(0,1);
  lcd.print("Tap to continue... ");
  lcdScroll(15);
  
  while(touchVal <= 0)
      touchVal = digitalRead(touchPin);
  touchVal = -1;
  digitalWrite(buzzPin, HIGH);
  delay(200);
  digitalWrite(buzzPin, LOW);
  //delay(1000);  
}

void Continue(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.setRGB(127,127,127);
  lcd.print("Monitoring....");
  lcdScroll(15);
}

void updateThingSpeak(String tsData)
{
    Serial.println("Connected to ThingSpeak...");
    Serial.println();
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: 184.106.153.149\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    delay(5000);
    client.stop();
    client.connect(server, 80); 
}



