#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char GetColorValue(unsigned char* data, int x, int y, int channel, const int CHANNELS, const int WIDTH)
{
    //**** Övning - hämta rätt värde "val" från data här
    unsigned char val = 0;

    val = data[(y*WIDTH + x)*CHANNELS + channel];

    return val;
}

int main(int argc, char* argv[])
{
    //Allocate an "image"
    unsigned char* imageMemory = malloc(64);

    //Fill image data with some values
    for(int i=0;i<64;i++)
    {
        imageMemory[i] = i;
    }

    //Properties of the image (4 channels = RGBA)
    const int CHANNELS = 4;
    const int WIDTH = 4;

    //The color value that we want
    int x = 3;
    int y = 3;
    int channel = 0;

    unsigned char val = GetColorValue(imageMemory, x, y, channel, CHANNELS, WIDTH);

    printf("%d\n", val);

    free(imageMemory);

	return 0;
}