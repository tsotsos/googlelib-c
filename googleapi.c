/** @file googleapi.c
 *  @brief Code for functions handling google api
 * 
 *  Source code for handling google api responses that return
 *  by googlelib header file.
 *  here.
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
#include "googleapi.h"
         /*
char * GoogleDrive (char * token)
{
  connection *c;
  char *response;
  char *request=NULL;
  const char * format = "GET https://%s%s?access_token=%s\r\n";
  size_t length_request = snprintf(NULL,0,format,"www.googleapis.com","/drive/v2/files",token)+1;
  request = malloc(length_request);
  snprintf(request,length_request,format,"www.googleapis.com","/drive/v2/files",token);
  printf("\nREQUEST:%s\n",request);
  c = sslConnect ("www.googleapis.com",443);
  sslWrite (c, request);
  response = sslRead (c);
  sslDisconnect (c);
  printf("%s",response);
  FILE *f = fopen("file.txt", "w");
  fprintf(f, "%s", response);
  printf("\nResponse length %zu\n",strlen(response));
  return response;
}
*/
