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
#ifndef GAPI_H_
#define GAPI_H_
#include "googlelib.h"
#include <stdio.h>
/**
 * struct Json - Holding json info
 * @name: stores socket
 * @value: stores SSL
 *
 * This struct is holding name/value type of Json
 * response from google.
 */
typedef struct {
  char * name;
  char * value;
} Json;

char *concat(char *s1, char *s2);

char *trim(char *input);

char *find_between(char *response,char *first, char *last);

char * ReadFile(char * filename);

char * getValue(char * string, char * value);

config getSettings(char *filename);

void setSetting(char * filename, char *search_string , char *replace_string );

Json * GoogleResponse(char * response);
#endif // GAPI_H_


