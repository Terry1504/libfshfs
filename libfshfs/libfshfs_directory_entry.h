/*
 * Directory entry functions
 *
 * Copyright (C) 2009-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSHFS_DIRECTORY_ENTRY_H )
#define _LIBFSHFS_DIRECTORY_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_directory_entry libfshfs_directory_entry_t;

struct libfshfs_directory_entry
{
	/* Record type
	 */
	uint16_t record_type;

	/* Name size
	 */
	uint16_t name_size;

	/* Name
	 */
	uint8_t *name;

	/* Parent identifier
	 */
	uint32_t parent_identifier;

	/* File or directory catalog record
	 */
	intptr_t *catalog_record;
};

int libfshfs_directory_entry_initialize(
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_directory_entry_free(
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *identifier,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf8_name_size(
     libfshfs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf8_name(
     libfshfs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf16_name_size(
     libfshfs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf16_name(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_DIRECTORY_ENTRY_H ) */

