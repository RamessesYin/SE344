#include"read.h"
GLubyte* gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{

    FILE		*pFile;
    tgaheader	tgaHeader;
    unsigned long lImageSize;
    short		sDepth;
    GLubyte		*pBits = NULL;

    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_RGB;
    *iComponents = GL_RGB;


    // Open the file
    pFile = fopen(szFileName, "rb");
    if (pFile == NULL)
        return NULL;

    fread(&tgaHeader, 18/* sizeof(TGAHEADER) */,1,pFile);

    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bpp / 8;

    if (tgaHeader.bpp != 8 && tgaHeader.bpp != 24 && tgaHeader.bpp != 32)
        return NULL;

    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

    pBits = (GLubyte*) malloc(lImageSize * sizeof(GLubyte));

    if (pBits == NULL)
        return NULL;

    if (fread(pBits, lImageSize, 1, pFile)!=1)
    {
        free(pBits);
        return NULL;

    }

    switch (sDepth)
    {
    case 3:
        *eFormat = GL_BGR;
        *iComponents = GL_RGB;
        break;
    case 4:
        *eFormat = GL_BGRA;
        *iComponents = GL_RGBA;
        break;
    case 1:
        *eFormat = GL_LUMINANCE;
        *iComponents = GL_LUMINANCE;
        break;
    default:
        break;
    }

    fclose(pFile);
    return pBits;

}

GLubyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight)
{
        FILE*	pFile;
        BMPInfo *pBitmapInfo = NULL;
        unsigned long lInfoSize = 0;
        unsigned long lBitSize = 0;
        GLubyte *pBits = NULL;					// Bitmaps bits
        BMPHeader	bitmapHeader;

        // Attempt to open the file
        pFile = fopen(szFileName, "rb");
        if(pFile == NULL)
            return NULL;

        // File is Open. Read in bitmap header information
        fread(&bitmapHeader, sizeof(BMPHeader), 1, pFile);

        // Read in bitmap information structure
        lInfoSize = bitmapHeader.offset - sizeof(BMPHeader);
        pBitmapInfo = (BMPInfo *) malloc(sizeof(GLbyte)*lInfoSize);
        if(fread(pBitmapInfo, lInfoSize, 1, pFile) != 1)
        {
            free(pBitmapInfo);
            fclose(pFile);
            return NULL;
        }

        // Save the size and dimensions of the bitmap
        *nWidth = pBitmapInfo->header.width;
        *nHeight = pBitmapInfo->header.height;
        lBitSize = pBitmapInfo->header.imageSize;

        // If the size isn't specified, calculate it anyway
        if(pBitmapInfo->header.bits != 24)
        {
            free(pBitmapInfo);
            return NULL;
        }

        if(lBitSize == 0)
            lBitSize = (*nWidth *
            pBitmapInfo->header.bits + 7) / 8 *
            abs(*nHeight);

        // Allocate space for the actual bitmap
        free(pBitmapInfo);
        pBits = (GLubyte*)malloc(sizeof(GLubyte)*lBitSize);

        // Read in the bitmap bits, check for corruption
        if(fread(pBits, lBitSize, 1, pFile) != 1)
        {
            free(pBits);
            pBits = NULL;
        }

        // Close the bitmap file now that we have all the data we need
        fclose(pFile);

        return pBits;


}
