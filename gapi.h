/** @file gapi.h
 *  @brief Function prototypes for handling google api
 *
 *  Contains function prototypes for handling google api responses
 *  that return by googlelib header file. Also struct for Json
 *  hosted here.
 *
 *  @author Georgios Tsotsos
 *  @bug Not any known bugs.
 */
 
/* Copyright 2015 Georgios Tsotsos
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
 * @brief Holding json info
 * This struct is holding name/value type of Json
 * response from google.
 * 
 * @param name: stores socket
 * @param value: stores SSL
 *
 */
typedef struct {
  char * name;
  char * value;
} Json;

/**
 * @brief Concatenate function.
 * @param s1:  string
 * @param s2:  string
 *
 * Concatenates two strings
 **/

char *concat(char *s1, char *s2);
/**
 * @brief Triming function.
 * @param input:  string
 *
 * Remove spaces of a string
 **/
char *trim(char *input);

/**
 * @brief Finds between two strings
 * @param input:  input string
 * @param first:  string, starting limit
 * @param last :  string, last limit
 *
 * Gets the containing string between two others (first and last)
 **/
char *find_between(char *response,char *first, char *last);

/**
 * @brief Reads a file
 * @param filename:  The filename
 *
 * Reads a file to buffer *char
 **/
char * ReadFile(char * filename);

/**
 * @brief Parses a Json style string
 * @param json:  Json style string
 * @param value:  The wanted value
 *
 * Finds a value based on its name in a Json string
 **/
char* parseJson(char *json, char *value);

 /**
 * @brief Find a value of config
 * @param string:  Config string
 * @param value:  The wanted value
 *
 * Finds a value based on its name in a Config string
 **/
char * getValue(char * string, char * value);

/**
 * @brief Get the settings from config file
 * @param filename:  Config filename
 *
 * Stores the settings from config file to a config
 * struct.
 **/
config getSettings(char *filename);

/**
 * @brief Saves settings to file
 * @param filename:  Config filename
 * @param search_string:  name of value needs to renew
 * @param replace_string:  new value
 *
 * Saves/replace the wanted value by its name (preferable to config file)
 * TODO: implement a way to save value if there is not.
 **/
void setSetting(char * filename, char *search_string , char *replace_string );

/**
 * @brief Handles response from google api.
 * @param response:  response string
 *
 * Stores the google response if it is type of "application/json"
 * to an 'array' of structs (Json).
 * TODO: implement HTTP response error handling in case the response is
 * not "application/json".
 **/
Json * GoogleResponse(char * response);
#endif // GAPI_H_


