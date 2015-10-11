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
  settings.refresh_token=getValue(file,"refresh_token");

  return settings;
}

void setSetting(char * filename, char *name , char *value )
{
  FILE *input = fopen(filename, "r");
  FILE *output = fopen("tmp.txt", "w");
  char buffer[512];
  int controller = 0;
  while (fgets(buffer, sizeof(buffer), input) != NULL) {
    char *pos = strstr(buffer, name);
    if (pos != NULL) {
      const char * format = "%s=\"%s\"; ";
      char * newvar = NULL;
      size_t flen = snprintf(NULL,0,format,name,value);
      newvar = malloc(flen+1);
      snprintf(newvar,flen,format,name,value);
      printf("\n\n\n %s",newvar);
      char *temp = calloc(strlen(buffer) - strlen(name) + flen + 1, 1);
      memcpy(temp, buffer, pos - buffer);
      memcpy(temp + (pos - buffer), newvar, flen);
      fputs(temp, output);
      free(temp);
      controller = 1;
    } else
      fputs(buffer, output);
  }
  if ( controller != 1  ) {
    const char * format = "%s=\"%s\";\n";
    char * newvar = NULL;
    size_t flen = snprintf(NULL,0,format,name,value);
    newvar = malloc(flen+1);
    snprintf(newvar,flen,format,name,value);
    char *temp = calloc(
                   strlen(buffer) + flen+1, 1);
    memcpy(temp, newvar, flen);
    fputs(temp, output);
  }
  fclose(output);
  fclose(input);

  /* Rename the temporary file to the original file */
  rename("tmp.txt", filename);
}


Json * parseResponse(char * response)
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
      if (val == NULL) // check for integer
        val = find_between(curLine,": ",",");
      printf("key=%s, val=%s\n",key,val);
      if ( ((key != NULL) && (val !=NULL) ) ) {
        if (i==0)
          json = malloc(1 * sizeof(Json));
        else
          json = realloc (json, ((i + 1) * sizeof(Json)));
        json[i].name = key;
        json[i].value = val;
        json[0].length = i; //'array' size storing
        i++;
      }
      if (nextLine) *nextLine = '\n';
      curLine = nextLine ? (nextLine+1) : NULL;
    }
  } else
    json = 0;
  return json;
}
