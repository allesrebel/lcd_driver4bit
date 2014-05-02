# LCD 4 Bit Driver for MSP430
This library is a simple implmentation to communicate with the widely used HD44780 lcd controller.
(Or anything that supports it)

This particiular implementation should be work for any MSP430 design, but we implemented it on the MSP430G2553 in the project. 

## Written by Alles Rebel and Evan Manrique
Written for a microprocessor based design course at Calpoly San Luis Obispo

### Change Log
  * 5/1/2014 v0.93
  	* Fixed minor comment problems
  	* Removed some unimplemented functions
  * 5/1/2014 v0.9
	* Added ability to use any pin on same port as a db pin
	* minor demo fixes, both buttons work now