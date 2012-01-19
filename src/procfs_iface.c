/*
** File procfs_iface.c for project usbwall
**
** Made by Philippe THIERRY
** Login   <phil@reseau-libre.net>
**
** Started on  jeu. 19 janv. 2012 10:34:39 CET Philippe THIERRY
**
** Copyright (C) 2009 - Philippe THIERRY
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/*
** \file procfs_iface.c
**
** Brief documentation on one line
** Detailed documentation
** On multiple lines...
**
** \author Philippe THIERRY
**
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include "procfs_iface.h"
#include "trace.h"
#include "usbwall.h"
#include "keylist.h"

static struct proc_dir_entry* usbwalldir = NULL;
static struct proc_dir_entry* usbwallkeyctrl = NULL;

/*!
 ** \fn write_func
 ** 
 ** \param file 
 ** \param buffer 
 ** \param count 
 ** \param data 
 ** 
 ** \return 
 */
static int usbwall_keyctrl_write(struct file* file,
                                 const char* buffer,
                                 unsigned long count,
                                 void* data)
{
    int len;
    procfs_info_t u_keyinfo;
    /* struct fb_data_t *fb_data = (struct fb_data_t *)data; */

    /* MOD_INC_USE_COUNT; */
    if(count > sizeof(procfs_info_t))
        len = sizeof(procfs_info_t);
    else
        len = count;
    DBG_TRACE("reading %ld len from userspace", count);
    if(copy_from_user(u_keyinfo.buffer, buffer, len)) {
        /* MOD_DEC_USE_COUNT; */
        return -EFAULT;
    }
    u_keyinfo.buffer[len] = '\0';
    DBG_TRACE("reading: vendor: %x, product: %x, serial: %s",
	      u_keyinfo.info.idVendor,
              u_keyinfo.info.idProduct,
              u_keyinfo.info.idSerialNumber);
    if ((u_keyinfo.info.keyflags & USBWALL_KEY_ADD)) {
      DBG_TRACE("adding key %s to whitelist", u_keyinfo.info.idSerialNumber);
      key_add(&(u_keyinfo.info));
    }
   if ((u_keyinfo.info.keyflags & USBWALL_KEY_DEL)) {
      DBG_TRACE("deleting key %s to whitelist", u_keyinfo.info.idSerialNumber);
      key_del(&(u_keyinfo.info));
    }
    /* MOD_DEC_USE_COUNT; */

    return len;
}



/*!
 ** \fn usbwall_proc_init
 ** 
 ** \return 
 */
int usbwall_proc_init()
{
    usbwalldir = proc_mkdir("usbwall", NULL);
    if (usbwalldir == NULL) {
	goto fail_proc_mkdir;
    }
    usbwallkeyctrl = create_proc_entry("key_ctrl", 0600, usbwalldir);
    if (usbwallkeyctrl == NULL) {
	goto fail_proc_entry;
    }
    usbwallkeyctrl->write_proc = usbwall_keyctrl_write;
    return 0;

/* failure management - std linux usage */
fail_proc_entry:
    remove_proc_entry("usbwall", NULL);
fail_proc_mkdir:
    return 1;
}

/*!
 ** \fn usbwall_proc_release
 */
void usbwall_proc_release()
{
    remove_proc_entry("key_ctrl", usbwalldir);
    remove_proc_entry("usbwall", NULL);
}
