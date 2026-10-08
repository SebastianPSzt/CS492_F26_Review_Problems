#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>


/*
Assumptions:
- UART Port mapped to physical address space via MMIO
- 16bit address-space
- Base address: 1x1000
- Each register is mapped to 1 byte
- MMIO ops take 10 cycles, regular ops take 1 cycle

Register Offsets:
+0x00 -> status register
+0x01 -> tx_data
+0x02 -> rx_data

Status Register Bit Masks:
- 0x01 -> tx_data empty
- 0x02 -> rx_data ready

Bit Mask Example:
- status register: b01010011
- tx_data bitmask: b00000001
- s & t: b00000001
- We have extracted the ONE bit that represents whether or not tx_data is empty (ready for transmission)
- If the value is nonzero, then its empty and ready, if its zero, then its not empty and not ready to be written to
*/

/**
 * 0) How many cycles does the following program body take?
 * 
 * 1) Given the fact that the CPU can process 200k cycles a second, and a full write takes 200 cycles, how many write operations do we
 * need to do to achieve 20% CPU utilization
 */
int main(void) {
    // Declared volatile to prevent compiler optimizations (we always want it to read/write to memory)
    volatile uint8_t * restrict status = (volatile uint8_t *)0x1000;
    volatile uint8_t * restrict tx_data = (volatile uint8_t *)0x1001;

    char c[1] = "H";

    while(!(*status & 0x01));
    *tx_data = *(volatile uint8_t *)(c);
    
    exit(EXIT_SUCCESS);
}

/*
Corresponding Assembly:
    LDR r0, =0x1000         @ Load base address into register r0
    LDR r1, =c              @ Load character address into register r1

while_loop:
    LDR r2, [r0]            @ Load current status into register r2
    AND r2, r2, 0x01        @ Apply TX_data empty bitmask to status
    CMP r2, #0              @ Check if TX_data is empty and ready for write
    BEQ while_Loop          @ Loop if not empty

    LDR r3, [r1]            @ Load the character into register r3
    STR r3, [r0, #1]        @ Store the character in the TX_data register
    
    RET                     @ Return from the function call
*/