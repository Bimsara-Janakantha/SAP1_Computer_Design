/* This code is used to reset EEPROM to all 0 */

int address[] = {12,11,10,9,8,7,6,5,4,3,2};
int data[] = {22,24,26,28,30,32,34,36};
int WE = 13;
int dt = 10;

void Write(){
  digitalWrite(WE, HIGH);
  delay(10);
  digitalWrite(WE, LOW);
  delay(2);
  digitalWrite(WE, HIGH);
}


void setAdd(int value){
  int i, Bit, binAddress[11];

  Serial.print("Decimal Address: ");
  Serial.print(value);
  Serial.print("\t Binary Address: ");
  
  for(i=10; i>=0; i--){
    Bit = (value & 1);
    binAddress[i] = Bit;
    value = value >> 1;
  }

  for(i=0; i<11; i++){
    Serial.print(binAddress[i]);
    Serial.print(" ");
    digitalWrite(address[i], binAddress[i]);
  }

  Serial.print("  \t");

  return;
}


void printData(){
  int i, value;
  Serial.print("Data Values: ");
  
  for(i=7; i>=0; i--){
    value = digitalRead(data[i]);
    Serial.print(value);
    Serial.print(" ");
    delay(10);
  }
  
  Serial.print("\n");
  
  return;  
}


void setup() {
  // Initializing Address Pins
  for(int i=0; i<11; i++){
    pinMode(address[i], OUTPUT);
  }


  // Initializing Data Pins
  for(int i=0; i<8; i++){
    pinMode(data[i], OUTPUT);
  }
  
  // ReadOnly
  digitalWrite(WE, 1);
  
  Serial.begin(9600);
  Serial.print("Serial Monitor Starts\n\n");
  Serial.print("READING DATA FROM EEPROM 28C16\n");

  for(int i=0; i<2048; i++){
    setAdd(i);
    Serial.println();
    delay(2);
    for(int i=0; i<7; i++){
      digitalWrite(data[i], 0);
      delay(dt);
      Write();
      delay(dt);
    }
  }

  Serial.println("Done");

  //setAdd(66);
  //Write();

}

void loop() {
  
}
