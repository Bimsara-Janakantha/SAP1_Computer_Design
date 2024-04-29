/* This code is for programming EEPROM 28C16 as a 4 digit 7 Segment display driver 
 * Version 3.4 - WRITING DATA TO EEPROM  - Common Anode Mode - Sign and Magnitude Notation - Digits from Left to right
 * Created by: Janakantha S.M.B.G.
 * Last Modified: 12-04-2024 */


int address[] = {12,11,10,9,8,7,6,5,4,3,2};
int data[] = {22,24,26,28,30,32,34,36};
int WE = 13;
int dt = 100;

int digit[12][8]{
  // a  b  c  d  e  f  g  h 
    {0, 0, 0, 0, 0, 0, 1, 1}, // Zero
    {1, 0, 0, 1, 1, 1, 1, 1}, // One
    {0, 0, 1, 0, 0, 1, 0, 1}, // Two
    {0, 0, 0, 0, 1, 1, 0, 1}, // Three
    {1, 0, 0, 1, 1, 0, 0, 1}, // Four
    {0, 1, 0, 0, 1, 0, 0, 1}, // Five
    {0, 1, 0, 0, 0, 0, 0, 1}, // Six
    {0, 0, 0, 1, 1, 1, 1, 1}, // Seven
    {0, 0, 0, 0, 0, 0, 0, 1}, // Eight
    {0, 0, 0, 0, 1, 0, 0, 1}, // Nine
    {1, 1, 1, 1, 1, 1, 0, 1}, // Negative
    {1, 1, 1, 1, 1, 1, 1, 1}  // NULL
};


void Write(){
  digitalWrite(WE, HIGH);
  delay(10);
  digitalWrite(WE, LOW);
  delayMicroseconds(1);
  digitalWrite(WE, HIGH);
}


void setAdd(int value){
  int i, Bit, binAddress[7];

  Serial.print("\nDecimal Address: ");
  Serial.print(value);
  Serial.print("\t Binary Address: ");
  
  for(i=6; i>=0; i--){
    Bit = (value & 1);
    binAddress[i] = Bit;
    value = value >> 1;
  }

  for(i=0; i<7; i++){
    Serial.print(binAddress[i]);
    Serial.print(" ");
    digitalWrite(address[i+4], binAddress[i]);
  }

  Serial.print("  \n");

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


void setData(int num){
  int i;
  Serial.print("Data Values: ");

  for(i=0; i<8; i++){
    digitalWrite(data[i], digit[num][i]);
    delay(5);
    Serial.print(digit[num][i]);
    Serial.print(" ");
  }

  Serial.print("\n");
  
  return;
}


void BCD(int num){
  int i, j, k, digit, temp = num;

  setAdd(num); // Set constant Address


  for(i=1; i>=0; i--){
    for(j=1; j>=0; j--){
      digit = num % 10; // Finding Digit
      for(k=0; k<2; k++){        
        Serial.print("Digit = ");
        Serial.print(digit);
        Serial.print("\t\t");

        // Completing Address
        digitalWrite(address[0], 0); // Signed Mode
        digitalWrite(address[1], i); 
        digitalWrite(address[2], j);
        digitalWrite(address[3], k); // Sign bit

        // Positive Numbers 
        if(k==0){
          // First Digit (MSD)
          if((i==0) && (j==0)){
            setData(11); // NULL Digit
          }

          // Middle Digits
          else if((!digit) && (temp<100) && (i^j)){
            setData(11);  // NULL Digit
          }

          // Forth Digit (LSD)
          else{
            setData(digit);
          }
        }

        // Negative Numbers
        else{
          // 1st Digit (MSD)
          if((i==0) && (j==0)){
            if(temp<100){setData(11);}
            else{setData(10);}
          }

          // 2nd Digit
          else if((i==0) && (j==1)){
            if(temp<10){setData(11);}
            else if(temp<100){setData(10);}
            else{setData(digit);}
          }

          // 3rd Digit
          else if((i==1) && (j==0)){
            if(temp<10){setData(10);}
            else{setData(digit);}
          }

          // 4th Digit (LSD)
          else{
            setData(digit);
          }
        }

        Write();
        delay(dt);
        
      }      
      
      num = num / 10;
      
    }
  }
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

  pinMode(WE,OUTPUT);
  Serial.begin(9600);
  Serial.print("Serial Monitor Starts\n\n");
  Serial.print("READING DATA FROM EEPROM 28C16\n");

  for(int i=0; i<128; i++){
    BCD(i);
    delay(10);
  }

}

void loop() {
  
}
