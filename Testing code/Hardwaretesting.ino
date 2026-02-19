// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(26, OUTPUT);
  
  pinMode(25,INPUT);
  pinMode(32,INPUT);
  pinMode(33,INPUT);
  pinMode(27,INPUT);
}

// the loop function runs over and over again forever
void loop() {
 bool val1=digitalRead(25);//down
 bool val2=digitalRead(32);//left
 bool val3=digitalRead(33); //up
 bool val4=digitalRead(27);//right
 
  if (val1==HIGH){
    Serial.println("BUTTON 1 CLICKED");
    delay(20);
    }

     if (val2==HIGH){
    Serial.println("BUTTON 2 CLICKED");
    delay(20);
    }

     if (val3==HIGH){
    Serial.println("BUTTON 3 CLICKED");
    delay(20);
    }

     if (val4==HIGH){
    Serial.println("BUTTON 4 CLICKED");
    delay(20);
    }
    
  digitalWrite(26, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(26, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
