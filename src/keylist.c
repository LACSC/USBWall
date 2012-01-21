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
#include "usbwall.h"

static struct list_head key_list_head;
static struct mass_storage_info* keyinfo_lastadd;

int     key_add_first_element(struct mass_storage_info*	keyinfo) 
{
  INIT_LIST_HEAD(&key_list_head); /* Initialize the list */
  list_add(&keyinfo->list, &key_list_head); /* Insert struct afert the head */
  keyinfo_lastadd = keyinfo;
  return 0;
}

int	key_add(struct mass_storage_info*	keyinfo)
{
  list_add(&keyinfo->list, &keyinfo_lastadd->list); /* Insert struct afert the last element */
  keyinfo_lastadd = keyinfo;
  return 0;
}

int	key_del(struct mass_storage_info*	keyinfo)
{
  /*list_del(keyinfo, keys_list);*/
  return 0;
}

int	is_key_authorized(struct mass_storage_info*	keyinfo)
{
  return 0;
}

