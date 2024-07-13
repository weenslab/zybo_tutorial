#include <stdio.h>

#define MEM_INP_BASE 	0x40000000
#define MEM_GPIO_BASE 	0x41200000
#define MEM_OUT_BASE 	0x42000000

uint32_t *mem_inp_p, *mem_gpio_p, *mem_out_p;

int main()
{
	// Initialization
	mem_inp_p = (uint32_t *)MEM_INP_BASE;
	mem_gpio_p = (uint32_t *)MEM_GPIO_BASE;
	mem_out_p = (uint32_t *)MEM_OUT_BASE;

	// Write input
	printf("Input:\n");
    for (int i = 0; i <= 7; i++)
    {
    	uint8_t a = i + 1;
    	uint8_t b = 8 - i;
    	uint8_t y = i + 1;
    	*(mem_inp_p+i) = (y << 16) | (b << 8) | a;
    	printf(" a=%d, b=%d, y=%d\n", a, b, y);
    }

    // Start module
    *(mem_gpio_p+0) = 0x1;
    *(mem_gpio_p+0) = 0x0;

    // Wait until ready
    while (!(*(mem_gpio_p+2) & (1 << 0)));

    // Read input
    printf("Output:\n");
    for (int i = 0; i <= 7; i++)
    	printf(" %ld\n", (uint32_t)*(mem_out_p+i));

    return 0;
}
