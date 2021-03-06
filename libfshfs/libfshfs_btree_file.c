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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfshfs_btree_file.h"
#include "libfshfs_btree_header.h"
#include "libfshfs_btree_node_descriptor.h"
#include "libfshfs_btree_node_vector.h"
#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"

#include "fshfs_btree.h"

/* Creates a B-tree file
 * Make sure the value btree_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_initialize(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_initialize";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( *btree_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file value already set.",
		 function );

		return( -1 );
	}
	*btree_file = memory_allocate_structure(
	               libfshfs_btree_file_t );

	if( *btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_file,
	     0,
	     sizeof( libfshfs_btree_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree file.",
		 function );

		memory_free(
		 *btree_file );

		*btree_file = NULL;

		return( -1 );
	}
	if( libfshfs_btree_header_initialize(
	     &( ( *btree_file )->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_file != NULL )
	{
		memory_free(
		 *btree_file );

		*btree_file = NULL;
	}
	return( -1 );
}

/* Frees B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_free(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_free";
	int result            = 1;

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( *btree_file != NULL )
	{
		if( libfshfs_btree_header_free(
		     &( ( *btree_file )->header ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header.",
			 function );

			result = -1;
		}
		if( libfdata_vector_free(
		     &( ( *btree_file )->nodes_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free B-tree nodes vector.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *btree_file )->nodes_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free B-tree nodes cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *btree_file );

		*btree_file = NULL;
	}
	return( result );
}

/* Retrieves a specific B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_get_node_by_number(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     int node_number,
     libfshfs_btree_node_t **node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_get_node_by_number";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes vector.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes cache.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     btree_file->nodes_vector,
	     (intptr_t *) file_io_handle,
	     btree_file->nodes_cache,
	     node_number,
	     (intptr_t **) node,
	     0,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree node: %d.",
		 function,
		 node_number );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the B-tree root node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_get_root_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t **root_node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_get_root_node";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes vector.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes cache.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     btree_file->nodes_vector,
	     (intptr_t *) file_io_handle,
	     btree_file->nodes_cache,
	     (int) btree_file->header->root_node_number,
	     (intptr_t **) root_node,
	     0,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node: %" PRIu32 ".",
		 function,
		 btree_file->header->root_node_number );

		return( -1 );
	}
	return( 1 );
}

/* Reads the B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_read(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	uint8_t header_node_data[ 512 ];

	libfshfs_btree_node_descriptor_t *header_node_descriptor = NULL;
	static char *function                                    = "libfshfs_btree_file_read";
	ssize_t read_count                                       = 0;
	off64_t file_offset                                      = 0;

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file - nodes vector already set.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file - nodes cache already set.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	/* Read the header record first to determine the B-tree node size.
	 */
	file_offset = fork_descriptor->extents[ 0 ][ 0 ] * io_handle->allocation_block_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading B-tree header node at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek B-tree header node offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              header_node_data,
	              512,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header node data.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_descriptor_initialize(
	     &header_node_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree header node descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_descriptor_read_data(
	     header_node_descriptor,
	     header_node_data,
	     512,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header node descriptor.",
		 function );

		goto on_error;
	}
	if( header_node_descriptor->type != LIBFSHFS_BTREE_NODE_TYPE_HEADER_NODE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unuspported B-tree header node type.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_descriptor_free(
	     &header_node_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree header node descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_header_read_data(
	     btree_file->header,
	     &( header_node_data[ sizeof( fshfs_btree_node_descriptor_t ) ] ),
	     512 - sizeof( fshfs_btree_node_descriptor_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header.",
		 function );

		goto on_error;
	}
	/* Read the root node using the nodes vector
	 */
	if( libfshfs_btree_node_vector_initialize(
	     &( btree_file->nodes_vector ),
	     io_handle,
	     btree_file->header->node_size,
	     fork_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree nodes vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( btree_file->nodes_cache ),
	     LIBFSHFS_MAXIMUM_CACHE_ENTRIES_BTREE_FILE_ALLOCATION_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree nodes cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( btree_file->nodes_cache != NULL )
	{
		libfcache_cache_free(
		 &( btree_file->nodes_cache ),
		 NULL );
	}
	if( btree_file->nodes_vector != NULL )
	{
		libfdata_vector_free(
		 &( btree_file->nodes_vector ),
		 NULL );
	}
	if( header_node_descriptor != NULL )
	{
		libfshfs_btree_node_descriptor_free(
		 &header_node_descriptor,
		 NULL );
	}
	return( -1 );
}

