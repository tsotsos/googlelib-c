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
#include "gapi.h"
/**
 * concat - Concatenate function.
 * @s1: string
 * @s2: string
 *
 * Concatenates two strings
 **/
char * concat(char *s1, char *s2)
{
  char *result = malloc(strlen(s1)+strlen(s2)+1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}
/**
 * trim - Triming function.
 * @input: string
 *
 * Remove spaces of a string
 **/
char * trim(char *input)
{
  int i,j;
  char *output=input;
  int len = strlen(input);
  for (i = 0, j = 0; i < len; i++,j++) {
    if (input[i]!=' ')
      output[j]=input[i];
    else
      j--;
  }
  output[j]=0;
  return output;
}
/**
 * find_between - Finds between two strings
 * @input: input string
 * @first: string, starting limit
 * @last : string, last limit
 *
 * Gets the containing string between two others (first and last)
 **/
char * find_between(char *input,char *first, char *last)
{
  char *target = NULL;
  char *start, *end;
  if ( (start = strstr( input, first ) )) {
    start += strlen( first );
    if ( (end = strstr( start, last ) )) {
      target = ( char * )malloc( end - start + 1 );
      memcpy( target, start, end - start );
      target[end - start] = '\0';
    }
  }
  if ( target )
    return target;
  else
    free(target);
  return NULL;
}
/**
 * ReadFile - Reads a file
 * @filename: The filename
 *
 * Reads a file to buffer *char
 **/
char * ReadFile(char * filename)
{
  char *buffer = NULL;
  int string_size,read_size;
  FILE *handler = fopen(filename,"r");

  if (handler) {
    fseek(handler,0,SEEK_END);
    string_size = ftell (handler);
    rewind(handler);
    buffer = (char*) malloc (sizeof(char) * (string_size + 1) );
    read_size = fread(buffer,sizeof(char),string_size,handler);
    buffer[string_size] = '\0';
    if (string_size != read_size) {
      free(buffer);
      buffer = NULL;
    }
  }

  return buffer;
}
/**
 * ParseJson - Parses a Json style string
 * @json: Json style string
 * @value: The wanted value
 *
 * Finds a value based on its name in a Json string
 **/
char* parseJson(char *json, char *value)
{
  value = concat("{\"", value);
  value = concat(value,"\": \"");
  char *last = "\"";
  char *target = NULL;
  char *start, *end;

  if ( (start = strstr( json, value ) )) {
    start += strlen( value );
    if ( (end = strstr( start, last ) )) {
      target = ( char * )malloc( end - start + 1 );
      memcpy( target, start, end - start );
      target[end - start] = '\0';
    }
  }
  if ( target )
    return target;
  else
    free(target);
  exit(EXIT_FAILURE);
}
/**
 * getValue - Find a value of config
 * @string: Config string
 * @value: The wanted value
 *
 * Finds a value based on its name in a Config string
 **/
char * getValue(char * string, char * value)
{
  string=trim(string);
  value = concat(value,"=\"");
  char *last = "\"";
  char *target = NULL;
  char *start, *end;

  if ( (start = strstr( string, value ) )) {
    start += strlen( value );
    if ( (end = strstr( start, last ) )) {
      target = ( char * )malloc( end - start + 1 );
      memcpy( target, start, end - start );
      target[end - start] = '\0';
    }
  }
  if ( target )
    return target;
  else
    free(target);
  return NULL;
}
/**
 * getSettings - Get the settings from config file
 * @filename: Config filename
 *
 * Stores the settings from config file to a config
 * struct.
 **/
config getSettings(char *filename)
{
  config settings;
  char * file = trim(ReadFile(filename));
  settings.authhost=getValue(file,"authhost");
  settings.authpage=getValue(file,"authpage");
  settings.tokenhost=getValue(file,"tokenhost");
  settings.tokenpage=getValue(file,"tokenpage");
  settings.client_id=getValue(file,"client_id");
  settings.client_secret=getValue(file,"client_secret");
  settings.redirect_uri=getValue(file,"redirect_uri");
  settings.accesstoken=getValue(file,"accesstoken");

  return settings;
}
/**
 * setSettings - Saves settings to file
 * @filename: Config filename
 * @search_string: name of value needs to renew
 * @replace_string: new value
 *
 * Saves/replace the wanted value by its name (preferable to config file)
 * TODO: implement a way to save value if there is not.
 **/
void setSetting(char * filename, char *search_string , char *replace_string )
{
  FILE *input = fopen(filename, "r");
  FILE *output = fopen("tmp.txt", "w");
  char buffer[512];
  while (fgets(buffer, sizeof(buffer), input) != NULL) {
    char *pos = strstr(buffer, search_string);
    if (pos != NULL) {
      char *temp = calloc(
                     strlen(buffer) - strlen(search_string) + strlen(replace_string) + 1, 1);
      memcpy(temp, buffer, pos - buffer);
      memcpy(temp + (pos - buffer), replace_string, strlen(replace_string));
      memcpy(temp + (pos - buffer) + strlen(replace_string),
             pos + strlen(search_string),
             1 + strlen(buffer) - ((pos - buffer) + strlen(search_string)));
      fputs(temp, output);

      free(temp);
    } else
      fputs(buffer, output);
  }
  fclose(output);
  fclose(input);

  /* Rename the temporary file to the original file */
  rename("tmp.txt", filename);
}
/**
 * GoogleResponse - Handles response from google api.
 * @response: response string
 *
 * Stores the google response if it is type of "application/json"
 * to an 'array' of structs (Json).
 * TODO: implement HTTP response error handling in case the response is
 * not "application/json".
 **/
Json * GoogleResponse(char * response)
{
  Json *json=0;
  // char *http_code = find_between(response,"HTTP/1.1 ","\r");
  char *content_type = find_between(response,"Content-Type: ",";");
  // Checks for content type = "application/json"
  int content_type_cmp = strcmp(content_type,"application/json");
  int i=0;
  if (content_type_cmp == 0) {
    char * curLine = find_between(response,"{","}");
    // read every line of Json string
    while(curLine) {
      char * nextLine = strchr(curLine, '\n');
      if (nextLine) *nextLine = '\0';
      char * key = find_between(curLine,"\"","\":");
      char * val = find_between(curLine,": \"","\"");
      if ( ((key != NULL) && (val !=NULL) ) ) {
        if (i==0)
          json = malloc(1 * sizeof(Json));
        else
          json = realloc (json, ((i + 1) * sizeof(Json)));
        json[i].name = key;
        json[i].value = val;
        i++;
      }
      if (nextLine) *nextLine = '\n';
      curLine = nextLine ? (nextLine+1) : NULL;
    }
    return json;
  } else {
    json = 0;
    return json;
  }

}

