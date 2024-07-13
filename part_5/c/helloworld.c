#include <stdio.h>
#include "sleep.h"

#define MEM_GPIO_BASE 	0x41200000

uint32_t *mem_gpio_p;

int main()
{
	mem_gpio_p = (uint32_t *)MEM_GPIO_BASE;

	while (1)
	{
		*(mem_gpio_p+0) = 0x0;
		sleep(1);
		*(mem_gpio_p+0) = 0xF;
		sleep(1);
		printf("Hello, World!\n");
	}

    return 0;
}
