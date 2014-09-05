#define IR_RECV_PIN  2
#define IR_SEND_PIN  4
#define IR_TIMEOUT_USEC  1000000
#define BUFFER_SIZE 100

void setup()
{
  Serial.begin(115200);
  pinMode(IR_RECV_PIN, INPUT);
  pinMode(IR_SEND_PIN, OUTPUT);
}

void loop()
{
 int irdata[BUFFER_SIZE];
 int i = 0;

 Serial.println("ready");

 while(true) {
   int c = Serial.read();
   if(c == -1 || c == 13 || c == 10 || c == 32) continue;
   irdata[i++] = c;
   Serial.print(c);
   Serial.print(", ");
   if(c==84) break;
 }
 Serial.println();

 long t = 425;
 
 for(int i=0; i<BUFFER_SIZE; i++) {
   int c = irdata[i];
   switch(c) {
     case -1:
       break;    
     case 76: //L
       Serial.println("leader");
       send_leader(t);
       break;
   
     case 84: //T
       send_trailer(t);
       Serial.println("trailer");
       return;
       break;
    
     default:
       send_4bits(t, c);
       break;
    }
  }
}

void on(long us) {
  long init = micros();
  while(micros()-init <= us) {
    digitalWrite(IR_SEND_PIN, HIGH);
    delayMicroseconds(9);
    digitalWrite(IR_SEND_PIN, LOW);
    delayMicroseconds(17);
  }
}

void off(long us) {
  delayMicroseconds(us);
}

void send_leader(long t) {
  on(8 * t);
  off(4 * t);
}

void send_trailer(long t) {
  on(t);
}

void send_4bits(long t, int c) {
  int x = 0;
  if( 48 <= c && c <= 57) {
    x = c - 48;
  } else if (41 <= c && c <= 46) {
    x = c - 31;
  } else if (97 <= c && c <= 102) {
    x = c - 87;
  } else {
    Serial.print("unknown char:");
    Serial.println(c);
    return;
  }
  for(int i = 0; i<4; i++) {
    if(x & 1) {
      on(t);
      off(3*t);
      //Serial.print("1");
    } else {
      on(t);
      off(t);
      //Serial.print("0");
    }
    x = x >> 1;
  }
  //Serial.println();
}
