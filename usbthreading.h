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


#ifndef DEF_USBTMCTHREAD_H
#define DEF_USBTMCTHREAD_H


#include <QObject>
#include <QThread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "global.h"
#include "tmc_dev.h"



class usbtmcThread : public QThread
{
  Q_OBJECT

public:

  int tmcdev_msg(struct tmcdev *, const char *, int);
  int tmcdev_getscreendata(struct tmcdev *, struct device_settings *);


signals:
  void usbtmcReady();
  void usbtmcScrnReady();

private:

  int cmd_rw, status, tx_stat, rx_stat, job;

  char cmd_str[1024];

  struct tmcdev *device;

  struct device_settings *devparms;

  void run();
};


#endif





