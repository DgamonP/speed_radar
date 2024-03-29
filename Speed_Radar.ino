////////////Librarias MPU6050///////////
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

///////////Variables////////////////////
int button=12;                 //boton de Encendido
int radar=11;                  //Gatillo de Radar
int segment[4][7];            //LCD segment data stored here
int measuredSpeed, oldSpeed, unit;  //converted data to speed

int ax, ay, az;
int16_t Tmp;
char flag;

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

void setup() {
Serial.begin(115200);//for debugging
    delay(2000);
    while (!Serial){ };
    Serial.println("hola rpi soy Leonardo");
    ///////////////////Inicializacion Radar////////////////////
    pinMode(button,OUTPUT);
    pinMode(radar,OUTPUT);
    flag = 'I';  
    Serial.println(flag);
    ///////////////////Inicializacion MPU6050/////////////////
    Wire.begin();           //Iniciando I2C  
    sensor.initialize();    //Iniciando el sensor
    if (sensor.testConnection()) 
    Serial.println("Sensor init");
    else Serial.println("Error init");
}

void loop(){
   if (Serial.available()) {
     flag = Serial.read();
      Serial.println("new flag");
      Serial.println(flag);
      speed_flag();
   } 
   else {
      if (flag=='S'){
        speed_comparation();
        speed_print();
        }
   }
}

void speed_comparation(){
  ///////////////////Obtencion de la Velocidad////////////////
    delay(350);// update rate
    
    unsigned long scanTimeStart = millis();//timeout for scanning LCD
    while (millis() - scanTimeStart < 100) {//jump in and scan the LCD!
     for (int i = 7; i < 11; i++) { //sweep the common pins
      if (analogRead(i) < 10) {// LOW
        for (int j = 0; j < 7; j++) {//sweep the segments after we find an enabled common
          if (analogRead(j) > 600) {//HIGH
            segment[i - 7][j] = 1;//set the segment to a 1
            }
          }
        }
      }
    }

    measuredSpeed = 0;//clear the speed, and we'll set it now based on the segment data

    /*
     *  ONES POSITION
     */
  
    //     3,5
    // 3,4     2,5
    //     2,4
    // 1,4     1,5
    //     0,5
  
    //Zero - Ones
    if (segment[3][4] == 1 && segment[3][5] == 1 && segment[2][5] == 1 && segment[2][4] == 0 && segment[1][4] == 1 && segment[1][5] == 1 && segment[0][5] == 1) {
      measuredSpeed = 0;
    }
    //One - Ones
    if (segment[3][4] == 0 && segment[3][5] == 0 && segment[2][5] == 1 && segment[2][4] == 0 && segment[1][4] == 0 && segment[1][5] == 1 && segment[0][5] == 0) {
      measuredSpeed = 1;
    }
    //Two - Ones
    if (segment[3][4] == 0 && segment[3][5] == 1 && segment[2][5] == 1 && segment[2][4] == 1 && segment[1][4] == 1 && segment[1][5] == 0 && segment[0][5] == 1) {
      measuredSpeed = 2;
    }
    //Three - Ones
    if (segment[3][4] == 0 && segment[3][5] == 1 && segment[2][5] == 1 && segment[2][4] == 1 && segment[1][4] == 0 && segment[1][5] == 1 && segment[0][5] == 1) {
      measuredSpeed = 3;
    }
    //Four - Ones
    if (segment[3][4] == 1 && segment[3][5] == 0 && segment[2][5] == 1 && segment[2][4] == 1 && segment[1][4] == 0 && segment[1][5] == 1 && segment[0][5] == 0) {
      measuredSpeed = 4;
    }
    //Five - Ones
    if (segment[3][4] == 1 && segment[3][5] == 1 && segment[2][5] == 0 && segment[2][4] == 1 && segment[1][4] == 0 && segment[1][5] == 1 && segment[0][5] == 1) {
      measuredSpeed = 5;
    }
    //Six - Ones
    if (segment[3][4] == 1 && segment[3][5] == 1 && segment[2][5] == 0 && segment[2][4] == 1 && segment[1][4] == 1 && segment[1][5] == 1 && segment[0][5] == 1) {
      measuredSpeed = 6;
    }
    //Seven - Ones
    if (segment[3][4] == 0 && segment[3][5] == 1 && segment[2][5] == 1 && segment[2][4] == 0 && segment[1][4] == 0 && segment[1][5] == 1 && segment[0][5] == 0) {
      measuredSpeed = 7;
    }
    //Eight - Ones
    if (segment[3][4] == 1 && segment[3][5] == 1 && segment[2][5] == 1 && segment[2][4] == 1 && segment[1][4] == 1 && segment[1][5] == 1 && segment[0][5] == 1) {
      measuredSpeed = 8;
    }
    //Nine - Ones
    if (segment[3][4] == 1 && segment[3][5] == 1 && segment[2][5] == 1 && segment[2][4] == 1 && segment[1][4] == 0 && segment[1][5] == 1 && segment[0][5] == 1) {
      measuredSpeed = 9;
    }
  
  
  /*
   * TENS POSITION
   */
    //     3,3
    // 3,2     2,3
    //     2,2
    // 1,2     1,3
    //     0,3
  
    //One - Tens
    if (segment[3][2] == 0 && segment[2][2] == 0 && segment[2][3] == 1 && segment[3][3] == 0 && segment[1][2] == 0 && segment[1][3] == 1 && segment[0][3] == 0) {
      measuredSpeed = measuredSpeed + (1 * 10);
    }
    //Two - Tens
    if (segment[3][2] == 0 && segment[2][2] == 1 && segment[2][3] == 1 && segment[3][3] == 1 && segment[1][2] == 1 && segment[1][3] == 0 && segment[0][3] == 1) {
      measuredSpeed = measuredSpeed + (2 * 10);
    }
    //Three - Tens
    if (segment[3][2] == 0 && segment[2][2] == 1 && segment[2][3] == 1 && segment[3][3] == 1 && segment[1][2] == 0 && segment[1][3] == 1 && segment[0][3] == 1) {
      measuredSpeed = measuredSpeed + (3 * 10);
    }
    //Four - Tens
    if (segment[3][2] == 1 && segment[2][2] == 1 && segment[2][3] == 1 && segment[3][3] == 0 && segment[1][2] == 0 && segment[1][3] == 1 && segment[0][3] == 0) {
      measuredSpeed = measuredSpeed + (4 * 10);
    }
    //Five - Tens
    if (segment[3][2] == 1 && segment[2][2] == 1 && segment[2][3] == 0 && segment[3][3] == 1 && segment[1][2] == 0 && segment[1][3] == 1 && segment[0][3] == 1) {
      measuredSpeed = measuredSpeed + (5 * 10);
    }
    //Six - Tens
    if (segment[3][2] == 1 && segment[2][2] == 1 && segment[2][3] == 0 && segment[3][3] == 1 && segment[1][2] == 1 && segment[1][3] == 1 && segment[0][3] == 1) {
      measuredSpeed = measuredSpeed + (6 * 10);
    }
    //Seven - Tens
    if (segment[3][2] == 0 && segment[2][2] == 0 && segment[2][3] == 1 && segment[3][3] == 1 && segment[1][2] == 0 && segment[1][3] == 1 && segment[0][3] == 0) {
      measuredSpeed = measuredSpeed + (7 * 10);
    }
    //Eight - Tens
    if (segment[3][2] == 1 && segment[2][2] == 1 && segment[2][3] == 1 && segment[3][3] == 1 && segment[1][2] == 1 && segment[1][3] == 1 && segment[0][3] == 1) {
      measuredSpeed = measuredSpeed + (8 * 10);
    }
    //Nine - Tens
    if (segment[3][2] == 1 && segment[2][2] == 1 && segment[2][3] == 1 && segment[3][3] == 1 && segment[1][2] == 0 && segment[1][3] == 1 && segment[0][3] == 1) {
      measuredSpeed = measuredSpeed + (9 * 10);
    }
  
  /*
   * HUNDREDS
   */
    //     3,1
    // 3,0     2,1
    //     2,0
    // 1,0     1,1
    //     0,1
    //One - Hundreds
    if (segment[3][1] == 0 && segment[3][0] == 0 && segment[2][1] == 1 && segment[2][0] == 0 && segment[1][0] == 0 && segment[1][1] == 1 && segment[0][1] == 0) {
      measuredSpeed = measuredSpeed + (1 * 100);
    }
    if (segment[3][1] == 1 && segment[3][0] == 0 && segment[2][1] == 1 && segment[2][0] == 1 && segment[1][0] == 1 && segment[1][1] == 0 && segment[0][1] == 1) {
      measuredSpeed = measuredSpeed + (2 * 100);
    }
    if (segment[1][6] == 1){
      unit = 1;
    }else {
      unit=0;
      }
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 7; j++) {
        segment[i][j] = 0;
        
//              return measuredSpeed;
      }
    }

}

void speed_print(){
  ///////////////////Imprimir solo si esta la en la unidad correta y si es requerido los datos//////////////////
          if (unit == 1){
          if (flag=='S'){
          sensor.getAcceleration(&ax, &ay, &az);     //Calcular los angulos de inclinacion:
          float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
          float accel_ang_y=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
          Tmp=sensor.getTemperature();
          //Mostrar los angulos separadas por un [tab]
          Serial.print("{\"speed\":"); 
          Serial.print(measuredSpeed);
          Serial.print(",\"tilt\":"); 
          Serial.print(accel_ang_x);
          Serial.print(",\"temp\":"); 
          Serial.print(Tmp/340.00+26.53);
          Serial.println("}"); 
          delay(300);
          }
        }
        else if (unit == 0 && flag=='S'){
          digitalWrite(button,HIGH);
          delay(500);
          digitalWrite(button,LOW);
          delay(3000);
          digitalWrite(radar,HIGH);
          delay(1000);
          digitalWrite(radar,LOW);
          delay(1000);
          digitalWrite(radar,HIGH);
         }
          if (flag == 'I'){
          Serial.println("shot down BEGIN");
          digitalWrite(radar,LOW);
          digitalWrite(button,HIGH);
          delay(3500);
          digitalWrite(button,LOW);
          flag=' ';
          }
}
  

void speed_flag(){
    if (flag=='S'){
          //S for init transmition serial data
          digitalWrite(button,HIGH);
          delay(300);
          digitalWrite(button,LOW);
          delay(3000);
          digitalWrite(radar,HIGH);
          delay(1000);
          digitalWrite(radar,LOW);
          delay(1000);
          digitalWrite(radar,HIGH);
          }
        if (flag== 'R'){
          digitalWrite(button,HIGH);
          delay(500);
          digitalWrite(button,LOW);
          digitalWrite(radar,LOW);
          }
        if (flag== 'A'){
          Serial.println("shot down BEGIN");
          digitalWrite(radar,LOW);
          digitalWrite(button,HIGH);
          delay(3100);
          digitalWrite(button,LOW);
          }
        if (flag== 'O'){
          flag=' '; unit=0;
          digitalWrite(radar,LOW);
          digitalWrite(button,HIGH);
          delay(800);
          digitalWrite(button,LOW);
          delay(1000);
          digitalWrite(button,HIGH);
          delay(3000);
          digitalWrite(button,LOW);
        }
  }
  
