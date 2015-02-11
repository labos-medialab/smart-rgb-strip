//definirani pwm izlaza
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;

boolean flag=0;

//definirane vrijednosti pwm izlaza
int redInt=0,greenInt=0,blueInt=0;

//stringovi
String inputString = "";//string koji ucita sa serijskog
String nullString = "";//string za reset
String prefixString = "";//pomocni string
String sufixString = "";//pomocni string
String percentString = "%";//pomocni string

void setup() {

  Serial.begin(9600);//serijska veza na 9600bps

  //definiranje outputa
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
}

void loop() {

  //demo pri paljenju
  if (inputString == nullString && flag == 0){
    flag=1;
    lightShow();
  }
  
  else if(inputString.indexOf(":")>0) pretvorba();
  else if(inputString == "pf") pulse(2,1500,255);
  else if(inputString == "df") dynamicRGB(20,255);
  else if(inputString == "ls") lightShow();
  
  else{
    if(inputString == "red") redInt=255;
    else if(inputString == "green") greenInt=255;
    else if(inputString == "blue") blueInt=255;
    
    else if(inputString == "red++")  { if(redInt<255) redInt++;}
    else if(inputString == "green++"){ if(greenInt<255) greenInt++;}
    else if(inputString == "blue++") { if(blueInt<255) blueInt++;}
    
    else if(inputString == "red--")  { if (redInt>0) redInt--;}
    else if(inputString == "green--"){ if(greenInt>0) greenInt--;}
    else if(inputString == "blue--") { if(blueInt>0) blueInt--;}
    
    else if(inputString == "r") { redInt=255;greenInt=0;blueInt=0;}
    else if(inputString == "o") { redInt=255;greenInt=128;blueInt=0;}
    else if(inputString == "y") { redInt=255;greenInt=255;blueInt=0;}
    else if(inputString == "yg"){ redInt=128;greenInt=255;blueInt=0;}
    
    else if(inputString == "g") { redInt=0;greenInt=255;blueInt=0;}
    else if(inputString == "gb"){ redInt=0;greenInt=255;blueInt=128;}
    else if(inputString == "c") { redInt=0;greenInt=255;blueInt=255;}
    else if(inputString == "a") { redInt=0;greenInt=128;blueInt=255;}
    
    else if(inputString == "b") { redInt=0;greenInt=0;blueInt=255;}
    else if(inputString == "v") { redInt=128;greenInt=0;blueInt=255;}
    else if(inputString == "m") { redInt=255;greenInt=0;blueInt=255;}
    else if(inputString == "p") { redInt=255;greenInt=0;blueInt=128;}
    
    else if(inputString == "w"){ redInt=255;greenInt=255;blueInt=255;}
    else if(inputString != nullString){ redInt=0;greenInt=0;blueInt=0;}
    
    staticRGB(redInt,greenInt,blueInt);
  }
  delay(100);
}

void serialEvent() {
  inputString=nullString;
  while (Serial.available() > 0) {
    inputString += (char)Serial.read();
    delay(10);
  }
}

void pretvorba(){
  
  int tempInt=0;
  String tempString = "";
  
  //odvajanje stringa
  prefixString = inputString.substring(0,inputString.indexOf(':'));
  sufixString = inputString.substring(inputString.indexOf(':')+1,inputString.length());
  
  if (prefixString == "#"){
    int tempIntArray[6];
    
    for(int i=0;i<6;i++){
      tempInt=sufixString.charAt(i);
      if(tempInt<=57) tempIntArray[i]=tempInt - 48;
      if(tempInt>64 && tempInt<71) tempIntArray[i]=tempInt - 55;
      if(tempInt>95 && tempInt<103) tempIntArray[i]=tempInt - 87;
    }
    
    redInt = tempIntArray[0]*16 + tempIntArray[1];
    greenInt = tempIntArray[2]*16 + tempIntArray[3];
    blueInt = tempIntArray[4]*16 + tempIntArray[5];
    
    staticRGB(redInt,greenInt,blueInt);
  }
  
  else if (prefixString == "rgb"){
    
    tempString = sufixString.substring(0,sufixString.indexOf(','));
    redInt = tempString.toInt();
    
    tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.lastIndexOf(','));
    greenInt = tempString.toInt();
    
    tempString = sufixString.substring(sufixString.lastIndexOf(',')+1,sufixString.length());
    blueInt = tempString.toInt();
    
    staticRGB(redInt,greenInt,blueInt);
  }
  
  else if (prefixString == "df"){  
    int FADESPEED = 10;
    int Brightness = 100;
    
    tempString = sufixString.substring(0,sufixString.indexOf(','));
    FADESPEED = tempString.toInt();
    
    tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.length());
    Brightness = tempString.toInt();
    
    
    dynamicRGB(FADESPEED, Brightness);
  }
  
  else if (prefixString == "pf"){
    
    int pulsN0 = 2;
    int FADESPEED = 10;
    int Brightness = 100;
    
    tempString = sufixString.substring(0,sufixString.indexOf(','));
    pulsN0 = tempString.toInt();
    
    tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.lastIndexOf(','));
    FADESPEED = tempString.toInt();
    
    tempString = sufixString.substring(sufixString.lastIndexOf(',')+1,sufixString.length());
    Brightness = tempString.toInt();
    
    pulse(pulsN0,FADESPEED,Brightness);
  }
  
  else if(prefixString == "red"){
    if(sufixString.endsWith(percentString)==0){
      redInt = sufixString.toInt();
    }
    
    else {
      sufixString = sufixString.substring(0,sufixString.length()-1);
      redInt = sufixString.toInt();
      redInt = map(redInt, 0, 100, 0, 255);
    }
    
    staticRGB(redInt,greenInt,blueInt);
  }
  
  else if(prefixString == "green"){
    if(sufixString.endsWith(percentString)==0){
      greenInt = sufixString.toInt();
    }
    
    else {
      sufixString = sufixString.substring(0,sufixString.length()-1);
      greenInt = sufixString.toInt();
      greenInt = map(greenInt, 0, 100, 0, 255);
    }
    
    staticRGB(redInt,greenInt,blueInt);
  }
  
  else if(prefixString == "blue"){
    if(sufixString.endsWith(percentString)==0){
      blueInt = sufixString.toInt();
    }
    
    else {
      sufixString = sufixString.substring(0,sufixString.length()-1);
      blueInt = sufixString.toInt();
      blueInt = map(blueInt, 0, 100, 0, 255);
    }
    
    staticRGB(redInt,greenInt,blueInt);
  }
  
  //nekakve druge naredbe
  
  else if(prefixString == "r"){//red
  
    if(sufixString.endsWith(percentString)==1){
      sufixString = sufixString.substring(0,sufixString.length()-1);
    }
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 255);
    greenInt=0;
    blueInt=0;
    staticRGB(redInt,greenInt,blueInt);
  }
  else if(prefixString == "o"){//orange
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
      
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 255);
    greenInt = map(tempInt, 0, 100, 0, 128);
    blueInt=0;
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "y"){//y
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
      
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 255);
    greenInt = map(tempInt, 0, 100, 0, 255);
    blueInt=0;
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "yg" || prefixString == "gy"){//yellow-green
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 128);
    greenInt = map(tempInt, 0, 100, 0, 255);
    blueInt=0;
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "g"){//green
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt=0;
    greenInt = map(tempInt, 0, 100, 0, 255);
    blueInt=0;
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "gb" || prefixString == "bg"){//green-blue
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt=0;
    greenInt = map(tempInt, 0, 100, 0, 255);
    blueInt = map(tempInt, 0, 100, 0, 128);
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "c"){//cyan
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt=0;
    greenInt = map(tempInt, 0, 100, 0, 255);
    blueInt = map(tempInt, 0, 100, 0, 255);
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "a"){//azure
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt=0;
    greenInt = map(tempInt, 0, 100, 0, 128);
    blueInt = map(tempInt, 0, 100, 0, 255);
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "b"){//blue
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt=0;
    greenInt=0;
    blueInt = map(tempInt, 0, 100, 0, 255);
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "v"){//violet
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 128);
    greenInt=0;
    blueInt = map(tempInt, 0, 100, 0, 255);
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "m"){//magenta
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 255);
    greenInt=0;
    blueInt = map(tempInt, 0, 100, 0, 255);
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "p"){//pink
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 255);
    greenInt=0;
    blueInt = map(tempInt, 0, 100, 0, 128);
    staticRGB(redInt,greenInt,blueInt); 
  }
  else if(prefixString == "w"){//pink
  
    if(sufixString.endsWith(percentString)==1)
      sufixString = sufixString.substring(0,sufixString.length()-1);
    
    tempInt = sufixString.toInt();
    
    redInt = map(tempInt, 0, 100, 0, 255);
    greenInt = map(tempInt, 0, 100, 0, 255);
    blueInt = map(tempInt, 0, 100, 0, 255);
    staticRGB(redInt,greenInt,blueInt); 
  }  
  else{} 
}//pretvorba

void dynamicRGB(int fs, int b){
  delay(10);
  int a;
  
  while(redInt +  greenInt  + blueInt != 0){
    if(redInt != 0) redInt--;
    if(greenInt != 0) greenInt--;
    if(blueInt != 0) blueInt--;
    
    analogWrite(redPin, redInt);
    analogWrite(greenPin, greenInt);
    analogWrite(bluePin, blueInt);
    
    if (Serial.available() > 0) break;
    
    delay(fs);
  }
  

  
  delay(100);
  
  for (a = 0; a < b+1; a++) { 
    if (Serial.available() > 0) break;
    analogWrite(redPin, a);
    delay(fs);
  } 
  
  while (1){
    
    for (a = 0; a < b+1; a++) {
      if (Serial.available() > 0) break; 
      analogWrite(bluePin, a);
      delay(fs);
    } 
    
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;
      analogWrite(redPin, a);
      delay(fs);
    }
    
    for (a = 0; a < b+1; a++) { 
      if (Serial.available() > 0) break;
      analogWrite(greenPin, a);
      delay(fs);
    } 
  
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;
      analogWrite(bluePin, a);
      delay(fs);
    } 
  
    for (a = 0; a < b+1; a++) { 
      if (Serial.available() > 0) break;
      analogWrite(redPin, a);
      delay(fs);
    } 
    
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;
      analogWrite(greenPin, a);
      delay(fs);
    }
  
    if (Serial.available() > 0) break;
  }
}

void lightShow()
{
  while (1)
  {
    int microphone_value = analogRead(A5);
    //Serial.println(microphone_value);
    if (Serial.available() > 0) break;
  
    redInt = blueInt = greenInt = 0;
    
    if (microphone_value > 480 && microphone_value < 490)
      redInt = map(microphone_value, 480, 490, 0, 20);
    if (microphone_value > 490 && microphone_value < 520)
      blueInt = map(microphone_value, 490, 520, 0, 30);
    if (microphone_value > 520 && microphone_value < 540)
      greenInt = map(microphone_value, 520, 540, 0, 40);
    if (microphone_value > 540 && microphone_value < 600)
      redInt = greenInt = map(microphone_value, 540, 600, 0, 40);
    if (microphone_value > 600 && microphone_value < 1000)
      redInt = greenInt = blueInt = map(microphone_value, 600, 1000, 0, 255);
     
    analogWrite(redPin, redInt);
    analogWrite(greenPin, greenInt);
    analogWrite(bluePin, blueInt);
  }
}
  
void staticRGB(int a, int b, int c){
  inputString=nullString;
  analogWrite(redPin, a);
  analogWrite(greenPin, b);
  analogWrite(bluePin, c);
}

void pulse(int p,int fs, int b){
  
  int a=0,bp=0;
  
  do{
    if (Serial.available() > 0) break;
    bp++;
    for (a = 0; a < b+1; a++) {
      if (Serial.available() > 0) break;
      analogWrite(redPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
    for (a = b; a > 0; a--) {
      if (Serial.available() > 0) break;  
      analogWrite(redPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
  }
  while(bp<p);bp=0;
  
  do{
    if (Serial.available() > 0) break;
    bp++;
    for (a = 0; a < b+1; a++) {
      if (Serial.available() > 0) break;
      analogWrite(redPin, a);
      analogWrite(bluePin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;  
      analogWrite(redPin, a);
      analogWrite(bluePin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
  }
  
  while(bp<p);bp=0;
  
  do{
    if (Serial.available() > 0) break;
    bp++;
    for (a = 0; a < b+1; a++) {
      if (Serial.available() > 0) break;
      analogWrite(bluePin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;  
      analogWrite(bluePin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
  }
  while(bp<p);bp=0;
  
  do{
    if (Serial.available() > 0) break;
    bp++;
    for (a = 0; a < b+1; a++) {
    if (Serial.available() > 0) break;
      analogWrite(bluePin, a);
      analogWrite(greenPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;  
      analogWrite(bluePin, a);
      analogWrite(greenPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
  }
  while(bp<p);bp=0;

  do{
    if (Serial.available() > 0) break;
    bp++;
    for (a = 0; a < b+1; a++) {
      if (Serial.available() > 0) break;
      analogWrite(greenPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;  
      analogWrite(greenPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
  }
  while(bp<p);bp=0;
  
  do{
    if (Serial.available() > 0) break;
    bp++;
    for (a = 0; a < b+1; a++) {
      if (Serial.available() > 0) break; 
      analogWrite(greenPin, a);
      analogWrite(redPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;  
      analogWrite(greenPin, a);
      analogWrite(redPin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
  }
  while(bp<p);bp=0;
  
  do{
    if (Serial.available() > 0) break;
    bp++;
    for (a = 0; a < b+1; a++) {
      if (Serial.available() > 0) break; 
      analogWrite(greenPin, a);
      analogWrite(redPin, a);
      analogWrite(bluePin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
    for (a = b; a > 0; a--) { 
      if (Serial.available() > 0) break;  
      analogWrite(greenPin, a);
      analogWrite(redPin, a);
      analogWrite(bluePin, a);
      delayMicroseconds(fs);
    }
    if (Serial.available() > 0) break;
  }
  while(bp<p);bp=0;
}