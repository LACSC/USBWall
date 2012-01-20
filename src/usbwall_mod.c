/**
 * \file usbwall_mod.c
 * \brief module for USB mass-storage device filtering
 * \author David FERNANDES
 * \version 0.1
 * \date 19 december 2011
 *
 * File usbwall_mod.c for project usbwall
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
#include "procfs_iface.h"


/* Module informations */
MODULE_AUTHOR ("David FERNANDES");
MODULE_DESCRIPTION ("Module for USB mass-storage device filtering");
MODULE_LICENSE ("GPL");
MODULE_VERSION (USBWALL_MODVERSION);

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
static char serialnumber[32] = "";
static char my_idSerialNumber[32] = "001D92DC4AF0C95163A2092C";
static struct mass_storage_info my_pen_drive;
static int usbwall_register, cmpserialnumber;

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
  int i;
  DBG_TRACE ("entering in the function probe");

  /* Temporary white liste : my pen drive initialization */
  i = 0;
  while(my_idSerialNumber[i] != '\0')
  {
    my_pen_drive.idSerialNumber[i] = my_idSerialNumber[i];
    i++;
  }

  dev = interface_to_usbdev (intf);
  usb_string (dev, dev->descriptor.iSerialNumber, serialnumber, 32);

  cmpserialnumber = strcmp (serialnumber, my_pen_drive.idSerialNumber);

  /* Research if the device is on the white list */
  /* If the device is on the white liste : the module is released */
  if (dev->descriptor.idProduct == my_pen_drive.idProduct &&
      dev->descriptor.idVendor == my_pen_drive.idVendor &&
      cmpserialnumber == 0)
  {
    DBG_TRACE ("the device is on the white liste");
    return -EMEDIUMTYPE;
  }
  /* Else : creation a fake device */
  DBG_TRACE ("the device is not on the white liste");
  DBG_TRACE ("its idVendor is %x", dev->descriptor.idVendor);
  DBG_TRACE ("its idProduct is %x", dev->descriptor.idProduct);
  DBG_TRACE ("its SerialNumber is %s", serialnumber);

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
  DBG_TRACE ("device disconnected");
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
  /* USB driver register*/
  usbwall_register = 0;
  usbwall_register = usb_register (&usbwall_driver);
  if (usbwall_register)
  {
    DBG_TRACE ("Registering usb driver failed, error : %d", usbwall_register);
  }
  usbwall_proc_init();
  DBG_TRACE ("module loaded");
  return usbwall_register;
}

/** 
 * \fn __exit usbwall_exit (void)
 *
 * Unloading the module : unregister USB driver and charactere device
 */
static void __exit usbwall_exit (void)
{

  usbwall_proc_release();
  /* USB driver unregister*/
  usb_deregister (&usbwall_driver);
  DBG_TRACE ("module unloaded");
}

module_init (usbwall_init);
module_exit (usbwall_exit);
