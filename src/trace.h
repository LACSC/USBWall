/*
** File trace.h for project usbwall
**
** Made by Philippe THIERRY
** Login   <t0090690@reseau-libre.net>
**
** Started on  Fri Jan  6 12:57:29 2012 Philippe THIERRY
**
** Copyright (C) 2012 - Philippe THIERRY
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
** \file trace.h
**
** Brief documentation on one line
** Detailed documentation
** On multiple lines...
**
** \author Philippe THIERRY
**
*/

#ifndef TRACE_H_
# define TRACE_H_

#define DBG_PREFIX "usbwall:"

unsigned int dbgline = 0;

/* INFO: thanks to Jean-Marc LACROIX for this macro */
/*
** following variable used to create a counter on each output,
** very useful to debug !!!!
*/
/*
** following macro edit a message via syslog with dedicated
** pattern such ...
** Please note that debug message is under control of dbg_count_line
** dbg_trace 00000058 : 0300 : dummy_get_stats : enter device net_0
*/
#define DBG_TRACE(fmt, arg...)                  \
printk("%s %08u : %04d : %s : " fmt "\n",       \
       DBG_PREFIX,                              \
       dbgline++,		                \
       __LINE__,                                \
       __func__,                                \
## arg)

/*
Exemple d'utilisation de la macro:
DBG_TRACE("trace a ecrire: val=%d\n", val);
*/

#endif /* !TRACE_H_ */
