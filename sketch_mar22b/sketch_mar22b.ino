#include <Servo.h>

Servo myservo1;
Servo myservo2;
int potPin = 3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo1.attach(9);  
  //myservo1.write(currentAngle);
  myservo2.attach(8);  
  //myservo2.write(currentAngle);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val;
  double volt;
  val = analogRead(potPin);

  volt = map(val,0,1023,0,180);
  myservo1.write(volt);
  myservo2.write(volt);
  //volt /= 100;
  Serial.println(volt);
  
  delay(100);
}









/*
#include <Servo.h>

Servo myservo;



int currentAngle = 150;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  
  myservo.write(currentAngle);
}

void loop() {
  myservo.write(currentAngle);
  
  delay(1000);
}

*/





















/*
#include <Wire.h>


long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
float deg;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
}


void loop() {
  float oldx = rotX;
  recordGyroRegisters();
  //Serial.print("Gyro (deg)");
  //Serial.print(" X=");
  rotX *= 0.1;
  if(rotX > 2 || rotX < -2){
    deg += rotX;
  }
  Serial.println(deg);
  delay(100);
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}





void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}
*/
