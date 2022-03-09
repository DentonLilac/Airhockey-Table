
//All positions are releative to T(op)Gate, with the gates at 
//the top and bottom of the table from a top down view

/* --Old Analog Inputs, no intersects in final version.
int Top_Goal = A0; //Top gate
int Bottom_Goal = A1; //Bottom Gate
int TopIntersect_Sensor = A2; //powerup sensor
int BottomIntersect_Sensor = A3; //powerup sensor
int RightIntersect_Sensor = A4; //powerup sensor
int LeftIntersect_Sensor = A5; //powerup sensor
int Left_Sensor = A6; //left side Led sensor
int Right_Sensor = A7; //Right side led sensor
int Return_Sensor = A8; //return tray sensor
int Bottom_Sensor = A9; //bottom led sensor
int Top_Sensor = A10; // top led sensor
*/


int Top_Goal1_Sensor = A0; //Top goal #1
int Top_Goal2_Sensor = A1; //Top goal #2
int Bottom_Goal1_Sensor = A2; //Bottom goal #1
//int Bottom_Goal2_Sensor = A3; //Bottom goal #1
int Left_Sensor = A4; //left side Led sensor
int Right_Sensor = A5; //Right side led sensor
//int Return_Sensor = A8; //return tray sensor
int Bottom_Sensor = A6; //bottom led sensor
int Top_Sensor = A7; // top led sensor


//Bumper side laser photoresistor data on table walls.
int Top_Sensor_Data = 0;
int Right_Sensor_Data = 0;
int Bottom_Sensor_Data = 0;
int Left_Sensor_Data = 0;

//Sensor data from the photoresistors inside the goals. Bottom Goal only needed one laser.
int Top_Goal1_Sensor_Data = 0;
int Top_Goal2_Sensor_Data = 0;
int Bottom_Goal1_Sensor_Data = 0;
//int Bottom_Goal2_Sensor_Data = 0;


//Timer indexes.
int Top_Sensor_Timer = 1;
int Bottom_Sensor_Timer = 2;
int Left_Sensor_Timer = 3;
int Right_Sensor_Timer = 4;
int Top_Goal_Timer = 5;
int Bottom_Goal_Timer = 6;
int Bottom_Left_Timer = 7;
int Bottom_Right_Timer = 8;
int Top_Left_Timer = 9;
int Top_Right_Timer = 10;
int Powerup_Timer = 11;

int Light_Samples_Timer = 12;
int Light_Sample_Collector_Timer = 13;

int LED_Startup_Timer = 20;
int LED_Shutdown_Timer = 21;

int Win_Timer = 22;

int LED_Fade_In_Timer = 23;

int LED_Game_Start_Timer = 24;

int Auto_Off_Timer = 63;


//Time in milliseconds before no action taking place causes table to enter shut-down mode until the power-on button is released and pressed again.
long Auto_Off_Time = 60000 * 5; // 60 (milli)seconds times 5, five minute auto-off.

//Value that turns on and off depending on if the table was shut down automatically.
bool Auto_Off = false;

//Enable/disable automatically turning the table off after Auto_Off_Time milliseconds(ms).
bool Auto_Off_Enabled = true;

//Speed for LEDs to fade in when turning table on.
float LED_Startup_Speed = 3000;

//Dont worry about these vars shhh...
float LED_Fade_In_Speed = 3750;
//bool LED_Fade_In_Pause = false;
float Brightness_Mod = 1;

//LED_Startup_Timer = 20;
//

int Top_Score=0;
int Bottom_Score=0;
int Win_Score=7; //score to win (There's only 7 LEDs on the table's score keeping LEDs, so you can really only make this 7 or less.
int Win_Display_Time = 8000; //Time to flash LEDs for winner/loser after a game is completed.


bool Top_Win = false;
bool Bottom_Win = false;

//Powerups were going to be a feature, but were later cut from the table. Could maybe be re-implemented somehow.. Goodluck.
bool Top_Left_Powerup = false;
bool Top_Right_Powerup = false;
bool Bottom_Left_Powerup = false;
bool Bottom_Right_Powerup = false;
bool Powerup_Active = false;


bool ShuttingDown = false;
bool StartingUp = false;

//Output digital pins to control the 5V Relay to power the Blower motor fans and the LED/laser Power Supply Unit.
int fan_relay = 4;
int psu_relay = 2;

//Photoresistor light threshold drop below average level before triggering event. Light_Threshold is for side table lasers, and Light_Threshold2 is for the average drop in goal sensors.
int Light_Threshold = 40; //25
int Light_Threshold2= 70;

//start_button indicates the digital pin for the table's on/off power button.
int start_button=7;

//Mode_button was going to be a seperate button on the table to change gamemodes.. but that idea got axed in construction.
//int mode_button=22;
//bool mode_button_held=false;

//Pin to control the output color to strobe the LED light on the power button.
int power_led=10;



const int Light_Samples = 30; //Amount of samples to take on start_light_sample() to get an average light value
int Sample_Delay = 40; //Delay in between samples in millis
int TopSenSamples[ Light_Samples ];
int RightSenSamples[ Light_Samples ];
int BottomSenSamples[ Light_Samples ];
int LeftSenSamples[ Light_Samples ];

int TopGoal1SenSamples[ Light_Samples ];
int TopGoal2SenSamples[ Light_Samples ];
int BottomGoal1SenSamples[ Light_Samples ];
//int BottomGoal2SenSamples[ Light_Samples ];

//Averaged light levels over time from photoresistors.
int TopSenAvg = 0;
int RightSenAvg = 0;
int BottomSenAvg = 0;
int LeftSenAvg = 0;
int TopGoal1SenAvg = 0;
int TopGoal2SenAvg = 0;
int BottomGoal1SenAvg = 0;

int LastTopGoal1Avg = 0;
int LastTopGoal2Avg = 0;
int LastBottomGoal1Avg = 0;
//int BottomGoal2SenAvg = 0;

int Current_Light_Sample = Light_Samples;



int GameMode=0;

//Gamemodes:
//0 - Off/Setup
//1 - Base Game
//2 - Advanced Mode -Unused right now.



void setup() {
  
  // put your setup code here, to run once:

  Serial.begin(115200);
  
  pinMode(start_button, INPUT_PULLUP);
  //pinMode(mode_button, INPUT);
  pinMode(power_led, OUTPUT);

  pinMode(fan_relay, OUTPUT);
  pinMode(psu_relay, OUTPUT);

  //Turn off all relays on first boot, just to make sure.
  digitalWrite(fan_relay, LOW);
  digitalWrite(psu_relay, LOW);
  digitalWrite(power_led, LOW);
  //Serial.begin(115200);
  

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //Randomize the random() function seed so that colors generated are random between each game.
  randomSeed( analogRead( 8 ) );

  //Call setup to LEDs to initialize various LED strips and provide default colors.
  led_setup();

  //Defunct code for when I was going to render the score to RCA video screens on each side of the table, but the Arduino Mega2560 Rev3 we were using wasn't powerful enough to render both the LED animations as well as drawing full frames to the RCA screens.
  //That and we didn't have time to build a frame above the table to hold the screens like we originally were planning. RIP video code.
  //video_setup();
 
}


//Called continuously while shutting down/auto-off to finish turning everything off once LEDs have been faded.
void DoShutdown()
{
    if( timer_running( LED_Shutdown_Timer ) == false && ShuttingDown == true )
    {
        GameMode = 0;
        //led_reset();
        digitalWrite(psu_relay, LOW);
        digitalWrite(power_led, LOW);
        ShuttingDown = false;

        Top_Score = 0;
        Bottom_Score = 0;
        Top_Win = false;
        Bottom_Win = false;
    }
}

void loop() {
    // put your main code here, to run repeatedly:
  
    //Read voltage data from the photoresistors across the table, to be processed and averaged later.
    Top_Sensor_Data = analogRead( Top_Sensor );
    Right_Sensor_Data = analogRead( Right_Sensor );
    Bottom_Sensor_Data = analogRead( Bottom_Sensor );
    Left_Sensor_Data = analogRead( Left_Sensor );
  
    Top_Goal1_Sensor_Data = analogRead( Top_Goal1_Sensor );
    Top_Goal2_Sensor_Data = analogRead( Top_Goal2_Sensor );
    Bottom_Goal1_Sensor_Data = analogRead( Bottom_Goal1_Sensor );
  
    //Update all running timers with the timer library I wrote. (-Denton)
    timer_update( millis() );
  
    //Update all led related functions.
    led_loop();
  
    //Collect light samples here and there.
    if( Current_Light_Sample < Light_Samples ){
      collect_light_sample();
    }
  
    //Start new light sampling if the last samples are done collecting.
    if( timer_running( Light_Sample_Collector_Timer ) == false ){
      timer_start( Light_Sample_Collector_Timer, 5000 );
      start_light_sample();
    }

    //Call to update video rendering... if it was used.
    //video_loop();

    //StartingUp is true as long as the startup timer is running.
    StartingUp = timer_running( LED_Startup_Timer );

    //If power button is on/down.
    if(digitalRead(start_button)==LOW){

      //Is the gamemode is default 0, and not in the process of shutting down, and not Automatically turned off, start turn on process.
      if( GameMode == 0 && ShuttingDown == false && Auto_Off == false ){
        
        StartingUp = true;
        led_generate_default();
        //Turn on fans.
        digitalWrite(fan_relay, HIGH);
        delay(500);
        //Turn on LED/Laser PSU and turn on the power button LED.
        digitalWrite(psu_relay, HIGH);
        digitalWrite(power_led, HIGH);
        //Initiate LED startup to slowly fade them in.
        led_startup_timer();
        timer_start( LED_Game_Start_Timer, 2000 );
        GameMode = 1;
        Top_Score = 0;
        Bottom_Score = 0;
        Top_Win = false;
        Bottom_Win = false;
        //Restart the automatic shutoff timer. It constantly counts down, and resets every time a goal is made. If the timer ever runs out, it auto shuts the table off.
        renew_auto_off_timer();
      }

      //If previously turned on and running and in the process of shutting down, call this.
      if( GameMode >= 0 && ShuttingDown == true ){
        DoShutdown();
      }
      
      //delay(1000);

      //If gamemode is not 0, AKA the table is running.
      if(GameMode!=0){

        //Keep power LED solid while on.
        analogWrite( power_led, 255 );

        //Execute main game update loop.
        gameloop();
  
        //Serial.println( timer_remaining( Auto_Off_Timer ) );
        //Serial.println( timer_remaining( Light_Samples_Timer ));
        if( timer_running( Auto_Off_Timer ) == false && Auto_Off == false && Auto_Off_Enabled == true )
        {
            Serial.println( "Automatically shutting table off due to inactivity." );
            Auto_Off = true;
            led_shutdown_timer();
            ShuttingDown = true;
            digitalWrite(fan_relay, LOW);
        }
        
        
      }else{
        //include idle effects
      }

    //Power button is up/off.
    }else{
  
      if( GameMode == 0 ){
        //Strobe power button LED on and off to show people where the dang switch to turn this heckin' thing on is.
        analogWrite( power_led, ( cos( float( millis() / 150.0 ) ) + 1.0 ) * 127.5 );
        //Serial.println( ( cos( float( millis() / 150.0 ) ) + 1.0 ) * 127.5 );
        if( ShuttingDown == true ){
          DoShutdown();
        }else{
          //If button is off and ShuttingDown process is done, reset Auto-off so that the table can be reset on new power button on press.
          Auto_Off = false;
        }
      }else{
        //Do shutdown process. 
        if( ShuttingDown == false ){
          led_shutdown_timer();
          ShuttingDown = true;
          digitalWrite(fan_relay, LOW);
        }
        DoShutdown();
      }
    }
    //LED EFFECTS

    if( millis() > 3000 ){
    //Serial.print( timer_running( Light_Samples_Timer ) );
    //Serial.print( Current_Light_Sample );
    //Serial.print(" ");
    //Serial.print( timer_remaining( Light_Samples_Timer ) );
    //Serial.print(" ");
  

    /*
    Serial.print( Top_Sensor_Data );
    Serial.print(" ");
    Serial.print( Right_Sensor_Data );
    Serial.print(" ");
    Serial.print( Bottom_Sensor_Data );
    Serial.print(" ");
    Serial.print( Left_Sensor_Data );
    Serial.print(" ");
  
    Serial.print( TopSenAvg );
    Serial.print(" ");
    Serial.print( RightSenAvg );
    Serial.print(" ");
    Serial.print( BottomSenAvg );
    Serial.print(" ");
    Serial.println( LeftSenAvg );
    */
    
  
    /*
    Serial.print( Top_Goal1_Sensor_Data );
    Serial.print(" ");
  
    Serial.print( TopGoal1SenAvg );
    Serial.print(" ");
  
    Serial.print( Top_Goal2_Sensor_Data );
    Serial.print(" ");
  
    Serial.print( TopGoal2SenAvg );
    Serial.print(" ");
  
    Serial.print( Bottom_Goal1_Sensor_Data );
    Serial.print(" ");
  
    Serial.println( BottomGoal1SenAvg );
    */
    
  
    //Serial.print( "GM: " );
    //Serial.println( GameMode );
    //Serial.print(" ");
    
    //Serial.print(" ");
    //Serial.println( 800);
  
    //Serial.println( LeftSenAvg );
   
    
    }
  
  
}


void renew_auto_off_timer()
{
  Serial.println( "Auto-off Timer Renewed!" );
  timer_start( Auto_Off_Timer, Auto_Off_Time );
}
