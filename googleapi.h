/** @file googleapi.h
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
#ifndef GOOGLEAPI_H_
#define GOOGLEAPI_H_
#include "google.h"
/**
 * @brief Holding json info
 * This struct is holding name/value type of Json
 * response from google.
 * 
 * @param name: Name of response
 * @param value: Value of response
 *
 */
typedef struct {
  char * name;
  char * value;
  size_t length;
} Json;

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
 * @param name: Name of value to add o replace
 * @param value:  The value
 *
 * Saves/replace the wanted value by its name (preferable to config file)
 **/
void setSetting(char * filename, char *name , char *value );

/**
 * @brief Handles response from google api.
 * @param response:  response string
 *
 * Stores the google response if it is type of "application/json"
 * to an 'array' of structs (Json).
 * TODO: implement HTTP response error handling in case the response is
 * not "application/json".
 **/
Json * parseResponse(char * response);
#endif // GOOGLEAPI_H_


