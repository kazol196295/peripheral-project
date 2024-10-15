#include <Servo.h>
#include <HX711_ADC.h>
#include <EEPROM.h>

Servo myservo;
int trigPin1 = 5;
int echoPin1 = 6;

long duration1;
int distance1;

const int HX711_dout = 3; // mcu > HX711 dout pin
const int HX711_sck = 2;  // mcu > HX711 sck pin
HX711_ADC LoadCell(HX711_dout, HX711_sck);
void pingpong()
{
    myservo.write(0);
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = duration1 * 0.034 / 2;

    Serial.print(distance1);
    Serial.println(" cm ");

    // if distance from either sensor is less than or equal to 5 cm
    if (distance1 <= 20)
    {
        myservo.write(90);
        Serial.println("kkkk"); // rotate servo motor to 90 degrees
        delay(800);             // wait for the servo to move
        myservo.write(0);
        delay(100);
        while (distance1 <= 20)
        { // while object is still in range of either sensor
            // read distance from first sensor
            digitalWrite(trigPin1, LOW);
            delayMicroseconds(2);
            digitalWrite(trigPin1, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin1, LOW);
            duration1 = pulseIn(echoPin1, HIGH);
            distance1 = duration1 * 0.034 / 2;

            Serial.print(distance1);
            Serial.println(" cm ");
            delay(100);
        }
        // myservo.write(0);    // rotate servo motor back to original position
        delay(100); // wait for the servo to move
    }
}
void setup()
{
    myservo.attach(12); // attach servo motor to pin 10
    myservo.write(0);
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    float calibrationValue = 96.06;
    Serial.begin(9600);
    LoadCell.begin();     // start connection to HX711
    LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
    EEPROM.get(0, calibrationValue);
    LoadCell.setCalFactor(calibrationValue);
}

void loop()
{
    // read distance from first sensor
    LoadCell.update();            // retrieves data from the load cell
    float i = LoadCell.getData(); // get output value

    Serial.print(i);
    Serial.println(" g ");
    if (i > 400.0)
    {
        pingpong();
    }
}