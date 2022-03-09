int PUP_Count = 0;
bool LED_Fade_In_Pause = false;
int LED_Fade_Pause_I = 10;

//Begin new collection of light samples to get the average light levels on sensors.
void start_light_sample()
{

  Current_Light_Sample = 0;
  //collect_light_sample();
  timer_start( Light_Samples_Timer, Sample_Delay );
  
}

void collect_light_sample()
{

  if( timer_running( Light_Samples_Timer ) == false && Current_Light_Sample < Light_Samples ) //&& timer_running( Bottom_Goal_Timer ) == false && timer_running( Top_Goal_Timer ) == false ){
  {
    //Serial.println( "Got sample! ");
    TopSenSamples[ Current_Light_Sample ] = Top_Sensor_Data;
    RightSenSamples[ Current_Light_Sample ] = Right_Sensor_Data;
    BottomSenSamples[ Current_Light_Sample ] = Bottom_Sensor_Data;
    //LeftSenSamples[ Current_Light_Sample ] = Left_Sensor_Data;
    LeftSenSamples[ Current_Light_Sample ] = int(Left_Sensor_Data);
    TopGoal1SenSamples[ Current_Light_Sample ] = Top_Goal1_Sensor_Data;
    TopGoal2SenSamples[ Current_Light_Sample ] = Top_Goal2_Sensor_Data;
    BottomGoal1SenSamples[ Current_Light_Sample ] = Bottom_Goal1_Sensor_Data;
    //BottomGoal2SenSamples[ Current_Light_Sample ] = Bottom_Goal2_Sensor_Data;
    //Serial.println( analogRead( Left_Sensor ) );
    //Serial.print("Samp '");
    //Serial.print( Current_Light_Sample );
    //Serial.print("' Data: ");
    //Serial.println( Left_Sensor_Data );

    Current_Light_Sample = Current_Light_Sample + 1;
    if( Current_Light_Sample >= Light_Samples ){
      //do averaging, end of samples

      if( TopGoal1SenAvg > 50 && TopGoal1SenAvg <= 1024 ){
        LastTopGoal1Avg = TopGoal1SenAvg;
      }
      if( TopGoal2SenAvg > 50 && TopGoal2SenAvg <= 1024 ){
        LastTopGoal2Avg = TopGoal2SenAvg;
      }
      if( BottomGoal1SenAvg > 50 && BottomGoal1SenAvg <= 1024 ){
        LastBottomGoal1Avg = BottomGoal1SenAvg;
      }
      
      TopSenAvg = 0;
      RightSenAvg = 0;
      BottomSenAvg = 0;
      LeftSenAvg = 0;
      TopGoal1SenAvg = 0;
      TopGoal2SenAvg = 0;
      BottomGoal1SenAvg = 0;
      //BottomGoal2SenAvg = 0;

      for( int i = 0; i < Light_Samples; i++ ){

        TopGoal1SenAvg = TopGoal1SenAvg + TopGoal1SenSamples[i];
        TopGoal2SenAvg = TopGoal2SenAvg + TopGoal2SenSamples[i];
        BottomGoal1SenAvg = BottomGoal1SenAvg + BottomGoal1SenSamples[i];
        //BottomGoal2SenAvg = BottomGoal2SenAvg + BottomGoal2SenSamples[i];
        TopSenAvg = TopSenAvg + TopSenSamples[i];
        RightSenAvg = RightSenAvg + RightSenSamples[i];
        BottomSenAvg = BottomSenAvg + BottomSenSamples[i];

        //Temporary fix for random LeftSenSamples[i] returning 0 for no reason when plotting all sensor values in serial????????? What even???
        //if( LeftSenSamples[i] >= 100 )
        //{
          LeftSenAvg = LeftSenAvg + LeftSenSamples[i];
        //}else{
        //  LeftSenAvg = LeftSenAvg + 680;
        //}
        //Serial.print(" EAT THE RICH: ");
        //Serial.println( LeftSenSamples[i] );
      }
      TopSenAvg = int( floorf( TopSenAvg / Light_Samples ) );
      RightSenAvg = int( floorf( RightSenAvg / Light_Samples ) );
      BottomSenAvg = int( floorf( BottomSenAvg / Light_Samples ) );
      LeftSenAvg = int( floorf( LeftSenAvg / Light_Samples ) );

      TopGoal1SenAvg = int( floorf( TopGoal1SenAvg / Light_Samples ) );
      TopGoal2SenAvg = int( floorf( TopGoal2SenAvg / Light_Samples ) );
      //BottomGoal1SenAvg = int( floorf( BottomGoal1SenAvg / Light_Samples ) );
      Serial.print( BottomGoal1SenAvg );
      Serial.print( " / " );
      Serial.print( Light_Samples );
      Serial.print( " = " );
      BottomGoal1SenAvg = int( floorf( BottomGoal1SenAvg / Light_Samples ) );
      Serial.println( BottomGoal1SenAvg );

      //Sometimes we get weird averages that loop all the way around to max int val... soooo dirty fix. Shhhh don't tell.

      //BottomGoal2SenAvg = int( floorf( BottomGoal2SenAvg / Light_Samples ) );
      //Serial.print(" LA: ");
      //Serial.println( LeftSenAvg );

      //Serial.println( "Averaged?" );
      
    }else{
      timer_start( Light_Samples_Timer, Sample_Delay );
    }
    
  }

  if( TopGoal1SenAvg < 20 or TopGoal1SenAvg > 1024 ){
        TopGoal1SenAvg = LastTopGoal1Avg;
        /*
        Serial.print( "Bad top1 avg: " );
        Serial.print( TopGoal1SenAvg );
        Serial.print("; Set to last: " );
        Serial.println( LastTopGoal1Avg );*/
      }
      if( TopGoal2SenAvg < 20 or TopGoal2SenAvg > 1024 ){
        TopGoal2SenAvg = LastTopGoal2Avg;
      }
      if( BottomGoal1SenAvg < 20 or BottomGoal1SenAvg > 1024 ){
        BottomGoal1SenAvg = LastBottomGoal1Avg;
        /*Serial.print( "Bad bottom1 avg: " );
        Serial.print( BottomGoal1SenAvg );
        Serial.print("; Set to last: " );
        Serial.println( LastBottomGoal1Avg );*/
      }

  
}

//Trigger effects and scoring when top scores on the bottom goal.
void top_score(){
      //led_clear_main();
      //Display the goal explosion effect longer if it's the final goal.
      if( Top_Score + 1 == 7 )
      {
          timer_start(Top_Goal_Timer, Win_Display_Time );
          timer_start(Bottom_Goal_Timer, Win_Display_Time );
          
      }else{
          timer_start(Top_Goal_Timer, 3000);
      }
      renew_auto_off_timer();
      Serial.println("TOP GOAL!");
      Top_Score++;
      bump_top();
      bump_bottom();
      bump_left();
      bump_right();
      //LED_Fade_In_Pause = true;
      //LED_Fade_Pause_I = 100;
      flip_led_direction();
      new_colors_after_goal();
}

//Trigger effects and scoring when bottom scores on the top goal.
void bottom_score(){
      //led_clear_main();
      //Display the goal explosion effect longer if it's the final goal.
      if( Bottom_Score + 1 == 7 )
      {
          timer_start(Top_Goal_Timer, Win_Display_Time );
          timer_start(Bottom_Goal_Timer, Win_Display_Time );
      }else{
          timer_start(Bottom_Goal_Timer, 3000);
      }
      renew_auto_off_timer();
      Serial.println("BOTTOM GOAL!");
      Bottom_Score++;
      bump_top();
      bump_bottom();
      bump_left();
      bump_right();
      //LED_Fade_In_Pause = true;
      //LED_Fade_Pause_I = 100;
      flip_led_direction();
      new_colors_after_goal();
}

//Change colors after a goal is scored.
void new_colors_after_goal()
{
    //LED_Fade_In_Pause = false;
    led_generate_new_colors();
    timer_start( LED_Fade_In_Timer, LED_Fade_In_Speed );
}


void gameloop(){
  
 if(GameMode==1){


    //Allow a goal if startup/shutdown conditions aren't in place and the goal sensor is triggered.
    if( StartingUp == false && ShuttingDown == false && ( Top_Goal1_Sensor_Data < TopGoal1SenAvg - Light_Threshold2 or Top_Goal2_Sensor_Data < TopGoal2SenAvg - Light_Threshold2 ) )
    {
        if( timer_running( Top_Goal_Timer ) == false && timer_running( Bottom_Goal_Timer ) == false && Top_Win == false && Bottom_Win == false && timer_running( LED_Game_Start_Timer ) == false ){
            top_score();
        }
    } 

    if( StartingUp == false && ShuttingDown == false && Bottom_Goal1_Sensor_Data < BottomGoal1SenAvg - Light_Threshold2 )
    {
        if( timer_running( Bottom_Goal_Timer ) == false && timer_running( Top_Goal_Timer ) == false && Top_Win == false && Bottom_Win == false && timer_running( LED_Game_Start_Timer ) == false ){
            bottom_score();
        }
    }

    //Reset scores after Win_Timer stops running.
    if( timer_running( Win_Timer ) == false && ( Top_Win == true or Bottom_Win == true ) ){
            Top_Score=0;
            Bottom_Score=0;
            Top_Win = false;
            Bottom_Win = false;
            led_generate_default();
            bump_top();
            bump_bottom();
            bump_left();
            bump_right();
            renew_auto_off_timer();
            timer_start( LED_Game_Start_Timer, 2000 );
    }

    //Trigger win if either player has reached max Win_Score.
    if ( ( Top_Score >= Win_Score || Bottom_Score >= Win_Score ) && timer_running( Win_Timer ) == false ){
      if(Bottom_Score >= Win_Score){
          Bottom_Win = true;
          Serial.println("BOTTOM PLAYER WINS!");
          led_clear_main();
          timer_start( Win_Timer, Win_Display_Time );
       }//include win effectes here
      if(Top_Score >= Win_Score){
          Top_Win = true;
          Serial.println("TOP PLAYER WINS!");
          led_clear_main();
          timer_start( Win_Timer, Win_Display_Time );
         }//include win effecte here
      
    }

    //For waiting until after goal animation plays to generate new colors and fade them in.

    
    /*if( ( (timer_running(Top_Goal_Timer) == false) && ( timer_running(Bottom_Goal_Timer) == false) ) && LED_Fade_Pause_I == 100 )
    {
        new_colors_after_goal();
        //Serial.println( "Gen colors!!!" );
        LED_Fade_Pause_I = 10;
        //LED_Fade_In_Pause = false;
      
    }*/
    /*
    if( LED_Fade_In_Pause1 == false )
    {
      Serial.print("test-GM: ");
      Serial.println( GameMode );
      Serial.print("StartingUp: ");
      Serial.println( StartingUp );
      new_colors_after_goal();
    }
    */

 }

 if(GameMode==2){
  //following code for Powerup activation.  When both sensors trip while Powerup is not active, start timer that prevents reactivation until over, then resetting corner
  /* --Not doing intersects and powerups for now.
  if(analogRead(TopIntersect_Sensor)>Light_Threshold2 && analogRead(LeftIntersect_Sensor)>Light_Threshold2 && !timer_running(Top_Left_Powerup) && !timer_running(Powerup_Timer)){
    timer_start(Top_Left_Timer, 2000);
    Top_Left_Powerup = true;
    } 
      else if(!timer_running(Top_Left_Powerup) && !timer_running(Powerup_Timer)){
        Top_Left_Powerup = false;
      }
  if(analogRead(TopIntersect_Sensor)>Light_Threshold2 && analogRead(RightIntersect_Sensor)>Light_Threshold2 && !timer_running(Top_Right_Powerup) && !timer_running(Powerup_Timer)){
    timer_start(Top_Right_Timer, 2000);
    Top_Right_Powerup = true;
    } 
      else if(!timer_running(Top_Right_Powerup) && !timer_running(Powerup_Timer)){
        Top_Right_Powerup = false;
    }

  if(analogRead(BottomIntersect_Sensor)>Light_Threshold2 && analogRead(LeftIntersect_Sensor)>Light_Threshold2 && !timer_running(Bottom_Left_Powerup) && !timer_running(Powerup_Timer)){
    timer_start(Bottom_Left_Timer, 2000);
    Bottom_Left_Powerup = true;
    } 
      else if(!timer_running(Bottom_Left_Powerup) && !timer_running(Powerup_Timer)){
        Bottom_Left_Powerup = false;
    } 
  if(analogRead(BottomIntersect_Sensor)>Light_Threshold2 && analogRead(RightIntersect_Sensor)>Light_Threshold2 && !timer_running(Bottom_Right_Powerup) && !timer_running(Powerup_Timer)){
    timer_start(Bottom_Right_Timer, 2000);
    Bottom_Right_Powerup = true;
    } 
      else if(!timer_running(Bottom_Right_Powerup) && !timer_running(Powerup_Timer)){
        Bottom_Right_Powerup = false;
      }
  if(Top_Left_Powerup == true && Top_Right_Powerup == true && Bottom_Left_Powerup == true && Bottom_Right_Powerup == true && !timer_running(Powerup_Timer)){
    timer_start(Powerup_Timer, 5000);
    Powerup_Active = true;
    Serial.println("POWERUP ACTIVE!D");
  }
    else if(!timer_running(Powerup_Timer) && Powerup_Active == true){
      Powerup_Active = false;
      Serial.println("POWERIP DEACTIVATED!");
    }
    */
//following code for scoring
  if (analogRead(Top_Goal1_Sensor) < Light_Threshold && !timer_running(Top_Goal_Timer)){
      timer_start(Top_Goal_Timer,100);
      Serial.println("TOP GOAL!");
      if(Powerup_Active == true){
      Top_Score + 2;
    }
      else{
        Top_Score++;
      }
  }
    if (analogRead(Bottom_Goal1_Sensor) > Light_Threshold && !timer_running(Bottom_Goal_Timer)){
      timer_start(Bottom_Goal_Timer, 100);
      Serial.println("BOTTOM GOAL!");
      if(Powerup_Active == true){
      Bottom_Score + 2;
      }
      else{
        Bottom_Score++;
      }
    }
  
    if (Top_Score >= Win_Score || Bottom_Score >= Win_Score){
      if(Bottom_Score >= Win_Score){
        Serial.println("PLAYER 2 WINS!");
       }
      if(Top_Score >= Win_Score){
        Serial.println("PLAYER 1 WINS!");
       }
      Top_Score=0;
      Bottom_Score=0;
    }
   }
}
