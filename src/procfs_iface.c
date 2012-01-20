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

#define USBWALL_PROC_STATUS_BUFFER_SIZE 256

static struct proc_dir_entry* usbwalldir = NULL;
static struct proc_dir_entry* usbwallkeyctrl = NULL;
static struct proc_dir_entry* usbwallstatus = NULL;

static char status_buffer[USBWALL_PROC_STATUS_BUFFER_SIZE];

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
 ** \brief usbwall_status_read
 ** 
 ** \param page the kernel mapped memory page in which the data should be copied
 ** \param start unused
 ** \param off the current string offset asked by the userspace (if read in multiple times)
 ** \param count the asked bytes count to read
 ** \param eof EOF signal pointer to be setted
 ** \param data unused
 ** 
 ** \return the number of effectively copied bytes
 */
int usbwall_status_read(char *page,
                        char **start,
                        off_t off,
                        int count,
                        int *eof,
                        void *data)
{
   int is_eof = 0;

   DBG_TRACE("entering status read, asking for reading %d bytes. Current offset is %ld", count, off);
   /*
   ** test buffer by now
   */
   sprintf(status_buffer, "USBWall module release %s\n", USBWALL_MODVERSION);
   if (off >= strlen(status_buffer)) {
     DBG_TRACE("offset greater than string size, leaving");
     count = 0;
     is_eof = 1;
     goto end;
   }
   if (count > (strlen(status_buffer) - off)) {
     count = (strlen(status_buffer) - off + 1);
     is_eof = 1;
   }
   snprintf(&(page[off]), count, &(status_buffer[off]));

end:
   if (is_eof) {
     DBG_TRACE("EOF=1");
     /* update file offset */
      *eof = 1;
   } else {
      *eof = 0;
   }
   DBG_TRACE("end of function. Returning %d", count);
   return count;
}

/*!
 ** \fn usbwall_proc_init initialize the usbwall procfs itnerface
 ** 
 ** \return 1 if init failed, or 0.
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
    usbwallstatus = create_proc_entry("status", 0400, usbwalldir);
    if (usbwallstatus == NULL) {
	goto fail_proc_entry_2;
    }

    usbwallkeyctrl->write_proc = usbwall_keyctrl_write;
    usbwallstatus->read_proc = usbwall_status_read;
    return 0;

/* failure management - std linux usage */
fail_proc_entry_2:
    remove_proc_entry("status", usbwalldir);
fail_proc_entry:
    remove_proc_entry("usbwall", NULL);
fail_proc_mkdir:
    return 1;
}

/*!
 ** \fn usbwall_proc_release release the usbwall procfs interface
 */
void usbwall_proc_release()
{
    remove_proc_entry("status", usbwalldir);
    remove_proc_entry("key_ctrl", usbwalldir);
    remove_proc_entry("usbwall", NULL);
}
