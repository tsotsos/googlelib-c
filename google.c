/** @file google.c
 *  @brief Code for functions make requests to google api
 *
 *  This file contains source that is responsible for making
 *  any connection to Google Api. All connections are made by
 *  using sockets and OpenSSL, following Google Api regulations.
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
#include "google.h"

char * concat(char *s1, char *s2)
{
  char *result = malloc(strlen(s1)+strlen(s2)+1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

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

int tcpConnect (char * server_addr, int server_port)
{
  int error, handle;
  struct hostent *host;
  struct sockaddr_in server;

  host = gethostbyname (server_addr);
  handle = socket (AF_INET, SOCK_STREAM, 0);
  if (handle == -1) {
    perror ("Socket");
    handle = 0;
  } else {
    server.sin_family = AF_INET;
    server.sin_port = htons (server_port);
    server.sin_addr = *((struct in_addr *) host->h_addr_list[0]);
    // NOT C11 : bzero (&(server.sin_zero), 8);
    memset (&(server.sin_zero),0, 8);
    error = connect (handle, (struct sockaddr *) &server,
                     sizeof (struct sockaddr));
    if (error == -1) {
      perror ("Connect");
      handle = 0;
    }
  }

  return handle;
}

connection *sslConnect (char * server_addr,int server_port)
{
  connection *c;

  c = malloc (sizeof (connection));
  c->sslHandle = NULL;
  c->sslContext = NULL;

  c->socket = tcpConnect (server_addr, server_port);
  if (c->socket) {
    SSL_load_error_strings ();
    SSL_library_init ();

    c->sslContext = SSL_CTX_new (SSLv23_client_method ());
    if (c->sslContext == NULL)
      ERR_print_errors_fp (stderr);

    c->sslHandle = SSL_new (c->sslContext);
    if (c->sslHandle == NULL)
      ERR_print_errors_fp (stderr);

    if (!SSL_set_fd (c->sslHandle, c->socket))
      ERR_print_errors_fp (stderr);

    // Handshake
    if (SSL_connect (c->sslHandle) != 1)
      ERR_print_errors_fp (stderr);
  } else {
    perror ("Connection failed");
  }

  return c;
}

void sslDisconnect (connection *c)
{
  if (c->socket)
    close (c->socket);
  if (c->sslHandle) {
    SSL_shutdown (c->sslHandle);
    SSL_free (c->sslHandle);
  }
  if (c->sslContext)
    SSL_CTX_free (c->sslContext);

  free (c);
}

char * status_message (int code)
{
  char * message = NULL;
  switch (code) {
  case SSL_ERROR_WANT_READ:
    message = "Be advised no there is an SSL error: SSL_ERROR_WANT_READ";
    break;
  case SSL_ERROR_WANT_WRITE:
    message = "Be advised no there is an SSL error: SSL_ERROR_WANT_WRITE";
    break;
  case SSL_ERROR_ZERO_RETURN:
    message = "Be advised no there is an SSL error: SSL_ERROR_ZERO_RETURN";
    break;
  case 100:
    message = "Continue";
    break;
  case 101:
    message = "Switching Protocols";
    break;
  case 200:
    message = "OK";
    break;
  case 201:
    message = "Created";
    break;
  case 202:
    message = "Accepted";
    break;
  case 203:
    message = "Non-Authoritative Information";
    break;
  case 204:
    message = "No Content";
    break;
  case 205:
    message = "Reset Content";
    break;
  case 206:
    message = "Partial Content";
    break;
  case 300:
    message = "Multiple Choices";
    break;
  case 301:
    message = "Moved Permanently";
    break;
  case 302:
    message = "Moved Temporarily";
    break;
  case 303:
    message = "See Other";
    break;
  case 304:
    message = "Not Modified";
    break;
  case 305:
    message = "Use Proxy";
    break;
  case 400:
    message = "Bad Request";
    break;
  case 401:
    message = "Unauthorized";
    break;
  case 402:
    message = "Payment Required";
    break;
  case 403:
    message = "Forbidden";
    break;
  case 404:
    message = "Not Found";
    break;
  case 405:
    message = "Method Not Allowed";
    break;
  case 406:
    message = "Not Acceptable";
    break;
  case 407:
    message = "Proxy Authentication Required";
    break;
  case 408:
    message = "Request Time-out";
    break;
  case 409:
    message = "Conflict";
    break;
  case 410:
    message = "Gone";
    break;
  case 411:
    message = "Length Required";
    break;
  case 412:
    message = "Precondition Failed";
    break;
  case 413:
    message = "Request Entity Too Large";
    break;
  case 414:
    message = "Request-URI Too Large";
    break;
  case 415:
    message = "Unsupported Media Type";
    break;
  case 500:
    message = "Internal Server Error";
    break;
  case 501:
    message = "Not Implemented";
    break;
  case 502:
    message = "Bad Gateway";
    break;
  case 503:
    message = "Service Unavailable";
    break;
  case 504:
    message = "Gateway Time-out";
    break;
  case 505:
    message = "HTTP Version not supported";
    break;
  default:
    message = "Unknown http status code ";
    break;
  }
  return message;
}
int getIntCode (char * buffer)
{
  printf("buffer: %s\n",buffer);
  char * nextLine = strchr(buffer, '\n');
  char * status_line = NULL;
  status_line = ( char * )malloc( nextLine - buffer + 1 );
  status_line[nextLine - buffer] = '\0';
  memcpy( status_line, buffer, (nextLine - buffer));
  printf("status line: %s",status_line);
  char *a = strstr(status_line, "HTTP/1.0");
  char *b = strstr (status_line , "HTTP/1.1");
  if ( ( a != NULL ) )
    memmove(status_line,status_line+strlen("HTTP/1.0"),
            1+strlen(status_line+strlen("HTTP/1.1")));
  else if ( ( b != NULL ) )
    memmove(status_line,status_line+strlen("HTTP/1.1"),
            1+strlen(status_line+strlen("HTTP/1.1")));
  else
    status_line="0";
  return atoi(status_line);
}
int sslRead (char **response, connection *c)
{
  const int readSize = 512;
  char * rc = NULL;
  int received, ssl_error,count = 0;
  int code = 0;
  char buffer[512];
  if (c) {
    while ((received = SSL_read (c->sslHandle, buffer, readSize))) {
      buffer[received] = '\0';
      ssl_error = SSL_get_error (c->sslHandle, received);
      if (ssl_error !=0) {
        return ssl_error;
      }
      if ( (count == 0) )
        code = getIntCode(buffer);
      if (!rc)
        rc = malloc (readSize + 1);
      else
        rc = realloc (rc, (count + 1) * readSize + 1);
      if (received >0)
        strcat (rc, buffer);
      if ( (strstr(rc,"chunked") ) ) {
        if ( ( (rc[strlen(rc)-5] == '0') && (rc[strlen(rc)-4] == '\r')
               && (rc[strlen(rc)-3] == '\n')&& (rc[strlen(rc)-2] == '\r')
               && (rc[strlen(rc)-1] == '\n') ) ) {
          break;
        }
      }
      count++;
    }
  }
  *response = rc;
  return code;
}


void sslWrite (connection *c, char *text)
{
  if (c)
    SSL_write (c->sslHandle, text, strlen (text));
}

char * GoogleAuthLink ( config settings, char * scope )
{
  char * link=  malloc(2048);
  sprintf(link,
          "https://%s%s?redirect_uri=%s&response_type=code&client_id=%s&scope=%s",
          settings.authhost,settings.authpage,settings.redirect_uri,settings.client_id,
          scope);
  return link;
}

GoogleResponse GoogleAuthToken ( char * code, config settings)
{
  connection *c;
  GoogleResponse result;
  char *response=NULL;
  char *postvars = NULL;
  char *post = NULL;
  const char * postvars_format =
    "code=%s&client_id=%s&client_secret=%s&redirect_uri=%s&grant_type=authorization_code";
  const char * post_format = "POST %s HTTP/1.1\r\n"
                             "Host: %s\r\n"
                             "Content-type: application/x-www-form-urlencoded\r\n"
                             "Content-length: %zu\r\n\r\n"
                             "%s\r\n\r\n";
  size_t postvars_length = snprintf(NULL,0,postvars_format,code,
                                    settings.client_id,settings.client_secret,settings.redirect_uri) + 1;
  postvars = malloc(postvars_length);
  snprintf(postvars,postvars_length,postvars_format,code,settings.client_id,
           settings.client_secret,settings.redirect_uri);
  size_t post_length = snprintf(NULL,0,post_format, settings.tokenpage,
                                settings.tokenhost,strlen(postvars),postvars) + 1;
  post = malloc(post_length);
  snprintf(post,post_length,post_format, settings.tokenpage,settings.tokenhost,
           strlen(postvars),postvars);
  c = sslConnect (settings.tokenhost,443);
  sslWrite (c, post);
  int read = sslRead (&response,c);
  result.message = response;
  result.http_code = read;
  result.http_message = status_message(read);
  result.error = find_between(response,"\"error\": \"","\"");
  result.error_description = find_between(response,"\"error_description\": \"",
                                          "\"");
  sslDisconnect (c);
  free(post);
  free(postvars);
  return result;
}
GoogleResponse GoogleAuthRefreshToken ( config settings)
{
  connection *c;
  GoogleResponse result;
  char *response=NULL;
  char *postvars = NULL;
  char *post = NULL;
  const char * postvars_format =
    "refresh_token=%s&client_id=%s&client_secret=%s&redirect_uri=%s&grant_type=refresh_token";
  const char * post_format = "POST %s HTTP/1.1\r\n"
                             "Host: %s\r\n"
                             "Content-type: application/x-www-form-urlencoded\r\n"
                             "Content-length: %zu\r\n\r\n"
                             "%s\r\n\r\n";
  size_t postvars_length = snprintf(NULL,0,postvars_format,settings.refresh_token,
                                    settings.client_id,settings.client_secret,settings.redirect_uri) + 1;
  postvars = malloc(postvars_length);
  snprintf(postvars,postvars_length,postvars_format,settings.refresh_token,
           settings.client_id,
           settings.client_secret,settings.redirect_uri);
  size_t post_length = snprintf(NULL,0,post_format, settings.tokenpage,
                                settings.tokenhost,strlen(postvars),postvars) + 1;
  post = malloc(post_length);
  snprintf(post,post_length,post_format, settings.tokenpage,settings.tokenhost,
           strlen(postvars),postvars);
  c = sslConnect (settings.tokenhost,443);
  int read = sslRead (&response,c);
  result.message = response;
  result.http_code = read;
  result.http_message = status_message(read);
  result.error = find_between(response,"\"error\": \"","\"");
  result.error_description = find_between(response,"\"error_description\": \"",
                                          "\"");
  result.message = response;
  sslDisconnect (c);
  free(post);
  free(postvars);
  return result;
}

