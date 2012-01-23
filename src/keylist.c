/*
** File keylkist.c for project usbwall
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
** \file keylist.c
**
** Brief documentation on one line
** Detailed documentation
** On multiple lines...
**
** \author Philippe THIERRY
**
*/

#include <linux/list.h>
#include "keylist_info.h"
#include "usbwall.h"
#include "trace.h"

static struct list_head key_list_head;
static struct internal_token_info* keyinfo_tmp;
static int listempty = 0;

int	key_add(struct internal_token_info*	keyinfo)
{
  if(listempty == 0)
  {
    INIT_LIST_HEAD(&key_list_head); /* Initialize the list */
    list_add(&keyinfo->list, &key_list_head); /* Insert struct after the head */
    //keyinfo_lastadd = keyinfo;
  }
  else
  {
    list_add(&keyinfo->list, &key_list_head); /* Insert struct after the last element */;
    //keyinfo_lastadd = keyinfo;
  }
  listempty++;
  return 0;
}

int	key_del(struct internal_token_info*	keyinfo)
{
  if(listempty != 0)
  {
    list_del(&keyinfo->list); /* Delete struct */
    listempty--;
    return 0;
  }
  else
  {
    DBG_TRACE ("error : the list is empty");
    return 1;
  }
}

int	is_key_authorized(struct internal_token_info*	keyinfo)
{
  if(listempty != 0)
  {
    list_for_each_entry(keyinfo_tmp, &key_list_head, list) /* Get each item */
    { 
      DBG_TRACE ("Vendor list %x, Product list %x", keyinfo_tmp->info.idVendor, keyinfo_tmp->info.idProduct);
      if(keyinfo->info.idVendor == keyinfo_tmp->info.idVendor  && keyinfo->info.idProduct == keyinfo_tmp->info.idProduct)
      {
        return 0;
      } 
    }
    return 1;
  }
  else
  {
    DBG_TRACE ("error : the list is empty");
    return 1;
  }
}

