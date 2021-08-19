#include "msp.h"
#include <stdio.h>
#include <stdlib.h>

void wait_loop(int t);  // function prototype
void wait(int w);

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;


/*INPUT*/

P9 ->DIR &= ~(BIT0 | BIT1 | BIT2 |BIT3);        //  P9 pins set to input, input pins handle the keypad in task 2

/*OUTPUT*/

P8 ->DIR |= BIT2 | BIT3 | BIT4 | BIT5;      // P8 pins set to output

P4 ->DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;      // P4 pins set to output

P1 ->DIR|= BIT0;
P5 ->DIR|= BIT0;
P2 ->DIR|= BIT5;

// lookup table for each number and digit that will be displayed on the 7-segment display
int AF[16];
AF[0]  =  0b11000000; /*0*/
AF[1]  =  0b11111001; /*1*/
AF[2]  =  0b10100100; /*2*/
AF[3]  =  0b10110000; /*3*/
AF[4]  =  0b10011001; /*4*/
AF[5]  =  0b10010010; /*5*/
AF[6]  =  0b10000010; /*6*/
AF[7]  =  0b11111000; /*7*/
AF[8]  =  0b10000000; /*8*/
AF[9]  =  0b10010000; /*9*/
//AF[10] =  0b11111111; /*OPEN*/
//AF[11] =  0b11111111; /*LOCK 0b10000011*/
AF[12] =  0b11000110; /*C*/
AF[13] =  0b10100001; /*D*/
AF[14] =  0b11110111; /*_*/
AF[15] =  0b11000111; /*L*/

int num_arr[4][9]={ {0,10,3, 0,2,0,0,0,1},  //Array that holds the position of each digit
                    {0,11,6, 0,5,0,0,0,4}, // as given by keypad input and then used with
                    {0,12,9, 0,8,0,0,0,7},//  the look up table to display each digit
                    {0,13,15,0,0,0,0,0,14}};

int disp[4]={0,0,0,0};// Array that holds keypad input

int pass[4]={0,0,0,0};

int state = 0;
int key=0;
int key1=0;
int key2=0;
int key3=0;
int posy  = 0;
int posx  = 0;
int posy1 = 0;
int posx1 = 0;
int posy2 = 0;
int posx2 = 0;
int posy3 = 0;
int posx3 = 0;
int num0 = 0;
int num1  = 0;
int num2 = 0;
int num3= 0;
int count;
int row;
int dnum = 0;
int dnum1 = 0;
int dnum2 = 0;
int dnum3 = 0;
int store = 0;
int CNT = 0;

//LOCK BOX variables
int check1 = 0;
int check2 = 0;
int lockbox = 0;
int lbox = 1;
int add = 0;
int s= 0;
int inter0 = 0;
int inter1 = 0;
int inter2 = 0;
int inter3 = 0;
int interrupt1 = 0;
int interrupt2 = 0;
int interrupt3 = 0;
int interrupt4 = 0;
int led =0;
int wrongcount = 0;

while(1)   // Infinite while loop that loops through each digit being displayed while only turning on one at a time
{
P8-> OUT = 0XFF;
P4-> OUT = 0xFF;
P5-> OUT = 0xFF;
P1-> OUT = 0X00;

P8 -> OUT = 0b00011100;  //BIT5 set to LOW
if(wrongcount==5){
    P4-> OUT = 0xFF;
}
if(add == 0){
//P8 -> OUT = 0b00011100;  //BIT5 set to LOW
P4 -> OUT = AF[disp[0]];// Port 4 activated to display input
//P4-> OUT =0XFF;
//P5 -> OUT = 0b11111110;
}
//pass[0] = 0;
if(add == 1){
//P8 -> OUT = 0b00011100;  //BIT5 set to LOW
P4 -> OUT = AF[pass[0]];// Port 4 activated to display input
}


wait_loop(0);// Time wasting loop to make digits legible and flicker-free

key = P9 -> IN & 0b00001111;
    if(key)
    {
        posy = 0;
    }

switch(state)
{

case 0:// Idle

    if (key != 0) //if key pressed
    {
        posx = key;  //x-axis position set to key value
        row = posy; //row is set to y-axis value
        state = 1; // state changes to state 1
        count = 0;// count is set to 0
    }
break;

case 1:// Key-Press-DeBounce

    if((key == posx))//Key press detected, count will increase by 1
    {
        count++;
    }

    if((key != posx))//Key press no longer detected, state will go back to state 0
    {
        state =0;
    }

    if(count>3)// if key press continues to be detected, state will move to 2 and the count will go back to 0
    {
        state = 2;
        count = 0;
    }
break;

case 2:// Process Input

P4-> OUT = 0xFF;//Port 4 is cleared
num0 = num_arr[posy][posx];// num0 will be assigned a digit according to key press

if(num0 < 10)
{
    count = 0;// Count will go back to 0
    state = 3;// State will change to state 3
}


if(num0 == 10)
{
    //lbox = 1;
    state = 0;
    check1 = 1;
    check2 = 1;
    num0 = 0;
    break;
}
break;

case 3:// Key-Release-DeBounce

    if(key == 0)// if key is released, count will increase
    {
        count ++;
    }

    if(key != 0)// if key remains pressed, count will be set to 0
    {
        count = 0;
    }

    if(count>3)// key release is confirmed, state will be set to 0
    {
        state  = 0;
        CNT++;// CNT increases by 1 which allows to cycle through switch(store)
    }
break;
}// End switch
// The next three switch statements run the same way as described in the previous one
//but they change and detect a value at a different bit of port 8


P8 -> OUT = 0b00101100;  //BIT4 set to LOW

if(wrongcount==5){
    P4-> OUT = 0xFF;
}


if(add==0){
//P8 -> OUT = 0b00101100;  //BIT4 set to LOW
P4 -> OUT = AF[disp[1]];// Port 4 activated to display input
}

if(add == 1){
//P8 -> OUT = 0b00101100;  //BIT5 set to LOW
P4 -> OUT = AF[pass[1]];// Port 4 activated to display input
}




wait_loop(0);// Time wasting loop to make digits legible and flicker-free
key1 = P9 -> IN & 0b00001111;// reads key pressed as 8,4,2,1
    if(key1)
    {
        posy1 = 1;// BIT4 position, row 2
    }

switch(state)// Switch statement when P8.4 is activated when set to LOW
{

case 00:// Idle

    if (key1 != 0)
    {
        posx1 = key1;
        row = posy1;
        state = 12;
        count = 0;
    }

break;

case 12:// Key-Press-DeBounce

    if((key1 == posx1))
    {
        count++;
    }

    if((key1 != posx1))
    {
        state =0;
    }

    if(count>3)
    {
        state = 22;
        count = 0;
    }
break;

case 22:// Process Input

P4-> OUT = 0xFF;
num1 = num_arr[posy1][posx1];




    if(num1 < 11)
{
    count = 0;// Count will go back to 0
    state = 32;// State will change to state 3
}

    if(num1 == 11)
{
    state = 00;
    lockbox = 1;
    num1 =0;
    break;
}

break;

case 32:// Key-Release-DeBounce

    if(key1 == 0)
    {
        count ++;
    }

    if(key1 != 0){
        count = 0;
    }

    if(count>3)
    {
        state  = 00;
        CNT++;// CNT increases by 1 which allows to cycle through switch(store)
    }
break;
}   // End switch



P8 -> OUT = 0b00110100;  //BIT3 set to LOW
    if(wrongcount==5){
    P4-> OUT = 0xFF;
}
    if(add==0){
//P8 -> OUT = 0b00110100;  //BIT3 set to LOW
P4 -> OUT = AF[disp[2]];// Port 4 activated to display input
//P4-> OUT =0XFF;
}


if(add==1){
    P8 -> OUT = 0b00110100;  //BIT3 set to LOW
    P4 -> OUT = AF[pass[2]];// Port 4 activated to display input
}


wait_loop(0);// Time wasting loop to make digits legible and flicker-free
key2 = P9 -> IN & 0b00001111;// reads key pressed as 8,4,2,1
    if(key2)
    {
        posy2 = 2;// BIT3 position, row 3
    }

switch(state)// Switch statement when P8.3 is activated when set to LOW
{

case 000:// Idle

    if (key2 != 0)
    {
        posx2 = key2;
        row = posy2;
        state = 13;
        count = 0;
    }
break;

case 13:// Key-Press-DeBounce

    if((key2 == posx2))
    {
        count++;
    }

    if((key2 != posx2))
    {
        state =000;
    }

    if(count>3)
    {
        state = 23;
        count = 0;
    }
break;

case 23:// Process Input

P4-> OUT = 0xFF;
num2 = num_arr[posy2][posx2];

    if(num2 < 12)
{
    count = 0;// Count will go back to 0
    state = 33;// State will change to state 3
}
    if(num2 == 12)
{
    num2 = 0;
    state = 000;
    break;
}

break;

case 33:// Key-Release-DeBounce

    if(key2 == 0)
    {
        count ++;
    }

    if(key2 != 0)
    {
        count = 0;
    }

    if(count>3)
    {
        state  = 000;
        CNT++;// CNT increases by 1 which allows to cycle through switch(store)
    }
break;
}   // End switch

P8 -> OUT = 0b00111000;  //BIT2 set to LOW
if(wrongcount==5){
    P4-> OUT = 0xFF;
}


if(add==0){
//P8 -> OUT = 0b00111000;  //BIT2 set to LOW
P4 -> OUT = AF[disp[3]];// Port 4 activated to display input
//P4-> OUT =0XFF;
}

if(add==1){
//P8 -> OUT = 0b00111000;  //BIT2 set to LOW
P4 -> OUT = AF[pass[3]];// Port 4 activated to display input
}

wait_loop(0);// Time wasting loop to make digits legible and flicker-free
key3 = P9 -> IN & 0b00001111;// reads key pressed as 8,4,2,1
    if(key3)
    {
        posy3 = 3;// BIT2 position, row 4
    }

switch(state)// Switch statement when P8.2 is activated when set to LOW
{

case 0000:// Idle

    if (key3 != 0)
    {
        posx3 = key3;
        row = posy3;
        state = 14;
        count = 0;
    }
break;

case 14:// Key-Press-DeBounce

    if((key3 == posx3))
    {
        count++;
    }

    if((key3 != posx3))
    {
        state =0000;
    }

    if(count>3)
    {
        state = 24;
        count = 0;
    }
break;

case 24:// Process Input

P4-> OUT = 0xFF;
num3 = num_arr[posy3][posx3];

    if(num3 < 13)
{
    count = 0;// Count will go back to 0
    state = 34;// State will change to state 3
}

    if(num3 == 13)
{
    num3 = 0;
    state = 0000;
    break;
}

    if(num3 == 14)
{
    num3 = 0;
state  = 0000;
break;
}

    if(num3 == 15)
{
    num3 =0;
state  = 0000;
break;
}

break;

case 34:// Key-Release-DeBounce

    if(key3 == 0)
    {
        count ++;
    }

    if(key3 != 0)
    {
        count = 0;
    }

    if(count>3)
    {
        state  = 0000;
        CNT++;// CNT increases by 1 which allows to cycle through switch(store)
    }
break;
}   // End switch








if(add==0){
switch(store)  // Switch statement that cycles through the array and saves password
{             //  that holds the output digits

case 0: //disp[0]

if(CNT==1)//When CNT is 1, case 0 checks for a digit to put in disp[0]
{

    if(num0>0){
        dnum = num0;// detected num0 is written into dnum
    }
    if(num1>0){
        dnum = num1;// detected num1 is written into dnum
    }
   if(num2>0){
       dnum = num2;// detected num2 is written into dnum
    }
    if(num3>0){
        dnum = num3;// detected num3 is written into dnum
    }
disp[0] = dnum; // dnum is displayed after being set to key pressed

    store = 1; //Position 0 is filled, move to position 1
    dnum  = 0;// Values cleared for next array position
    num0  = 0;
    num1  = 0;
    num2  = 0;
    num3  = 0;
}// End case 0
break;

case 1://disp[1]

if(CNT==2)//When CNT is 2, case 1 checks for a digit to put in disp[1]
{

    if(num0>0){
        dnum1 = num0;// detected num0 is written into dnum1
    }
    if(num1>0){
        dnum1 = num1;// detected num1 is written into dnum1
    }
    if(num2>0){
        dnum1 = num2;// detected num2 is written into dnum1
    }
    if(num3>0){
        dnum1 = num3;// detected num3 is written into dnum1
    }
disp[1] = dnum1; // dnum1 is displayed after being set to key pressed

    store = 2; // Position 1 is filled, move to position 2
    dnum1 = 0;// Values cleared for next array position
    num0  = 0;
    num1  = 0;
    num2  = 0;
    num3  = 0;
} //End case 1
break;

case 2://disp[2]

if(CNT==3)//When CNT is 3, case 2 checks for a digit to put in disp[2]
{

    if(num0>0){
        dnum2 = num0;// detected num0 is written into dnum2
    }
    if(num1>0){
        dnum2 = num1;// detected num1 is written into dnum2
    }
    if(num2>0){
        dnum2 = num2;// detected num2 is written into dnum2
    }
    if(num3>0){
        dnum2 = num3;// detected num3 is written into dnum2
    }
disp[2] = dnum2; // dnum2 is displayed after being set to key pressed


    store = 3; // Position 2 is filled, move to position 3
    dnum2 = 0;// Values cleared for next array position
    num0  = 0;
    num1  = 0;
    num2  = 0;
    num3  = 0;
}// End case 2
break;

case 3://disp[3]

if(CNT==4)//When CNT is 4, case 3 checks for a digit to put in disp[3]
{

    if(num0>0){
        dnum3 = num0;// detected num0 is written into dnum3
    }
    if(num1>0){
        dnum3 = num1;// detected num1 is written into dnum3
    }
    if(num2>0){
        dnum3 = num2;// detected num2 is written into dnum3
    }
    if(num3>0){
        dnum3 = num3;// detected num3 is written into dnum3
    }
disp[3] = dnum3; // dnum3 is displayed after being set to key pressed


    store = 0; // Array position 3 filled, back to position 0
    dnum3 = 0;// Values cleared for next array position
    num0  = 0;
    num1  = 0;
    num2  = 0;
    num3  = 0;
    lbox = 2;
    CNT   = 0;// Array is full and CNT is set to 0 which allows array position 0 to be written into
    add++;
}// End case 3
break;
}// switch store

}





if(add>0){// Switch statement takes user input after inputting password

    switch(store)  // Switch statement that cycles through the array
    {             //  that holds the output digits

    case 0: //disp[0]

    if(CNT==1)//When CNT is 1, case 0 checks for a digit to put in disp[0]
    {

        if(num0>0){
            dnum = num0;// detected num0 is written into dnum
        }
        if(num1>0){
            dnum = num1;// detected num1 is written into dnum
        }
       if(num2>0){
           dnum = num2;// detected num2 is written into dnum
        }
        if(num3>0){
            dnum = num3;// detected num3 is written into dnum
        }
    pass[0] = dnum; // dnum is displayed after being set to key pressed
        store = 1; //Position 0 is filled, move to position 1
        dnum  = 0;// Values cleared for next array position
        num0  = 0;
        num1  = 0;
        num2  = 0;
        num3  = 0;
    }// End case 0
    break;

    case 1://disp[1]

    if(CNT==2)//When CNT is 2, case 1 checks for a digit to put in disp[1]
    {

        if(num0>0){
            dnum1 = num0;// detected num0 is written into dnum1
        }
        if(num1>0){
            dnum1 = num1;// detected num1 is written into dnum1
        }
        if(num2>0){
            dnum1 = num2;// detected num2 is written into dnum1
        }
        if(num3>0){
            dnum1 = num3;// detected num3 is written into dnum1
        }
    pass[1] = dnum1; // dnum1 is displayed after being set to key pressed

        store = 2; // Position 1 is filled, move to position 2
        dnum1 = 0;// Values cleared for next array position
        num0  = 0;
        num1  = 0;
        num2  = 0;
        num3  = 0;
    } //End case 1
    break;

    case 2://disp[2]

    if(CNT==3)//When CNT is 3, case 2 checks for a digit to put in disp[2]
    {

        if(num0>0){
            dnum2 = num0;// detected num0 is written into dnum2
        }
        if(num1>0){
            dnum2 = num1;// detected num1 is written into dnum2
        }
        if(num2>0){
            dnum2 = num2;// detected num2 is written into dnum2
        }
        if(num3>0){
            dnum2 = num3;// detected num3 is written into dnum2
        }
    pass[2] = dnum2; // dnum2 is displayed after being set to key pressed


        store = 3; // Position 2 is filled, move to position 3
        dnum2 = 0;// Values cleared for next array position
        num0  = 0;
        num1  = 0;
        num2  = 0;
        num3  = 0;
    }// End case 2
    break;

    case 3://disp[3]

    if(CNT==4)//When CNT is 4, case 3 checks for a digit to put in disp[3]
    {

        if(num0>0){
            dnum3 = num0;// detected num0 is written into dnum3
        }
        if(num1>0){
            dnum3 = num1;// detected num1 is written into dnum3
        }
        if(num2>0){
            dnum3 = num2;// detected num2 is written into dnum3
        }
        if(num3>0){
            dnum3 = num3;// detected num3 is written into dnum3
        }
    pass[3] = dnum3; // dnum3 is displayed after being set to key pressed


        store = 0; // Array position 3 filled, back to position 0
        dnum3 = 0;// Values cleared for next array position
        num0  = 0;
        num1  = 0;
        num2  = 0;
        num3  = 0;
        //lbox = 3;
        CNT   = 0;// Array is full and CNT is set to 0 which allows array position 0 to be written into
        //add = 0;
        //wrong=1;
    }// End case 3
    break;
    }// switch store

}


switch(lbox){

case 1:// Normal

    if(check1==1){// If open key pressed

while(s<260){// displays {0,0,0,0} when in normal state

P8 -> OUT = 0b00011100;
P4 -> OUT = AF[0];
wait_loop(0);


P8 -> OUT = 0b00101100;
P4 -> OUT = AF[0];
wait_loop(0);


P8 -> OUT = 0b00110100;
P4 -> OUT = AF[0];
wait_loop(0);


P8 -> OUT = 0b00111000;
P4 -> OUT = AF[0];
wait_loop(0);


P1-> OUT  = 0b00000001;// solenoid led is turned on for ~5 sec
s++;// increase while loop count

}// while loop
s = 0;
lbox = 1;// back to normal state
check1 = 0;// sets open button back to zero
check2 = 0;//sets open button 2 to zero
   }// if loop
break;

case 2:// locking

add=0;

    if(lockbox==1){// if locked button is pressed


while(s<150){// while loop that displays password for ~5 sec while system is locking
led++;// increases led count to make led blink
// code below diaplasys password while checking for an input while the system is locking
P8 -> OUT = 0b00011100;
inter0 = P9 -> IN & 0b00001110;
interrupt1 = inter0;
P4 -> OUT = AF[disp[0]];
wait_loop(0);

P8 -> OUT = 0b00101100;
inter1 = P9 -> IN & 0b00001110;
interrupt2 = inter1;
P4 -> OUT = AF[disp[1]];
wait_loop(0);

P8 -> OUT = 0b00110100;
inter2 = P9 -> IN & 0b00001110;
interrupt3 = inter2;
P4 -> OUT = AF[disp[2]];
wait_loop(0);

P8 -> OUT = 0b00111000;
inter3 = P9 -> IN & 0b00001110;
interrupt4 = inter3;
P4 -> OUT = AF[disp[3]];
wait_loop(0);

s++;// while loop count increases
    if(led<10)
{
P5 -> OUT = 0b11111110;// led light is turned on
}

    if(led==10){
P5 -> OUT = 0b11111111;// led light is turned off

}

    if(led==20)
{
led=0;// led count resets to make light turn on again and blink
}

    if(interrupt1>0||interrupt2>0||interrupt3>0||interrupt4>0)// checks to see if key has been pressed
{
break;
}

}// while loop

s=0;// while loop counter reset
led=0;// led counter reset


if(interrupt1>0||interrupt2>0||interrupt3>0||interrupt4>0)// if key pressed values set to 0 and system returns to normal state
{
lbox = 1;// system back to state 1
inter0 = 0;
inter1 = 0;
inter2 = 0;
inter3 = 0;
interrupt1 = 0;
interrupt2 = 0;
interrupt3 = 0;
interrupt4 = 0;
disp[0] = 0;
disp[1] = 0;
disp[2] = 0;
disp[3] = 0;
lockbox = 0;
check1 =0;
check2 = 0;
break;
}

while(s<30){// _LOC displayed to show that lock box is now locked until correct password is entered
led++;// led counter increases
P8 -> OUT = 0b00011100;
//interupt = P9 -> IN & 0b00001111;
P4 -> OUT = AF[14];
wait(0);


P8 -> OUT = 0b00101100;
//interupt = P9 -> IN & 0b00001111;
P4 -> OUT = AF[15];
wait(0);


P8 -> OUT = 0b00110100;
//interupt = P9 -> IN & 0b00001111;
P4 -> OUT = AF[0];
wait(0);


P8 -> OUT = 0b00111000;
//interupt = P9 -> IN & 0b00001111;
P4 -> OUT = AF[12];
wait(0);

s++; // while loop counter increases
    if(led<10)
{
P5 -> OUT = 0b11111110;// led turns on
}

    if(led==10)
{
P5 -> OUT = 0b11111111;// led turns off
}

    if(led==20)// led count reset to turn on led
{
led=0;// led counter reset
}

}// while loop

lockbox = 0;// lockbutton reset
s=0;// while loop counter reset
led=0;// led counter reset
lbox = 3;// system moves to case 3
check2 = 0;
add=1;

}// if lockbox
break;

case 3:// Locked

    if(check2==1)// if open key is pressed
{
    if(disp[0]==pass[0] && disp[1]==pass[1] && disp[2]==pass[2] && disp[3]==pass[3])// checks to see if entered password matches initial input
{
// when password == initial input
disp[0]=0;// resets arrays back to 0
disp[1]=0;
disp[2]=0;
disp[3]=0;
pass[0]=0;
pass[1]=0;
pass[2]=0;
pass[3]=0;
add=0;
lbox = 1;// system back to state 1
check1 = 0;// open button reset
check2 = 0;// open button reset

while(s<260)
{

P8 -> OUT = 0b00011100;
P4 -> OUT = AF[0];
wait_loop(0);

P8 -> OUT = 0b00101100;
P4 -> OUT = AF[0];
wait_loop(0);

P8 -> OUT = 0b00110100;
P4 -> OUT = AF[0];
wait_loop(0);

P8 -> OUT = 0b00111000;
P4 -> OUT = AF[0];
wait_loop(0);

P1-> OUT  = 0b00000001;// solenoid turned on for ~5 sec
s++;// while loop increases
}
s = 0;
}

if(disp[0]!=pass[0] || disp[1]!=pass[1] || disp[2]!=pass[2] || disp[3]!=pass[3])// if wrong password entered
{
wrongcount++;// wrong count increases every time wrong password entered
while(s<30){
led++;// led counter increases
// _LOC displayed when password is wrong
P8 -> OUT = 0b00011100;
P4 -> OUT = AF[14];
wait(0);

P8 -> OUT = 0b00101100;
P4 -> OUT = AF[15];
wait(0);

P8 -> OUT = 0b00110100;
P4 -> OUT = AF[0];
wait(0);

P8 -> OUT = 0b00111000;
P4 -> OUT = AF[12];
wait(0);
s++;// while loop counter increases

    if(led<10)
{
P5 -> OUT = 0b11111110;// led turns on
}

    if(led==10)
{
P5 -> OUT = 0b11111111; // led turns off
}

    if(led==20)// led reset to make led blink
{
led=0;// led counter reset
}
}
}
s=0;// while loop counter reset
lbox =3;// wrong password, back to state 3
check2 = 0;// open button reset

    if(wrongcount==5)// when wrongcount is 4, system moves to state 4
{
lbox = 4;// state 4
wrongcount=0; // wrong password reset
add=2;// add set to 2 to disable display output
}
}
break;

case 4:// Lock down

P4-> OUT = 0XFF;// display cleared to avoid number bunch-up

while(s<1000)
{

// _Ld_ displayed while on lock down
// No input accepted during this stage
P8 -> OUT = 0b00011100;
P4 -> OUT = AF[14];
wait(0);

P8 -> OUT = 0b00101100;
P4 -> OUT = AF[15];
wait(0);

P8 -> OUT = 0b00110100;
P4 -> OUT = AF[13];
wait(0);


P8 -> OUT = 0b00111000;
P4 -> OUT = AF[14];
wait(0);
s++;// while loop counter increased
}
add=1;// setting add to 1 activates second input switch statement again to accept input in stage 3
s = 0;// while loop counter set to 0
lbox = 3;// system back to stage 3 to accept password again
break;
}//lbox switch
}   //While loop
}   // Main Loop

// function definition for time wasting loop
void wait_loop(int t)
{
    for(t=0; t<1400; t++)
    {
    }
}

void wait(int w)
{
    for(w=0; w<1400; w++)
    {
    }
}
