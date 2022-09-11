#include<espnow.h>
#include<ESP8266WiFi.h>

// car state
#define CAR_STATE_IDLE		0
#define CAR_STATE_START		1
#define CAR_STATE_CHECK1	2
#define CAR_STATE_CHECK2	3
#define CAR_STATE_CHECK3	4
#define CAR_STATE_CHECK4	5

// detect color
#define COLOR_UNKNOWN			0
#define COLOR_RED				  1
#define COLOR_GREEN			  2
#define COLOR_YELLOW			3
#define COLOR_BLUE				4

int led [4] = {16, 5, 4, 0};  //{D0, D1, D2, D3}
int state = CAR_STATE_IDLE;

// Define a data structure
typedef struct struct_message {
  int r;
  int g;
  int b;
} struct_message;
// Create a structured object
struct_message color;
 
// Callback function executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&color, incomingData, sizeof(color));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("R= ");
  Serial.print(color.r);
  Serial.print(" ");
  Serial.print("G= ");
  Serial.print(color.g);
  Serial.print(" ");
  Serial.print("B= ");
  Serial.println(color.b);
  Serial.println();

  int color_val = COLOR_UNKNOWN;
  if(color.b>=color.r && color.r>color.g && color.g>66){ //deteceted green
    color_val = COLOR_GREEN;
  }
  if(color.g>=color.b && color.b>color.r && color.g>240){ //deteceted red(checkpoint1、4)
    color_val = COLOR_RED;
  }
  if(color.b>=color.g && color.g>=color.r && color.r<=70){ //detected yellow(checkpoint2)
    color_val = COLOR_YELLOW;
  }
  if(color.r>=color.g && color.g>color.b && color.b>=90){ //detected blue(checkpoint3)
    color_val = COLOR_BLUE;
  }

  // state machine
  switch (state) {
    case CAR_STATE_IDLE:	// 0
      if (color_val == COLOR_GREEN) {
        state = CAR_STATE_START;
      }
      break;
    case CAR_STATE_START:	// 1
      if (color_val == COLOR_RED) {
        state = CAR_STATE_CHECK1;
      }
      break;
    case CAR_STATE_CHECK1:	// 2
      if (color_val == COLOR_YELLOW) {
        state = CAR_STATE_CHECK2;
      }
      break;
    case CAR_STATE_CHECK2:	// 3
      if (color_val == COLOR_BLUE) {
        state = CAR_STATE_CHECK3;
      }
      break;
    case CAR_STATE_CHECK3:	// 4
      if (color_val == COLOR_RED) {
        state = CAR_STATE_CHECK4;
      }
      break;
    case CAR_STATE_CHECK4:	// 5
      if (color_val == COLOR_GREEN) {
        state = CAR_STATE_IDLE;
      }
      break;
    default:
      Serial.println("Err state");
      break;
  }
  Serial.println(state);
}

void setup() {
  Serial.begin(115200);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Initilize ESP-NOW
  if (esp_now_init()!=0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(16, OUTPUT); //D0
  pinMode(5, OUTPUT); //D1
  pinMode(4, OUTPUT);//D2
  pinMode(0, OUTPUT);//D3
  pinMode(14, OUTPUT);//D5
  pinMode(12, OUTPUT);//D6
}

void loop() {
  int i;

  if (state != CAR_STATE_IDLE) {	// car run
    digitalWrite(14, 1); //A1
    digitalWrite(12, 0); //A2
  }
  else {	// car stop
    digitalWrite(14, 0); //A1
    digitalWrite(12, 1); //A2    
    for (i = 0 ; i <= 3 ; i++){
      digitalWrite(led[i], LOW);
      delay(10);
    }
  }
  if ((state >= CAR_STATE_CHECK1) && (state <= CAR_STATE_CHECK4)) {
    for (i = 0 ; i <= (state - 2) ; i++){
      digitalWrite(led[i], HIGH);
      delay(10);
    }
  }
}
  
