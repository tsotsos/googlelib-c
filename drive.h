/** @file drive.h
 *  @brief Function prototypes for Google Drive Api.
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
#ifndef DRIVE_H_
#define DRIVE_H_
#include "google.h"
#include "dprint.h"
#include <ctype.h>
#define DRIVE_FILES "drive/v2/files"
#define DRIVE_VERSION "v2"
#define DRIVE_ABOUT "about"
#define d_free(p) { free(p); p = 0; }

int urlencode(char *dest, const char *src);

int DriveAbout (char **dest, char *token, char *includeSubscribed, long maxChangeIdCount,
                    long startChangeId );

#endif // DRIVE_H_




