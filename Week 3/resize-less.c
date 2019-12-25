#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{

    // Initialize and store file names
    char *infile = argv[2];
    char *outfile = argv[3];

    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    int resize_factor = atoi(argv[1]);
    if (resize_factor < 0 || resize_factor > 99)
    {
        fprintf(stderr, "n must be possitive and less than 100\n");
        return 1;
    }
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // Declare infile's and outfile's header files
    BITMAPFILEHEADER in_bf;
    BITMAPINFOHEADER in_bi;
    BITMAPFILEHEADER out_bf;
    BITMAPINFOHEADER out_bi;

    // read infile's BITMAPFILEHEADER & BITMAPINFOHEADER files
    fread(&in_bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&in_bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (in_bf.bfType != 0x4d42 || in_bf.bfOffBits != 54 || in_bi.biSize != 40 ||
        in_bi.biBitCount != 24 || in_bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    out_bf = in_bf;
    out_bi = in_bi;
    // change outfile's info header height and width values based on resize_factor...
    out_bi.biWidth = in_bi.biWidth * resize_factor;
    out_bi.biHeight = in_bi.biHeight * resize_factor;

    // calc infile and outfile padding so we can calc outfile image size
    int in_padding = (4 - (in_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int out_padding = (4 - (out_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Modify biSizeImage in outfile's info header
    out_bi.biSizeImage = (out_bi.biWidth * sizeof(RGBTRIPLE) + out_padding) * abs(out_bi.biHeight);
    out_bf.bfSize = out_bi.biSizeImage + out_bf.bfOffBits;

    // write BITMAPFILEHEADER & BITMAPTINFOHEADER values for outfile
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Write resized data to the outfile
    // Loop through infile's scanlines
    for (int i = 0, biHeight = abs(in_bi.biHeight); i < biHeight; i++)
    {
        // VERTICAL resizing
        for (int v = 0; v < resize_factor; v++)
        {
            // iterate over pixels in infile's scanline
            for (int j = 0; j < in_bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // HORIZONTAL resizing
                for (int h = 0; h < resize_factor; h++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            fseek(inptr, in_padding, SEEK_CUR);

            // add any necessary padding to outfile
            for (int p = 0; p < out_padding; p++)
            {
                fputc(0x00, outptr);
            }
            fseek(inptr, -((in_bi.biWidth * sizeof(RGBTRIPLE)) + in_padding), SEEK_CUR);
        }

        // move pointer ahead to the end of the last scanline
        fseek(inptr, (in_bi.biWidth * sizeof(RGBTRIPLE)) + in_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // finished!
    return 0;
}
