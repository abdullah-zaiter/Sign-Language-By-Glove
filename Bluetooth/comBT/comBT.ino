    #include <SoftwareSerial.h>

    SoftwareSerial BTSerial(2, 3); // RX, TX
    
    unsigned char mao = 0;
    unsigned char i=0;    
    
    void setup() {
      Serial.begin(9600);
      BTSerial.begin(115200);
    }
    
    void loop() {
      if(BTSerial.available()){
          mao = BTSerial.read();
          if(mao==1){
          BTSerial.write(i);
          BTSerial.write(i+1);
          BTSerial.write(i+2);
          BTSerial.write(i+3);
          BTSerial.write(i+4);
          }else{
          BTSerial.write(i+5);
          BTSerial.write(i+6);
          BTSerial.write(i+7);
          BTSerial.write(i+8);
          BTSerial.write(i+9);
          }             
          i=i+10; 
     }

    }
