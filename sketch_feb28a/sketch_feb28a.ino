#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float threshhold =80.0;
int samplesNo =0;
int steps=0,maxStep=0,minStep=0;
int16_t samplesAvg [50];
int16_t xAvg [50];
int16_t yAvg [50];
int16_t zAvg [50];
int16_t tempx,tempy,tempz=0;
bool flag =true;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
      Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  tempx=AcX;tempy=AcY;tempz=AcZ;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 
  xAvg[samplesNo]=AcX;yAvg[samplesNo]=AcY;zAvg[samplesNo]=AcZ;
  int16_t t =sqrt(((xAvg[samplesNo]-tempx)* (xAvg[samplesNo]-tempx))+ ((yAvg[samplesNo] - tempy)*(yAvg[samplesNo] - tempy)) + ((zAvg[samplesNo] - tempz)*(zAvg[samplesNo]-tempz)));
 if(samplesNo !=0){
 samplesAvg[samplesNo] = (t + samplesAvg[samplesNo-1]* samplesNo)/(samplesNo+1); 
 }else{
  samplesAvg[samplesNo]=t;
 }
  if (samplesAvg[samplesNo]>threshhold )
{
   if(samplesAvg[samplesNo]>maxStep){
    maxStep=samplesAvg[samplesNo];
  }

  if(flag==0){
  steps=steps+1;
  displayFunctions(steps);
  flag=1;
    threshhold=(maxStep+minStep)/2;
  }
}
  if (samplesAvg[samplesNo] <threshhold  ){
    if(samplesAvg[samplesNo]<minStep){
    minStep=samplesAvg[samplesNo];}
  if( flag==1){
  flag=0;
  threshhold=(maxStep+minStep)/2;
  }
  }


  samplesNo+=1;
   if(samplesNo==50){
    samplesNo=0;}
    delay(2);
}
//called each 2 seconds
int prevSteps=0;
float distance=0;
const float Height = 1.70 ;
void displayFunctions(int steps){
  int stepsD = steps - prevSteps;
  float ratio =1;
  if (stepsD<=2){
    ratio = 5 ;
  }else if(stepsD == 3){
  ratio = 4 ;

  }else if(stepsD == 4){
  ratio = 3;

  }else if(stepsD == 5){

  ratio =2;
  }else if(stepsD == 6){

  ratio = 1.2;
  }else if(stepsD == 7 ||stepsD == 8){
  ratio = 1;

  }else if(stepsD > 8){
  ratio = 0.833;

  }

  float curDistance = stepsD * (Height /ratio );
  distance += curDistance;
  float vel= distance/2/60;
  float cal= vel * 1.25;
  Serial.println('\n');
  Serial.print("steps=");
  Serial.println(steps);

    Serial.println('\n');
  Serial.print("Distance(km)=");
  Serial.println(distance);

     Serial.println('\n');
  Serial.print("velocity(kmH)=");
  Serial.println(vel);

     Serial.println('\n');
  Serial.print("calories(C/kg/h)=");
  Serial.println(cal);

prevSteps = steps;
}
