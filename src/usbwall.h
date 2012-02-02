/**
 * \file usbwall.h
 * \brief structure to store information of mass storages
 * \author David FERNANDES
 * \version 0.1
 * \date 10 january 2012
 *
 * File usbwall.h for project usbwall
 *
 * LACSC - ECE PARIS Engineering school 
 *
 * Made by David FERNANDES
 * Login   <dfernand@ece.fr>
 *
 * Started on  Mon Jan  9 9:58:47 2012 David FERNANDES
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

#ifndef USBWALL_H_
#define USBWALL_H_

#define USBWALL_MAJOR 0
#define USBWALL_MEDIUM 2
#define USBWALL_CURRENT 1

#define USBWALL_MODVERSION "0.2.1"

/*
** define the ioctl magic number
*/
# define USBWALL_IOC_MAGIC		'u'

# define USBWALL_ADDKEY		_IOW(USBWALL_IOC_MAGIC, 0, long) /* pointer */
# define USBWALL_DELKEY		_IOW(USBWALL_IOC_MAGIC, 1, long) /* pointer */

enum keyflags
{
  USBWALL_KEY_ADD = 0x1 << 0,
  USBWALL_KEY_DEL = 0x1 << 1,
  USBWALL_KEY_ACCESS_READ = 0x1 << 2,
  USBWALL_KEY_ACCESS_WRITE = 0x1 << 3,
  USBWALL_KEY_ACCESS_EXEC = 0x1 << 4,
  USBWALL_KEY_ACCESS_SETUID = 0x1 << 5,
  USBWALL_KEY_ACCESS_SETGID = 0x1 << 6
};

typedef enum keyflags keyflags_t;

/**
 * \struct mass_storage_info
 *
 * mass storage indentification
 */
struct usbwall_token_info
{
  keyflags_t keyflags;
  uint16_t idVendor;
  uint16_t idProduct;
  char idSerialNumber[32];
};

union procfs_info
{
  struct usbwall_token_info info;
  char   buffer[64];
};

typedef union procfs_info procfs_info_t;

#endif /*!USBWALL_H_*/
