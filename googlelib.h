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

/**
 * struct connection - Holding connection info
 * @socket: stores socket
 * @sslHandle: stores SSL
 * @sslContext: SSL_CTX struct
 *
 * Holding connection info
 */
typedef struct {
  int socket;
  SSL *sslHandle;
  SSL_CTX *sslContext;
} connection;
/**
 * struct config - Holding settings
 * @authhost: google server for authentication (accounts.google.com)
 * @authpage: authenticatiion page (/o/oauth2/auth)
 * @tokenhost: token server (googleapis.com)
 * @tokenpage: token page
 * @client_id: Google application client id
 * @client_secret: Google application client secret
 * @redirect_uri: Give redirect uri
 * @accesstoken: returned access token
 *
 * Holding settings info.
 */
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

int tcpConnect (char * server_addr, int server_port);

connection * sslConnect (char * server_addr,int server_port);

void sslDisconnect (connection *c);

char * sslRead (connection *c);

void sslWrite (connection * c, char *text);

char * GoogleAuthLink ( config settings, char * scope );

char * GoogleAuthToken ( char * code, config settings);

#endif // GOOGLELIB_H_

