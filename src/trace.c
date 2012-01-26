/*
** File trace.c for project usbwall
**
** Made by Philippe THIERRY
** Login   <phil@reseau-libre.net>
**
** Started on  jeu. 19 janv. 2012 11:16:03 CET Philippe THIERRY
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
** \file trace.c
**
** Brief documentation on one line
** Detailed documentation
** On multiple lines...
**
** \author Philippe THIERRY
**
*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include "trace.h"

static short dbglevel = 0;

module_param(dbglevel, short, 0640);
MODULE_PARM_DESC(dbglevel, "Module debug level, from 0 (no debug) to 5 (full debug)");

static unsigned int dbgline = 0;

short dbglevel_get(void)
{
  return dbglevel;
}

unsigned int dbgline_get_and_inc(void)
{
  return dbgline++;
}
