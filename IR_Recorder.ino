#define IR_RECV_PIN      2
#define IR_TIMEOUT_USEC  1000000
 
void setup()
{
  Serial.begin(115200);
  pinMode(IR_RECV_PIN, INPUT);
}
 
void loop()
{ 
  int previousBit = digitalRead(IR_RECV_PIN);
  
  //Serial.println("ready");
  while(previousBit == digitalRead(IR_RECV_PIN)) {} 

  //Serial.println(">>>");

  unsigned long initTime = micros();
  unsigned long previousTimeUSec = micros();  
  previousBit = digitalRead(IR_RECV_PIN);

  while (micros() - previousTimeUSec < IR_TIMEOUT_USEC) {
    int currentBit = digitalRead(IR_RECV_PIN);
    if (previousBit == currentBit) continue;

    unsigned long now = micros();
    unsigned long dur = now - previousTimeUSec;
    Serial.print(dur);
    Serial.print(", ");
 
    previousBit = currentBit;
    previousTimeUSec = now;
  }
  Serial.println();
  //Serial.println("\n<<<");
}
