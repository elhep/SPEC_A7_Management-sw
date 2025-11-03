/*
This code comes from https://github.com/derekmulcahy/xvcpi/blob/master/xvcpi.c
but the GPIO control parts have been modified to make it work on IMX93

TODO: update pin numbers
*/

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>

#include "gpio.c"

// #define IMX93_PERI_BASE      0x10000000
// #define IMX93_PERI_SIZE      0x1000

// #define IMX93_GPIO_OFFSET    0x600
// #define IMX93_GPIO_DATA_0    0x20
// #define IMX93_GPIO_DSET_0    0x30
// #define IMX93_GPIO_DCLR_0    0x40

/* GPIO setup macros */
// #define MODE_GPIO(g) (*(pio_base+((g)/10))>>(((g)%10)*3) & 7)
// #define INP_GPIO(g) do { *(pio_base+((g)/10)) &= ~(7<<(((g)%10)*3)); } while (0)
// #define SET_MODE_GPIO(g, m) do { /* clear the mode bits first, then set as necessary */ \
//       INP_GPIO(g);						\
//       *(pio_base+((g)/10)) |=  ((m)<<(((g)%10)*3)); } while (0)
// #define OUT_GPIO(g) SET_MODE_GPIO(g, 1)

// #define GPIO_SET (*(pio_base + (IMX93_GPIO_OFFSET >> 2) + (IMX93_GPIO_DSET_0 >> 2)))  /* sets   bits which are 1, ignores bits which are 0 */
// #define GPIO_CLR (*(pio_base + (IMX93_GPIO_OFFSET >> 2) + (IMX93_GPIO_DCLR_0 >> 2))) /* clears bits which are 1, ignores bits which are 0 */
// #define GPIO_LEV (*(pio_base + (IMX93_GPIO_OFFSET >> 2) + (IMX93_GPIO_DATA_0 >> 2))) /* current level of the pin */

// static int dev_mem_fd;
// static volatile uint32_t *pio_base;

static bool     imx93gpio_init(void);
static int      imx93gpio_read(void);
static void     imx93gpio_write(int tck, int tms, int tdi);
static uint32_t imx93gpio_xfer(int n, uint32_t tms, uint32_t tdi);

// /* GPIO numbers for each signal. Negative values are invalid */
// static int tms_gpio = 11;
// static int tdo_gpio = 19;
// static int tdi_gpio = 20;
// static int tck_gpio = 21;
// static int trst_gpio = 2;  // not used

static const struct gpio_config TCK_GPIO = { 0, "/dev/gpiochip0", "jtag_tck"};
static const struct gpio_config TDI_GPIO = { 1, "/dev/gpiochip0", "jtag_tdi"};
static const struct gpio_config TDO_GPIO = { 14, "/dev/gpiochip0", "jtag_tdo"};
static const struct gpio_config TMS_GPIO = { 15, "/dev/gpiochip0", "jtag_tms"};

struct gpiod_line_request *tck_request, *tdi_request, *tdo_request, *tms_request;

static int verbose = 0;

/* Transition delay coefficients */
#define JTAG_DELAY (40)
static unsigned int jtag_delay = JTAG_DELAY;

// ********************************************************************
// fucntions for imx93gpio ********************************************
// ********************************************************************

static int imx93gpio_read(void)
{  
   enum gpiod_line_value value = gpiod_line_request_get_value(tdo_request, TDO_GPIO.pin);

   if(value == GPIOD_LINE_VALUE_ACTIVE)
      return 1;
   else
      return 0;

   // return !!(GPIO_LEV & 1<<tdo_gpio);
}

static void imx93gpio_write(int tck, int tms, int tdi)
{
   // uint32_t set = tck<<tck_gpio | tms<<tms_gpio | tdi<<tdi_gpio;
   // uint32_t clear = !tck<<tck_gpio | !tms<<tms_gpio | !tdi<<tdi_gpio;

   // GPIO_SET = set;
   // GPIO_CLR = clear;
   gpiod_line_request_set_value(tdi_request, TDI_GPIO.pin, tdi ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
   gpiod_line_request_set_value(tms_request, TMS_GPIO.pin, tms ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
   gpiod_line_request_set_value(tck_request, TCK_GPIO.pin, tck ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);

   for (unsigned int i = 0; i < jtag_delay; i++)
      asm volatile ("");
}

static uint32_t imx93gpio_xfer(int n, uint32_t tms, uint32_t tdi)
{
   uint32_t tdo = 0;

   for (int i = 0; i < n; i++) {
      imx93gpio_write(0, tms & 1, tdi & 1);
      imx93gpio_write(1, tms & 1, tdi & 1);
      tdo |= imx93gpio_read() << i;
      tms >>= 1;
      tdi >>= 1;
   }
   return tdo;
}

static bool imx93gpio_init(void)
{
//    dev_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
//    if (dev_mem_fd < 0) {
//       perror("open");
//       return false;
//    }

//    if (verbose) {
//       printf("address=%08x size=%08x\n", IMX93_PERI_BASE, IMX93_PERI_SIZE);
//    }

//    pio_base = mmap(NULL, IMX93_PERI_SIZE, PROT_READ | PROT_WRITE,
//             MAP_SHARED, dev_mem_fd, IMX93_PERI_BASE);

//    if (pio_base == MAP_FAILED) {
//       perror("mmap");
//       close(dev_mem_fd);
//       return false;
//    }

// //    /*
// //     * Configure TDO as an input, and TDI, TCK, TMS
// //     * as outputs.  Drive TDI and TCK low, and TMS high.
// //     */

   tck_request = request_output_line(TCK_GPIO.chip, TCK_GPIO.pin, GPIOD_LINE_VALUE_INACTIVE, TCK_GPIO.consumer);
	if (!tck_request) {
		fprintf(stderr, "failed to request TCK line: %s\n",
			strerror(errno));
		return EXIT_FAILURE;
	}
	tdi_request = request_output_line(TDI_GPIO.chip, TDI_GPIO.pin, GPIOD_LINE_VALUE_INACTIVE, TDI_GPIO.consumer);
	if (!tdi_request) {
		fprintf(stderr, "failed to request TDI line: %s\n",
			strerror(errno));
		return EXIT_FAILURE;
   }
   tms_request = request_output_line(TMS_GPIO.chip, TMS_GPIO.pin, GPIOD_LINE_VALUE_ACTIVE, TMS_GPIO.consumer);
   if (!tms_request) {
      fprintf(stderr, "failed to request TMS line: %s\n",
         strerror(errno));
      return EXIT_FAILURE;
   }
   tdo_request = request_input_line(TDO_GPIO.chip, TDO_GPIO.pin, TDO_GPIO.consumer);
   if (!tdo_request) {
      fprintf(stderr, "failed to request TDO line: %s\n",
         strerror(errno));
      return EXIT_FAILURE;
   }

// //    INP_GPIO(tdo_gpio);

//    GPIO_CLR = 1<<tdi_gpio | 1<<tck_gpio;
//    GPIO_SET = 1<<tms_gpio;

//    // OUT_GPIO(tdi_gpio);
//    // OUT_GPIO(tck_gpio);
//    // OUT_GPIO(tms_gpio);

   imx93gpio_write(0, 1, 0);

   return true;
}

static int sread(int fd, void *target, int len) {
   unsigned char *t = target;
   while (len) {
      int r = read(fd, t, len);
      if (r <= 0)
         return r;
      t += r;
      len -= r;
   }
   return 1;
}

int handle_data(int fd) {
   const char xvcInfo[] = "xvcServer_v1.0:2048\n";

   do {
      char cmd[16];
      unsigned char buffer[2048], result[1024];
      memset(cmd, 0, 16);

      if (sread(fd, cmd, 2) != 1)
         return 1;

      if (memcmp(cmd, "ge", 2) == 0) {
         if (sread(fd, cmd, 6) != 1)
            return 1;
         memcpy(result, xvcInfo, strlen(xvcInfo));
         if (write(fd, result, strlen(xvcInfo)) != strlen(xvcInfo)) {
            perror("write");
            return 1;
         }
         if (verbose) {
            printf("%u : Received command: 'getinfo'\n", (int)time(NULL));
            printf("\t Replied with %s\n", xvcInfo);
         }
         break;
      } else if (memcmp(cmd, "se", 2) == 0) {
         if (sread(fd, cmd, 9) != 1)
            return 1;
         memcpy(result, cmd + 5, 4);
         if (write(fd, result, 4) != 4) {
            perror("write");
            return 1;
         }
         if (verbose) {
            printf("%u : Received command: 'settck'\n", (int)time(NULL));
            printf("\t Replied with '%.*s'\n\n", 4, cmd + 5);
         }
         break;
      } else if (memcmp(cmd, "sh", 2) == 0) {
         if (sread(fd, cmd, 4) != 1)
            return 1;
         if (verbose) {
            printf("%u : Received command: 'shift'\n", (int)time(NULL));
         }
      } else {

         fprintf(stderr, "invalid cmd '%s'\n", cmd);
         return 1;
      }

      int len;
      if (sread(fd, &len, 4) != 1) {
         fprintf(stderr, "reading length failed\n");
         return 1;
      }

      int nr_bytes = (len + 7) / 8;
      if (nr_bytes * 2 > sizeof(buffer)) {
         fprintf(stderr, "buffer size exceeded\n");
         return 1;
      }

      if (sread(fd, buffer, nr_bytes * 2) != 1) {
         fprintf(stderr, "reading data failed\n");
         return 1;
      }
      memset(result, 0, nr_bytes);

      if (verbose) {
         printf("\tNumber of Bits  : %d\n", len);
         printf("\tNumber of Bytes : %d \n", nr_bytes);
         printf("\n");
      }

      imx93gpio_write(0, 1, 1);

      int bytesLeft = nr_bytes;
      int bitsLeft = len;
      int byteIndex = 0;
      uint32_t tdi, tms, tdo;

      while (bytesLeft > 0) {
         tms = 0;
         tdi = 0;
         tdo = 0;
         if (bytesLeft >= 4) {
            memcpy(&tms, &buffer[byteIndex], 4);
            memcpy(&tdi, &buffer[byteIndex + nr_bytes], 4);

            tdo = imx93gpio_xfer(32, tms, tdi);
            memcpy(&result[byteIndex], &tdo, 4);

            bytesLeft -= 4;
            bitsLeft -= 32;
            byteIndex += 4;

            if (verbose) {
               printf("LEN : 0x%08x\n", 32);
               printf("TMS : 0x%08x\n", tms);
               printf("TDI : 0x%08x\n", tdi);
               printf("TDO : 0x%08x\n", tdo);
            }

         } else {
            memcpy(&tms, &buffer[byteIndex], bytesLeft);
            memcpy(&tdi, &buffer[byteIndex + nr_bytes], bytesLeft);

            tdo = imx93gpio_xfer(bitsLeft, tms, tdi);
            memcpy(&result[byteIndex], &tdo, bytesLeft);

            bytesLeft = 0;

            if (verbose) {
               printf("LEN : 0x%08x\n", bitsLeft);
               printf("TMS : 0x%08x\n", tms);
               printf("TDI : 0x%08x\n", tdi);
               printf("TDO : 0x%08x\n", tdo);
            }
            break;
         }
      }

      imx93gpio_write(0, 1, 0);

      if (write(fd, result, nr_bytes) != nr_bytes) {
         perror("write");
         return 1;
      }

   } while (1);
   /* Note: Need to fix JTAG state updates, until then no exit is allowed */
   return 0;
}

int main(int argc, char **argv) {
   int i;
   int s;
   int c;

   struct sockaddr_in address;

   opterr = 0;

   while ((c = getopt(argc, argv, "vd:")) != -1) {
      switch (c) {
      case 'v':
         verbose = 1;
         break;
      case 'd':
         jtag_delay = atoi(optarg);
         if (jtag_delay < 0)
             jtag_delay = JTAG_DELAY;
         break;
      case '?':
         fprintf(stderr, "usage: %s [-v]\n", *argv);
         return 1;
      }
   }
   if (verbose)
      printf("jtag_delay=%d\n", jtag_delay);

   if (!imx93gpio_init()) {
      fprintf(stderr,"Failed in imx93gpio_init()\n");
      return -1;
   }

   s = socket(AF_INET, SOCK_STREAM, 0);

   if (s < 0) {
      perror("socket");
      return 1;
   }

   i = 1;
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &i, sizeof i);

   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(2542);
   address.sin_family = AF_INET;

   if (bind(s, (struct sockaddr*) &address, sizeof(address)) < 0) {
      perror("bind");
      return 1;
   }

   if (listen(s, 0) < 0) {
      perror("listen");
      return 1;
   }

   fd_set conn;
   int maxfd = 0;

   FD_ZERO(&conn);
   FD_SET(s, &conn);

   maxfd = s;

   while (1) {
      fd_set read = conn, except = conn;
      int fd;

      if (select(maxfd + 1, &read, 0, &except, 0) < 0) {
         perror("select");
         break;
      }

      for (fd = 0; fd <= maxfd; ++fd) {
         if (FD_ISSET(fd, &read)) {
            if (fd == s) {
               int newfd;
               socklen_t nsize = sizeof(address);

               newfd = accept(s, (struct sockaddr*) &address, &nsize);

               if (verbose)
                  printf("connection accepted - fd %d\n", newfd);
               if (newfd < 0) {
                  perror("accept");
               } else {
                 int flag = 1;
                 int optResult = setsockopt(newfd,
                                               IPPROTO_TCP,
                                               TCP_NODELAY,
                                               (char *)&flag,
                                               sizeof(int));
                 if (optResult < 0)
                    perror("TCP_NODELAY error");
                  if (newfd > maxfd) {
                     maxfd = newfd;
                  }
                  FD_SET(newfd, &conn);
               }
            }
            else if (handle_data(fd)) {

               if (verbose)
                  printf("connection closed - fd %d\n", fd);
               close(fd);
               FD_CLR(fd, &conn);
            }
         }
         else if (FD_ISSET(fd, &except)) {
            if (verbose)
               printf("connection aborted - fd %d\n", fd);
            close(fd);
            FD_CLR(fd, &conn);
            if (fd == s)
               break;
         }
      }
   }
   return 0;
}

/*
 * This work, "xvcpi.c", is a derivative of "xvcServer.c" (https://github.com/Xilinx/XilinxVirtualCable)
 * by Avnet and is used by Xilinx for XAPP1251.
 *
 * "xvcServer.c" is licensed under CC0 1.0 Universal (http://creativecommons.org/publicdomain/zero/1.0/)
 * by Avnet and is used by Xilinx for XAPP1251.
 *
 * "xvcServer.c", is a derivative of "xvcd.c" (https://github.com/tmbinc/xvcd)
 * by tmbinc, used under CC0 1.0 Universal (http://creativecommons.org/publicdomain/zero/1.0/).
 *
 * Portions of "xvcpi.c" are derived from OpenOCD (http://openocd.org)
 *
 * "xvcpi.c" is licensed under CC0 1.0 Universal (http://creativecommons.org/publicdomain/zero/1.0/)
 * by Derek Mulcahy.*
 */
