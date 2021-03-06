// Morse.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://upload.wikimedia.org/wikipedia/commons/b/b5/International_Morse_Code.svg

#include "pch.h"
#include <string.h>
#include <iostream>

#define R_SIZE 4
#define D_SIZE 100

using namespace std;

// longest morse letter is 4 long
char read_in[R_SIZE] = { '\0' }; // initialize the read in array with null chars
char display[D_SIZE] = { '\0' };
int r_index, d_index;

const char *morse[] = { "01", "1000", "1010", "100", "0", "0010", "110", "0000", "00", "0111", "101", "0100", "11", "10", "111", "0110", "1101", "010", "000", "1", "001", "0001", "011", "1001", "1011", "1100" };

char lookup();
void print();

int main()
{
	char input, output = '\0';
	int i;
	r_index = 0;
	d_index = 0;

	while (true)
	{
		cin >> input;

		// the user is entering data
		if(input != 'r')
			read_in[r_index++] = input;

		// the user wants to read
		else
		{
			// lookup read_in's string in the morse table
			output = lookup();
			
			// a match was found
			if (output != '\0')
			{
				display[d_index++] = output;
				print();

				// reset the read in array to have only nulls (terminate the string)
				r_index = 0;
				for (i = 0; i < R_SIZE; i++)
					read_in[i] = '\0';
			}
		}

		// prevent from reading out of the array
		if (r_index >= R_SIZE)
			r_index = 0;
	}
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

void print()
{
	int i;
	for (i = 0; i < D_SIZE; i++)
	{
		cout << display[i];
	}
	cout << endl;
}