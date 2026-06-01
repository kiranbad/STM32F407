#include <stdint.h>

int global_init = 100;      // Goes to .data section
int global_uninit;          // Goes to .bss section

int main(void)
{
    int local_var = 10;

    global_uninit = 50;

    while(1)
    {
    }
}
