#ifndef _BB2D_H_
#define _BB2D_H_

#define VID 0x1962
#define PID 0x2080

#define BB2_EP 0x01

#define CMD_READ  0x72
#define CMD_WRITE 0x77

#define REPORT_SIZE 64
#define TIME_OUT 1000 //1s

int BB2D_Connect();
int BB2D_Disconnect();

#endif //_BB2D_H_
