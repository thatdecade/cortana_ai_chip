/*

Cortana Arduino Code for the ATTiny45 by Dustin Westaby

Version History:
 2/13/11 Initial Draft and test circuit
 2/14/11 Updated Comments, re-arranged structure
 9/17/11 Timer Off Code Added
 9/27/12 converted to arduino, expanded animations (Ryuuzaki Julio)
11/29/12 cleaned up and rewrote logic

Ouputs:
 pin    label   connections
  2     PB3 =   LED
  3     PB4 =   LED
  5     PB0 =   LED
  6     PB1 =   LED
  7     PB2 =   LED
*/

#include <util/delay.h>

//arduino specific digital pin numbers
int myPins[6] = {2,3,4,1,0};

//performs subtraction then ensures positive result
#define ABS_SUB(a, b) ((a) < (b)? ((b) - (a)): ((a) - (b)))

/* -------------------- */
/* Helper Functions     */
/* -------------------- */

// the setup routine runs once when you press reset:
void setup()
{
   int i;

   // initialize the digital pin as an output.
   for (i=0;i<=4;i++)
   {
      pinMode(myPins[i], OUTPUT);
   }

}

void delay_ms(uint16_t ms)
{
   while ( ms )
   {
      _delay_ms(1);
      ms--;
   }
}

void delay_us(uint16_t us)
{
   while ( us )
   {
      _delay_us(1);
      us--;
   }
}

/* -------------------- */
/* Animation Functions  */
/* -------------------- */

void blinkBlink(int delayTime, int MaxFlickr)
{
   int i, RandomFlickrAmmount;

   //randomize number of blinks and delay to help blinking look more alive
   RandomFlickrAmmount = random(1,MaxFlickr);

   for (i = 0; i <= RandomFlickrAmmount; i++)
   {
      //Turn ON all LEDs 01234
      PORTB = 0b00011111;
      delay(delayTime - RandomFlickrAmmount);

      //Turn OFF all LEDs 01234
      PORTB = 0b00000000;
      delay(delayTime - RandomFlickrAmmount);
   }

} //end blink function

void spinSpin(int count_delay)
{
   int i;
   int repeat           = random(1,3)*5;    //random number of spins times 5 LEDs
   int circle_direction = random(0,1);      //random selection of clockwise or counterclockwise animations
   int pin_to_on  = random(0,4);            //random starting LED position for animation (on)
   int pin_to_off = random(0,4);            //random starting LED position for animation (off)

   //loop ends after random number of spins
   for (i = 0; i <= repeat; i++)
   {
      if (circle_direction == 1)
      {
         //each animation section turns off one LED and turns on one LED
         digitalWrite(myPins[pin_to_on++], HIGH);
         digitalWrite(myPins[pin_to_off++], LOW);
      }
      else
      {
         //each animation section turns off one LED and turns on one LED
         digitalWrite(myPins[pin_to_on--], HIGH);
         digitalWrite(myPins[pin_to_off--], LOW);
      }

      //delay before next spin animation
      delay(count_delay);

      //the following overrun check works because circle_direction can only be 0 or 1.
      //When circle_direction is 0, the pin_to_on pin_to_off are decremented, down to 0.  The overrun check compares to 0, then sets to 4.
      //When circle_direction is 1, the pin_to_on pin_to_off are incremented, up to 4.  The overrun check compares to 4, then sets 0.
      if (pin_to_on == circle_direction*4)
      {
         pin_to_on = ABS_SUB(circle_direction*4,4);
      }
      if (pin_to_off == circle_direction*4)
      {
         pin_to_off = ABS_SUB(circle_direction*4,4);

         //one rotation complete
         //speed up the spinning, down to 16
         if (count_delay > 16)
         {
            count_delay = count_delay - 2;
         }
      }

   } //end loop

} //end spin function

/* -------------------- */
/* Main Function        */
/* -------------------- */

void loop()
{
   int i = 0;
   int count_delay, e, repeat;
   int time_on, time_off, max_value, min_value, rate_of_change;
   boolean fade_direction;
   int Chances_of_Flickr;
   int Chances_of_Spin;
   int X_Loops;
   int randomInt;

   /* Set output pins */
   DDRB = 0b00011111;

   /* ---------------------------------------------------------------- */
   /* All OFF Short */
   /* ---------------------------------------------------------------- */

   count_delay = 20; //Delay starting point
   for (i = 0; i <= 4; i++)
   {
      digitalWrite(myPins[i], LOW);  //turn off each of the 4 LEDs in sequence
      delay(count_delay*2 + i*20);   //delay increases after each iteration
   }

   /* ---------------------------------------------------------------- */
   /* Spin Circle (single runner) */
   /* ---------------------------------------------------------------- */

   repeat = 8;       //Number of spins
   count_delay = 60; //Delay between circle movement in ms

   for(e = 0; e <= repeat; e++)
   {
      for (i = 0; i <= 4; i++)
      {
         digitalWrite(myPins[i], HIGH);
         delay(count_delay);
         digitalWrite(myPins[i], LOW);
      }

      if (count_delay >= 6)
      {
         count_delay = count_delay - 6;
      }
   }

   /* ---------------------------------------------------------------- */
   /* Spin Circle (chased runner) */
   /* ---------------------------------------------------------------- */

   spinSpin(16);  //spin with constant speed of 16ms

   /* ---------------------------------------------------------------- */
   /* All ON Long */
   /* ---------------------------------------------------------------- */

   count_delay = 20; //Delay starting point

   for (i = 0; i <= 4; i++)
   {
      digitalWrite(myPins[i], HIGH);  //turn on each of the 4 LEDs in sequence
      delay(count_delay*2 + i*20);    //delay increases after each iteration
   }
   delay(200);

   /* ---------------------------------------------------------------- */
   /* Blink Blink */
   /* ---------------------------------------------------------------- */

   blinkBlink(50,5);
   delay_ms(100);
   blinkBlink(50,2);
   delay(200);

   /* ---------------------------------------------------------------- */
   /* Fade In and Out Continuous (Software PWM) */
   /* ---------------------------------------------------------------- */

   max_value = 400;           //Max for LEDs ON in us
   min_value = 3;             //Max for LEDs OFF in us
   time_on  = min_value;      //Set Starting Time ON to min
   time_off = max_value;      //Set Starting Time OFF to max
   rate_of_change = 1;        //This is the speed that the fade goes between min and max
   fade_direction = false;    //Direction is defined for the direction of the fade (in or out)
   Chances_of_Flickr = 2;     //The breathing animation has 2 in X_Loops chances of flickering.
   Chances_of_Spin   = 1;     //The breathing animation has 1 in X_Loops chances of Spining.
   X_Loops = 3000;

   while(true) // Repeat Forever
   {

      //Turn ON all LEDs 01234
      PORTB = 0b00011111;
      delay_us(time_on);

      //Turn OFF all LEDs 01234
      PORTB = 0b00000000;
      delay_us(time_off);

      if (fade_direction==true)
      {
         //In the UP direction the Time ON increases while the Time OFF decreases.
         //The result is that the LEDs get brighter
         time_on  += rate_of_change;
         time_off -= rate_of_change;

         //When the end of the fade is reached, switch directions
         if (time_on>max_value)
         {
            fade_direction = false;
         }
      }
      else
      {
         //In the DOWN direction the Time ON decreases while the Time OFF increases.
         //The result is that the LEDs get dimmer
         time_on  -= rate_of_change;
         time_off += rate_of_change;

         //When the end of the fade is reached, switch directions
         if (time_on <= min_value)
         {
            fade_direction=true;
         }
      }

      //Throughout the sequence, random chance of blinking or spinning instead of normal fading
      randomInt= random(1, X_Loops);
      if (randomInt <= Chances_of_Flickr)
      {
         blinkBlink(15, 4);
      }

      if (randomInt >= X_Loops-Chances_of_Spin)
      {
         spinSpin(30);  //spin with increasing speed, from 20 to 16ms
      }

   } //end inf loop

} // end main loop
