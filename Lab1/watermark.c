#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Too few arguments!\n");
        return EXIT_FAILURE;
    }
    int origWidth, origHeight, origChannels, markWidth, markHeight, markChannels;
    char *origFilename = argv[1];
    char *markFilename = argv[2];
    char *newFilename = argv[3];

    unsigned char *origImg = stbi_load(origFilename, &origWidth, &origHeight, &origChannels, 0);
    unsigned char *markImg = stbi_load(markFilename, &markWidth, &markHeight, &markChannels, 0);

    if (origWidth - 10 < markWidth || origHeight - 10 < markHeight)
    {
        fprintf(stderr, "Watermark too big!\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < markHeight; i++)
    {
        for (int j = 0; j < markWidth; j++)
        {
            int orig_y = (origHeight - markHeight - 10 + i) * origWidth;
            int orig_x = origWidth - markWidth - 10 + j;
            origImg[(orig_y + orig_x) * origChannels] = markImg[(i * markWidth + j) * markChannels];
        }
    }

    if (strstr(newFilename, ".png") != NULL)
        stbi_write_png(newFilename, origWidth, origHeight, origChannels, origImg, origWidth * origChannels);
    else if (strstr(newFilename, ".jpg") != NULL || strstr(newFilename, "jpeg") != NULL)
        stbi_write_jpg(newFilename, origWidth, origHeight, origChannels, origImg, 100);

    stbi_image_free(origImg);
    stbi_image_free(markImg);

    printf("Sucessfully watermarked image.\n");
    return EXIT_SUCCESS;
}