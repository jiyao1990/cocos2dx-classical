/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-classical
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __lpk__lpk__
#define __lpk__lpk__

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    
// to debug output
#define DEBUG_LPK
    
/**
 * define generic lpk archive information. 
 * lpk archive header ('LPK\x1A')
 */
#define LPK_MAGIC 0x1A4B504C
    
// invalid block index
#define LPK_INDEX_INVALID 0xFFFFFFFF

// hash initial
#define LPK_HASH_TAG_TABLE_INDEX 0
#define LPK_HASH_TAG_NAME_A 0x100
#define LPK_HASH_TAG_NAME_B 0x200
    
/* define values used by flags. */
#define LPK_FLAG_USED			0x80000000	/* entry is used */
#define LPK_FLAG_DELETED        0x40000000  /* file is deleted */
#define LPK_FLAG_COMPRESSED		0x00000080  /* file is compressed. */
#define LPK_FLAG_ENCRYPTED		0x00008000	/* indicates whether file is encrypted. */
#define LPK_FLAG_CRC			0x20000000	/* compressed block offset table has CRC checksum. */

// to get compress algorithm
#define LPK_MASK_COMPRESSED 0x0000007f
#define LPK_SHIFT_COMPRESSED 0

// to get encrypt algorithm
#define LPK_MASK_ENCRYPTED 0x00007f00
#define LPK_SHIFT_ENCRYPTED 8
    
// error code
#define LPK_SUCCESS 0
#define LPK_ERROR_OPEN			-1		/* open error on file. */
#define LPK_ERROR_CLOSE			-2		/* close error on file. */
#define LPK_ERROR_SEEK			-3		/* lseek error on file. */
#define LPK_ERROR_READ			-4		/* read error on file. */
#define LPK_ERROR_WRITE			-5		/* write error on file. */
#define LPK_ERROR_MALLOC		-6		/* memory allocation error. */
#define LPK_ERROR_FORMAT		-7		/* format errror. */
#define LPK_ERROR_SIZE			-9		/* buffer size is to small. */
#define LPK_ERROR_EXIST			-10		/* file or block does not exist in archive. */
#define LPK_ERROR_DECRYPT		-11		/* we don't know the decryption seed. */
#define LPK_ERROR_UNPACK		-12		/* error on unpacking file. */
    
typedef enum {
    LPKC_DEFAULT,
    LPKC_NONE,
    LPKC_ZLIB
} LPKCompressAlgorithm;
    
typedef enum {
    LPKE_DEFAULT,
    LPKE_NONE,
    LPKE_XOR,
    LPKE_TEA,
    LPKE_XXTEA
} LPKEncryptAlgorithm;
    
typedef enum {
    LPKP_DEFAULT,
    LPKP_IOS,
    LPKP_ANDROID
} LPKPlatform;
    
/* lpk archive header. */
typedef struct {
    uint32_t lpk_magic;         /* the 0x1A4B504C ('LPK\x1A') signature. */
    uint32_t header_size;		/* lpk archive header size. */
    uint32_t archive_size;		/* size of lpk archive. */
    uint16_t version;           /* 0000, start from zero */
    uint16_t block_size;		/* size of file block is (512 * 2 ^ block size). */
    uint32_t hash_table_offset;	/* file position of lpk_hash. */
    uint32_t hash_table_count;	/* number of entries in hash table. */
    uint32_t deleted_hash;      /* head of deleted hash link */
} lpk_header;

/* hash entry, all files in the archive are searched by their hashes. */
typedef struct {
    uint32_t hash_i;        /* the index hash */
    uint32_t hash_a;		/* the first two uint32_ts are the encrypted file. */
    uint32_t hash_b;		/* the first two uint32_ts are the encrypted file. */
    uint16_t locale;		/* locale information, in Windows LCID. */
    uint16_t platform;      /* platform information and zero is default. */
    uint32_t offset;		/* block file starting position in the archive. */
    uint32_t packed_size;	/* file size after processed, i.e., compress, or encrypt, or both). */
    uint32_t file_size;     /* file size. */
    uint32_t flags;         /* flags. */
    
    /*
     * next hash to the file with same name but different locale and platform, 
     * no next if it is LPK_INDEX_INVALID
     */
    uint32_t next_hash;
    
    /*
     * previous hash to the file with same name but different locale and platform,
     * no previous if it is LPK_INDEX_INVALID
     */
    uint32_t prev_hash;
    
#ifdef DEBUG_LPK
    char filename[128];
#endif
} lpk_hash;
    
typedef struct {
    FILE* fp;
    lpk_header h; // header
    lpk_hash* het; // hash entry table
    uint32_t files; // file count
} lpk_file;
    
extern int lpk_open_file(lpk_file* lpk, const char* filepath);
extern int lpk_close_file(lpk_file* lpk);
extern uint32_t lpk_get_file_hash_table_index(lpk_file* lpk, const char* filepath, uint16_t locale, LPKPlatform platform);
extern uint32_t lpk_get_file_size(lpk_file* lpk, const char* filepath, uint16_t locale, LPKPlatform platform);
extern uint8_t* lpk_extract_file(lpk_file* lpk, const char* filepath, uint32_t* size, const char* key, const uint32_t keyLen, uint16_t locale, LPKPlatform platform);
extern int lpk_apply_patch(lpk_file* lpk, lpk_file* patch);
extern int lpk_get_used_hash_count(lpk_file* lpk);
extern void lpk_debug_output(lpk_file* lpk);
    
#ifdef __cplusplus
}
#endif
    
#endif /* defined(__lpk__lpk__) */
