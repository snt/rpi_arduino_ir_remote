#define IR_RECV_PIN  2
#define IR_SEND_PIN  4
#define IR_TIMEOUT_USEC  1000000
#define BUFFER_SIZE 256

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
   if(c=='Z') break;
 }
 Serial.println();

 long t = 425;
 char format = 'A';
 
 for(int i=0; i<BUFFER_SIZE; i++) {
   int c = irdata[i];
   switch(c) {
     case -1:
       break;    
     case 'L':
       Serial.println("leader");
       send_leader(format, t);
       break;
   
     case 'T':
       send_trailer(format, t);
       Serial.println("trailer");
       return;
       break;
     case 'X': //config  Xtnnn; t = [NAS] NEC, AEHA, SONY; nnn = pulse width [us]
       format = irdata[i+1];
       switch(format) {
         case 'N':
           t = 562;
           break;
         case 'A':
           t = 100 * (irdata[i+2] - '0') + 10 * (irdata[i+3] - '0') + (irdata[i+4] - '0');
           break;
         case 'S':
           t = 600;
           break;
         default:
           Serial.println("Not in the format: Xtnnn; t = [NAS] NEC, AEHA, SONY; nnn = pulse width [us]");
           break;
       }
       Serial.print("format=");
       Serial.println(format);
       Serial.print("T=");
       Serial.println(t);
       i+=4;
       break;
     default:
       send_4bits(format, t, c);
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
  //Serial.print(" ");
  //Serial.print(us);
}

void off(long us) {
  delayMicroseconds(us);
  //Serial.print("-");
  //Serial.print(us);
}

void send_leader(char format, long t) {
  switch(format) {
    case 'N':
      on(16 * t);
      off(8 * t);
      break;      
    case 'A':
      on(8 * t);
      off(4 * t);
      break;
    case 'S':
      on(4 * t);
      break;
    default:
      Serial.println("fail on send leader");
  }  
}

void send_trailer(char format, long t) {
  on(t);
}

void send_4bits(char format, long t, int c) {
  int x = 0;
  if( '0' <= c && c <= '9') {
    x = c - '0';
  } else if ('A' <= c && 'F' <= 46) {
    x = c - 'A' + 10;
  } else if ('a' <= c && c <= 'f') {
    x = c - 'a' + 10;
  } else {
    Serial.print("unknown char:");
    Serial.println(c);
    return;
  }
  switch(format) {
    case 'N':
    case 'A':
      for(int i = 0; i<4; i++) {
        if(x & 1) {
          on(t);
          off(3*t);
        } else {
          on(t);
          off(t);
        }
        x = x >> 1;
      }
      break;
    case 'S':
      for(int i = 0; i<4; i++) {
        if(x & 1) {
          off(2*t);
          on(t);
        } else {
          off(t);
          on(t);
        }
        x = x >> 1;
      }
      break;
    default:
      Serial.println("invalid format");
  }
  //Serial.println();
}
