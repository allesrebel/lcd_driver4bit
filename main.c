/*
 * main.c - Good or Evil!
 * A demo application using the 4 bit lcd driver and
 * button interrupts. A simple personality test game
 *
 * Made for Project 1 of microprocessor based design class
 *
 *  Created on: May 1, 2014
 *     Authors: Rebel, Manrique
 */

#include <msp430.h>
#include "lcd_driver4bit.h"

/*
 *Pinout:

 P1.7 -15 ---14-DB7
 P1.6 -14 ---3 -DB6
 P1.5 -7 ---12 -DB5
 P1.4 -6 ---11 -DB4

 P2.2 -10---6 -E
 P2.1 -9 ---5 -R/W
 P2.0 -8 ---4 -RS
 */

/*
 * Game Functions/Variables
 */
void askQuestion(char* question, char* options);
int answerFlag = 0;
int goodCount = 0;
int menuFlag = 1;

//Custom Characters
void make_line();

/*
 * Main
 */

int i = 0;
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Set clock to 16MHz
	BCSCTL1 = CALBC1_16MHZ;
	// Set range
	DCOCTL = CALDCO_16MHZ;
	// Set DCO step + modulation

	//Initialize Registers as outputs
	P1DIR &= ~(BIT3 + BIT2);

	// Interrupt Button Push
	P1REN |= BIT3 + BIT2;		// pull up resisters on port 3 and 2
	P1IE |= BIT3 + BIT2;		// interrupts enabled on port 3 and 2
	P1IES |= BIT3 + BIT2;		// interrupt triggered on hi->lo edge transistion
	P1IFG &= ~(BIT3 + BIT2);	// ensure that the interrupts are cleared

	lcd_initialize();

	_enable_interrupts();

	//Game Loop
	while (1) {
		while (menuFlag == 1) {
			//title screen
			askQuestion("Ready to play...", "good or evil?");
			//reset count if answer was good
			goodCount = 0;
		}
		askQuestion("Found a wallet", "Return or keep?");
		askQuestion("Starving kids", "Feed or Ignore?");
		askQuestion("Music source", "Buy or download?");
		askQuestion("See Poop", "clean or Ignore?");
		askQuestion("Pick school", "CP or UCSB?");
		askQuestion("Pick major", "EE or Polsci?");
		askQuestion("Favorite food", "Pizza or other?");
		askQuestion("MCU", "TI or Atmel?");
		askQuestion("Utensil", "Fork or Straw");
		askQuestion("See Red Light", "Stop or Faster?");
		///compare ..
		if (goodCount >= 8) {
			askQuestion("You're good!!", "Play again...?");
			//to reset count if user wants to play again
			goodCount = 0;
		} else {
			askQuestion("You're evil!!", "Play again...?");
			//to reset count if user wants to play again
			goodCount = 0;
		}
	}
}

/*
 * Ask A Question to determine morality
 * 	param question = the question to be asked
 * 	param options = the options displayed
 */
void askQuestion(char* question, char* options) {
	// Set up display, remove any old characters
	lcd_wr_inst(0x01);	// Clear Display
	__delay_cycles(32000); //allows for cleaning of the screen

	lcd_wr_inst(0x80);	// sets cursor to the top left
	__delay_cycles(4000); //allows cursor to move

	lcd_send_string(question);	//write out the question

	lcd_wr_inst(0xC0);	// sets cursor to the top left
	__delay_cycles(4000); //allows cursor to move

	lcd_send_string(options);	//write out the options

	//clear the flag just in case
	answerFlag = 0;

	while (answerFlag != 1) {
		//introduce a delay to aid in de-bouncing
		__delay_cycles(10000000);
	}

	//Prep for the next question
	answerFlag = 0;

}

//Button push interrupt ISR
#pragma vector=PORT1_VECTOR
__interrupt void ISR_PORT1(void) {
	if ((P1IFG & BIT3)== BIT3) {
		//port 3 has a interrupt (our good)
		//Answer was a good one
		goodCount++;
		P1IFG &= ~BIT3;  // P1.3 IFG cleared
		//if last question - clear the menu flag
		menuFlag = 0;
		answerFlag = 1;
	} else if ((P1IFG & BIT2)== BIT2) {
		//port 2 has a interrupt (our bad)
		//answer was a bad one, - so no change in score
		P1IFG &= ~BIT2;  // P1.2 IFG cleared
		//for last question, if user wants to return to menu
		answerFlag = 1;
		menuFlag = 1;
	}
}
