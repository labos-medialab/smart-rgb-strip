//definirani pwm izlaza
const int r1Pin = 3, r2Pin = 9,
          g1Pin = 5, g2Pin = 10,
          b1Pin = 6, b2Pin = 11;

//definirane vrijednosti pwm izlaza
int r1Int=0, g1Int=0, b1Int=0,
    r2Int=0, g2Int=0, b2Int=0,
    newr1Int=0, newg1Int=0, newb1Int=0;
    
int sFS=2,
    dFS=50,dB=255,
    pFS=1500,pB=255;

int fnc=-1;

boolean rFlag=1,gFlag=1,bFlag=1;

//stringovi
String inputString = "";//string koji učita sa serijskog
String nullString = "";//string za reset
String prefixString = "";//pomoćni string
String sufixString = "";//pomoćni string
//String percentString = "%";//pomoćni string

void setRGB(){
  fnc=0;
  while(newr1Int != r1Int || newg1Int != g1Int || newb1Int != b1Int){
    
    if(r1Int < newr1Int) r1Int++;
    if(r1Int > newr1Int) r1Int--;
    if(g1Int < newg1Int) g1Int++;
    if(g1Int > newg1Int) g1Int--;
    if(b1Int < newb1Int) b1Int++;
    if(b1Int > newb1Int) b1Int--;
    
    if(Serial.available() > 0) serialEvent();    
    delay(sFS);
    
    analogWrite(r1Pin, r1Int); analogWrite(r2Pin, r2Int);
    analogWrite(g1Pin, g1Int); analogWrite(g2Pin, g2Int);
    analogWrite(b1Pin, b1Int); analogWrite(b2Pin, b2Int);
  }
  while(1) {
    if(Serial.available() > 0) serialEvent();
  }
}

void setup(){
  analogWrite(r1Pin, 255); analogWrite(r2Pin, 255);delay(150);
  analogWrite(r1Pin, 0); analogWrite(r2Pin, 0);delay(150);
  analogWrite(g1Pin, 255); analogWrite(g2Pin, 255);delay(150);
  analogWrite(g1Pin, 0); analogWrite(g2Pin, 0);delay(150);
  analogWrite(b1Pin, 255); analogWrite(b2Pin, 255);delay(150);
  analogWrite(b1Pin, 0); analogWrite(b2Pin, 0);
  
  Serial.begin(9600);//serijska veza na 9600bps
  
  //definiranje outputa
  pinMode(r1Pin, OUTPUT); pinMode(r2Pin, OUTPUT);
  pinMode(g1Pin, OUTPUT); pinMode(g2Pin, OUTPUT);
  pinMode(b1Pin, OUTPUT); pinMode(b2Pin, OUTPUT);
}

void loop(){}

void serialEvent() {
  String tempString = "";
  inputString=nullString;
  while (Serial.available() > 0) {
    inputString += (char)Serial.read();
    delay(10);
  }
  
  if(inputString=="dF") if(fnc!=1)dynamicRGB();
  if(inputString=="pF") {
    rFlag=1;gFlag=1;bFlag=1;
    pFS=1500,pB=255;
    if(fnc!=2)pulse();
  }
  if(inputString.startsWith("pF") && inputString.indexOf(":")<0 && inputString.indexOf(";")>0){
    if(inputString.indexOf("r")>0)rFlag=1;
    else rFlag=0;
    if(inputString.indexOf("g")>0)gFlag=1;
    else gFlag=0;
    if(inputString.indexOf("b")>0)bFlag=1;
    else bFlag=0;
  }
  
  if(inputString.startsWith("#")){
    int tempIntArray[6];
    
    for(int i=0;i<6;i++){
      int tempInt=inputString.charAt(i+1);
      if(tempInt<=57) tempIntArray[i]=tempInt - 48;
      if(tempInt>64 && tempInt<71) tempIntArray[i]=tempInt - 55;
      if(tempInt>95 && tempInt<103) tempIntArray[i]=tempInt - 87;
    }
    newr1Int = tempIntArray[0]*16 + tempIntArray[1];
    newg1Int = tempIntArray[2]*16 + tempIntArray[3];
    newb1Int = tempIntArray[4]*16 + tempIntArray[5];
    setRGB();
  }
  
  if(inputString.indexOf(":")>0){
    prefixString = inputString.substring(0,inputString.indexOf(':'));
    sufixString = inputString.substring(inputString.indexOf(':')+1,inputString.length());
    
    if(prefixString == "rgb"){
      tempString = sufixString.substring(0,sufixString.indexOf(','));
      newr1Int = tempString.toInt();
      tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.lastIndexOf(','));
      newg1Int = tempString.toInt();
      tempString = sufixString.substring(sufixString.lastIndexOf(',')+1,sufixString.length());
      newb1Int = tempString.toInt();
      setRGB();
    }
    
    if(prefixString == "sFS") sFS = sufixString.toInt();
    if(prefixString == "dFS") dFS = sufixString.toInt();
    if(prefixString == "dB") dB = sufixString.toInt();
    
    if(prefixString == "dF"){
      tempString = sufixString.substring(0,sufixString.indexOf(','));
      dFS = tempString.toInt();
      tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.length());
      dB = tempString.toInt();
      if(fnc!=1) dynamicRGB();
    }
    if(prefixString == "pF"){
      if(sufixString.indexOf(";")>0){
        if(sufixString.indexOf("r")>0)rFlag=1;
        else rFlag=0;
        if(sufixString.indexOf("g")>0)gFlag=1;
        else gFlag=0;
        if(sufixString.indexOf("b")>0)bFlag=1;
        else bFlag=0;
        sufixString=sufixString.substring(0,sufixString.indexOf(';'));
      }
      else{
        tempString = sufixString.substring(0,sufixString.indexOf(','));
        pFS = tempString.toInt();
        tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.length());
        pB = tempString.toInt();
        rFlag=1;gFlag=1;bFlag=1;
      }
      if(fnc!=2) pulse();
    }
  }
}

void dynamicRGB(){
  fnc=1;
  delay(10);
  while(r1Int +  g1Int  + b1Int != 0){
    if(r1Int != 0) r1Int--;
    if(g1Int != 0) g1Int--;
    if(b1Int != 0) b1Int--;
    analogWrite(r1Pin, r1Int);
    analogWrite(g1Pin, g1Int);
    analogWrite(b1Pin, b1Int);
    if (Serial.available() > 0) serialEvent();
    delay(dFS);
  }
  delay(10);
  for (r1Int = 0; r1Int < dB+1; r1Int++){
    if (Serial.available() > 0) serialEvent();
    analogWrite(r1Pin, r1Int);
    delay(dFS);
  } 
  while (1){
    for (b1Int = 0; b1Int < dB+1; b1Int++){
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(b1Pin, b1Int);
      delay(dFS);
    } 
    for (r1Int = dB; r1Int > 0; r1Int--){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(r1Pin, r1Int);
      delay(dFS);
    }
    for (g1Int = 0; g1Int < dB+1; g1Int++){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(g1Pin, g1Int);
      delay(dFS);
    }
    for (b1Int = dB; b1Int > 0; b1Int--){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(b1Pin, b1Int);
      delay(dFS);
    } 
    for (r1Int = 0; r1Int < dB+1; r1Int++){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(r1Pin, r1Int);
      delay(dFS);
    } 
    for (g1Int = dB; g1Int > 0; g1Int--){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(g1Pin, g1Int);
      delay(dFS);
    }
    if (Serial.available() > 0) serialEvent(); 
  }
}

void pulse(){
  fnc=2;
  delay(10);
  while(r1Int +  g1Int  + b1Int != 0){
    if(r1Int != 0) r1Int--;
    if(g1Int != 0) g1Int--;
    if(b1Int != 0) b1Int--;
    analogWrite(r1Pin, r1Int);
    analogWrite(g1Pin, g1Int);
    analogWrite(b1Pin, b1Int);
    if (Serial.available() > 0) serialEvent();
    delayMicroseconds(pFS);
  }
  while(1){
    if(rFlag){
      for(r1Int=0; r1Int<=pB; r1Int++){
        analogWrite(r1Pin, r1Int);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) serialEvent();
      }
      for(r1Int=pB; r1Int>0; r1Int--){
        analogWrite(r1Pin, r1Int);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) serialEvent();
      }
    }
    if(gFlag){
      for(g1Int=0; g1Int<=pB; g1Int++){
        analogWrite(g1Pin, g1Int);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) serialEvent();
      }
      for(g1Int=pB; g1Int>0; g1Int--){
        analogWrite(g1Pin, g1Int);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) serialEvent();
      }
    }
    if(bFlag){
      for(b1Int=0; b1Int<=pB; b1Int++){
        analogWrite(b1Pin, b1Int);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) serialEvent();
      }
      for(b1Int=pB; b1Int>0; b1Int--){
        analogWrite(b1Pin, b1Int);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) serialEvent();
      }
    }
    if (Serial.available() > 0) serialEvent();
  }
}
