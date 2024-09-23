/* STRAIN zero error = +0.06 */

#define mtrDirPin1 7  // In1 Pin on Motor Driver - Controls Motor Direction
#define mtrDirPin2 8  // In2 Pin on Motor Driver - Controls Motor Direction
#define mtrEnPin 10    // EnA Pin on Motor Driver - Turns Motor ON/OFF
#define irPin 2       // This is the IR sensor's (near the encoder disc) Output Pin
#define currentPin A0 // This pin will be used to sense Motor Current
#define isrTestPin 13 // This pin will toggle every time ISR is called

unsigned long pulseCount=0;  // No of pulses counted
unsigned char index=0;      // A dummy index used to store Current readings in an array
int analogValues[100]; 
int timeValues[100];
float strainValues[100]; 
bool testFinished = 0;
float rot = 0.0;
float big = 0.0;
float omega = 0.0;  // Variable to store omega
float strain = 0.0;
unsigned long prevPulseTime = 0;
unsigned long testStartTime = 0; 
unsigned long timed = 0.0;

void setup()
{
  // This code will run once initially
  pinMode(mtrDirPin1,OUTPUT);
  pinMode(mtrDirPin2,OUTPUT);
  pinMode(mtrEnPin,OUTPUT);
  pinMode(irPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(irPin),pulseDetected,FALLING);
  pinMode(isrTestPin,OUTPUT);
  Serial.begin(9600);
  digitalWrite(mtrDirPin1,HIGH);
  digitalWrite(mtrDirPin2,LOW);
  digitalWrite(mtrEnPin,HIGH);    // Full Speed
  analogWrite(mtrEnPin, 128); // Variable Speed, set second parameter 0 - 255 /* !!! TO BE MODIFIED !!!
  delay(5000); // since the com port cannot be used by 2 apps at once, after uploading, I will close arduino ide and open coolterm app and then start
  Serial.print("Time, ");
  Serial.print("Extension, ");
  Serial.println("Current");
}

void loop(){
 
}

void pulseDetected()  // This is the Interrupt Service Routine
{ // This will run everytime a falling edge is detected on the irPin
  digitalWrite(isrTestPin,!digitalRead(isrTestPin));
  // The ISR Test Pin toggled everytime a falling edge is detected!
  pulseCount++; // We detected a pulse! Increase the pule count by 1
  index++;
  if (index>=100)
  {
    index = 0;
  }
  // Index incrments with pulseCount but if it goes above 99, its resetted to 0
  if (pulseCount % 8 ==0){
      rot = pulseCount/8;  
      big = rot/10;
      strain=  big * 1.5; // !!!have to check again!!!
      float arr[3] = {millis(),strain,analogRead(A0)};
      analogValues[index] = arr;
      // to print time, strain,current in csv format, i have printed commas below
      Serial.print(arr[0]); 
      Serial.print(","); 
      Serial.print(arr[1]);
      Serial.print(",");
      Serial.println(arr[2]);
  }
}

