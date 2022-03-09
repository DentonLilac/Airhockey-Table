//Bad timer code by Denton Lilac

//Max number of timers
#define NUM_MAX_TIMERS 64

//Max length of string name for each timer
#define MAX_TIMER_STR_LEN 64

unsigned long Timer_Start_Times[ NUM_MAX_TIMERS ];
unsigned long Timer_End_Times[ NUM_MAX_TIMERS ];

//Max number of timers
//#define NUM_MAX_TIMERS 64

//Max length of string name for each timer
//#define MAX_TIMER_STR_LEN 64

//unsigned long Timer_Start_Times[ NUM_MAX_TIMERS ];
//unsigned long Timer_End_Times[ NUM_MAX_TIMERS ];


//Start a new timer with index i
//Runs from (current time in milliseconds from millis()) to millis() + Run_Time
void timer_start( int i, long Run_Time )
{
  unsigned long CurTime = long( millis() );
  Timer_Start_Times[ i ] = CurTime;
  Timer_End_Times[ i ] = CurTime + long( Run_Time );
  
}


//Runs constantly from calling timer_update( millis() ) in a loop and updates timers back to 0 (default value) if they've passed their run time length.
void timer_update( unsigned long CurTime )
{
  
  for( int i = 0; i < NUM_MAX_TIMERS; i++ )
  {
    
    if( CurTime >= Timer_End_Times[ i ] )
    {
      Timer_Start_Times[ i ] = 0;
      Timer_End_Times[ i ] = 0;
    }
    
  }
  
}

//Call this to check if a timer is currently running, like timer_running(1) to check if timer 1 is running.
bool timer_running( int i )
{
  
  if( Timer_End_Times[ i ] == 0 )
  {
    return false;
  }else{
    return true;
  }
  
}

//Returns the time remaining on a running timer before it ends, or 0 if it's not running.
unsigned long timer_remaining( int i )
{
  //return Timer_End_Times[ i ] - Timer_Start_Times[ i ];
  if( Timer_End_Times[ i ] == 0 )
  {
      return 0;
      
  }else{
    
      return Timer_End_Times[ i ] - millis(); 
  }
  
}

//Resets a timer to it's default 0 state.
void timer_stop( int i )
{
  Timer_Start_Times[ i ] = 0;
  Timer_End_Times[ i ] = 0;
}
