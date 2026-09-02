#include <htc.h>
#include <pic18f4520.h>

#define _XTAL_FREQ 8000000UL

// CONFIG1H - Oscillator
#pragma config OSC    = HS       // High Speed external crystal (8MHz)
#pragma config FCMEN  = OFF      // Fail-Safe Clock Monitor disabled
#pragma config IESO   = OFF      // Internal/External Oscillator Switchover disabled

// CONFIG2L - Power-up and Brown-out
#pragma config PWRT   = OFF      // Power-up Timer disabled
#pragma config BOREN  = OFF      // Brown-out Reset disabled
#pragma config BORV   = 3        // BOR voltage (irrelevant since BOREN=OFF)

// CONFIG2H - Watchdog Timer
#pragma config WDT    = OFF      // Watchdog Timer disabled
#pragma config WDTPS  = 32768    // WDT postscaler (irrelevant since WDT=OFF)

// CONFIG3H - CCP, ADC, MCLR
#pragma config CCP2MX = PORTC    // CCP2 MUX on RC1
#pragma config PBADEN = OFF      // PORTB<4:0> digital on Reset
#pragma config LPT1OSC = OFF     // Timer1 in regular mode
#pragma config MCLRE  = ON       // MCLR pin enabled

// CONFIG4L - Stack, LVP, XINST
#pragma config STVREN = ON       // Stack over/underflow reset enabled
#pragma config LVP    = OFF      // Single-Supply ICSP disabled
#pragma config XINST  = OFF      // Instruction set extension disabled

// CONFIG5L - Code Protection
#pragma config CP0    = OFF
#pragma config CP1    = OFF
#pragma config CP2    = OFF
#pragma config CP3    = OFF

// CONFIG5H
#pragma config CPB    = OFF      // Boot block code protection off
#pragma config CPD    = OFF      // Data EEPROM code protection off

// CONFIG6L - Write Protection
#pragma config WRT0   = OFF
#pragma config WRT1   = OFF
#pragma config WRT2   = OFF
#pragma config WRT3   = OFF

// CONFIG6H
#pragma config WRTC   = OFF
#pragma config WRTB   = OFF
#pragma config WRTD   = OFF

// CONFIG7L - Table Read Protection
#pragma config EBTR0  = OFF
#pragma config EBTR1  = OFF
#pragma config EBTR2  = OFF
#pragma config EBTR3  = OFF

// CONFIG7H
#pragma config EBTRB  = OFF
/*LED PINS (outputs)*/
#define GREEN_LED1  LATAbits.LATA0
#define RED_LED1    LATAbits.LATA1
#define GREEN_LED2  LATAbits.LATA2
#define RED_LED2    LATAbits.LATA3
#define GREEN_LED3  LATAbits.LATA4
#define RED_LED3    LATAbits.LATA5
#define GREEN_LED4  LATCbits.LATC0
#define RED_LED4    LATCbits.LATC1

/* ============================================================
   IR SENSOR PINS (inputs)
   ============================================================ */
#define L1_IR1  PORTBbits.RB0
#define L1_IR2  PORTBbits.RB1
#define L1_IR3  PORTBbits.RB2

#define L2_IR1  PORTBbits.RB3
#define L2_IR2  PORTBbits.RB4
#define L2_IR3  PORTBbits.RB5

#define L3_IR1  PORTCbits.RC2
#define L3_IR2  PORTCbits.RC3
#define L3_IR3  PORTCbits.RC4

#define L4_IR1  PORTCbits.RC5
#define L4_IR2  PORTCbits.RC6
#define L4_IR3  PORTCbits.RC7

/* ============================================================
   LCD PINS
   ============================================================ */
#define RS  PORTEbits.RE0
#define EN  PORTEbits.RE1

/* ============================================================
   LCD FUNCTIONS
   ============================================================ */
void LCD_Delay(void)   
{ 
__delay_ms(5); 
}

void LCD_Cmd(unsigned char cmd)
{
    LATD = cmd; 
	RS = 0;  
	EN = 1;  
	LCD_Delay();  
	EN = 0;
}

void LCD_Data(unsigned char data)
{
    LATD = data; 
    RS = 1;  
	EN = 1;  
	LCD_Delay();  
	EN = 0;
}

void LCD_String(const char *str)
{
    while(*str)  LCD_Data(*str++);
}

void LCD_Init(void)
{
    __delay_ms(20);
    LCD_Cmd(0x38);//Configures the display for 8-bit mode, 2 display lines, and a 5x8 font matrix
    LCD_Cmd(0x0C);//Turns the display ON and hides the cursor
    LCD_Cmd(0x06);//moves left to righ
    LCD_Cmd(0x01);//Clears the display memory
    __delay_ms(5);
}

void LCD_ClearLine2(void)
{
    LCD_Cmd(0xC0);
    LCD_String("                ");  // 16 spaces
    LCD_Cmd(0xC0);
}

/* ============================================================
   MAIN PROGRAM
   ============================================================ */
void main(void)
{
    unsigned char l1_busy, l2_busy, l3_busy, l4_busy;
    unsigned char freeLanes[4];
    unsigned char count, i;

    // This stores the PREVIOUS state — used to detect changes
    
    unsigned char prev_l1 = 0xFF;
    unsigned char prev_l2 = 0xFF;
    unsigned char prev_l3 = 0xFF;
    unsigned char prev_l4 = 0xFF;

    /* --- Port Setup --- */
    ADCON1 = 0x0F;
    TRISA  = 0x00;
    TRISB  = 0xFF;
    TRISC  = 0xFF;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISD  = 0x00;
    TRISE  = 0x00;

    /* --- LCD Startup --- */
    LCD_Init();
    LCD_Cmd(0x80);
    LCD_String("  Toll System   ");   
    
    while(1)
    {
        /* --------------------------------------------------
           STEP 1 : Read current lane statuses
           -------------------------------------------------- */
        l1_busy = (L1_IR1==0 && L1_IR2==0 && L1_IR3==0);
        l2_busy = (L2_IR1==0 && L2_IR2==0 && L2_IR3==0);
        l3_busy = (L3_IR1==0 && L3_IR2==0 && L3_IR3==0);
        l4_busy = (L4_IR1==0 && L4_IR2==0 && L4_IR3==0);

        /* --------------------------------------------------
           STEP 2 : Update LEDs immediately
           -------------------------------------------------- */
        GREEN_LED1 = !l1_busy;   RED_LED1 = l1_busy;
        GREEN_LED2 = !l2_busy;   RED_LED2 = l2_busy;
        GREEN_LED3 = !l3_busy;   RED_LED3 = l3_busy;
        GREEN_LED4 = !l4_busy;   RED_LED4 = l4_busy;

        
        if (l1_busy != prev_l1 ||
            l2_busy != prev_l2 ||
            l3_busy != prev_l3 ||
            l4_busy != prev_l4)
        {
            /* --- State changed — save new state --- */
            prev_l1 = l1_busy;
            prev_l2 = l2_busy;
            prev_l3 = l3_busy;
            prev_l4 = l4_busy;

            /* --- Build free lanes array --- */
            count = 0;
            if (!l1_busy) freeLanes[count++] = '1';
            if (!l2_busy) freeLanes[count++] = '2';
            if (!l3_busy) freeLanes[count++] = '3';
            if (!l4_busy) freeLanes[count++] = '4';

            /* --- Update LCD line 2 only now --- */
            LCD_ClearLine2();

            if (count == 0)
            {
                LCD_String("All Busy");
            }
            else
            {
                LCD_String("Free: ");
                for (i = 0; i < count; i++)
                {
                    LCD_Data(freeLanes[i]);
                    LCD_Data(' ');
                }
            }
        }

        
        __delay_ms(50);
    }
}