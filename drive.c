/** @file drive.c
 *  @brief Functions to format properly headers for GoogleDrive APi
 *
 *  This file contains source that is responsible for making
 *  all headers for GoogleConnect function.
 *
 *  @author Georgios Tsotsos
 *  @bug Not any known bugs.
 */
/*
 * Copyright 2015 Georgios Tsotsos
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
#include "drive.h"

char * DriveFiles ( char *token)
{
  char *request=NULL;
  const char * format = "GET https://%s/%s?access_token=%s\r\n";
  size_t length_request = snprintf(NULL,0,format,GOOGLEAPI_HOST,DRIVE_FILES,
                                   token)+1;
  request = malloc(length_request);
  snprintf(request,length_request,format,GOOGLEAPI_HOST,DRIVE_FILES,token);
  return request;
}
