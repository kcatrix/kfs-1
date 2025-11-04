#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F

void kernel_main(void)
{
    char *video_memory = (char *) VGA_ADDRESS;

    video_memory[0] = '4';
    video_memory[1] = WHITE_ON_BLACK;

    video_memory[2] = '2';
    video_memory[3] = WHITE_ON_BLACK;

    while (1);
}
