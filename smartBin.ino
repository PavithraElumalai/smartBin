 //LANGUAGE : 'ARDUINO'
//PROJECT : 'SMART BIN SYSTEM'
//COMPONENTS : ULTRASONIC DISTANCE SENSOR, PASSIVE INFRARED MOTION SENSOR, LM35 SENSOR

// CODE

// ultrasonic distance sensor: trig pin = 1 ; echo pin = 2
//pir sensor : in = 3 
//temp sensor : in = 5 

#include<Servo.h>
#include<SoftwareSerial.h>


int trig_pin = 1;
int echo-pin = 2;
int pir_in = 3;
int servo_pin=4;
int  temp_pin = a0; 
int angle;
Servo servo;
SoftwareSerial BTserial(rx,tx);

void setup()
{
	Serial.begin(9600);
	BTserial.begin(9600);
	pinMode(trig_pin , OUTPUT);
	PinMode(echo_pin, INPUT);
	PinMode(pir_in , INPUT);
	//PinMode(pir_out, OUTPUT);
	PinMode(temp_pin,INPUT);
	servo.attach(servo_pin);

}

void loop()
{
	long time,cm,max=5,pir_flag=0;
	int val;
	float r,celcius;

	//ultrasonic distance sensor
	
	digitalWrite(trig_pin, LOW);
	delayMicroseconds(3);
	digitalWrite(trig_pin,HIGH);
	delayMicroseconds(10);
	digitalWrite(trig_pin, LOW);

	time = pulseIn(echo_pin, HIGH);
	cm = convert_to_distance(time);

	//pir sensor

	digitalWrite(pir_in,LOW);
	delayMicroseconds(5);
		
	// conditions and functionalities

	if( cm < 5)
	{
		//Serial.print(" WARNING ! CLEAR BIN");
		BTserial.print("FULL");
		pir_flag = 1;
	}
	else
	{	
		//Serial.print("trash at ");
		//Serial.print(cm);
		//Serial.print(" cm");
		//Serial.println();
		BTserial.print(cm);
		BTserial.print("cm");

		delay(50);
	}
	if(DigitalRead(pir_in) == HIGH)
	{
		if(pir_flag == 0)
		{
			for(angle = 0;angle < 180;angle+=5)
			{
				servo.write(angle);

			}
			delay(10);
			for(angle = 180;angle > 0; angle -= 5)
			{
				servo.write(angle);
			}
			
		}
		else
		{
			BTserial.print("FULL!SORRY");
			//Serial.println();
		}
	}
	

	//	lm35 sensor


	val = analogRead(temp_pin);
	r = (val/1024.0)*5000;
	celcius = mv/10;
	//Serial.print(" the temperature is ");
	//Serial.print(celcius);
	//Serial.print(" degrees celcius");
	//Serial.println();
	BTserial.print("temp:");
	BTserial.print(celcius);
	delay(5);	
}

long convert_to_distance(long ms)
{
	//speed of sound is 29 mu_s/cm
	return ms / 29 / 2;
}
