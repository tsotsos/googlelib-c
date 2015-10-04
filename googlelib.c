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
#include "googlelib.h"
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

// Establish a connection using an SSL layer
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

char *sslRead (connection *c)
{
   const int readSize = 1024;
   char *rc = NULL;
   int received, count = 0;
   char buffer[1024];
   if (c) {
      while ((received = SSL_read (c->sslHandle, buffer, readSize))) {
         if (!rc)
            rc = malloc (readSize * sizeof (char*) + 1);
         else
            rc = realloc (rc, (count + 1) *
                          readSize * sizeof (char*) + 1);

         buffer[received] = '\0';
         if (received > 0)
            strcat (rc, buffer);
         if ( ( (rc[strlen(rc)-5] == '0') && (rc[strlen(rc)-4] == '\r') && (rc[strlen(rc)-3] == '\n')&& (rc[strlen(rc)-2] == '\r')&& (rc[strlen(rc)-1] == '\n') ) ) {
            break;
         }
         count++;
      }
   }
   return rc;
}


void sslWrite (connection *c, char *text)
{
   if (c)
      SSL_write (c->sslHandle, text, strlen (text));
}

char * GoogleAuthLink ( config settings )
{
   char * link=  malloc(2048);
   sprintf(link,"https://%s%s?redirect_uri=%s&response_type=code&client_id=%s&scope=profile",settings.authhost,settings.authpage,settings.redirect_uri,settings.client_id);
   return link;
}
char * GoogleAuthToken ( char * code, config settings)
{
   connection *c;
   char *response;
   char *postvars = NULL;
   char *post = NULL;
   const char * postvars_format = "code=%s&client_id=%s&client_secret=%s&redirect_uri=%s&grant_type=authorization_code";
   const char * post_format = "POST %s HTTP/1.1\r\n"
                              "Host: %s\r\n"
                              "Content-type: application/x-www-form-urlencoded\r\n"
                              "Content-length: %zu\r\n\r\n"
                              "%s\r\n\r\n";
   size_t postvars_length = snprintf(NULL,0,postvars_format,code,settings.client_id,settings.client_secret,settings.redirect_uri) + 1;
   postvars = malloc(postvars_length * sizeof(char));
   snprintf(postvars,postvars_length,postvars_format,code,settings.client_id,settings.client_secret,settings.redirect_uri);
   size_t post_length = snprintf(NULL,0,post_format, settings.tokenpage,settings.tokenhost,strlen(postvars),postvars) + 1;
   post = malloc(post_length * sizeof(char));
   snprintf(post,post_length,post_format, settings.tokenpage,settings.tokenhost,strlen(postvars),postvars);
   c = sslConnect (settings.tokenhost,443);
   sslWrite (c, post);
   response = sslRead (c);
   sslDisconnect (c);
   free(post);
   free(postvars);
   return response;
}


