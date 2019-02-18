/*****************************************************************************
 * Copyright (C) 1994-2007 YAE
 * $Id$
 *
 * Author: Doug Kwan <chun_tak@yahoo.com>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************
 *
 * gcr.c
 * GCR encoding/decoding utility
 *
 */
#include <iostream>
#include "gcr.h"

#define DOS_TRACK_BYTES 4096 
#define RAW_TRACK_BITS (RAW_TRACK_BYTES*8) 

static BYTE	GCR_encoding_table[64] = {
	0x96, 0x97, 0x9A, 0x9B, 0x9D, 0x9E, 0x9F, 0xA6,
	0xA7, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB2, 0xB3,
	0xB4, 0xB5, 0xB6, 0xB7, 0xB9, 0xBA, 0xBB, 0xBC,
	0xBD, 0xBE, 0xBF, 0xCB, 0xCD, 0xCE, 0xCF, 0xD3,
	0xD6, 0xD7, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,
	0xDF, 0xE5, 0xE6, 0xE7, 0xE9, 0xEA, 0xEB, 0xEC,
	0xED, 0xEE, 0xEF, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,
	0xF7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };

static BYTE	GCR_decoding_table[256];
static	int	Swap_Bit[4] = { 0, 2, 1, 3 }; /* swap lower 2 bits */
static BYTE	GCR_buffer[256];
static BYTE	GCR_buffer2[86];

static int	Position=0;
static BYTE	*Track_Nibble;

/* physical sector no. to DOS 3.3 logical sector no. table */
static int	Logical_Sector[16] = {
	0x0, 0x7, 0xE, 0x6, 0xD, 0x5, 0xC, 0x4,
	0xB, 0x3, 0xA, 0x2, 0x9, 0x1, 0x8, 0xF };

static int	Physical_Sector[16];

/* static function prototypes */

static void init_GCR_table(void);
static BYTE gcr_read_nibble(void);
static void gcr_write_nibble( BYTE );
static void decode62( BYTE* );
static void encode62( BYTE* );
static void FM_encode( BYTE );
static BYTE FM_decode(void);
static void write_sync( int );
static int read_address_field( int*, int*, int* );
static void write_address_field( int, int, int );
static int read_data_field(void);
static void write_data_field(void);

#define FM_ENCODE(x)	gcrWriteNibble( ((x) >> 1) | 0xAA );\
			gcrWriteNibble( (x) | 0xAA )

static void init_GCR_table(void)
{
	static int	initialized = 0;
	int		i;

	if ( !initialized ) {	
	   for( i = 0; i < 64; i++ )
	      GCR_decoding_table[GCR_encoding_table[i]] = i;
	   for( i = 0; i < 16; i++ )
	      Physical_Sector[Logical_Sector[i]] = i;
	   initialized = 1;
	}
}

static BYTE gcr_read_nibble(void)
{ 
	BYTE	data;
	
	data = Track_Nibble[Position++];
	if ( Position >= RAW_TRACK_BYTES )
	   Position = 0;	
	return data;
}

static void gcr_write_nibble( BYTE data )
{
	Track_Nibble[Position++] = data;
	if ( Position >= RAW_TRACK_BYTES )
	   Position = 0;
}

static void decode62( BYTE *page )
{
	int	i, j;

	/* get 6 bits from GCR_buffer & 2 from GCR_buffer2 */
	for( i = 0, j = 86; i < 256; i++ ) {
	  if ( --j < 0 ) j = 85;
	  page[i] = (GCR_buffer[i] << 2) | Swap_Bit[GCR_buffer2[j] & 0x03];
	  GCR_buffer2[j] >>= 2;
	}
}

static void encode62( BYTE *page )
{
	int	i, j;

	/* 86 * 3 = 258, so the first two byte are encoded twice */
	GCR_buffer2[0] = Swap_Bit[page[1]&0x03];
	GCR_buffer2[1] = Swap_Bit[page[0]&0x03];

	/* save higher 6 bits in GCR_buffer and lower 2 bits in GCR_buffer2 */
	for( i = 255, j = 2; i >= 0; i--, j = j == 85? 0: j + 1 ) {
	   GCR_buffer2[j] = (GCR_buffer2[j] << 2) | Swap_Bit[page[i]&0x03];
	   GCR_buffer[i] = page[i] >> 2;
	}
	 
	/* clear off higher 2 bits of GCR_buffer2 set in the last call */
	for( i = 0; i < 86; i++ )
	   GCR_buffer2[i] &= 0x3f;
}

/*
 * write an FM encoded value, used in writing address fields 
 */
static void FM_encode( BYTE data )
{
	gcr_write_nibble( (data >> 1) | 0xAA );
	gcr_write_nibble( data | 0xAA );
}

/*
 * return an FM encoded value, used in reading address fields 
 */
static BYTE FM_decode(void)
{
	int		tmp; 

	/* C does not specify order of operand evaluation, don't
	 * merge the following two expression into one
	 */
	tmp = (gcr_read_nibble() << 1) | 0x01;
	return gcr_read_nibble() & tmp;
}

static void write_sync( int length )
{
	while( length-- )
	   gcr_write_nibble( 0xFF );
}

/*
 * read_address_field: try to read a address field in a track
 * returns 1 if succeed, 0 otherwise
 */
static int read_address_field( int *volume, int *track, int *sector )
{
	int	max_try;
	BYTE	nibble;

	max_try = 100; 
	while( --max_try ) {
	   nibble = gcr_read_nibble(); 
	   check_D5:	
	   if ( nibble != 0xD5 )
	      continue;
	   nibble = gcr_read_nibble();
	   if ( nibble != 0xAA )
	   goto check_D5;
	   nibble = gcr_read_nibble();
	   if ( nibble != 0x96 )
	      goto check_D5;
	   *volume = FM_decode();
	   *track = FM_decode();
	   *sector = FM_decode();
	   return ( *volume ^ *track ^ *sector ) == FM_decode() &&
	      gcr_read_nibble() == 0xDE;
	}
	return 0;
}

static void write_address_field( int volume, int track, int sector )
{
	/*
	 * write address mark
	 */
	gcr_write_nibble( 0xD5 );
	gcr_write_nibble( 0xAA );
	gcr_write_nibble( 0x96 );

	/*
	 * write Volume, Track, Sector & Check-sum
	 */
	FM_encode( volume );
	FM_encode( track );
	FM_encode( sector );
	FM_encode( volume ^ track ^ sector );

	/*
	 * write epilogue
	 */
	gcr_write_nibble( 0xDE );
	gcr_write_nibble( 0xAA );
	gcr_write_nibble( 0xEB );
}

/*
 * read_data_field: read_data_field into GCR_buffers, return 0 if fail
 */
static int read_data_field(void)
{
	int	i, max_try;
	BYTE	nibble, checksum;

	/*
	 * read data mark
	 */
	max_try = 32;
	while( --max_try ) {
	   nibble = gcr_read_nibble();
	check_D5:
	   if ( nibble != 0xD5 )
	      continue;
	   nibble = gcr_read_nibble();
	   if ( nibble != 0xAA )
	      goto check_D5;
	   nibble = gcr_read_nibble();
	   if ( nibble == 0xAD )
	      break;
	}
	if ( !max_try ) /* fails to get address mark */
	   return 0;

 	for( i = 0x55, checksum = 0; i >= 0; i-- ) {
	   checksum ^= GCR_decoding_table[gcr_read_nibble()]; 
	   GCR_buffer2[i] = checksum;
	}

	for( i = 0; i < 256; i++ ) {
	   checksum ^= GCR_decoding_table[gcr_read_nibble()]; 
	   GCR_buffer[i] = checksum;
	}

	/* verify sector checksum */
	if ( checksum ^ GCR_decoding_table[gcr_read_nibble()] )
	   return 0; 

	/* check epilogue */
	return gcr_read_nibble() == 0xDE && gcr_read_nibble() == 0xAA;
}

static void write_data_field(void)
{
	int	i;
	BYTE	last, checksum;

	/* write prologue */
	gcr_write_nibble( 0xD5 );
	gcr_write_nibble( 0xAA );
	gcr_write_nibble( 0xAD );

	/* write GCR encode data */
 	for( i = 0x55, last = 0; i >= 0; i-- ) {
	   checksum = last^ GCR_buffer2[i];
	   gcr_write_nibble( GCR_encoding_table[checksum] );
	   last = GCR_buffer2[i];
	}
	for( i = 0; i < 256; i++ ) {
	   checksum = last ^ GCR_buffer[i];
	   gcr_write_nibble( GCR_encoding_table[checksum] );
	   last = GCR_buffer[i];
	}

	/* write checksum and epilogue */
	gcr_write_nibble( GCR_encoding_table[last] );
	gcr_write_nibble( 0xDE );
	gcr_write_nibble( 0xAA );
	gcr_write_nibble( 0xEB );
}

void SectorsToNibbles( BYTE *sectors, BYTE *nibbles, int volume, int track )
{
	int	i;

	init_GCR_table();
	Track_Nibble = nibbles;
	Position = 0;

	write_sync( 128 );

	for( i = 0; i < 16; i ++ ) {
	   encode62( sectors + Logical_Sector[i] * 0x100 );
	   write_sync( 16 );
	   write_address_field( volume, track, i );
	   write_sync( 8 );
	   write_data_field();
	}
}

int NibblesToSectors( BYTE *nibbles, BYTE *sectors, int volume, int track )
{
	int	i, scanned[16], max_try, sectors_read;
	int	vv, tt, ss;	/* volume, track no. and sector no. */
	FILE	*fp;

	init_GCR_table();
	Track_Nibble = nibbles;
	Position = 0;

	for( i = 0; i < 16; i++ )
	   scanned[i] = 0;
	sectors_read = 0;

	max_try = 200;
	while( --max_try ) { 
	   if ( !read_address_field( &vv, &tt, &ss ) )
	      continue;

	   if ( (volume && vv != volume ) || tt != track || ss < 0 || ss > 15 ){
	      printf("phy sector %d address field invalid\n", ss );
	      continue;	/* invalid values for vv, tt and ss, try again */
	   }

	   ss = Logical_Sector[ss];
	   if ( scanned[ss] )	/* sector has been read */
	      continue;

	   if ( read_data_field() ) {
	      decode62( sectors + ss * 0x100 );
	      scanned[ss] = 1;	/* this sector's ok */ 
	      sectors_read++;
	   }
	   else {
	      printf("fail reading data field of logical sector %d\n", ss );
	   }
	}

	/* if has failed to read any one sector, report error */
	if ( sectors_read == 16 )
	   return 1;
	else {
	   printf( "sectos_read = %d\n",sectors_read);
	   for( i = 0; i < 16; i++ )
	      if ( !scanned[i] )
	         printf( "sector %d(%d) corrupted\n", i, Physical_Sector[i] );
	   if((fp = fopen( ".track", "w"))!=NULL) {
	      fwrite( nibbles, 1, RAW_TRACK_BYTES, fp );
	      fclose(fp);
	   }
	   return 0;
	}
}
