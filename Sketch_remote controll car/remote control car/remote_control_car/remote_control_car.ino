#include <espnow.h>
#include <ESP8266WiFi.h>

// Motor right(A)
#define pwm_R 4  //D2
#define in1_F 16 //D0
#define in2_B 14 //D5
// Motor left(B)
#define pwm_L 5  //D1
#define in3_F 0  //D3
#define in4_B 2  //D4
//color detector
#define S0 1   //TX
#define S1 3   //RX
#define S2 13  //D7
#define S3 12  //D6
#define sensorOut 15   //D8
#define SEND_INTERVAL 0.03
int sendTmr=0,sendTry=0;

// Define the data to recive
typedef struct struct_message {
  int x;
  int y;
  int z;
} struct_message;
// create struct name
struct_message Dir_Spd_ctrl;
// Callback function executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&Dir_Spd_ctrl, incomingData, sizeof(Dir_Spd_ctrl));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("xValue:");
  Serial.print(Dir_Spd_ctrl.x);
  Serial.print(" ");
  Serial.print("yValue");
  Serial.print(Dir_Spd_ctrl.y);
  Serial.print(" ");
  Serial.print("speed:");
  Serial.println(Dir_Spd_ctrl.z);
  Serial.println();
}

uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0xDC, 0x0A, 0xCB};
typedef struct sending {
  int r;
  int g;
  int b;
} sending;
// Create sending name
sending color;
// Callback function called when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  if (sendStatus == 0){
    if(sendTry == 1) Serial.println("Delivery success");
    else  Serial.printf("Tried %03d\n", sendTry);
    sendTry = 0; //reset flag
  }
  else{
    if(sendTry++ >= 99) {
      Serial.println("Fatal Error: Delivery Failed!!!");
    }
  }
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Initilize ESP-NOW
  if (esp_now_init()!=0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);

  //Set motor output
  pinMode(pwm_R, OUTPUT);
  pinMode(pwm_L, OUTPUT);
  pinMode(in1_F, OUTPUT);
  pinMode(in2_B, OUTPUT);
  pinMode(in3_F, OUTPUT);
  pinMode(in4_B, OUTPUT);
  //Color detector pin setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

void loop() {
  sensor();
  motor();
}

void motor(){
  delay(20);
  int spdValue = Dir_Spd_ctrl.z;
  int xValue = Dir_Spd_ctrl.x;
  int yValue = Dir_Spd_ctrl.y;
  int MotorSpeed1 = map(spdValue, 0, 4095, 0, 250);
	int MotorSpeed2 = map(spdValue, 0, 4095, 0, 250);
	// Adjust to prevent "buzzing" at very low speed
	if (MotorSpeed1 < 10)MotorSpeed1 = 0;
	if (MotorSpeed2 < 10)MotorSpeed2 = 0;
  
  if (xValue < 1700){
    //Backward
    digitalWrite(in1_F, LOW);
    digitalWrite(in2_B, HIGH);
    digitalWrite(in3_F, LOW);
    digitalWrite(in4_B, HIGH); 
  }
  else if (xValue >1800){
    //Forward
    digitalWrite(in1_F, HIGH);
    digitalWrite(in2_B, LOW);
    digitalWrite(in3_F, HIGH);
    digitalWrite(in4_B, LOW);
  }
  else{
    // motor stop
    digitalWrite(pwm_L, LOW);
    digitalWrite(pwm_R, LOW); 
  }
  if (yValue < 1840){// Move Left
    // convert yValue to increase
    yValue = yValue - 1840;
    yValue = yValue * -1;
    yValue = map(yValue, 0, 1840, 0, 250);
    MotorSpeed1 = MotorSpeed1 - yValue;
    MotorSpeed2 = MotorSpeed2 + yValue;
    // Don't exceed range of 0-255 for motor speeds
    if (MotorSpeed1 < 0)MotorSpeed1 = 0;
    if (MotorSpeed2 > 250)MotorSpeed2 = 250; 
  }
  else if (yValue > 1890){// Move Right
    yValue = map(yValue, 1890, 4095, 0, 250);
    MotorSpeed1 = MotorSpeed1 + yValue;
    MotorSpeed2 = MotorSpeed2 - yValue;
    // Don't exceed range of 0-255 for motor speeds
    if (MotorSpeed1 > 250)MotorSpeed1 = 250;
    if (MotorSpeed2 < 0)MotorSpeed2 = 0;
  }
  // Set the motor speeds
  analogWrite(pwm_L, MotorSpeed1);
  analogWrite(pwm_R, MotorSpeed2);
}

void sensor(){
  // store LED value
  int redFrequency = 0;
  int greenFrequency = 0;
  int blueFrequency = 0;

  // filter out the R value
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  delay(100);
  
  // filter out the G value
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  delay(100);
 
  //  filter out the B value
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);
  delay(100);
  
  // Format structured data
  color.r = redFrequency;
  color.g = greenFrequency;
  color.b = blueFrequency;
  // Send message via ESP-NOW
  if((sendTmr++ >= SEND_INTERVAL) && (sendTry == 0)) {
    sendTmr = 0; //reset tmier
    sendTry = 1; //start sending flag
  }
  if(sendTry > 0){
    esp_now_send(broadcastAddress, (uint8_t *) &color, sizeof(color));
  }
}
