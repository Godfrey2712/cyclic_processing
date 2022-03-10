//ASSIGNMENT 2 BY GODFREY INYAMA//
//DEMONSTRATION OF CYCLIC EXECUTIVE//


#define LED 21 //Task1 WatchDog Signal from SignalB in Assignment1
#define error_LED 5 // Vizualise the error_code from Task7
#define digital_input 4 //Task2 Digital Input
#define squarewave_reader 2 //Task3 3.3v square wave
#define analogue_reader 34 //Task4 Analogue Input

float analogue_input_task4[4] = {0,0,0,0}; //
float task5_average = 0; //initializing task5_average value as LOW

int error_code = 0;//initializing error value as LOW
int monitor_task2 = false; //initializing task5_average value as false
int readValue = 0;
int ledValue = 0;
float half_max = 3.3/2;
int Counter = 0;

int frequency_counted;
unsigned long my_time;
volatile int task3_frequency;
unsigned long frequency_task_time = 0;

//task list
void frequency_ISR();
void frequency_routine();

const int Time_Task1 = 14; //actual value = 14.45
const int Time_Task2 = 200; 
const int Time_Task3 = 1000;
const int Time_Task4 = 42; //actual value = 41.67
const int Time_Task5 = 42; //actual value = 41.67
const int Time_Task6 = 100;
const int Time_Task7 = 33; //actual value = 33.33
const int Time_Task8 = 33; //actual value = 33.33 
const int Time_Task9 = 5000;
const float B = 50;

unsigned int previousTime_LED = 0;
int previousTime;

//Function models
void task1();
void task2();
void task3();
void task4();
void task5();
void task6();
void task7();
void task8();
void task9();

//function for task 1
void task1() 
{
  unsigned int presentTime = millis();
//for task 1
  if (presentTime - previousTime_LED >= Time_Task1) {
  while (millis()  <= (presentTime + (B/1000))) {
  digitalWrite(LED, HIGH);
  Serial.print(presentTime / 1000); //Counting back in seconds
  Serial.println("Task 1");
    }
    digitalWrite(LED, LOW);
    previousTime_LED = presentTime;
  //Serial.println ("Task 1");
  //digitalWrite (LED, HIGH);
  //delayMicroseconds(50);
  //digitalWrite (LED, LOW);
  }
}
  
void task2() {
  Serial.println ("Task 2");
  if (digitalRead (digital_input)){
    monitor_task2 = true;
  }
  else{
    monitor_task2 = false;
  }
  }

void frequency_ISR(){
  frequency_counted = frequency_counted + 1;
      
    }

void task3() {
  task3_frequency = frequency_counted;
  frequency_counted = 0;
  Serial.printf("%d ", task3_frequency);
   
  }

void task4() {
  Serial.println ("Task 4");
  Serial.println (analogRead(analogue_reader));
  for(int i=1; i<4; i++){
    analogue_input_task4[i] = analogue_input_task4[i+1];
  }
  analogue_input_task4[4] = analogRead (analogue_reader);
  Serial.print("Analog value = ");
  Serial.println(analogRead(analogue_reader));
  }

void task5() {

  task5_average = 0;
  task5_average = analogue_input_task4[1] + analogue_input_task4[2] + analogue_input_task4[3] + analogue_input_task4[4];
  task5_average = task5_average / 4;
  Serial.print ("Task 5 average = ");
  Serial.println (analogue_input_task4[4]);

  }

void task6() {
  Serial.println ("Task 6");
  for(int i=0; i<1000; i++){
    __asm__ __volatile__ ("nop");
  }
  }

void task7() {
  Serial.println ("Task 7");
  float half_max = 3.3/2;
  if(analogRead (analogue_reader) > half_max){
    error_code = 1;
  }
  else{
    error_code = 0;
  }
  }

void task8() {
  Serial.println ("Task 8");
 
  if(error_code){
    digitalWrite(error_LED,HIGH);
  }
  else{
   digitalWrite(error_LED,LOW);
  }
  }

void task9() {
  Serial.println ("Task 9");
  Serial.print ("SWITCH VALUE : ");
  Serial.println (monitor_task2);
  Serial.print ("Square wave Frequency = ");
  Serial.println (monitor_task2);
  Serial.print ("Analogue Input = ");
  Serial.println (task5_average);
  
  }
  
void setup() {


// initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

// make the pushbutton's pin an input:
  pinMode(digital_input, INPUT);
  pinMode(squarewave_reader, INPUT);
  pinMode(analogue_reader, INPUT);
  
// put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(error_LED, OUTPUT);

attachInterrupt(digitalPinToInterrupt(squarewave_reader), frequency_ISR, RISING);
  previousTime = millis();
}

void loop() {
 
Counter++;

if (Counter % Time_Task1 == 0) task1();
if (Counter % Time_Task2 == 0) task2();
if (Counter % Time_Task3 == 0) task3();
if (Counter % Time_Task4 == 0) task4();
if (Counter % Time_Task5 == 0) task5();
if (Counter % Time_Task6 == 0) task6();
if (Counter % Time_Task7 == 0) task7();
if (Counter % Time_Task8 == 0) task8();
if (Counter % Time_Task9 == 0) task9();
}
