/*
 * Volume functions
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSHFS_INTERNAL_VOLUME_H )
#define _LIBFSHFS_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfshfs_extern.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_internal_volume libfshfs_internal_volume_t;

struct libfshfs_internal_volume
{
	/* The IO handle
	 */
	libfshfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;
};

LIBFSHFS_EXTERN \
int libfshfs_volume_initialize(
     libfshfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_free(
     libfshfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_signal_abort(
     libfshfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_open(
     libfshfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBFSHFS_EXTERN \
int libfshfs_volume_open_wide(
     libfshfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );
#endif

LIBFSHFS_EXTERN \
int libfshfs_volume_open_file_io_handle(
     libfshfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_close(
     libfshfs_volume_t *volume,
     libcerror_error_t **error );

int libfshfs_volume_open_read(
     libfshfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf8_label_size(
     libfshfs_volume_t *volume,
     size_t *utf8_label_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf8_label(
     libfshfs_volume_t *volume,
     uint8_t *utf8_label,
     size_t utf8_label_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf16_label_size(
     libfshfs_volume_t *volume,
     size_t *utf16_label_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf16_label(
     libfshfs_volume_t *volume,
     uint16_t *utf16_label,
     size_t utf16_label_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

