/*
 * libeg/load_icns.c
 * Loading function for .icns Apple icon images
 *
 * Copyright (c) 2006 Christoph Pfisterer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *  * Neither the name of Christoph Pfisterer nor the names of the
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "libegint.h"

//
// Decompress .icns RLE data
//

VOID egDecompressIcnsRLE(IN OUT UINT8 **CompData, IN OUT UINTN *CompLen, IN UINT8 *PixelData, IN UINTN PixelCount)
{
    UINT8 *cp;
    UINT8 *cp_end;
    UINT8 *pp;
    UINTN pp_left;
    UINTN len, i;
    UINT8 value;
    
    // setup variables
    cp = *CompData;
    cp_end = cp + *CompLen;
    pp = PixelData;
    pp_left = PixelCount;
    
    // decode
    while (cp + 1 < cp_end && pp_left > 0) {
        len = *cp++;
        if (len & 0x80) {   // compressed data: repeat next byte
            len -= 125;
            if (len > pp_left)
                break;
            value = *cp++;
            for (i = 0; i < len; i++) {
                *pp = value;
                pp += 4;
            }
        } else {            // uncompressed data: copy bytes
            len++;
            if (len > pp_left || cp + len > cp_end)
                break;
            for (i = 0; i < len; i++) {
                *pp = *cp++;
                pp += 4;
            }
        }
        pp_left -= len;
    }
    
    if (pp_left > 0) {
        Print(L" egDecompressIcnsRLE: still need %d bytes of pixel data\n", pp_left);
    }
    
    // record what's left of the compressed data stream
    *CompData = cp;
    *CompLen = (UINTN)(cp_end - cp);
}

//
// Load Apple .icns icons
//

EG_IMAGE * egDecodeICNS(IN UINT8 *FileData, IN UINTN FileDataLength, IN UINTN IconSize, IN BOOLEAN WantAlpha)
{
    EG_IMAGE            *NewImage;
    UINT8               *Ptr, *BufferEnd, *DataPtr, *MaskPtr;
    UINT32              BlockLen, DataLen, MaskLen;
    UINTN               FetchPixelSize, PixelCount, i;
    UINT8               *CompData;
    UINTN               CompLen;
    UINT8               *SrcPtr;
    EG_PIXEL            *DestPtr;
    
    if (FileDataLength < 8 || FileData == NULL ||
        FileData[0] != 'i' || FileData[1] != 'c' || FileData[2] != 'n' || FileData[3] != 's') {
        // not an icns file...
        return NULL;
    }
    
    FetchPixelSize = IconSize;
    for (;;) {
        DataPtr = NULL;
        DataLen = 0;
        MaskPtr = NULL;
        MaskLen = 0;
        
        Ptr = FileData + 8;
        BufferEnd = FileData + FileDataLength;
        // iterate over tagged blocks in the file
        while (Ptr + 8 <= BufferEnd) {
            BlockLen = ((UINT32)Ptr[4] << 24) + ((UINT32)Ptr[5] << 16) + ((UINT32)Ptr[6] << 8) + (UINT32)Ptr[7];
            if (Ptr + BlockLen > BufferEnd)   // block continues beyond end of file
                break;
            
            // extract the appropriate blocks for each pixel size
            if (FetchPixelSize == 128) {
                if (Ptr[0] == 'i' && Ptr[1] == 't' && Ptr[2] == '3' && Ptr[3] == '2') {
                    if (Ptr[8] == 0 && Ptr[9] == 0 && Ptr[10] == 0 && Ptr[11] == 0) {
                        DataPtr = Ptr + 12;
                        DataLen = BlockLen - 12;
                    }
                } else if (Ptr[0] == 't' && Ptr[1] == '8' && Ptr[2] == 'm' && Ptr[3] == 'k') {
                    MaskPtr = Ptr + 8;
                    MaskLen = BlockLen - 8;
                }
                
            } else if (FetchPixelSize == 48) {
                if (Ptr[0] == 'i' && Ptr[1] == 'h' && Ptr[2] == '3' && Ptr[3] == '2') {
                    DataPtr = Ptr + 8;
                    DataLen = BlockLen - 8;
                } else if (Ptr[0] == 'h' && Ptr[1] == '8' && Ptr[2] == 'm' && Ptr[3] == 'k') {
                    MaskPtr = Ptr + 8;
                    MaskLen = BlockLen - 8;
                }
                
            } else if (FetchPixelSize == 32) {
                if (Ptr[0] == 'i' && Ptr[1] == 'l' && Ptr[2] == '3' && Ptr[3] == '2') {
                    DataPtr = Ptr + 8;
                    DataLen = BlockLen - 8;
                } else if (Ptr[0] == 'l' && Ptr[1] == '8' && Ptr[2] == 'm' && Ptr[3] == 'k') {
                    MaskPtr = Ptr + 8;
                    MaskLen = BlockLen - 8;
                }
                
            } else if (FetchPixelSize == 16) {
                if (Ptr[0] == 'i' && Ptr[1] == 's' && Ptr[2] == '3' && Ptr[3] == '2') {
                    DataPtr = Ptr + 8;
                    DataLen = BlockLen - 8;
                } else if (Ptr[0] == 's' && Ptr[1] == '8' && Ptr[2] == 'm' && Ptr[3] == 'k') {
                    MaskPtr = Ptr + 8;
                    MaskLen = BlockLen - 8;
                }
                
            }
            
            Ptr += BlockLen;
        }
        
        /* FUTURE: try to load a different size and scale it later
            if (DataPtr == NULL && FetchPixelSize == 32) {
                FetchPixelSize = 128;
                continue;
            }
        */
        break;
    }
    
    if (DataPtr == NULL)
        return NULL;   // no image found
    
    // allocate image structure and buffer
    NewImage = egCreateImage(FetchPixelSize, FetchPixelSize, WantAlpha);
    if (NewImage == NULL)
        return NULL;
    PixelCount = FetchPixelSize * FetchPixelSize;
    
    if (DataLen < PixelCount * 3) {
        
        // pixel data is compressed, RGB planar
        CompData = DataPtr;
        CompLen  = DataLen;
        egDecompressIcnsRLE(&CompData, &CompLen, PLPTR(NewImage, r), PixelCount);
        egDecompressIcnsRLE(&CompData, &CompLen, PLPTR(NewImage, g), PixelCount);
        egDecompressIcnsRLE(&CompData, &CompLen, PLPTR(NewImage, b), PixelCount);
        // possible assertion: CompLen == 0
        if (CompLen > 0) {
            Print(L" egLoadICNSIcon: %d bytes of compressed data left\n", CompLen);
        }
        
    } else {
        
        // pixel data is uncompressed, RGB interleaved
        SrcPtr  = DataPtr;
        DestPtr = NewImage->PixelData;
        for (i = 0; i < PixelCount; i++, DestPtr++) {
            DestPtr->r = *SrcPtr++;
            DestPtr->g = *SrcPtr++;
            DestPtr->b = *SrcPtr++;
        }
        
    }
    
    // add/set alpha plane
    if (MaskPtr != NULL && MaskLen >= PixelCount && WantAlpha)
        egInsertPlane(MaskPtr, PLPTR(NewImage, a), PixelCount);
    else
        egSetPlane(PLPTR(NewImage, a), WantAlpha ? 255 : 0, PixelCount);
    
    // FUTURE: scale to originally requested size if we had to load another size
    
    return NewImage;
}

/* EOF */
