#include <FastLED.h>

#define NUM_LEDS 396//480
#define BRIGHTNESS 100 //SET BRIGHTNESS HERE 
#define LED_DATA_PIN 22
#define TOP_LED_DATA_PIN 24
#define BOTTOM_LED_DATA_PIN 25

// START/STOPS TBD
#define TOP_STRIP_LEFT_START 334
#define TOP_STRIP_LEFT_END 353

#define TOP_STRIP_GOAL_START 354
#define TOP_STRIP_GOAL_END 373

#define TOP_STRIP_RIGHT_START 374
#define TOP_STRIP_RIGHT_END 396



#define RIGHT_STRIP_START 0 //0
#define RIGHT_STRIP_END 135 //136

#define BOTTOM_STRIP_RIGHT_START 136
#define BOTTOM_STRIP_RIGHT_END 155

#define BOTTOM_STRIP_GOAL_START 156
#define BOTTOM_STRIP_GOAL_END 175

#define BOTTOM_STRIP_LEFT_START 176
#define BOTTOM_STRIP_LEFT_END 195

#define LEFT_STRIP_START 196
//#define LEFT_STRIP_END 299
#define LEFT_STRIP_END 333

int Bottom_Strip_Goal_Effect_1_Start = BOTTOM_STRIP_GOAL_START + 8 - 100;
int Bottom_Strip_Goal_Effect_1_End = BOTTOM_STRIP_GOAL_START + 8;
int Bottom_Strip_Goal_Effect_2_Start = BOTTOM_STRIP_GOAL_START + 9;
int Bottom_Strip_Goal_Effect_2_End = BOTTOM_STRIP_GOAL_START + 9 + 100;

int Top_Strip_Goal_Effect_1_Start = TOP_STRIP_GOAL_START + 8 - 100;
int Top_Strip_Goal_Effect_1_End = TOP_STRIP_GOAL_START + 8;
int Top_Strip_Goal_Effect_2_Start = TOP_STRIP_GOAL_START + 9;
int Top_Strip_Goal_Effect_2_End = TOP_STRIP_GOAL_START + 9 + 100;


    
CRGB real_leds[NUM_LEDS]; //Real LED output for lights around table walls.
CRGB leds1[NUM_LEDS]; //Main display color for wall LEDs
CRGB leds2[NUM_LEDS]; //Secondary color for wall LEDs
CRGB leds3[NUM_LEDS]; //Color channel for bouncing effect against wall lasers.
CRGB temp_led;

CRGB top_score_leds[7]; //LEDs for displaying top players score
CRGB bottom_score_leds[7]; //LEDs for displaying bottom players score

int GenNum = 0;
bool GenFlip = false;

bool ReverseLEDs = false;


CRGBPalette16  lavaPalette = CRGBPalette16(
  CRGB::DarkRed,  CRGB::Maroon,   CRGB::DarkRed,  CRGB::Maroon,
  CRGB::DarkRed,  CRGB::Maroon,   CRGB::DarkRed,  CRGB::DarkRed,
  CRGB::DarkRed,  CRGB::DarkRed,  CRGB::Red,      CRGB::Orange,
  CRGB::White,    CRGB::Orange,   CRGB::Red,      CRGB::DarkRed
);

uint16_t noiseScale = 15;
uint16_t indexScale = 33;




void led_setup()
{
    //Need to set pins
    //FastLED.addLeds<NEOPIXEL,0 >(leds, NUM_LEDS);
    if( millis() < 1000 ){
    FastLED.addLeds<WS2812B,LED_DATA_PIN,GRB>(real_leds, NUM_LEDS);
    FastLED.addLeds<WS2812B,TOP_LED_DATA_PIN,GRB>(top_score_leds, 7);
    FastLED.addLeds<WS2812B,BOTTOM_LED_DATA_PIN,GRB>(bottom_score_leds, 7);
    FastLED.setBrightness(BRIGHTNESS);
    }
    
    led_reset();
    delay(500);
    led_generate_default();
    
    led_startup_timer();

    for( int i = Top_Strip_Goal_Effect_2_End; i > Top_Strip_Goal_Effect_2_Start; i-- )
    {
      int ij = i % (NUM_LEDS - 1);
      Serial.println( ij );
    }
    
}



void led_generate_default()
{
    for( int i = 0; i < NUM_LEDS; i++ )
    {
        leds1[i] = CRGB( 255, 17 * (sin( float(i) / 9 ) + 1), 122 * ( ( sin(float(i) / 9 ) * -1 ) + 1 ) );
    }
}


void led_sudden_death_effect()
{
    fill_solid( leds1, NUM_LEDS, CRGB( constrain( ( sin( millis() / 80.0 ) + 2 ) * 85, 10, 255 ), 0, 0 ) );
}


void led_generate_rainbow()
{

    int RandSat = random( 180, 255 );
    for( int i = 0; i < NUM_LEDS; i++ )
    {
        leds1[i].setHSV( int( floor( ( i / float( NUM_LEDS ) ) * 255 ) ) * 2, RandSat, 255 ); 
    }
}

void led_generate_new_dual_color_ring()
{ 

    int RandLights = random( NUM_LEDS/3, NUM_LEDS);
    int RandHue = random( 0, 255 );
    int RandSat = random( 160, 255 );
    int RandVar = random( 0, 50 );
    int RandVal1 = random( 150, 255 );
    int RandVal2 = random( 150, 255 );
    CHSV hsv1( RandHue, RandSat, 255 );
    CHSV hsv2( (RandHue + 102 + RandVar) % 255, RandSat, 255 );
    CRGB rgb1;
    CRGB rgb2;
    hsv2rgb_rainbow( hsv1, rgb1 );
    hsv2rgb_rainbow( hsv2, rgb2 );

    
    for( int i = 0; i < NUM_LEDS; i++ )
    {
        //leds1[i] = CRGB( 255, 17 * (sin( float(i) / 9 ) + 1), 122 * ( ( sin(float(i) / 9 ) * -1 ) + 1 ) );
        float consin1 = constrain( ( sin( float(i) / 9.0 ) / 2.0 ) + 0.75, 0.0, 1.0 );
        float consin2 = constrain( ( sin( float(i) / 9.0 ) * -0.5 ) + 0.75, 0.0, 1.0 );

        leds1[i] = CRGB( ( rgb1.r * consin1 ) + ( rgb2.r * consin2 ), ( rgb1.g * consin1 ) + ( rgb2.g * consin2 ), ( rgb1.b * consin1 ) + ( rgb2.b * consin2 ) );
    }
    
}


void led_generate_new_color_strip()
{

    int ColorLength = random( 16, 128 );
    float ColorNum = float(random( 2, 7 ));
    int RandHue = random( 0, 255 );
    int RandSat = random( 160, 255 );
    int RandVar = random( 0, 50 );
    int RandVal1 = random( 150, 255 );
    int RandVal2 = random( 150, 255 );
    CHSV hsv1( RandHue, RandSat, 255 );
    CHSV hsv2( (RandHue + 102 + RandVar) % 255, RandSat, 255 );
    CRGB rgb1;
    CRGB rgb2;
    hsv2rgb_rainbow( hsv1, rgb1 );
    hsv2rgb_rainbow( hsv2, rgb2 );

    
    if( random(0, 3) == 0 )
    {
        int StartPos1 = ( RIGHT_STRIP_END - ColorLength ) / 2;
        for( int i = StartPos1; i <= StartPos1 + ColorLength; i++ )
        {
    
            leds1[i] = CRGB( rgb1.r, rgb1.g, rgb1.b );
        }

        int StartPos2 = StartPos1 + LEFT_STRIP_START;
        for( int i = StartPos2; i <= ColorLength + StartPos2; i++ )
        {
    
            leds1[i] = CRGB( rgb2.r, rgb2.g, rgb2.b );
        }
        
    }else{

        int StartPos1 = ( RIGHT_STRIP_END - ColorLength ) / 2;
        for( int i = StartPos1; i <= ColorLength + StartPos1; i++ )
        {
            float Sin = ( sin( (( float(i - StartPos1) / float(ColorLength) ) * PI * 2.0 * ColorNum) - (PI/2.0) ) + 1.0 ) / 2.0;
            Serial.println( float(i) / float(ColorLength) );
            leds1[i] = CRGB( rgb1.r * Sin, rgb1.g * Sin, rgb1.b * Sin );
        }

        int StartPos2 = StartPos1 + LEFT_STRIP_START;
        for( int i = StartPos2; i <= ColorLength + StartPos2; i++ )
        {
            float Sin = ( sin( (( float(i-StartPos2) / float(ColorLength) ) * PI * 2.0 *ColorNum) - ( PI / 2.0 ) ) + 1.0) / 2.0;
            leds1[i] = CRGB( rgb2.r * Sin, rgb2.g * Sin, rgb2.b * Sin );
        }
    }
}


void led_generate_new_color_glows()
{
    int RandLights = random( NUM_LEDS/3, NUM_LEDS);
    int RandHue = random( 0, 255 );
    int RandSat = random( 160, 255 );
    int RandVar = random( 0, 50 );
    int RandVal1 = random( 150, 255 );
    int RandVal2 = random( 150, 255 );
    CHSV hsv1( RandHue, RandSat, 255 );
    CHSV hsv2( (RandHue + 102 + RandVar) % 255, RandSat, 255 );
    CRGB rgb1;
    CRGB rgb2;
    hsv2rgb_rainbow( hsv1, rgb1 );
    hsv2rgb_rainbow( hsv2, rgb2 );

    
    for( int i = 0; i < NUM_LEDS; i++ )
    {
        //leds1[i] = CRGB( 255, 17 * (sin( float(i) / 9 ) + 1), 122 * ( ( sin(float(i) / 9 ) * -1 ) + 1 ) );
        float consin1 = constrain( ( sin( float(i) / 9.0 ) ), 0.0, 1.0 );
        //float consin2 = constrain( ( sin( float(i) / 9.0 ) * -0.5 ) + 0.75, 0.0, 1.0 );

        if( consin1 < 0.66 ){
            leds1[i] = CRGB( ( rgb1.r * consin1 ), ( rgb1.g * consin1 ), ( rgb1.b * consin1 ));
        }else{
            leds1[i] = CRGB( ( rgb2.r * consin1 ), ( rgb2.g * consin1 ), ( rgb2.b * consin1 ) );
        }
    }
}


void led_generate_solid_color()
{

    int RandHue = random( 0, 255 );
    int RandSat = random( 160, 255 );
    int RandVal = random( 127, 255 );
    int RandVar = random( 0, 50 );
    CHSV hsv1( RandHue, RandSat, RandVal );
    CHSV hsv2( (RandHue + 102 + RandVar) % 255, RandSat, RandVal );
    CRGB rgb1;
    CRGB rgb2;
    
    hsv2rgb_rainbow( hsv1, rgb1 );
    hsv2rgb_rainbow( hsv2, rgb2 );

    Serial.println( "New Color Generated!");
    Serial.println( RandHue );
    Serial.println( RandSat );
    Serial.println( RandVal );
    Serial.println( RandVar );
    Serial.print("Oppo: ");
    Serial.println( (RandHue + 102 + RandVar) % 255 );
    
    //for( int i = 0; i < LEFT_STRIP_START; i++ )
    for( int i = RIGHT_STRIP_END / 2; i < (RIGHT_STRIP_END/2) + LEFT_STRIP_START + 1; i++ )
    {
        //leds1[i] = CRGB( 255, 17 * (sin( float(i) / 9 ) + 1), 122 * ( ( sin(float(i) / 9 ) * -1 ) + 1 ) );
        //float consin1 = constrain( ( sin( float(i) / 9.0 ) ), 0.0, 1.0 );
        //float consin2 = constrain( ( sin( float(i) / 9.0 ) * -0.5 ) + 0.75, 0.0, 1.0 );

        //if( consin1 < 0.66 ){
            leds1[i] = CRGB( rgb1.r , rgb1.g, rgb1.b );
        //}else{
            //leds1[i] = CRGB( ( rgb2.r * consin1 ), ( rgb2.g * consin1 ), ( rgb2.b * consin1 ) );
        //}
    }

    for( int i = (RIGHT_STRIP_END/2) + LEFT_STRIP_START + 1; i < LEFT_STRIP_START + (RIGHT_STRIP_END/2) + LEFT_STRIP_START + 5; i++ )
    {
        int j = i % NUM_LEDS;
        leds1[j] = CRGB( rgb2.r, rgb2.g, rgb2.b );
    }
}


void led_generate_new_colors()
{

  //Serial.println( "Generating new color!");
  led_clear_main();
  //fill_solid(leds1, NUM_LEDS, CRGB(0,0,0));
  
  int randnum = random( 0, 6 );
  
  if( randnum == 0 ){
    
    led_generate_new_dual_color_ring();
    
  }else if( randnum == 1 ){
    
    led_generate_new_color_strip();
    
  }else if( randnum == 2 ){
    
    led_generate_new_color_glows();
    
  }else if( randnum == 3 ){
    
    led_generate_solid_color();
    
  }else if( randnum >= 4 ){
    
    led_generate_rainbow();
    
  }
  
}

//Clear the main leds1 buffer for wall LEDs.
void led_clear_main()
{
    fill_solid(leds1, NUM_LEDS, CRGB(0,0,0));
}


void led_startup_timer()
{
  led_reset();
  timer_start( LED_Startup_Timer, LED_Startup_Speed );
}

void led_shutdown_timer()
{
  //led_reset();
  timer_start( LED_Shutdown_Timer, LED_Startup_Speed );
}

void led_reset()
{
    FastLED.setBrightness(BRIGHTNESS);

    //Clear all the LEDs, wait 1 second, then start displaying.
    FastLED.clear();
    FastLED.show();
}

void led_rotate_effect()
{

    static uint8_t hue = 0;

    if( ReverseLEDs == true ){


        temp_led = CRGB( leds1[0].r, leds1[0].g, leds1[0].b );
        for( int i = 0; i < NUM_LEDS - 1; i++ ) {  
            leds1[i] = leds1[i+1];
        }
        leds1[NUM_LEDS-1] = temp_led;
    
        //delay(5);

        if( Top_Score >= 3 or Bottom_Score >= 3 ){
            temp_led = CRGB( leds1[0].r, leds1[0].g, leds1[0].b );
            for( int i = 0; i < NUM_LEDS - 1; i++ ) {  
                leds1[i] = leds1[i+1];
            }
            leds1[NUM_LEDS-1] = temp_led;
        }

        if( Top_Score == 6 or Bottom_Score == 6 ){
            temp_led = CRGB( leds1[0].r, leds1[0].g, leds1[0].b );
            for( int i = 0; i < NUM_LEDS - 1; i++ ) {  
                leds1[i] = leds1[i+1];
            }
            leds1[NUM_LEDS-1] = temp_led;
        }
        
        
    }else{
    
        temp_led = CRGB( leds1[NUM_LEDS-1].r, leds1[NUM_LEDS-1].g, leds1[NUM_LEDS-1].b );
        for(int i = NUM_LEDS; i >= 0; i--) {  
            leds1[i] = leds1[i-1];
        }
        leds1[0] = temp_led;
    
        //delay(5);

        if( Top_Score >= 3 or Bottom_Score >= 3 ){
            temp_led = CRGB( leds1[NUM_LEDS-1].r, leds1[NUM_LEDS-1].g, leds1[NUM_LEDS-1].b );
            for(int i = NUM_LEDS; i >= 0; i--) {  
                leds1[i] = leds1[i-1];
            }
            leds1[0] = temp_led;
        }

        if( Top_Score == 6 or Bottom_Score == 6 ){
            temp_led = CRGB( leds1[NUM_LEDS-1].r, leds1[NUM_LEDS-1].g, leds1[NUM_LEDS-1].b );
            for(int i = NUM_LEDS; i >= 0; i--) {  
                leds1[i] = leds1[i-1];
            }
            leds1[0] = temp_led;
        }
    }

    //real_leds = leds1;
    //memmove( &real_leds[0], &leds1[0], NUM_LEDS * sizeof( CRGB) );

    /*
    //Combine leds1 and leds2
    for(int i = NUM_LEDS; i > 0; i--) {  
        //leds2[i]-=20;
        leds2[i]-= CRGB( 20, 20, 20 );
        real_leds[i] = CRGB( constrain( leds1[i].r + leds2[i].r, 0 , 255 ), constrain( leds1[i].g + leds2[i].g, 0, 255 ), constrain( leds1[i].b + leds2[i].b, 0, 255 ) );
        //leds1[i] = leds1[i-1];
        //CHSV TempCol = rgb2hsv_approximate( leds[i] );
        //leds[i] = CHSV( TempCol.hue, TempCol.sat, Brightness_Mod * 255.0 );
    }*/

    /*
    //Temp setting goal colors to solid
    for( int i = BOTTOM_STRIP_GOAL_START; i <= BOTTOM_STRIP_GOAL_END; i++ )
    {
      //real_leds[i] = CRGB( 255, 122 * (sin( millis() / 300.0 ) + 1), 122 * (sin( millis() / 300.0 ) + 1) );// CRGB::White;
      real_leds[i] = CRGB( 122 * (sin( millis() / 300.0 ) + 1), 255, 122 * (sin( millis() / 300.0 ) + 1) );// CRGB::White;
    }

    for( int i = TOP_STRIP_GOAL_START; i <= TOP_STRIP_GOAL_END; i++ )
    {
      real_leds[i] = CRGB( 122 * (sin( millis() / 300.0 ) + 1), 122 * (sin( millis() / 300.0 ) + 1), 255 );// CRGB::White;
    }*/

    //FastLED.show();
    //update_all_leds();
}


void led_strobe_effect()
{
    for( int i = 0; i < NUM_LEDS; i++ )
    {
        real_leds[i] = CRGB( random(255), 0,  random(255) );
    }

    delay( 10 );
    FastLED.show();
    
}


void led_lava_effect()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t noise = inoise8(i * noiseScale, millis() / 5);
    uint8_t index = inoise8(i * indexScale, millis() /10);
    leds1[i] = ColorFromPalette(lavaPalette, index, noise);
  }
  //FastLED.show();
  update_all_leds();
}


void led_purple_effect()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t noise = inoise8(i * noiseScale, millis() / 4);
    uint8_t noise2 = inoise8( ( i + 1337 ) * noiseScale, millis() / 2);
    uint8_t scale = constrain( noise - 70, 0, 255 );
    //float scale2 = float( noise2 ) / 768.0; //constrain( noise2 - 50, 0, 255 );
    uint8_t scale2 = constrain( scale2 - 100, 0, 255 );
    real_leds[i] = CRGB( scale, scale2, scale );
  }
  FastLED.show();
}

void new_purple_effect()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t noise = inoise8(i * ( noiseScale * 2 ), millis() / 5);
    uint8_t noise2 = inoise8( ( i + 1337 ) * noiseScale, millis() / 3);
    uint8_t noise3 = inoise8( ( i + 6969 ) * ( noiseScale * 0.75 ), millis() / 8);
    uint8_t scale = constrain( noise - 90, 0, 255 );
    //float scale2 = float( noise2 ) / 768.0; //constrain( noise2 - 50, 0, 255 );
    uint8_t scale2 = constrain( noise2 - 130, 0, 255 );
    uint8_t scale3 = constrain( noise3 - 90, 0, 255 );
    leds1[i] = CRGB( scale, scale2 / 2.0, scale3 );
  }

  //Temp setting goal colors to solid
    for( int i = BOTTOM_STRIP_GOAL_START; i <= BOTTOM_STRIP_GOAL_END; i++ )
    {
      //real_leds[i] = CRGB( 255, 122 * (sin( millis() / 300.0 ) + 1), 122 * (sin( millis() / 300.0 ) + 1) );// CRGB::White;
      real_leds[i] = CRGB( 122 * (sin( millis() / 300.0 ) + 1), 255, 122 * (sin( millis() / 300.0 ) + 1) );// CRGB::White;
    }

    for( int i = TOP_STRIP_GOAL_START; i <= TOP_STRIP_GOAL_END; i++ )
    {
      real_leds[i] = CRGB( 122 * (sin( millis() / 300.0 ) + 1), 122 * (sin( millis() / 300.0 ) + 1), 255 );// CRGB::White;
    }
    
  //FastLED.show();
  update_all_leds();
}


void update_all_leds()
{

  
  //Combine leds1 and leds2
    for(int i = NUM_LEDS - 1; i >= 0; i--) {  
        //leds2[i]-=20;
        leds3[i]-= CRGB( 20, 20, 20 );
        leds2[i]-= CRGB( 3, 3, 3 );
        real_leds[i] = CRGB( constrain( leds1[i].r + leds2[i].r + leds3[i].r, 0 , 255 ), constrain( leds1[i].g + leds2[i].g + leds3[i].g, 0, 255 ), constrain( leds1[i].b + leds2[i].b + leds3[i].b, 0, 255 ) );
        //leds1[i] = leds1[i-1];
        //CHSV TempCol = rgb2hsv_approximate( leds[i] );
        //leds[i] = CHSV( TempCol.hue, TempCol.sat, Brightness_Mod * 255.0 );
    }

    
    for( int i = 0; i < 7; i++ ){
      bottom_score_leds[i] = CRGB(0,0,0);
      if( Top_Score - 1 >= i ){
        if( timer_running( Top_Goal_Timer ) && Top_Score - 1 == i ){
            bottom_score_leds[i] = CRGB( 0, ( sin( millis() / 60 ) + 1 ) * 127.5, 0 );
        }else{
            bottom_score_leds[i] = CRGB( 0, 255, 0 );
        }

        if( timer_running( Win_Timer ) )
        {
          if( Top_Win == true ){
              bottom_score_leds[i] = CRGB( ( sin( ( millis() / 60 ) - i ) + 1 ) * 127.5, ( sin( ( millis() / 60 ) - i ) + 1 ) * 127.5, 20 );
          }else{
            //Bottom win
            bottom_score_leds[i] = CRGB( ( sin( millis() / 60 ) + 1 ) * 127.5, 0, 0 );
          }
        }
      }

      top_score_leds[i] = CRGB(0,0,0);
      if( Bottom_Score - 1 >= i ){
        if( timer_running( Bottom_Goal_Timer ) && Bottom_Score - 1 == i ){
            top_score_leds[i] = CRGB( 0, ( sin( millis() / 60 ) + 1 ) * 127.5, 0 );
        }else{
            top_score_leds[i] = CRGB( 0, 255, 0 );
        }

        if( timer_running( Win_Timer ) )
        {
          if( Bottom_Win == true ){
              top_score_leds[i] = CRGB( ( sin( ( millis() / 60 ) - i ) + 1 ) * 127.5, ( sin( ( millis() / 60 ) - i ) + 1 ) * 127.5, 20 );
          }else{
            //Bottom win
              top_score_leds[i] = CRGB( ( sin( millis() / 60 ) + 1 ) * 127.5, 0, 0 );
          }
        }
      }
    }
    FastLED.setBrightness( Brightness_Mod * BRIGHTNESS );
    //FastLED.setBrightness( BRIGHTNESS );
    FastLED.show();
}



//Called to change direction of rotating LEDs around table walls.
void flip_led_direction()
{
  ReverseLEDs = !ReverseLEDs;
}


//Effects for bumping side table sensors.
void bump_top()
{
  Serial.println( "Bump top!" );
  for( int i = TOP_STRIP_RIGHT_START; i <= TOP_STRIP_RIGHT_END; i++ )
  {
    leds3[i] = CRGB( 255, 255, 255 );// CRGB::White;
  }

  for( int i = TOP_STRIP_LEFT_START; i <= TOP_STRIP_LEFT_END; i++ )
  {
    leds3[i] = CRGB( 255, 255, 255 );// CRGB::White;
  }
}

void bump_right()
{
  for( int i = RIGHT_STRIP_START; i <= RIGHT_STRIP_END; i++ )
  {
    leds3[i] = CRGB( 255, 255, 255 );// CRGB::White;
  }
}

void bump_bottom()
{
  for( int i = BOTTOM_STRIP_RIGHT_START; i <= BOTTOM_STRIP_RIGHT_END; i++ )
  {
    leds3[i] = CRGB( 255, 255, 255 );// CRGB::White;
  }

  for( int i = BOTTOM_STRIP_LEFT_START; i <= BOTTOM_STRIP_LEFT_END; i++ )
  {
    leds3[i] = CRGB( 255, 255, 255 );// CRGB::White;
  }
}

void bump_left()
{
  for( int i = LEFT_STRIP_START; i <= LEFT_STRIP_END; i++ )
  {
    leds3[i] = CRGB( 255, 255, 255 );// CRGB::White;
  }

}


void top_score_led_explosion()
{


    if( random( 0, 5 ) == 3 ){
        CHSV hsv1( random( 0, 255 ), random( 150, 255 ), 255 );
        CRGB rgb1;
        hsv2rgb_rainbow( hsv1, rgb1 );
        leds2[ Top_Strip_Goal_Effect_1_End ] = rgb1;
        leds2[ Top_Strip_Goal_Effect_1_End - 1 ] = rgb1;

        leds2[ Top_Strip_Goal_Effect_2_Start ] = rgb1;
        leds2[ Top_Strip_Goal_Effect_2_Start + 1 ] = rgb1;
        
    }else{
        leds2[ Top_Strip_Goal_Effect_1_End ].fadeLightBy( 32 );
        leds2[ Top_Strip_Goal_Effect_2_Start ].fadeLightBy( 32 );
    }
    
    for( int i = Top_Strip_Goal_Effect_1_Start; i < Top_Strip_Goal_Effect_1_End; i++ )
    {
      leds2[i] = leds2[i+1];
      //leds2[i+1].fadeLightBy( 32 );
    }
    leds2[ Bottom_Strip_Goal_Effect_1_Start ] = CRGB( 0, 0, 0 );

    
    /*if( random( 0, 5 ) == 3 ){
        CHSV hsv2( random( 0, 255 ), random( 150, 255 ), 255 );
        CRGB rgb2;
        hsv2rgb_rainbow( hsv2, rgb2 );
        leds2[ Top_Strip_Goal_Effect_2_Start ] = CHSV( random( 0, 255 ), random( 150, 255 ), random( 200, 255 ) );
    }else{
        leds2[ Top_Strip_Goal_Effect_2_Start ].fadeLightBy( 32 );
    }*/
    
    for( int i = Top_Strip_Goal_Effect_2_End; i > Top_Strip_Goal_Effect_2_Start; i-- )
    {
      int ij = i % (NUM_LEDS);
      if( ij == 0 ){
        leds2[ij] = leds2[NUM_LEDS - 1];
      }else{
        leds2[ij] = leds2[ij-1];
      }
      //leds2[i-1].fadeLightBy( 32 );
    }
    leds2[ Top_Strip_Goal_Effect_2_End ] = CRGB( 0, 0, 0 );
}

void bottom_score_led_explosion()
{
    /*
    int Bottom_Strip_Goal_Effect_1_Start = BOTTOM_STRIP_GOAL_START + 8 - 100;
    int Bottom_Strip_Goal_Effect_1_End = BOTTOM_STRIP_GOAL_START + 8;
    int Bottom_Strip_Goal_Effect_2_Start = BOTTOM_STRIP_GOAL_START + 9;
    int Bottom_Strip_Goal_Effect_2_End = BOTTOM_STRIP_GOAL_START + 9 + 100;

    int Top_Strip_Goal_Effect_1_Start = TOP_STRIP_GOAL_START + 8 - 100;
    int Top_Strip_Goal_Effect_1_End = TOP_STRIP_GOAL_START + 8;
    int Top_Strip_Goal_Effect_2_Start = TOP_STRIP_GOAL_START + 9;
    int Top_Strip_Goal_Effect_2_End = TOP_STRIP_GOAL_START + 9 + 100;
    */
    

    /*
    for( int i = Bottom_Strip_Goal_Effect_1_Start; i < Bottom_Strip_Goal_Effect_1_End; i++ )
    {
      float Cos = ( ( cos( (millis() / 50.0) + (float(i)/4) ) + 1.0 ) );
      float DistFromCenter = (100.0 - abs( (Bottom_Strip_Goal_Effect_1_End) - i )) / 100.0;
      float Mul = Cos * 80.0 * DistFromCenter;
      leds2[i] = CRGB( Mul, Mul, 10 );
      //leds1[i] = leds1[i] * ( 1 - DistFromCenter );
    }

    for( int i = Bottom_Strip_Goal_Effect_2_Start; i < Bottom_Strip_Goal_Effect_2_End; i++ )
    {
      float Cos = ( ( cos( (millis() / 50.0) - (float(i)/4) ) + 1.0 ) );
      float DistFromCenter = (100.0 - abs( (Bottom_Strip_Goal_Effect_1_End) - i )) / 100.0;
      float Mul = Cos * 80.0 * DistFromCenter;
      leds2[i] = CRGB( Mul, Mul, 10 );
    }
    */

    if( random( 0, 5 ) == 3 ){
        CHSV hsv1( random( 0, 255 ), random( 150, 255 ), 255 );
        CRGB rgb1;
        hsv2rgb_rainbow( hsv1, rgb1 );
        leds2[ Bottom_Strip_Goal_Effect_1_End ] = rgb1;
        leds2[ Bottom_Strip_Goal_Effect_1_End - 1 ] = rgb1;

        leds2[ Bottom_Strip_Goal_Effect_2_Start ] = rgb1;
        leds2[ Bottom_Strip_Goal_Effect_2_Start + 1 ] = rgb1;
    }else{
        leds2[ Bottom_Strip_Goal_Effect_1_End ].fadeLightBy( 32 );
        leds2[ Bottom_Strip_Goal_Effect_2_Start ].fadeLightBy( 32 );
    }
    
    for( int i = Bottom_Strip_Goal_Effect_1_Start; i < Bottom_Strip_Goal_Effect_1_End; i++ )
    {
      leds2[i] = leds2[i+1];
      //leds2[i+1].fadeLightBy( 32 );
    }
    leds2[ Bottom_Strip_Goal_Effect_1_Start ] = CRGB( 0, 0, 0 );

    /*
    if( random( 0, 5 ) == 3 ){
        CHSV hsv2( random( 0, 255 ), random( 150, 255 ), 255 );
        CRGB rgb2;
        hsv2rgb_rainbow( hsv2, rgb2 );
        leds2[ Bottom_Strip_Goal_Effect_2_Start ] = CHSV( random( 0, 255 ), random( 150, 255 ), random( 200, 255 ) );
    }else{
        leds2[ Bottom_Strip_Goal_Effect_2_Start ].fadeLightBy( 32 );
    }
    */
    
    for( int i = Bottom_Strip_Goal_Effect_2_End; i > Bottom_Strip_Goal_Effect_2_Start; i-- )
    {
      leds2[i] = leds2[i-1];
      //leds2[i-1].fadeLightBy( 32 );
    }
    leds2[ Bottom_Strip_Goal_Effect_2_End ] = CRGB( 0, 0, 0 );
}

void led_loop()
{

    
    if( timer_remaining( LED_Startup_Timer ) > 0 )
    {
        Brightness_Mod = ( LED_Startup_Speed - float( timer_remaining( LED_Startup_Timer ) ) ) / LED_Startup_Speed ;
    }

    if( timer_remaining( LED_Shutdown_Timer ) > 0 )
    {
        Brightness_Mod = ( float( timer_remaining( LED_Shutdown_Timer ) ) / LED_Startup_Speed ) ;
    }

    /*
    if( timer_remaining( LED_Fade_In_Timer ) > 0 )
    {
      if( timer_remaining( LED_Fade_In_Timer ) < 750 )
      {
          Brightness_Mod = ( (LED_Fade_In_Speed - 3000) - float( timer_remaining( LED_Fade_In_Timer ) ) ) / (LED_Fade_In_Speed - 3000);
      }else{
          Brightness_Mod = 0.01;
      }
    }
    */
    
    if( timer_remaining( LED_Shutdown_Timer ) == 0 && timer_remaining( LED_Startup_Timer ) == 0 && GameMode != 0 )
    {
        //Brightness_Mod = 1;
    }

    
    if( GameMode > 0 )
    {

      if( timer_running( Top_Goal_Timer ) == true )
      {
          top_score_led_explosion();
      }

      if( timer_running( Bottom_Goal_Timer ) == true )
      {
          bottom_score_led_explosion();
      }

      if( timer_running( Top_Goal_Timer ) == false && timer_running( Bottom_Goal_Timer ) == false )
      {

          //Sudden death animation when both scores are one point from winning.
          if( Top_Score == 6 && Bottom_Score == 6 )
          {
              led_sudden_death_effect();
              
          }else{
              led_rotate_effect();
          }

      }          
      
      //led_strobe_effect();
      //led_lava_effect();
      //led_purple_effect();
      //new_purple_effect();

      update_all_leds();
      
    }

    
    //Bumping has been disabled for the side lasers because... it's so hard to get them lined up right.
    //So I'm just disabling them for now. If someone in the future wants to realign the lasers up with the sensors on the interior walls.. power to you, brother. Just uncomment the bump functions after you do and they'll light up with bumps again!
    if( analogRead(Top_Sensor) < TopSenAvg - Light_Threshold )
    {
        //bump_top();
    }

    if( analogRead(Right_Sensor) < RightSenAvg - Light_Threshold )
    {
        //bump_right();
    }

    if( analogRead(Bottom_Sensor) < BottomSenAvg - Light_Threshold )
    {
        //bump_bottom();
    }

    if( analogRead(Left_Sensor) < LeftSenAvg - Light_Threshold )
    {
        //bump_left();
    }
}
