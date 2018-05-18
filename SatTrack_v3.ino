#include <AccelStepper.h>


// Define a stepper motor 1 for arduino 
// direction Digital 9 (CW), pulses Digital 8 (CLK)
AccelStepper stepper(1, 8, 9);

//AZ345.0 EL5.0 UP0 DN0

char serialData;
String sChar;
String sAzMoveTo;
String sElMoveTo;
int iAzMoveTo;
int AzSpeed;

String sIncomingData;
String sIncomingPartOne;
String sIncomingPartTwo;
String sSpaceChr = " ";
char c;
int iIncomingSection;
String AzAngle;
String ElAngle;
int SpeedMax = 10000;
int Acceleration = 5000;
float AzCal;// = 357.33333;//11.18889;//4028 step per revolution.
float GearRatio = 20.1;//Gear reduction ratio -(1=None, 1:??)
int StepsPerRev = 6400.0;//200 typical.  My controller goes as fine as 6400.
float StepperDegPerRev;
float GearDegPerStep;
float GearStepsPerRev;
float GearStepsPerDeg;


void setup()
{  
   Serial.begin(9600);           // set up Serial library at 9600 bps
   Serial.println("SatTrack Ready!");
  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(SpeedMax);//1100    
  stepper.setAcceleration(Acceleration);//1100


   StepperDegPerRev = (360.0/StepsPerRev);
   GearDegPerStep = (StepperDegPerRev/GearRatio);
   GearStepsPerRev = (360.0/GearDegPerStep);
   AzCal = (GearStepsPerRev/360.0);
   Serial.print("Calculated Steps per degree:  ");
   Serial.println(AzCal);
}

void loop()
{

  
    while (Serial.available()) {
      delay(10);//Good
      serialData = Serial.read();//Good
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
        Serial.println("AzAngle:  " + AzAngle);
        Serial.println("ElAngle:  " + ElAngle);
      }
      sAzMoveTo = AzAngle;
      sElMoveTo = ElAngle;
    }
    if (sAzMoveTo.length() != 0){
      iAzMoveTo = sAzMoveTo.toInt();//Good
      sAzMoveTo = "";//Good
      //AzAngle = "";

            
      
      stepper.moveTo(AzCal * iAzMoveTo);//Good
      //Serial.println(iMoveTo);
    }
  
    if (stepper.distanceToGo() != 0){
      stepper.run();
    }

}
