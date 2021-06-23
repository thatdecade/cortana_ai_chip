// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define NEO_PIN 0
#define LED_PIN 1
#define BUTTON_PIN 2

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(7, NEO_PIN);

#define RED  0xFF0000
#define BLUE 0x0025FF

typedef enum
{
  SPARK_MODE = 0,
  SETUP_SPIN_FADE_RED_TO_BLUE,
  SPINNING_MODE_FADE_RED_TO_BLUE,
  BREATHING_ENERGY,
  INFECTED_SPINNING,
  INFECTED_BREATHING,
  FIRE_SPINNING,
  FIRE_BREATHING,
  GRUNT_BIRTHDAY_FAST,
  GRUNT_BIRTHDAY_SLOW,
  MAX_MODE
};

uint8_t  mode   = SETUP_SPIN_FADE_RED_TO_BLUE; // Startup animation effect
         
void setup() 
{
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(85); // 1/3 brightness

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, HIGH);
  
  randomSeed(analogRead(A0));  // make sure nothing is connected to A0 so that random noise acts as seed
}

void loop() 
{
  uint32_t t;
  static byte r_color = 0;
  static byte g_color = 0;
  static byte b_color = 0;
  
  
  static byte brightness = 85;
  static bool dim_direction = false;
  
  static uint32_t primary_mix = 20;
  static uint32_t secondary_mix = 100;
  static bool mix_direction = false;

  boolean newState = digitalRead(BUTTON_PIN);
  static boolean oldState = HIGH;

  // Check if state changed from high to low (button press).
  if((newState == LOW) && (oldState == HIGH)) 
  {
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW) 
    {
      pixels.clear();
      if(++mode >= MAX_MODE) 
      {
        mode = 0;
      }
    }
  }
  oldState = newState;
  
  //update color mixes
  primary_mix   = get_fade_value(&mix_direction, primary_mix, 1, 20, 100);
  secondary_mix = 101 - primary_mix;
  if(secondary_mix < 1)
  {
    secondary_mix = 1;
  }
  
  //run requested animation
  switch(mode)
  {
    case GRUNT_BIRTHDAY_FAST:
      rainbow();
      delay(2);
      break;
    case GRUNT_BIRTHDAY_SLOW:
      rainbow();
      delay(20);
      break;
    case SETUP_SPIN_FADE_RED_TO_BLUE:
      r_color = 255;
      g_color = 0;
      b_color = 0;
      mode = SPINNING_MODE_FADE_RED_TO_BLUE;
      break;
    case SPINNING_MODE_FADE_RED_TO_BLUE:
      if (b_color < 250)
      {
        r_color -= 10;
        b_color += 10;
      }
      spin_animation(((uint32_t)r_color << 16) + ((uint32_t)g_color >> 8) + b_color);
      break;
      
     case BREATHING_ENERGY:
      brightness = get_fade_value(&dim_direction, brightness, 1, 10, 85);

      pixels.fill(pixels.Color(primary_mix, secondary_mix, 222));
      pixels.setBrightness(brightness);
      pixels.show();
      delay(30);
      break;

     case INFECTED_SPINNING:
      spin_animation(pixels.Color(primary_mix, 222, secondary_mix));
      break;
     case INFECTED_BREATHING:
      brightness = get_fade_value(&dim_direction, brightness, 1, 10, 85);
      pixels.setBrightness(brightness);
      pixels.fill(pixels.Color(primary_mix, 222, secondary_mix));
      pixels.show();
      delay(30);
      break;

     case FIRE_SPINNING:
      spin_animation(pixels.Color(255, primary_mix, 0));
      break;
     case FIRE_BREATHING:
      brightness = get_fade_value(&dim_direction, brightness, 1, 10, 85);
      pixels.setBrightness(brightness);
      pixels.fill(pixels.Color(255, primary_mix, 0));
      pixels.show();
      delay(30);
      break;
      
     case SPARK_MODE: // Random sparks - just one LED on at a time!
      int i = random(32);
      pixels.setPixelColor(i, RED);
      pixels.show();
      delay(10);
      pixels.setPixelColor(i, pixels.Color(primary_mix, secondary_mix, 222));
      break;

  }

}

void rainbow() {
  static uint16_t j = 0;

  for(byte i=0; i<pixels.numPixels(); i++) 
  {
    pixels.setPixelColor(i, Wheel((i*1+j) & 255));
  }
  pixels.show();

  j++;
  if (j > 256)
  {
    j = 0;
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


byte get_fade_value(bool *fade_direction, byte value, byte rate, byte min_value, byte max_value)
{
  if (*fade_direction==true)
  {
     value += rate;
     
     //When the end of the fade is reached, switch directions
     if (value > max_value)
     {
        *fade_direction = false;
     }
  }
  else
  {
     value -= rate;
     
     //When the end of the fade is reached, switch directions
     if (value <= min_value)
     {
        *fade_direction=true;
     }
  }

  return value;
}

void spin_animation(uint32_t color)
{
  static uint8_t offset = 0;

  for(int i=1; i<7; i++) 
  {
    uint32_t c = 0;
    if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
    pixels.setPixelColor(   i, c);
  }
  pixels.show();
  offset++;
  delay(50);
}
