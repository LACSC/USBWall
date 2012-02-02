/*
** usbwall_chrdev.c for usbwall in /usbwall/usbwall/src
**
** Made by t0090690
** Login   <t0090690@fr.thalesgroup.com>
**
** Started on  mer 01 fév 2012 13:55:36 CET t0090690
** Last update mer 01 fév 2012 13:55:36 CET t0090690
*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/rwsem.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/device.h>

/* for ioctl */
#include <asm/ioctl.h>
#include <asm/uaccess.h>
#include <linux/sched.h>

#include "trace.h"
#include "usbwall.h"
#include "keylist.h"

static struct cdev	*cdev;

static dev_t			dev;

/*!
** \var static kclass *usbwall_class
**
** \brief Kernel specific device class container
**
** \par
** CAUTION: the usage of a define here is used to avoid a pmccabe SEGV
** pmccabe seems not tu support the 'class' name as a pure C struct name and
** try to detect a C++ OO class. This behaviour make it execute a
** strcmp(NULL).
** \par
** BTW, if pmccabe is no more needed, just delete the define line and replace
** the folliwing one with a "static struct class ..."
** \par
** The struct class is an internal kernel structure used to manage devices
** familly.
*/
#define kclass			struct class
static kclass			*usbwall_class;

static struct device		*usbwall_class_device;

static unsigned int usbwall_major = 0;

static int open_count = 0;

struct context {
  uint32_t	ioctlstats;
};

static int32_t
usbwall_chrdev_open(struct inode		*inode __attribute__((unused)),
                    struct file		*filp)
{
  struct context *ctx = NULL;

  DBG_TRACE(DBG_LEVEL_DEBUG, "Entering open");
  /* simple (need to be updated) support for single access */
  if (open_count != 0) {
    DBG_TRACE(DBG_LEVEL_ERROR, "Device busy, check lsof");
    return -EBUSY;
  }
  ctx = kmalloc(sizeof(struct context), GFP_KERNEL);
  if (NULL == ctx) {
    DBG_TRACE(DBG_LEVEL_ERROR, "unable to allocate local context");
    return -ENOMEM;
  }
  filp->private_data = (void*)ctx;
  open_count += 1;
  DBG_TRACE(DBG_LEVEL_DEBUG, "Leaving open");
  return 0;
}

static long
usbwall_chrdev_ioctl(
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,34)) /* check the last old mode ioctl structure */
                  struct inode	*inode __attribute__((unused)),
#endif
                  struct file	*filep __attribute__((unused)),
                  unsigned int	cmd,
                  unsigned long	arg)
{
  struct internal_token_info *internal_keyinfo = NULL;
  DBG_TRACE(DBG_LEVEL_DEBUG, "Entering ioctl");

  switch (cmd) {
      case USBWALL_IO_ADDKEY:
          internal_keyinfo = kmalloc(sizeof(internal_keyinfo),GFP_KERNEL);
          if (internal_keyinfo == NULL) {
              DBG_TRACE(DBG_LEVEL_ERROR, "net enough memory to add key");
              goto err_nomem;
          }
          DBG_TRACE(DBG_LEVEL_DEBUG, "reading %ld len from userspace", sizeof(internal_keyinfo));
          if(copy_from_user(&(internal_keyinfo->info), (struct usbwall_key_info*)arg, sizeof(struct usbwall_token_info))) {
              /* MOD_DEC_USE_COUNT; */
              DBG_TRACE(DBG_LEVEL_ERROR, "bad argument: unable to get back content from userspace");
              goto err_badarg;
          }
          DBG_TRACE(DBG_LEVEL_NOTICE, "reading: vendor: %x, product: %x, serial: %s",
                    internal_keyinfo->info.idVendor,
                    internal_keyinfo->info.idProduct,
                    internal_keyinfo->info.idSerialNumber);
          key_add(internal_keyinfo);
          break;

      case USBWALL_IO_DELKEY:
          internal_keyinfo = kmalloc(sizeof(internal_keyinfo),GFP_KERNEL);
          if (internal_keyinfo == NULL) {
              DBG_TRACE(DBG_LEVEL_ERROR, "net enough memory to add key");
              goto err_nomem;
          }
          DBG_TRACE(DBG_LEVEL_DEBUG, "reading %ld len from userspace", sizeof(internal_keyinfo));
          if(copy_from_user(&(internal_keyinfo->info), (struct usbwall_key_info*)arg, sizeof(struct usbwall_token_info))) {
              /* MOD_DEC_USE_COUNT; */
              DBG_TRACE(DBG_LEVEL_ERROR, "bad argument: unable to get back content from userspace");
              goto err_badarg;
          }
          DBG_TRACE(DBG_LEVEL_NOTICE, "reading: vendor: %x, product: %x, serial: %s",
                    internal_keyinfo->info.idVendor,
                    internal_keyinfo->info.idProduct,
                    internal_keyinfo->info.idSerialNumber);
          key_del(internal_keyinfo);
          kfree(internal_keyinfo);
          break;

      default:
          goto err_cmd;
  }
  DBG_TRACE(DBG_LEVEL_DEBUG, "Leaving ioctl");
  return 0;

err_badarg:
  DBG_TRACE(DBG_LEVEL_DEBUG, "Leaving ioctl with error FAULT");
  kfree(internal_keyinfo);
  return -EFAULT;
err_cmd:
  DBG_TRACE(DBG_LEVEL_DEBUG, "Leaving ioctl with error INVAL");
  return -EINVAL;
err_nomem:
  DBG_TRACE(DBG_LEVEL_DEBUG, "Leaving ioctl with error NOMEM");
  return -ENOMEM;
}

static int
usbwall_chrdev_release(struct inode        *inode __attribute__((unused)),
                       struct file	   *file __attribute__((unused)))
{
  open_count -= 1;
  return 0;
}

/*!
** @brief Structure that describles function which are called when /dev/usbwall
** is used from userspace.
** Only hc_write is called when /dev/hc is written from userspace.
** @arg owner definition correspond to the current module
** @arg open replacement function
** @arg ioctl replacement function
** @arg close replacement function
**
*/
static struct file_operations usbwall_fops =
{
  owner : THIS_MODULE,
  open : usbwall_chrdev_open,
  unlocked_ioctl : usbwall_chrdev_ioctl,
  release : usbwall_chrdev_release,
};

/*!
** @brief Initialize the chrdev module part.
** - Load the usbwall virtual char device
** - create the associated structures (cdev_list vector)
** - register a usbwall_class kernel device class
** - register the "usbwall" device
** @return 0 if okay, possitive value otherwise.
*/
int32_t
usbwall_chrdev_init(void)
{
  int8_t ret = 0;
  int8_t		udev_ret = 1;
  int32_t		err;
  int32_t		devno;

  DBG_TRACE(DBG_LEVEL_INFO, "registering char device... ");
  usbwall_class_device = NULL;
  /*
  ** In order to create a dev with dynamic MAJOR
  */
  dev = MKDEV(0, 0);

  /*
  ** first, allocating char devices (allocate MAJOR & MINORS)
  ** the MAJOR is choosen by the kernel. num_cpu + 1 minors are
  ** supported by the module, starting at 0.
  ** This module devices prefix is "usbwall"
  */
  udev_ret = alloc_chrdev_region(&dev, 0, 1, "usbwall");
  if (udev_ret != 0)
  {
    DBG_TRACE(DBG_LEVEL_ERROR, "Allocation of char device region failed");
    DBG_TRACE(DBG_LEVEL_ERROR, "alloc_chrdev_region returns %d\n", udev_ret);
    goto failure;
  }
  usbwall_major = MAJOR(dev);
  DBG_TRACE(DBG_LEVEL_NOTICE, "dynamic major: %d\n", usbwall_major);


  devno = MKDEV(usbwall_major, 0);
  DBG_TRACE(DBG_LEVEL_INFO, "Loading cdev for device");
  /* allocating the cdev structure and add it to the cdev_list */
  cdev = cdev_alloc();
  if (cdev == NULL)
  {
    DBG_TRACE(DBG_LEVEL_ERROR, "Allocating of the cdev impossible");
    goto failure;
  }
  /* initialize the corresponding cdev */
  cdev_init(cdev, &usbwall_fops);
  cdev->owner = THIS_MODULE;
  cdev->ops = &usbwall_fops;
  /*
  ** register the cdev to the corresponding major/minor.
  ** We consider here one cdev per minor.
  */
  err = cdev_add(cdev, devno, 1);
  if (err != 0)
  {
    DBG_TRACE(DBG_LEVEL_ERROR, "Unable to add cdev !\n");
    ret = 1;
  }
  else
  {
    DBG_TRACE(DBG_LEVEL_INFO, "cdev loaded correctly.");
  }

  usbwall_class = class_create(THIS_MODULE, "usbwall_class");
  /*
  ** This needs a relativly recent kernel (2.6.18 doesn't support NULL
  ** parrent). Otherwise just print a warning when loading.
  */
  /*
  *,* INFO: Since 2.6.28, the device_create function prototype has been modified
  ** As a consequence, the function call has to be kernel-version specific.
  */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 27)
  usbwall_class_device = device_create(usbwall_class, NULL, dev, NULL, "usbwall");
#else
  usbwall_class_device = device_create(usbwall_class, NULL, dev, "usbwall");
#endif
  printk("done.\n");

  return 0;

failure:
  return ret;
}

void
usbwall_chrdev_exit(void)
{
  device_destroy(usbwall_class, dev);
  class_destroy(usbwall_class);
  cdev_del(cdev);
  unregister_chrdev_region(dev, usbwall_major);
}
