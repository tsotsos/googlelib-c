/** @file dprint.c
 *  @brief Functions for dynamic malloc on printf.
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
#include "dprint.h"
int a_sprintf ( char **str, char* fmt, ... )
{
        va_list argp;
        va_start ( argp, fmt );
        char one_char[1];
        int len = vsnprintf ( one_char, 1, fmt, argp );
        if ( len < 1 ) {
                *str = NULL;
                return len;
        }
        va_end ( argp );

        *str = (char * ) malloc ( len+1 );
         if ( !str )
                return -1;
        va_start ( argp, fmt );
        vsnprintf ( *str, len+1, fmt, argp );
        va_end ( argp );
        return len;
}

