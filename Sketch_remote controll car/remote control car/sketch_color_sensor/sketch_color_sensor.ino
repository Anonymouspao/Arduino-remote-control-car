int S0 = 1;        //TCS 3200 感測器 S0 連接 TX
int S1 = 3;        //TCS 3200 感測器 S1 連接 RX
int S2 = 13;       //TCS 3200 感測器 S2 連接 D7
int S3 = 12;       //TCS 3200 感測器 S3 連接 D6
int sensorOut = 15;      //TCS 3200 感測器 OUT 連接 D8

// 儲存由光電二極體讀取的值
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

void setup() {
  // 設定S0~S4為輸出
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // 設定 sensorOut 為輸出
  pinMode(sensorOut, INPUT);
  
  // 設定頻率尺度(frequency scaling)為 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
   // 開始串列通訊
  Serial.begin(9600);
}
void loop() {
  // 設定光電二極體讀取濾過的紅色值
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // 讀取輸出頻率
  redFrequency = pulseIn(sensorOut, LOW);
  delay(100);
  
  // 設定光電二極體讀取濾過的綠色值
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // 讀取輸出頻率
  greenFrequency = pulseIn(sensorOut, LOW);
  delay(100);
 
  // 設定光電二極體讀取濾過的藍色值
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // 讀取輸出頻率
  blueFrequency = pulseIn(sensorOut, LOW);
  delay(100);

  int R=redFrequency, G=greenFrequency, B=blueFrequency;
  Serial.print(R);
  Serial.print(", ");
  Serial.print(G);
  Serial.print(", ");
  Serial.println(B);
    
}
