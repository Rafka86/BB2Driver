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

/*void
void BB2D_SetChar  (const unsigned short addr,          char   src);
void BB2D_SetUChar (const unsigned short addr, unsigned char   src);
void BB2D_SetShort (const unsigned short addr,          short  src);
void BB2D_SetUShort(const unsigned short addr, unsigned short  src);
void BB2D_SetInt   (const unsigned short addr,          int    src);
void BB2D_SetUInt  (const unsigned short addr, unsigned int    src);
void BB2D_SetLong  (const unsigned short addr,          long   src);
void BB2D_SetULong (const unsigned short addr, unsigned long   src);
void BB2D_SetFloat (const unsigned short addr,          float  src);
void BB2D_SetDouble(const unsigned short addr,          double src); 
*/

#endif //_BB2D_H_
