/** @file googlelib.h
 *  @brief Function prototypes for making requests to Google.
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
 * @brief Holding connection info
 * @param protocol: TCP protocol
 * @param code: Status code number
 * @param message: String message
 *
 * Holding http status info
 */
typedef struct {
  char *protocol;
  int code;
  char *message;
} http_status;
/**
 * @brief Holding connection info
 * @param socket: stores socket
 * @param sslHandle: stores SSL
 * @param sslContext: SSL_CTX struct
 *
 * Holding connection info
 */
typedef struct {
  int socket;
  SSL *sslHandle;
  SSL_CTX *sslContext;
} connection;
/**
 * @brief Holding settings
 * @param authhost: Google server for authentication (accounts.google.com)
 * @param authpage: Authenticatiion page (/o/oauth2/auth)
 * @param tokenhost: Token server (googleapis.com)
 * @param tokenpage: Token page
 * @param client_id: Google application client id
 * @param client_secret: Google application client secret
 * @param redirect_uri: Give redirect uri
 * @param refresh_token: Access token
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
  char * refresh_token;
} config;
/**
 * @brief Establishes a tcp connection.
 * @param server_addr: Server address
 * @param server_port: Server port
 *
 * Establishes a TCP connection.
 **/
int tcpConnect (char * server_addr, int server_port);

/**
 * @brief Establishes a connection using SSL layer
 * @param  server_addr: Server address
 * @param  server_port: Server port
 *
 * Establishes a connection using SSL layer and handles
 * connection errors.
 **/
connection * sslConnect (char * server_addr,int server_port);

/**
 * @brief Stops connection
 * @param c: connection struct
 *
 * Closes socket and free the SSL connection.
 **/
void sslDisconnect (connection *c);

/**
 * @brief Error handling for SSL-layer
 * @param ssl_error: SSL error cathced by connection
 *
 * @return Message for SSL error.
 *
 **/
char *sslError (int32_t ssl_error);

/**
 * @brief Converts status line string to integer
 * @param buffer: Input Buffer
 * 
 * @return HTTP status code Int.
 *
 **/
http_status HttpStatus (char * buffer);

/**
 * @brief Wrapper for SSL_read
 * @param c: connection struct
 *
 * Utilize SSL_read to receive chunked encoding connections
 * until the expected response is back. This mechanism
 * controled by last 5 characters.
 * They should be : 0-CR-LF-CR-LF.
 *
 **/
char * sslRead (connection *c);

/**
 * @brief Wrapper for SSL_write
 * @param c: connection struct
 * @param text: text to send
 *
 * Simplify SSL_write if the (struct) connection exists
 **/
void sslWrite (connection * c, char *text);

/**
 * @brief Formats a string with link for google authorization
 * @param settings: Config struct which icludes settings
 * @param scope: Scopes from google api. For its values
 * see at : https://developers.google.com/+/web/api/rest/oauth
 *
 * Use this function to force user authorize your application.
 * 
 **/
char * GoogleAuthLink ( config settings, char * scope );

/**
 * @brief Sends a POST request to exchange
 * authentication code with accsess token
 * @param code: string with access code from user
 * @param settings: config struct which icludes settings
 *
 * This function returns a full HTTP response. Almost every time this
 * response is in json format. There is a function for handling the Json
 * string * at gapi.h.
 **/
char * GoogleAuthToken ( char * code, config settings);

/**
 * @brief Sends a POST request in order to refresh token
 * @param settings: config struct which icludes settings
 *
 **/
char * GoogleAuthRefreshToken ( config settings);
#endif // GOOGLELIB_H_

