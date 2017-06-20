#ifndef _BB2D_H_
#define _BB2D_H_

#define VID 0x1962
#define PID 0x2080

#define BB2_EP 0x01

#define CMD_READ  0x72
#define CMD_WRITE 0x77

#define REPORT_SIZE 64
#define MEMORY_SIZE 360

#define TIME_OUT 1000 //1s

int BB2D_Connect();
int BB2D_Disconnect();
int BB2D_Read(const unsigned short addr, const unsigned short size);

         char   BB2D_GetChar  (const unsigned short addr);
unsigned char   BB2D_GetUChar (const unsigned short addr);
         short  BB2D_GetShort (const unsigned short addr);
unsigned short  BB2D_GetUShort(const unsigned short addr);
         int    BB2D_GetInt   (const unsigned short addr);
unsigned int    BB2D_GetUInt  (const unsigned short addr);
         long   BB2D_GetLong  (const unsigned short addr);
unsigned long   BB2D_GetULong (const unsigned short addr);
         float  BB2D_GetFloat (const unsigned short addr);
         double BB2D_GetDouble(const unsigned short addr); 

int BB2D_WriteChar  (const unsigned short addr, const          char   src);
int BB2D_WriteUChar (const unsigned short addr, const unsigned char   src);
int BB2D_WriteShort (const unsigned short addr, const          short  src);
int BB2D_WriteUShort(const unsigned short addr, const unsigned short  src);
int BB2D_WriteInt   (const unsigned short addr, const          int    src);
int BB2D_WriteUInt  (const unsigned short addr, const unsigned int    src);
int BB2D_WriteLong  (const unsigned short addr, const          long   src);
int BB2D_WriteULong (const unsigned short addr, const unsigned long   src);
int BB2D_WriteFloat (const unsigned short addr, const          float  src);
int BB2D_WriteDouble(const unsigned short addr, const          double src);

#endif //_BB2D_H_
