#include <IRLib.h>
#include <Servo.h>
#include <Stepper.h>

#define MY_PROTOCOL NEC //Defines the type of remote being used
#define RIGHT_ARROW   0x61A0A857 //Move stepper clockwise
#define LEFT_ARROW    0x61A06897 //Move stepper counterclockwise
#define SELECT_BUTTON 0x61A018E7 //Stop the stepper
#define RECALL_BUTTON 0X61A008F7 //center the servo
#define BUTTON_0 0x61A0906F  //Pushing buttons 0-9 moves to fixed positions
#define BUTTON_1 0x61A000FF  // each 20 degrees greater
#define BUTTON_2 0x61A0807F
#define BUTTON_3 0x61A040BF
#define BUTTON_4 0x61A0C03F
#define BUTTON_5 0x61A020DF
#define BUTTON_6 0x61A0A05F
#define BUTTON_7 0x61A0609F
#define BUTTON_8 0x61A0E01F
#define BUTTON_9 0x61A010EF

#define STEPS 32 //number of steps taken by the stepper motor on an input
int Steps2Take;

Stepper stepper(STEPS, 5, 4, 3, 2); //pins used with ULN 2003 Motor driver, written in order IN1 IN3 IN2 IN4
IRrecv My_Receiver(11);//Receive on pin 11
IRdecode My_Decoder;
Servo My_Servo;  // create servo object to control a servo
int pos;         // variable to store the servo position
int Speed;       // Number of degrees to move each time a left/right button is pressed
long Previous;   // Stores previous code to handle NEC repeat codes
void setup()
{
  My_Servo.attach(9);  // attaches the servo on pin 9 to the servo object
  pos = 90;            // start at midpoint 90 degrees
  Speed = 3;           // servo moves 3 degrees each time left/right is pushed
  My_Servo.write(pos); // Set initial position
  My_Receiver.enableIRIn(); // Start the receiver
  stepper.setSpeed(240); //60 rpm
}

void loop()
{
  if (My_Receiver.GetResults(&My_Decoder)) { //sets up IR reciever to read an input
    My_Decoder.decode();
    if (My_Decoder.decode_type == MY_PROTOCOL) {
      if (My_Decoder.value == 0xFFFFFFFF)
        My_Decoder.value = Previous; //makes sure only one input is happening at a time
      switch (My_Decoder.value) {
        case LEFT_ARROW:    stepper.step(360); break;
        case RIGHT_ARROW:   stepper.step(-360); break;
        case SELECT_BUTTON: stepper.step(0); break;
        case RECALL_BUTTON: pos = 90; break;
        case BUTTON_0:      pos = 0 * 20; break;
        case BUTTON_1:      pos = 1 * 20; break;
        case BUTTON_2:      pos = 2 * 20; break;
        case BUTTON_3:      pos = 3 * 20; break;
        case BUTTON_4:      pos = 4 * 20; break;
        case BUTTON_5:      pos = 5 * 20; break;
        case BUTTON_6:      pos = 6 * 20; break;
        case BUTTON_7:      pos = 7 * 20; break;
        case BUTTON_8:      pos = 8 * 20; break;
        case BUTTON_9:      pos = 9 * 20; break;
      }
      My_Servo.write(pos); // tell servo to go to position in variable 'pos'
      Previous = My_Decoder.value;
    }
    My_Receiver.resume();
  }
}
