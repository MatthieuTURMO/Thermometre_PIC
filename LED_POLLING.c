/*
 * File:   newmain.c
 * Author: tverdu
 *
 * Created on 5 février 2016, 13:52
 */


#include <xc.h>

#pragma config POSCMOD = XT // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = OFF // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2 // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)

void main(void) {
    TRISAbits.TRISA0=0;
    TRISDbits.TRISD6=1;
    
    while(1){
        if((PORTDbits.RD6)==0x00)
            PORTA=0x01;
        else
            PORTA=0x00;
    }
    
    
    return;
}
