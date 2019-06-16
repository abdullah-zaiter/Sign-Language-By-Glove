//Programa : Teste MPU-6050
//Alteracoes e adaptacoes : Arduino e Cia
//
//Baseado no programa original de JohnChi
 
//Carrega a biblioteca Wire
#include<Wire.h>
 
//GYRO INIT

#include "SoftwareSerial.h"
SoftwareSerial serial_connection(3,2);//Create a serial connection with TX and RX on these pins
#define BUFFER_SIZE 64//This will prevent buffer overruns.
char inData[BUFFER_SIZE];//This is a character buffer where the data sent by the python script will go.
char inChar=-1;//Initialie the first character as nothing
int count=0;//This is the number of lines sent in from the python script
int i=0;//Arduinos are not the most capable chips in the world so I just create the looping variable once
//GYRO INIT END


//Endereco I2C do MPU6050
const int MPU=0x68;  

//Variaveis para armazenar valores dos sensores
int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//Variaveis para os 5 giroscópios
int GY1 = 6;
int GY2 = 7;
int GY3 = 9;
int GY4 = 10;
int GY5 = 11;

unsigned long avgRunTime = 0;
  

void setup()
{
  
//GYRO MODULE
  serial_connection.begin(115200);//Initialize communications with the bluetooth module
  serial_connection.println("Ready!!!");//Send something to just start comms. This will never be seen.
  Serial.println("Started");//Tell the serial monitor that the sketch has started.

  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 

  pinMode(GY1, OUTPUT);
  pinMode(GY2, OUTPUT);
  pinMode(GY3, OUTPUT);
  pinMode(GY4, OUTPUT);
  pinMode(GY5, OUTPUT);

  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);

}

void loop()
{
  unsigned long startT = micros();
  unsigned long temp;
    
  for(int j = 0; j < 100; j++)
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    
    //serial_connection.print("Begin");
    //serial_connection.print("\n"); 
    
    for(int i = 1; i < 6;i++)
    {
      switchGyro(i);
      //Solicita os dados do sensor
      Wire.requestFrom(MPU,14,true);  
      
      //Armazena o valor dos sensores nas variaveis correspondentes
      AcX=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
      AcY=Wire.read()<<8|Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=Wire.read()<<8|Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

      //Mostra os valores na serial
      //serial_connection.print(AcX, DEC);
      //serial_connection.print(", "); 
      //serial_connection.print(AcY, DEC);
      //serial_connection.print(", "); 
      //serial_connection.print(AcZ, DEC);
      //serial_connection.print("\n"); 
      //serial_connection.print(GyX, DEC);
      //serial_connection.print(", "); 
      //serial_connection.print(GyY, DEC);
      //serial_connection.print(", "); 
      //serial_connection.print(GyZ, DEC);
      //serial_connection.print("\n"); 
      //serial_connection.println(Tmp/340.00+36.53);
      
      //Aguarda 300 ms e reinicia o processo
      //delay(200);
      temp = 0;
      //for(unsigned long startDel = micros(); temp <200000; )
      //{
      //  temp = micros() - startDel; 
      //}
    }

    //Serial.println(delta);
    //avgRunTime+=delta; 
    //serial_connection.print(delta);
    //serial_connection.print("\n"); 
  }
  unsigned long endT = micros();
  unsigned long delta = endT - startT;
    
  
  avgRunTime = delta/100;
  Serial.println(avgRunTime);
}



void switchGyro(int gyroNum)
{
  digitalWrite(GY1, HIGH);
  digitalWrite(GY2, HIGH);
  digitalWrite(GY3, HIGH);
  digitalWrite(GY4, HIGH);
  digitalWrite(GY5, HIGH);
  switch(gyroNum)
  {
    case 1:
      digitalWrite(GY1, LOW);
      break;
    case 2:
      digitalWrite(GY2, LOW);
      break;
    case 3:
      digitalWrite(GY3, LOW);
      break;
    case 4:
      digitalWrite(GY4, LOW);
      break;
    case 5:
      digitalWrite(GY5, LOW);
      break;
    default:
      digitalWrite(GY1, LOW);
      break;
  }
}
