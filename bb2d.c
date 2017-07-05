#include <stdio.h>
#include <stdlib.h>
#include <libusb.h>

#include "bb2d.h"
#include "byteconverter.h"

enum MEMTYPE {
  NONE,
  CHAR,
  UCHAR,
  SHORT,
  USHORT,
  LONG,
  ULONG,
  DOUBLE
};

typedef struct libusb_context       libusb_context;
typedef struct libusb_device        libusb_device;
typedef struct libusb_device_handle libusb_device_handle;

byte report[REPORT_SIZE];
byte memmap[MEMORY_SIZE];
byte memtype[MEMORY_SIZE];

libusb_context       *ctx=NULL;
libusb_device_handle *dev_handle=NULL;

int BB2D_Error(const int ecd, const char *msg) {
  if (ecd != 0) perror(libusb_error_name(ecd));
  perror(msg);
  return ecd != 0 ? ecd : EXIT_FAILURE;
}

void SetMemoryTypes() {
  memtype[  0] = USHORT;
  memtype[  2] = UCHAR;
  memtype[  3] = UCHAR;
  memtype[  4] = SHORT;
  memtype[  6] = SHORT;
  memtype[ 10] = SHORT;
  memtype[ 12] = SHORT;
  memtype[ 14] = SHORT;
  memtype[ 16] = DOUBLE;
  memtype[ 24] = DOUBLE;
  memtype[ 32] = DOUBLE;
  memtype[ 40] = DOUBLE;
  memtype[ 48] = DOUBLE;
  memtype[ 56] = DOUBLE;
  memtype[ 64] = DOUBLE;
  memtype[ 72] = DOUBLE;
  memtype[ 80] = DOUBLE;
  memtype[ 88] = LONG;
  memtype[ 96] = LONG;
  memtype[104] = SHORT;
  memtype[106] = USHORT;
  memtype[108] = USHORT;
  memtype[110] = USHORT;
  memtype[112] = USHORT;
  memtype[114] = USHORT;
  memtype[116] = CHAR;
  memtype[117] = CHAR;
  memtype[118] = CHAR;
  memtype[119] = CHAR;
  memtype[120] = DOUBLE;
  memtype[128] = DOUBLE;
  memtype[136] = DOUBLE;
  memtype[144] = DOUBLE;
  memtype[152] = DOUBLE;
  memtype[160] = DOUBLE;
  memtype[168] = DOUBLE;
  memtype[176] = DOUBLE;
  memtype[184] = USHORT;
  memtype[186] = UCHAR;
  memtype[187] = UCHAR;
  memtype[188] = USHORT;
  memtype[190] = USHORT;
  memtype[192] = ULONG;
  memtype[200] = ULONG;
  memtype[208] = ULONG;
  memtype[216] = DOUBLE;
  memtype[224] = DOUBLE;
  memtype[232] = DOUBLE;
  memtype[240] = DOUBLE;
  memtype[248] = DOUBLE;
  memtype[256] = DOUBLE;
  memtype[264] = DOUBLE;
  memtype[272] = DOUBLE;
  memtype[280] = DOUBLE;
  memtype[288] = DOUBLE;
  memtype[296] = DOUBLE;
  memtype[304] = DOUBLE;
  memtype[312] = DOUBLE;
  memtype[320] = DOUBLE;
  memtype[328] = DOUBLE;
  memtype[336] = DOUBLE;
  memtype[344] = DOUBLE;
  memtype[352] = DOUBLE;
}

int BB2D_Init() {
  if (ctx != NULL) return BB2D_Error(0, "Beauto Balancer 2 Driver has already been initialized.");

  int res = libusb_init(&ctx);
  if (res < 0) return BB2D_Error(res, "USB init error.");
  libusb_set_debug(ctx, 3);

  int i;
  for (i = 0; i < REPORT_SIZE; i++) report[i] = 0;
  for (i = 0; i < MEMORY_SIZE; i++) {
    memmap[i] = 0;
    memtype[i] = NONE;
  }
  SetMemoryTypes();
  dev_handle = NULL;

  return EXIT_SUCCESS;
}

int BB2D_Connect() {
  if (ctx == NULL) return BB2D_Error(0, "Beauto Balancer 2 Driver has not been initialized.");
  if (dev_handle != NULL) return BB2D_Error(0, "Connection has already been established.");

  libusb_device **devs;
  int res = libusb_get_device_list(ctx, &devs);
  if (res < 0) return BB2D_Error(res, "Get device error.");

  dev_handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
  if (dev_handle == NULL) return BB2D_Error(0, "Cannot open device.");
  libusb_free_device_list(devs, 1);

  if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
    fprintf(stderr, "Kernel driver active.\n");
    if (libusb_detach_kernel_driver(dev_handle, 0) == 0)
      fprintf(stderr, "Kernel driver detached.\n");
  }

  res = libusb_claim_interface(dev_handle, 0);
  if (res < 0) return BB2D_Error(res, "Cannot claim interface.");

  fprintf(stderr, "Connect.\n");
  return EXIT_SUCCESS;
}

int BB2D_Disconnect() {
  if (ctx == NULL) return BB2D_Error(0, "Beauto Balancer 2 Driver has not been initialized.");
  if (dev_handle == NULL) return BB2D_Error(0, "Connection has not been established.");

  int res = libusb_release_interface(dev_handle, 0);
  if (res != 0) return BB2D_Error(res, "Cannot release interface.");

  libusb_close(dev_handle);
  dev_handle = NULL;

  fprintf(stderr, "Disconnect.\n");
  return EXIT_SUCCESS;
}

int BB2D_Exit() {
  if (ctx == NULL) return BB2D_Error(0, "Beauto Balancer 2 Driver has not been initialized.");
  
  int res;
  if (dev_handle != NULL) {
    res = BB2D_Disconnect();
    if (res) return BB2D_Error(res, "Cannot disconnected.");
  }
  
  libusb_exit(ctx);
  ctx = NULL;

  return EXIT_SUCCESS;
}

#define DATA_MAX 60 //byte
int BB2D_Read(const unsigned short addr, const unsigned short size) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");
  
  if (addr > 352 || addr + size > 360) return BB2D_Error(0, "Out of range.");

  unsigned int a = addr;
  unsigned int s = size;
  int act, res, i;
  while (s > 0) {
    report[REPORT_ADDR_CMD] = (byte)CMD_READ;
    ushort2byte(report, REPORT_ADDR_SEND_TRGT, a);
    report[REPORT_ADDR_SEND_SIZE] = (byte)(s > DATA_MAX ? DATA_MAX : s);

    res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
    if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");

    res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
    if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

    act = report[REPORT_ADDR_RECV_SIZE];
    for (i = 0; i < act; i++) memmap[a + i] = report[REPORT_ADDR_RECV_BODY + i];
    a += act;
    s -= act;
  }

  return EXIT_SUCCESS;
}

char BB2D_GetChar(const unsigned short addr) {
  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != CHAR) return BB2D_Error(0, "Invalid data type.");
  return (char)memmap[addr];
}

unsigned char BB2D_GetUChar(const unsigned short addr) {
  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != UCHAR) return BB2D_Error(0, "Invalid data type.");
  return memmap[addr];
}

short BB2D_GetShort(const unsigned short addr) {
  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != SHORT) return BB2D_Error(0, "Invalid data type.");
  return byte2short(memmap, addr);
}

unsigned short BB2D_GetUShort(const unsigned short addr) {
  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != USHORT) return BB2D_Error(0, "Invalid data type.");
  return byte2ushort(memmap, addr);
}

long BB2D_GetLong(const unsigned short addr) {
  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != LONG) return BB2D_Error(0, "Invalid data type.");
  return byte2long(memmap, addr);
}

unsigned long BB2D_GetULong(const unsigned short addr) {
  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != ULONG) return BB2D_Error(0, "Invalid data type.");
  return byte2ulong(memmap, addr);
}

double BB2D_GetDouble(const unsigned short addr) {
  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != DOUBLE) return BB2D_Error(0, "Invalid data type.");
  return byte2double(memmap, addr);
}

int BB2D_ReadChar (const unsigned short addr, char* out) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != CHAR) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_READ;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(char);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");
  
  *out = (char)report[REPORT_ADDR_RECV_BODY];

  return EXIT_SUCCESS;
}

int BB2D_ReadUChar (const unsigned short addr, unsigned char* out) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != UCHAR) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_READ;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(char);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");
  
  *out = report[REPORT_ADDR_RECV_BODY];

  return EXIT_SUCCESS;
}

int BB2D_ReadShort (const unsigned short addr, short* out) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != SHORT) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_READ;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(short);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");
  
  *out = byte2short(report, REPORT_ADDR_RECV_BODY);

  return EXIT_SUCCESS;
}

int BB2D_ReadUShort(const unsigned short addr, unsigned short* out) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != USHORT) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_READ;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(unsigned short);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");
  
  *out = byte2ushort(report, REPORT_ADDR_RECV_BODY);

  return EXIT_SUCCESS;
}

int BB2D_ReadLong(const unsigned short addr, long* out) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != LONG) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_READ;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(long);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");
  
  *out = byte2long(report, REPORT_ADDR_RECV_BODY);

  return EXIT_SUCCESS;
}

int BB2D_ReadULong(const unsigned short addr, unsigned long* out) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != ULONG) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_READ;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(unsigned long);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");
  
  *out = byte2ulong(report, REPORT_ADDR_RECV_BODY);

  return EXIT_SUCCESS;
}

int BB2D_ReadDouble(const unsigned short addr, double* out) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != DOUBLE) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_READ;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(double);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");
  
  *out = byte2double(report, REPORT_ADDR_RECV_BODY);

  return EXIT_SUCCESS;
}
                                            
int BB2D_WriteChar(const unsigned short addr, const char src) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != CHAR) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_WRITE;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(char);
  report[REPORT_ADDR_SEND_BODY] = (byte)src;

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

  return EXIT_SUCCESS;
}

int BB2D_WriteUChar(const unsigned short addr, const unsigned char src) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != UCHAR) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_WRITE;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(unsigned char);
  report[REPORT_ADDR_SEND_BODY] = src;

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

  return EXIT_SUCCESS;
}

int BB2D_WriteShort(const unsigned short addr, const short src) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != SHORT) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_WRITE;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(short);
  short2byte(report, REPORT_ADDR_SEND_BODY, src);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

  return EXIT_SUCCESS;
}

int BB2D_WriteUShort(const unsigned short addr, const unsigned short src) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != USHORT) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_WRITE;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(unsigned short);
  ushort2byte(report, REPORT_ADDR_SEND_BODY, src);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

  return EXIT_SUCCESS;
}

int BB2D_WriteLong(const unsigned short addr, const long src) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != LONG) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_WRITE;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(long);
  long2byte(report, REPORT_ADDR_SEND_BODY, src);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

  return EXIT_SUCCESS;
}

int BB2D_WriteULong(const unsigned short addr, const unsigned long src) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != ULONG) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_WRITE;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(unsigned long);
  ulong2byte(report, REPORT_ADDR_SEND_BODY, src);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

  return EXIT_SUCCESS;
}

int BB2D_WriteDouble(const unsigned short addr, const double src) {
  if (ctx == NULL) return BB2D_Error(0, "Connection has not been established.");

  if (addr >= MEMORY_SIZE) return BB2D_Error(0, "Out of range.");
  if (memtype[addr] != DOUBLE) return BB2D_Error(0, "Invalid data type.");

  report[REPORT_ADDR_CMD] = (byte)CMD_WRITE;
  ushort2byte(report, REPORT_ADDR_SEND_TRGT, addr);
  report[REPORT_ADDR_SEND_SIZE] = (byte)sizeof(double);
  double2byte(report, REPORT_ADDR_SEND_BODY, src);

  int res, act;
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Writing failed.");
  res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
  if (res != 0 || act != REPORT_SIZE) return BB2D_Error(res, "Reading failed.");

  return EXIT_SUCCESS;
}
