/*
Sections:
1- connections
2- usage
3- display mappings


1. Connections

1   234567  +
.   ......  :
            -
            
1- ONBIT (OBREG)
2- DATA0 (SDREG)
3- DATA1 (SDREG)
4- DATA2 (SDREG
5- DATA3 (SDREG)
6- CLOCK (CRREG)
7- RESET (CRREG)
11, 12, 13 -> MP3 shield
3 -> IR transmitter
A5 -> IR reciever
A2 -> relay trigger


Uno: 
ONBIT -> 7
DATA0 -> 6
DATA1 -> 5
DATA2 -> 4
DATA3 -> 2
CLOCK -> 10
RESET -> 8
IR_TX -> 3 //can't be changed
IR_RX -> A5

Mega:
ONBIT->30
DATA0->31
DATA1->32
DATA2->33
DATA3->34
CLOCK->35
RESET->36
IR_TX->9 //can't be changed
IR_RX->37

*/
