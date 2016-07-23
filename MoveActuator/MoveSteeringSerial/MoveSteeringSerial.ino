void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  
}
int sig = 190;
void loop() {
  
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    sig = Serial.parseInt();
    Serial.println(sig);
    analogWrite(7, sig);
  }
  //analogWrite(8, sig);
}
