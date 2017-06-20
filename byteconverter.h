#ifndef _BYTECONVERTER_H_
#define _BYTECONVERTER_H_

typedef unsigned char byte;

         short  byte2short (const byte* src, const unsigned short offset);
unsigned short  byte2ushort(const byte* src, const unsigned short offset);
         int    byte2int   (const byte* src, const unsigned short offset);
unsigned int    byte2uint  (const byte* src, const unsigned short offset);
         long   byte2long  (const byte* src, const unsigned short offset);
unsigned long   byte2ulong (const byte* src, const unsigned short offset);
         float  byte2float (const byte* src, const unsigned short offset);
         double byte2double(const byte* src, const unsigned short offset);

void  short2byte(byte* dst, const unsigned short offset, const          short  src);
void ushort2byte(byte* dst, const unsigned short offset, const unsigned short  src);
void    int2byte(byte* dst, const unsigned short offset, const          int    src);
void   uint2byte(byte* dst, const unsigned short offset, const unsigned int    src);
void   long2byte(byte* dst, const unsigned short offset, const          long   src);
void  ulong2byte(byte* dst, const unsigned short offset, const unsigned long   src);
void  float2byte(byte* dst, const unsigned short offset, const          float  src);
void double2byte(byte* dst, const unsigned short offset, const          double src);

#endif //_BYTECONVERTER_H_
