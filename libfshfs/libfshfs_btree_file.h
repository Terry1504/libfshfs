/*
 * B-tree file functions
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

#if !defined( _LIBFSHFS_BTREE_FILE_H )
#define _LIBFSHFS_BTREE_FILE_H

#include <common.h>
#include <types.h>

#include "libfshfs_btree_header.h"
#include "libfshfs_btree_node.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_btree_file libfshfs_btree_file_t;

struct libfshfs_btree_file
{
	/* The header
	 */
	libfshfs_btree_header_t *header;

	/* The nodes vector
	 */
	libfdata_vector_t *nodes_vector;

	/* The nodes cache
	 */
	libfcache_cache_t *nodes_cache;
};

int libfshfs_btree_file_initialize(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error );

int libfshfs_btree_file_free(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error );

int libfshfs_btree_file_get_node_by_number(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     int node_number,
     libfshfs_btree_node_t **node,
     libcerror_error_t **error );

int libfshfs_btree_file_get_root_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t **root_node,
     libcerror_error_t **error );

int libfshfs_btree_file_read(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_BTREE_FILE_H ) */

