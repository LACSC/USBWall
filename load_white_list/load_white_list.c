/**
 * \file load_white_list.c
 * \brief load the white list (user space to kernel space)
 * \author David FERNANDES
 * \version 0.1
 * \date 10 january 2012
 *
 * File pendrivesinfo.c for project usbwall
 *
 * Made by David FERNANDES
 * Login   <dfernand@ece.fr>
 *
 * Started on  Tue Jan  10 14:49:06 2012 David FERNANDES
 *
 * Copyright (C) 2012 - David FERNANDES
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include "mass_storage_info.h"

int main()
{
  /* variables */
  FILE *fd_whitelist;
  int fd_chardev, line_number, i, tmp_idProduct, tmp_idVendor;
  char buffer[256], tmp_idSerialNumber[32];
  /* File containing pen drives informations : white list */ 
  char *fileinfo = "whitelist";   

  /* opening file containing pen drives informations */ 
  fd_whitelist = fopen(fileinfo, "rb");
  if(fd_whitelist == NULL)
  {
    fprintf(stderr, "Cannot open whitelist file\n");
    return 1;
  }

  /* reading file containing pen drives informations */
  line_number = 0;
  while(fgets(buffer,255,fd_whitelist)) 
  {
    line_number++;                                   
  }
  fseek(fd_whitelist, 0, SEEK_SET);
  struct mass_storage_info infopendrive[line_number];
  for(i=0;i<line_number;i++)
  {
    fscanf(fd_whitelist,"%d %d %s", &tmp_idVendor, &tmp_idProduct, infopendrive[i].idSerialNumber);
    infopendrive[i].idVendor = tmp_idVendor;       
    infopendrive[i].idProduct = tmp_idProduct;
    printf("Mass storage : %x(hex) : %x(hex) : %s : is loaded in the white list\n", infopendrive[i].idVendor, infopendrive[i].idProduct, infopendrive[i].idSerialNumber);
  }
  
  /* closing file containing pen drives informations */
  fclose (fd_whitelist);
  
  /* communication between user space and kernel space */
  /* opening char device */
  fd_chardev = open("/dev/usbwall_chardev", O_RDONLY);
  if(fd_chardev == -1)
  {
    fprintf(stderr, "Cannot open the char device\n");
    exit(1);
  }

  /* ioctl char device : passing white list from user space to kernel space */
  ioctl(fd_chardev,IO_SET_PENDRIVE, infopendrive);

  /* closing char device */
  close (fd_chardev);
  
  return 0;
}

