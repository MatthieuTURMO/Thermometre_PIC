/*
 * File:   newmain.c
 * Author: tverdu
 *
 * Created on 5 février 2016, 13:52
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>


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


/*Fonction qui effectue un léger délai*/
void delay(){
    int j,i;
    for(i=0;i<10000;i++){
        for(j=0;j<100;j++);
    }
    
}

/*Un délai légèrement plus important*/
void petitDelay(){
    int j,i;
    for(i=0;i<100;i++){
        for(j=0;j<100;j++);
    }
    
}

/*Supprimer le contenu de l'intégralité de l'afficheur LCD*/
void clearAll(){
        LCDHome();         
         petitDelay();
         LCDclear();
         newLine();
         LCDclear();
         petitDelay();         
        LCDHome();
}


/*Fonction principale*/
void main(void) {
    
    char phrase[10][16];
    char lettre='K';
    
    strcpy(phrase[0],"Bonjour");
    strcpy(phrase[1],"Nous");
    strcpy(phrase[2],"Sommes Present");
    strcpy(phrase[3],"Aujourd'hui");
    strcpy(phrase[4],"Pour vous");
    strcpy(phrase[5],"A bientot");
    strcpy(phrase[6],"...");
    
    
    int i=0;
    
    InitLCD_b();
 
        
    while(1){
        if(i>5)i=0;     
        
        
         putsLCD_b(phrase[i]);
         newLine();
         putsLCD_b(phrase[i+1]);
         i++;     
        
         
         delay();
         clearAll();
    }
        
    
    
    return;
}
