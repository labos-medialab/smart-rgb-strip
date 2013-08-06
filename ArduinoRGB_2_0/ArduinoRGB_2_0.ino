//definirani pwm izlaza
const int r1Pin = 3; const int r2Pin = 9;
const int g1Pin = 5; const int g2Pin = 10;
const int b1Pin = 6; const int b2Pin = 11;

boolean flag=0;

//definirane vrijednosti pwm izlaza
int r1Int=0, g1Int=0, b1Int=0,
    r2Int=0, g2Int=0, b2Int=0;

//stringovi
String inputString = "";//string koji učita sa serijskog
String nullString = "";//string za reset
String prefixString = "";//pomoćni string
String sufixString = "";//pomoćni string
String percentString = "%";//pomoćni string

void setRGB(){
  analogWrite(r1Pin, r1Int); analogWrite(r2Pin, r2Int);
  analogWrite(g1Pin, g1Int); analogWrite(g2Pin, g2Int);
  analogWrite(b1Pin, b1Int); analogWrite(b2Pin, b2Int);
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
}

void setup(){
  Serial.begin(9600);//serijska veza na 9600bps
  
  //definiranje outputa
  pinMode(r1Pin, OUTPUT); pinMode(r2Pin, OUTPUT);
  pinMode(g1Pin, OUTPUT); pinMode(g2Pin, OUTPUT);
  pinMode(b1Pin, OUTPUT); pinMode(b2Pin, OUTPUT);
}

void loop(){
  if(flag==0) printValue();
}


