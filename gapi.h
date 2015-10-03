/* 
 * Copyright 2015 Georgios Tsotsos <tsotsos@linux.com>
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
#include "googlelib.h"//for demo only, this should be <GoogleLib/googlelib.h>

/* Concatenate 2 char* */
char *concat(char *s1, char *s2);
/* Removes spaces from char* */
char *trim(char *input);
/* Finds string between two others*/
char *find_between(char *response,char *first, char *last);
/* Read file */
char * ReadFile(char * filename);
/* Gets a value from config file */
char * getValue(char * string, char * value);
/* getSettings definition */
config getSettings(char *filename);
/* SetSettings definition*/
void setSetting(char * filename, char *search_string , char *replace_string );
/* Stores to an 'array' of Json Struct the json response from google  */
Json * GoogleResponse(char * response);
#endif // GAPI_H_

