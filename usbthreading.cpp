/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2015 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/


#include "usbthreading.h"




void usbtmcThread::run()
{
  int i, j, n=0;

  char str[128];

  if(job == 1)
  {
    msleep(20);

    tx_stat = tmcdev_write(device, cmd_str);

    msleep(20);

    if(cmd_rw)
    {
      rx_stat = tmcdev_read(device);
    }

    emit usbtmcReady();
  }
  else if(job == 2)
    {
      for(i=0; i<MAX_CHNS; i++)
      {
        if(!devparms->chandisplay[i])  // Download data only when channel is switched on
        {
          continue;
        }

        msleep(10);

        sprintf(str, ":WAV:SOUR CHAN%i", i + 1);

        tmcdev_write(device, str);

        msleep(10);

        tmcdev_write(device, ":WAV:FORM BYTE");

        msleep(10);

        tmcdev_write(device, ":WAV:MODE NORM");

        msleep(10);

        tmcdev_write(device, ":WAV:DATA?");

        msleep(10);

        n = tmcdev_read(device);

        if(n < 0)
        {
          printf("Can not read from device.\n");
          return;
        }

        if(n > WAVFRM_MAX_BUFSZ)
        {
          printf("Datablock too big for buffer.");
          return;
        }

        if(n < 16)
        {
          return;
        }

        for(j=0; j<n; j++)
        {
          devparms->wavebuf[i][j] = (int)(((unsigned char *)device->buf)[j]) - 127;
        }
      }

      devparms->wavebufbufsz = n;

      emit usbtmcScrnReady();
    }
}


int usbtmcThread::tmcdev_msg(struct tmcdev *dev, const char *cmd, int rw)
{
  job = 1;

  cmd_rw = rw;

  strcpy(cmd_str, cmd);

  device = dev;

  run();

  return 0;
}


int usbtmcThread::tmcdev_getscreendata(struct tmcdev *dev, struct device_settings *dev_parms)
{
  job = 2;

  devparms = dev_parms;

  device = dev;

  run();

  return 0;
}


//   usbworkerThread = new usbtmcThread(this);
//
//   connect(usbworkerThread, SIGNAL(usbtmcReady()), this, SLOT(usbtmcReadyHandle()));
//
//   usbworkerThread->start();












