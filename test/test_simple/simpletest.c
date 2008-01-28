/*
libdmtx - Data Matrix Encoding/Decoding Library
Copyright (C) 2007 Mike Laughton

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact: mike@dragonflylogic.com
*/

/* $Id$ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dmtx.h>

int
main(int argc, char **argv)
{
   int count = 0;
   unsigned char testString[] = "30Q324343430794<OQQ";
   DmtxImage image;
   DmtxEncode *encode;
   DmtxDecode decode;
   DmtxPixelLoc p0, p1;

   fprintf(stdout, "input:  \"%s\"\n", testString);

   /*
    * 1) Write a new Data Matrix barcode (in memory)
    */

   encode = dmtxEncodeStructCreate();
   dmtxEncodeDataMatrix(encode, strlen((char *)testString), testString, DMTX_SYMBOL_SQUARE_AUTO);

   // Take copy of new image before freeing DmtxEncode struct
   image = encode->image;
   image.pxl = (DmtxPixel *)malloc(image.width * image.height *
         sizeof(DmtxPixel));
   if(image.pxl == NULL) {
      perror("Malloc error");
      exit(1);
   }
   memcpy(image.pxl, encode->image.pxl, image.width * image.height *
         sizeof(DmtxPixel));
   dmtxEncodeStructDestroy(&encode);

   /*
    * 2) Read the Data Matrix barcode from above
    */

   p0.X = p0.Y = 0;
   p1.X = image.width - 1;
   p1.Y = image.height - 1;
   decode = dmtxDecodeInit(&image, p0, p1, 5);

   count = dmtxFindNextRegion(&decode);

   if(count > 0) {
      fprintf(stdout, "output: \"");
      fwrite(decode.matrix.output, sizeof(unsigned char),
            decode.matrix.outputIdx, stdout);
      fprintf(stdout, "\"\n\n");
   }

   dmtxImageDeInit(&image);

   exit(0);
}