/**
 * \file usbwall_mod.c
 * \brief module for USB mass-storage device filtering
 * \author David FERNANDES
 * \version 0.1
 * \date 19 december 2011
 *
 * File usbwall_mod.c for project usbwall
 * 
 * LACSC - ECE PARIS Engineering school 
 *
 * Made by David FERNANDES
 * Login   <dfernand@ece.fr>
 *
 * Started on  Mon Dec  19 9:17:29 2011 David FERNANDES
 *
 * Copyright (C) 2011 - David FERNANDES
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

/* Headers */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/usb.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/usb/ch9.h>
#include "trace.h"
#include "usbwall.h"
#include "keylist.h"
#include "procfs_iface.h"
#include "keylist_info.h"
#include "usbwall_chrdev.h"

/* Module informations */
MODULE_AUTHOR ("David FERNANDES");
MODULE_DESCRIPTION ("Module for USB mass-storage device filtering");
MODULE_LICENSE ("GPL");
MODULE_VERSION (USBWALL_MODVERSION);


enum authmode {
  USBWALL_AUTH_EVENT = 0,
  USBWALL_AUTH_LIST,
  USBWALL_AUTH_MAX
};

short authmode = USBWALL_AUTH_LIST;

module_param(authmode, short, 0640);
MODULE_PARM_DESC(authmode, "Module device authentication method: 0 for event based (ask for userspace answer), 1 for list based (internal device list)");

/**
 * \struct usb_device_id usbwall_id_table []
 *
 * Devices supported by the module : All mass storage
 */
static struct usb_device_id usbwall_id_table[] = {
        {.driver_info = 42, .bDeviceClass = USB_CLASS_MASS_STORAGE},
        {}
};

MODULE_DEVICE_TABLE (usb, usbwall_id_table);

/* kernel's representation of a USB device */
static struct usb_device *dev;

/* my variables */
static char idSerialNumber[32] = "";
static struct internal_token_info my_device;
static int usbwall_register;

/** 
 * \fn usbwall_probe
 * \param *intf usb_interface
 * \param *devid usb_device_id
 * \return -ENOTSUP if the device is on the white liste else 0 
 *
 * Function called by the kernel when a device is detected
 */
static int usbwall_probe (struct usb_interface *intf, const struct usb_device_id *devid)
{
  DBG_TRACE (DBG_LEVEL_DEBUG, "entering in the function probe");

  dev = interface_to_usbdev (intf);
  usb_string (dev, dev->descriptor.iSerialNumber, idSerialNumber, 32);

  my_device.info.idVendor = dev->descriptor.idVendor;
  my_device.info.idProduct = dev->descriptor.idProduct;
  strcpy(my_device.info.idSerialNumber, idSerialNumber);

  DBG_TRACE (DBG_LEVEL_INFO, "the device introduced has the following info");
  DBG_TRACE (DBG_LEVEL_INFO, "idVendor : %x", my_device.info.idVendor);
  DBG_TRACE (DBG_LEVEL_INFO, "idProduct : %x", my_device.info.idProduct);
  DBG_TRACE (DBG_LEVEL_INFO, "SerialNumber : %s", my_device.info.idSerialNumber);

  /* Research if the device is on the white list */
  /* If the device is on the white liste : the module is released */
  if(is_key_authorized(&my_device))
  {
    DBG_TRACE (DBG_LEVEL_INFO, "the device is on the white list");
    return -EMEDIUMTYPE;
  }
  /* Else : creation a fake device */
  DBG_TRACE (DBG_LEVEL_INFO, "the device isn't on the white list");
  return 0;
}

/** 
 * \fn usbwall_disconnect
 * \param  struct usb_interface *intf
 *
 * Function called when a device is desconnected
 */
static void usbwall_disconnect (struct usb_interface *intf)
{
  DBG_TRACE (DBG_LEVEL_INFO, "device disconnected");
}

/** 
 * \struct struct usb_driver usbwall_driver
 *
 * Identifies USB interface driver to usbcore
 */
static struct usb_driver usbwall_driver = {
  .name = "usbwall",
  .probe = usbwall_probe,
  .disconnect = usbwall_disconnect,
  .id_table = usbwall_id_table,
};


/** 
 * \fn __init usbwall_init
 * \return usbwall_register; O if register success, else error number (register failed). 
 *
 * Loading the module : register USB driver and charactere device
 */
static int __init usbwall_init (void)
{
  if (authmode >= USBWALL_AUTH_MAX) {
    DBG_TRACE(DBG_LEVEL_ERROR, "invalid authmode %d", authmode);
    return -EINVAL;
  }
  /* USB driver register*/
  usbwall_register = 0;
  usbwall_register = usb_register (&usbwall_driver);
  if (usbwall_register)
  {
    DBG_TRACE (DBG_LEVEL_ERROR, "Registering usb driver failed, error : %d", usbwall_register);
  }
  usbwall_proc_init();
  usbwall_chrdev_init();
  keylist_init();
  DBG_TRACE (DBG_LEVEL_INFO, "module loaded");
  return usbwall_register;
}

/** 
 * \fn __exit usbwall_exit (void)
 *
 * Unloading the module : unregister USB driver and charactere device
 */
static void __exit usbwall_exit (void)
{
  keylist_release();
  usbwall_chrdev_exit();
  usbwall_proc_release();
  /* USB driver unregister*/
  usb_deregister (&usbwall_driver);
  DBG_TRACE (DBG_LEVEL_INFO, "module unloaded");
}

module_init (usbwall_init);
module_exit (usbwall_exit);
