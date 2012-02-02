/*
** File keylkist.h for project usbwall
** 
** LACSC - ECE PARIS Engineering school 
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
** \file keylist.h
**
** Brief documentation on one line
** Detailed documentation
** On multiple lines...
**
** \author Philippe THIERRY
**
*/

#ifndef KEYLIST_H_
#define KEYLIST_H_

#include "keylist_info.h"
#include <linux/list.h>

int	key_add(struct internal_token_info*	keyinfo);

int	key_del(struct internal_token_info*	keyinfo);

int	is_key_authorized(struct internal_token_info*	keyinfo);

void 	print_keylist(char* status_buffer);

int 	keylist_init(void);

void 	keylist_release(void);

#endif /*! KEYLIST_H_*/
