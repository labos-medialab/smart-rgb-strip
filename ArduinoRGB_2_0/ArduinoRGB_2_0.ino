//definirani pwm izlaza
const int rPin = 3,
          gPin = 5,
          bPin = 6;

//definirane vrijednosti pwm izlaza
int rInt=0, gInt=0, bInt=0,
    newrInt=0, newgInt=0, newbInt=0;
    
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

void setRGB(bool timeout){
  fnc=0;
  while(newrInt != rInt || newgInt != gInt || newbInt != bInt){
    
    if(rInt < newrInt) rInt++;
    if(rInt > newrInt) rInt--;
    if(gInt < newgInt) gInt++;
    if(gInt > newgInt) gInt--;
    if(bInt < newbInt) bInt++;
    if(bInt > newbInt) bInt--;
    
    if(Serial.available() > 0) serialEvent();    
    if(timeout)delay(sFS);
    
    analogWrite(rPin, rInt);
    analogWrite(gPin, gInt);
    analogWrite(bPin, bInt);
  }
}

void setup(){
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);

  newrInt = 255; newgInt = 0; newbInt = 0; setRGB(1);
  newrInt = 0; newgInt = 0; newbInt = 0; setRGB(1);
  newrInt = 0; newgInt = 255; newbInt = 0; setRGB(1);
  newrInt = 0; newgInt = 0; newbInt = 0; setRGB(1);
  newrInt = 0; newgInt = 0; newbInt = 255; setRGB(1);
  newrInt = 0; newgInt = 0; newbInt = 0; setRGB(1);
  
  Serial.begin(9600);//serijska veza na 9600bps
  randomSeed(analogRead(0));

  lightShow();
}

void loop(){

}

void serialEvent() {
  String tempString = "";
  inputString = Serial.readStringUntil('\n');
  Serial.println(inputString);
  
  if(inputString.indexOf(":")>0){
    prefixString = inputString.substring(0,inputString.indexOf(':'));
    sufixString = inputString.substring(inputString.indexOf(':')+1,inputString.length());
    
    if(prefixString == "rgb"){
      fnc=-1;
      tempString = sufixString.substring(0,sufixString.indexOf(','));
      newrInt = tempString.toInt();
      tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.lastIndexOf(','));
      newgInt = tempString.toInt();
      tempString = sufixString.substring(sufixString.lastIndexOf(',')+1,sufixString.length());
      newbInt = tempString.toInt();
      setRGB(1);
    }
    
    if(prefixString == "df"){
      tempString = sufixString.substring(0,sufixString.indexOf(','));
      dFS = tempString.toInt();
      tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.length());
      dB = tempString.toInt();
      if(fnc!=1) dynamicRGB();
    }
    if(prefixString == "pf"){
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
  
  if(inputString=="df") if(fnc!=1)dynamicRGB();
  if(inputString=="ls") if(fnc!=3)lightShow();
  if(inputString=="pf") {
    rFlag=1;gFlag=1;bFlag=1;
    pFS=1500,pB=255;
    if(fnc!=2)pulse();
  }
  if(inputString=="off"){
    fnc=-1;
    newrInt = 0;
    newgInt = 0;
    newbInt = 0;
    setRGB(1);
  }
  
  if(inputString.startsWith("help")){
    Serial.print(F("LOL\nno help for you!\n"));
  }
  
  if(inputString.startsWith("pf") && inputString.indexOf(":")<0 && inputString.indexOf(";")>0){
    if(inputString.indexOf("r")>0)rFlag=1;
    else rFlag=0;
    if(inputString.indexOf("g")>0)gFlag=1;
    else gFlag=0;
    if(inputString.indexOf("b")>0)bFlag=1;
    else bFlag=0;
  }
  
  if(inputString.startsWith("#")){
    int tempIntArray[6];
    fnc = -1;
    for(int i=0;i<6;i++){
      int tempInt=inputString.charAt(i+1);
      if(tempInt<=57) tempIntArray[i]=tempInt - 48;
      if(tempInt>64 && tempInt<71) tempIntArray[i]=tempInt - 55;
      if(tempInt>95 && tempInt<103) tempIntArray[i]=tempInt - 87;
    }
    newrInt = tempIntArray[0]*16 + tempIntArray[1];
    newgInt = tempIntArray[2]*16 + tempIntArray[3];
    newbInt = tempIntArray[4]*16 + tempIntArray[5];
    setRGB(1);
  }
}

void dynamicRGB(){
  fnc=1;
  delay(10);
  while(rInt +  gInt  + bInt != 0){
    if(rInt != 0) rInt--;
    if(gInt != 0) gInt--;
    if(bInt != 0) bInt--;
    analogWrite(rPin, rInt);
    analogWrite(gPin, gInt);
    analogWrite(bPin, bInt);
    if (Serial.available() > 0) {break; serialEvent();}
    delay(dFS);
  }
  delay(10);
  for (rInt = 0; rInt < dB+1; rInt++){
    if (Serial.available() > 0) {break; serialEvent();}
    analogWrite(rPin, rInt);
    delay(dFS);
  } 
  while (1){
    for (bInt = 0; bInt < dB+1; bInt++){
      if (Serial.available() > 0) {break; serialEvent();} 
      analogWrite(bPin, bInt);
      delay(dFS);
    } 
    for (rInt = dB; rInt > 0; rInt--){ 
      if (Serial.available() > 0) {break; serialEvent();} 
      analogWrite(rPin, rInt);
      delay(dFS);
    }
    for (gInt = 0; gInt < dB+1; gInt++){ 
      if (Serial.available() > 0) {break; serialEvent();} 
      analogWrite(gPin, gInt);
      delay(dFS);
    }
    for (bInt = dB; bInt > 0; bInt--){ 
      if (Serial.available() > 0) {break; serialEvent();} 
      analogWrite(bPin, bInt);
      delay(dFS);
    } 
    for (rInt = 0; rInt < dB+1; rInt++){ 
      if (Serial.available() > 0) {break; serialEvent();} 
      analogWrite(rPin, rInt);
      delay(dFS);
    } 
    for (gInt = dB; gInt > 0; gInt--){ 
      if (Serial.available() > 0) {break; serialEvent();} 
      analogWrite(gPin, gInt);
      delay(dFS);
    }
    if (Serial.available() > 0) {break; serialEvent();} 
  }
}

void pulse(){
  fnc=2;
  delay(10);
  while(rInt +  gInt  + bInt != 0){
    if(rInt != 0) rInt--;
    if(gInt != 0) gInt--;
    if(bInt != 0) bInt--;
    analogWrite(rPin, rInt);
    analogWrite(gPin, gInt);
    analogWrite(bPin, bInt);
    if (Serial.available() > 0) {break; serialEvent();}
    delayMicroseconds(pFS);
  }
  while(1){
    if(rFlag){
      for(rInt=0; rInt<=pB; rInt++){
        analogWrite(rPin, rInt);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) {break; serialEvent();}
      }
      for(rInt=pB; rInt>0; rInt--){
        analogWrite(rPin, rInt);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) {break; serialEvent();}
      }
    }
    if(gFlag){
      for(gInt=0; gInt<=pB; gInt++){
        analogWrite(gPin, gInt);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) {break; serialEvent();}
      }
      for(gInt=pB; gInt>0; gInt--){
        analogWrite(gPin, gInt);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) {break; serialEvent();}
      }
    }
    if(bFlag){
      for(bInt=0; bInt<=pB; bInt++){
        analogWrite(bPin, bInt);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) {break; serialEvent();}
      }
      for(bInt=pB; bInt>0; bInt--){
        analogWrite(bPin, bInt);
        delayMicroseconds(pFS);
        if (Serial.available() > 0) {break; serialEvent();}
      }
    }
    if (Serial.available() > 0) {break; serialEvent();}
  }
}

void lightShow(){
  fnc=3;
  delay(10);

  newrInt = 0; newgInt = 0; newbInt = 0; setRGB(1);
  sFS=4;

  /*
  int k[3]={0,1,2},k1[3];
  int tmp1 = random(3);
  int smjer = random(2);

  k1[0]=k[tmp1];
  int j=1;
  if(smjer){
    for(int i=0;i<=2;i++){
      if(i!=tmp1){
        k1[j]=k[i]; j++;
      }
    }
  }
  else{
    for(int i=2;i>=0;i--){
      if(i!=tmp1){
        k1[j]=k[i]; j++;
      }
    }
  }

  int randArray[3];
  int tmp = random(1000);

  randArray[k1[0]] = tmp;
  if(tmp>=500) randArray[k1[1]] = random(500);
  else randArray[k1[1]] = random(500,1000);
  randArray[k1[2]] = random(1000);

  sinR = randArray[0];
  sinG = randArray[1];
  sinB = randArray[2];

  int deltaR = random(50);
  int deltaG = random(50);
  int deltaB = random(50);

  if(sinR >= 500) deltaR=-deltaR;
  if(sinG >= 500) deltaG=-deltaG;
  if(sinB >= 500) deltaB=-deltaB;
  /**/

  int colorArray[]{
  	255,0,0,
  	255,128,0,
  	255,255,0,
  	128,255,0,
  	0,255,0,
  	0,255,128,
  	0,255,255,
  	0,128,255,
  	0,0,255,
  	128,0,255,
  	255,0,255,
  	255,0,128,
  	255,128,128,
  	128,255,128,
  	128,128,255,
  	255,255,128,
  	128,255,255,
  	255,128,255,
  	255,255,255
  };

  while(1){
    int micVal = analogRead(A5);
    if(micVal > 480){
      micVal = map(micVal, 480, 1024, 0, 255);

      int i = random(19), j=i*3;
      newrInt = colorArray[j];
      newgInt = colorArray[j+1];
      newbInt = colorArray[j+2];

      newrInt = map(newrInt, 0, 255, 0, micVal);
      newgInt = map(newgInt, 0, 255, 0, micVal);
      newbInt = map(newbInt, 0, 255, 0, micVal);
      setRGB(1);

      newrInt = 0;
      newgInt = 0;
      newbInt = 0;
      setRGB(1);
    }
    if(Serial.available() > 0) break;
  }
}
