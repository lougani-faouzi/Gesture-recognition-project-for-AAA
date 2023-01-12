/*#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

int Vdata = 1; 
void setup(){ 
  
   Serial.begin(9600); 
   SerialBT.begin(9600); 
   
} 

void loop(){ 

SerialBT.println(Vdata);

delay(1000); 

}*/

#include "BluetoothSerial.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "/home/louganifaouzi/Téléchargements/arduino-ide_nightly-20221126_Linux_64bit/Pipeline.h"
#include "/home/louganifaouzi/Téléchargements/arduino-ide_nightly-20221126_Linux_64bit/Classifier.h"
#include <cstring>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;

void setup(void) {
  Serial.begin(115200);
  SerialBT.begin(9600);   
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
 // Serial.print("Accelerometer range set to: ");_1 (DSP: 1222 micros, Classifier: 54 micros)

  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}


void loop() {
  
  sensors_event_t a, g, temp;  
  mpu.getEvent(&a, &g, &temp);

float ax, ay, az, gx, gy, gz;

ax=a.acceleration.x;
ay=a.acceleration.y;
az=a.acceleration.z;
gx=g.gyro.x;
gy=g.gyro.y;
gz=g.gyro.z;

Serial.print("IMU: ");
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.print(az);
  Serial.print(",");
  Serial.print(gx);
  Serial.print(",");
  Serial.print(gy);
  Serial.print(",");
  Serial.print(gz);
  Serial.print("\n");

  int Vdata;
  float features[] = {ax, ay, az, gx, gy, gz};

  if (!pipeline.transform(features))
    return;

  String t0="target_0";
  if(t0.equals(forest.predictLabel(pipeline.X))){
  Vdata = 1;
  SerialBT.println(Vdata);  
  }
  
  String t1="target_1";
  if(t1.equals(forest.predictLabel(pipeline.X))){
  Vdata = 2;
  SerialBT.println(Vdata);  
  }

  
  String t2="target_2";
  if(t2.equals(forest.predictLabel(pipeline.X))){
  Vdata = 3;
  SerialBT.println(Vdata);  
  }
  /*
  Serial.print("Predicted gesture: ");
  Serial.print(forest.predictLabel(pipeline.X));
  Serial.print(" (DSP: ");
  Serial.print(pipeline.latencyInMicros());
  Serial.print(" micros, Classifier: ");
  Serial.print(forest.latencyInMicros());
  Serial.println(" micros)");  
  */

  delay(1000);
}