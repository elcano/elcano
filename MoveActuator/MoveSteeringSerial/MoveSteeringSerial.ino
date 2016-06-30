void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  
}
int sig = 128;
void loop() {
  
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    sig = Serial.parseInt();
    Serial.println(sig);
  }
  analogWrite(3, sig);
}
