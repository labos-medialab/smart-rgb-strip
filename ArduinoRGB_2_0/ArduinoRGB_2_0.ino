//definirani pwm izlaza
const int r1Pin = 3; const int r2Pin = 9;
const int g1Pin = 5; const int g2Pin = 10;
const int b1Pin = 6; const int b2Pin = 11;

boolean flag=0;

//definirane vrijednosti pwm izlaza
int r1Int=0, g1Int=0, b1Int=0,
    r2Int=0, g2Int=0, b2Int=0;
    
int fs=50,b=255;

int fnc=-1;

//stringovi
String inputString = "";//string koji učita sa serijskog
String nullString = "";//string za reset
String prefixString = "";//pomoćni string
String sufixString = "";//pomoćni string
String percentString = "%";//pomoćni string

void setRGB(){
  fnc=0;
  analogWrite(r1Pin, r1Int); analogWrite(r2Pin, r2Int);
  analogWrite(g1Pin, g1Int); analogWrite(g2Pin, g2Int);
  analogWrite(b1Pin, b1Int); analogWrite(b2Pin, b2Int);
  while(1) {
    if(Serial.available() > 0) break;
  }
}

void printValue(){
  flag=1;
  Serial.print("Strip1: ");
  Serial.print(r1Int);
  Serial.print(", ");
  Serial.print(g1Int);
  Serial.print(", ");
  Serial.println(b1Int);

  Serial.print("Strip2: ");
  Serial.print(r2Int);
  Serial.print(", ");
  Serial.print(g2Int);
  Serial.print(", ");
  Serial.println(b2Int);
  Serial.println();
  
  Serial.println(prefixString);
  Serial.println(sufixString);
}

void setup(){
  Serial.begin(9600);//serijska veza na 9600bps
  
  //definiranje outputa
  pinMode(r1Pin, OUTPUT); pinMode(r2Pin, OUTPUT);
  pinMode(g1Pin, OUTPUT); pinMode(g2Pin, OUTPUT);
  pinMode(b1Pin, OUTPUT); pinMode(b2Pin, OUTPUT);
}

void loop(){ }

void serialEvent() {
  String tempString = "";
  flag=0;
  inputString=nullString;
  while (Serial.available() > 0) {
    inputString += (char)Serial.read();
    delay(10);
  }
  
  if (inputString.startsWith("#")){
    int tempIntArray[6];
    
    for(int i=0;i<6;i++){
      int tempInt=inputString.charAt(i+1);
      if(tempInt<=57) tempIntArray[i]=tempInt - 48;
      if(tempInt>64 && tempInt<71) tempIntArray[i]=tempInt - 55;
      if(tempInt>95 && tempInt<103) tempIntArray[i]=tempInt - 87;
    }
    
    r1Int = tempIntArray[0]*16 + tempIntArray[1];
    g1Int = tempIntArray[2]*16 + tempIntArray[3];
    b1Int = tempIntArray[4]*16 + tempIntArray[5];
    
    setRGB();
  }
  
  if(inputString.indexOf(":")>0){
    prefixString = inputString.substring(0,inputString.indexOf(':'));
    sufixString = inputString.substring(inputString.indexOf(':')+1,inputString.length());
    
    if(prefixString == "rgb"){
      tempString = sufixString.substring(0,sufixString.indexOf(','));
      r1Int = tempString.toInt();
      tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.lastIndexOf(','));
      g1Int = tempString.toInt();
      tempString = sufixString.substring(sufixString.lastIndexOf(',')+1,sufixString.length());
      b1Int = tempString.toInt();
      setRGB();
    }
    
    if(prefixString == "dF"){
      tempString = sufixString.substring(0,sufixString.indexOf(','));
      fs = tempString.toInt();
      tempString = sufixString.substring(sufixString.indexOf(',')+1,sufixString.length());
      b = tempString.toInt();
      if(fnc!=1) dynamicRGB();
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
    delay(fs);
  }
  delay(10);
  for (r1Int = 0; r1Int < b+1; r1Int++){
    if (Serial.available() > 0) serialEvent();
    analogWrite(r1Pin, r1Int);
    delay(fs);
  } 
  while (1){
    for (b1Int = 0; b1Int < b+1; b1Int++){
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(b1Pin, b1Int);
      delay(fs);
    } 
    for (r1Int = b; r1Int > 0; r1Int--){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(r1Pin, r1Int);
      delay(fs);
    }
    for (g1Int = 0; g1Int < b+1; g1Int++){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(g1Pin, g1Int);
      delay(fs);
    }
    for (b1Int = b; b1Int > 0; b1Int--){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(b1Pin, b1Int);
      delay(fs);
    } 
    for (r1Int = 0; r1Int < b+1; r1Int++){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(r1Pin, r1Int);
      delay(fs);
    } 
    for (g1Int = b; g1Int > 0; g1Int--){ 
      if (Serial.available() > 0) serialEvent(); 
      analogWrite(g1Pin, g1Int);
      delay(fs);
    }
    if (Serial.available() > 0) serialEvent(); 
  }
}
