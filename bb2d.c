#include <stdio.h>
#include <stdlib.h>
#include <libusb.h>

#include "bb2d.h"
#include "byteconverter.h"

typedef struct libusb_context       libusb_context;
typedef struct libusb_device        libusb_device;
typedef struct libusb_device_handle libusb_device_handle;

byte report[REPORT_SIZE];
byte memmap[MEMORY_SIZE];

libusb_context       *ctx = NULL;
libusb_device_handle *dev_handle;

int error(const int ecd, const char *msg) {
  if (ecd != 0) perror(libusb_error_name(ecd));
  perror(msg);
  return ecd != 0 ? ecd : EXIT_FAILURE;
}

int BB2D_Connect() {
  if (ctx != NULL) return error(0, "Connection has been established.");

  int res = libusb_init(&ctx);
  if (res < 0) return error(res, "USB init error.");
  libusb_set_debug(ctx, 3);

  libusb_device **devs;
  res = libusb_get_device_list(ctx, &devs);
  if (res < 0) return error(res, "Get device error.");

  dev_handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
  if (dev_handle == NULL) return error(0, "Cannot open device.");
  libusb_free_device_list(devs, 1);

  if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
    fprintf(stderr, "Kernel driver active.\n");
    if (libusb_detach_kernel_driver(dev_handle, 0) == 0)
      fprintf(stderr, "Kernel driver detached.\n");
  }

  res = libusb_claim_interface(dev_handle, 0);
  if (res < 0) return error(res, "Cannot claim interface.");

  fprintf(stderr, "Connect.\n");
  return EXIT_SUCCESS;
}

int BB2D_Disconnect() {
  if (ctx == NULL) return error(0, "Connection has not been established.");

  int res = libusb_release_interface(dev_handle, 0);
  if (res != 0) return error(res, "Cannot release interface.");

  libusb_close(dev_handle);
  libusb_exit(ctx);
  ctx = NULL;

  fprintf(stderr, "Disconnect.\n");
  return EXIT_SUCCESS;
}

#define DATA_MAX 60 //byte
int BB2D_Read(const unsigned short addr, const unsigned short size) {
  if (ctx == NULL) return error(0, "Connection has not been established.");
  
  if (addr > 352 || addr + size > 360) return error(0, "Out of range.");

  unsigned int a = addr;
  unsigned int s = size;
  int act, res, i;
  while (s > 0) {
    report[0] = (byte)CMD_READ;
    ushort2byte(report, 1, a);
    report[3] = (byte)(s > DATA_MAX ? DATA_MAX : s);

    res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_OUT), report, REPORT_SIZE, &act, TIME_OUT);
    if (res != 0 || act != REPORT_SIZE) return error(res, "Writing failed.");

    res = libusb_interrupt_transfer(dev_handle, (BB2_EP | LIBUSB_ENDPOINT_IN), report, REPORT_SIZE, &act, TIME_OUT);
    if (res != 0 || act != REPORT_SIZE) return error(res, "Reading failed.");

    act = report[1];
    for (i = 0; i < act; i++) memmap[a + i] = report[2 + i];
    a += act;
    s -= act;
  }

  return EXIT_SUCCESS;
}

char BB2D_GetChar(const unsigned short addr) {
  if (addr > 360) return error(0, "Out of range.");
  return (char)memmap[addr];
}

unsigned char BB2D_GetUChar(const unsigned short addr) {
  if (addr > 360) return error(0, "Out of range.");
  return memmap[addr];
}

short BB2D_GetShort(const unsigned short addr) {
  if (addr > 358) return error(0, "Out of range.");
  return byte2short(memmap, addr);
}

unsigned short BB2D_GetUShort(const unsigned short addr) {
  if (addr > 358) return error(0, "Out of range.");
  return byte2ushort(memmap, addr);
}

int BB2D_GetInt(const unsigned short addr){
  if (addr > 356) return error(0, "Out of range.");
  return byte2int(memmap, addr);
}

unsigned int BB2D_GetUInt(const unsigned short addr){
  if (addr > 356) return error(0, "Out of range.");
  return byte2uint(memmap, addr);
}

long BB2D_GetLong(const unsigned short addr) {
  if (addr > 352) return error(0, "Out of range.");
  return byte2long(memmap, addr);
}

unsigned long BB2D_GetULong(const unsigned short addr) {
  if (addr > 352) return error(0, "Out of range.");
  return byte2ulong(memmap, addr);
}

float BB2D_GetFloat(const unsigned short addr) {
  if (addr > 356) return error(0, "Out of range.");
  return byte2float(memmap, addr);
}

double BB2D_GetDouble(const unsigned short addr) {
  if (addr > 352) return error(0, "Out of range.");
  return byte2double(memmap, addr);
}
