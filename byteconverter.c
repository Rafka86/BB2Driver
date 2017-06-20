#include <stdio.h>

#include "byteconverter.h"

short byte2short(const byte* src, const unsigned short offset) {
  byte buf[2];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];

  return *((short *)buf);
}

unsigned short byte2ushort(const byte* src, const unsigned short offset) {
  byte buf[2];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];

  return *((unsigned short *)buf);
}

int byte2int(const byte* src, const unsigned short offset) {
  byte buf[4];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];
  buf[2] = src[offset + 2];
  buf[3] = src[offset + 3];

  return *((int *)buf);
}

unsigned int byte2uint(const byte* src, const unsigned short offset) {
  byte buf[4];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];
  buf[2] = src[offset + 2];
  buf[3] = src[offset + 3];

  return *((unsigned int *)buf);
}

long byte2long(const byte* src, const unsigned short offset) {
  byte buf[8];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];
  buf[2] = src[offset + 2];
  buf[3] = src[offset + 3];
  buf[4] = src[offset + 4];
  buf[5] = src[offset + 5];
  buf[6] = src[offset + 6];
  buf[7] = src[offset + 7];

  return *((long *)buf);
}

unsigned long byte2ulong(const byte* src, const unsigned short offset) {
  byte buf[8];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];
  buf[2] = src[offset + 2];
  buf[3] = src[offset + 3];
  buf[4] = src[offset + 4];
  buf[5] = src[offset + 5];
  buf[6] = src[offset + 6];
  buf[7] = src[offset + 7];

  return *((unsigned long *)buf);
}

float byte2float(const byte* src, const unsigned short offset) {
  byte buf[4];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];
  buf[2] = src[offset + 2];
  buf[3] = src[offset + 3];

  return *((float *)buf);
}

double byte2double(const byte* src, const unsigned short offset) {
  byte buf[8];
  buf[0] = src[offset    ];
  buf[1] = src[offset + 1];
  buf[2] = src[offset + 2];
  buf[3] = src[offset + 3];
  buf[4] = src[offset + 4];
  buf[5] = src[offset + 5];
  buf[6] = src[offset + 6];
  buf[7] = src[offset + 7];

  return *((double *)buf);
}

void short2byte(byte* dst, const unsigned short offset, const short src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
}

void ushort2byte(byte* dst, const unsigned short offset, const unsigned short src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
}

void int2byte(byte* dst, const unsigned short offset, const int src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
  dst[offset + 2] = bufp[2];
  dst[offset + 3] = bufp[3];
}

void uint2byte(byte* dst, const unsigned short offset, const unsigned int src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
  dst[offset + 2] = bufp[2];
  dst[offset + 3] = bufp[3];
}

void long2byte(byte* dst, const unsigned short offset, const long src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
  dst[offset + 2] = bufp[2];
  dst[offset + 3] = bufp[3];
  dst[offset + 4] = bufp[4];
  dst[offset + 5] = bufp[5];
  dst[offset + 6] = bufp[6];
  dst[offset + 7] = bufp[7];
}

void ulong2byte(byte* dst, const unsigned short offset, const unsigned long src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
  dst[offset + 2] = bufp[2];
  dst[offset + 3] = bufp[3];
  dst[offset + 4] = bufp[4];
  dst[offset + 5] = bufp[5];
  dst[offset + 6] = bufp[6];
  dst[offset + 7] = bufp[7];
}

void float2byte(byte* dst, const unsigned short offset, const float src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
  dst[offset + 2] = bufp[2];
  dst[offset + 3] = bufp[3];
}

void double2byte(byte* dst, const unsigned short offset, const double src) {
  byte *bufp = (byte *)&src;
  dst[offset    ] = bufp[0];
  dst[offset + 1] = bufp[1];
  dst[offset + 2] = bufp[2];
  dst[offset + 3] = bufp[3];
  dst[offset + 4] = bufp[4];
  dst[offset + 5] = bufp[5];
  dst[offset + 6] = bufp[6];
  dst[offset + 7] = bufp[7];
}
