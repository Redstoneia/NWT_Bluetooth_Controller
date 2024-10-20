#include <SoftwareSerial.h>
#include <Servo.h>

Servo Servo;

const int bluetoothTx = 3;  // TX Pin of the HC-05
const int bluetoothRx = 2;  // RX Pin of HC-05

const int mot_f = 6;  // motor acceleration forward value
const int mot_b = 5;  // motor acceleration backward value

String buffer = "";  // buffer

bool capturing = false;  // Gibt an, ob gerade Zeichen zwischen zwei 'x' gesammelt werden


int v = 0;  // final velocity value
int d = 0;  // final direction value

int servo = 0;  //servo position value after mapping

int mot = 0;  // motor acceleration value

String capture_v = "";  // exacted velocity value string from the input
String capture_d = "";  // exacted direction value string from the input

 // SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);  // SoftwareSerial für Bluetooth

void setup() {
  Serial.begin(9600);  // start of serial com

  Servo.attach(9); // servo pin output

  pinMode(mot_f, OUTPUT);  // motor speed pin output
  pinMode(mot_b, OUTPUT);  // motor direction output

 // bluetooth.begin(9600);  // start of bluetooth com
 Serial.println("ready");
}

void loop() {
  if (Serial.available() > 0) {
    char inputChar = Serial.read();  // bluetooth com input

    if (inputChar == 'x') {
      if (capturing) {  // main code
        // Serial.println("Text between 'x' " + buffer);
        capture_v = buffer.substring(0, 3);  // extracting characters for the velocity value
        capture_d = buffer.substring(3, 6);  // extracting characters for the direction value
        v = capture_v.toInt();               // string to integer (v)
        d = capture_d.toInt();               // string to integer (d)
       /* Serial.print("Int v: ");
        Serial.println(v);
        Serial.print("Int d: ");
        Serial.println(d); */
        
        servo = map(d, 0, 200, 30, 150);  // remapping the input values to the servo values
       /* Serial.print("servo: ");
        Serial.println(servo); */
        Servo.write(servo);  // output of the servo value to the servo

         if (v > 100) {
         // Serial.print("v: ");
         mot = map(v, 100, 200, 0, 255);
         // Serial.println(mot);
         analogWrite(mot_f, mot);
        }
        else if (v < 100)  {
        // Serial.print("b: ");
         mot = map(v, 100, 0, 0, 255);
        // Serial.println(mot);
         analogWrite(mot_b, mot);
        }
        else if (v = 100)  {
        // Serial.print("mot = 0 ");
         analogWrite(mot_f, 0);
         analogWrite(mot_b, 0);
        }
        delay(1);
      }

      buffer = "";  // clear buffer
      capturing = true;
    } else if (capturing) {
      // adding characters to the buffer string
      buffer += inputChar;
    }
    delay(1);
  }
}
