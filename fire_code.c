#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"

/*
Declaring Variables for code
*/

//Global Variables
volatile int temperature_arr[45];
volatile int t_diff[45];
volatile int current_val;



//Sensor dependant variables
volatile int sensor_bits = 0;
volatile int sensor_chc = 0;




//Variables for input data from sensor
volatile unsigned int temp_idx=0;
volatile unsigned int posi_idx=0;

//Humidity values for bechmark
volatile int whole_hum = 0;
volatile int deci_hum = 0;

//Temprerature values for becnhmarks
volatile int whole_temp = 0;
volatile int deci_temp = 0;

//Display Characters for Temperature
char display_whole_t[5];
char display_deci_t[5];

//char message1[] = "TEMp TOO HAWT!!!!";
//char message2[] = "everything seems okay!!!";
char message[60];

//Function for serial output
void ser_output(char *str){
    while(*str != 0){
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}

//Initialize system
void main(void)
{
    //Variables for Serial Display
     char temp[] = "Temp: ";
     char period[] = ".";
     char degrees[] = "C  ";
     char skip_l[] = " \r\n";

    //Set up timers & extra
     WDTCTL = WDTPW | WDTHOLD;

     BCSCTL1= CALBC1_1MHZ;
     DCOCTL = CALDCO_1MHZ;

     P1SEL = BIT1|BIT2;
     P1SEL2 = BIT1|BIT2;

     UCA0CTL1 |= UCSWRST+UCSSEL_2;
     UCA0BR0 = 52;
     UCA0BR1 = 0;
     UCA0MCTL = UCBRS_0;
     UCA0CTL1 &= ~UCSWRST;

    //Delay by about 1 or so seconds
     __delay_cycles(1900000);
     P2DIR |= BIT4;
     P2OUT &= ~BIT4;
     __delay_cycles(19000);
     P2OUT |= BIT4;
     __delay_cycles(19);
     P2DIR &= ~BIT4;
     P2SEL |= BIT4;


     TA1CTL = TASSEL_2|MC_2 ;
     TA1CCTL2 = CAP | CCIE | CCIS_0 | CM_2 | SCS ;

    //Enable interrupts
     _enable_interrupts();

     LcdInit(); // initialize LCD display

        //Initialize
        while (1){

            //Once every piece of data is obtained from the sensor for both the temperature and extra
            if (temp_idx >= 40){

                //For loop collects benchmark values for the whole number
                for (posi_idx = 1; posi_idx <= 8; posi_idx++){

                    //sets the current position value
                    current_val = t_diff[posi_idx];

                    if (current_val >= 110)
                        whole_hum |= (0x01 << (8-posi_idx));

                        }


                //For loop collects benchmark values for the decimals
                for (posi_idx = 9; posi_idx <= 16; posi_idx++){
                    //sets the current position value
                    current_val = t_diff[posi_idx];

                    if (current_val >= 110)

                        deci_hum |= (0x01 << (16-posi_idx));}


                //For loop collects benchmark values for the decimals
                for (posi_idx = 17; posi_idx <= 24; posi_idx++){
                    //sets the current position value
                    current_val = t_diff[posi_idx];

                    if (current_val >= 110)

                        whole_temp |= (0x01 << (24-posi_idx));



                }


                //For loop collects benchmark values for floor temperature
                for (posi_idx = 25; posi_idx <= 32; posi_idx++){

                    //sets the current position value
                    current_val = t_diff[posi_idx];

                    if (current_val >= 110)
                        deci_temp |= (0x01 << (32-posi_idx));}

                //For loop collects benchmark values
                for (posi_idx = 33; posi_idx<=40; posi_idx++){
                    //sets the current position value
                    current_val = t_diff[posi_idx];

                    if (t_diff[posi_idx] >= 110)
                        sensor_chc |= (0x01 << (40-posi_idx));}



                /*
                Serial display settings
                */
                ltoa(whole_temp,display_whole_t,10);
                ltoa(deci_temp,display_deci_t,10);

                //If statements dependant on temperature
                if (whole_temp > 45){
                    //LCD Display Message
                    LcdWriteString("IT'S BURNING!!         ");
                    LcdSetPosition(1,1);

                    //Piezo Sound
                    P2DIR |= BIT5;
                    P2OUT ^= BIT5;

                    __delay_cycles(200000);
                    // Serial Output display to the terminal
                    strcpy(message, "FIRE AAAAAAH!!! CALLL THE COPS!!! THE FIREPEOPLE!!! AAAAAAAAAH!!!!");

                    //Display set message
                    ser_output(message);

                    //Display temperature values
                    ser_output(temp);
                    ser_output(display_whole_t);
                    ser_output(period);
                    ser_output(display_deci_t);
                    ser_output(degrees);

                    //New line
                    ser_output(skip_l);
                }else if ((whole_temp > 40) && (whole_hum < 20)){
                    //LCD Display Message
                    LcdWriteString("SEEK HELP!!!!        ");
                    LcdSetPosition(1,1);

                    //Piezo Sound
                    P2DIR |= BIT5;
                    P2OUT ^= BIT5;

                    __delay_cycles(200000);

                    // Serial Output display to the terminal
                    strcpy(message, "IT IS TOO DRY AND HOT THE PLACE MIGHT BURN DOWN SEEK HELP !!!! AAAAAAH");

                    //Display set message
                    ser_output(message);

                    //Display temperature values
                    ser_output(temp);
                    ser_output(display_whole_t);
                    ser_output(period);
                    ser_output(display_deci_t);
                    ser_output(degrees);

                    //new line
                    ser_output(skip_l);
                }else if ((whole_temp == 21) ){
                   // LCD Display Message
                    LcdWriteString("Beautiful Weather            ");
                    LcdSetPosition(1,1);

                    __delay_cycles(200000);

                    strcpy(message, "What a lovely day outside!!! GO TOUCH GRASS!!!");
                    //Display set message
                    ser_output(message);

                    //Display temperature values
                    ser_output(temp);
                    ser_output(display_whole_t);
                    ser_output(period);
                    ser_output(display_deci_t);
                    ser_output(degrees);

                    //New Line
                    //ser_output(skip_l);
                }else if ((whole_temp < 5) ){

                   // LCD Display Message
                    LcdWriteString("Almost freezing!            ");
                    LcdSetPosition(1,1);

                    __delay_cycles(200000);

                   strcpy(message, "BRRRR. It's cold outside! Bring a Sweater");
                   ser_output(message);

                   ser_output(temp);
                   ser_output(display_whole_t);
                   ser_output(period);
                   ser_output(display_deci_t);
                   ser_output(degrees);
                   ser_output(skip_l);

                    }else {

                   // LCD Display Message
                    LcdWriteString("The temp is ok ;)          ");
                    LcdSetPosition(1,1);
                    __delay_cycles(200000);


                    //Serial Output display to the terminal
                    strcpy(message, "Temperature outside is at normal levels! No need to panic. ");

                    //Display set message
                    ser_output(message);

                    //New line
                    ser_output(skip_l);
                    }

                //Delay service to abt 1 second
                __delay_cycles(1000000);

                WDTCTL = WDT_MRST_0_064;
                }

        }
}

//interrupt service routine
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A1(void){
    //When interrupt add temp values to dummy array
        temperature_arr[temp_idx] = TA1CCR2;

        //increment temp_idx
        temp_idx += 1;

        //Clear interrupt
        TA1CCTL2 &= ~CCIFG ;

        if (temp_idx>=2){
            t_diff[temp_idx-1] = temperature_arr[temp_idx-1] - temperature_arr[temp_idx-2];
            }
}
