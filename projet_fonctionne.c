/*
 * File:   main.c
 * Author: tverdu
 *
 * Created on 10 fÈvrier 2016, 14:08
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
  
#pragma config POSCMOD = XT           /* Primary Oscillator Select (Primary oscillator disabled) */ 
#pragma config OSCIOFNC = OFF            /* Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15)) */ 
#pragma config FCKSM = CSDCMD           /* Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled) */ 
#pragma config FNOSC = PRI           /* Oscillator Select (Fast RC Oscillator with Postscaler (FRCDIV)) */ 
#pragma config IESO = ON               /* Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled) */ 
#pragma config WDTPS = PS32768          /* Watchdog Timer Postscaler (1:32,768) */ 
#pragma config FWPSA = PR128            /* WDT Prescaler (Prescaler ratio of 1:128) */ 
#pragma config WINDIS = ON              /* Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled)) */ 
#pragma config FWDTEN = OFF             /* Watchdog Timer Enable (Watchdog Timer is disabled) */ 
#pragma config ICS = PGx2               /* Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2) */ 
#pragma config GWRP = OFF               /* General Code Segment Write Protect (Writes to program memory are allowed) */ 
#pragma config GCP = OFF                /* General Code Segment Code Protect (Code protection is disabled) */ 
#pragma config JTAGEN = OFF             /* JTAG Port Enable (JTAG port is disabled) */ 
  
volatile struct { 
    unsigned int annee; 
    unsigned int mois;
    unsigned int jour; 
    unsigned int heure;
    unsigned int minute;
    unsigned int seconde; 
    
} date;

/*
 Configuration du CAN afin de récupérer la température actuelle
 */
 
void ConfTemp(){
    
    
    
    

    AD1PCFG = 0xFFEF;     // AN4 en temps qu'analogique, tous les autres numériques
    AD1CON1 = 0x8000;     // SAMP bit = 0 arrete l'acquisition et commence a convertir
    AD1CHS  = 0x0004;     // Connecte AN4 comme une entrée
                                         
    AD1CSSL = 0;
    AD1CON3 = 0x0002;    // Acquisition manuelle de 3Tcy
    AD1CON2 = 0;
    
}

float Conversion(){
    float vin;
    float Temperature;   
    
    AD1CON1bits.SAMP = 1;             // commence a acquerir des données
    TMR1=0x0000;
    while(TMR1<1000);                 // délai
    AD1CON1bits.SAMP = 0;              // commence a convertir
    while (!AD1CON1bits.DONE){};      // tant que la conversion n'est pas finie

    vin=ADC1BUF0*0.003222;
    Temperature =(vin-0.5)*100;          //on calcule

    return Temperature;
}

/*  
**  Initial Real Time Clock Calender module 
*/  
void RTCC_Init( void ) 
{   
    /* Enable the secondary oscillator for 32.768KHz crystal */ 
    __builtin_write_OSCCONL(OSCCON | 0x02); 
    __builtin_write_RTCWEN();
 
    TRISDbits.TRISD8  = 0;       /* set RTCC alarm pin to output */ 
    
    RCFGCAL= 0b1010011100000000;      /* enable the RTCC module */ 
                                      /* enable the RTCC output pin */
                                      /* set the RTCC Value register window to Year */
    PADCFG1=0b0000000000000000;       /* set RTCC output pin to alarm pulse */

    RTCVAL  = 0x0016;   /* set year to xx16 */ 
    RTCVAL  = 0x0210;   /* set Month = 3, Day = 10 */
    RTCVAL  = 0x0314;   /* set Weekday = 3, Hour = 14 */ 
    RTCVAL  = 0x1000;   /* set Minutes = 10, Seconds = 0 */ 
    
    /* disable alarm and set alarm register pointer to Month and Day */ 
    ALCFGRPT = 0x0000; 
    
    ALCFGRPTbits.ALRMPTR=0b10;
    ALRMVAL  = 0x0210;  /* set Month = 2, Day = 3*/
    ALRMVAL  = 0x0314;  /* set Weekday = 3, Hour = 14 */
    ALRMVAL  = 0x1000;  /* set Minutes = 10, Seconds = 0 */
    
    ALCFGRPTbits.ARPT=0x00;
    ALCFGRPTbits.AMASK   = 0b0001; //Alarme toutes les secondes
    ALCFGRPTbits.CHIME   = 1;      //On répète à l'infini
    ALCFGRPTbits.ALRMEN  = 1;      //On active l'alarme
    
    IFS3bits.RTCIF=0;
    IEC3bits.RTCIE=1;
    
    IFS1bits.INT1IF=0;  /* Clear RTCC alarm interrupt assertion */
    IEC1bits.INT1IE=1;  /* Enable RTCC alarm interrupt */
    IPC5bits.INT1IP=4;  /* Set RTCC alarm interrupt priority */
    
    RCFGCALbits.RTCWREN = 0;       /* lock the RTCC, no more writes */ 
} 

void AfficherDate(){
    unsigned int tmp; /*variable temporaire avant decalage*/  
    
    RCFGCALbits.RTCPTR=0b11;     /* set the RTCC Value register window to Year */ 
        
    tmp = RTCVAL;
    date.annee = tmp;
    tmp = RTCVAL;
    date.mois = tmp>>8;
    date.jour =(tmp<<8)>>8;
    tmp = RTCVAL;    
    date.heure = (tmp<<8)>>8;
    tmp = RTCVAL;
    date.minute = tmp>>8;
    date.seconde = (tmp<<8)>>8;

    char chaineFinale[25]; /*contient la date+heure a afficher*/ 
    sprintf(chaineFinale, "%x/%x/%x %x:%x:%02x", date.jour, date.mois, date.annee, date.heure, date.minute, date.seconde);
    
    LCDHome();
    putsLCD_b(chaineFinale);

}


void AfficherTemp(){
    TRISBbits.TRISB4=1;
    float Temperature;
    
    char txt[16];
    
    Temperature=Conversion();
    sprintf(txt, "Temp : %.2lf C", Temperature);
    newLine();
    putsLCD_b(txt);
    
}
    
/*  
**  RTCC Alarm ISR 
*/  
void __attribute__((__interrupt__,no_auto_psv__)) _RTCCInterrupt( void )
{     
    IFS1bits.INT1IF=0;
    IFS3bits.RTCIF=0;
    AfficherDate();
    AfficherTemp();
    
}   

int main( void ) 
{   
    InitLCD_b();
    RTCC_Init();
    ConfTemp();
    
    
    AfficherDate();
    AfficherTemp();

    while(1) 
    {
                
    } 
    return 0 ; 
}   

