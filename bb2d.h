#ifndef _BB2D_H_
#define _BB2D_H_

#define VID 0x1962
#define PID 0x2080

#define BB2_EP 0x01

#define CMD_READ  0x72
#define CMD_WRITE 0x77

#define REPORT_SIZE           64
#define REPORT_ADDR_CMD        0
#define REPORT_ADDR_SEND_TRGT  1
#define REPORT_ADDR_SEND_SIZE  3
#define REPORT_ADDR_SEND_BODY  4
#define REPORT_ADDR_RECV_SIZE  1
#define REPORT_ADDR_RECV_BODY  2

#define MEMORY_SIZE 360

#define TIME_OUT 1000 //1s

int BB2D_Init();
int BB2D_Connect();
int BB2D_Disconnect();
int BB2D_Exit();
int BB2D_Read(const unsigned short addr, const unsigned short size);

         char   BB2D_GetChar  (const unsigned short addr);
unsigned char   BB2D_GetUChar (const unsigned short addr);
         short  BB2D_GetShort (const unsigned short addr);
unsigned short  BB2D_GetUShort(const unsigned short addr);
         long   BB2D_GetLong  (const unsigned short addr);
unsigned long   BB2D_GetULong (const unsigned short addr);
         double BB2D_GetDouble(const unsigned short addr);

int BB2D_ReadChar  (const unsigned short addr,          char*   out);
int BB2D_ReadUChar (const unsigned short addr, unsigned char*   out);
int BB2D_ReadShort (const unsigned short addr,          short*  out);
int BB2D_ReadUShort(const unsigned short addr, unsigned short*  out);
int BB2D_ReadLong  (const unsigned short addr,          long*   out);
int BB2D_ReadULong (const unsigned short addr, unsigned long*   out);
int BB2D_ReadDouble(const unsigned short addr,          double* out); 

int BB2D_WriteChar  (const unsigned short addr, const          char   src);
int BB2D_WriteUChar (const unsigned short addr, const unsigned char   src);
int BB2D_WriteShort (const unsigned short addr, const          short  src);
int BB2D_WriteUShort(const unsigned short addr, const unsigned short  src);
int BB2D_WriteLong  (const unsigned short addr, const          long   src);
int BB2D_WriteULong (const unsigned short addr, const unsigned long   src);
int BB2D_WriteDouble(const unsigned short addr, const          double src);

#endif //_BB2D_H_
