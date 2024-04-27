#include    "msp430.h"
#include    <string.h>

int size_of_buffer = 300; // create an int variable that will be used to buffer

void ser_output(char * gps_data){
  // this function will print a char to the serial
  unsigned int idx=0; // create an unsigned int idx

  while(gps_data[idx]){ // go through the data array, while searching for the end of it

      while ((UCA0STAT & UCBUSY)); // if the UART modules are busy then wait
      UCA0TXBUF = gps_data[idx]; // transmit the ith elemnt of the gps_data through UART

      if (UCA0TXBUF == '\n'){
        break; // if the message has ended then break out of the loop

        }
      idx++; // increment idx by 1
    }
}

void main(){

  /*
  Variable Declaration
  */
  char GPS_read_byte = -1; // this is used to tell which byte it is currently reading
  char raw_string_buffer[size_of_buffer]; // this is used to hold the GPS raw string
  char buffer_for_field[80] = ""; // this will act as a buffer to hold the field in the GPS raw string
  int length_of_field = 0; // this sets the length of the field
  char GPS_command[7] = "$GPGGA"; // this shows the string related to the messages
  int length_of_message = 6; // how long the message is
  int count=0; // this will be used as a counter
  int accuracy = 0; // this will be used to see how accurate the transmission is
  int linecount=0; // this is used to count the lines
  int number_of_interested_fields = 8; // this will be used to keep track of the fields of interest
  int idx_1[number_of_interested_fields]; // this creates the idx variable

  /*
  Initialize Timing stuff
  */
  WDTCTL = WDTPW + WDTHOLD; // stops the watchdog timer

  BCSCTL1 = CALBC1_1MHZ; // sets basic clock system control to 1 MHz
  DCOCTL = CALDCO_1MHZ; // sets DCO clock frequency control to 1 MHz

/*
Sets pins
*/
  P1DIR &= ~BIT1; // sets the P1 direction to bitwise and
  P1OUT =  0; // sets the P1 output to 0
  P1SEL  = BIT1; // sets Port 1 Selection to P1.1
  P1SEL2 = BIT1; // sets Port 1 Selection 2 to P1.1

  UCA0CTL1 |= UCSSEL_2; // sets USCI A0 Control Register 1 to USCI 0 Clock Source 2
  UCA0BR0 = 0xD0; // sets USCI A0 Baud Rate 0
  UCA0BR1 = 0; // sets USCI A0 Baud Rate 1
  UCA0MCTL = UCBRS0; // sets USCI A0 modulation control to USCI second stage modulation select 0
  UCA0CTL1 &= ~UCSWRST; // sets USCI A0 Control Register 1 to bitwise and equals to not USCI software reset

  __bis_SR_register(GIE); // enables interrupts

  memset(raw_string_buffer, 0, size_of_buffer); // sets the number of bytes of the raw_string buffer equal to the size_of_buffer to 0

  while(1){

    if ((IFG2 & UCA0RXIFG)){ // this will check to see if a new byte has been received from the GPS module

      GPS_read_byte = UCA0RXBUF; // sets GPS_read_byte equal to USCI A0 Receiver Buffer
      raw_string_buffer[linecount] = GPS_read_byte;   // this will place the data that has been read into the buffer
      linecount++; // increments the line count
      IFG2 &= ~UCA0RXIFG; // this will tell the MSP430 that the byte has been read
      if (GPS_read_byte == '\n'){ // once the transmission has finished, then it will be to parse through the message

        count = 0; // sets the counter equal to 0
        accuracy = 0; // sets the accuracy to 0
        volatile unsigned int idx = 0; // initializes the idx variable

        while (idx < length_of_message){ // this will check to see if the received message starts with $GPGGA

            if (raw_string_buffer[idx] == GPS_command[idx]){ // if it starts with that
                accuracy++; // increment the accuracy
            }
            idx++; // increment the idx value for the while loop
        }
        if (accuracy == length_of_message){ // if the accuracy is equal to the length of the message
          int idx_2 = 0; // create the int idx_2

          while(raw_string_buffer[idx_2] && idx_2 < size_of_buffer){

            if (raw_string_buffer[idx_2]==',' || raw_string_buffer[idx_2]=='*'){

              idx_1[count]=idx_2; // set it equal to idx_2
              count++; // increment the counter
            }
            idx_2++; // increment the idx_2 variable
          }
          //Output values to the terminal

            ser_output("GPS raw string: ");
            ser_output(raw_string_buffer);
            strncpy(buffer_for_field, raw_string_buffer+idx[idx]+1, length_of_field);
            ser_output(buffer_for_field);
            ser_output("\n");
            memset(buffer_for_field, 0, 80); // sets 80 bytes of buffer_for_field equal to 0
        }
          __delay_cycles(10000000);  // this will have the program delay for 10 seconds before transmitting the next message
      }
        linecount = 0; // sets the linecount equal to 0
    }
  }
}
