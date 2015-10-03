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
#ifndef GOOGLELIB_H_
#define GOOGLELIB_H_
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/*struct to define SSL connection */
typedef struct {
   int socket;
   SSL *sslHandle;
   SSL_CTX *sslContext;
} connection;
/*struct for settings */
typedef struct {
   char * authhost;
   char * authpage;
   char * tokenhost;
   char * tokenpage;
   char * client_id;
   char * client_secret;
   char * redirect_uri;
   char * accesstoken;
} config;
/* struct for json response */
typedef struct {
   char * name;
   char * value;
} Json;

/* TCP Connection */
int tcpConnect (char * server_addr, int server_port);

/* SSL Connection */
connection * sslConnect (char * server_addr,int server_port);

/* SSL Disconnect */
void sslDisconnect (connection *c);

/* SSL Read response */
char * sslRead (connection *c);

/* SSL Write conncetion */
void sslWrite (connection * c, char *text);

/* Returns the Google oAuth2 link for verification code */
char * GoogleAuthLink ( config settings );

/* Exchange verification code for access token */
char * GoogleAuthToken ( char * code, config settings);

#endif // GOOGLELIB_H_

