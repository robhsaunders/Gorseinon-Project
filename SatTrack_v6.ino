#include <AccelStepper.h>
#include <SoftwareSerial.h>

//AZ0.0 EL0.0 UP0 DN0
//AZ345.0 EL5.0 UP0 DN0

// Define a stepper motor 1 for arduino 
// direction Digital 9 (CW), pulses Digital 8 (CLK)
AccelStepper stepperAz(1, 8, 9);
AccelStepper stepperEl(1,10,11);
SoftwareSerial Bluetooth(3, 2); // RX, TX

int SpeedMaxAz = 2000;
int SpeedMaxEl = 1000;
int AccelerationAz = 5000;
int AccelerationEl = 5000;
float GearRatioAz = 20.1;//Gear reduction ratio -(1=None, 1:??)
int StepsPerRevAz = 6400.0;//200 typical.  My controller goes as fine as 6400.
float GearRatioEl = 15.0;//Gear reduction ratio -(1=None, 1:??)
int StepsPerRevEl = 6400.0;//200 typical.  My controller goes as fine as 6400.

char serialData;
String sChar;
String sAzMoveTo;
String sElMoveTo;
int iAzMoveTo;
int iElMoveTo;
int AzSpeed;

String sIncomingData;
String sIncomingPartOne;
String sIncomingPartTwo;
String sSpaceChr = " ";
char c;
int iIncomingSection;
String AzAngle;
String ElAngle;

float AzCal;// = 357.33333;//11.18889;//4028 step per revolution.
float ElCal;

float StepperDegPerRev;
float GearDegPerStep;
float GearStepsPerRev;
float GearStepsPerDeg;


void setup()
{  
   //Serial.begin(9600);  
   Bluetooth.begin(9600); // set up Serial library at 9600 bps
   Serial.println("SatTrack Ready!");
  
  // Change these to suit your stepper if you want
  stepperAz.setMaxSpeed(SpeedMaxAz);//1100    
  stepperAz.setAcceleration(AccelerationAz);//1100
  stepperEl.setMaxSpeed(SpeedMaxEl);//1100    
  stepperEl.setAcceleration(AccelerationEl);//1100

   StepperDegPerRev = (360.0/StepsPerRevAz);
   GearDegPerStep = (StepperDegPerRev/GearRatioAz);
   GearStepsPerRev = (360.0/GearDegPerStep);
   AzCal = (GearStepsPerRev/360.0);
   Bluetooth.print("Calculated Steps per degree for Azimuth:  ");
   Bluetooth.println(AzCal);
   
   StepperDegPerRev = (360.0/StepsPerRevEl);
   GearDegPerStep = (StepperDegPerRev/GearRatioEl);
   GearStepsPerRev = (360.0/GearDegPerStep);
   ElCal = (GearStepsPerRev/360.0);
   Bluetooth.print("Calculated Steps per degree for Elivation:  ");
   Bluetooth.println(ElCal);
}

void loop()
{

  if (stepperAz.distanceToGo() == 0){
    if (stepperEl.distanceToGo() == 0){
      //while (Serial.available()) {
      while (Bluetooth.available()) {
        delay(10);//Good
        //serialData = Serial.read();
        serialData = Bluetooth.read();
        sIncomingData += serialData;
        //Serial.println("sIncomingData:  " + sIncomingData);
        c = serialData;
        sChar = c;
        //Serial.println("sChar:  " + sChar);
        //Serial.println("char c =  " + c);
        if (sChar == "A"){iIncomingSection = 1;}
        if (iIncomingSection == 1){if (sChar == "."){iIncomingSection = 2;}}
        if (sChar == "E"){iIncomingSection = 3;}
        if (iIncomingSection == 3){if (sChar == "."){iIncomingSection = 4;}}
        if (iIncomingSection == 1){sIncomingPartOne += serialData;}
        if (iIncomingSection == 2){AzAngle = sIncomingPartOne.substring(2);}
        if (iIncomingSection == 3){sIncomingPartTwo += serialData;}
        if (iIncomingSection == 4){ElAngle = sIncomingPartTwo.substring(2);iIncomingSection = 5;}
        if (iIncomingSection == 5){
          //Serial.println("sIncomingPartOne:  " + sIncomingPartOne);
          //Serial.println("sIncomingPartTwo:  " + sIncomingPartTwo);
          sIncomingPartOne = "";sIncomingPartTwo = "";
          //Serial.println("Step5");
          iIncomingSection = 0;
          Bluetooth.println("AzAngle:  " + AzAngle);
          Bluetooth.println("ElAngle:  " + ElAngle);
        }
        sAzMoveTo = AzAngle;
        sElMoveTo = ElAngle;
      }
    }
  }
      if (sAzMoveTo.length() != 0){
        iAzMoveTo = sAzMoveTo.toInt();
        sAzMoveTo = "";//Good
        stepperAz.moveTo(AzCal * iAzMoveTo);
      }
    
    if (stepperAz.distanceToGo() != 0){
      stepperAz.run();
    }
    if (sElMoveTo.length() != 0){
      iElMoveTo = sElMoveTo.toInt();
      sElMoveTo = "";//Good
      stepperEl.moveTo(ElCal * iElMoveTo);
    }
  
    if (stepperEl.distanceToGo() != 0){
      stepperEl.run();
    }
}
