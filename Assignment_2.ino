/************************************************************************************
-----------------------ASSIGNMENT 2 BY GODFREY INYAMA--------------------------------
---------------------PRESENTED ON THE 10TH OF MARCH 2022-----------------------------
---------------------SUBMITTED ON THE 11TH OF MARCH 2022-----------------------------
                    AIM:DEMONSTRATION OF CYCLIC EXECUTIVE
This code is developed to show how a cyclic executive works, by performing certain tasks.
Task 1: Output of Signal B from Assignment 1
Task 2: Monitoring of one digital input
Task 3: Measuring the frequency of a 3.3v square wave signal
Task 4: Reading one analogue input
Task 5: Computing the average of the last four analogue signals
Task 6: Executing _asm_ _ volatile_ ("nop");
Task 7: Performing  if (average_analogue_in > half of maximum range for 
analogue input):
    error_code = 1
else:
    error_code = 0
Task 8: Vizualing the error with an LED
Task 9: Logging the following every 5sec
State of the digital input (pushbutton / switch);
Frequency value (Hz, as an integer);
Filtered analogue input.
**************************************************************************************/


#define LED 21                             //Task1 WatchDog Signal from Signal B in Assignment1 which is 50us
#define error_LED 5                        // LED to vizualize the error_code from Task 7 in Task 8
#define digital_input 4                    //Task2 Digital Input Monitor using one switch
#define squarewave_reader 2                //Task3 3.3v square wave for reading frequency
#define analogue_reader 34                 //Task4 Analogue Input reader using a potentiometer

float analogue_input_task4[4] = {0,0,0,0}; //array to store 4 cocurrent values from task 4 in a map
float task5_average = 0;                   //initializing task5_average value as LOW

int error_code = 0;                        //initializing error value as LOW
int monitor_task2 = 0;                     //initializing task5_average value as false
int Counter = 0;                           // Counter to record the number of cycle went through by the Cyclic Executive

//======================================================//
//-----------variables for Task 3-----------------------//
//======================================================//
int freq_flag = 0; //the flag to indicate if the signal is low then went high
int task3_frequency = 0; //the value of the frequency in Hz
int freq_count = 0; //The number of pulses over the time period
int unfiltered_frequency;
int unfiltered_frequency_old;
unsigned long start_timeF;
unsigned long currentTime;


//Rates (time) for each task performance
const int Time_Task1 = 14;                     //actual value = 14.45
const int Time_Task2 = 200; 
const int Time_Task3 = 1000;
const int Time_Task4 = 42;                     //actual value = 41.67
const int Time_Task5 = 42;                     //actual value = 41.67
const int Time_Task6 = 100;
const int Time_Task7 = 33;                     //actual value = 33.33
const int Time_Task8 = 33;                     //actual value = 33.33 
const int Time_Task9 = 5000;
const int B = 50;                              //time of HIGH from Assignment 1


//======================================================//
//---------------On and Off Times-----------------------//
//======================================================//
const unsigned int onTime = B / 1000;
const unsigned int offTime = Time_Task1;
 
unsigned long previousMillis=0;                //Tracks the last time event fired
int interval = onTime;                         // Interval is how long we wait
boolean ledState = true;                       // Used to track if LED should be on or off

//===============================================================//
//------------Function models to be called in the loop-----------// 
//===============================================================//
void task1();
void task2();
void task3();
void task4();
void task5();
void task6();
void task7();
void task8();
void task9();

/=================================//
//-------function for task 1------//
//================================//
void task1() {
  //Serial.println("Task 1");
  // Set LED to state of ledState each timethrough loop()
  // If ledState hasn't changed, neither will the pin
  digitalWrite(LED, ledState);
 
  // This keeps all timing consistent, regardless of how much code is inside the next if-statement
  unsigned long currentMillis = millis();
 
  // Compare to previous capture to see if enough time has passed
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    while (millis() <= (currentMillis + onTime)){
  
  //keep LED on for onTime
      digitalWrite(LED, HIGH);
    }
    digitalWrite(LED, LOW);
 
  // Save the current time to compare "later"
    previousMillis = currentMillis;
  }
}
  

/=================================//
//-------function for task 2------//
//================================//
void task2() {
  //Serial.println ("Task 2");
  if (digitalRead (digital_input)){
    monitor_task2 = 1;
  }
  else{
    monitor_task2 = 0;
  }
}



/=================================//
//-------function for task 3------//
//================================//
void task3() {
  //Serial.println("Task 3");
  unfiltered_frequency=digitalRead(squarewave_reader);
  unfiltered_frequency_old= unfiltered_frequency;
  freq_count=0;    
 

  start_timeF= micros(); //define the start time of the clock
  currentTime=micros();
    while ((currentTime-start_timeF) < 40000){  //effectively delays for 0.04seconds, the optimum time for detecting frequency that keeps errors within +/- 2.5%
       
      unfiltered_frequency=digitalRead(squarewave_reader);
      if (unfiltered_frequency_old != unfiltered_frequency){ //If frequency input is high and was previously low (ie flag is on) add one to the value of the frequency counter.
        freq_count++;
        unfiltered_frequency_old=unfiltered_frequency;}
     
      currentTime=micros();
        
    } 
    if (micros()>= start_timeF +40000){ //calculate the frequency by scaling 0.04s up to 1s
        task3_frequency =freq_count*25/2;  
        
      }
}


/=================================//
//-------function for task 4------//
//================================//
void task4() {
  //Serial.println ("Task 4");

  for(int i=1; i<4; i++){
    analogue_input_task4[i] = analogue_input_task4[i+1];
  }
  analogue_input_task4[4] = analogRead (analogue_reader);
  }


/=================================//
//-------function for task 5------//
//================================//
void task5() {
  //Serial.println ("Task 5");
  task5_average = 0;
  float analogue_value = analogRead (analogue_reader);
  analogue_input_task4[0] = analogue_value;
  analogue_input_task4[1] = analogue_value+1;
  analogue_input_task4[2] = analogue_value+2;
  analogue_input_task4[3] = analogue_value+3;
  task5_average = analogue_input_task4[0] + analogue_input_task4[1] + analogue_input_task4[2] + analogue_input_task4[3];
  task5_average = task5_average / 4;
}


/=================================//
//-------function for task 6------//
//================================//
void task6() {
  //Serial.println ("Task 6");
  for(int i=0; i<1000; i++){
    __asm__ __volatile__ ("nop");
  }
}


/=================================//
//-------function for task 7------//
//================================//
void task7() {
  //Serial.println ("Task 7");

  int max_range = 2513;
  int half_max = max_range / 2;
  if(task5_average > half_max){
    error_code = 1;
  }
  else{
    error_code = 0;
  }
}


/=================================//
//-------function for task 8------//
//================================//
void task8() {
  //Serial.println ("Task 8");
 
  if(error_code){
    digitalWrite(error_LED,HIGH);
  }
  else
   {
   digitalWrite(error_LED,LOW);
  }
}


/=================================//
//-------function for task 9------//
//================================//
void task9() {  
  Serial.println((String)monitor_task2+ "," + (String) task3_frequency + "Hz," + (String)task5_average);
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
}

void loop(){
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
