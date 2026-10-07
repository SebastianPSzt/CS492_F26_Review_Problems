#include<stdlib.h>
#include<stdio.h>

/*
Assume the following specifications:
- UART peripheral mapped to physical memory (MMIO)
- 32-bit address-space
- Base address: 0x10000000
- Each register allocated 4 bytes
- Data may only be transmitted/received **1 byte** at a time
- MMIO operations take 10 cycles, all other operations take 1 cycle

Register specification:
+0 status register
+4 tx_data register (transmit to port)
+8 rx_data register (receive from port)

Status register bit masks:
- 0x01 - tx_data empty
- 0x02 - rx_data ready
*/

#define UART_STATUS_TX_EMPTY_MASK 0x01
#define UART_STATUS_RX_READY_MASK 0x02

/**
 * 0) What is the program doing?
 
 * 1) Under the assumption that both while loops take the same amount of cycles, will both for loops take the same amount of cycles?
 Show your work.
 
 * 2) If for each for-loop iteration, the while loop iterates 10 times, how many cycles will it take to complete the function body
 (successful read/write on 11th cycle)

 * 3) How would we implement a function that constantly waits to receive new data from the port, and then transmits it to
 a 1-byte software buffer that can be read by some external process? Give c-style pseudocode based on the c code below.
 */
int main(void) {
    volatile uint32_t * uart_status = (volatile uint32_t*)(0x10000000);
    volatile uint32_t * uart_tx_data = (volatile uint32_t*)(0x10000000 + 0x04);
    volatile uint32_t * uart_rx_data = (volatile uint32_t*)(0x10000000 + 0x08);

    uint8_t w[8];

    for (int i = 0; i < 8; i++) {
        while((*uart_status & UART_STATUS_RX_READY_MASK) == 0);
        
        w[i] = *(uint8_t*)(uart_rx_data);
    }

    for (int j = 0; j < 8; j++) {
        while((*uart_status & UART_STATUS_TX_EMPTY_MASK) == 0);

        *uart_tx_data = *(volatile uint32_t*)(&(w[j]));
    }

    return 0;
}

/*
Corresponding Function Body Assembly:
    LDR r1, =0x10000000     @ base address
    LDR r2, =w              @ word array address

    MOV r0, #8              @ loop 1 counter

for_loop1:
while_loop1:
    LDR r3, [r1]
    AND r3, r3, 0x02
    CMP r3, #0
    BEQ while_loop1

    LDRB r3, [r1, #8]
    STRB r3, [r2]

    ADD r2, r2, #1
    SUB r0, r0, #1
    CMP r0, #0
    BNE for_loop1


    LDR r2, =w              @ reload word arr base addr
    MOV r0, #8              @ reset loop counter


for_loop2:
while_loop2:
    LDR r3, [r1]            @ load status register
    AND r3, r3, 0x01        @ apply tx_data bitmask
    CMP r3, 0               @ cmp
    BEQ while_loop2         @ keep iterating if not ready

    LDRB r3, [r2]           @ read w[i]
    STRB r3, [r1, #4]       @ store in tx_data

    ADD r2, r2, #1
    SUB r0, r0, #1
    CMP r0, #0
    BNE for_loop2
 */