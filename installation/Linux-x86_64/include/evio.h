/*-----------------------------------------------------------------------------
 * Copyright (c) 2013 Jefferson Science Associates,
 *                    Thomas Jefferson National Accelerator Faciltiy,
 *
 * This software was developed under a United States Government license
 * described in the NOTICE file included as part of this distribution.
 *
 * JLAB Data Acquisition Group, 12000 Jefferson Ave., Newport News, VA 23606
 * Email: timmmer@jlab.org  Tel: (757) 269-5130
 *-----------------------------------------------------------------------------
 * 
 * Description:
 *  Event I/O test program
 *  
 * Author:  Elliott Wolin, June 2001
 *          Carl Timmer 2013, JLAB Data Acquisition Group
 *
 */


#ifndef __EVIO_h__
#define __EVIO_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** Evio format version, not the evio package version #. */
#define EV_VERSION 6

/** Size of block header in 32 bit words.
 *  Must never be smaller than 8, but can be set larger.*/
#define EV_HDSIZ 8
#define EV_HDSIZ_V6 14
#define EV_HDSIZ_BYTES 32
#define EV_HDSIZ_BYTES_V6 56


#ifndef S_SUCCESS
#define S_SUCCESS 0
#define S_FAILURE -1
#endif

/* see "Programming with POSIX threads' by Butenhof */
#define evio_err_abort(code,text) do { \
    fprintf (stderr, "%s at \"%s\":%d: %s\n", \
        text, __FILE__, __LINE__, strerror (code)); \
    exit (-1); \
} while (0)


#define S_EVFILE    		0x00730000	/**< evfile.msg Event File I/O */
#define S_EVFILE_TRUNC		0x40730001	/**< Event truncated on read/write */
#define S_EVFILE_BADBLOCK	0x40730002	/**< Bad block number encountered */
#define S_EVFILE_BADHANDLE	0x80730001	/**< Bad handle (file/stream not open) */
#define S_EVFILE_ALLOCFAIL	0x80730002	/**< Failed to allocate memory */
#define S_EVFILE_BADFILE	0x80730003	/**< File format error */
#define S_EVFILE_UNKOPTION	0x80730004	/**< Unknown option specified */
#define S_EVFILE_UNXPTDEOF	0x80730005	/**< Unexpected end of file while reading event */
#define S_EVFILE_BADSIZEREQ 0x80730006  /**< Invalid buffer size request to evIoct */
#define S_EVFILE_BADARG     0x80730007  /**< Invalid function argument */
#define S_EVFILE_BADMODE    0x80730008  /**< Wrong mode used in evOpen for this operation */

/**
 * @addtogroup swap
 * @{
 */

/* macros for swapping ints of various sizes */
#define EVIO_SWAP64(x) ( (((x) >> 56) & 0x00000000000000FFL) | \
                         (((x) >> 40) & 0x000000000000FF00L) | \
                         (((x) >> 24) & 0x0000000000FF0000L) | \
                         (((x) >> 8)  & 0x00000000FF000000L) | \
                         (((x) << 8)  & 0x000000FF00000000L) | \
                         (((x) << 24) & 0x0000FF0000000000L) | \
                         (((x) << 40) & 0x00FF000000000000L) | \
                         (((x) << 56) & 0xFF00000000000000L) )

#define EVIO_SWAP32(x) ( (((x) >> 24) & 0x000000FF) | \
                         (((x) >> 8)  & 0x0000FF00) | \
                         (((x) << 8)  & 0x00FF0000) | \
                         (((x) << 24) & 0xFF000000) )

#define EVIO_SWAP16(x) ( (((x) >> 8) & 0x00FF) | \
                         (((x) << 8) & 0xFF00) )

/** Calculate a 64 bit result from 2 32 bit inputs. */
#define EVIO_TO_64_BITS(low, high) ((((uint64_t) low) & 0xffffffffL) && \
                                    ((((uint64_t) high) << 32)))


/** @} */

#include <stdio.h>
#include <pthread.h>
#include <stddef.h>

#ifdef _MSC_VER
    typedef __int64 int64_t;	// Define it from MSVC's internal type
    #include "msinttypes.h"
    #define strcasecmp _stricmp
    #define strncasecmp strnicmp
#else
    #include <stdint.h>		  // Use the C99 official header
#endif


/**
 * This structure contains information about file
 * opened for either reading or writing.
 */
typedef struct evfilestruct {

    FILE    *file;         /**< pointer to file. */
    int      handle;       /**< handle used to access this structure. */
    int      rw;           /**< are we reading, writing, piping? */
    int      magic;        /**< magic number. */
    int      bigEndian;    /**< if big endian = 1 else 0 */
    int      byte_swapped; /**< bytes need swapping = 1 else 0 */
    int      version;      /**< evio FORMAT version number. */
    int      append;       /**< open buffer or file for writing in append mode = 1, else 0.
                                If append = 2, then an event was already been appended. */
    uint32_t eventCount;   /**< current number of events in (or written to) file/buffer
                            *   NOT including dictionary(ies). If the file being written to is split,
                            *   this value refers to all split files taken together. */

    /* block stuff */
    uint32_t *buf;         /**< For files, sockets, and reading buffers = pointer to
                            *   buffer of block-being-read / blocks-being-written.
                            *   When writing to file/socket/pipe, this buffer may
                            *   contain multiple blocks.
                            *   When writing to buffer, this points to block header
                            *   in block currently being written to (no separate
                            *   block buffer exists).
                            *   For reading ver 1-3 files, this points to block being
                            *   parsed (multiple block are read in at once) and pBuf
                            *   points to the beginning of actual buffer in memory. */
    uint32_t *pBuf;          /**< For reading ver 1-3 files, this points to the beginning
                            *   of actual buffer in memory. */
    uint32_t  *next;         /**< pointer to next word in buffer to be read/written. */
    uint32_t  left;          /**< # of valid 32 bit unread/unwritten words in buffer. */
    uint32_t  blksiz;        /**< size of block in 32 bit words - v3 or
                            *   size of actual data in block (including header) - v4. */
    uint32_t  blknum;        /**< block number of block being read/written. Next to be used, starting at 1. */
    int       blkNumDiff;    /**< When reading, the difference between blknum read in and
                            *   the expected (sequential) value. Used in debug message. */
    uint32_t  blkSizeTarget; /**< target size of block in 32 bit words (including block header). */
    uint32_t  blkEvCount;    /**< number of events written to block so far (including dictionary). */
    uint32_t  bufSize;       /**< When reading, size of block buffer (buf) in 32 bit words.
                            *   When writing file/sock/pipe, size of buffer being written to
                            *   that is actually being used (must be <= bufRealSize). */
    uint32_t  bufRealSize;   /**< When writing file/sock/pipe, total size of buffer being written to.
                            *   Amount of memory actually allocated in 32 bit words (not all may
                            *   be used). */
    uint32_t  blkEvMax;      /**< max number of events per block. */
    int       isLastBlock;   /**< 1 if buf contains last block of file/sock/buf, else 0. */
    uint32_t  blocksToParse; /**< reading file verions 1-3, # of blocks yet to be parsed. */


    /* file stuff: splitting, auto naming, internal buffer */
    char     *baseFileName;   /**< base name of file to be written to. */
    char     *fileName;       /**< actual name of file to be written to. */
    char     *runType;        /**< run type used in auto naming of split files. */
    int       specifierCount; /**< number of C printing int format specifiers in file name (0, 1, 2). */
    int       splitting;      /**< 0 if not splitting file, else 1. */
    int       lastEmptyBlockHeaderExists;/**< 1 if internal buffer has the last empty block header
                                        * written, else 0. */
    uint32_t *currentHeader;  /**< When writing to file/socket/pipe, this points to
                             *   current block header of block being written. */
    uint32_t  bytesToBuf;     /**< # bytes written to internal buffer including dict. */
    uint32_t  eventsToBuf;    /**< # events written to internal buffer including dictionary. */
    uint32_t  eventsToFile;   /**< # of events written to file including dictionary.
                             * If the file is being split, this value refers to the file
                             * currently being written to. */
    uint64_t  bytesToFile;    /**< # bytes flushed to the current file (including ending
                             *   empty block & dictionary), not the total in all split files. */
    uint32_t  streamId;       /**< stream id # used in auto naming of files. */
    uint32_t  runNumber;      /**< run # used in auto naming of split files. */
    uint32_t  splitNumber;    /**< number of next split file (used in auto naming). */
    uint64_t  split;          /**< # of bytes at which to split file when writing
                             *  (defaults to EV_SPLIT_SIZE, 1GB). */

    uint64_t  fileSize;       /**< size of file being read from, in bytes. */
    uint64_t  filePosition;   /**< how far into the file have we read, in bytes. */


    /* buffer stuff */
    char     *rwBuf;         /**< pointer to buffer if reading/writing from/to buffer. */
    uint32_t  rwBufSize;     /**< size of rwBuf buffer in bytes. */
    uint32_t  rwBytesOut;    /**< number of bytes written to rwBuf with evWrite. */
    uint32_t  rwBytesIn;     /**< number of bytes read from rwBuf so far
                            *   (i.e. # bytes in buffer already used).*/
    int       rwFirstWrite;  /**< 1 if this evWrite is the first for this rwBuf, else 0.
                            *   Needed for calculating accurate value for rwBytesOut. */

    /* socket stuff */
    int   sockFd;            /**< socket file descriptor if reading/writing from/to socket. */

    /* randomAcess stuff */
    int        randomAccess; /**< if true, use random access file/buffer reading. */
    size_t     mmapFileSize; /**< size of mapped file in bytes. */
    uint32_t  *mmapFile;     /**< pointer to memory mapped file. */
    uint32_t  **pTable;      /**< array of pointers to events in memory mapped file or buffer. */


    /* dictionary */
    int   hasAppendDictionary;
    int   wroteDictionary;      /**< dictionary already written out to a single (split fragment) file? */
    uint32_t dictLength;        /**< length of dictionary bank in bytes (including entire header). */
    uint32_t *dictBuf;          /**< buffer containing dictionary bank. */
    char *dictionary;           /**< xml format dictionary to either read or write. */

    /* first event */
    uint32_t firstEventLength;  /**< length of first event bank in bytes (including entire header). */
    uint32_t *firstEventBuf;    /**< buffer containing firstEvent bank. */

    /* Common block is first block in file/buf with dictionary and firstEvent */
    uint32_t commonBlkCount;    /**< Number of events written into common block.
                               *   This can be 2 at the most, dictionary + first event. */
    /* synchronization */
    int lockingOn;             /**< if = 1 (default), turn on the use of a mutex for thread safety. */

    /****************************/
    /*   Evio version 6 stuff   */
    /****************************/

    uint32_t fileIndexArrayLen; /**< file header's index array len in bytes. */
    uint32_t fileUserHeaderLen; /**< file header's user header len in bytes. */

    uint32_t curRecordIndexArrayLen; /**< current record header's index array len in bytes. */
    uint32_t curRecordUserHeaderLen; /**< current record header's user header len in bytes. */

    uint64_t trailerPosition; /**< trailer's position from start of file in bytes (0 if unknown). */
    uint64_t firstRecordPosition; /**< first record's position from start of file in bytes (0 if unknown). */

    uint32_t *eventLengths;   /**< For current record, an array containing the event lengths.
                                * blkEvCount tracks how many events and therefore entries in this array. */
    uint32_t eventLengthsLen; /**< Size of eventLengths array in words, convenience variable when reading. */

    //// WRITING ////

    uint32_t *dataBuf;      /**< For writing, pointer to buffer of events (data) to be written.
                             *   Due to evio version 6 having an array of event lengths
                             *   after the record header and before the record data, we must store
                             *   the data separately in order to facilitate writing the
                             *   record with fewest number of data copies.
                             *   Stores data for a single record.
                             *   For convenience, it'll be same size as "buf". */
    uint32_t  *dataNext;    /**< pointer to next word in dataBuf to be written. */
    uint32_t  dataLeft;     /**< # of valid 32 bit unwritten words in dataBuf. */
    uint32_t  bytesToDataBuf;  /**< # data bytes written to dataBuf. */

} EVFILE;


/** Structure for Sergei Boiarinov's use. */
typedef struct evioBlockHeaderV4_t {

    uint32_t length;       /**< total length of block in 32-bit words including this complete header. */
    uint32_t blockNumber;  /**< block id # starting at 1. */
    uint32_t headerLength; /**< length of this block header in 32-bit words (always 8). */
    uint32_t eventCount;   /**< # of events in this block (not counting dictionary). */
    uint32_t reserved1;    /**< reserved for future use. */
    uint32_t bitInfo;      /**< Contains version # in lowest 8 bits.
                                If dictionary is included as the first event, bit #9 is set.
                                If is a last block, bit #10 is set. */
    uint32_t reserved2;    /**< reserved for future use. */
    uint32_t magicNumber;  /**< written as 0xc0da0100 and used to check endianness. */

} evioBlockHeaderV4;

/** Offset in bytes from beginning of block header to block length. */
#define EVIO_BH_LEN_OFFSET 0
/** Offset in bytes from beginning of block header to block number. */
#define EVIO_BH_BLKNUM_OFFSET 32
/** Offset in bytes from beginning of block header to header length. */
#define EVIO_BH_HDRLEN_OFFSET 64
/** Offset in bytes from beginning of block header to event count. */
#define EVIO_BH_EVCOUNT_OFFSET 96
/** Offset in bytes from beginning of block header to bitInfo. */
#define EVIO_BH_BITINFO_OFFSET 160
/** Offset in bytes from beginning of block header to magic number. */
#define EVIO_BH_MAGNUM_OFFSET 224

/* prototypes */
void set_user_frag_select_func( int32_t (*f) (int32_t tag) );
int evioIsLocalHostBigEndian();
uint64_t evioToLongWord(uint32_t word1, uint32_t word2, int needToSwap);
void evioswap(uint32_t *buffer, int tolocal, uint32_t *dest);
void evioSwapFileHeaderV6(uint32_t *header);
void evioSwapRecordHeaderV6(uint32_t *header);
uint16_t *swap_int16_t(uint16_t *data, unsigned int length, uint16_t *dest);
uint32_t *swap_int32_t(uint32_t *data, unsigned int length, uint32_t *dest);
uint64_t *swap_int64_t(uint64_t *data, unsigned int length, uint64_t *dest);
/* do we need this for backwards compatibility???
int32_t swap_int32_t_value(int32_t val); */

int evOpen(char *filename, char *flags, int *handle);
int evOpenBuffer(char *buffer, uint32_t bufLen, char *flags, int *handle);
int evOpenSocket(int sockFd, char *flags, int *handle);

int evRead(int handle, uint32_t *buffer, uint32_t size);
int evReadAlloc(int handle, uint32_t **buffer, uint32_t *buflen);
int evReadNoCopy(int handle, const uint32_t **buffer, uint32_t *buflen);
int evReadRandom(int handle, const uint32_t **pEvent, uint32_t *buflen, uint32_t eventNumber);
int evGetRandomAccessTable(int handle, uint32_t *** const table, uint32_t *len);

int evWrite(int handle, const uint32_t *buffer);
int evIoctl(int handle, char *request, void *argp);
int evFlush(int handle);
int evClose(int handle);
int evGetBufferLength(int handle, uint32_t *length);
int evGetFileName(int handle, char *name, size_t maxLength);

int evIsLastBlock(uint32_t sixthWord);

int evGetDictionary(int handle, char **dictionary, uint32_t *len);
int evStringsToBuf(uint32_t *buffer, int bufLen, char **strings, int stringCount, int *dataLen);
int evBufToStrings(char *buffer, int bufLen, char ***pStrArray, int *strCount);

int evIsContainer(int type);
const char *evGetTypename(int type);
char *evPerror(int error);

void  evPrintBuffer(uint32_t *p, uint32_t len, int swap);

char *evStrReplace(char *orig, const char *replace, const char *with);
char *evStrReplaceEnvVar(const char *orig);
char *evStrFindSpecifiers(const char *orig, int *specifierCount);
char *evStrRemoveSpecifiers(const char *orig);
int   evGenerateBaseFileName(char *origName, char **baseName, int *count);
char *evGenerateFileName(EVFILE *a, int specifierCount, int runNumber,
                         int splitting, int splitNumber, char *runType, uint32_t streamId);

#ifdef __cplusplus
}

#endif

#endif
