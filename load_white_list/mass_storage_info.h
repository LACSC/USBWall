/*
** File mass_storage_info.h for project usbwall
**
** Made by David FERNANDES
** Login   <dfernand@ece.fr>
**
** Started on  Mon Jan  9 9:58:47 2012 David FERNANDES
**
** Copyright (C) 2012 - David FERNANDES
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
** \file usbwall.h
**
** Structure to store information of mass storages
**
** \author David FERNANDES
**
*/

#define IO_SET_PENDRIVE 1

/**
 * \struct mass_storage_info
 *
 * mass storage indentification
 */
struct mass_storage_info
{
  uint16_t idVendor;
  uint16_t idProduct;
  char idSerialNumber[32];
};
