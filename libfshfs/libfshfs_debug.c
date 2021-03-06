/*
 * Debug functions
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfdatetime.h"
#include "libfshfs_libuna.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the volume attribute flags
 */
void libfshfs_debug_print_volume_attribute_flags(
      uint32_t volume_attribute_flags )
{
	if( ( volume_attribute_flags & 0x00000040UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume hardware lock (kHFSVolumeHardwareLockBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000080UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume unmounted (kHFSVolumeUnmountedBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000100UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume spared blocks (kHFSVolumeSparedBlocksBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000200UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume no cache required (kHFSVolumeNoCacheRequiredBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000400UL ) != 0 )
	{
		libcnotify_printf(
		 "\tBoot volume inconsistent (kHFSBootVolumeInconsistentBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000800UL ) != 0 )
	{
		libcnotify_printf(
		 "\tCatalog node identifiers reused (kHFSCatalogNodeIDsReusedBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00001000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume journaled (kHFSVolumeJournaledBit)\n" );
	}

	if( ( volume_attribute_flags & 0x00004000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume software lock (kHFSVolumeSoftwareLockBit)\n" );
	}

	if( ( volume_attribute_flags & 0x40000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSContentProtectionBit)\n" );
	}
	if( ( volume_attribute_flags & 0x80000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSUnusedNodeFixBit)\n" );
	}
}

/* Prints the B-tree node type
 */
const char *libfshfs_debug_print_btree_node_type(
             uint8_t btree_node_type )
{
	switch( btree_node_type )
	{
		case LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE:
			return( "Leaf node (kBTLeafNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE:
			return( "Index node (kBTIndexNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_HEADER_NODE:
			return( "Header node (kBTHeaderNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_MAP_NODE:
			return( "Map node (kBTMapNode)" );
	}
	return( "Unknown" );
}

/* Prints the catalog record type
 */
const char *libfshfs_debug_print_catalog_record_type(
             uint16_t record_type )
{
	switch( record_type )
	{
		case 0x0001:
			return( "HFS+ directory record (kHFSPlusFolderRecord)" );

		case 0x0002:
			return( "HFS+ file record (kHFSPlusFileRecord)" );

		case 0x0003:
			return( "HFS+ directory thread record (kHFSPlusFolderThreadRecord)" );

		case 0x0004:
			return( "HFS+ file thread record (kHFSPlusFileThreadRecord)" );

		case 0x0100:
			return( "HFS directory record (kHFSFolderRecord)" );

		case 0x0200:
			return( "HFS file record (kHFSFileRecord)" );

		case 0x0300:
			return( "HFS directory thread record (kHFSFolderThreadRecord)" );

		case 0x0400:
			return( "HFS file thread record (kHFSFileThreadRecord)" );
	}
	return( "Unknown" );
}

/* Prints a HFS value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_hfs_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_hfs_time_t *hfs_time = NULL;
	static char *function             = "libfshfs_debug_print_hfs_time_value";

	if( libfdatetime_hfs_time_initialize(
	     &hfs_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create HFS time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_hfs_time_copy_from_byte_stream(
	     hfs_time,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to HFS time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_hfs_time_copy_to_utf8_string(
	     hfs_time,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy HFS time to string.",
		 function );

		goto on_error;
	}
/* TODO handle local time */
	libcnotify_printf(
	 "%s: %s: %s UTC\n",
	 function_name,
	 value_name,
	 date_time_string );

	if( libfdatetime_hfs_time_free(
	     &hfs_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free HFS time.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( hfs_time != NULL )
	{
		libfdatetime_hfs_time_free(
		 &hfs_time,
		 NULL );
	}
	return( -1 );
}

/* Prints an UTF-16 string value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_utf16_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libfshfs_debug_print_utf16_string_value";
	size_t string_size         = 0;
	int result                 = 0;

	if( ( byte_stream == NULL )
	 || ( byte_stream_size == 0 ) )
	{
		libcnotify_printf(
		 "%s: %s: \n",
		 function_name,
		 value_name );

		return( 1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  &string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  &string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of string.",
		 function );

		goto on_error;
	}
	if( ( string_size > (size_t) SSIZE_MAX )
	 || ( ( sizeof( system_character_t ) * string_size ) > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		goto on_error;
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
		  (libuna_utf16_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
		  (libuna_utf8_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 string );

	memory_free(
	 string );

	return( 1 );

on_error:
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

