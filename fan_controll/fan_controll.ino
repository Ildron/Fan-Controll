
/* Arduino fan-controller for heat recovery unit ,2 fans 3 speed's   */

/* I/O pinn's */

int Relay1 = 2;       // Relay 1 (Speed 2 or 3) connected to output pin 2
int Relay2 = 3;       // Relay 2 (Speed 1 or 2/3)  connected to output pin 3
int Relay3 = 4;       // Relay 3 (FAN 2 Speed 2 or 3) connected to output pin 4
int Relay4 = 5;       // Relay 3 (FAN 2 Speed 1 or 2/3) connected to output pin 5
int spjeld_input = 6; //input ,, read spjeld posisiton input pin 6
int Spjeld_motor = 8; // STR 2 (spjeld) connected to output pin 8
int Fans_on_off = 9;  // STR 1 (spjeld) connected to output pin 9

int speed_inn1 = 10;       // speed-1 input pin pin 10
int speed_inn2 = 11;       // speed-1 input pin pin 11
int speed_inn3 = 12;       // speed-1 input pin pin 12

int summer_pin_inn = 7;       //  input pin pin 7
int fireplace_mode_pin_inn = 13; // input pin pin 13

int spjel_pos = 0; // in stop position
int speed_switch = 2;
int last_speed_switch=2;

int summer_cool=0;
int fireplace_mode=0;

// the setup function runs once when you press reset or power the board
void setup() {
 
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(Relay1, OUTPUT);  // Relay 1 (Speed 2 or 3)
  digitalWrite(Relay1, LOW);
  
  pinMode(Relay2, OUTPUT);  // Relay 2 (Speed 1 or 2/3)
  digitalWrite(Relay2, LOW); 
  
  pinMode(Relay3, OUTPUT);  // Relay 3 (FAN 2 Speed 2 or 3)
  digitalWrite(Relay3, LOW); 
  
  pinMode(Relay4, OUTPUT);  // Relay 4 (FAN 2 Speed 1 or 2/3)
  digitalWrite(Relay4, LOW);  

  pinMode(spjeld_input, INPUT_PULLUP);  // input ,, read spjeld posisiton

  pinMode(Spjeld_motor, OUTPUT);   // STR 2 (spjeld)
  digitalWrite(Spjeld_motor, LOW); 
  
  pinMode(Fans_on_off, OUTPUT);   // STR1 (power fan on/off)
  digitalWrite(Fans_on_off, LOW);  
  
  pinMode(speed_inn1, INPUT_PULLUP);  // input ,, switch posisiton
  pinMode(speed_inn2, INPUT_PULLUP);  // input ,, switch posisiton
  pinMode(speed_inn3, INPUT_PULLUP);  // input ,, switch posisiton

  pinMode(summer_pin_inn, INPUT_PULLUP);          // input ,, read summer mode ,, no turning of spjeld
  pinMode(fireplace_mode_pin_inn, INPUT_PULLUP);  // input ,, run in-fan on higher speed than out-fan, overpressure

  
    Speed1(1,1);   // set inital speed
}

/**************************************************/
int check_speed_switch()
{
    int returnval=0;
    int temp=0;
      
   temp = digitalRead(speed_inn1);   // read switch position
    if (!temp) return (returnval=1);  // low 
   
   temp = digitalRead(speed_inn2);   // read switch position
    if (!temp) return(returnval=2);  // low  

   temp = digitalRead(speed_inn3);   // read switch position
    if (!temp) return (returnval=3);  // low 
}

/**************************************************/
 int check_summer_mode()
 {
    int returnval=0;
    int temp=0;
    
      temp = digitalRead(summer_pin_inn);
      if (!temp) return (returnval=1);  // low 
 }

/**************************************************/
 int check_fireplace_mode()
 {
    int returnval=0;
    int temp=0;
    
      temp = digitalRead(fireplace_mode_pin_inn);
      if (!temp) return (returnval=1);  // low 
 }

/****************************************/
int turn_spjeld()
{
    int returnval=0;
    int i=0;
    int j=0;

#ifdef DEBUG
  Serial.print("turning spjeld");
#endif
   digitalWrite(Fans_on_off, LOW); 
   if (speed_switch ==1) delay(500);
   if (speed_switch ==2) delay(1300);
   if (speed_switch ==3) delay(1300);
   
   spjel_pos = digitalRead(spjeld_input);   // read the spjeld possition
   if (spjel_pos)    // Low ,, in last right posiston 
       {
        digitalWrite(Spjeld_motor, HIGH);  //turn on spjeld motor  start turn it out off position
        for (i=0;i<100;i++)
        { 
         delay(50); 
         spjel_pos = digitalRead(spjeld_input);   // read the spjeld possition
         if (!spjel_pos) // High , means it is in middle position ,air goes both ways ,, 
             {  
              /*****************************/
              returnval = 1; 
              for (j=0;j<200;j++)     // wait max 5 sec , for it to go into end position again
                  { 
                  delay(25); 
                  spjel_pos = digitalRead(spjeld_input);   // read the spjeld possition
                  if (spjel_pos) // Low , in close position
                      {
                      digitalWrite(Spjeld_motor, LOW);  //turn off spjeld motor
                      #ifdef DEBUG
                      Serial.print("  stop count :");
                      Serial.print(j);
                      #endif
                      break;
                      }
                  else{}
                  }              
             /*****************************/
             break;
             }
         else { }
        }
       }
       else    // not in right posisition
       {
        returnval=0;
        digitalWrite(Spjeld_motor, HIGH);  //turn on spjeld motor , move it to the right position
        for (j=0;j<100;j++) 
                  { 
                  delay(50); 
                  spjel_pos = digitalRead(spjeld_input);   // read the spjeld possition
                  if (spjel_pos) // low
                      {
                      digitalWrite(Spjeld_motor, LOW);  //turn off spjeld motor
                      break;
                      }
                  else{}
                  }        
       }
       
   digitalWrite(Spjeld_motor, LOW);  //turn off spjeld motor   timed out
  
   digitalWrite(Fans_on_off, HIGH);  /* turn on fans again  */
  return (returnval); 
}
/**************************************************/
void Speed1(int in , int out)
{
  digitalWrite(Fans_on_off, LOW); // turn of solid state relay
  delay(50);
  if (in)
    {
     digitalWrite(Relay1, HIGH);  //Relay on, Speed 2 on
     digitalWrite(Relay2, LOW);   // Relay 2 off , speed 3 off 
    }
    if (out)
    {
     digitalWrite(Relay3, HIGH);  //Relay on, Speed 2 on
     digitalWrite(Relay4, LOW);   // Relay 4 off , speed 3 off 
    }
  delay(50);
  digitalWrite(Fans_on_off, HIGH); 
}

/**********************************************/
void Speed2(int in , int out)
{
  digitalWrite(Fans_on_off, LOW);
  delay(50);
  if (in)
    {
     digitalWrite(Relay1, LOW);  //Relay off, Speed 2 off
     digitalWrite(Relay2, LOW);  // Relay 2 off , speed 3 off 
    }
 if (out)
    {
     digitalWrite(Relay3, LOW);  //Relay off, Speed 2 off
     digitalWrite(Relay4, LOW);  // Relay 4 off , speed 3 off 
    }
  delay(50);
  digitalWrite(Fans_on_off, HIGH); 
}

/**********************************************/
void Speed3(int in , int out)
{
  digitalWrite(Fans_on_off, LOW);
  delay(50);   
  if (in)
    {
      digitalWrite(Relay2, HIGH);  // Relay 2 on , speed 3
    }
    if (out)
    {
      digitalWrite(Relay4, HIGH);  // Relay 2 on , speed 3
    }
  delay(50);
  digitalWrite(Fans_on_off, HIGH);  
}

/**********************************************/
void blink_led_1()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300); 
  digitalWrite(LED_BUILTIN, LOW); 
}


void blink_led_2()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300); 
  digitalWrite(LED_BUILTIN, LOW); 
  delay(300);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300); 
  digitalWrite(LED_BUILTIN, LOW); 
}

void blink_led_3()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300); 
  digitalWrite(LED_BUILTIN, LOW); 
  delay(300);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300); 
  digitalWrite(LED_BUILTIN, LOW); 
  delay(300);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300); 
  digitalWrite(LED_BUILTIN, LOW); 
}



// the loop function runs over and over again forever
/********************************************************/
void loop() {

int ok = 0;
  
  delay(50000); 
  /*
  Speed1(0,1);   
  delay(5000); 
  Speed2(0,1);   
  delay(5000); 
  Speed3(0,1);
  delay(5000); 
  Speed1(0,1);     

  delay(5000); 
  Speed1(1,0);   
  delay(5000); 
  Speed2(1,0);   
  delay(5000); 
  Speed3(1,0); 
  delay(5000); 
  Speed1(1,0);   

  delay(5000); 
  Speed1(1,1);   
  delay(5000); 
  Speed2(1,1);   
  delay(5000); 
  Speed3(1,1);
  delay(5000); 
  Speed1(1,1); 
  */

  /***** Check input switches  *****/
  fireplace_mode  = check_fireplace_mode();
  summer_cool     = check_summer_mode();
  
  /***** Check the position of the switch *****/
  speed_switch = check_speed_switch();
  if (speed_switch != last_speed_switch) 
     { 
      switch (speed_switch)
      {
      case 1 : Speed1(1,1);
               blink_led_1();
               break;
      case 2 : Speed2(1,1);
               blink_led_2();
               break;
      case 3 : Speed3(1,1);
               blink_led_3();
      }
      last_speed_switch = speed_switch;
     }
       
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);  
  digitalWrite(LED_BUILTIN, LOW);
  delay(300); 
  digitalWrite(LED_BUILTIN, HIGH); //1
  delay(300);  
  digitalWrite(LED_BUILTIN, LOW);
  delay(300); 
  digitalWrite(LED_BUILTIN, HIGH); //2
  delay(300);  
  digitalWrite(LED_BUILTIN, LOW);
  delay(300); 
  digitalWrite(LED_BUILTIN, HIGH); //3
  delay(300);  
  digitalWrite(LED_BUILTIN, LOW);
  delay(300); 
  digitalWrite(LED_BUILTIN, HIGH); //4
  delay(300);  
  digitalWrite(LED_BUILTIN, LOW);
  delay(300); 
  digitalWrite(LED_BUILTIN, HIGH); //5
  delay(300);  
  digitalWrite(LED_BUILTIN, LOW);  //5.1 sec


  
  /***** turn spjeld *****/
  ok = turn_spjeld();
  
}
