#include <esp_now.h>
#include <WiFi.h>

#define joyX 33
#define joyY 32
#define speed 34
 
// Variables for test data
int xValue, yValue, spdValue;
 
// MAC Address of responder
uint8_t broadcastAddress[] = {0xEC, 0xFA, 0xBC, 0x61, 0xF6, 0x09};
 
// Define a data structure
typedef struct struct_message {
  int x;
  int y;
  int z;
} struct_message;
// Create a structured object
struct_message Dir_Spd_ctrl;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  spdValue = (analogRead(speed)-4095)*-1;

  // Format structured data
  Dir_Spd_ctrl.x = xValue;
  Dir_Spd_ctrl.y = yValue;
  Dir_Spd_ctrl.z = spdValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Dir_Spd_ctrl, sizeof(Dir_Spd_ctrl));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(20);
}
