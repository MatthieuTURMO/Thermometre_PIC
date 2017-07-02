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



void delay(){
    int j,i;
    for(i=0;i<10000;i++){
        for(j=0;j<30;j++);
    }
    
}


void petitDelay(){
    int j,i;
    for(i=0;i<100;i++){
        for(j=0;j<100;j++);
    }
    
}

void clearAll(){
        LCDHome();         
         petitDelay();
         LCDclear();
         petitDelay();
        LCDHome();
}

/*Renvoie la température du capteur*/
float Conversion(){
    float vin;
    float Temperature;   
    
AD1CON1bits.SAMP = 1;             // commence a acquerir des données
delay();
                                  
AD1CON1bits.SAMP = 0;             // commence a convertir
while (!AD1CON1bits.DONE){};      // tant que la conversion n'est pas finie

vin=ADC1BUF0*0.003222;
Temperature =(vin-0.5)*100;       //on calcule

return Temperature;
}

/*Gère l'affichage sur le LCD*/
void afficher(){
    
    char txt[16];
    char phrase[10][16];
    TRISBbits.TRISB4=1;
    float Temperature;
    int i;
    
    strcpy(phrase[0],"Bonjour       ");
    strcpy(phrase[1],"La temperature");
    strcpy(phrase[2],"actuelle de la");
    strcpy(phrase[3],"piece est de  ");
   


for(i=0;i<4;i++){    
        Temperature=Conversion();
        sprintf(txt, "Temp : %.2lf C", Temperature);
        strcpy(phrase[4],txt);
        
        putsLCD_b(phrase[i]);
        newLine();
        putsLCD_b(phrase[4]);   
        delay();
        LCDHome();
    }
}

/*Configuration initiale du capteur de température*/
void ConfTemp(){

    AD1PCFG = 0xFFEF;                     // AN4 en temps qu'analogique, tous les autres numériques
    AD1CON1 = 0x8000;                     // SAMP bit = 0 arrete l'acquisition et commence a convertir
    AD1CHS  = 0x0004;                     // Connecte AN4 comme une entrée
                                         
    AD1CSSL = 0;
    AD1CON3 = 0x0002;                     // Acquisition manuelle de 3Tcy
    AD1CON2 = 0;
    
    
}

void main(void) {
    ConfTemp();
    InitLCD_b();

while (1)                            
{
    afficher(); 
    /*La fonction afficher appelle elle-même la fonction conversion, il ne suffit donc 
    que d'appeler afficher() ici*/
}
    return;
}
