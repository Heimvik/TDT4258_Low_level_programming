#include "usart.h"

void USART3_Init(void)
{
    PORTB.DIR &= ~PIN1_bm;
    PORTB.DIR |= PIN0_bm;

    USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
    USART3.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
}
void USART3_SendChar(char c)
{
    while (!(USART3.STATUS & USART_DREIF_bm))
    {
    ;
    }
    USART3.TXDATAL = c;
}
void USART3_SendString(char *str)
    {
    for(size_t i = 0; i < strlen(str); i++)
    {
    USART3_SendChar(str[i]);
    }
}

bool USART3_IsTxReady(void)
{
    return (bool)(USART3.STATUS & USART_DREIF_bm);
}

bool USART3_IsRxReady(void)
{
    return (bool)(USART3.STATUS & USART_RXCIF_bm);
}

uint8_t USART3_Read()
{
    while (!(USART3.STATUS & USART_RXCIF_bm))
    {
        ;
    }
    return USART3.RXDATAL;
}

// Helper function to convert a nibble (4 bits) to a hex character
char nibbleToHex(uint8_t nibble) {
    return (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
}

// Function to print a register value as hex with a label
void USART3_SendHex(uint8_t value, const char* regName) {
    // Send register name
    USART3_SendString(regName);
    USART3_SendString(": 0x");

    // Convert each nibble of the byte to a hex character and send it
    char hexStr[3];
    hexStr[0] = nibbleToHex((value >> 4) & 0x0F);  // High nibble
    hexStr[1] = nibbleToHex(value & 0x0F);         // Low nibble
    hexStr[2] = '\0';

    USART3_SendString(hexStr);
    USART3_SendString("\n");
}