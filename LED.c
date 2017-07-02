#include <p24fxxxx.h>
#include "lcd.h"
#include <stdio.h>


void InitLCD_b( void)
{
    AD1PCFG = 0xFFE0;
    TRISE = 0x0000;
    TRISD = 0x0000;
    TRISB = 0x001F;

    PORTDbits.RD5=0;
    PORTBbits.RB15=0;
    PORTDbits.RD4=0;

    // init TMR1
    T1CON = 0x8030;

    PMDATA = 0b00111000;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;



    PMDATA = 0b00001100;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;



    PMDATA = 0b00000001;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;

    TMR1 = 0; while( TMR1<100);   // 3 x 16us = 48us


    PMDATA = 0b00000110;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;

   // TMR1 = 0; while( TMR1<200);   // 3 x 16us = 48us

} // InitLCD
//
void PutLCD_b(char kar)
{
    PORTDbits.RD5=0;
    PORTBbits.RB15=1;
    PORTDbits.RD4=0;
    //TMR1 = 0; while( TMR1<1);   // 3 x 16us = 48us
    PMDATA = kar;
    //TMR1 = 0; while( TMR1<1);   // 3 x 16us = 48us
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;

}
//
void putsLCD_b( char *s)
{
    while( *s) PutLCD_b( *s++);
} //putsLCD


void LCDclear(void)
{
    putsLCD_b("                    ");
    
}

void newLine()
{
    PORTDbits.RD5=0;
    PORTBbits.RB15=0;
    PORTDbits.RD4=0;

    PMDATA = 0xC0;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;
}

void LCDHome()
{
    PORTDbits.RD5=0;
    PORTBbits.RB15=0;
    PORTDbits.RD4=0;

    PMDATA = 0x02;
    PORTDbits.RD4=1;
    TMR1 = 0; while( TMR1<3);   // 3 x 16us = 48us
    PORTDbits.RD4=0;
    TMR1 = 0; while(TMR1<60);
}
