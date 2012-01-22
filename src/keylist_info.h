/**
 * \file keylist_info.h
 * \brief structure to store information of mass storages and list structure
 * \author David FERNANDES
 * \version 0.1
 * \date 22 january 2012
 *
 * File keylist_info.h for project usbwall
 *
 * Made by David FERNANDES
 * Login   <dfernand@ece.fr>
 *
 * Started on  Sun Jan  22 15:17:29 2012 David FERNANDES
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


#ifndef KEYLIST_INFO_H_
#define KEYLIST_INFO_H_

#include "usbwall.h"
#include <linux/list.h>

struct internal_token_info {
 struct usbwall_token_info info;
 struct list_head list;
};

#endif /*! KEYLIST_INFO_H_*/
