    unsigned char mao = 0;
    unsigned char i=0;    
    
    void setup() {
      Serial.begin(9600); // Default communication rate of the Bluetooth module
    }
    
    void loop() {
      if(Serial.available() > 0){ // Checks whether data is comming from the serial port
          mao = Serial.read();
          if(mao==1){
          Serial.write(i);
          Serial.write(i+1);
          Serial.write(i+2);
          Serial.write(i+3);
          Serial.write(i+4);
          }else{
          Serial.write(i+5);
          Serial.write(i+6);
          Serial.write(i+7);
          Serial.write(i+8);
          Serial.write(i+9);
          }             
          i=i+10; 
     }

    }
