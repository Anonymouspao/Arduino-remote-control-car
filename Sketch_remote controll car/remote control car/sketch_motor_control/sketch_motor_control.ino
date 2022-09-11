// D0=16, D1=5, D2=4, D3=0, D4=2, D5=14, D6=12, D7=13, D8=15

// Motor right(A)
int pwm_R = 4;  //D2
int in1_F = 16; //D0
int in2_B = 14; //D5
// Motor left(B)
int pwm_L = 5;  //D1
int in3_F = 0;  //D3
int in4_B = 2;  //D4

void setup(){
  pinMode(pwm_R, OUTPUT);
  pinMode(pwm_L, OUTPUT);
  pinMode(in1_F, OUTPUT);
  pinMode(in2_B, OUTPUT);
  pinMode(in3_F, OUTPUT);
  pinMode(in4_B, OUTPUT);
}
 
void forward(){
  analogWrite(pwm_L, 255);
  analogWrite(pwm_R, 255);
  digitalWrite(in1_F, HIGH);
  digitalWrite(in2_B, LOW);
  digitalWrite(in3_F, HIGH);
  digitalWrite(in4_B, LOW);
  delay(1000);
  void stop();
}
 
void backward(){
  analogWrite(pwm_L, 255);
  analogWrite(pwm_R, 255);
  digitalWrite(in1_F, LOW);
  digitalWrite(in2_B, HIGH);
  digitalWrite(in3_F, LOW);
  digitalWrite(in4_B, HIGH);
  delay(1000);
  void stop();
}

void left(){
  analogWrite(pwm_L, 255);
  analogWrite(pwm_R, 0);
  digitalWrite(in1_F, HIGH);
  digitalWrite(in2_B, LOW);
  digitalWrite(in3_F, HIGH);
  digitalWrite(in4_B, LOW);
  delay(1000);
  void stop();
}

void right(){
  analogWrite(pwm_L, 0);
  analogWrite(pwm_R, 255);
  digitalWrite(in1_F, HIGH);
  digitalWrite(in2_B, LOW);
  digitalWrite(in3_F, HIGH);
  digitalWrite(in4_B, LOW);
  delay(1000);
  void stop();
}

void speed(){
  for(int i=10; i<=255; i++){
    analogWrite(pwm_L, i);
    analogWrite(pwm_R, i);
    delay(20);
  }
  digitalWrite(in1_F, HIGH);
  digitalWrite(in2_B, LOW);
  digitalWrite(in3_F, HIGH);
  digitalWrite(in4_B, LOW);
  delay(1000);

  for(int i=255; i>=0; i--){
    analogWrite(pwm_L, i);
    analogWrite(pwm_R, i);
    delay(20);
  }
  digitalWrite(in1_F, LOW);
  digitalWrite(in2_B, HIGH);
  digitalWrite(in3_F, LOW);
  digitalWrite(in4_B, HIGH);
  delay(1000);
}

void stop(){
  analogWrite(pwm_L, 0);
  analogWrite(pwm_R, 0);
  digitalWrite(in1_F, LOW);
  digitalWrite(in2_B, LOW);
  digitalWrite(in3_F, LOW);
  digitalWrite(in4_B, LOW);
  delay(20);
}
void loop(){
  forward();
  delay(1000);
  backward();
  delay(1000);
  left();
  delay(1000);
  right();
  delay(1000);
  speed();
}
