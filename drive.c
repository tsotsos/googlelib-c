/** @file drive.c
 *  @brief Functions to format properly headers for GoogleDrive APi
 *
 *  This file contains source that is responsible for making
 *  all headers for GoogleConnect function.
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
#include "drive.h"

int urlencode ( char *dest, const char *src )
{
        char *d;
        int i;
        for ( i=0, d=dest; src[i]; i++ ) {
                if ( isalnum ( src[i] ) ) * ( d++ ) = src[i];
                else {
                        sprintf ( d, "%%%02X", src[i] );
                        d += 3;
                }
        }
        *d = 0;
        return d-dest;
}
int DriveAbout ( char **dest, char *token, char *includeSubscribed,
                 long maxChangeIdCount,
                 long startChangeId )
{
        char * result = NULL;
        char * temp = NULL;
        int length = 0;
        if ( ( includeSubscribed != NULL ) ) {
                char *s = NULL;
                length = a_sprintf ( &s,"&includeSubscribed=%s",includeSubscribed );
                if ( !temp ) {
                        temp = malloc ( strlen ( s ) + 1 );
                        strncpy ( temp, s, strlen ( s ) + 1 );
                } else {
                        temp = realloc ( temp, strlen ( temp ) + strlen ( s ) + 1 );
                        strncat ( temp, s, strlen ( s ) + 1 );
                }
                free ( s );
        }
        if ( ( maxChangeIdCount >= 0 ) ) {
                char *s = NULL;
                length = a_sprintf ( &s, "&maxChangeIdCount=%lu",maxChangeIdCount );
                if ( !temp ) {
                        temp = malloc ( strlen ( s ) + 1 );
                        strncpy ( temp, s, strlen ( s ) + 1 );
                } else {
                        temp = realloc ( temp, strlen ( temp ) + strlen ( s ) + 1 );
                        strncat ( temp, s, strlen ( s ) + 1 );
                }
                free ( s );
        }
        if ( ( startChangeId >= 0 ) ) {
                char *s = NULL;
                length =  a_sprintf ( &s, "&startChangeId=%lu",startChangeId );
                if ( !temp ) {
                        temp = malloc ( strlen ( s ) + 1 );
                        strncpy ( temp, s, strlen ( s ) + 1 );
                } else {
                        temp = realloc ( temp, strlen ( temp ) + strlen ( s ) + 1 );
                        strncat ( temp, s, strlen ( s ) + 1 );
                }
                free ( s );
        }
        if ( temp )
                length = a_sprintf ( &result, "GET https://%s/drive/%s/%s?access_token=%s%s",
                                     GOOGLEAPI_HOST,DRIVE_VERSION,DRIVE_ABOUT,token,temp );
        else
                length = a_sprintf ( &result, "GET https://%s/drive/%s/%s?access_token=%s",
                                     GOOGLEAPI_HOST,DRIVE_VERSION,DRIVE_ABOUT,token );
        *dest = result;
        free ( temp );
        return length;
}

