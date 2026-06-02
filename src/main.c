#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "qrcodegen.h"
#include "TinyPngOut.h"

#define QR_MAX_INPUT_LENGTH 2953

int main()
{
    // Text data
    uint8_t qr[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    char inputText[QR_MAX_INPUT_LENGTH];
    memset(inputText, 0, QR_MAX_INPUT_LENGTH);

    fgets(inputText, QR_MAX_INPUT_LENGTH, stdin);

    bool ok = qrcodegen_encodeText(inputText,
        tempBuffer, qr, qrcodegen_Ecc_MEDIUM,
        qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX,
        qrcodegen_Mask_AUTO, true);
    if (!ok)
        return 0;

    FILE *file = fopen("out.png", "wb");
    if (!file)
    {
        printf("Couldn't open out file\n");
        exit(-1);
    }

    // qr codes are square -> 
    // one in is enough to tell the size
    int size = qrcodegen_getSize(qr);

    struct TinyPngOut writer;
    if (TinyPngOut_init(&writer, size, size, file) != TINYPNGOUT_OK)
    {
        printf("Couldn't initialize TinyPngOut writer\n");
        exit(-1);
    }

    uint8_t pixels[size * size * 3];

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) 
        {
            int i = (y * size + x) * 3;
            bool black = qrcodegen_getModule(qr, x, y); 
            
            pixels[i+0] = black ? 0 : 255;
            pixels[i+1] = black ? 0 : 255;
            pixels[i+2] = black ? 0 : 255;
        }
    }

    TinyPngOut_write(&writer, pixels, size * size * 3);
    fclose(file);
}


/*
for future updates:
    git add .
    git commit -m "describe changes"
    git push
*/