/* This code is for programming EEPROM 28C16 as a Control Matrix 1
 * Version 1.0 - WRITING DATA TO EEPROM
 * NOTE: NEED TO RESET EEPROM INITIALLY TO 0 */

int address[] = {12,11,10,9,8,7,6,5,4,3,2};
int data[] = {36,34,32,30,28,26,24,22}; // (IO: 7, 6, ..., 0) or (CP, EP, ..., EA)
int WE = 13;
int dt = 100;

bool FetchCycle[3][8]{
 // CP  EP  LM  EM  LI  EI  LA  EA
  { 0,  1,  1,  0,  0,  0,  0,  0 },  // Load to Memory
  { 1,  0,  0,  0,  0,  0,  0,  0 },  // Next Instruction
  { 0,  0,  0,  1,  1,  0,  0,  0 }   // Load to Instruction Register
};

bool Instruction[10][7][8]{
{ // LDA
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  1,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // ADD
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  1,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // SUB
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  1,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // XOR
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  1,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ //AND
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  1,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // OR
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  1,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // NAND
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  1,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // NOR
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  1,  0,  0,  1,  0,  0 },  // T3
   { 0,  0,  0,  1,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  1,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // OUT
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  0,  0,  0,  0,  0,  1 },  // T3
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
},
{ // HLT
  // CP  EP  LM  EM  LI  EI  LA  EA
   { 0,  1,  1,  0,  0,  0,  0,  0 },  // T0
   { 1,  0,  0,  0,  0,  0,  0,  0 },  // T1
   { 0,  0,  0,  1,  1,  0,  0,  0 },  // T2
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T3
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T4
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T5
   { 0,  0,  0,  0,  0,  0,  0,  0 },  // T6
}};

bool Operations[10][4]{
  // I3  I2  I1  I0
   { 0,  0,  0,  0 },  // LDA
   { 0,  0,  0,  1 },  // ADD
   { 0,  0,  1,  0 },  // SUB
   { 0,  0,  1,  1 },  // XOR
   { 0,  1,  0,  0 },  // AND
   { 0,  1,  0,  1 },  // OR
   { 0,  1,  1,  0 },  // NAND
   { 0,  1,  1,  1 },  // NOR
   { 1,  1,  1,  0 },  // OUT
   { 1,  1,  1,  1 }   // HLT
};

void Write(){
  digitalWrite(WE, HIGH);
  delay(10);
  digitalWrite(WE, LOW);
  delayMicroseconds(1);
  digitalWrite(WE, HIGH);
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

void setAdd(int op, int T){
  // Print Address
  Serial.print("Operation: ");
  Serial.print(op);
  Serial.print("\t\tBinary Address: ");
  Serial.print(Operations[op][0]);
  Serial.print(Operations[op][1]);
  Serial.print(Operations[op][2]);
  Serial.print(Operations[op][3]);
  
  // Set Operation
  digitalWrite(address[0], Operations[op][0]);
  digitalWrite(address[1], Operations[op][1]);
  digitalWrite(address[2], Operations[op][2]);
  digitalWrite(address[3], Operations[op][3]);

  // Set Instructions
  for(int t=6; t>=0; t--){
    int i = 0;
    if(t == T){i = 1;}
    digitalWrite(address[10-t], i);
    Serial.print(i); 
  }
  Serial.print("\t\t");
  delay(dt);
}

void setData(int op, int T){
  Serial.print("Binary Data: ");
  for (int i=0; i<8; i++){
    digitalWrite(data[i], Instruction[op][T][i]);
    Serial.print(Instruction[op][T][i]);
  }
  delay(dt);
  Serial.println();
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

  pinMode(WE, OUTPUT);

  Serial.begin(9600);
  Serial.print("Serial Monitor Starts\n\n");
  Serial.print("WRITING DATA TO EEPROM 28C16\n");

  // Writing Data
  for(int op=0; op<10; op++){
    for(int T=0; T<7; T++){
      setAdd(op, T);
      setData(op, T);
      Write();
      delay(dt);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
