/*
 * File:   main.c
 * Author: Jacob
 *
 * Created on November 28, 2018, 4:34 PM
 */


#include <xc.h>
#include <string.h>
#pragma config OSC=INTIO2
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config BOR=OFF

#define DELAYTIME 5

void cmd(unsigned char);
void data(unsigned char);
void delay(unsigned int);

#define R_SIZE 5  // length of 4 for a string of up inputs, null terminate at 5
#define D_SIZE 16
#define DIS_DELAY 200

// longest morse letter is 4 long
char read_in[R_SIZE] = { '\0' }; // initialize the read in array with null chars
char char_display[D_SIZE] = { '\0' };
char r_index, d_index;
char buttonPressed;

char alpha = 27;
const char *morse[] = { "01", "1000", "1010", "100", "0", "0010", "110", "0000", "00", "0111", "101", "0100", "11", "10", "111", "0110", "1101", "010", "000", "1", "001", "0001", "011", "1001", "1011", "1100", "1111" };

char lookup();
void clear_read();
void clear_display();
char get_input();
char get_press();
void print();

// delay function for 2ms
void delay(unsigned int itime){
    unsigned int i, j;
    for(i = 0; i < itime; i++)
        for(j = 0; j < 255; j++)
            ;
}

void cmd(unsigned char value){
    PORTD = value; // command sent to PORT D
    PORTB = 0x04; // rs0 rw0 en1
    delay(DELAYTIME); // delay 10ms
    PORTB = 0x00;
}

void data(unsigned char value){
    PORTD = value; // data sent to PORTD
    PORTB = 0x05; // rs1 rw0 en1
    delay(DELAYTIME); // delay 10ms
    PORTB = 0x01; // rs1 rw0 en0
}

void main(void) {
    
    TRISD = 0x00;
    ADCON1 = 0x0F;
    TRISB = 0;
    TRISC = 0xFF;
    PORTB = 0; // rs0 rw0 en0
    
    TRISAbits.RA1 = 1; // get button press input
    TRISAbits.RA2 = 0; // LED light up
    
    delay(DELAYTIME);
    cmd(0x0C); // display on cursor off
    delay(DELAYTIME);
    cmd(0x01); // clear display
    delay(DELAYTIME);
    cmd(0x06); // shift cursor right
    delay(DELAYTIME);
    cmd(0x80); // move cursor to start of next line
    delay(DELAYTIME);
    
    r_index = 0;
    d_index = 0;
    
    char input, output;
    
    while(1){
        output = '\0';
        buttonPressed = get_press();
        
        PORTAbits.RA2 = 0;
        delay(DIS_DELAY / 2);
        PORTAbits.RA2 = 1;
        delay(DIS_DELAY / 2);
        
        input = get_input();
        
        // read data
        read_in[r_index++] = input;

        // the user wants to read
        if(r_index == R_SIZE - 1 || buttonPressed)
        {
          // lookup read_in's string in the morse table
          output = lookup();

          // a match was found
          if (output != '\0')
          {
            char_display[d_index++] = output;
            print();

            // reset the read in array to have only nulls (terminate the string)
            clear_read();
          }
        }

        // prevent from reading out of the array
        if (r_index >= R_SIZE){
          print();
          clear_read();
        }    

        if(d_index >= D_SIZE)
          clear_display();
    }
}

void clear_read(){
    char i;
    r_index = 0;
    for (i = 0; i < R_SIZE; i++)
        read_in[i] = '\0';
}

void clear_display(){
    char i;
  
    delay(DELAYTIME);
    cmd(0x01);
    delay(DELAYTIME);
  
    d_index = 0;
    for (i = 0; i < D_SIZE; i++)
        char_display[i] = '\0';
}

// 0: dot
// 1: dash
// return null if no match
// return converted char if match
char lookup()
{
  char i;

  // traverse all the morse code strings
  for (i = 0; i < alpha; i++)
  {    
    // if the strings are the same
    if(!strcmp(morse[i], read_in))
    {
      return 'A' + i;
    }
  }

  // no match was found
  return '\0';
}

char get_input(){

    unsigned char result;
    
    ADCON0 = 0x01;
    ADCON1 = 0x00;
    ADCON2 = 0x08;
    ADCON0bits.GO = 1; // start ADC
    
    while(ADCON0bits.DONE == 1) // wait for conversion
        ;
    
    result = ADRESH;
    
    ADCON1 = 0x0F;
  
  if(result <= 0xFF / 3)
    return '0';
  else
    return '1';
}

char get_press(){

    if(PORTAbits.AN1 == 0)
      return 1;
    else
      return 0;
}

void print()
{
    //char i;
    
    delay(DELAYTIME);
    
    //for (i = 0; i < D_SIZE; i++)
    //    data(char_display[i]);
    data(char_display[d_index - 1]);
    delay(DELAYTIME);
}
