/****************************
Copyright © 2013 Luke Salisbury
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
****************************/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <cstdio>
#include <string.h>

typedef struct _BMPFileHeader
{
	uint16_t   FileType;     /* File type, always 4D42h ("BM") */
	uint32_t  FileSize;     /* Size of the file in bytes */
	uint16_t   Reserved1;    /* Always 0 */
	uint16_t   Reserved2;    /* Always 0 */
	uint32_t  BitmapOffset; /* Starting position of image data in bytes */
} BMPFILEHEADER;

typedef struct _BitmapHeader
{
		uint32_t Size;            /* Size of this header in bytes */
		int32_t  Width;           /* Image width in pixels */
		int32_t  Height;          /* Image height in pixels */
		uint16_t  Planes;          /* Number of color planes */
		uint16_t  BitsPerPixel;    /* Number of bits per pixel */
		uint32_t Compression;     /* Compression methods used */
		uint32_t SizeOfBitmap;    /* Size of bitmap in bytes */
		int32_t  HorzResolution;  /* Horizontal resolution in pixels per meter */
		int32_t  VertResolution;  /* Vertical resolution in pixels per meter */
		uint32_t ColorsUsed;      /* Number of colors in the image */
		uint32_t ColorsImportant; /* Minimum number of important colors */
} BITMAPHEADER;

typedef struct _BitmapPalette
{
		uint8_t Blue;      /* Blue component */
		uint8_t Green;     /* Green component */
		uint8_t Red;       /* Red component */
		uint8_t Reserved;  /* Padding (always 0) */
} BITMAPPALETTE;

void PutPixel(uint8_t * surface, int32_t x, int32_t y, int32_t w, uint32_t pixel)
{
	uint8_t * p = surface;
	p += ((y * w) + x) * 4;

	p[0] = pixel & 0xff;
	p[1] = (pixel >> 8) & 0xff;
	p[2] = (pixel >> 16) & 0xff;
	p[3] = (pixel >> 24) & 0xff;
}

/* Little Endian  AABBGGRR*/ 
#define RGBA32(r, g, b, a) ( (r) | (a << 24) | ((b)<<16)|((g)<<8) )
uint8_t * loadRLEBMP( const char * filename, int &w, int &h )
{
	FILE * file = fopen(filename,"rb");
	

	BITMAPHEADER header;
	BMPFILEHEADER fileheader;
	uint8_t * data = NULL;
	
	fread(&fileheader,14,1,file);
	fread(&header,40,1,file);

	if ( header.Compression == 1 )
	{
		
		uint32_t colors = 0;
		uint32_t * colours = new uint32_t[256];
		header.ColorsUsed = ( !header.ColorsUsed ? 256 : header.ColorsUsed);
		for  ( uint16_t c = 0; c < header.ColorsUsed ; c++  )
		{
			BITMAPPALETTE pal;
			fread(&pal,sizeof(BITMAPPALETTE),1,file);	
			if ( pal.Red == 0 &&  pal.Green == 255 && pal.Blue == 0 )
				colours[c] = 0x00000000;
			else
				colours[c] = RGBA32( pal.Red, pal.Green, pal.Blue, 255);
		}
		w = header.Width;
		h = header.Height;

		size_t size = header.Height * header.Width;
		if ( size <= 0 )
		{
			return NULL;
		}
		int32_t  x = 0;
		int32_t  y = header.Height - 1;
		data = new uint8_t[size*4];
	
		for ( uint32_t s = 0; s < header.SizeOfBitmap; s++ )
		{
			uint8_t a, b;
			fread(&a,1,1,file);	
			fread(&b,1,1,file);	

			if ( a == 0 )
			{
				if (b == 0) // End of Line
				{
					x = 0;
					y--;
				}
				else if (b == 1) //End of Bitmap
					break; 
				else if (b == 2) //Move
				{
					uint8_t mx = 0;
					uint8_t my = 0;
					fread( &mx, 1,1,file);	
					fread( &my, 1,1,file);	
					x += mx;
					y += my;
				}
				else // Pixel
				{
					int8_t d;
					for(uint16_t c = 0; c < b; c++)
					{
						fread(&d,1,1,file);	
						PutPixel(data, x, y, header.Width, colours[d]);
						x++;
					}
					if ( (b%2) )
						fread(&d,1,1,file);	
				}
			}
			else
			{
				for(uint16_t c = 0; c < a; c++)
				{
					PutPixel(data, x, y, header.Width, colours[b]);
					x++;
				}
				
			}	
		}
	}
	

		
	fclose(file);
	return data;
}
