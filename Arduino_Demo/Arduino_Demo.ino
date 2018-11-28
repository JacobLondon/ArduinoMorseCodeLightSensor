// Morse.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://upload.wikimedia.org/wikipedia/commons/b/b5/International_Morse_Code.svg

#include <string.h>

#define R_SIZE 4
#define D_SIZE 10
#define A0 0

// longest morse letter is 4 long
char read_in[R_SIZE] = { '\0' }; // initialize the read in array with null chars
char char_display[D_SIZE] = { '\0' };
int r_index, d_index;

const char *morse[] = { "01", "1000", "1010", "100", "0", "0010", "110", "0000", "00", "0111", "101", "0100", "11", "10", "111", "0110", "1101", "010", "000", "1", "001", "0001", "011", "1001", "1011", "1100" };

char lookup();
void s_print();

void setup(){
  Serial.begin(9600);
  r_index = 0;
  d_index = 0;
}

void loop(){

    bool buttonPressed = 0;
    char input, output = '\0';
    int i;
    input = read_input();
    Serial.print(r_index);
    Serial.print(", ");
    Serial.println(input);
    
    //if(r_ind
    //  Serial.println("here");
    //  input = 'r';ex == 3){
    //}
    
    // the user is entering data
    //if(input == '0' || input == '1')
    read_in[r_index++] = input;

    // the user wants to read
    if(r_index == R_SIZE || buttonPressed)
    {
      // lookup read_in's string in the morse table
      output = lookup();
      
      // a match was found
      if (output != '\0')
      {
        char_display[d_index++] = output;
        Serial.println(output);
        s_print();

        // reset the read in array to have only nulls (terminate the string)
        clear_read();
      }
    }

    // prevent from reading out of the array
    if (r_index >= R_SIZE){
      s_print();
      clear_read();
    }    

    if(d_index >= D_SIZE)
      clear_display();

    delay(1000);
}

void clear_read(){
  int i;
  r_index = 0;
  for (i = 0; i < R_SIZE; i++)
    read_in[i] = '\0';
}

void clear_display(){
  int i;
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
  int i;

  // traverse all the morse code strings
  for (i = 0; i < 26; i++)
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

char read_input(){

  float a = analogRead(A0);
  
  if(a <= 512)
    return '0';
  else
    return '1';
}

void s_print()
{
  int i;
  for (i = 0; i < D_SIZE; i++)
  {
    Serial.print(char_display[i]);
  }
  Serial.println();
}
