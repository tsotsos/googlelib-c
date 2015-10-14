/** @file google.h
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
#ifndef GOOGLE_H_
#define GOOGLE_H_
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

#define GOOGLEAPI_HOST "www.googleapis.com"
#define GOOGLEAUTH_HOST "accounts.google.com"
#define GOOGLEAPI_PORT 443
#define GOOGLEAUTHPAGE "/o/oauth2/auth"
#define GOOGLETOKENPAGE "/oauth2/v3/token"
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
 * @brief Holding complete google response
 * @param http_code: HTTP status code
 * @param http_message: Readable HTTP status
 * @param error: Google Error name
 * @param error_description: Google error description
 * @param message: the full message retrieved by server
 *
 * Holding Google response message and status.
 */
typedef struct {
        int http_code;
        char *http_message;
        char *error;
        char *error_description;
        char *message;
} GoogleResponse;
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
        char * client_id;
        char * client_secret;
        char * redirect_uri;
        char * refresh_token;
        char * access_token;
} config;

/**
 * @brief Concatenate function.
 * @param s1:  string
 * @param s2:  string
 *
 * Concatenates two strings
 **/

char *concat ( char *s1, char *s2 );
/**
 * @brief Triming function.
 * @param input:  string
 *
 * Remove spaces of a string
 **/
char *trim ( char *input );

/**
 * @brief Finds between two strings
 * @param input:  input string
 * @param first:  string, starting limit
 * @param last :  string, last limit
 *
 * Gets the containing string between two others (first and last)
 **/

char *find_between ( char *response,char *first, char *last );
/**
 * @brief Reads a file
 * @param filename:  The filename
 *
 * Reads a file to buffer *char
 **/
char * ReadFile ( char * filename );

/**
 * @brief Parses a Json style string
 * @param json:  Json style string
 * @param value:  The wanted value
 *
 * Finds a value based on its name in a Json string
 **/
char* parseJson ( char *json, char *value );

/**
* @brief Find a value of config
* @param string:  Config string
* @param value:  The wanted value
*
* Finds a value based on its name in a Config string
**/
char * getValue ( char * string, char * value );

/**
 * @brief Get the settings from config file
 * @param filename:  Config filename
 *
 * Stores the settings from config file to a config
 * struct.
 **/
config getSettings ( char *filename );

/**
 * @brief Saves settings to file
 * @param filename:  Config filename
 * @param name: Name of value to add o replace
 * @param value:  The value
 *
 * Saves/replace the wanted value by its name (preferable to config file)
 **/
void setSetting ( char * filename, char *name , char *value );

/**
 * @brief Establishes a tcp connection.
 * @param server_addr: Server address
 * @param server_port: Server port
 *
 * Establishes a TCP connection.
 **/
int tcpConnect ( char * server_addr, int server_port );

/**
 * @brief Establishes a connection using SSL layer
 * @param  server_addr: Server address
 * @param  server_port: Server port
 *
 * Establishes a connection using SSL layer and handles
 * connection errors.
 **/
connection * sslConnect ( char * server_addr,int server_port );

/**
 * @brief Stops connection
 * @param c: connection struct
 *
 * Closes socket and free the SSL connection.
 **/
void sslDisconnect ( connection *c );

/**
 * @brief Error handling for SSL-layer
 * @param ssl_error: SSL error cathced by connection
 *
 * @return Message for SSL error.
 *
 **/
char *sslError ( int32_t ssl_error );

/**
 * @brief HTTP status message and SSL messages
 * @param code: HTTP status code or SSL error code.
 *
 * @return Message of HTTP status or SSL error if any.
 *
 **/
char * http_status_messages ( int code );
/**
 * @brief Converts status line string to integer
 * @param buffer: Input Buffer
 *
 * @return HTTP status code Int.
 *
 **/
int getIntCode ( char * buffer );

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
int sslRead ( char **response, connection *c );

/**
 * @brief Wrapper for SSL_write
 * @param c: connection struct
 * @param text: text to send
 *
 * Simplify SSL_write if the (struct) connection exists
 **/
void sslWrite ( connection * c, char *text );

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
 * @brief Formats a Header string to exchange code with access_token
 * @param code: string with access code from user
 * @param settings: config struct which icludes settings
 *
 * This function returns GoogleResponse struct. Almost every time
 * 'message' is n json format.
 **/
char * HeadersAuthToken ( char *code, config settings );

/**
 * @brief Properly formats a char* with headers for token refresh
 * @param settings: config struct which icludes settings
 *
 **/
char * HeadersRefreshToken ( config settings );

/**
 * @brief Connects with google using the given headers
 * @param headers: Headers as formed by functions provided.
 *
 **/
GoogleResponse GoogleConnect ( char * headers );

/**
 * @brief Handles response from google api.
 * @param response:  response string
 *
 * Stores the google response if it is type of "application/json"
 * to an 'array' of structs (Json).
 **/
Json * parseResponse ( char * response );
#endif // GOOGLE_H_



