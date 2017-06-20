#include <stdio.h>
#include <stdlib.h>
#include <libusb.h>

#include "bb2d.h"
#include "byteconverter.h"

typedef struct libusb_context       libusb_context;
typedef struct libusb_device        libusb_device;
typedef struct libusb_device_handle libusb_device_handle;

byte report[REPORT_SIZE];

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
