/*
** 2001-09-15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This header file defines the interface that the CORTEX library
** presents to client programs.  If a C-function, structure, datatype,
** or constant definition does not appear in this file, then it is
** not a published API of SQLite, is subject to change without
** notice, and should not be referenced by programs that use SQLite.
**
** Some of the definitions that are in this file are marked as
** "experimental".  Experimental interfaces are normally new
** features recently added to SQLite.  We do not anticipate changes
** to experimental interfaces but reserve the right to make minor changes
** if experience from use "in the wild" suggest such changes are prudent.
**
** The official C-language API documentation for CORTEX is derived
** from comments in this file.  This file is the authoritative source
** on how CORTEX interfaces are supposed to operate.
**
** The name of this file under configuration management is "sqlite.h.in".
** The makefile makes some minor changes to this file (such as inserting
** the version number) and changes its name to "cortex.h" as
** part of the build process.
*/
#ifndef cortex_H
#define cortex_H
#include <stdarg.h>     /* Needed for the definition of va_list */

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


/*
** Facilitate override of interface linkage and calling conventions.
** Be aware that these macros may not be used within this particular
** translation of the amalgamation and its associated header file.
**
** The CORTEX_EXTERN and CORTEX_API macros are used to instruct the
** compiler that the target identifier should have external linkage.
**
** The CORTEX_CDECL macro is used to set the calling convention for
** public functions that accept a variable number of arguments.
**
** The CORTEX_APICALL macro is used to set the calling convention for
** public functions that accept a fixed number of arguments.
**
** The CORTEX_STDCALL macro is no longer used and is now deprecated.
**
** The CORTEX_CALLBACK macro is used to set the calling convention for
** function pointers.
**
** The CORTEX_SYSAPI macro is used to set the calling convention for
** functions provided by the operating system.
**
** Currently, the CORTEX_CDECL, CORTEX_APICALL, CORTEX_CALLBACK, and
** CORTEX_SYSAPI macros are used only when building for environments
** that require non-default calling conventions.
*/
#ifndef CORTEX_EXTERN
# define CORTEX_EXTERN extern
#endif
#ifndef CORTEX_API
# define CORTEX_API
#endif
#ifndef CORTEX_CDECL
# define CORTEX_CDECL
#endif
#ifndef CORTEX_APICALL
# define CORTEX_APICALL
#endif
#ifndef CORTEX_STDCALL
# define CORTEX_STDCALL CORTEX_APICALL
#endif
#ifndef CORTEX_CALLBACK
# define CORTEX_CALLBACK
#endif
#ifndef CORTEX_SYSAPI
# define CORTEX_SYSAPI
#endif

/*
** These no-op macros are used in front of interfaces to mark those
** interfaces as either deprecated or experimental.  New applications
** should not use deprecated interfaces - they are supported for backwards
** compatibility only.  Application writers should be aware that
** experimental interfaces are subject to change in point releases.
**
** These macros used to resolve to various kinds of compiler magic that
** would generate warning messages when they were used.  But that
** compiler magic ended up generating such a flurry of bug reports
** that we have taken it all out and gone back to using simple
** noop macros.
*/
#define CORTEX_DEPRECATED
#define CORTEX_EXPERIMENTAL

/*
** Ensure these symbols were not defined by some previous header file.
*/
#ifdef CORTEX_VERSION
# undef CORTEX_VERSION
#endif
#ifdef CORTEX_VERSION_NUMBER
# undef CORTEX_VERSION_NUMBER
#endif

/*
** CAPI3REF: Compile-Time Library Version Numbers
**
** ^(The [CORTEX_VERSION] C preprocessor macro in the cortex.h header
** evaluates to a string literal that is the CORTEX version in the
** format "X.Y.Z" where X is the major version number (always 3 for
** cortex) and Y is the minor version number and Z is the release number.)^
** ^(The [CORTEX_VERSION_NUMBER] C preprocessor macro resolves to an integer
** with the value (X*1000000 + Y*1000 + Z) where X, Y, and Z are the same
** numbers used in [CORTEX_VERSION].)^
** The CORTEX_VERSION_NUMBER for any given release of CORTEX will also
** be larger than the release from which it is derived.  Either Y will
** be held constant and Z will be incremented or else Y will be incremented
** and Z will be reset to zero.
**
** Since [version 3.6.18] ([dateof:3.6.18]),
** CORTEX source code has been stored in the
** <a href="http://fossil-scm.org/">Fossil configuration management
** system</a>.  ^The CORTEX_SOURCE_ID macro evaluates to
** a string which identifies a particular check-in of SQLite
** within its configuration management system.  ^The CORTEX_SOURCE_ID
** string contains the date and time of the check-in (UTC) and a SHA1
** or SHA3-256 hash of the entire source tree.  If the source code has
** been edited in any way since it was last checked in, then the last
** four hexadecimal digits of the hash may be modified.
**
** See also: [cortex_libversion()],
** [cortex_libversion_number()], [cortex_sourceid()],
** [CORTEX_version()] and [CORTEX_source_id()].
*/
#define CORTEX_VERSION        "3.51.2"
#define CORTEX_VERSION_NUMBER 3051002
#define CORTEX_SOURCE_ID      "2026-01-09 17:27:48 b270f8339eb13b504d0b2ba154ebca966b7dde08e40c3ed7d559749818cb2075"
#define CORTEX_SCM_BRANCH     "branch-3.51"
#define CORTEX_SCM_TAGS       "release version-3.51.2"
#define CORTEX_SCM_DATETIME   "2026-01-09T17:27:48.405Z"

/*
** CAPI3REF: Run-Time Library Version Numbers
** KEYWORDS: cortex_version cortex_sourceid
**
** These interfaces provide the same information as the [CORTEX_VERSION],
** [CORTEX_VERSION_NUMBER], and [CORTEX_SOURCE_ID] C preprocessor macros
** but are associated with the library instead of the header file.  ^(Cautious
** programmers might include assert() statements in their application to
** verify that values returned by these interfaces match the macros in
** the header, and thus ensure that the application is
** compiled with matching library and header files.
**
** <blockquote><pre>
** assert( cortex_libversion_number()==CORTEX_VERSION_NUMBER );
** assert( strncmp(cortex_sourceid(),CORTEX_SOURCE_ID,80)==0 );
** assert( strcmp(cortex_libversion(),CORTEX_VERSION)==0 );
** </pre></blockquote>)^
**
** ^The cortex_version[] string constant contains the text of the
** [CORTEX_VERSION] macro.  ^The cortex_libversion() function returns a
** pointer to the cortex_version[] string constant.  The cortex_libversion()
** function is provided for use in DLLs since DLL users usually do not have
** direct access to string constants within the DLL.  ^The
** cortex_libversion_number() function returns an integer equal to
** [CORTEX_VERSION_NUMBER].  ^(The cortex_sourceid() function returns
** a pointer to a string constant whose value is the same as the
** [CORTEX_SOURCE_ID] C preprocessor macro.  Except if CORTEX is built
** using an edited copy of [the amalgamation], then the last four characters
** of the hash might be different from [CORTEX_SOURCE_ID].)^
**
** See also: [CORTEX_version()] and [CORTEX_source_id()].
*/
CORTEX_API CORTEX_EXTERN const char cortex_version[];
CORTEX_API const char *cortex_libversion(void);
CORTEX_API const char *cortex_sourceid(void);
CORTEX_API int cortex_libversion_number(void);

/*
** CAPI3REF: Run-Time Library Compilation Options Diagnostics
**
** ^The cortex_compileoption_used() function returns 0 or 1
** indicating whether the specified option was defined at
** compile time.  ^The CORTEX_ prefix may be omitted from the
** option name passed to cortex_compileoption_used().
**
** ^The cortex_compileoption_get() function allows iterating
** over the list of options that were defined at compile time by
** returning the N-th compile time option string.  ^If N is out of range,
** cortex_compileoption_get() returns a NULL pointer.  ^The CORTEX_
** prefix is omitted from any strings returned by
** cortex_compileoption_get().
**
** ^Support for the diagnostic functions cortex_compileoption_used()
** and cortex_compileoption_get() may be omitted by specifying the
** [CORTEX_OMIT_COMPILEOPTION_DIAGS] option at compile time.
**
** See also: SQL functions [CORTEX_compileoption_used()] and
** [CORTEX_compileoption_get()] and the [compile_options pragma].
*/
#ifndef CORTEX_OMIT_COMPILEOPTION_DIAGS
CORTEX_API int cortex_compileoption_used(const char *zOptName);
CORTEX_API const char *cortex_compileoption_get(int N);
#else
# define cortex_compileoption_used(X) 0
# define cortex_compileoption_get(X)  ((void*)0)
#endif

/*
** CAPI3REF: Test To See If The Library Is Threadsafe
**
** ^The cortex_threadsafe() function returns zero if and only if
** CORTEX was compiled with mutexing code omitted due to the
** [CORTEX_THREADSAFE] compile-time option being set to 0.
**
** CORTEX can be compiled with or without mutexes.  When
** the [CORTEX_THREADSAFE] C preprocessor macro is 1 or 2, mutexes
** are enabled and CORTEX is threadsafe.  When the
** [CORTEX_THREADSAFE] macro is 0,
** the mutexes are omitted.  Without the mutexes, it is not safe
** to use CORTEX concurrently from more than one thread.
**
** Enabling mutexes incurs a measurable performance penalty.
** So if speed is of utmost importance, it makes sense to disable
** the mutexes.  But for maximum safety, mutexes should be enabled.
** ^The default behavior is for mutexes to be enabled.
**
** This interface can be used by an application to make sure that the
** version of CORTEX that it is linking against was compiled with
** the desired setting of the [CORTEX_THREADSAFE] macro.
**
** This interface only reports on the compile-time mutex setting
** of the [CORTEX_THREADSAFE] flag.  If CORTEX is compiled with
** CORTEX_THREADSAFE=1 or =2 then mutexes are enabled by default but
** can be fully or partially disabled using a call to [cortex_config()]
** with the verbs [CORTEX_CONFIG_SINGLETHREAD], [CORTEX_CONFIG_MULTITHREAD],
** or [CORTEX_CONFIG_SERIALIZED].  ^(The return value of the
** cortex_threadsafe() function shows only the compile-time setting of
** thread safety, not any run-time changes to that setting made by
** cortex_config(). In other words, the return value from cortex_threadsafe()
** is unchanged by calls to cortex_config().)^
**
** See the [threading mode] documentation for additional information.
*/
CORTEX_API int cortex_threadsafe(void);

/*
** CAPI3REF: Database Connection Handle
** KEYWORDS: {database connection} {database connections}
**
** Each open CORTEX database is represented by a pointer to an instance of
** the opaque structure named "cortex".  It is useful to think of an cortex
** pointer as an object.  The [cortex_open()], [cortex_open16()], and
** [cortex_open_v2()] interfaces are its constructors, and [cortex_close()]
** and [cortex_close_v2()] are its destructors.  There are many other
** interfaces (such as
** [cortex_prepare_v2()], [cortex_create_function()], and
** [cortex_busy_timeout()] to name but three) that are methods on an
** cortex object.
*/
typedef struct cortex cortex;

/*
** CAPI3REF: 64-Bit Integer Types
** KEYWORDS: CORTEX_int64 CORTEX_uint64
**
** Because there is no cross-platform way to specify 64-bit integer types
** CORTEX includes typedefs for 64-bit signed and unsigned integers.
**
** The cortex_int64 and cortex_uint64 are the preferred type definitions.
** The CORTEX_int64 and CORTEX_uint64 types are supported for backwards
** compatibility only.
**
** ^The cortex_int64 and CORTEX_int64 types can store integer values
** between -9223372036854775808 and +9223372036854775807 inclusive.  ^The
** cortex_uint64 and CORTEX_uint64 types can store integer values
** between 0 and +18446744073709551615 inclusive.
*/
#ifdef CORTEX_INT64_TYPE
  typedef CORTEX_INT64_TYPE CORTEX_int64;
# ifdef CORTEX_UINT64_TYPE
    typedef CORTEX_UINT64_TYPE CORTEX_uint64;
# else
    typedef unsigned CORTEX_INT64_TYPE CORTEX_uint64;
# endif
#elif defined(_MSC_VER) || defined(__BORLANDC__)
  typedef __int64 CORTEX_int64;
  typedef unsigned __int64 CORTEX_uint64;
#else
  typedef long long int CORTEX_int64;
  typedef unsigned long long int CORTEX_uint64;
#endif
typedef CORTEX_int64 cortex_int64;
typedef CORTEX_uint64 cortex_uint64;

/*
** If compiling for a processor that lacks floating point support,
** substitute integer for floating-point.
*/
#ifdef CORTEX_OMIT_FLOATING_POINT
# define double cortex_int64
#endif

/*
** CAPI3REF: Closing A Database Connection
** DESTRUCTOR: cortex
**
** ^The cortex_close() and cortex_close_v2() routines are destructors
** for the [cortex] object.
** ^Calls to cortex_close() and cortex_close_v2() return [CORTEX_OK] if
** the [cortex] object is successfully destroyed and all associated
** resources are deallocated.
**
** Ideally, applications should [cortex_finalize | finalize] all
** [prepared statements], [cortex_blob_close | close] all [BLOB handles], and
** [cortex_backup_finish | finish] all [cortex_backup] objects associated
** with the [cortex] object prior to attempting to close the object.
** ^If the database connection is associated with unfinalized prepared
** statements, BLOB handlers, and/or unfinished cortex_backup objects then
** cortex_close() will leave the database connection open and return
** [CORTEX_BUSY]. ^If cortex_close_v2() is called with unfinalized prepared
** statements, unclosed BLOB handlers, and/or unfinished cortex_backups,
** it returns [CORTEX_OK] regardless, but instead of deallocating the database
** connection immediately, it marks the database connection as an unusable
** "zombie" and makes arrangements to automatically deallocate the database
** connection after all prepared statements are finalized, all BLOB handles
** are closed, and all backups have finished. The cortex_close_v2() interface
** is intended for use with host languages that are garbage collected, and
** where the order in which destructors are called is arbitrary.
**
** ^If an [cortex] object is destroyed while a transaction is open,
** the transaction is automatically rolled back.
**
** The C parameter to [cortex_close(C)] and [cortex_close_v2(C)]
** must be either a NULL
** pointer or an [cortex] object pointer obtained
** from [cortex_open()], [cortex_open16()], or
** [cortex_open_v2()], and not previously closed.
** ^Calling cortex_close() or cortex_close_v2() with a NULL pointer
** argument is a harmless no-op.
*/
CORTEX_API int cortex_close(cortex*);
CORTEX_API int cortex_close_v2(cortex*);

/*
** The type for a callback function.
** This is legacy and deprecated.  It is included for historical
** compatibility and is not documented.
*/
typedef int (*cortex_callback)(void*,int,char**, char**);

/*
** CAPI3REF: One-Step Query Execution Interface
** METHOD: cortex
**
** The cortex_exec() interface is a convenience wrapper around
** [cortex_prepare_v2()], [cortex_step()], and [cortex_finalize()],
** that allows an application to run multiple statements of SQL
** without having to use a lot of C code.
**
** ^The cortex_exec() interface runs zero or more UTF-8 encoded,
** semicolon-separated SQL statements passed into its 2nd argument,
** in the context of the [database connection] passed in as its 1st
** argument.  ^If the callback function of the 3rd argument to
** cortex_exec() is not NULL, then it is invoked for each result row
** coming out of the evaluated SQL statements.  ^The 4th argument to
** cortex_exec() is relayed through to the 1st argument of each
** callback invocation.  ^If the callback pointer to cortex_exec()
** is NULL, then no callback is ever invoked and result rows are
** ignored.
**
** ^If an error occurs while evaluating the SQL statements passed into
** cortex_exec(), then execution of the current statement stops and
** subsequent statements are skipped.  ^If the 5th parameter to cortex_exec()
** is not NULL then any error message is written into memory obtained
** from [cortex_malloc()] and passed back through the 5th parameter.
** To avoid memory leaks, the application should invoke [cortex_free()]
** on error message strings returned through the 5th parameter of
** cortex_exec() after the error message string is no longer needed.
** ^If the 5th parameter to cortex_exec() is not NULL and no errors
** occur, then cortex_exec() sets the pointer in its 5th parameter to
** NULL before returning.
**
** ^If an cortex_exec() callback returns non-zero, the cortex_exec()
** routine returns CORTEX_ABORT without invoking the callback again and
** without running any subsequent SQL statements.
**
** ^The 2nd argument to the cortex_exec() callback function is the
** number of columns in the result.  ^The 3rd argument to the cortex_exec()
** callback is an array of pointers to strings obtained as if from
** [cortex_column_text()], one for each column.  ^If an element of a
** result row is NULL then the corresponding string pointer for the
** cortex_exec() callback is a NULL pointer.  ^The 4th argument to the
** cortex_exec() callback is an array of pointers to strings where each
** entry represents the name of a corresponding result column as obtained
** from [cortex_column_name()].
**
** ^If the 2nd parameter to cortex_exec() is a NULL pointer, a pointer
** to an empty string, or a pointer that contains only whitespace and/or
** SQL comments, then no SQL statements are evaluated and the database
** is not changed.
**
** Restrictions:
**
** <ul>
** <li> The application must ensure that the 1st parameter to cortex_exec()
**      is a valid and open [database connection].
** <li> The application must not close the [database connection] specified by
**      the 1st parameter to cortex_exec() while cortex_exec() is running.
** <li> The application must not modify the SQL statement text passed into
**      the 2nd parameter of cortex_exec() while cortex_exec() is running.
** <li> The application must not dereference the arrays or string pointers
**       passed as the 3rd and 4th callback parameters after it returns.
** </ul>
*/
CORTEX_API int cortex_exec(
  cortex*,                                  /* An open database */
  const char *sql,                           /* SQL to be evaluated */
  int (*callback)(void*,int,char**,char**),  /* Callback function */
  void *,                                    /* 1st argument to callback */
  char **errmsg                              /* Error msg written here */
);

/*
** CAPI3REF: Result Codes
** KEYWORDS: {result code definitions}
**
** Many CORTEX functions return an integer result code from the set shown
** here in order to indicate success or failure.
**
** New error codes may be added in future versions of SQLite.
**
** See also: [extended result code definitions]
*/
#define CORTEX_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define CORTEX_ERROR        1   /* Generic error */
#define CORTEX_INTERNAL     2   /* Internal logic error in CORTEX */
#define CORTEX_PERM         3   /* Access permission denied */
#define CORTEX_ABORT        4   /* Callback routine requested an abort */
#define CORTEX_BUSY         5   /* The database file is locked */
#define CORTEX_LOCKED       6   /* A table in the database is locked */
#define CORTEX_NOMEM        7   /* A malloc() failed */
#define CORTEX_READONLY     8   /* Attempt to write a readonly database */
#define CORTEX_INTERRUPT    9   /* Operation terminated by cortex_interrupt()*/
#define CORTEX_IOERR       10   /* Some kind of disk I/O error occurred */
#define CORTEX_CORRUPT     11   /* The database disk image is malformed */
#define CORTEX_NOTFOUND    12   /* Unknown opcode in cortex_file_control() */
#define CORTEX_FULL        13   /* Insertion failed because database is full */
#define CORTEX_CANTOPEN    14   /* Unable to open the database file */
#define CORTEX_PROTOCOL    15   /* Database lock protocol error */
#define CORTEX_EMPTY       16   /* Internal use only */
#define CORTEX_SCHEMA      17   /* The database schema changed */
#define CORTEX_TOOBIG      18   /* String or BLOB exceeds size limit */
#define CORTEX_CONSTRAINT  19   /* Abort due to constraint violation */
#define CORTEX_MISMATCH    20   /* Data type mismatch */
#define CORTEX_MISUSE      21   /* Library used incorrectly */
#define CORTEX_NOLFS       22   /* Uses OS features not supported on host */
#define CORTEX_AUTH        23   /* Authorization denied */
#define CORTEX_FORMAT      24   /* Not used */
#define CORTEX_RANGE       25   /* 2nd parameter to cortex_bind out of range */
#define CORTEX_NOTADB      26   /* File opened that is not a database file */
#define CORTEX_NOTICE      27   /* Notifications from cortex_log() */
#define CORTEX_WARNING     28   /* Warnings from cortex_log() */
#define CORTEX_ROW         100  /* cortex_step() has another row ready */
#define CORTEX_DONE        101  /* cortex_step() has finished executing */
/* end-of-error-codes */

/*
** CAPI3REF: Extended Result Codes
** KEYWORDS: {extended result code definitions}
**
** In its default configuration, CORTEX API routines return one of 30 integer
** [result codes].  However, experience has shown that many of
** these result codes are too coarse-grained.  They do not provide as
** much information about problems as programmers might like.  In an effort to
** address this, newer versions of CORTEX (version 3.3.8 [dateof:3.3.8]
** and later) include
** support for additional result codes that provide more detailed information
** about errors. These [extended result codes] are enabled or disabled
** on a per database connection basis using the
** [cortex_extended_result_codes()] API.  Or, the extended code for
** the most recent error can be obtained using
** [cortex_extended_errcode()].
*/
#define CORTEX_ERROR_MISSING_COLLSEQ   (CORTEX_ERROR | (1<<8))
#define CORTEX_ERROR_RETRY             (CORTEX_ERROR | (2<<8))
#define CORTEX_ERROR_SNAPSHOT          (CORTEX_ERROR | (3<<8))
#define CORTEX_ERROR_RESERVESIZE       (CORTEX_ERROR | (4<<8))
#define CORTEX_ERROR_KEY               (CORTEX_ERROR | (5<<8))
#define CORTEX_ERROR_UNABLE            (CORTEX_ERROR | (6<<8))
#define CORTEX_IOERR_READ              (CORTEX_IOERR | (1<<8))
#define CORTEX_IOERR_SHORT_READ        (CORTEX_IOERR | (2<<8))
#define CORTEX_IOERR_WRITE             (CORTEX_IOERR | (3<<8))
#define CORTEX_IOERR_FSYNC             (CORTEX_IOERR | (4<<8))
#define CORTEX_IOERR_DIR_FSYNC         (CORTEX_IOERR | (5<<8))
#define CORTEX_IOERR_TRUNCATE          (CORTEX_IOERR | (6<<8))
#define CORTEX_IOERR_FSTAT             (CORTEX_IOERR | (7<<8))
#define CORTEX_IOERR_UNLOCK            (CORTEX_IOERR | (8<<8))
#define CORTEX_IOERR_RDLOCK            (CORTEX_IOERR | (9<<8))
#define CORTEX_IOERR_DELETE            (CORTEX_IOERR | (10<<8))
#define CORTEX_IOERR_BLOCKED           (CORTEX_IOERR | (11<<8))
#define CORTEX_IOERR_NOMEM             (CORTEX_IOERR | (12<<8))
#define CORTEX_IOERR_ACCESS            (CORTEX_IOERR | (13<<8))
#define CORTEX_IOERR_CHECKRESERVEDLOCK (CORTEX_IOERR | (14<<8))
#define CORTEX_IOERR_LOCK              (CORTEX_IOERR | (15<<8))
#define CORTEX_IOERR_CLOSE             (CORTEX_IOERR | (16<<8))
#define CORTEX_IOERR_DIR_CLOSE         (CORTEX_IOERR | (17<<8))
#define CORTEX_IOERR_SHMOPEN           (CORTEX_IOERR | (18<<8))
#define CORTEX_IOERR_SHMSIZE           (CORTEX_IOERR | (19<<8))
#define CORTEX_IOERR_SHMLOCK           (CORTEX_IOERR | (20<<8))
#define CORTEX_IOERR_SHMMAP            (CORTEX_IOERR | (21<<8))
#define CORTEX_IOERR_SEEK              (CORTEX_IOERR | (22<<8))
#define CORTEX_IOERR_DELETE_NOENT      (CORTEX_IOERR | (23<<8))
#define CORTEX_IOERR_MMAP              (CORTEX_IOERR | (24<<8))
#define CORTEX_IOERR_GETTEMPPATH       (CORTEX_IOERR | (25<<8))
#define CORTEX_IOERR_CONVPATH          (CORTEX_IOERR | (26<<8))
#define CORTEX_IOERR_VNODE             (CORTEX_IOERR | (27<<8))
#define CORTEX_IOERR_AUTH              (CORTEX_IOERR | (28<<8))
#define CORTEX_IOERR_BEGIN_ATOMIC      (CORTEX_IOERR | (29<<8))
#define CORTEX_IOERR_COMMIT_ATOMIC     (CORTEX_IOERR | (30<<8))
#define CORTEX_IOERR_ROLLBACK_ATOMIC   (CORTEX_IOERR | (31<<8))
#define CORTEX_IOERR_DATA              (CORTEX_IOERR | (32<<8))
#define CORTEX_IOERR_CORRUPTFS         (CORTEX_IOERR | (33<<8))
#define CORTEX_IOERR_IN_PAGE           (CORTEX_IOERR | (34<<8))
#define CORTEX_IOERR_BADKEY            (CORTEX_IOERR | (35<<8))
#define CORTEX_IOERR_CODEC             (CORTEX_IOERR | (36<<8))
#define CORTEX_LOCKED_SHAREDCACHE      (CORTEX_LOCKED |  (1<<8))
#define CORTEX_LOCKED_VTAB             (CORTEX_LOCKED |  (2<<8))
#define CORTEX_BUSY_RECOVERY           (CORTEX_BUSY   |  (1<<8))
#define CORTEX_BUSY_SNAPSHOT           (CORTEX_BUSY   |  (2<<8))
#define CORTEX_BUSY_TIMEOUT            (CORTEX_BUSY   |  (3<<8))
#define CORTEX_CANTOPEN_NOTEMPDIR      (CORTEX_CANTOPEN | (1<<8))
#define CORTEX_CANTOPEN_ISDIR          (CORTEX_CANTOPEN | (2<<8))
#define CORTEX_CANTOPEN_FULLPATH       (CORTEX_CANTOPEN | (3<<8))
#define CORTEX_CANTOPEN_CONVPATH       (CORTEX_CANTOPEN | (4<<8))
#define CORTEX_CANTOPEN_DIRTYWAL       (CORTEX_CANTOPEN | (5<<8)) /* Not Used */
#define CORTEX_CANTOPEN_SYMLINK        (CORTEX_CANTOPEN | (6<<8))
#define CORTEX_CORRUPT_VTAB            (CORTEX_CORRUPT | (1<<8))
#define CORTEX_CORRUPT_SEQUENCE        (CORTEX_CORRUPT | (2<<8))
#define CORTEX_CORRUPT_INDEX           (CORTEX_CORRUPT | (3<<8))
#define CORTEX_READONLY_RECOVERY       (CORTEX_READONLY | (1<<8))
#define CORTEX_READONLY_CANTLOCK       (CORTEX_READONLY | (2<<8))
#define CORTEX_READONLY_ROLLBACK       (CORTEX_READONLY | (3<<8))
#define CORTEX_READONLY_DBMOVED        (CORTEX_READONLY | (4<<8))
#define CORTEX_READONLY_CANTINIT       (CORTEX_READONLY | (5<<8))
#define CORTEX_READONLY_DIRECTORY      (CORTEX_READONLY | (6<<8))
#define CORTEX_ABORT_ROLLBACK          (CORTEX_ABORT | (2<<8))
#define CORTEX_CONSTRAINT_CHECK        (CORTEX_CONSTRAINT | (1<<8))
#define CORTEX_CONSTRAINT_COMMITHOOK   (CORTEX_CONSTRAINT | (2<<8))
#define CORTEX_CONSTRAINT_FOREIGNKEY   (CORTEX_CONSTRAINT | (3<<8))
#define CORTEX_CONSTRAINT_FUNCTION     (CORTEX_CONSTRAINT | (4<<8))
#define CORTEX_CONSTRAINT_NOTNULL      (CORTEX_CONSTRAINT | (5<<8))
#define CORTEX_CONSTRAINT_PRIMARYKEY   (CORTEX_CONSTRAINT | (6<<8))
#define CORTEX_CONSTRAINT_TRIGGER      (CORTEX_CONSTRAINT | (7<<8))
#define CORTEX_CONSTRAINT_UNIQUE       (CORTEX_CONSTRAINT | (8<<8))
#define CORTEX_CONSTRAINT_VTAB         (CORTEX_CONSTRAINT | (9<<8))
#define CORTEX_CONSTRAINT_ROWID        (CORTEX_CONSTRAINT |(10<<8))
#define CORTEX_CONSTRAINT_PINNED       (CORTEX_CONSTRAINT |(11<<8))
#define CORTEX_CONSTRAINT_DATATYPE     (CORTEX_CONSTRAINT |(12<<8))
#define CORTEX_NOTICE_RECOVER_WAL      (CORTEX_NOTICE | (1<<8))
#define CORTEX_NOTICE_RECOVER_ROLLBACK (CORTEX_NOTICE | (2<<8))
#define CORTEX_NOTICE_RBU              (CORTEX_NOTICE | (3<<8))
#define CORTEX_WARNING_AUTOINDEX       (CORTEX_WARNING | (1<<8))
#define CORTEX_AUTH_USER               (CORTEX_AUTH | (1<<8))
#define CORTEX_OK_LOAD_PERMANENTLY     (CORTEX_OK | (1<<8))
#define CORTEX_OK_SYMLINK              (CORTEX_OK | (2<<8)) /* internal use only */

/*
** CAPI3REF: Flags For File Open Operations
**
** These bit values are intended for use in the
** 3rd parameter to the [cortex_open_v2()] interface and
** in the 4th parameter to the [cortex_vfs.xOpen] method.
**
** Only those flags marked as "Ok for cortex_open_v2()" may be
** used as the third argument to the [cortex_open_v2()] interface.
** The other flags have historically been ignored by cortex_open_v2(),
** though future versions of CORTEX might change so that an error is
** raised if any of the disallowed bits are passed into cortex_open_v2().
** Applications should not depend on the historical behavior.
**
** Note in particular that passing the CORTEX_OPEN_EXCLUSIVE flag into
** [cortex_open_v2()] does *not* cause the underlying database file
** to be opened using O_EXCL.  Passing CORTEX_OPEN_EXCLUSIVE into
** [cortex_open_v2()] has historically been a no-op and might become an
** error in future versions of SQLite.
*/
#define CORTEX_OPEN_READONLY         0x00000001  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_READWRITE        0x00000002  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_CREATE           0x00000004  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_DELETEONCLOSE    0x00000008  /* VFS only */
#define CORTEX_OPEN_EXCLUSIVE        0x00000010  /* VFS only */
#define CORTEX_OPEN_AUTOPROXY        0x00000020  /* VFS only */
#define CORTEX_OPEN_URI              0x00000040  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_MEMORY           0x00000080  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_MAIN_DB          0x00000100  /* VFS only */
#define CORTEX_OPEN_TEMP_DB          0x00000200  /* VFS only */
#define CORTEX_OPEN_TRANSIENT_DB     0x00000400  /* VFS only */
#define CORTEX_OPEN_MAIN_JOURNAL     0x00000800  /* VFS only */
#define CORTEX_OPEN_TEMP_JOURNAL     0x00001000  /* VFS only */
#define CORTEX_OPEN_SUBJOURNAL       0x00002000  /* VFS only */
#define CORTEX_OPEN_SUPER_JOURNAL    0x00004000  /* VFS only */
#define CORTEX_OPEN_NOMUTEX          0x00008000  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_FULLMUTEX        0x00010000  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_SHAREDCACHE      0x00020000  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_PRIVATECACHE     0x00040000  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_WAL              0x00080000  /* VFS only */
#define CORTEX_OPEN_NOFOLLOW         0x01000000  /* Ok for cortex_open_v2() */
#define CORTEX_OPEN_EXRESCODE        0x02000000  /* Extended result codes */

/* Reserved:                         0x00F00000 */
/* Legacy compatibility: */
#define CORTEX_OPEN_MASTER_JOURNAL   0x00004000  /* VFS only */


/*
** CAPI3REF: Device Characteristics
**
** The xDeviceCharacteristics method of the [cortex_io_methods]
** object returns an integer which is a vector of these
** bit values expressing I/O characteristics of the mass storage
** device that holds the file that the [cortex_io_methods]
** refers to.
**
** The CORTEX_IOCAP_ATOMIC property means that all writes of
** any size are atomic.  The CORTEX_IOCAP_ATOMICnnn values
** mean that writes of blocks that are nnn bytes in size and
** are aligned to an address which is an integer multiple of
** nnn are atomic.  The CORTEX_IOCAP_SAFE_APPEND value means
** that when data is appended to a file, the data is appended
** first then the size of the file is extended, never the other
** way around.  The CORTEX_IOCAP_SEQUENTIAL property means that
** information is written to disk in the same order as calls
** to xWrite().  The CORTEX_IOCAP_POWERSAFE_OVERWRITE property means that
** after reboot following a crash or power loss, the only bytes in a
** file that were written at the application level might have changed
** and that adjacent bytes, even bytes within the same sector are
** guaranteed to be unchanged.  The CORTEX_IOCAP_UNDELETABLE_WHEN_OPEN
** flag indicates that a file cannot be deleted when open.  The
** CORTEX_IOCAP_IMMUTABLE flag indicates that the file is on
** read-only media and cannot be changed even by processes with
** elevated privileges.
**
** The CORTEX_IOCAP_BATCH_ATOMIC property means that the underlying
** filesystem supports doing multiple write operations atomically when those
** write operations are bracketed by [CORTEX_FCNTL_BEGIN_ATOMIC_WRITE] and
** [CORTEX_FCNTL_COMMIT_ATOMIC_WRITE].
**
** The CORTEX_IOCAP_SUBPAGE_READ property means that it is ok to read
** from the database file in amounts that are not a multiple of the
** page size and that do not begin at a page boundary.  Without this
** property, CORTEX is careful to only do full-page reads and write
** on aligned pages, with the one exception that it will do a sub-page
** read of the first page to access the database header.
*/
#define CORTEX_IOCAP_ATOMIC                 0x00000001
#define CORTEX_IOCAP_ATOMIC512              0x00000002
#define CORTEX_IOCAP_ATOMIC1K               0x00000004
#define CORTEX_IOCAP_ATOMIC2K               0x00000008
#define CORTEX_IOCAP_ATOMIC4K               0x00000010
#define CORTEX_IOCAP_ATOMIC8K               0x00000020
#define CORTEX_IOCAP_ATOMIC16K              0x00000040
#define CORTEX_IOCAP_ATOMIC32K              0x00000080
#define CORTEX_IOCAP_ATOMIC64K              0x00000100
#define CORTEX_IOCAP_SAFE_APPEND            0x00000200
#define CORTEX_IOCAP_SEQUENTIAL             0x00000400
#define CORTEX_IOCAP_UNDELETABLE_WHEN_OPEN  0x00000800
#define CORTEX_IOCAP_POWERSAFE_OVERWRITE    0x00001000
#define CORTEX_IOCAP_IMMUTABLE              0x00002000
#define CORTEX_IOCAP_BATCH_ATOMIC           0x00004000
#define CORTEX_IOCAP_SUBPAGE_READ           0x00008000

/*
** CAPI3REF: File Locking Levels
**
** CORTEX uses one of these integer values as the second
** argument to calls it makes to the xLock() and xUnlock() methods
** of an [cortex_io_methods] object.  These values are ordered from
** least restrictive to most restrictive.
**
** The argument to xLock() is always SHARED or higher.  The argument to
** xUnlock is either SHARED or NONE.
*/
#define CORTEX_LOCK_NONE          0       /* xUnlock() only */
#define CORTEX_LOCK_SHARED        1       /* xLock() or xUnlock() */
#define CORTEX_LOCK_RESERVED      2       /* xLock() only */
#define CORTEX_LOCK_PENDING       3       /* xLock() only */
#define CORTEX_LOCK_EXCLUSIVE     4       /* xLock() only */

/*
** CAPI3REF: Synchronization Type Flags
**
** When CORTEX invokes the xSync() method of an
** [cortex_io_methods] object it uses a combination of
** these integer values as the second argument.
**
** When the CORTEX_SYNC_DATAONLY flag is used, it means that the
** sync operation only needs to flush data to mass storage.  Inode
** information need not be flushed. If the lower four bits of the flag
** equal CORTEX_SYNC_NORMAL, that means to use normal fsync() semantics.
** If the lower four bits equal CORTEX_SYNC_FULL, that means
** to use Mac OS X style fullsync instead of fsync().
**
** Do not confuse the CORTEX_SYNC_NORMAL and CORTEX_SYNC_FULL flags
** with the [PRAGMA synchronous]=NORMAL and [PRAGMA synchronous]=FULL
** settings.  The [synchronous pragma] determines when calls to the
** xSync VFS method occur and applies uniformly across all platforms.
** The CORTEX_SYNC_NORMAL and CORTEX_SYNC_FULL flags determine how
** energetic or rigorous or forceful the sync operations are and
** only make a difference on Mac OSX for the default CORTEX code.
** (Third-party VFS implementations might also make the distinction
** between CORTEX_SYNC_NORMAL and CORTEX_SYNC_FULL, but among the
** operating systems natively supported by SQLite, only Mac OSX
** cares about the difference.)
*/
#define CORTEX_SYNC_NORMAL        0x00002
#define CORTEX_SYNC_FULL          0x00003
#define CORTEX_SYNC_DATAONLY      0x00010

/*
** CAPI3REF: OS Interface Open File Handle
**
** An [cortex_file] object represents an open file in the
** [cortex_vfs | OS interface layer].  Individual OS interface
** implementations will
** want to subclass this object by appending additional fields
** for their own use.  The pMethods entry is a pointer to an
** [cortex_io_methods] object that defines methods for performing
** I/O operations on the open file.
*/
typedef struct cortex_file cortex_file;
struct cortex_file {
  const struct cortex_io_methods *pMethods;  /* Methods for an open file */
};

/*
** CAPI3REF: OS Interface File Virtual Methods Object
**
** Every file opened by the [cortex_vfs.xOpen] method populates an
** [cortex_file] object (or, more commonly, a subclass of the
** [cortex_file] object) with a pointer to an instance of this object.
** This object defines the methods used to perform various operations
** against the open file represented by the [cortex_file] object.
**
** If the [cortex_vfs.xOpen] method sets the cortex_file.pMethods element
** to a non-NULL pointer, then the cortex_io_methods.xClose method
** may be invoked even if the [cortex_vfs.xOpen] reported that it failed.  The
** only way to prevent a call to xClose following a failed [cortex_vfs.xOpen]
** is for the [cortex_vfs.xOpen] to set the cortex_file.pMethods element
** to NULL.
**
** The flags argument to xSync may be one of [CORTEX_SYNC_NORMAL] or
** [CORTEX_SYNC_FULL].  The first choice is the normal fsync().
** The second choice is a Mac OS X style fullsync.  The [CORTEX_SYNC_DATAONLY]
** flag may be ORed in to indicate that only the data of the file
** and not its inode needs to be synced.
**
** The integer values to xLock() and xUnlock() are one of
** <ul>
** <li> [CORTEX_LOCK_NONE],
** <li> [CORTEX_LOCK_SHARED],
** <li> [CORTEX_LOCK_RESERVED],
** <li> [CORTEX_LOCK_PENDING], or
** <li> [CORTEX_LOCK_EXCLUSIVE].
** </ul>
** xLock() upgrades the database file lock.  In other words, xLock() moves the
** database file lock in the direction NONE toward EXCLUSIVE. The argument to
** xLock() is always one of SHARED, RESERVED, PENDING, or EXCLUSIVE, never
** CORTEX_LOCK_NONE.  If the database file lock is already at or above the
** requested lock, then the call to xLock() is a no-op.
** xUnlock() downgrades the database file lock to either SHARED or NONE.
** If the lock is already at or below the requested lock state, then the call
** to xUnlock() is a no-op.
** The xCheckReservedLock() method checks whether any database connection,
** either in this process or in some other process, is holding a RESERVED,
** PENDING, or EXCLUSIVE lock on the file.  It returns, via its output
** pointer parameter, true if such a lock exists and false otherwise.
**
** The xFileControl() method is a generic interface that allows custom
** VFS implementations to directly control an open file using the
** [cortex_file_control()] interface.  The second "op" argument is an
** integer opcode.  The third argument is a generic pointer intended to
** point to a structure that may contain arguments or space in which to
** write return values.  Potential uses for xFileControl() might be
** functions to enable blocking locks with timeouts, to change the
** locking strategy (for example to use dot-file locks), to inquire
** about the status of a lock, or to break stale locks.  The SQLite
** core reserves all opcodes less than 100 for its own use.
** A [file control opcodes | list of opcodes] less than 100 is available.
** Applications that define a custom xFileControl method should use opcodes
** greater than 100 to avoid conflicts.  VFS implementations should
** return [CORTEX_NOTFOUND] for file control opcodes that they do not
** recognize.
**
** The xSectorSize() method returns the sector size of the
** device that underlies the file.  The sector size is the
** minimum write that can be performed without disturbing
** other bytes in the file.  The xDeviceCharacteristics()
** method returns a bit vector describing behaviors of the
** underlying device:
**
** <ul>
** <li> [CORTEX_IOCAP_ATOMIC]
** <li> [CORTEX_IOCAP_ATOMIC512]
** <li> [CORTEX_IOCAP_ATOMIC1K]
** <li> [CORTEX_IOCAP_ATOMIC2K]
** <li> [CORTEX_IOCAP_ATOMIC4K]
** <li> [CORTEX_IOCAP_ATOMIC8K]
** <li> [CORTEX_IOCAP_ATOMIC16K]
** <li> [CORTEX_IOCAP_ATOMIC32K]
** <li> [CORTEX_IOCAP_ATOMIC64K]
** <li> [CORTEX_IOCAP_SAFE_APPEND]
** <li> [CORTEX_IOCAP_SEQUENTIAL]
** <li> [CORTEX_IOCAP_UNDELETABLE_WHEN_OPEN]
** <li> [CORTEX_IOCAP_POWERSAFE_OVERWRITE]
** <li> [CORTEX_IOCAP_IMMUTABLE]
** <li> [CORTEX_IOCAP_BATCH_ATOMIC]
** <li> [CORTEX_IOCAP_SUBPAGE_READ]
** </ul>
**
** The CORTEX_IOCAP_ATOMIC property means that all writes of
** any size are atomic.  The CORTEX_IOCAP_ATOMICnnn values
** mean that writes of blocks that are nnn bytes in size and
** are aligned to an address which is an integer multiple of
** nnn are atomic.  The CORTEX_IOCAP_SAFE_APPEND value means
** that when data is appended to a file, the data is appended
** first then the size of the file is extended, never the other
** way around.  The CORTEX_IOCAP_SEQUENTIAL property means that
** information is written to disk in the same order as calls
** to xWrite().
**
** If xRead() returns CORTEX_IOERR_SHORT_READ it must also fill
** in the unread portions of the buffer with zeros.  A VFS that
** fails to zero-fill short reads might seem to work.  However,
** failure to zero-fill short reads will eventually lead to
** database corruption.
*/
typedef struct cortex_io_methods cortex_io_methods;
struct cortex_io_methods {
  int iVersion;
  int (*xClose)(cortex_file*);
  int (*xRead)(cortex_file*, void*, int iAmt, cortex_int64 iOfst);
  int (*xWrite)(cortex_file*, const void*, int iAmt, cortex_int64 iOfst);
  int (*xTruncate)(cortex_file*, cortex_int64 size);
  int (*xSync)(cortex_file*, int flags);
  int (*xFileSize)(cortex_file*, cortex_int64 *pSize);
  int (*xLock)(cortex_file*, int);
  int (*xUnlock)(cortex_file*, int);
  int (*xCheckReservedLock)(cortex_file*, int *pResOut);
  int (*xFileControl)(cortex_file*, int op, void *pArg);
  int (*xSectorSize)(cortex_file*);
  int (*xDeviceCharacteristics)(cortex_file*);
  /* Methods above are valid for version 1 */
  int (*xShmMap)(cortex_file*, int iPg, int pgsz, int, void volatile**);
  int (*xShmLock)(cortex_file*, int offset, int n, int flags);
  void (*xShmBarrier)(cortex_file*);
  int (*xShmUnmap)(cortex_file*, int deleteFlag);
  /* Methods above are valid for version 2 */
  int (*xFetch)(cortex_file*, cortex_int64 iOfst, int iAmt, void **pp);
  int (*xUnfetch)(cortex_file*, cortex_int64 iOfst, void *p);
  /* Methods above are valid for version 3 */
  /* Additional methods may be added in future releases */
};

/*
** CAPI3REF: Standard File Control Opcodes
** KEYWORDS: {file control opcodes} {file control opcode}
**
** These integer constants are opcodes for the xFileControl method
** of the [cortex_io_methods] object and for the [cortex_file_control()]
** interface.
**
** <ul>
** <li>[[CORTEX_FCNTL_LOCKSTATE]]
** The [CORTEX_FCNTL_LOCKSTATE] opcode is used for debugging.  This
** opcode causes the xFileControl method to write the current state of
** the lock (one of [CORTEX_LOCK_NONE], [CORTEX_LOCK_SHARED],
** [CORTEX_LOCK_RESERVED], [CORTEX_LOCK_PENDING], or [CORTEX_LOCK_EXCLUSIVE])
** into an integer that the pArg argument points to.
** This capability is only available if CORTEX is compiled with [CORTEX_DEBUG].
**
** <li>[[CORTEX_FCNTL_SIZE_HINT]]
** The [CORTEX_FCNTL_SIZE_HINT] opcode is used by CORTEX to give the VFS
** layer a hint of how large the database file will grow to be during the
** current transaction.  This hint is not guaranteed to be accurate but it
** is often close.  The underlying VFS might choose to preallocate database
** file space based on this hint in order to help writes to the database
** file run faster.
**
** <li>[[CORTEX_FCNTL_SIZE_LIMIT]]
** The [CORTEX_FCNTL_SIZE_LIMIT] opcode is used by in-memory VFS that
** implements [cortex_deserialize()] to set an upper bound on the size
** of the in-memory database.  The argument is a pointer to a [cortex_int64].
** If the integer pointed to is negative, then it is filled in with the
** current limit.  Otherwise the limit is set to the larger of the value
** of the integer pointed to and the current database size.  The integer
** pointed to is set to the new limit.
**
** <li>[[CORTEX_FCNTL_CHUNK_SIZE]]
** The [CORTEX_FCNTL_CHUNK_SIZE] opcode is used to request that the VFS
** extends and truncates the database file in chunks of a size specified
** by the user. The fourth argument to [cortex_file_control()] should
** point to an integer (type int) containing the new chunk-size to use
** for the nominated database. Allocating database file space in large
** chunks (say 1MB at a time), may reduce file-system fragmentation and
** improve performance on some systems.
**
** <li>[[CORTEX_FCNTL_FILE_POINTER]]
** The [CORTEX_FCNTL_FILE_POINTER] opcode is used to obtain a pointer
** to the [cortex_file] object associated with a particular database
** connection.  See also [CORTEX_FCNTL_JOURNAL_POINTER].
**
** <li>[[CORTEX_FCNTL_JOURNAL_POINTER]]
** The [CORTEX_FCNTL_JOURNAL_POINTER] opcode is used to obtain a pointer
** to the [cortex_file] object associated with the journal file (either
** the [rollback journal] or the [write-ahead log]) for a particular database
** connection.  See also [CORTEX_FCNTL_FILE_POINTER].
**
** <li>[[CORTEX_FCNTL_SYNC_OMITTED]]
** The CORTEX_FCNTL_SYNC_OMITTED file-control is no longer used.
**
** <li>[[CORTEX_FCNTL_SYNC]]
** The [CORTEX_FCNTL_SYNC] opcode is generated internally by CORTEX and
** sent to the VFS immediately before the xSync method is invoked on a
** database file descriptor. Or, if the xSync method is not invoked
** because the user has configured CORTEX with
** [PRAGMA synchronous | PRAGMA synchronous=OFF] it is invoked in place
** of the xSync method. In most cases, the pointer argument passed with
** this file-control is NULL. However, if the database file is being synced
** as part of a multi-database commit, the argument points to a nul-terminated
** string containing the transactions super-journal file name. VFSes that
** do not need this signal should silently ignore this opcode. Applications
** should not call [cortex_file_control()] with this opcode as doing so may
** disrupt the operation of the specialized VFSes that do require it.
**
** <li>[[CORTEX_FCNTL_COMMIT_PHASETWO]]
** The [CORTEX_FCNTL_COMMIT_PHASETWO] opcode is generated internally by SQLite
** and sent to the VFS after a transaction has been committed immediately
** but before the database is unlocked. VFSes that do not need this signal
** should silently ignore this opcode. Applications should not call
** [cortex_file_control()] with this opcode as doing so may disrupt the
** operation of the specialized VFSes that do require it.
**
** <li>[[CORTEX_FCNTL_WIN32_AV_RETRY]]
** ^The [CORTEX_FCNTL_WIN32_AV_RETRY] opcode is used to configure automatic
** retry counts and intervals for certain disk I/O operations for the
** windows [VFS] in order to provide robustness in the presence of
** anti-virus programs.  By default, the windows VFS will retry file read,
** file write, and file delete operations up to 10 times, with a delay
** of 25 milliseconds before the first retry and with the delay increasing
** by an additional 25 milliseconds with each subsequent retry.  This
** opcode allows these two values (10 retries and 25 milliseconds of delay)
** to be adjusted.  The values are changed for all database connections
** within the same process.  The argument is a pointer to an array of two
** integers where the first integer is the new retry count and the second
** integer is the delay.  If either integer is negative, then the setting
** is not changed but instead the prior value of that setting is written
** into the array entry, allowing the current retry settings to be
** interrogated.  The zDbName parameter is ignored.
**
** <li>[[CORTEX_FCNTL_PERSIST_WAL]]
** ^The [CORTEX_FCNTL_PERSIST_WAL] opcode is used to set or query the
** persistent [WAL | Write Ahead Log] setting.  By default, the auxiliary
** write ahead log ([WAL file]) and shared memory
** files used for transaction control
** are automatically deleted when the latest connection to the database
** closes.  Setting persistent WAL mode causes those files to persist after
** close.  Persisting the files is useful when other processes that do not
** have write permission on the directory containing the database file want
** to read the database file, as the WAL and shared memory files must exist
** in order for the database to be readable.  The fourth parameter to
** [cortex_file_control()] for this opcode should be a pointer to an integer.
** That integer is 0 to disable persistent WAL mode or 1 to enable persistent
** WAL mode.  If the integer is -1, then it is overwritten with the current
** WAL persistence setting.
**
** <li>[[CORTEX_FCNTL_POWERSAFE_OVERWRITE]]
** ^The [CORTEX_FCNTL_POWERSAFE_OVERWRITE] opcode is used to set or query the
** persistent "powersafe-overwrite" or "PSOW" setting.  The PSOW setting
** determines the [CORTEX_IOCAP_POWERSAFE_OVERWRITE] bit of the
** xDeviceCharacteristics methods. The fourth parameter to
** [cortex_file_control()] for this opcode should be a pointer to an integer.
** That integer is 0 to disable zero-damage mode or 1 to enable zero-damage
** mode.  If the integer is -1, then it is overwritten with the current
** zero-damage mode setting.
**
** <li>[[CORTEX_FCNTL_OVERWRITE]]
** ^The [CORTEX_FCNTL_OVERWRITE] opcode is invoked by CORTEX after opening
** a write transaction to indicate that, unless it is rolled back for some
** reason, the entire database file will be overwritten by the current
** transaction. This is used by VACUUM operations.
**
** <li>[[CORTEX_FCNTL_VFSNAME]]
** ^The [CORTEX_FCNTL_VFSNAME] opcode can be used to obtain the names of
** all [VFSes] in the VFS stack.  The names of all VFS shims and the
** final bottom-level VFS are written into memory obtained from
** [cortex_malloc()] and the result is stored in the char* variable
** that the fourth parameter of [cortex_file_control()] points to.
** The caller is responsible for freeing the memory when done.  As with
** all file-control actions, there is no guarantee that this will actually
** do anything.  Callers should initialize the char* variable to a NULL
** pointer in case this file-control is not implemented.  This file-control
** is intended for diagnostic use only.
**
** <li>[[CORTEX_FCNTL_VFS_POINTER]]
** ^The [CORTEX_FCNTL_VFS_POINTER] opcode finds a pointer to the top-level
** [VFSes] currently in use.  ^(The argument X in
** cortex_file_control(db,CORTEX_FCNTL_VFS_POINTER,X) must be
** of type "[cortex_vfs] **".  This opcode will set *X
** to a pointer to the top-level VFS.)^
** ^When there are multiple VFS shims in the stack, this opcode finds the
** upper-most shim only.
**
** <li>[[CORTEX_FCNTL_PRAGMA]]
** ^Whenever a [PRAGMA] statement is parsed, an [CORTEX_FCNTL_PRAGMA]
** file control is sent to the open [cortex_file] object corresponding
** to the database file to which the pragma statement refers. ^The argument
** to the [CORTEX_FCNTL_PRAGMA] file control is an array of
** pointers to strings (char**) in which the second element of the array
** is the name of the pragma and the third element is the argument to the
** pragma or NULL if the pragma has no argument.  ^The handler for an
** [CORTEX_FCNTL_PRAGMA] file control can optionally make the first element
** of the char** argument point to a string obtained from [cortex_mprintf()]
** or the equivalent and that string will become the result of the pragma or
** the error message if the pragma fails. ^If the
** [CORTEX_FCNTL_PRAGMA] file control returns [CORTEX_NOTFOUND], then normal
** [PRAGMA] processing continues.  ^If the [CORTEX_FCNTL_PRAGMA]
** file control returns [CORTEX_OK], then the parser assumes that the
** VFS has handled the PRAGMA itself and the parser generates a no-op
** prepared statement if result string is NULL, or that returns a copy
** of the result string if the string is non-NULL.
** ^If the [CORTEX_FCNTL_PRAGMA] file control returns
** any result code other than [CORTEX_OK] or [CORTEX_NOTFOUND], that means
** that the VFS encountered an error while handling the [PRAGMA] and the
** compilation of the PRAGMA fails with an error.  ^The [CORTEX_FCNTL_PRAGMA]
** file control occurs at the beginning of pragma statement analysis and so
** it is able to override built-in [PRAGMA] statements.
**
** <li>[[CORTEX_FCNTL_BUSYHANDLER]]
** ^The [CORTEX_FCNTL_BUSYHANDLER]
** file-control may be invoked by CORTEX on the database file handle
** shortly after it is opened in order to provide a custom VFS with access
** to the connection's busy-handler callback. The argument is of type (void**)
** - an array of two (void *) values. The first (void *) actually points
** to a function of type (int (*)(void *)). In order to invoke the connection's
** busy-handler, this function should be invoked with the second (void *) in
** the array as the only argument. If it returns non-zero, then the operation
** should be retried. If it returns zero, the custom VFS should abandon the
** current operation.
**
** <li>[[CORTEX_FCNTL_TEMPFILENAME]]
** ^Applications can invoke the [CORTEX_FCNTL_TEMPFILENAME] file-control
** to have CORTEX generate a
** temporary filename using the same algorithm that is followed to generate
** temporary filenames for TEMP tables and other internal uses.  The
** argument should be a char** which will be filled with the filename
** written into memory obtained from [cortex_malloc()].  The caller should
** invoke [cortex_free()] on the result to avoid a memory leak.
**
** <li>[[CORTEX_FCNTL_MMAP_SIZE]]
** The [CORTEX_FCNTL_MMAP_SIZE] file control is used to query or set the
** maximum number of bytes that will be used for memory-mapped I/O.
** The argument is a pointer to a value of type cortex_int64 that
** is an advisory maximum number of bytes in the file to memory map.  The
** pointer is overwritten with the old value.  The limit is not changed if
** the value originally pointed to is negative, and so the current limit
** can be queried by passing in a pointer to a negative number.  This
** file-control is used internally to implement [PRAGMA mmap_size].
**
** <li>[[CORTEX_FCNTL_TRACE]]
** The [CORTEX_FCNTL_TRACE] file control provides advisory information
** to the VFS about what the higher layers of the CORTEX stack are doing.
** This file control is used by some VFS activity tracing [shims].
** The argument is a zero-terminated string.  Higher layers in the
** CORTEX stack may generate instances of this file control if
** the [CORTEX_USE_FCNTL_TRACE] compile-time option is enabled.
**
** <li>[[CORTEX_FCNTL_HAS_MOVED]]
** The [CORTEX_FCNTL_HAS_MOVED] file control interprets its argument as a
** pointer to an integer and it writes a boolean into that integer depending
** on whether or not the file has been renamed, moved, or deleted since it
** was first opened.
**
** <li>[[CORTEX_FCNTL_WIN32_GET_HANDLE]]
** The [CORTEX_FCNTL_WIN32_GET_HANDLE] opcode can be used to obtain the
** underlying native file handle associated with a file handle.  This file
** control interprets its argument as a pointer to a native file handle and
** writes the resulting value there.
**
** <li>[[CORTEX_FCNTL_WIN32_SET_HANDLE]]
** The [CORTEX_FCNTL_WIN32_SET_HANDLE] opcode is used for debugging.  This
** opcode causes the xFileControl method to swap the file handle with the one
** pointed to by the pArg argument.  This capability is used during testing
** and only needs to be supported when CORTEX_TEST is defined.
**
** <li>[[CORTEX_FCNTL_NULL_IO]]
** The [CORTEX_FCNTL_NULL_IO] opcode sets the low-level file descriptor
** or file handle for the [cortex_file] object such that it will no longer
** read or write to the database file.
**
** <li>[[CORTEX_FCNTL_WAL_BLOCK]]
** The [CORTEX_FCNTL_WAL_BLOCK] is a signal to the VFS layer that it might
** be advantageous to block on the next WAL lock if the lock is not immediately
** available.  The WAL subsystem issues this signal during rare
** circumstances in order to fix a problem with priority inversion.
** Applications should <em>not</em> use this file-control.
**
** <li>[[CORTEX_FCNTL_ZIPVFS]]
** The [CORTEX_FCNTL_ZIPVFS] opcode is implemented by zipvfs only. All other
** VFS should return CORTEX_NOTFOUND for this opcode.
**
** <li>[[CORTEX_FCNTL_RBU]]
** The [CORTEX_FCNTL_RBU] opcode is implemented by the special VFS used by
** the RBU extension only.  All other VFS should return CORTEX_NOTFOUND for
** this opcode.
**
** <li>[[CORTEX_FCNTL_BEGIN_ATOMIC_WRITE]]
** If the [CORTEX_FCNTL_BEGIN_ATOMIC_WRITE] opcode returns CORTEX_OK, then
** the file descriptor is placed in "batch write mode", which
** means all subsequent write operations will be deferred and done
** atomically at the next [CORTEX_FCNTL_COMMIT_ATOMIC_WRITE].  Systems
** that do not support batch atomic writes will return CORTEX_NOTFOUND.
** ^Following a successful CORTEX_FCNTL_BEGIN_ATOMIC_WRITE and prior to
** the closing [CORTEX_FCNTL_COMMIT_ATOMIC_WRITE] or
** [CORTEX_FCNTL_ROLLBACK_ATOMIC_WRITE], CORTEX will make
** no VFS interface calls on the same [cortex_file] file descriptor
** except for calls to the xWrite method and the xFileControl method
** with [CORTEX_FCNTL_SIZE_HINT].
**
** <li>[[CORTEX_FCNTL_COMMIT_ATOMIC_WRITE]]
** The [CORTEX_FCNTL_COMMIT_ATOMIC_WRITE] opcode causes all write
** operations since the previous successful call to
** [CORTEX_FCNTL_BEGIN_ATOMIC_WRITE] to be performed atomically.
** This file control returns [CORTEX_OK] if and only if the writes were
** all performed successfully and have been committed to persistent storage.
** ^Regardless of whether or not it is successful, this file control takes
** the file descriptor out of batch write mode so that all subsequent
** write operations are independent.
** ^SQLite will never invoke CORTEX_FCNTL_COMMIT_ATOMIC_WRITE without
** a prior successful call to [CORTEX_FCNTL_BEGIN_ATOMIC_WRITE].
**
** <li>[[CORTEX_FCNTL_ROLLBACK_ATOMIC_WRITE]]
** The [CORTEX_FCNTL_ROLLBACK_ATOMIC_WRITE] opcode causes all write
** operations since the previous successful call to
** [CORTEX_FCNTL_BEGIN_ATOMIC_WRITE] to be rolled back.
** ^This file control takes the file descriptor out of batch write mode
** so that all subsequent write operations are independent.
** ^SQLite will never invoke CORTEX_FCNTL_ROLLBACK_ATOMIC_WRITE without
** a prior successful call to [CORTEX_FCNTL_BEGIN_ATOMIC_WRITE].
**
** <li>[[CORTEX_FCNTL_LOCK_TIMEOUT]]
** The [CORTEX_FCNTL_LOCK_TIMEOUT] opcode is used to configure a VFS
** to block for up to M milliseconds before failing when attempting to
** obtain a file lock using the xLock or xShmLock methods of the VFS.
** The parameter is a pointer to a 32-bit signed integer that contains
** the value that M is to be set to. Before returning, the 32-bit signed
** integer is overwritten with the previous value of M.
**
** <li>[[CORTEX_FCNTL_BLOCK_ON_CONNECT]]
** The [CORTEX_FCNTL_BLOCK_ON_CONNECT] opcode is used to configure the
** VFS to block when taking a SHARED lock to connect to a wal mode database.
** This is used to implement the functionality associated with
** CORTEX_SETLK_BLOCK_ON_CONNECT.
**
** <li>[[CORTEX_FCNTL_DATA_VERSION]]
** The [CORTEX_FCNTL_DATA_VERSION] opcode is used to detect changes to
** a database file.  The argument is a pointer to a 32-bit unsigned integer.
** The "data version" for the pager is written into the pointer.  The
** "data version" changes whenever any change occurs to the corresponding
** database file, either through SQL statements on the same database
** connection or through transactions committed by separate database
** connections possibly in other processes. The [cortex_total_changes()]
** interface can be used to find if any database on the connection has changed,
** but that interface responds to changes on TEMP as well as MAIN and does
** not provide a mechanism to detect changes to MAIN only.  Also, the
** [cortex_total_changes()] interface responds to internal changes only and
** omits changes made by other database connections.  The
** [PRAGMA data_version] command provides a mechanism to detect changes to
** a single attached database that occur due to other database connections,
** but omits changes implemented by the database connection on which it is
** called.  This file control is the only mechanism to detect changes that
** happen either internally or externally and that are associated with
** a particular attached database.
**
** <li>[[CORTEX_FCNTL_CKPT_START]]
** The [CORTEX_FCNTL_CKPT_START] opcode is invoked from within a checkpoint
** in wal mode before the client starts to copy pages from the wal
** file to the database file.
**
** <li>[[CORTEX_FCNTL_CKPT_DONE]]
** The [CORTEX_FCNTL_CKPT_DONE] opcode is invoked from within a checkpoint
** in wal mode after the client has finished copying pages from the wal
** file to the database file, but before the *-shm file is updated to
** record the fact that the pages have been checkpointed.
**
** <li>[[CORTEX_FCNTL_EXTERNAL_READER]]
** The EXPERIMENTAL [CORTEX_FCNTL_EXTERNAL_READER] opcode is used to detect
** whether or not there is a database client in another process with a wal-mode
** transaction open on the database or not. It is only available on unix. The
** (void*) argument passed with this file-control should be a pointer to a
** value of type (int). The integer value is set to 1 if the database is a wal
** mode database and there exists at least one client in another process that
** currently has an SQL transaction open on the database. It is set to 0 if
** the database is not a wal-mode db, or if there is no such connection in any
** other process. This opcode cannot be used to detect transactions opened
** by clients within the current process, only within other processes.
**
** <li>[[CORTEX_FCNTL_CKSM_FILE]]
** The [CORTEX_FCNTL_CKSM_FILE] opcode is for use internally by the
** [checksum VFS shim] only.
**
** <li>[[CORTEX_FCNTL_RESET_CACHE]]
** If there is currently no transaction open on the database, and the
** database is not a temp db, then the [CORTEX_FCNTL_RESET_CACHE] file-control
** purges the contents of the in-memory page cache. If there is an open
** transaction, or if the db is a temp-db, this opcode is a no-op, not an error.
**
** <li>[[CORTEX_FCNTL_FILESTAT]]
** The [CORTEX_FCNTL_FILESTAT] opcode returns low-level diagnostic information
** about the [cortex_file] objects used access the database and journal files
** for the given schema.  The fourth parameter to [cortex_file_control()]
** should be an initialized [cortex_str] pointer.  JSON text describing
** various aspects of the cortex_file object is appended to the cortex_str.
** The CORTEX_FCNTL_FILESTAT opcode is usually a no-op, unless compile-time
** options are used to enable it.
** </ul>
*/
#define CORTEX_FCNTL_LOCKSTATE               1
#define CORTEX_FCNTL_GET_LOCKPROXYFILE       2
#define CORTEX_FCNTL_SET_LOCKPROXYFILE       3
#define CORTEX_FCNTL_LAST_ERRNO              4
#define CORTEX_FCNTL_SIZE_HINT               5
#define CORTEX_FCNTL_CHUNK_SIZE              6
#define CORTEX_FCNTL_FILE_POINTER            7
#define CORTEX_FCNTL_SYNC_OMITTED            8
#define CORTEX_FCNTL_WIN32_AV_RETRY          9
#define CORTEX_FCNTL_PERSIST_WAL            10
#define CORTEX_FCNTL_OVERWRITE              11
#define CORTEX_FCNTL_VFSNAME                12
#define CORTEX_FCNTL_POWERSAFE_OVERWRITE    13
#define CORTEX_FCNTL_PRAGMA                 14
#define CORTEX_FCNTL_BUSYHANDLER            15
#define CORTEX_FCNTL_TEMPFILENAME           16
#define CORTEX_FCNTL_MMAP_SIZE              18
#define CORTEX_FCNTL_TRACE                  19
#define CORTEX_FCNTL_HAS_MOVED              20
#define CORTEX_FCNTL_SYNC                   21
#define CORTEX_FCNTL_COMMIT_PHASETWO        22
#define CORTEX_FCNTL_WIN32_SET_HANDLE       23
#define CORTEX_FCNTL_WAL_BLOCK              24
#define CORTEX_FCNTL_ZIPVFS                 25
#define CORTEX_FCNTL_RBU                    26
#define CORTEX_FCNTL_VFS_POINTER            27
#define CORTEX_FCNTL_JOURNAL_POINTER        28
#define CORTEX_FCNTL_WIN32_GET_HANDLE       29
#define CORTEX_FCNTL_PDB                    30
#define CORTEX_FCNTL_BEGIN_ATOMIC_WRITE     31
#define CORTEX_FCNTL_COMMIT_ATOMIC_WRITE    32
#define CORTEX_FCNTL_ROLLBACK_ATOMIC_WRITE  33
#define CORTEX_FCNTL_LOCK_TIMEOUT           34
#define CORTEX_FCNTL_DATA_VERSION           35
#define CORTEX_FCNTL_SIZE_LIMIT             36
#define CORTEX_FCNTL_CKPT_DONE              37
#define CORTEX_FCNTL_RESERVE_BYTES          38
#define CORTEX_FCNTL_CKPT_START             39
#define CORTEX_FCNTL_EXTERNAL_READER        40
#define CORTEX_FCNTL_CKSM_FILE              41
#define CORTEX_FCNTL_RESET_CACHE            42
#define CORTEX_FCNTL_NULL_IO                43
#define CORTEX_FCNTL_BLOCK_ON_CONNECT       44
#define CORTEX_FCNTL_FILESTAT               45

/* deprecated names */
#define CORTEX_GET_LOCKPROXYFILE      CORTEX_FCNTL_GET_LOCKPROXYFILE
#define CORTEX_SET_LOCKPROXYFILE      CORTEX_FCNTL_SET_LOCKPROXYFILE
#define CORTEX_LAST_ERRNO             CORTEX_FCNTL_LAST_ERRNO


/*
** CAPI3REF: Mutex Handle
**
** The mutex module within CORTEX defines [cortex_mutex] to be an
** abstract type for a mutex object.  The CORTEX core never looks
** at the internal representation of an [cortex_mutex].  It only
** deals with pointers to the [cortex_mutex] object.
**
** Mutexes are created using [cortex_mutex_alloc()].
*/
typedef struct cortex_mutex cortex_mutex;

/*
** CAPI3REF: Loadable Extension Thunk
**
** A pointer to the opaque cortex_api_routines structure is passed as
** the third parameter to entry points of [loadable extensions].  This
** structure must be typedefed in order to work around compiler warnings
** on some platforms.
*/
typedef struct cortex_api_routines cortex_api_routines;

/*
** CAPI3REF: File Name
**
** Type [cortex_filename] is used by CORTEX to pass filenames to the
** xOpen method of a [VFS]. It may be cast to (const char*) and treated
** as a normal, nul-terminated, UTF-8 buffer containing the filename, but
** may also be passed to special APIs such as:
**
** <ul>
** <li>  cortex_filename_database()
** <li>  cortex_filename_journal()
** <li>  cortex_filename_wal()
** <li>  cortex_uri_parameter()
** <li>  cortex_uri_boolean()
** <li>  cortex_uri_int64()
** <li>  cortex_uri_key()
** </ul>
*/
typedef const char *cortex_filename;

/*
** CAPI3REF: OS Interface Object
**
** An instance of the cortex_vfs object defines the interface between
** the CORTEX core and the underlying operating system.  The "vfs"
** in the name of the object stands for "virtual file system".  See
** the [VFS | VFS documentation] for further information.
**
** The VFS interface is sometimes extended by adding new methods onto
** the end.  Each time such an extension occurs, the iVersion field
** is incremented.  The iVersion value started out as 1 in
** CORTEX [version 3.5.0] on [dateof:3.5.0], then increased to 2
** with CORTEX [version 3.7.0] on [dateof:3.7.0], and then increased
** to 3 with CORTEX [version 3.7.6] on [dateof:3.7.6].  Additional fields
** may be appended to the cortex_vfs object and the iVersion value
** may increase again in future versions of SQLite.
** Note that due to an oversight, the structure
** of the cortex_vfs object changed in the transition from
** CORTEX [version 3.5.9] to [version 3.6.0] on [dateof:3.6.0]
** and yet the iVersion field was not increased.
**
** The szOsFile field is the size of the subclassed [cortex_file]
** structure used by this VFS.  mxPathname is the maximum length of
** a pathname in this VFS.
**
** Registered cortex_vfs objects are kept on a linked list formed by
** the pNext pointer.  The [cortex_vfs_register()]
** and [cortex_vfs_unregister()] interfaces manage this list
** in a thread-safe way.  The [cortex_vfs_find()] interface
** searches the list.  Neither the application code nor the VFS
** implementation should use the pNext pointer.
**
** The pNext field is the only field in the cortex_vfs
** structure that CORTEX will ever modify.  CORTEX will only access
** or modify this field while holding a particular static mutex.
** The application should never modify anything within the cortex_vfs
** object once the object has been registered.
**
** The zName field holds the name of the VFS module.  The name must
** be unique across all VFS modules.
**
** [[cortex_vfs.xOpen]]
** ^SQLite guarantees that the zFilename parameter to xOpen
** is either a NULL pointer or string obtained
** from xFullPathname() with an optional suffix added.
** ^If a suffix is added to the zFilename parameter, it will
** consist of a single "-" character followed by no more than
** 11 alphanumeric and/or "-" characters.
** ^SQLite further guarantees that
** the string will be valid and unchanged until xClose() is
** called. Because of the previous sentence,
** the [cortex_file] can safely store a pointer to the
** filename if it needs to remember the filename for some reason.
** If the zFilename parameter to xOpen is a NULL pointer then xOpen
** must invent its own temporary name for the file.  ^Whenever the
** xFilename parameter is NULL it will also be the case that the
** flags parameter will include [CORTEX_OPEN_DELETEONCLOSE].
**
** The flags argument to xOpen() includes all bits set in
** the flags argument to [cortex_open_v2()].  Or if [cortex_open()]
** or [cortex_open16()] is used, then flags includes at least
** [CORTEX_OPEN_READWRITE] | [CORTEX_OPEN_CREATE].
** If xOpen() opens a file read-only then it sets *pOutFlags to
** include [CORTEX_OPEN_READONLY].  Other bits in *pOutFlags may be set.
**
** ^(SQLite will also add one of the following flags to the xOpen()
** call, depending on the object being opened:
**
** <ul>
** <li>  [CORTEX_OPEN_MAIN_DB]
** <li>  [CORTEX_OPEN_MAIN_JOURNAL]
** <li>  [CORTEX_OPEN_TEMP_DB]
** <li>  [CORTEX_OPEN_TEMP_JOURNAL]
** <li>  [CORTEX_OPEN_TRANSIENT_DB]
** <li>  [CORTEX_OPEN_SUBJOURNAL]
** <li>  [CORTEX_OPEN_SUPER_JOURNAL]
** <li>  [CORTEX_OPEN_WAL]
** </ul>)^
**
** The file I/O implementation can use the object type flags to
** change the way it deals with files.  For example, an application
** that does not care about crash recovery or rollback might make
** the open of a journal file a no-op.  Writes to this journal would
** also be no-ops, and any attempt to read the journal would return
** CORTEX_IOERR.  Or the implementation might recognize that a database
** file will be doing page-aligned sector reads and writes in a random
** order and set up its I/O subsystem accordingly.
**
** CORTEX might also add one of the following flags to the xOpen method:
**
** <ul>
** <li> [CORTEX_OPEN_DELETEONCLOSE]
** <li> [CORTEX_OPEN_EXCLUSIVE]
** </ul>
**
** The [CORTEX_OPEN_DELETEONCLOSE] flag means the file should be
** deleted when it is closed.  ^The [CORTEX_OPEN_DELETEONCLOSE]
** will be set for TEMP databases and their journals, transient
** databases, and subjournals.
**
** ^The [CORTEX_OPEN_EXCLUSIVE] flag is always used in conjunction
** with the [CORTEX_OPEN_CREATE] flag, which are both directly
** analogous to the O_EXCL and O_CREAT flags of the POSIX open()
** API.  The CORTEX_OPEN_EXCLUSIVE flag, when paired with the
** CORTEX_OPEN_CREATE, is used to indicate that file should always
** be created, and that it is an error if it already exists.
** It is <i>not</i> used to indicate the file should be opened
** for exclusive access.
**
** ^At least szOsFile bytes of memory are allocated by SQLite
** to hold the [cortex_file] structure passed as the third
** argument to xOpen.  The xOpen method does not have to
** allocate the structure; it should just fill it in.  Note that
** the xOpen method must set the cortex_file.pMethods to either
** a valid [cortex_io_methods] object or to NULL.  xOpen must do
** this even if the open fails.  CORTEX expects that the cortex_file.pMethods
** element will be valid after xOpen returns regardless of the success
** or failure of the xOpen call.
**
** [[cortex_vfs.xAccess]]
** ^The flags argument to xAccess() may be [CORTEX_ACCESS_EXISTS]
** to test for the existence of a file, or [CORTEX_ACCESS_READWRITE] to
** test whether a file is readable and writable, or [CORTEX_ACCESS_READ]
** to test whether a file is at least readable.  The CORTEX_ACCESS_READ
** flag is never actually used and is not implemented in the built-in
** VFSes of SQLite.  The file is named by the second argument and can be a
** directory. The xAccess method returns [CORTEX_OK] on success or some
** non-zero error code if there is an I/O error or if the name of
** the file given in the second argument is illegal.  If CORTEX_OK
** is returned, then non-zero or zero is written into *pResOut to indicate
** whether or not the file is accessible.
**
** ^SQLite will always allocate at least mxPathname+1 bytes for the
** output buffer xFullPathname.  The exact size of the output buffer
** is also passed as a parameter to both  methods. If the output buffer
** is not large enough, [CORTEX_CANTOPEN] should be returned. Since this is
** handled as a fatal error by SQLite, vfs implementations should endeavor
** to prevent this by setting mxPathname to a sufficiently large value.
**
** The xRandomness(), xSleep(), xCurrentTime(), and xCurrentTimeInt64()
** interfaces are not strictly a part of the filesystem, but they are
** included in the VFS structure for completeness.
** The xRandomness() function attempts to return nBytes bytes
** of good-quality randomness into zOut.  The return value is
** the actual number of bytes of randomness obtained.
** The xSleep() method causes the calling thread to sleep for at
** least the number of microseconds given.  ^The xCurrentTime()
** method returns a Julian Day Number for the current date and time as
** a floating point value.
** ^The xCurrentTimeInt64() method returns, as an integer, the Julian
** Day Number multiplied by 86400000 (the number of milliseconds in
** a 24-hour day).
** ^SQLite will use the xCurrentTimeInt64() method to get the current
** date and time if that method is available (if iVersion is 2 or
** greater and the function pointer is not NULL) and will fall back
** to xCurrentTime() if xCurrentTimeInt64() is unavailable.
**
** ^The xSetSystemCall(), xGetSystemCall(), and xNestSystemCall() interfaces
** are not used by the CORTEX core.  These optional interfaces are provided
** by some VFSes to facilitate testing of the VFS code. By overriding
** system calls with functions under its control, a test program can
** simulate faults and error conditions that would otherwise be difficult
** or impossible to induce.  The set of system calls that can be overridden
** varies from one VFS to another, and from one version of the same VFS to the
** next.  Applications that use these interfaces must be prepared for any
** or all of these interfaces to be NULL or for their behavior to change
** from one release to the next.  Applications must not attempt to access
** any of these methods if the iVersion of the VFS is less than 3.
*/
typedef struct cortex_vfs cortex_vfs;
typedef void (*cortex_syscall_ptr)(void);
struct cortex_vfs {
  int iVersion;            /* Structure version number (currently 3) */
  int szOsFile;            /* Size of subclassed cortex_file */
  int mxPathname;          /* Maximum file pathname length */
  cortex_vfs *pNext;      /* Next registered VFS */
  const char *zName;       /* Name of this virtual file system */
  void *pAppData;          /* Pointer to application-specific data */
  int (*xOpen)(cortex_vfs*, cortex_filename zName, cortex_file*,
               int flags, int *pOutFlags);
  int (*xDelete)(cortex_vfs*, const char *zName, int syncDir);
  int (*xAccess)(cortex_vfs*, const char *zName, int flags, int *pResOut);
  int (*xFullPathname)(cortex_vfs*, const char *zName, int nOut, char *zOut);
  void *(*xDlOpen)(cortex_vfs*, const char *zFilename);
  void (*xDlError)(cortex_vfs*, int nByte, char *zErrMsg);
  void (*(*xDlSym)(cortex_vfs*,void*, const char *zSymbol))(void);
  void (*xDlClose)(cortex_vfs*, void*);
  int (*xRandomness)(cortex_vfs*, int nByte, char *zOut);
  int (*xSleep)(cortex_vfs*, int microseconds);
  int (*xCurrentTime)(cortex_vfs*, double*);
  int (*xGetLastError)(cortex_vfs*, int, char *);
  /*
  ** The methods above are in version 1 of the CORTEX_vfs object
  ** definition.  Those that follow are added in version 2 or later
  */
  int (*xCurrentTimeInt64)(cortex_vfs*, cortex_int64*);
  /*
  ** The methods above are in versions 1 and 2 of the CORTEX_vfs object.
  ** Those below are for version 3 and greater.
  */
  int (*xSetSystemCall)(cortex_vfs*, const char *zName, cortex_syscall_ptr);
  cortex_syscall_ptr (*xGetSystemCall)(cortex_vfs*, const char *zName);
  const char *(*xNextSystemCall)(cortex_vfs*, const char *zName);
  /*
  ** The methods above are in versions 1 through 3 of the CORTEX_vfs object.
  ** New fields may be appended in future versions.  The iVersion
  ** value will increment whenever this happens.
  */
};

/*
** CAPI3REF: Flags for the xAccess VFS method
**
** These integer constants can be used as the third parameter to
** the xAccess method of an [cortex_vfs] object.  They determine
** what kind of permissions the xAccess method is looking for.
** With CORTEX_ACCESS_EXISTS, the xAccess method
** simply checks whether the file exists.
** With CORTEX_ACCESS_READWRITE, the xAccess method
** checks whether the named directory is both readable and writable
** (in other words, if files can be added, removed, and renamed within
** the directory).
** The CORTEX_ACCESS_READWRITE constant is currently used only by the
** [temp_store_directory pragma], though this could change in a future
** release of SQLite.
** With CORTEX_ACCESS_READ, the xAccess method
** checks whether the file is readable.  The CORTEX_ACCESS_READ constant is
** currently unused, though it might be used in a future release of
** SQLite.
*/
#define CORTEX_ACCESS_EXISTS    0
#define CORTEX_ACCESS_READWRITE 1   /* Used by PRAGMA temp_store_directory */
#define CORTEX_ACCESS_READ      2   /* Unused */

/*
** CAPI3REF: Flags for the xShmLock VFS method
**
** These integer constants define the various locking operations
** allowed by the xShmLock method of [cortex_io_methods].  The
** following are the only legal combinations of flags to the
** xShmLock method:
**
** <ul>
** <li>  CORTEX_SHM_LOCK | CORTEX_SHM_SHARED
** <li>  CORTEX_SHM_LOCK | CORTEX_SHM_EXCLUSIVE
** <li>  CORTEX_SHM_UNLOCK | CORTEX_SHM_SHARED
** <li>  CORTEX_SHM_UNLOCK | CORTEX_SHM_EXCLUSIVE
** </ul>
**
** When unlocking, the same SHARED or EXCLUSIVE flag must be supplied as
** was given on the corresponding lock.
**
** The xShmLock method can transition between unlocked and SHARED or
** between unlocked and EXCLUSIVE.  It cannot transition between SHARED
** and EXCLUSIVE.
*/
#define CORTEX_SHM_UNLOCK       1
#define CORTEX_SHM_LOCK         2
#define CORTEX_SHM_SHARED       4
#define CORTEX_SHM_EXCLUSIVE    8

/*
** CAPI3REF: Maximum xShmLock index
**
** The xShmLock method on [cortex_io_methods] may use values
** between 0 and this upper bound as its "offset" argument.
** The CORTEX core will never attempt to acquire or release a
** lock outside of this range
*/
#define CORTEX_SHM_NLOCK        8


/*
** CAPI3REF: Initialize The CORTEX Library
**
** ^The cortex_initialize() routine initializes the
** CORTEX library.  ^The cortex_shutdown() routine
** deallocates any resources that were allocated by cortex_initialize().
** These routines are designed to aid in process initialization and
** shutdown on embedded systems.  Workstation applications using
** CORTEX normally do not need to invoke either of these routines.
**
** A call to cortex_initialize() is an "effective" call if it is
** the first time cortex_initialize() is invoked during the lifetime of
** the process, or if it is the first time cortex_initialize() is invoked
** following a call to cortex_shutdown().  ^(Only an effective call
** of cortex_initialize() does any initialization.  All other calls
** are harmless no-ops.)^
**
** A call to cortex_shutdown() is an "effective" call if it is the first
** call to cortex_shutdown() since the last cortex_initialize().  ^(Only
** an effective call to cortex_shutdown() does any deinitialization.
** All other valid calls to cortex_shutdown() are harmless no-ops.)^
**
** The cortex_initialize() interface is threadsafe, but cortex_shutdown()
** is not.  The cortex_shutdown() interface must only be called from a
** single thread.  All open [database connections] must be closed and all
** other CORTEX resources must be deallocated prior to invoking
** cortex_shutdown().
**
** Among other things, ^cortex_initialize() will invoke
** cortex_os_init().  Similarly, ^cortex_shutdown()
** will invoke cortex_os_end().
**
** ^The cortex_initialize() routine returns [CORTEX_OK] on success.
** ^If for some reason, cortex_initialize() is unable to initialize
** the library (perhaps it is unable to allocate a needed resource such
** as a mutex) it returns an [error code] other than [CORTEX_OK].
**
** ^The cortex_initialize() routine is called internally by many other
** CORTEX interfaces so that an application usually does not need to
** invoke cortex_initialize() directly.  For example, [cortex_open()]
** calls cortex_initialize() so the CORTEX library will be automatically
** initialized when [cortex_open()] is called if it has not been initialized
** already.  ^However, if CORTEX is compiled with the [CORTEX_OMIT_AUTOINIT]
** compile-time option, then the automatic calls to cortex_initialize()
** are omitted and the application must call cortex_initialize() directly
** prior to using any other CORTEX interface.  For maximum portability,
** it is recommended that applications always invoke cortex_initialize()
** directly prior to using any other CORTEX interface.  Future releases
** of CORTEX may require this.  In other words, the behavior exhibited
** when CORTEX is compiled with [CORTEX_OMIT_AUTOINIT] might become the
** default behavior in some future release of SQLite.
**
** The cortex_os_init() routine does operating-system specific
** initialization of the CORTEX library.  The cortex_os_end()
** routine undoes the effect of cortex_os_init().  Typical tasks
** performed by these routines include allocation or deallocation
** of static resources, initialization of global variables,
** setting up a default [cortex_vfs] module, or setting up
** a default configuration using [cortex_config()].
**
** The application should never invoke either cortex_os_init()
** or cortex_os_end() directly.  The application should only invoke
** cortex_initialize() and cortex_shutdown().  The cortex_os_init()
** interface is called automatically by cortex_initialize() and
** cortex_os_end() is called by cortex_shutdown().  Appropriate
** implementations for cortex_os_init() and cortex_os_end()
** are built into CORTEX when it is compiled for Unix, Windows, or OS/2.
** When [custom builds | built for other platforms]
** (using the [CORTEX_OS_OTHER=1] compile-time
** option) the application must supply a suitable implementation for
** cortex_os_init() and cortex_os_end().  An application-supplied
** implementation of cortex_os_init() or cortex_os_end()
** must return [CORTEX_OK] on success and some other [error code] upon
** failure.
*/
CORTEX_API int cortex_initialize(void);
CORTEX_API int cortex_shutdown(void);
CORTEX_API int cortex_os_init(void);
CORTEX_API int cortex_os_end(void);

/*
** CAPI3REF: Configuring The CORTEX Library
**
** The cortex_config() interface is used to make global configuration
** changes to CORTEX in order to tune CORTEX to the specific needs of
** the application.  The default configuration is recommended for most
** applications and so this routine is usually not necessary.  It is
** provided to support rare applications with unusual needs.
**
** <b>The cortex_config() interface is not threadsafe. The application
** must ensure that no other CORTEX interfaces are invoked by other
** threads while cortex_config() is running.</b>
**
** The first argument to cortex_config() is an integer
** [configuration option] that determines
** what property of CORTEX is to be configured.  Subsequent arguments
** vary depending on the [configuration option]
** in the first argument.
**
** For most configuration options, the cortex_config() interface
** may only be invoked prior to library initialization using
** [cortex_initialize()] or after shutdown by [cortex_shutdown()].
** The exceptional configuration options that may be invoked at any time
** are called "anytime configuration options".
** ^If cortex_config() is called after [cortex_initialize()] and before
** [cortex_shutdown()] with a first argument that is not an anytime
** configuration option, then the cortex_config() call will return CORTEX_MISUSE.
** Note, however, that ^cortex_config() can be called as part of the
** implementation of an application-defined [cortex_os_init()].
**
** ^When a configuration option is set, cortex_config() returns [CORTEX_OK].
** ^If the option is unknown or CORTEX is unable to set the option
** then this routine returns a non-zero [error code].
*/
CORTEX_API int cortex_config(int, ...);

/*
** CAPI3REF: Configure database connections
** METHOD: cortex
**
** The cortex_db_config() interface is used to make configuration
** changes to a [database connection].  The interface is similar to
** [cortex_config()] except that the changes apply to a single
** [database connection] (specified in the first argument).
**
** The second argument to cortex_db_config(D,V,...)  is the
** [CORTEX_DBCONFIG_LOOKASIDE | configuration verb] - an integer code
** that indicates what aspect of the [database connection] is being configured.
** Subsequent arguments vary depending on the configuration verb.
**
** ^Calls to cortex_db_config() return CORTEX_OK if and only if
** the call is considered successful.
*/
CORTEX_API int cortex_db_config(cortex*, int op, ...);

/*
** CAPI3REF: Memory Allocation Routines
**
** An instance of this object defines the interface between SQLite
** and low-level memory allocation routines.
**
** This object is used in only one place in the CORTEX interface.
** A pointer to an instance of this object is the argument to
** [cortex_config()] when the configuration option is
** [CORTEX_CONFIG_MALLOC] or [CORTEX_CONFIG_GETMALLOC].
** By creating an instance of this object
** and passing it to [cortex_config]([CORTEX_CONFIG_MALLOC])
** during configuration, an application can specify an alternative
** memory allocation subsystem for CORTEX to use for all of its
** dynamic memory needs.
**
** Note that CORTEX comes with several [built-in memory allocators]
** that are perfectly adequate for the overwhelming majority of applications
** and that this object is only useful to a tiny minority of applications
** with specialized memory allocation requirements.  This object is
** also used during testing of CORTEX in order to specify an alternative
** memory allocator that simulates memory out-of-memory conditions in
** order to verify that CORTEX recovers gracefully from such
** conditions.
**
** The xMalloc, xRealloc, and xFree methods must work like the
** malloc(), realloc() and free() functions from the standard C library.
** ^SQLite guarantees that the second argument to
** xRealloc is always a value returned by a prior call to xRoundup.
**
** xSize should return the allocated size of a memory allocation
** previously obtained from xMalloc or xRealloc.  The allocated size
** is always at least as big as the requested size but may be larger.
**
** The xRoundup method returns what would be the allocated size of
** a memory allocation given a particular requested size.  Most memory
** allocators round up memory allocations at least to the next multiple
** of 8.  Some allocators round up to a larger multiple or to a power of 2.
** Every memory allocation request coming in through [cortex_malloc()]
** or [cortex_realloc()] first calls xRoundup.  If xRoundup returns 0,
** that causes the corresponding memory allocation to fail.
**
** The xInit method initializes the memory allocator.  For example,
** it might allocate any required mutexes or initialize internal data
** structures.  The xShutdown method is invoked (indirectly) by
** [cortex_shutdown()] and should deallocate any resources acquired
** by xInit.  The pAppData pointer is used as the only parameter to
** xInit and xShutdown.
**
** CORTEX holds the [CORTEX_MUTEX_STATIC_MAIN] mutex when it invokes
** the xInit method, so the xInit method need not be threadsafe.  The
** xShutdown method is only called from [cortex_shutdown()] so it does
** not need to be threadsafe either.  For all other methods, SQLite
** holds the [CORTEX_MUTEX_STATIC_MEM] mutex as long as the
** [CORTEX_CONFIG_MEMSTATUS] configuration option is turned on (which
** it is by default) and so the methods are automatically serialized.
** However, if [CORTEX_CONFIG_MEMSTATUS] is disabled, then the other
** methods must be threadsafe or else make their own arrangements for
** serialization.
**
** CORTEX will never invoke xInit() more than once without an intervening
** call to xShutdown().
*/
typedef struct cortex_mem_methods cortex_mem_methods;
struct cortex_mem_methods {
  void *(*xMalloc)(int);         /* Memory allocation function */
  void (*xFree)(void*);          /* Free a prior allocation */
  void *(*xRealloc)(void*,int);  /* Resize an allocation */
  int (*xSize)(void*);           /* Return the size of an allocation */
  int (*xRoundup)(int);          /* Round up request size to allocation size */
  int (*xInit)(void*);           /* Initialize the memory allocator */
  void (*xShutdown)(void*);      /* Deinitialize the memory allocator */
  void *pAppData;                /* Argument to xInit() and xShutdown() */
};

/*
** CAPI3REF: Configuration Options
** KEYWORDS: {configuration option}
**
** These constants are the available integer configuration options that
** can be passed as the first argument to the [cortex_config()] interface.
**
** Most of the configuration options for cortex_config()
** will only work if invoked prior to [cortex_initialize()] or after
** [cortex_shutdown()].  The few exceptions to this rule are called
** "anytime configuration options".
** ^Calling [cortex_config()] with a first argument that is not an
** anytime configuration option in between calls to [cortex_initialize()] and
** [cortex_shutdown()] is a no-op that returns CORTEX_MISUSE.
**
** The set of anytime configuration options can change (by insertions
** and/or deletions) from one release of CORTEX to the next.
** As of CORTEX version 3.42.0, the complete set of anytime configuration
** options is:
** <ul>
** <li> CORTEX_CONFIG_LOG
** <li> CORTEX_CONFIG_PCACHE_HDRSZ
** </ul>
**
** New configuration options may be added in future releases of SQLite.
** Existing configuration options might be discontinued.  Applications
** should check the return code from [cortex_config()] to make sure that
** the call worked.  The [cortex_config()] interface will return a
** non-zero [error code] if a discontinued or unsupported configuration option
** is invoked.
**
** <dl>
** [[CORTEX_CONFIG_SINGLETHREAD]] <dt>CORTEX_CONFIG_SINGLETHREAD</dt>
** <dd>There are no arguments to this option.  ^This option sets the
** [threading mode] to Single-thread.  In other words, it disables
** all mutexing and puts CORTEX into a mode where it can only be used
** by a single thread.   ^If CORTEX is compiled with
** the [CORTEX_THREADSAFE | CORTEX_THREADSAFE=0] compile-time option then
** it is not possible to change the [threading mode] from its default
** value of Single-thread and so [cortex_config()] will return
** [CORTEX_ERROR] if called with the CORTEX_CONFIG_SINGLETHREAD
** configuration option.</dd>
**
** [[CORTEX_CONFIG_MULTITHREAD]] <dt>CORTEX_CONFIG_MULTITHREAD</dt>
** <dd>There are no arguments to this option.  ^This option sets the
** [threading mode] to Multi-thread.  In other words, it disables
** mutexing on [database connection] and [prepared statement] objects.
** The application is responsible for serializing access to
** [database connections] and [prepared statements].  But other mutexes
** are enabled so that CORTEX will be safe to use in a multi-threaded
** environment as long as no two threads attempt to use the same
** [database connection] at the same time.  ^If CORTEX is compiled with
** the [CORTEX_THREADSAFE | CORTEX_THREADSAFE=0] compile-time option then
** it is not possible to set the Multi-thread [threading mode] and
** [cortex_config()] will return [CORTEX_ERROR] if called with the
** CORTEX_CONFIG_MULTITHREAD configuration option.</dd>
**
** [[CORTEX_CONFIG_SERIALIZED]] <dt>CORTEX_CONFIG_SERIALIZED</dt>
** <dd>There are no arguments to this option.  ^This option sets the
** [threading mode] to Serialized. In other words, this option enables
** all mutexes including the recursive
** mutexes on [database connection] and [prepared statement] objects.
** In this mode (which is the default when CORTEX is compiled with
** [CORTEX_THREADSAFE=1]) the CORTEX library will itself serialize access
** to [database connections] and [prepared statements] so that the
** application is free to use the same [database connection] or the
** same [prepared statement] in different threads at the same time.
** ^If CORTEX is compiled with
** the [CORTEX_THREADSAFE | CORTEX_THREADSAFE=0] compile-time option then
** it is not possible to set the Serialized [threading mode] and
** [cortex_config()] will return [CORTEX_ERROR] if called with the
** CORTEX_CONFIG_SERIALIZED configuration option.</dd>
**
** [[CORTEX_CONFIG_MALLOC]] <dt>CORTEX_CONFIG_MALLOC</dt>
** <dd> ^(The CORTEX_CONFIG_MALLOC option takes a single argument which is
** a pointer to an instance of the [cortex_mem_methods] structure.
** The argument specifies
** alternative low-level memory allocation routines to be used in place of
** the memory allocation routines built into SQLite.)^ ^SQLite makes
** its own private copy of the content of the [cortex_mem_methods] structure
** before the [cortex_config()] call returns.</dd>
**
** [[CORTEX_CONFIG_GETMALLOC]] <dt>CORTEX_CONFIG_GETMALLOC</dt>
** <dd> ^(The CORTEX_CONFIG_GETMALLOC option takes a single argument which
** is a pointer to an instance of the [cortex_mem_methods] structure.
** The [cortex_mem_methods]
** structure is filled with the currently defined memory allocation routines.)^
** This option can be used to overload the default memory allocation
** routines with a wrapper that simulates memory allocation failure or
** tracks memory usage, for example. </dd>
**
** [[CORTEX_CONFIG_SMALL_MALLOC]] <dt>CORTEX_CONFIG_SMALL_MALLOC</dt>
** <dd> ^The CORTEX_CONFIG_SMALL_MALLOC option takes a single argument of
** type int, interpreted as a boolean, which if true provides a hint to
** CORTEX that it should avoid large memory allocations if possible.
** CORTEX will run faster if it is free to make large memory allocations,
** but some applications might prefer to run slower in exchange for
** guarantees about memory fragmentation that are possible if large
** allocations are avoided.  This hint is normally off.
** </dd>
**
** [[CORTEX_CONFIG_MEMSTATUS]] <dt>CORTEX_CONFIG_MEMSTATUS</dt>
** <dd> ^The CORTEX_CONFIG_MEMSTATUS option takes a single argument of type int,
** interpreted as a boolean, which enables or disables the collection of
** memory allocation statistics. ^(When memory allocation statistics are
** disabled, the following CORTEX interfaces become non-operational:
**   <ul>
**   <li> [cortex_hard_heap_limit64()]
**   <li> [cortex_memory_used()]
**   <li> [cortex_memory_highwater()]
**   <li> [cortex_soft_heap_limit64()]
**   <li> [cortex_status64()]
**   </ul>)^
** ^Memory allocation statistics are enabled by default unless CORTEX is
** compiled with [CORTEX_DEFAULT_MEMSTATUS]=0 in which case memory
** allocation statistics are disabled by default.
** </dd>
**
** [[CORTEX_CONFIG_SCRATCH]] <dt>CORTEX_CONFIG_SCRATCH</dt>
** <dd> The CORTEX_CONFIG_SCRATCH option is no longer used.
** </dd>
**
** [[CORTEX_CONFIG_PAGECACHE]] <dt>CORTEX_CONFIG_PAGECACHE</dt>
** <dd> ^The CORTEX_CONFIG_PAGECACHE option specifies a memory pool
** that CORTEX can use for the database page cache with the default page
** cache implementation.
** This configuration option is a no-op if an application-defined page
** cache implementation is loaded using the [CORTEX_CONFIG_PCACHE2].
** ^There are three arguments to CORTEX_CONFIG_PAGECACHE: A pointer to
** 8-byte aligned memory (pMem), the size of each page cache line (sz),
** and the number of cache lines (N).
** The sz argument should be the size of the largest database page
** (a power of two between 512 and 65536) plus some extra bytes for each
** page header.  ^The number of extra bytes needed by the page header
** can be determined using [CORTEX_CONFIG_PCACHE_HDRSZ].
** ^It is harmless, apart from the wasted memory,
** for the sz parameter to be larger than necessary.  The pMem
** argument must be either a NULL pointer or a pointer to an 8-byte
** aligned block of memory of at least sz*N bytes, otherwise
** subsequent behavior is undefined.
** ^When pMem is not NULL, CORTEX will strive to use the memory provided
** to satisfy page cache needs, falling back to [cortex_malloc()] if
** a page cache line is larger than sz bytes or if all of the pMem buffer
** is exhausted.
** ^If pMem is NULL and N is non-zero, then each database connection
** does an initial bulk allocation for page cache memory
** from [cortex_malloc()] sufficient for N cache lines if N is positive or
** of -1024*N bytes if N is negative. ^If additional
** page cache memory is needed beyond what is provided by the initial
** allocation, then CORTEX goes to [cortex_malloc()] separately for each
** additional cache line. </dd>
**
** [[CORTEX_CONFIG_HEAP]] <dt>CORTEX_CONFIG_HEAP</dt>
** <dd> ^The CORTEX_CONFIG_HEAP option specifies a static memory buffer
** that CORTEX will use for all of its dynamic memory allocation needs
** beyond those provided for by [CORTEX_CONFIG_PAGECACHE].
** ^The CORTEX_CONFIG_HEAP option is only available if CORTEX is compiled
** with either [CORTEX_ENABLE_MEMSYS3] or [CORTEX_ENABLE_MEMSYS5] and returns
** [CORTEX_ERROR] if invoked otherwise.
** ^There are three arguments to CORTEX_CONFIG_HEAP:
** An 8-byte aligned pointer to the memory,
** the number of bytes in the memory buffer, and the minimum allocation size.
** ^If the first pointer (the memory pointer) is NULL, then CORTEX reverts
** to using its default memory allocator (the system malloc() implementation),
** undoing any prior invocation of [CORTEX_CONFIG_MALLOC].  ^If the
** memory pointer is not NULL then the alternative memory
** allocator is engaged to handle all of SQLites memory allocation needs.
** The first pointer (the memory pointer) must be aligned to an 8-byte
** boundary or subsequent behavior of CORTEX will be undefined.
** The minimum allocation size is capped at 2**12. Reasonable values
** for the minimum allocation size are 2**5 through 2**8.</dd>
**
** [[CORTEX_CONFIG_MUTEX]] <dt>CORTEX_CONFIG_MUTEX</dt>
** <dd> ^(The CORTEX_CONFIG_MUTEX option takes a single argument which is a
** pointer to an instance of the [cortex_mutex_methods] structure.
** The argument specifies alternative low-level mutex routines to be used
** in place of the mutex routines built into SQLite.)^  ^SQLite makes a copy of
** the content of the [cortex_mutex_methods] structure before the call to
** [cortex_config()] returns. ^If CORTEX is compiled with
** the [CORTEX_THREADSAFE | CORTEX_THREADSAFE=0] compile-time option then
** the entire mutexing subsystem is omitted from the build and hence calls to
** [cortex_config()] with the CORTEX_CONFIG_MUTEX configuration option will
** return [CORTEX_ERROR].</dd>
**
** [[CORTEX_CONFIG_GETMUTEX]] <dt>CORTEX_CONFIG_GETMUTEX</dt>
** <dd> ^(The CORTEX_CONFIG_GETMUTEX option takes a single argument which
** is a pointer to an instance of the [cortex_mutex_methods] structure.  The
** [cortex_mutex_methods]
** structure is filled with the currently defined mutex routines.)^
** This option can be used to overload the default mutex allocation
** routines with a wrapper used to track mutex usage for performance
** profiling or testing, for example.   ^If CORTEX is compiled with
** the [CORTEX_THREADSAFE | CORTEX_THREADSAFE=0] compile-time option then
** the entire mutexing subsystem is omitted from the build and hence calls to
** [cortex_config()] with the CORTEX_CONFIG_GETMUTEX configuration option will
** return [CORTEX_ERROR].</dd>
**
** [[CORTEX_CONFIG_LOOKASIDE]] <dt>CORTEX_CONFIG_LOOKASIDE</dt>
** <dd> ^(The CORTEX_CONFIG_LOOKASIDE option takes two arguments that determine
** the default size of [lookaside memory] on each [database connection].
** The first argument is the
** size of each lookaside buffer slot ("sz") and the second is the number of
** slots allocated to each database connection ("cnt").)^
** ^(CORTEX_CONFIG_LOOKASIDE sets the <i>default</i> lookaside size.
** The [CORTEX_DBCONFIG_LOOKASIDE] option to [cortex_db_config()] can
** be used to change the lookaside configuration on individual connections.)^
** The [-DCORTEX_DEFAULT_LOOKASIDE] option can be used to change the
** default lookaside configuration at compile-time.
** </dd>
**
** [[CORTEX_CONFIG_PCACHE2]] <dt>CORTEX_CONFIG_PCACHE2</dt>
** <dd> ^(The CORTEX_CONFIG_PCACHE2 option takes a single argument which is
** a pointer to an [cortex_pcache_methods2] object.  This object specifies
** the interface to a custom page cache implementation.)^
** ^SQLite makes a copy of the [cortex_pcache_methods2] object.</dd>
**
** [[CORTEX_CONFIG_GETPCACHE2]] <dt>CORTEX_CONFIG_GETPCACHE2</dt>
** <dd> ^(The CORTEX_CONFIG_GETPCACHE2 option takes a single argument which
** is a pointer to an [cortex_pcache_methods2] object.  CORTEX copies off
** the current page cache implementation into that object.)^ </dd>
**
** [[CORTEX_CONFIG_LOG]] <dt>CORTEX_CONFIG_LOG</dt>
** <dd> The CORTEX_CONFIG_LOG option is used to configure the SQLite
** global [error log].
** (^The CORTEX_CONFIG_LOG option takes two arguments: a pointer to a
** function with a call signature of void(*)(void*,int,const char*),
** and a pointer to void. ^If the function pointer is not NULL, it is
** invoked by [cortex_log()] to process each logging event.  ^If the
** function pointer is NULL, the [cortex_log()] interface becomes a no-op.
** ^The void pointer that is the second argument to CORTEX_CONFIG_LOG is
** passed through as the first parameter to the application-defined logger
** function whenever that function is invoked.  ^The second parameter to
** the logger function is a copy of the first parameter to the corresponding
** [cortex_log()] call and is intended to be a [result code] or an
** [extended result code].  ^The third parameter passed to the logger is
** a log message after formatting via [cortex_snprintf()].
** The CORTEX logging interface is not reentrant; the logger function
** supplied by the application must not invoke any CORTEX interface.
** In a multi-threaded application, the application-defined logger
** function must be threadsafe. </dd>
**
** [[CORTEX_CONFIG_URI]] <dt>CORTEX_CONFIG_URI
** <dd>^(The CORTEX_CONFIG_URI option takes a single argument of type int.
** If non-zero, then URI handling is globally enabled. If the parameter is zero,
** then URI handling is globally disabled.)^ ^If URI handling is globally
** enabled, all filenames passed to [cortex_open()], [cortex_open_v2()],
** [cortex_open16()] or
** specified as part of [ATTACH] commands are interpreted as URIs, regardless
** of whether or not the [CORTEX_OPEN_URI] flag is set when the database
** connection is opened. ^If it is globally disabled, filenames are
** only interpreted as URIs if the CORTEX_OPEN_URI flag is set when the
** database connection is opened. ^(By default, URI handling is globally
** disabled. The default value may be changed by compiling with the
** [CORTEX_USE_URI] symbol defined.)^
**
** [[CORTEX_CONFIG_COVERING_INDEX_SCAN]] <dt>CORTEX_CONFIG_COVERING_INDEX_SCAN
** <dd>^The CORTEX_CONFIG_COVERING_INDEX_SCAN option takes a single integer
** argument which is interpreted as a boolean in order to enable or disable
** the use of covering indices for full table scans in the query optimizer.
** ^The default setting is determined
** by the [CORTEX_ALLOW_COVERING_INDEX_SCAN] compile-time option, or is "on"
** if that compile-time option is omitted.
** The ability to disable the use of covering indices for full table scans
** is because some incorrectly coded legacy applications might malfunction
** when the optimization is enabled.  Providing the ability to
** disable the optimization allows the older, buggy application code to work
** without change even with newer versions of SQLite.
**
** [[CORTEX_CONFIG_PCACHE]] [[CORTEX_CONFIG_GETPCACHE]]
** <dt>CORTEX_CONFIG_PCACHE and CORTEX_CONFIG_GETPCACHE
** <dd> These options are obsolete and should not be used by new code.
** They are retained for backwards compatibility but are now no-ops.
** </dd>
**
** [[CORTEX_CONFIG_SQLLOG]]
** <dt>CORTEX_CONFIG_SQLLOG
** <dd>This option is only available if CORTEX is compiled with the
** [CORTEX_ENABLE_SQLLOG] pre-processor macro defined. The first argument should
** be a pointer to a function of type void(*)(void*,cortex*,const char*, int).
** The second should be of type (void*). The callback is invoked by the library
** in three separate circumstances, identified by the value passed as the
** fourth parameter. If the fourth parameter is 0, then the database connection
** passed as the second argument has just been opened. The third argument
** points to a buffer containing the name of the main database file. If the
** fourth parameter is 1, then the SQL statement that the third parameter
** points to has just been executed. Or, if the fourth parameter is 2, then
** the connection being passed as the second parameter is being closed. The
** third parameter is passed NULL In this case.  An example of using this
** configuration option can be seen in the "test_sqllog.c" source file in
** the canonical CORTEX source tree.</dd>
**
** [[CORTEX_CONFIG_MMAP_SIZE]]
** <dt>CORTEX_CONFIG_MMAP_SIZE
** <dd>^CORTEX_CONFIG_MMAP_SIZE takes two 64-bit integer (cortex_int64) values
** that are the default mmap size limit (the default setting for
** [PRAGMA mmap_size]) and the maximum allowed mmap size limit.
** ^The default setting can be overridden by each database connection using
** either the [PRAGMA mmap_size] command, or by using the
** [CORTEX_FCNTL_MMAP_SIZE] file control.  ^(The maximum allowed mmap size
** will be silently truncated if necessary so that it does not exceed the
** compile-time maximum mmap size set by the
** [CORTEX_MAX_MMAP_SIZE] compile-time option.)^
** ^If either argument to this option is negative, then that argument is
** changed to its compile-time default.
**
** [[CORTEX_CONFIG_WIN32_HEAPSIZE]]
** <dt>CORTEX_CONFIG_WIN32_HEAPSIZE
** <dd>^The CORTEX_CONFIG_WIN32_HEAPSIZE option is only available if CORTEX is
** compiled for Windows with the [CORTEX_WIN32_MALLOC] pre-processor macro
** defined. ^CORTEX_CONFIG_WIN32_HEAPSIZE takes a 32-bit unsigned integer value
** that specifies the maximum size of the created heap.
**
** [[CORTEX_CONFIG_PCACHE_HDRSZ]]
** <dt>CORTEX_CONFIG_PCACHE_HDRSZ
** <dd>^The CORTEX_CONFIG_PCACHE_HDRSZ option takes a single parameter which
** is a pointer to an integer and writes into that integer the number of extra
** bytes per page required for each page in [CORTEX_CONFIG_PAGECACHE].
** The amount of extra space required can change depending on the compiler,
** target platform, and CORTEX version.
**
** [[CORTEX_CONFIG_PMASZ]]
** <dt>CORTEX_CONFIG_PMASZ
** <dd>^The CORTEX_CONFIG_PMASZ option takes a single parameter which
** is an unsigned integer and sets the "Minimum PMA Size" for the multithreaded
** sorter to that integer.  The default minimum PMA Size is set by the
** [CORTEX_SORTER_PMASZ] compile-time option.  New threads are launched
** to help with sort operations when multithreaded sorting
** is enabled (using the [PRAGMA threads] command) and the amount of content
** to be sorted exceeds the page size times the minimum of the
** [PRAGMA cache_size] setting and this value.
**
** [[CORTEX_CONFIG_STMTJRNL_SPILL]]
** <dt>CORTEX_CONFIG_STMTJRNL_SPILL
** <dd>^The CORTEX_CONFIG_STMTJRNL_SPILL option takes a single parameter which
** becomes the [statement journal] spill-to-disk threshold.
** [Statement journals] are held in memory until their size (in bytes)
** exceeds this threshold, at which point they are written to disk.
** Or if the threshold is -1, statement journals are always held
** exclusively in memory.
** Since many statement journals never become large, setting the spill
** threshold to a value such as 64KiB can greatly reduce the amount of
** I/O required to support statement rollback.
** The default value for this setting is controlled by the
** [CORTEX_STMTJRNL_SPILL] compile-time option.
**
** [[CORTEX_CONFIG_SORTERREF_SIZE]]
** <dt>CORTEX_CONFIG_SORTERREF_SIZE
** <dd>The CORTEX_CONFIG_SORTERREF_SIZE option accepts a single parameter
** of type (int) - the new value of the sorter-reference size threshold.
** Usually, when CORTEX uses an external sort to order records according
** to an ORDER BY clause, all fields required by the caller are present in the
** sorted records. However, if CORTEX determines based on the declared type
** of a table column that its values are likely to be very large - larger
** than the configured sorter-reference size threshold - then a reference
** is stored in each sorted record and the required column values loaded
** from the database as records are returned in sorted order. The default
** value for this option is to never use this optimization. Specifying a
** negative value for this option restores the default behavior.
** This option is only available if CORTEX is compiled with the
** [CORTEX_ENABLE_SORTER_REFERENCES] compile-time option.
**
** [[CORTEX_CONFIG_MEMDB_MAXSIZE]]
** <dt>CORTEX_CONFIG_MEMDB_MAXSIZE
** <dd>The CORTEX_CONFIG_MEMDB_MAXSIZE option accepts a single parameter
** [cortex_int64] parameter which is the default maximum size for an in-memory
** database created using [cortex_deserialize()].  This default maximum
** size can be adjusted up or down for individual databases using the
** [CORTEX_FCNTL_SIZE_LIMIT] [cortex_file_control|file-control].  If this
** configuration setting is never used, then the default maximum is determined
** by the [CORTEX_MEMDB_DEFAULT_MAXSIZE] compile-time option.  If that
** compile-time option is not set, then the default maximum is 1073741824.
**
** [[CORTEX_CONFIG_ROWID_IN_VIEW]]
** <dt>CORTEX_CONFIG_ROWID_IN_VIEW
** <dd>The CORTEX_CONFIG_ROWID_IN_VIEW option enables or disables the ability
** for VIEWs to have a ROWID.  The capability can only be enabled if CORTEX is
** compiled with -DCORTEX_ALLOW_ROWID_IN_VIEW, in which case the capability
** defaults to on.  This configuration option queries the current setting or
** changes the setting to off or on.  The argument is a pointer to an integer.
** If that integer initially holds a value of 1, then the ability for VIEWs to
** have ROWIDs is activated.  If the integer initially holds zero, then the
** ability is deactivated.  Any other initial value for the integer leaves the
** setting unchanged.  After changes, if any, the integer is written with
** a 1 or 0, if the ability for VIEWs to have ROWIDs is on or off.  If SQLite
** is compiled without -DCORTEX_ALLOW_ROWID_IN_VIEW (which is the usual and
** recommended case) then the integer is always filled with zero, regardless
** if its initial value.
** </dl>
*/
#define CORTEX_CONFIG_SINGLETHREAD         1  /* nil */
#define CORTEX_CONFIG_MULTITHREAD          2  /* nil */
#define CORTEX_CONFIG_SERIALIZED           3  /* nil */
#define CORTEX_CONFIG_MALLOC               4  /* cortex_mem_methods* */
#define CORTEX_CONFIG_GETMALLOC            5  /* cortex_mem_methods* */
#define CORTEX_CONFIG_SCRATCH              6  /* No longer used */
#define CORTEX_CONFIG_PAGECACHE            7  /* void*, int sz, int N */
#define CORTEX_CONFIG_HEAP                 8  /* void*, int nByte, int min */
#define CORTEX_CONFIG_MEMSTATUS            9  /* boolean */
#define CORTEX_CONFIG_MUTEX               10  /* cortex_mutex_methods* */
#define CORTEX_CONFIG_GETMUTEX            11  /* cortex_mutex_methods* */
/* previously CORTEX_CONFIG_CHUNKALLOC    12 which is now unused. */
#define CORTEX_CONFIG_LOOKASIDE           13  /* int int */
#define CORTEX_CONFIG_PCACHE              14  /* no-op */
#define CORTEX_CONFIG_GETPCACHE           15  /* no-op */
#define CORTEX_CONFIG_LOG                 16  /* xFunc, void* */
#define CORTEX_CONFIG_URI                 17  /* int */
#define CORTEX_CONFIG_PCACHE2             18  /* cortex_pcache_methods2* */
#define CORTEX_CONFIG_GETPCACHE2          19  /* cortex_pcache_methods2* */
#define CORTEX_CONFIG_COVERING_INDEX_SCAN 20  /* int */
#define CORTEX_CONFIG_SQLLOG              21  /* xSqllog, void* */
#define CORTEX_CONFIG_MMAP_SIZE           22  /* cortex_int64, cortex_int64 */
#define CORTEX_CONFIG_WIN32_HEAPSIZE      23  /* int nByte */
#define CORTEX_CONFIG_PCACHE_HDRSZ        24  /* int *psz */
#define CORTEX_CONFIG_PMASZ               25  /* unsigned int szPma */
#define CORTEX_CONFIG_STMTJRNL_SPILL      26  /* int nByte */
#define CORTEX_CONFIG_SMALL_MALLOC        27  /* boolean */
#define CORTEX_CONFIG_SORTERREF_SIZE      28  /* int nByte */
#define CORTEX_CONFIG_MEMDB_MAXSIZE       29  /* cortex_int64 */
#define CORTEX_CONFIG_ROWID_IN_VIEW       30  /* int* */

/*
** CAPI3REF: Database Connection Configuration Options
**
** These constants are the available integer configuration options that
** can be passed as the second parameter to the [cortex_db_config()] interface.
**
** The [cortex_db_config()] interface is a var-args function.  It takes a
** variable number of parameters, though always at least two.  The number of
** parameters passed into cortex_db_config() depends on which of these
** constants is given as the second parameter.  This documentation page
** refers to parameters beyond the second as "arguments".  Thus, when this
** page says "the N-th argument" it means "the N-th parameter past the
** configuration option" or "the (N+2)-th parameter to cortex_db_config()".
**
** New configuration options may be added in future releases of SQLite.
** Existing configuration options might be discontinued.  Applications
** should check the return code from [cortex_db_config()] to make sure that
** the call worked.  ^The [cortex_db_config()] interface will return a
** non-zero [error code] if a discontinued or unsupported configuration option
** is invoked.
**
** <dl>
** [[CORTEX_DBCONFIG_LOOKASIDE]]
** <dt>CORTEX_DBCONFIG_LOOKASIDE</dt>
** <dd> The CORTEX_DBCONFIG_LOOKASIDE option is used to adjust the
** configuration of the [lookaside memory allocator] within a database
** connection.
** The arguments to the CORTEX_DBCONFIG_LOOKASIDE option are <i>not</i>
** in the [DBCONFIG arguments|usual format].
** The CORTEX_DBCONFIG_LOOKASIDE option takes three arguments, not two,
** so that a call to [cortex_db_config()] that uses CORTEX_DBCONFIG_LOOKASIDE
** should have a total of five parameters.
** <ol>
** <li><p>The first argument ("buf") is a
** pointer to a memory buffer to use for lookaside memory.
** The first argument may be NULL in which case CORTEX will allocate the
** lookaside buffer itself using [cortex_malloc()].
** <li><P>The second argument ("sz") is the
** size of each lookaside buffer slot.  Lookaside is disabled if "sz"
** is less than 8.  The "sz" argument should be a multiple of 8 less than
** 65536.  If "sz" does not meet this constraint, it is reduced in size until
** it does.
** <li><p>The third argument ("cnt") is the number of slots. Lookaside is disabled
** if "cnt"is less than 1.  The "cnt" value will be reduced, if necessary, so
** that the product of "sz" and "cnt" does not exceed 2,147,418,112.  The "cnt"
** parameter is usually chosen so that the product of "sz" and "cnt" is less
** than 1,000,000.
** </ol>
** <p>If the "buf" argument is not NULL, then it must
** point to a memory buffer with a size that is greater than
** or equal to the product of "sz" and "cnt".
** The buffer must be aligned to an 8-byte boundary.
** The lookaside memory
** configuration for a database connection can only be changed when that
** connection is not currently using lookaside memory, or in other words
** when the value returned by [CORTEX_DBSTATUS_LOOKASIDE_USED] is zero.
** Any attempt to change the lookaside memory configuration when lookaside
** memory is in use leaves the configuration unchanged and returns
** [CORTEX_BUSY].
** If the "buf" argument is NULL and an attempt
** to allocate memory based on "sz" and "cnt" fails, then
** lookaside is silently disabled.
** <p>
** The [CORTEX_CONFIG_LOOKASIDE] configuration option can be used to set the
** default lookaside configuration at initialization.  The
** [-DCORTEX_DEFAULT_LOOKASIDE] option can be used to set the default lookaside
** configuration at compile-time.  Typical values for lookaside are 1200 for
** "sz" and 40 to 100 for "cnt".
** </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_FKEY]]
** <dt>CORTEX_DBCONFIG_ENABLE_FKEY</dt>
** <dd> ^This option is used to enable or disable the enforcement of
** [foreign key constraints].  This is the same setting that is
** enabled or disabled by the [PRAGMA foreign_keys] statement.
** The first argument is an integer which is 0 to disable FK enforcement,
** positive to enable FK enforcement or negative to leave FK enforcement
** unchanged.  The second parameter is a pointer to an integer into which
** is written 0 or 1 to indicate whether FK enforcement is off or on
** following this call.  The second parameter may be a NULL pointer, in
** which case the FK enforcement setting is not reported back. </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_TRIGGER]]
** <dt>CORTEX_DBCONFIG_ENABLE_TRIGGER</dt>
** <dd> ^This option is used to enable or disable [CREATE TRIGGER | triggers].
** There should be two additional arguments.
** The first argument is an integer which is 0 to disable triggers,
** positive to enable triggers or negative to leave the setting unchanged.
** The second parameter is a pointer to an integer into which
** is written 0 or 1 to indicate whether triggers are disabled or enabled
** following this call.  The second parameter may be a NULL pointer, in
** which case the trigger setting is not reported back.
**
** <p>Originally this option disabled all triggers.  ^(However, since
** CORTEX version 3.35.0, TEMP triggers are still allowed even if
** this option is off.  So, in other words, this option now only disables
** triggers in the main database schema or in the schemas of [ATTACH]-ed
** databases.)^ </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_VIEW]]
** <dt>CORTEX_DBCONFIG_ENABLE_VIEW</dt>
** <dd> ^This option is used to enable or disable [CREATE VIEW | views].
** There must be two additional arguments.
** The first argument is an integer which is 0 to disable views,
** positive to enable views or negative to leave the setting unchanged.
** The second parameter is a pointer to an integer into which
** is written 0 or 1 to indicate whether views are disabled or enabled
** following this call.  The second parameter may be a NULL pointer, in
** which case the view setting is not reported back.
**
** <p>Originally this option disabled all views.  ^(However, since
** CORTEX version 3.35.0, TEMP views are still allowed even if
** this option is off.  So, in other words, this option now only disables
** views in the main database schema or in the schemas of ATTACH-ed
** databases.)^ </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_FTS3_TOKENIZER]]
** <dt>CORTEX_DBCONFIG_ENABLE_FTS3_TOKENIZER</dt>
** <dd> ^This option is used to enable or disable using the
** [fts3_tokenizer()] function - part of the [FTS3] full-text search engine
** extension - without using bound parameters as the parameters. Doing so
** is disabled by default. There must be two additional arguments. The first
** argument is an integer. If it is passed 0, then using fts3_tokenizer()
** without bound parameters is disabled. If it is passed a positive value,
** then calling fts3_tokenizer without bound parameters is enabled. If it
** is passed a negative value, this setting is not modified - this can be
** used to query for the current setting. The second parameter is a pointer
** to an integer into which is written 0 or 1 to indicate the current value
** of this setting (after it is modified, if applicable).  The second
** parameter may be a NULL pointer, in which case the value of the setting
** is not reported back. Refer to [FTS3] documentation for further details.
** </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_LOAD_EXTENSION]]
** <dt>CORTEX_DBCONFIG_ENABLE_LOAD_EXTENSION</dt>
** <dd> ^This option is used to enable or disable the [cortex_load_extension()]
** interface independently of the [load_extension()] SQL function.
** The [cortex_enable_load_extension()] API enables or disables both the
** C-API [cortex_load_extension()] and the SQL function [load_extension()].
** There must be two additional arguments.
** When the first argument to this interface is 1, then only the C-API is
** enabled and the SQL function remains disabled.  If the first argument to
** this interface is 0, then both the C-API and the SQL function are disabled.
** If the first argument is -1, then no changes are made to the state of either
** the C-API or the SQL function.
** The second parameter is a pointer to an integer into which
** is written 0 or 1 to indicate whether [cortex_load_extension()] interface
** is disabled or enabled following this call.  The second parameter may
** be a NULL pointer, in which case the new setting is not reported back.
** </dd>
**
** [[CORTEX_DBCONFIG_MAINDBNAME]] <dt>CORTEX_DBCONFIG_MAINDBNAME</dt>
** <dd> ^This option is used to change the name of the "main" database
** schema.  This option does not follow the
** [DBCONFIG arguments|usual CORTEX_DBCONFIG argument format].
** This option takes exactly one additional argument so that the
** [cortex_db_config()] call has a total of three parameters.  The
** extra argument must be a pointer to a constant UTF8 string which
** will become the new schema name in place of "main".  ^SQLite does
** not make a copy of the new main schema name string, so the application
** must ensure that the argument passed into CORTEX_DBCONFIG MAINDBNAME
** is unchanged until after the database connection closes.
** </dd>
**
** [[CORTEX_DBCONFIG_NO_CKPT_ON_CLOSE]]
** <dt>CORTEX_DBCONFIG_NO_CKPT_ON_CLOSE</dt>
** <dd> Usually, when a database in [WAL mode] is closed or detached from a
** database handle, CORTEX checks if if there are other connections to the
** same database, and if there are no other database connection (if the
** connection being closed is the last open connection to the database),
** then CORTEX performs a [checkpoint] before closing the connection and
** deletes the WAL file.  The CORTEX_DBCONFIG_NO_CKPT_ON_CLOSE option can
** be used to override that behavior. The first argument passed to this
** operation (the third parameter to [cortex_db_config()]) is an integer
** which is positive to disable checkpoints-on-close, or zero (the default)
** to enable them, and negative to leave the setting unchanged.
** The second argument (the fourth parameter) is a pointer to an integer
** into which is written 0 or 1 to indicate whether checkpoints-on-close
** have been disabled - 0 if they are not disabled, 1 if they are.
** </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_QPSG]] <dt>CORTEX_DBCONFIG_ENABLE_QPSG</dt>
** <dd>^(The CORTEX_DBCONFIG_ENABLE_QPSG option activates or deactivates
** the [query planner stability guarantee] (QPSG).  When the QPSG is active,
** a single SQL query statement will always use the same algorithm regardless
** of values of [bound parameters].)^ The QPSG disables some query optimizations
** that look at the values of bound parameters, which can make some queries
** slower.  But the QPSG has the advantage of more predictable behavior.  With
** the QPSG active, CORTEX will always use the same query plan in the field as
** was used during testing in the lab.
** The first argument to this setting is an integer which is 0 to disable
** the QPSG, positive to enable QPSG, or negative to leave the setting
** unchanged. The second parameter is a pointer to an integer into which
** is written 0 or 1 to indicate whether the QPSG is disabled or enabled
** following this call.
** </dd>
**
** [[CORTEX_DBCONFIG_TRIGGER_EQP]] <dt>CORTEX_DBCONFIG_TRIGGER_EQP</dt>
** <dd> By default, the output of EXPLAIN QUERY PLAN commands does not
** include output for any operations performed by trigger programs. This
** option is used to set or clear (the default) a flag that governs this
** behavior. The first parameter passed to this operation is an integer -
** positive to enable output for trigger programs, or zero to disable it,
** or negative to leave the setting unchanged.
** The second parameter is a pointer to an integer into which is written
** 0 or 1 to indicate whether output-for-triggers has been disabled - 0 if
** it is not disabled, 1 if it is.
** </dd>
**
** [[CORTEX_DBCONFIG_RESET_DATABASE]] <dt>CORTEX_DBCONFIG_RESET_DATABASE</dt>
** <dd> Set the CORTEX_DBCONFIG_RESET_DATABASE flag and then run
** [VACUUM] in order to reset a database back to an empty database
** with no schema and no content. The following process works even for
** a badly corrupted database file:
** <ol>
** <li> If the database connection is newly opened, make sure it has read the
**      database schema by preparing then discarding some query against the
**      database, or calling cortex_table_column_metadata(), ignoring any
**      errors.  This step is only necessary if the application desires to keep
**      the database in WAL mode after the reset if it was in WAL mode before
**      the reset.
** <li> cortex_db_config(db, CORTEX_DBCONFIG_RESET_DATABASE, 1, 0);
** <li> [cortex_exec](db, "[VACUUM]", 0, 0, 0);
** <li> cortex_db_config(db, CORTEX_DBCONFIG_RESET_DATABASE, 0, 0);
** </ol>
** Because resetting a database is destructive and irreversible, the
** process requires the use of this obscure API and multiple steps to
** help ensure that it does not happen by accident. Because this
** feature must be capable of resetting corrupt databases, and
** shutting down virtual tables may require access to that corrupt
** storage, the library must abandon any installed virtual tables
** without calling their xDestroy() methods.
**
** [[CORTEX_DBCONFIG_DEFENSIVE]] <dt>CORTEX_DBCONFIG_DEFENSIVE</dt>
** <dd>The CORTEX_DBCONFIG_DEFENSIVE option activates or deactivates the
** "defensive" flag for a database connection.  When the defensive
** flag is enabled, language features that allow ordinary SQL to
** deliberately corrupt the database file are disabled.  The disabled
** features include but are not limited to the following:
** <ul>
** <li> The [PRAGMA writable_schema=ON] statement.
** <li> The [PRAGMA journal_mode=OFF] statement.
** <li> The [PRAGMA schema_version=N] statement.
** <li> Writes to the [CORTEX_dbpage] virtual table.
** <li> Direct writes to [shadow tables].
** </ul>
** </dd>
**
** [[CORTEX_DBCONFIG_WRITABLE_SCHEMA]] <dt>CORTEX_DBCONFIG_WRITABLE_SCHEMA</dt>
** <dd>The CORTEX_DBCONFIG_WRITABLE_SCHEMA option activates or deactivates the
** "writable_schema" flag. This has the same effect and is logically equivalent
** to setting [PRAGMA writable_schema=ON] or [PRAGMA writable_schema=OFF].
** The first argument to this setting is an integer which is 0 to disable
** the writable_schema, positive to enable writable_schema, or negative to
** leave the setting unchanged. The second parameter is a pointer to an
** integer into which is written 0 or 1 to indicate whether the writable_schema
** is enabled or disabled following this call.
** </dd>
**
** [[CORTEX_DBCONFIG_LEGACY_ALTER_TABLE]]
** <dt>CORTEX_DBCONFIG_LEGACY_ALTER_TABLE</dt>
** <dd>The CORTEX_DBCONFIG_LEGACY_ALTER_TABLE option activates or deactivates
** the legacy behavior of the [ALTER TABLE RENAME] command such that it
** behaves as it did prior to [version 3.24.0] (2018-06-04).  See the
** "Compatibility Notice" on the [ALTER TABLE RENAME documentation] for
** additional information. This feature can also be turned on and off
** using the [PRAGMA legacy_alter_table] statement.
** </dd>
**
** [[CORTEX_DBCONFIG_DQS_DML]]
** <dt>CORTEX_DBCONFIG_DQS_DML</dt>
** <dd>The CORTEX_DBCONFIG_DQS_DML option activates or deactivates
** the legacy [double-quoted string literal] misfeature for DML statements
** only, that is DELETE, INSERT, SELECT, and UPDATE statements. The
** default value of this setting is determined by the [-DCORTEX_DQS]
** compile-time option.
** </dd>
**
** [[CORTEX_DBCONFIG_DQS_DDL]]
** <dt>CORTEX_DBCONFIG_DQS_DDL</dt>
** <dd>The CORTEX_DBCONFIG_DQS option activates or deactivates
** the legacy [double-quoted string literal] misfeature for DDL statements,
** such as CREATE TABLE and CREATE INDEX. The
** default value of this setting is determined by the [-DCORTEX_DQS]
** compile-time option.
** </dd>
**
** [[CORTEX_DBCONFIG_TRUSTED_SCHEMA]]
** <dt>CORTEX_DBCONFIG_TRUSTED_SCHEMA</dt>
** <dd>The CORTEX_DBCONFIG_TRUSTED_SCHEMA option tells CORTEX to
** assume that database schemas are untainted by malicious content.
** When the CORTEX_DBCONFIG_TRUSTED_SCHEMA option is disabled, SQLite
** takes additional defensive steps to protect the application from harm
** including:
** <ul>
** <li> Prohibit the use of SQL functions inside triggers, views,
** CHECK constraints, DEFAULT clauses, expression indexes,
** partial indexes, or generated columns
** unless those functions are tagged with [CORTEX_INNOCUOUS].
** <li> Prohibit the use of virtual tables inside of triggers or views
** unless those virtual tables are tagged with [CORTEX_VTAB_INNOCUOUS].
** </ul>
** This setting defaults to "on" for legacy compatibility, however
** all applications are advised to turn it off if possible. This setting
** can also be controlled using the [PRAGMA trusted_schema] statement.
** </dd>
**
** [[CORTEX_DBCONFIG_LEGACY_FILE_FORMAT]]
** <dt>CORTEX_DBCONFIG_LEGACY_FILE_FORMAT</dt>
** <dd>The CORTEX_DBCONFIG_LEGACY_FILE_FORMAT option activates or deactivates
** the legacy file format flag.  When activated, this flag causes all newly
** created database files to have a schema format version number (the 4-byte
** integer found at offset 44 into the database header) of 1.  This in turn
** means that the resulting database file will be readable and writable by
** any CORTEX version back to 3.0.0 ([dateof:3.0.0]).  Without this setting,
** newly created databases are generally not understandable by CORTEX versions
** prior to 3.3.0 ([dateof:3.3.0]).  As these words are written, there
** is now scarcely any need to generate database files that are compatible
** all the way back to version 3.0.0, and so this setting is of little
** practical use, but is provided so that CORTEX can continue to claim the
** ability to generate new database files that are compatible with  version
** 3.0.0.
** <p>Note that when the CORTEX_DBCONFIG_LEGACY_FILE_FORMAT setting is on,
** the [VACUUM] command will fail with an obscure error when attempting to
** process a table with generated columns and a descending index.  This is
** not considered a bug since CORTEX versions 3.3.0 and earlier do not support
** either generated columns or descending indexes.
** </dd>
**
** [[CORTEX_DBCONFIG_STMT_SCANSTATUS]]
** <dt>CORTEX_DBCONFIG_STMT_SCANSTATUS</dt>
** <dd>The CORTEX_DBCONFIG_STMT_SCANSTATUS option is only useful in
** CORTEX_ENABLE_STMT_SCANSTATUS builds. In this case, it sets or clears
** a flag that enables collection of the cortex_stmt_scanstatus_v2()
** statistics. For statistics to be collected, the flag must be set on
** the database handle both when the SQL statement is prepared and when it
** is stepped. The flag is set (collection of statistics is enabled)
** by default. <p>This option takes two arguments: an integer and a pointer to
** an integer.  The first argument is 1, 0, or -1 to enable, disable, or
** leave unchanged the statement scanstatus option.  If the second argument
** is not NULL, then the value of the statement scanstatus setting after
** processing the first argument is written into the integer that the second
** argument points to.
** </dd>
**
** [[CORTEX_DBCONFIG_REVERSE_SCANORDER]]
** <dt>CORTEX_DBCONFIG_REVERSE_SCANORDER</dt>
** <dd>The CORTEX_DBCONFIG_REVERSE_SCANORDER option changes the default order
** in which tables and indexes are scanned so that the scans start at the end
** and work toward the beginning rather than starting at the beginning and
** working toward the end. Setting CORTEX_DBCONFIG_REVERSE_SCANORDER is the
** same as setting [PRAGMA reverse_unordered_selects]. <p>This option takes
** two arguments which are an integer and a pointer to an integer.  The first
** argument is 1, 0, or -1 to enable, disable, or leave unchanged the
** reverse scan order flag, respectively.  If the second argument is not NULL,
** then 0 or 1 is written into the integer that the second argument points to
** depending on if the reverse scan order flag is set after processing the
** first argument.
** </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_ATTACH_CREATE]]
** <dt>CORTEX_DBCONFIG_ENABLE_ATTACH_CREATE</dt>
** <dd>The CORTEX_DBCONFIG_ENABLE_ATTACH_CREATE option enables or disables
** the ability of the [ATTACH DATABASE] SQL command to create a new database
** file if the database filed named in the ATTACH command does not already
** exist.  This ability of ATTACH to create a new database is enabled by
** default.  Applications can disable or reenable the ability for ATTACH to
** create new database files using this DBCONFIG option.<p>
** This option takes two arguments which are an integer and a pointer
** to an integer.  The first argument is 1, 0, or -1 to enable, disable, or
** leave unchanged the attach-create flag, respectively.  If the second
** argument is not NULL, then 0 or 1 is written into the integer that the
** second argument points to depending on if the attach-create flag is set
** after processing the first argument.
** </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_ATTACH_WRITE]]
** <dt>CORTEX_DBCONFIG_ENABLE_ATTACH_WRITE</dt>
** <dd>The CORTEX_DBCONFIG_ENABLE_ATTACH_WRITE option enables or disables the
** ability of the [ATTACH DATABASE] SQL command to open a database for writing.
** This capability is enabled by default.  Applications can disable or
** reenable this capability using the current DBCONFIG option.  If
** this capability is disabled, the [ATTACH] command will still work,
** but the database will be opened read-only.  If this option is disabled,
** then the ability to create a new database using [ATTACH] is also disabled,
** regardless of the value of the [CORTEX_DBCONFIG_ENABLE_ATTACH_CREATE]
** option.<p>
** This option takes two arguments which are an integer and a pointer
** to an integer.  The first argument is 1, 0, or -1 to enable, disable, or
** leave unchanged the ability to ATTACH another database for writing,
** respectively.  If the second argument is not NULL, then 0 or 1 is written
** into the integer to which the second argument points, depending on whether
** the ability to ATTACH a read/write database is enabled or disabled
** after processing the first argument.
** </dd>
**
** [[CORTEX_DBCONFIG_ENABLE_COMMENTS]]
** <dt>CORTEX_DBCONFIG_ENABLE_COMMENTS</dt>
** <dd>The CORTEX_DBCONFIG_ENABLE_COMMENTS option enables or disables the
** ability to include comments in SQL text.  Comments are enabled by default.
** An application can disable or reenable comments in SQL text using this
** DBCONFIG option.<p>
** This option takes two arguments which are an integer and a pointer
** to an integer.  The first argument is 1, 0, or -1 to enable, disable, or
** leave unchanged the ability to use comments in SQL text,
** respectively.  If the second argument is not NULL, then 0 or 1 is written
** into the integer that the second argument points to depending on if
** comments are allowed in SQL text after processing the first argument.
** </dd>
**
** </dl>
**
** [[DBCONFIG arguments]] <h3>Arguments To CORTEX_DBCONFIG Options</h3>
**
** <p>Most of the CORTEX_DBCONFIG options take two arguments, so that the
** overall call to [cortex_db_config()] has a total of four parameters.
** The first argument (the third parameter to cortex_db_config()) is an integer.
** The second argument is a pointer to an integer.  If the first argument is 1,
** then the option becomes enabled.  If the first integer argument is 0, then the
** option is disabled.  If the first argument is -1, then the option setting
** is unchanged.  The second argument, the pointer to an integer, may be NULL.
** If the second argument is not NULL, then a value of 0 or 1 is written into
** the integer to which the second argument points, depending on whether the
** setting is disabled or enabled after applying any changes specified by
** the first argument.
**
** <p>While most CORTEX_DBCONFIG options use the argument format
** described in the previous paragraph, the [CORTEX_DBCONFIG_MAINDBNAME]
** and [CORTEX_DBCONFIG_LOOKASIDE] options are different.  See the
** documentation of those exceptional options for details.
*/
#define CORTEX_DBCONFIG_MAINDBNAME            1000 /* const char* */
#define CORTEX_DBCONFIG_LOOKASIDE             1001 /* void* int int */
#define CORTEX_DBCONFIG_ENABLE_FKEY           1002 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_TRIGGER        1003 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_FTS3_TOKENIZER 1004 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_LOAD_EXTENSION 1005 /* int int* */
#define CORTEX_DBCONFIG_NO_CKPT_ON_CLOSE      1006 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_QPSG           1007 /* int int* */
#define CORTEX_DBCONFIG_TRIGGER_EQP           1008 /* int int* */
#define CORTEX_DBCONFIG_RESET_DATABASE        1009 /* int int* */
#define CORTEX_DBCONFIG_DEFENSIVE             1010 /* int int* */
#define CORTEX_DBCONFIG_WRITABLE_SCHEMA       1011 /* int int* */
#define CORTEX_DBCONFIG_LEGACY_ALTER_TABLE    1012 /* int int* */
#define CORTEX_DBCONFIG_DQS_DML               1013 /* int int* */
#define CORTEX_DBCONFIG_DQS_DDL               1014 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_VIEW           1015 /* int int* */
#define CORTEX_DBCONFIG_LEGACY_FILE_FORMAT    1016 /* int int* */
#define CORTEX_DBCONFIG_TRUSTED_SCHEMA        1017 /* int int* */
#define CORTEX_DBCONFIG_STMT_SCANSTATUS       1018 /* int int* */
#define CORTEX_DBCONFIG_REVERSE_SCANORDER     1019 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_ATTACH_CREATE  1020 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_ATTACH_WRITE   1021 /* int int* */
#define CORTEX_DBCONFIG_ENABLE_COMMENTS       1022 /* int int* */
#define CORTEX_DBCONFIG_MAX                   1022 /* Largest DBCONFIG */

/*
** CAPI3REF: Enable Or Disable Extended Result Codes
** METHOD: cortex
**
** ^The cortex_extended_result_codes() routine enables or disables the
** [extended result codes] feature of SQLite. ^The extended result
** codes are disabled by default for historical compatibility.
*/
CORTEX_API int cortex_extended_result_codes(cortex*, int onoff);

/*
** CAPI3REF: Last Insert Rowid
** METHOD: cortex
**
** ^Each entry in most CORTEX tables (except for [WITHOUT ROWID] tables)
** has a unique 64-bit signed
** integer key called the [ROWID | "rowid"]. ^The rowid is always available
** as an undeclared column named ROWID, OID, or _ROWID_ as long as those
** names are not also used by explicitly declared columns. ^If
** the table has a column of type [INTEGER PRIMARY KEY] then that column
** is another alias for the rowid.
**
** ^The cortex_last_insert_rowid(D) interface usually returns the [rowid] of
** the most recent successful [INSERT] into a rowid table or [virtual table]
** on database connection D. ^Inserts into [WITHOUT ROWID] tables are not
** recorded. ^If no successful [INSERT]s into rowid tables have ever occurred
** on the database connection D, then cortex_last_insert_rowid(D) returns
** zero.
**
** As well as being set automatically as rows are inserted into database
** tables, the value returned by this function may be set explicitly by
** [cortex_set_last_insert_rowid()]
**
** Some virtual table implementations may INSERT rows into rowid tables as
** part of committing a transaction (e.g. to flush data accumulated in memory
** to disk). In this case subsequent calls to this function return the rowid
** associated with these internal INSERT operations, which leads to
** unintuitive results. Virtual table implementations that do write to rowid
** tables in this way can avoid this problem by restoring the original
** rowid value using [cortex_set_last_insert_rowid()] before returning
** control to the user.
**
** ^(If an [INSERT] occurs within a trigger then this routine will
** return the [rowid] of the inserted row as long as the trigger is
** running. Once the trigger program ends, the value returned
** by this routine reverts to what it was before the trigger was fired.)^
**
** ^An [INSERT] that fails due to a constraint violation is not a
** successful [INSERT] and does not change the value returned by this
** routine.  ^Thus INSERT OR FAIL, INSERT OR IGNORE, INSERT OR ROLLBACK,
** and INSERT OR ABORT make no changes to the return value of this
** routine when their insertion fails.  ^(When INSERT OR REPLACE
** encounters a constraint violation, it does not fail.  The
** INSERT continues to completion after deleting rows that caused
** the constraint problem so INSERT OR REPLACE will always change
** the return value of this interface.)^
**
** ^For the purposes of this routine, an [INSERT] is considered to
** be successful even if it is subsequently rolled back.
**
** This function is accessible to SQL statements via the
** [last_insert_rowid() SQL function].
**
** If a separate thread performs a new [INSERT] on the same
** database connection while the [cortex_last_insert_rowid()]
** function is running and thus changes the last insert [rowid],
** then the value returned by [cortex_last_insert_rowid()] is
** unpredictable and might not equal either the old or the new
** last insert [rowid].
*/
CORTEX_API cortex_int64 cortex_last_insert_rowid(cortex*);

/*
** CAPI3REF: Set the Last Insert Rowid value.
** METHOD: cortex
**
** The cortex_set_last_insert_rowid(D, R) method allows the application to
** set the value returned by calling cortex_last_insert_rowid(D) to R
** without inserting a row into the database.
*/
CORTEX_API void cortex_set_last_insert_rowid(cortex*,cortex_int64);

/*
** CAPI3REF: Count The Number Of Rows Modified
** METHOD: cortex
**
** ^These functions return the number of rows modified, inserted or
** deleted by the most recently completed INSERT, UPDATE or DELETE
** statement on the database connection specified by the only parameter.
** The two functions are identical except for the type of the return value
** and that if the number of rows modified by the most recent INSERT, UPDATE,
** or DELETE is greater than the maximum value supported by type "int", then
** the return value of cortex_changes() is undefined. ^Executing any other
** type of SQL statement does not modify the value returned by these functions.
** For the purposes of this interface, a CREATE TABLE AS SELECT statement
** does not count as an INSERT, UPDATE or DELETE statement and hence the rows
** added to the new table by the CREATE TABLE AS SELECT statement are not
** counted.
**
** ^Only changes made directly by the INSERT, UPDATE or DELETE statement are
** considered - auxiliary changes caused by [CREATE TRIGGER | triggers],
** [foreign key actions] or [REPLACE] constraint resolution are not counted.
**
** Changes to a view that are intercepted by
** [INSTEAD OF trigger | INSTEAD OF triggers] are not counted. ^The value
** returned by cortex_changes() immediately after an INSERT, UPDATE or
** DELETE statement run on a view is always zero. Only changes made to real
** tables are counted.
**
** Things are more complicated if the cortex_changes() function is
** executed while a trigger program is running. This may happen if the
** program uses the [changes() SQL function], or if some other callback
** function invokes cortex_changes() directly. Essentially:
**
** <ul>
**   <li> ^(Before entering a trigger program the value returned by
**        cortex_changes() function is saved. After the trigger program
**        has finished, the original value is restored.)^
**
**   <li> ^(Within a trigger program each INSERT, UPDATE and DELETE
**        statement sets the value returned by cortex_changes()
**        upon completion as normal. Of course, this value will not include
**        any changes performed by sub-triggers, as the cortex_changes()
**        value will be saved and restored after each sub-trigger has run.)^
** </ul>
**
** ^This means that if the changes() SQL function (or similar) is used
** by the first INSERT, UPDATE or DELETE statement within a trigger, it
** returns the value as set when the calling statement began executing.
** ^If it is used by the second or subsequent such statement within a trigger
** program, the value returned reflects the number of rows modified by the
** previous INSERT, UPDATE or DELETE statement within the same trigger.
**
** If a separate thread makes changes on the same database connection
** while [cortex_changes()] is running then the value returned
** is unpredictable and not meaningful.
**
** See also:
** <ul>
** <li> the [cortex_total_changes()] interface
** <li> the [count_changes pragma]
** <li> the [changes() SQL function]
** <li> the [data_version pragma]
** </ul>
*/
CORTEX_API int cortex_changes(cortex*);
CORTEX_API cortex_int64 cortex_changes64(cortex*);

/*
** CAPI3REF: Total Number Of Rows Modified
** METHOD: cortex
**
** ^These functions return the total number of rows inserted, modified or
** deleted by all [INSERT], [UPDATE] or [DELETE] statements completed
** since the database connection was opened, including those executed as
** part of trigger programs. The two functions are identical except for the
** type of the return value and that if the number of rows modified by the
** connection exceeds the maximum value supported by type "int", then
** the return value of cortex_total_changes() is undefined. ^Executing
** any other type of SQL statement does not affect the value returned by
** cortex_total_changes().
**
** ^Changes made as part of [foreign key actions] are included in the
** count, but those made as part of REPLACE constraint resolution are
** not. ^Changes to a view that are intercepted by INSTEAD OF triggers
** are not counted.
**
** The [cortex_total_changes(D)] interface only reports the number
** of rows that changed due to SQL statement run against database
** connection D.  Any changes by other database connections are ignored.
** To detect changes against a database file from other database
** connections use the [PRAGMA data_version] command or the
** [CORTEX_FCNTL_DATA_VERSION] [file control].
**
** If a separate thread makes changes on the same database connection
** while [cortex_total_changes()] is running then the value
** returned is unpredictable and not meaningful.
**
** See also:
** <ul>
** <li> the [cortex_changes()] interface
** <li> the [count_changes pragma]
** <li> the [changes() SQL function]
** <li> the [data_version pragma]
** <li> the [CORTEX_FCNTL_DATA_VERSION] [file control]
** </ul>
*/
CORTEX_API int cortex_total_changes(cortex*);
CORTEX_API cortex_int64 cortex_total_changes64(cortex*);

/*
** CAPI3REF: Interrupt A Long-Running Query
** METHOD: cortex
**
** ^This function causes any pending database operation to abort and
** return at its earliest opportunity. This routine is typically
** called in response to a user action such as pressing "Cancel"
** or Ctrl-C where the user wants a long query operation to halt
** immediately.
**
** ^It is safe to call this routine from a thread different from the
** thread that is currently running the database operation.  But it
** is not safe to call this routine with a [database connection] that
** is closed or might close before cortex_interrupt() returns.
**
** ^If an SQL operation is very nearly finished at the time when
** cortex_interrupt() is called, then it might not have an opportunity
** to be interrupted and might continue to completion.
**
** ^An SQL operation that is interrupted will return [CORTEX_INTERRUPT].
** ^If the interrupted SQL operation is an INSERT, UPDATE, or DELETE
** that is inside an explicit transaction, then the entire transaction
** will be rolled back automatically.
**
** ^The cortex_interrupt(D) call is in effect until all currently running
** SQL statements on [database connection] D complete.  ^Any new SQL statements
** that are started after the cortex_interrupt() call and before the
** running statement count reaches zero are interrupted as if they had been
** running prior to the cortex_interrupt() call.  ^New SQL statements
** that are started after the running statement count reaches zero are
** not effected by the cortex_interrupt().
** ^A call to cortex_interrupt(D) that occurs when there are no running
** SQL statements is a no-op and has no effect on SQL statements
** that are started after the cortex_interrupt() call returns.
**
** ^The [cortex_is_interrupted(D)] interface can be used to determine whether
** or not an interrupt is currently in effect for [database connection] D.
** It returns 1 if an interrupt is currently in effect, or 0 otherwise.
*/
CORTEX_API void cortex_interrupt(cortex*);
CORTEX_API int cortex_is_interrupted(cortex*);

/*
** CAPI3REF: Determine If An SQL Statement Is Complete
**
** These routines are useful during command-line input to determine if the
** currently entered text seems to form a complete SQL statement or
** if additional input is needed before sending the text into
** CORTEX for parsing.  ^These routines return 1 if the input string
** appears to be a complete SQL statement.  ^A statement is judged to be
** complete if it ends with a semicolon token and is not a prefix of a
** well-formed CREATE TRIGGER statement.  ^Semicolons that are embedded within
** string literals or quoted identifier names or comments are not
** independent tokens (they are part of the token in which they are
** embedded) and thus do not count as a statement terminator.  ^Whitespace
** and comments that follow the final semicolon are ignored.
**
** ^These routines return 0 if the statement is incomplete.  ^If a
** memory allocation fails, then CORTEX_NOMEM is returned.
**
** ^These routines do not parse the SQL statements and thus
** will not detect syntactically incorrect SQL.
**
** ^(If CORTEX has not been initialized using [cortex_initialize()] prior
** to invoking cortex_complete16() then cortex_initialize() is invoked
** automatically by cortex_complete16().  If that initialization fails,
** then the return value from cortex_complete16() will be non-zero
** regardless of whether or not the input SQL is complete.)^
**
** The input to [cortex_complete()] must be a zero-terminated
** UTF-8 string.
**
** The input to [cortex_complete16()] must be a zero-terminated
** UTF-16 string in native byte order.
*/
CORTEX_API int cortex_complete(const char *sql);
CORTEX_API int cortex_complete16(const void *sql);

/*
** CAPI3REF: Register A Callback To Handle CORTEX_BUSY Errors
** KEYWORDS: {busy-handler callback} {busy handler}
** METHOD: cortex
**
** ^The cortex_busy_handler(D,X,P) routine sets a callback function X
** that might be invoked with argument P whenever
** an attempt is made to access a database table associated with
** [database connection] D when another thread
** or process has the table locked.
** The cortex_busy_handler() interface is used to implement
** [cortex_busy_timeout()] and [PRAGMA busy_timeout].
**
** ^If the busy callback is NULL, then [CORTEX_BUSY]
** is returned immediately upon encountering the lock.  ^If the busy callback
** is not NULL, then the callback might be invoked with two arguments.
**
** ^The first argument to the busy handler is a copy of the void* pointer which
** is the third argument to cortex_busy_handler().  ^The second argument to
** the busy handler callback is the number of times that the busy handler has
** been invoked previously for the same locking event.  ^If the
** busy callback returns 0, then no additional attempts are made to
** access the database and [CORTEX_BUSY] is returned
** to the application.
** ^If the callback returns non-zero, then another attempt
** is made to access the database and the cycle repeats.
**
** The presence of a busy handler does not guarantee that it will be invoked
** when there is lock contention. ^If CORTEX determines that invoking the busy
** handler could result in a deadlock, it will go ahead and return [CORTEX_BUSY]
** to the application instead of invoking the
** busy handler.
** Consider a scenario where one process is holding a read lock that
** it is trying to promote to a reserved lock and
** a second process is holding a reserved lock that it is trying
** to promote to an exclusive lock.  The first process cannot proceed
** because it is blocked by the second and the second process cannot
** proceed because it is blocked by the first.  If both processes
** invoke the busy handlers, neither will make any progress.  Therefore,
** CORTEX returns [CORTEX_BUSY] for the first process, hoping that this
** will induce the first process to release its read lock and allow
** the second process to proceed.
**
** ^The default busy callback is NULL.
**
** ^(There can only be a single busy handler defined for each
** [database connection].  Setting a new busy handler clears any
** previously set handler.)^  ^Note that calling [cortex_busy_timeout()]
** or evaluating [PRAGMA busy_timeout=N] will change the
** busy handler and thus clear any previously set busy handler.
**
** The busy callback should not take any actions which modify the
** database connection that invoked the busy handler.  In other words,
** the busy handler is not reentrant.  Any such actions
** result in undefined behavior.
**
** A busy handler must not close the database connection
** or [prepared statement] that invoked the busy handler.
*/
CORTEX_API int cortex_busy_handler(cortex*,int(*)(void*,int),void*);

/*
** CAPI3REF: Set A Busy Timeout
** METHOD: cortex
**
** ^This routine sets a [cortex_busy_handler | busy handler] that sleeps
** for a specified amount of time when a table is locked.  ^The handler
** will sleep multiple times until at least "ms" milliseconds of sleeping
** have accumulated.  ^After at least "ms" milliseconds of sleeping,
** the handler returns 0 which causes [cortex_step()] to return
** [CORTEX_BUSY].
**
** ^Calling this routine with an argument less than or equal to zero
** turns off all busy handlers.
**
** ^(There can only be a single busy handler for a particular
** [database connection] at any given moment.  If another busy handler
** was defined  (using [cortex_busy_handler()]) prior to calling
** this routine, that other busy handler is cleared.)^
**
** See also:  [PRAGMA busy_timeout]
*/
CORTEX_API int cortex_busy_timeout(cortex*, int ms);

/*
** CAPI3REF: Set the Setlk Timeout
** METHOD: cortex
**
** This routine is only useful in CORTEX_ENABLE_SETLK_TIMEOUT builds. If
** the VFS supports blocking locks, it sets the timeout in ms used by
** eligible locks taken on wal mode databases by the specified database
** handle. In non-CORTEX_ENABLE_SETLK_TIMEOUT builds, or if the VFS does
** not support blocking locks, this function is a no-op.
**
** Passing 0 to this function disables blocking locks altogether. Passing
** -1 to this function requests that the VFS blocks for a long time -
** indefinitely if possible. The results of passing any other negative value
** are undefined.
**
** Internally, each CORTEX database handle stores two timeout values - the
** busy-timeout (used for rollback mode databases, or if the VFS does not
** support blocking locks) and the setlk-timeout (used for blocking locks
** on wal-mode databases). The cortex_busy_timeout() method sets both
** values, this function sets only the setlk-timeout value. Therefore,
** to configure separate busy-timeout and setlk-timeout values for a single
** database handle, call cortex_busy_timeout() followed by this function.
**
** Whenever the number of connections to a wal mode database falls from
** 1 to 0, the last connection takes an exclusive lock on the database,
** then checkpoints and deletes the wal file. While it is doing this, any
** new connection that tries to read from the database fails with an
** CORTEX_BUSY error. Or, if the CORTEX_SETLK_BLOCK_ON_CONNECT flag is
** passed to this API, the new connection blocks until the exclusive lock
** has been released.
*/
CORTEX_API int cortex_setlk_timeout(cortex*, int ms, int flags);

/*
** CAPI3REF: Flags for cortex_setlk_timeout()
*/
#define CORTEX_SETLK_BLOCK_ON_CONNECT 0x01

/*
** CAPI3REF: Convenience Routines For Running Queries
** METHOD: cortex
**
** This is a legacy interface that is preserved for backwards compatibility.
** Use of this interface is not recommended.
**
** Definition: A <b>result table</b> is a memory data structure created by the
** [cortex_get_table()] interface.  A result table records the
** complete query results from one or more queries.
**
** The table conceptually has a number of rows and columns.  But
** these numbers are not part of the result table itself.  These
** numbers are obtained separately.  Let N be the number of rows
** and M be the number of columns.
**
** A result table is an array of pointers to zero-terminated UTF-8 strings.
** There are (N+1)*M elements in the array.  The first M pointers point
** to zero-terminated strings that  contain the names of the columns.
** The remaining entries all point to query results.  NULL values result
** in NULL pointers.  All other values are in their UTF-8 zero-terminated
** string representation as returned by [cortex_column_text()].
**
** A result table might consist of one or more memory allocations.
** It is not safe to pass a result table directly to [cortex_free()].
** A result table should be deallocated using [cortex_free_table()].
**
** ^(As an example of the result table format, suppose a query result
** is as follows:
**
** <blockquote><pre>
**        Name        | Age
**        -----------------------
**        Alice       | 43
**        Bob         | 28
**        Cindy       | 21
** </pre></blockquote>
**
** There are two columns (M==2) and three rows (N==3).  Thus the
** result table has 8 entries.  Suppose the result table is stored
** in an array named azResult.  Then azResult holds this content:
**
** <blockquote><pre>
**        azResult&#91;0] = "Name";
**        azResult&#91;1] = "Age";
**        azResult&#91;2] = "Alice";
**        azResult&#91;3] = "43";
**        azResult&#91;4] = "Bob";
**        azResult&#91;5] = "28";
**        azResult&#91;6] = "Cindy";
**        azResult&#91;7] = "21";
** </pre></blockquote>)^
**
** ^The cortex_get_table() function evaluates one or more
** semicolon-separated SQL statements in the zero-terminated UTF-8
** string of its 2nd parameter and returns a result table to the
** pointer given in its 3rd parameter.
**
** After the application has finished with the result from cortex_get_table(),
** it must pass the result table pointer to cortex_free_table() in order to
** release the memory that was malloced.  Because of the way the
** [cortex_malloc()] happens within cortex_get_table(), the calling
** function must not try to call [cortex_free()] directly.  Only
** [cortex_free_table()] is able to release the memory properly and safely.
**
** The cortex_get_table() interface is implemented as a wrapper around
** [cortex_exec()].  The cortex_get_table() routine does not have access
** to any internal data structures of SQLite.  It uses only the public
** interface defined here.  As a consequence, errors that occur in the
** wrapper layer outside of the internal [cortex_exec()] call are not
** reflected in subsequent calls to [cortex_errcode()] or
** [cortex_errmsg()].
*/
CORTEX_API int cortex_get_table(
  cortex *db,          /* An open database */
  const char *zSql,     /* SQL to be evaluated */
  char ***pazResult,    /* Results of the query */
  int *pnRow,           /* Number of result rows written here */
  int *pnColumn,        /* Number of result columns written here */
  char **pzErrmsg       /* Error msg written here */
);
CORTEX_API void cortex_free_table(char **result);

/*
** CAPI3REF: Formatted String Printing Functions
**
** These routines are work-alikes of the "printf()" family of functions
** from the standard C library.
** These routines understand most of the common formatting options from
** the standard library printf()
** plus some additional non-standard formats ([%q], [%Q], [%w], and [%z]).
** See the [built-in printf()] documentation for details.
**
** ^The cortex_mprintf() and cortex_vmprintf() routines write their
** results into memory obtained from [cortex_malloc64()].
** The strings returned by these two routines should be
** released by [cortex_free()].  ^Both routines return a
** NULL pointer if [cortex_malloc64()] is unable to allocate enough
** memory to hold the resulting string.
**
** ^(The cortex_snprintf() routine is similar to "snprintf()" from
** the standard C library.  The result is written into the
** buffer supplied as the second parameter whose size is given by
** the first parameter. Note that the order of the
** first two parameters is reversed from snprintf().)^  This is an
** historical accident that cannot be fixed without breaking
** backwards compatibility.  ^(Note also that cortex_snprintf()
** returns a pointer to its buffer instead of the number of
** characters actually written into the buffer.)^  We admit that
** the number of characters written would be a more useful return
** value but we cannot change the implementation of cortex_snprintf()
** now without breaking compatibility.
**
** ^As long as the buffer size is greater than zero, cortex_snprintf()
** guarantees that the buffer is always zero-terminated.  ^The first
** parameter "n" is the total size of the buffer, including space for
** the zero terminator.  So the longest string that can be completely
** written will be n-1 characters.
**
** ^The cortex_vsnprintf() routine is a varargs version of cortex_snprintf().
**
** See also:  [built-in printf()], [printf() SQL function]
*/
CORTEX_API char *cortex_mprintf(const char*,...);
CORTEX_API char *cortex_vmprintf(const char*, va_list);
CORTEX_API char *cortex_snprintf(int,char*,const char*, ...);
CORTEX_API char *cortex_vsnprintf(int,char*,const char*, va_list);

/*
** CAPI3REF: Memory Allocation Subsystem
**
** The CORTEX core uses these three routines for all of its own
** internal memory allocation needs. "Core" in the previous sentence
** does not include operating-system specific [VFS] implementation.  The
** Windows VFS uses native malloc() and free() for some operations.
**
** ^The cortex_malloc() routine returns a pointer to a block
** of memory at least N bytes in length, where N is the parameter.
** ^If cortex_malloc() is unable to obtain sufficient free
** memory, it returns a NULL pointer.  ^If the parameter N to
** cortex_malloc() is zero or negative then cortex_malloc() returns
** a NULL pointer.
**
** ^The cortex_malloc64(N) routine works just like
** cortex_malloc(N) except that N is an unsigned 64-bit integer instead
** of a signed 32-bit integer.
**
** ^Calling cortex_free() with a pointer previously returned
** by cortex_malloc() or cortex_realloc() releases that memory so
** that it might be reused.  ^The cortex_free() routine is
** a no-op if it is called with a NULL pointer.  Passing a NULL pointer
** to cortex_free() is harmless.  After being freed, memory
** should neither be read nor written.  Even reading previously freed
** memory might result in a segmentation fault or other severe error.
** Memory corruption, a segmentation fault, or other severe error
** might result if cortex_free() is called with a non-NULL pointer that
** was not obtained from cortex_malloc() or cortex_realloc().
**
** ^The cortex_realloc(X,N) interface attempts to resize a
** prior memory allocation X to be at least N bytes.
** ^If the X parameter to cortex_realloc(X,N)
** is a NULL pointer then its behavior is identical to calling
** cortex_malloc(N).
** ^If the N parameter to cortex_realloc(X,N) is zero or
** negative then the behavior is exactly the same as calling
** cortex_free(X).
** ^cortex_realloc(X,N) returns a pointer to a memory allocation
** of at least N bytes in size or NULL if insufficient memory is available.
** ^If M is the size of the prior allocation, then min(N,M) bytes of the
** prior allocation are copied into the beginning of the buffer returned
** by cortex_realloc(X,N) and the prior allocation is freed.
** ^If cortex_realloc(X,N) returns NULL and N is positive, then the
** prior allocation is not freed.
**
** ^The cortex_realloc64(X,N) interface works the same as
** cortex_realloc(X,N) except that N is a 64-bit unsigned integer instead
** of a 32-bit signed integer.
**
** ^If X is a memory allocation previously obtained from cortex_malloc(),
** cortex_malloc64(), cortex_realloc(), or cortex_realloc64(), then
** cortex_msize(X) returns the size of that memory allocation in bytes.
** ^The value returned by cortex_msize(X) might be larger than the number
** of bytes requested when X was allocated.  ^If X is a NULL pointer then
** cortex_msize(X) returns zero.  If X points to something that is not
** the beginning of memory allocation, or if it points to a formerly
** valid memory allocation that has now been freed, then the behavior
** of cortex_msize(X) is undefined and possibly harmful.
**
** ^The memory returned by cortex_malloc(), cortex_realloc(),
** cortex_malloc64(), and cortex_realloc64()
** is always aligned to at least an 8 byte boundary, or to a
** 4 byte boundary if the [CORTEX_4_BYTE_ALIGNED_MALLOC] compile-time
** option is used.
**
** The pointer arguments to [cortex_free()] and [cortex_realloc()]
** must be either NULL or else pointers obtained from a prior
** invocation of [cortex_malloc()] or [cortex_realloc()] that have
** not yet been released.
**
** The application must not read or write any part of
** a block of memory after it has been released using
** [cortex_free()] or [cortex_realloc()].
*/
CORTEX_API void *cortex_malloc(int);
CORTEX_API void *cortex_malloc64(cortex_uint64);
CORTEX_API void *cortex_realloc(void*, int);
CORTEX_API void *cortex_realloc64(void*, cortex_uint64);
CORTEX_API void cortex_free(void*);
CORTEX_API cortex_uint64 cortex_msize(void*);

/*
** CAPI3REF: Memory Allocator Statistics
**
** CORTEX provides these two interfaces for reporting on the status
** of the [cortex_malloc()], [cortex_free()], and [cortex_realloc()]
** routines, which form the built-in memory allocation subsystem.
**
** ^The [cortex_memory_used()] routine returns the number of bytes
** of memory currently outstanding (malloced but not freed).
** ^The [cortex_memory_highwater()] routine returns the maximum
** value of [cortex_memory_used()] since the high-water mark
** was last reset.  ^The values returned by [cortex_memory_used()] and
** [cortex_memory_highwater()] include any overhead
** added by CORTEX in its implementation of [cortex_malloc()],
** but not overhead added by any underlying system library
** routines that [cortex_malloc()] may call.
**
** ^The memory high-water mark is reset to the current value of
** [cortex_memory_used()] if and only if the parameter to
** [cortex_memory_highwater()] is true.  ^The value returned
** by [cortex_memory_highwater(1)] is the high-water mark
** prior to the reset.
*/
CORTEX_API cortex_int64 cortex_memory_used(void);
CORTEX_API cortex_int64 cortex_memory_highwater(int resetFlag);

/*
** CAPI3REF: Pseudo-Random Number Generator
**
** CORTEX contains a high-quality pseudo-random number generator (PRNG) used to
** select random [ROWID | ROWIDs] when inserting new records into a table that
** already uses the largest possible [ROWID].  The PRNG is also used for
** the built-in random() and randomblob() SQL functions.  This interface allows
** applications to access the same PRNG for other purposes.
**
** ^A call to this routine stores N bytes of randomness into buffer P.
** ^The P parameter can be a NULL pointer.
**
** ^If this routine has not been previously called or if the previous
** call had N less than one or a NULL pointer for P, then the PRNG is
** seeded using randomness obtained from the xRandomness method of
** the default [cortex_vfs] object.
** ^If the previous call to this routine had an N of 1 or more and a
** non-NULL P then the pseudo-randomness is generated
** internally and without recourse to the [cortex_vfs] xRandomness
** method.
*/
CORTEX_API void cortex_randomness(int N, void *P);

/*
** CAPI3REF: Compile-Time Authorization Callbacks
** METHOD: cortex
** KEYWORDS: {authorizer callback}
**
** ^This routine registers an authorizer callback with a particular
** [database connection], supplied in the first argument.
** ^The authorizer callback is invoked as SQL statements are being compiled
** by [cortex_prepare()] or its variants [cortex_prepare_v2()],
** [cortex_prepare_v3()], [cortex_prepare16()], [cortex_prepare16_v2()],
** and [cortex_prepare16_v3()].  ^At various
** points during the compilation process, as logic is being created
** to perform various actions, the authorizer callback is invoked to
** see if those actions are allowed.  ^The authorizer callback should
** return [CORTEX_OK] to allow the action, [CORTEX_IGNORE] to disallow the
** specific action but allow the SQL statement to continue to be
** compiled, or [CORTEX_DENY] to cause the entire SQL statement to be
** rejected with an error.  ^If the authorizer callback returns
** any value other than [CORTEX_IGNORE], [CORTEX_OK], or [CORTEX_DENY]
** then the [cortex_prepare_v2()] or equivalent call that triggered
** the authorizer will fail with an error message.
**
** When the callback returns [CORTEX_OK], that means the operation
** requested is ok.  ^When the callback returns [CORTEX_DENY], the
** [cortex_prepare_v2()] or equivalent call that triggered the
** authorizer will fail with an error message explaining that
** access is denied.
**
** ^The first parameter to the authorizer callback is a copy of the third
** parameter to the cortex_set_authorizer() interface. ^The second parameter
** to the callback is an integer [CORTEX_COPY | action code] that specifies
** the particular action to be authorized. ^The third through sixth parameters
** to the callback are either NULL pointers or zero-terminated strings
** that contain additional details about the action to be authorized.
** Applications must always be prepared to encounter a NULL pointer in any
** of the third through the sixth parameters of the authorization callback.
**
** ^If the action code is [CORTEX_READ]
** and the callback returns [CORTEX_IGNORE] then the
** [prepared statement] statement is constructed to substitute
** a NULL value in place of the table column that would have
** been read if [CORTEX_OK] had been returned.  The [CORTEX_IGNORE]
** return can be used to deny an untrusted user access to individual
** columns of a table.
** ^When a table is referenced by a [SELECT] but no column values are
** extracted from that table (for example in a query like
** "SELECT count(*) FROM tab") then the [CORTEX_READ] authorizer callback
** is invoked once for that table with a column name that is an empty string.
** ^If the action code is [CORTEX_DELETE] and the callback returns
** [CORTEX_IGNORE] then the [DELETE] operation proceeds but the
** [truncate optimization] is disabled and all rows are deleted individually.
**
** An authorizer is used when [cortex_prepare | preparing]
** SQL statements from an untrusted source, to ensure that the SQL statements
** do not try to access data they are not allowed to see, or that they do not
** try to execute malicious statements that damage the database.  For
** example, an application may allow a user to enter arbitrary
** SQL queries for evaluation by a database.  But the application does
** not want the user to be able to make arbitrary changes to the
** database.  An authorizer could then be put in place while the
** user-entered SQL is being [cortex_prepare | prepared] that
** disallows everything except [SELECT] statements.
**
** Applications that need to process SQL from untrusted sources
** might also consider lowering resource limits using [cortex_limit()]
** and limiting database size using the [max_page_count] [PRAGMA]
** in addition to using an authorizer.
**
** ^(Only a single authorizer can be in place on a database connection
** at a time.  Each call to cortex_set_authorizer overrides the
** previous call.)^  ^Disable the authorizer by installing a NULL callback.
** The authorizer is disabled by default.
**
** The authorizer callback must not do anything that will modify
** the database connection that invoked the authorizer callback.
** Note that [cortex_prepare_v2()] and [cortex_step()] both modify their
** database connections for the meaning of "modify" in this paragraph.
**
** ^When [cortex_prepare_v2()] is used to prepare a statement, the
** statement might be re-prepared during [cortex_step()] due to a
** schema change.  Hence, the application should ensure that the
** correct authorizer callback remains in place during the [cortex_step()].
**
** ^Note that the authorizer callback is invoked only during
** [cortex_prepare()] or its variants.  Authorization is not
** performed during statement evaluation in [cortex_step()], unless
** as stated in the previous paragraph, cortex_step() invokes
** cortex_prepare_v2() to reprepare a statement after a schema change.
*/
CORTEX_API int cortex_set_authorizer(
  cortex*,
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*),
  void *pUserData
);

/*
** CAPI3REF: Authorizer Return Codes
**
** The [cortex_set_authorizer | authorizer callback function] must
** return either [CORTEX_OK] or one of these two constants in order
** to signal CORTEX whether or not the action is permitted.  See the
** [cortex_set_authorizer | authorizer documentation] for additional
** information.
**
** Note that CORTEX_IGNORE is also used as a [conflict resolution mode]
** returned from the [cortex_vtab_on_conflict()] interface.
*/
#define CORTEX_DENY   1   /* Abort the SQL statement with an error */
#define CORTEX_IGNORE 2   /* Don't allow access, but don't generate an error */

/*
** CAPI3REF: Authorizer Action Codes
**
** The [cortex_set_authorizer()] interface registers a callback function
** that is invoked to authorize certain SQL statement actions.  The
** second parameter to the callback is an integer code that specifies
** what action is being authorized.  These are the integer action codes that
** the authorizer callback may be passed.
**
** These action code values signify what kind of operation is to be
** authorized.  The 3rd and 4th parameters to the authorization
** callback function will be parameters or NULL depending on which of these
** codes is used as the second parameter.  ^(The 5th parameter to the
** authorizer callback is the name of the database ("main", "temp",
** etc.) if applicable.)^  ^The 6th parameter to the authorizer callback
** is the name of the inner-most trigger or view that is responsible for
** the access attempt or NULL if this access attempt is directly from
** top-level SQL code.
*/
/******************************************* 3rd ************ 4th ***********/
#define CORTEX_CREATE_INDEX          1   /* Index Name      Table Name      */
#define CORTEX_CREATE_TABLE          2   /* Table Name      NULL            */
#define CORTEX_CREATE_TEMP_INDEX     3   /* Index Name      Table Name      */
#define CORTEX_CREATE_TEMP_TABLE     4   /* Table Name      NULL            */
#define CORTEX_CREATE_TEMP_TRIGGER   5   /* Trigger Name    Table Name      */
#define CORTEX_CREATE_TEMP_VIEW      6   /* View Name       NULL            */
#define CORTEX_CREATE_TRIGGER        7   /* Trigger Name    Table Name      */
#define CORTEX_CREATE_VIEW           8   /* View Name       NULL            */
#define CORTEX_DELETE                9   /* Table Name      NULL            */
#define CORTEX_DROP_INDEX           10   /* Index Name      Table Name      */
#define CORTEX_DROP_TABLE           11   /* Table Name      NULL            */
#define CORTEX_DROP_TEMP_INDEX      12   /* Index Name      Table Name      */
#define CORTEX_DROP_TEMP_TABLE      13   /* Table Name      NULL            */
#define CORTEX_DROP_TEMP_TRIGGER    14   /* Trigger Name    Table Name      */
#define CORTEX_DROP_TEMP_VIEW       15   /* View Name       NULL            */
#define CORTEX_DROP_TRIGGER         16   /* Trigger Name    Table Name      */
#define CORTEX_DROP_VIEW            17   /* View Name       NULL            */
#define CORTEX_INSERT               18   /* Table Name      NULL            */
#define CORTEX_PRAGMA               19   /* Pragma Name     1st arg or NULL */
#define CORTEX_READ                 20   /* Table Name      Column Name     */
#define CORTEX_SELECT               21   /* NULL            NULL            */
#define CORTEX_TRANSACTION          22   /* Operation       NULL            */
#define CORTEX_UPDATE               23   /* Table Name      Column Name     */
#define CORTEX_ATTACH               24   /* Filename        NULL            */
#define CORTEX_DETACH               25   /* Database Name   NULL            */
#define CORTEX_ALTER_TABLE          26   /* Database Name   Table Name      */
#define CORTEX_REINDEX              27   /* Index Name      NULL            */
#define CORTEX_ANALYZE              28   /* Table Name      NULL            */
#define CORTEX_CREATE_VTABLE        29   /* Table Name      Module Name     */
#define CORTEX_DROP_VTABLE          30   /* Table Name      Module Name     */
#define CORTEX_FUNCTION             31   /* NULL            Function Name   */
#define CORTEX_SAVEPOINT            32   /* Operation       Savepoint Name  */
#define CORTEX_COPY                  0   /* No longer used */
#define CORTEX_RECURSIVE            33   /* NULL            NULL            */

/*
** CAPI3REF: Deprecated Tracing And Profiling Functions
** DEPRECATED
**
** These routines are deprecated. Use the [cortex_trace_v2()] interface
** instead of the routines described here.
**
** These routines register callback functions that can be used for
** tracing and profiling the execution of SQL statements.
**
** ^The callback function registered by cortex_trace() is invoked at
** various times when an SQL statement is being run by [cortex_step()].
** ^The cortex_trace() callback is invoked with a UTF-8 rendering of the
** SQL statement text as the statement first begins executing.
** ^(Additional cortex_trace() callbacks might occur
** as each triggered subprogram is entered.  The callbacks for triggers
** contain a UTF-8 SQL comment that identifies the trigger.)^
**
** The [CORTEX_TRACE_SIZE_LIMIT] compile-time option can be used to limit
** the length of [bound parameter] expansion in the output of cortex_trace().
**
** ^The callback function registered by cortex_profile() is invoked
** as each SQL statement finishes.  ^The profile callback contains
** the original statement text and an estimate of wall-clock time
** of how long that statement took to run.  ^The profile callback
** time is in units of nanoseconds, however the current implementation
** is only capable of millisecond resolution so the six least significant
** digits in the time are meaningless.  Future versions of SQLite
** might provide greater resolution on the profiler callback.  Invoking
** either [cortex_trace()] or [cortex_trace_v2()] will cancel the
** profile callback.
*/
CORTEX_API CORTEX_DEPRECATED void *cortex_trace(cortex*,
   void(*xTrace)(void*,const char*), void*);
CORTEX_API CORTEX_DEPRECATED void *cortex_profile(cortex*,
   void(*xProfile)(void*,const char*,cortex_uint64), void*);

/*
** CAPI3REF: SQL Trace Event Codes
** KEYWORDS: CORTEX_TRACE
**
** These constants identify classes of events that can be monitored
** using the [cortex_trace_v2()] tracing logic.  The M argument
** to [cortex_trace_v2(D,M,X,P)] is an OR-ed combination of one or more of
** the following constants.  ^The first argument to the trace callback
** is one of the following constants.
**
** New tracing constants may be added in future releases.
**
** ^A trace callback has four arguments: xCallback(T,C,P,X).
** ^The T argument is one of the integer type codes above.
** ^The C argument is a copy of the context pointer passed in as the
** fourth argument to [cortex_trace_v2()].
** The P and X arguments are pointers whose meanings depend on T.
**
** <dl>
** [[CORTEX_TRACE_STMT]] <dt>CORTEX_TRACE_STMT</dt>
** <dd>^An CORTEX_TRACE_STMT callback is invoked when a prepared statement
** first begins running and possibly at other times during the
** execution of the prepared statement, such as at the start of each
** trigger subprogram. ^The P argument is a pointer to the
** [prepared statement]. ^The X argument is a pointer to a string which
** is the unexpanded SQL text of the prepared statement or an SQL comment
** that indicates the invocation of a trigger.  ^The callback can compute
** the same text that would have been returned by the legacy [cortex_trace()]
** interface by using the X argument when X begins with "--" and invoking
** [cortex_expanded_sql(P)] otherwise.
**
** [[CORTEX_TRACE_PROFILE]] <dt>CORTEX_TRACE_PROFILE</dt>
** <dd>^An CORTEX_TRACE_PROFILE callback provides approximately the same
** information as is provided by the [cortex_profile()] callback.
** ^The P argument is a pointer to the [prepared statement] and the
** X argument points to a 64-bit integer which is approximately
** the number of nanoseconds that the prepared statement took to run.
** ^The CORTEX_TRACE_PROFILE callback is invoked when the statement finishes.
**
** [[CORTEX_TRACE_ROW]] <dt>CORTEX_TRACE_ROW</dt>
** <dd>^An CORTEX_TRACE_ROW callback is invoked whenever a prepared
** statement generates a single row of result.
** ^The P argument is a pointer to the [prepared statement] and the
** X argument is unused.
**
** [[CORTEX_TRACE_CLOSE]] <dt>CORTEX_TRACE_CLOSE</dt>
** <dd>^An CORTEX_TRACE_CLOSE callback is invoked when a database
** connection closes.
** ^The P argument is a pointer to the [database connection] object
** and the X argument is unused.
** </dl>
*/
#define CORTEX_TRACE_STMT       0x01
#define CORTEX_TRACE_PROFILE    0x02
#define CORTEX_TRACE_ROW        0x04
#define CORTEX_TRACE_CLOSE      0x08

/*
** CAPI3REF: SQL Trace Hook
** METHOD: cortex
**
** ^The cortex_trace_v2(D,M,X,P) interface registers a trace callback
** function X against [database connection] D, using property mask M
** and context pointer P.  ^If the X callback is
** NULL or if the M mask is zero, then tracing is disabled.  The
** M argument should be the bitwise OR-ed combination of
** zero or more [CORTEX_TRACE] constants.
**
** ^Each call to either cortex_trace(D,X,P) or cortex_trace_v2(D,M,X,P)
** overrides (cancels) all prior calls to cortex_trace(D,X,P) or
** cortex_trace_v2(D,M,X,P) for the [database connection] D.  Each
** database connection may have at most one trace callback.
**
** ^The X callback is invoked whenever any of the events identified by
** mask M occur.  ^The integer return value from the callback is currently
** ignored, though this may change in future releases.  Callback
** implementations should return zero to ensure future compatibility.
**
** ^A trace callback is invoked with four arguments: callback(T,C,P,X).
** ^The T argument is one of the [CORTEX_TRACE]
** constants to indicate why the callback was invoked.
** ^The C argument is a copy of the context pointer.
** The P and X arguments are pointers whose meanings depend on T.
**
** The cortex_trace_v2() interface is intended to replace the legacy
** interfaces [cortex_trace()] and [cortex_profile()], both of which
** are deprecated.
*/
CORTEX_API int cortex_trace_v2(
  cortex*,
  unsigned uMask,
  int(*xCallback)(unsigned,void*,void*,void*),
  void *pCtx
);

/*
** CAPI3REF: Query Progress Callbacks
** METHOD: cortex
**
** ^The cortex_progress_handler(D,N,X,P) interface causes the callback
** function X to be invoked periodically during long running calls to
** [cortex_step()] and [cortex_prepare()] and similar for
** database connection D.  An example use for this
** interface is to keep a GUI updated during a large query.
**
** ^The parameter P is passed through as the only parameter to the
** callback function X.  ^The parameter N is the approximate number of
** [virtual machine instructions] that are evaluated between successive
** invocations of the callback X.  ^If N is less than one then the progress
** handler is disabled.
**
** ^Only a single progress handler may be defined at one time per
** [database connection]; setting a new progress handler cancels the
** old one.  ^Setting parameter X to NULL disables the progress handler.
** ^The progress handler is also disabled by setting N to a value less
** than 1.
**
** ^If the progress callback returns non-zero, the operation is
** interrupted.  This feature can be used to implement a
** "Cancel" button on a GUI progress dialog box.
**
** The progress handler callback must not do anything that will modify
** the database connection that invoked the progress handler.
** Note that [cortex_prepare_v2()] and [cortex_step()] both modify their
** database connections for the meaning of "modify" in this paragraph.
**
** The progress handler callback would originally only be invoked from the
** bytecode engine.  It still might be invoked during [cortex_prepare()]
** and similar because those routines might force a reparse of the schema
** which involves running the bytecode engine.  However, beginning with
** CORTEX version 3.41.0, the progress handler callback might also be
** invoked directly from [cortex_prepare()] while analyzing and generating
** code for complex queries.
*/
CORTEX_API void cortex_progress_handler(cortex*, int, int(*)(void*), void*);

/*
** CAPI3REF: Opening A New Database Connection
** CONSTRUCTOR: cortex
**
** ^These routines open an CORTEX database file as specified by the
** filename argument. ^The filename argument is interpreted as UTF-8 for
** cortex_open() and cortex_open_v2() and as UTF-16 in the native byte
** order for cortex_open16(). ^(A [database connection] handle is usually
** returned in *ppDb, even if an error occurs.  The only exception is that
** if CORTEX is unable to allocate memory to hold the [cortex] object,
** a NULL will be written into *ppDb instead of a pointer to the [cortex]
** object.)^ ^(If the database is opened (and/or created) successfully, then
** [CORTEX_OK] is returned.  Otherwise an [error code] is returned.)^ ^The
** [cortex_errmsg()] or [cortex_errmsg16()] routines can be used to obtain
** an English language description of the error following a failure of any
** of the cortex_open() routines.
**
** ^The default encoding will be UTF-8 for databases created using
** cortex_open() or cortex_open_v2().  ^The default encoding for databases
** created using cortex_open16() will be UTF-16 in the native byte order.
**
** Whether or not an error occurs when it is opened, resources
** associated with the [database connection] handle should be released by
** passing it to [cortex_close()] when it is no longer required.
**
** The cortex_open_v2() interface works like cortex_open()
** except that it accepts two additional parameters for additional control
** over the new database connection.  ^(The flags parameter to
** cortex_open_v2() must include, at a minimum, one of the following
** three flag combinations:)^
**
** <dl>
** ^(<dt>[CORTEX_OPEN_READONLY]</dt>
** <dd>The database is opened in read-only mode.  If the database does
** not already exist, an error is returned.</dd>)^
**
** ^(<dt>[CORTEX_OPEN_READWRITE]</dt>
** <dd>The database is opened for reading and writing if possible, or
** reading only if the file is write protected by the operating
** system.  In either case the database must already exist, otherwise
** an error is returned.  For historical reasons, if opening in
** read-write mode fails due to OS-level permissions, an attempt is
** made to open it in read-only mode. [cortex_db_readonly()] can be
** used to determine whether the database is actually
** read-write.</dd>)^
**
** ^(<dt>[CORTEX_OPEN_READWRITE] | [CORTEX_OPEN_CREATE]</dt>
** <dd>The database is opened for reading and writing, and is created if
** it does not already exist. This is the behavior that is always used for
** cortex_open() and cortex_open16().</dd>)^
** </dl>
**
** In addition to the required flags, the following optional flags are
** also supported:
**
** <dl>
** ^(<dt>[CORTEX_OPEN_URI]</dt>
** <dd>The filename can be interpreted as a URI if this flag is set.</dd>)^
**
** ^(<dt>[CORTEX_OPEN_MEMORY]</dt>
** <dd>The database will be opened as an in-memory database.  The database
** is named by the "filename" argument for the purposes of cache-sharing,
** if shared cache mode is enabled, but the "filename" is otherwise ignored.
** </dd>)^
**
** ^(<dt>[CORTEX_OPEN_NOMUTEX]</dt>
** <dd>The new database connection will use the "multi-thread"
** [threading mode].)^  This means that separate threads are allowed
** to use CORTEX at the same time, as long as each thread is using
** a different [database connection].
**
** ^(<dt>[CORTEX_OPEN_FULLMUTEX]</dt>
** <dd>The new database connection will use the "serialized"
** [threading mode].)^  This means the multiple threads can safely
** attempt to use the same database connection at the same time.
** (Mutexes will block any actual concurrency, but in this mode
** there is no harm in trying.)
**
** ^(<dt>[CORTEX_OPEN_SHAREDCACHE]</dt>
** <dd>The database is opened with [shared cache] enabled, overriding
** the default shared cache setting provided by
** [cortex_enable_shared_cache()].)^
** The [use of shared cache mode is discouraged] and hence shared cache
** capabilities may be omitted from many builds of SQLite.  In such cases,
** this option is a no-op.
**
** ^(<dt>[CORTEX_OPEN_PRIVATECACHE]</dt>
** <dd>The database is opened with [shared cache] disabled, overriding
** the default shared cache setting provided by
** [cortex_enable_shared_cache()].)^
**
** [[OPEN_EXRESCODE]] ^(<dt>[CORTEX_OPEN_EXRESCODE]</dt>
** <dd>The database connection comes up in "extended result code mode".
** In other words, the database behaves as if
** [cortex_extended_result_codes(db,1)] were called on the database
** connection as soon as the connection is created. In addition to setting
** the extended result code mode, this flag also causes [cortex_open_v2()]
** to return an extended result code.</dd>
**
** [[OPEN_NOFOLLOW]] ^(<dt>[CORTEX_OPEN_NOFOLLOW]</dt>
** <dd>The database filename is not allowed to contain a symbolic link</dd>
** </dl>)^
**
** If the 3rd parameter to cortex_open_v2() is not one of the
** required combinations shown above optionally combined with other
** [CORTEX_OPEN_READONLY | CORTEX_OPEN_* bits]
** then the behavior is undefined.  Historic versions of SQLite
** have silently ignored surplus bits in the flags parameter to
** cortex_open_v2(), however that behavior might not be carried through
** into future versions of CORTEX and so applications should not rely
** upon it.  Note in particular that the CORTEX_OPEN_EXCLUSIVE flag is a no-op
** for cortex_open_v2().  The CORTEX_OPEN_EXCLUSIVE does *not* cause
** the open to fail if the database already exists.  The CORTEX_OPEN_EXCLUSIVE
** flag is intended for use by the [cortex_vfs|VFS interface] only, and not
** by cortex_open_v2().
**
** ^The fourth parameter to cortex_open_v2() is the name of the
** [cortex_vfs] object that defines the operating system interface that
** the new database connection should use.  ^If the fourth parameter is
** a NULL pointer then the default [cortex_vfs] object is used.
**
** ^If the filename is ":memory:", then a private, temporary in-memory database
** is created for the connection.  ^This in-memory database will vanish when
** the database connection is closed.  Future versions of CORTEX might
** make use of additional special filenames that begin with the ":" character.
** It is recommended that when a database filename actually does begin with
** a ":" character you should prefix the filename with a pathname such as
** "./" to avoid ambiguity.
**
** ^If the filename is an empty string, then a private, temporary
** on-disk database will be created.  ^This private database will be
** automatically deleted as soon as the database connection is closed.
**
** [[URI filenames in cortex_open()]] <h3>URI Filenames</h3>
**
** ^If [URI filename] interpretation is enabled, and the filename argument
** begins with "file:", then the filename is interpreted as a URI. ^URI
** filename interpretation is enabled if the [CORTEX_OPEN_URI] flag is
** set in the third argument to cortex_open_v2(), or if it has
** been enabled globally using the [CORTEX_CONFIG_URI] option with the
** [cortex_config()] method or by the [CORTEX_USE_URI] compile-time option.
** URI filename interpretation is turned off
** by default, but future releases of CORTEX might enable URI filename
** interpretation by default.  See "[URI filenames]" for additional
** information.
**
** URI filenames are parsed according to RFC 3986. ^If the URI contains an
** authority, then it must be either an empty string or the string
** "localhost". ^If the authority is not an empty string or "localhost", an
** error is returned to the caller. ^The fragment component of a URI, if
** present, is ignored.
**
** ^SQLite uses the path component of the URI as the name of the disk file
** which contains the database. ^If the path begins with a '/' character,
** then it is interpreted as an absolute path. ^If the path does not begin
** with a '/' (meaning that the authority section is omitted from the URI)
** then the path is interpreted as a relative path.
** ^(On windows, the first component of an absolute path
** is a drive specification (e.g. "C:").)^
**
** [[core URI query parameters]]
** The query component of a URI may contain parameters that are interpreted
** either by CORTEX itself, or by a [VFS | custom VFS implementation].
** CORTEX and its built-in [VFSes] interpret the
** following query parameters:
**
** <ul>
**   <li> <b>vfs</b>: ^The "vfs" parameter may be used to specify the name of
**     a VFS object that provides the operating system interface that should
**     be used to access the database file on disk. ^If this option is set to
**     an empty string the default VFS object is used. ^Specifying an unknown
**     VFS is an error. ^If cortex_open_v2() is used and the vfs option is
**     present, then the VFS specified by the option takes precedence over
**     the value passed as the fourth parameter to cortex_open_v2().
**
**   <li> <b>mode</b>: ^(The mode parameter may be set to either "ro", "rw",
**     "rwc", or "memory". Attempting to set it to any other value is
**     an error)^.
**     ^If "ro" is specified, then the database is opened for read-only
**     access, just as if the [CORTEX_OPEN_READONLY] flag had been set in the
**     third argument to cortex_open_v2(). ^If the mode option is set to
**     "rw", then the database is opened for read-write (but not create)
**     access, as if CORTEX_OPEN_READWRITE (but not CORTEX_OPEN_CREATE) had
**     been set. ^Value "rwc" is equivalent to setting both
**     CORTEX_OPEN_READWRITE and CORTEX_OPEN_CREATE.  ^If the mode option is
**     set to "memory" then a pure [in-memory database] that never reads
**     or writes from disk is used. ^It is an error to specify a value for
**     the mode parameter that is less restrictive than that specified by
**     the flags passed in the third parameter to cortex_open_v2().
**
**   <li> <b>cache</b>: ^The cache parameter may be set to either "shared" or
**     "private". ^Setting it to "shared" is equivalent to setting the
**     CORTEX_OPEN_SHAREDCACHE bit in the flags argument passed to
**     cortex_open_v2(). ^Setting the cache parameter to "private" is
**     equivalent to setting the CORTEX_OPEN_PRIVATECACHE bit.
**     ^If cortex_open_v2() is used and the "cache" parameter is present in
**     a URI filename, its value overrides any behavior requested by setting
**     CORTEX_OPEN_PRIVATECACHE or CORTEX_OPEN_SHAREDCACHE flag.
**
**  <li> <b>psow</b>: ^The psow parameter indicates whether or not the
**     [powersafe overwrite] property does or does not apply to the
**     storage media on which the database file resides.
**
**  <li> <b>nolock</b>: ^The nolock parameter is a boolean query parameter
**     which if set disables file locking in rollback journal modes.  This
**     is useful for accessing a database on a filesystem that does not
**     support locking.  Caution:  Database corruption might result if two
**     or more processes write to the same database and any one of those
**     processes uses nolock=1.
**
**  <li> <b>immutable</b>: ^The immutable parameter is a boolean query
**     parameter that indicates that the database file is stored on
**     read-only media.  ^When immutable is set, CORTEX assumes that the
**     database file cannot be changed, even by a process with higher
**     privilege, and so the database is opened read-only and all locking
**     and change detection is disabled.  Caution: Setting the immutable
**     property on a database file that does in fact change can result
**     in incorrect query results and/or [CORTEX_CORRUPT] errors.
**     See also: [CORTEX_IOCAP_IMMUTABLE].
**
** </ul>
**
** ^Specifying an unknown parameter in the query component of a URI is not an
** error.  Future versions of CORTEX might understand additional query
** parameters.  See "[query parameters with special meaning to SQLite]" for
** additional information.
**
** [[URI filename examples]] <h3>URI filename examples</h3>
**
** <table border="1" align=center cellpadding=5>
** <tr><th> URI filenames <th> Results
** <tr><td> file:data.db <td>
**          Open the file "data.db" in the current directory.
** <tr><td> file:/home/fred/data.db<br>
**          file:///home/fred/data.db <br>
**          file://localhost/home/fred/data.db <br> <td>
**          Open the database file "/home/fred/data.db".
** <tr><td> file://darkstar/home/fred/data.db <td>
**          An error. "darkstar" is not a recognized authority.
** <tr><td style="white-space:nowrap">
**          file:///C:/Documents%20and%20Settings/fred/Desktop/data.db
**     <td> Windows only: Open the file "data.db" on fred's desktop on drive
**          C:. Note that the %20 escaping in this example is not strictly
**          necessary - space characters can be used literally
**          in URI filenames.
** <tr><td> file:data.db?mode=ro&cache=private <td>
**          Open file "data.db" in the current directory for read-only access.
**          Regardless of whether or not shared-cache mode is enabled by
**          default, use a private cache.
** <tr><td> file:/home/fred/data.db?vfs=unix-dotfile <td>
**          Open file "/home/fred/data.db". Use the special VFS "unix-dotfile"
**          that uses dot-files in place of posix advisory locking.
** <tr><td> file:data.db?mode=readonly <td>
**          An error. "readonly" is not a valid option for the "mode" parameter.
**          Use "ro" instead:  "file:data.db?mode=ro".
** </table>
**
** ^URI hexadecimal escape sequences (%HH) are supported within the path and
** query components of a URI. A hexadecimal escape sequence consists of a
** percent sign - "%" - followed by exactly two hexadecimal digits
** specifying an octet value. ^Before the path or query components of a
** URI filename are interpreted, they are encoded using UTF-8 and all
** hexadecimal escape sequences replaced by a single byte containing the
** corresponding octet. If this process generates an invalid UTF-8 encoding,
** the results are undefined.
**
** <b>Note to Windows users:</b>  The encoding used for the filename argument
** of cortex_open() and cortex_open_v2() must be UTF-8, not whatever
** codepage is currently defined.  Filenames containing international
** characters must be converted to UTF-8 prior to passing them into
** cortex_open() or cortex_open_v2().
**
** <b>Note to Windows Runtime users:</b>  The temporary directory must be set
** prior to calling cortex_open() or cortex_open_v2().  Otherwise, various
** features that require the use of temporary files may fail.
**
** See also: [cortex_temp_directory]
*/
CORTEX_API int cortex_open(
  const char *filename,   /* Database filename (UTF-8) */
  cortex **ppDb          /* OUT: CORTEX db handle */
);
CORTEX_API int cortex_open16(
  const void *filename,   /* Database filename (UTF-16) */
  cortex **ppDb          /* OUT: CORTEX db handle */
);
CORTEX_API int cortex_open_v2(
  const char *filename,   /* Database filename (UTF-8) */
  cortex **ppDb,         /* OUT: CORTEX db handle */
  int flags,              /* Flags */
  const char *zVfs        /* Name of VFS module to use */
);

/*
** CAPI3REF: Obtain Values For URI Parameters
**
** These are utility routines, useful to [VFS|custom VFS implementations],
** that check if a database file was a URI that contained a specific query
** parameter, and if so obtains the value of that query parameter.
**
** The first parameter to these interfaces (hereafter referred to
** as F) must be one of:
** <ul>
** <li> A database filename pointer created by the CORTEX core and
** passed into the xOpen() method of a VFS implementation, or
** <li> A filename obtained from [cortex_db_filename()], or
** <li> A new filename constructed using [cortex_create_filename()].
** </ul>
** If the F parameter is not one of the above, then the behavior is
** undefined and probably undesirable.  Older versions of CORTEX were
** more tolerant of invalid F parameters than newer versions.
**
** If F is a suitable filename (as described in the previous paragraph)
** and if P is the name of the query parameter, then
** cortex_uri_parameter(F,P) returns the value of the P
** parameter if it exists or a NULL pointer if P does not appear as a
** query parameter on F.  If P is a query parameter of F and it
** has no explicit value, then cortex_uri_parameter(F,P) returns
** a pointer to an empty string.
**
** The cortex_uri_boolean(F,P,B) routine assumes that P is a boolean
** parameter and returns true (1) or false (0) according to the value
** of P.  The cortex_uri_boolean(F,P,B) routine returns true (1) if the
** value of query parameter P is one of "yes", "true", or "on" in any
** case or if the value begins with a non-zero number.  The
** cortex_uri_boolean(F,P,B) routines returns false (0) if the value of
** query parameter P is one of "no", "false", or "off" in any case or
** if the value begins with a numeric zero.  If P is not a query
** parameter on F or if the value of P does not match any of the
** above, then cortex_uri_boolean(F,P,B) returns (B!=0).
**
** The cortex_uri_int64(F,P,D) routine converts the value of P into a
** 64-bit signed integer and returns that integer, or D if P does not
** exist.  If the value of P is something other than an integer, then
** zero is returned.
**
** The cortex_uri_key(F,N) returns a pointer to the name (not
** the value) of the N-th query parameter for filename F, or a NULL
** pointer if N is less than zero or greater than the number of query
** parameters minus 1.  The N value is zero-based so N should be 0 to obtain
** the name of the first query parameter, 1 for the second parameter, and
** so forth.
**
** If F is a NULL pointer, then cortex_uri_parameter(F,P) returns NULL and
** cortex_uri_boolean(F,P,B) returns B.  If F is not a NULL pointer and
** is not a database file pathname pointer that the CORTEX core passed
** into the xOpen VFS method, then the behavior of this routine is undefined
** and probably undesirable.
**
** Beginning with CORTEX [version 3.31.0] ([dateof:3.31.0]) the input F
** parameter can also be the name of a rollback journal file or WAL file
** in addition to the main database file.  Prior to version 3.31.0, these
** routines would only work if F was the name of the main database file.
** When the F parameter is the name of the rollback journal or WAL file,
** it has access to all the same query parameters as were found on the
** main database file.
**
** See the [URI filename] documentation for additional information.
*/
CORTEX_API const char *cortex_uri_parameter(cortex_filename z, const char *zParam);
CORTEX_API int cortex_uri_boolean(cortex_filename z, const char *zParam, int bDefault);
CORTEX_API cortex_int64 cortex_uri_int64(cortex_filename, const char*, cortex_int64);
CORTEX_API const char *cortex_uri_key(cortex_filename z, int N);

/*
** CAPI3REF:  Translate filenames
**
** These routines are available to [VFS|custom VFS implementations] for
** translating filenames between the main database file, the journal file,
** and the WAL file.
**
** If F is the name of an CORTEX database file, journal file, or WAL file
** passed by the CORTEX core into the VFS, then cortex_filename_database(F)
** returns the name of the corresponding database file.
**
** If F is the name of an CORTEX database file, journal file, or WAL file
** passed by the CORTEX core into the VFS, or if F is a database filename
** obtained from [cortex_db_filename()], then cortex_filename_journal(F)
** returns the name of the corresponding rollback journal file.
**
** If F is the name of an CORTEX database file, journal file, or WAL file
** that was passed by the CORTEX core into the VFS, or if F is a database
** filename obtained from [cortex_db_filename()], then
** cortex_filename_wal(F) returns the name of the corresponding
** WAL file.
**
** In all of the above, if F is not the name of a database, journal or WAL
** filename passed into the VFS from the CORTEX core and F is not the
** return value from [cortex_db_filename()], then the result is
** undefined and is likely a memory access violation.
*/
CORTEX_API const char *cortex_filename_database(cortex_filename);
CORTEX_API const char *cortex_filename_journal(cortex_filename);
CORTEX_API const char *cortex_filename_wal(cortex_filename);

/*
** CAPI3REF:  Database File Corresponding To A Journal
**
** ^If X is the name of a rollback or WAL-mode journal file that is
** passed into the xOpen method of [cortex_vfs], then
** cortex_database_file_object(X) returns a pointer to the [cortex_file]
** object that represents the main database file.
**
** This routine is intended for use in custom [VFS] implementations
** only.  It is not a general-purpose interface.
** The argument cortex_file_object(X) must be a filename pointer that
** has been passed into [cortex_vfs].xOpen method where the
** flags parameter to xOpen contains one of the bits
** [CORTEX_OPEN_MAIN_JOURNAL] or [CORTEX_OPEN_WAL].  Any other use
** of this routine results in undefined and probably undesirable
** behavior.
*/
CORTEX_API cortex_file *cortex_database_file_object(const char*);

/*
** CAPI3REF: Create and Destroy VFS Filenames
**
** These interfaces are provided for use by [VFS shim] implementations and
** are not useful outside of that context.
**
** The cortex_create_filename(D,J,W,N,P) allocates memory to hold a version of
** database filename D with corresponding journal file J and WAL file W and
** an array P of N URI Key/Value pairs.  The result from
** cortex_create_filename(D,J,W,N,P) is a pointer to a database filename that
** is safe to pass to routines like:
** <ul>
** <li> [cortex_uri_parameter()],
** <li> [cortex_uri_boolean()],
** <li> [cortex_uri_int64()],
** <li> [cortex_uri_key()],
** <li> [cortex_filename_database()],
** <li> [cortex_filename_journal()], or
** <li> [cortex_filename_wal()].
** </ul>
** If a memory allocation error occurs, cortex_create_filename() might
** return a NULL pointer.  The memory obtained from cortex_create_filename(X)
** must be released by a corresponding call to cortex_free_filename(Y).
**
** The P parameter in cortex_create_filename(D,J,W,N,P) should be an array
** of 2*N pointers to strings.  Each pair of pointers in this array corresponds
** to a key and value for a query parameter.  The P parameter may be a NULL
** pointer if N is zero.  None of the 2*N pointers in the P array may be
** NULL pointers and key pointers should not be empty strings.
** None of the D, J, or W parameters to cortex_create_filename(D,J,W,N,P) may
** be NULL pointers, though they can be empty strings.
**
** The cortex_free_filename(Y) routine releases a memory allocation
** previously obtained from cortex_create_filename().  Invoking
** cortex_free_filename(Y) where Y is a NULL pointer is a harmless no-op.
**
** If the Y parameter to cortex_free_filename(Y) is anything other
** than a NULL pointer or a pointer previously acquired from
** cortex_create_filename(), then bad things such as heap
** corruption or segfaults may occur. The value Y should not be
** used again after cortex_free_filename(Y) has been called.  This means
** that if the [cortex_vfs.xOpen()] method of a VFS has been called using Y,
** then the corresponding [cortex_module.xClose() method should also be
** invoked prior to calling cortex_free_filename(Y).
*/
CORTEX_API cortex_filename cortex_create_filename(
  const char *zDatabase,
  const char *zJournal,
  const char *zWal,
  int nParam,
  const char **azParam
);
CORTEX_API void cortex_free_filename(cortex_filename);

/*
** CAPI3REF: Error Codes And Messages
** METHOD: cortex
**
** ^If the most recent cortex_* API call associated with
** [database connection] D failed, then the cortex_errcode(D) interface
** returns the numeric [result code] or [extended result code] for that
** API call.
** ^The cortex_extended_errcode()
** interface is the same except that it always returns the
** [extended result code] even when extended result codes are
** disabled.
**
** The values returned by cortex_errcode() and/or
** cortex_extended_errcode() might change with each API call.
** Except, there are some interfaces that are guaranteed to never
** change the value of the error code.  The error-code preserving
** interfaces include the following:
**
** <ul>
** <li> cortex_errcode()
** <li> cortex_extended_errcode()
** <li> cortex_errmsg()
** <li> cortex_errmsg16()
** <li> cortex_error_offset()
** </ul>
**
** ^The cortex_errmsg() and cortex_errmsg16() return English-language
** text that describes the error, as either UTF-8 or UTF-16 respectively,
** or NULL if no error message is available.
** (See how CORTEX handles [invalid UTF] for exceptions to this rule.)
** ^(Memory to hold the error message string is managed internally.
** The application does not need to worry about freeing the result.
** However, the error string might be overwritten or deallocated by
** subsequent calls to other CORTEX interface functions.)^
**
** ^The cortex_errstr(E) interface returns the English-language text
** that describes the [result code] E, as UTF-8, or NULL if E is not a
** result code for which a text error message is available.
** ^(Memory to hold the error message string is managed internally
** and must not be freed by the application)^.
**
** ^If the most recent error references a specific token in the input
** SQL, the cortex_error_offset() interface returns the byte offset
** of the start of that token.  ^The byte offset returned by
** cortex_error_offset() assumes that the input SQL is UTF-8.
** ^If the most recent error does not reference a specific token in the input
** SQL, then the cortex_error_offset() function returns -1.
**
** When the serialized [threading mode] is in use, it might be the
** case that a second error occurs on a separate thread in between
** the time of the first error and the call to these interfaces.
** When that happens, the second error will be reported since these
** interfaces always report the most recent result.  To avoid
** this, each thread can obtain exclusive use of the [database connection] D
** by invoking [cortex_mutex_enter]([cortex_db_mutex](D)) before beginning
** to use D and invoking [cortex_mutex_leave]([cortex_db_mutex](D)) after
** all calls to the interfaces listed here are completed.
**
** If an interface fails with CORTEX_MISUSE, that means the interface
** was invoked incorrectly by the application.  In that case, the
** error code and message may or may not be set.
*/
CORTEX_API int cortex_errcode(cortex *db);
CORTEX_API int cortex_extended_errcode(cortex *db);
CORTEX_API const char *cortex_errmsg(cortex*);
CORTEX_API const void *cortex_errmsg16(cortex*);
CORTEX_API const char *cortex_errstr(int);
CORTEX_API int cortex_error_offset(cortex *db);

/*
** CAPI3REF: Set Error Codes And Message
** METHOD: cortex
**
** Set the error code of the database handle passed as the first argument
** to errcode, and the error message to a copy of nul-terminated string
** zErrMsg. If zErrMsg is passed NULL, then the error message is set to
** the default message associated with the supplied error code.  Subsequent
** calls to [cortex_errcode()] and [cortex_errmsg()] and similar will
** return the values set by this routine in place of what was previously
** set by CORTEX itself.
**
** This function returns CORTEX_OK if the error code and error message are
** successfully set, CORTEX_NOMEM if an OOM occurs, and CORTEX_MISUSE if
** the database handle is NULL or invalid.
**
** The error code and message set by this routine remains in effect until
** they are changed, either by another call to this routine or until they are
** changed to by CORTEX itself to reflect the result of some subsquent
** API call.
**
** This function is intended for use by CORTEX extensions or wrappers.  The
** idea is that an extension or wrapper can use this routine to set error
** messages and error codes and thus behave more like a core SQLite
** feature from the point of view of an application.
*/
CORTEX_API int cortex_set_errmsg(cortex *db, int errcode, const char *zErrMsg);

/*
** CAPI3REF: Prepared Statement Object
** KEYWORDS: {prepared statement} {prepared statements}
**
** An instance of this object represents a single SQL statement that
** has been compiled into binary form and is ready to be evaluated.
**
** Think of each SQL statement as a separate computer program.  The
** original SQL text is source code.  A prepared statement object
** is the compiled object code.  All SQL must be converted into a
** prepared statement before it can be run.
**
** The life-cycle of a prepared statement object usually goes like this:
**
** <ol>
** <li> Create the prepared statement object using [cortex_prepare_v2()].
** <li> Bind values to [parameters] using the cortex_bind_*()
**      interfaces.
** <li> Run the SQL by calling [cortex_step()] one or more times.
** <li> Reset the prepared statement using [cortex_reset()] then go back
**      to step 2.  Do this zero or more times.
** <li> Destroy the object using [cortex_finalize()].
** </ol>
*/
typedef struct cortex_stmt cortex_stmt;

/*
** CAPI3REF: Run-time Limits
** METHOD: cortex
**
** ^(This interface allows the size of various constructs to be limited
** on a connection by connection basis.  The first parameter is the
** [database connection] whose limit is to be set or queried.  The
** second parameter is one of the [limit categories] that define a
** class of constructs to be size limited.  The third parameter is the
** new limit for that construct.)^
**
** ^If the new limit is a negative number, the limit is unchanged.
** ^(For each limit category CORTEX_LIMIT_<i>NAME</i> there is a
** [limits | hard upper bound]
** set at compile-time by a C preprocessor macro called
** [limits | CORTEX_MAX_<i>NAME</i>].
** (The "_LIMIT_" in the name is changed to "_MAX_".))^
** ^Attempts to increase a limit above its hard upper bound are
** silently truncated to the hard upper bound.
**
** ^Regardless of whether or not the limit was changed, the
** [cortex_limit()] interface returns the prior value of the limit.
** ^Hence, to find the current value of a limit without changing it,
** simply invoke this interface with the third parameter set to -1.
**
** Run-time limits are intended for use in applications that manage
** both their own internal database and also databases that are controlled
** by untrusted external sources.  An example application might be a
** web browser that has its own databases for storing history and
** separate databases controlled by JavaScript applications downloaded
** off the Internet.  The internal databases can be given the
** large, default limits.  Databases managed by external sources can
** be given much smaller limits designed to prevent a denial of service
** attack.  Developers might also want to use the [cortex_set_authorizer()]
** interface to further control untrusted SQL.  The size of the database
** created by an untrusted script can be contained using the
** [max_page_count] [PRAGMA].
**
** New run-time limit categories may be added in future releases.
*/
CORTEX_API int cortex_limit(cortex*, int id, int newVal);

/*
** CAPI3REF: Run-Time Limit Categories
** KEYWORDS: {limit category} {*limit categories}
**
** These constants define various performance limits
** that can be lowered at run-time using [cortex_limit()].
** A concise description of these limits follows, and additional information
** is available at [limits | Limits in SQLite].
**
** <dl>
** [[CORTEX_LIMIT_LENGTH]] ^(<dt>CORTEX_LIMIT_LENGTH</dt>
** <dd>The maximum size of any string or BLOB or table row, in bytes.<dd>)^
**
** [[CORTEX_LIMIT_SQL_LENGTH]] ^(<dt>CORTEX_LIMIT_SQL_LENGTH</dt>
** <dd>The maximum length of an SQL statement, in bytes.</dd>)^
**
** [[CORTEX_LIMIT_COLUMN]] ^(<dt>CORTEX_LIMIT_COLUMN</dt>
** <dd>The maximum number of columns in a table definition or in the
** result set of a [SELECT] or the maximum number of columns in an index
** or in an ORDER BY or GROUP BY clause.</dd>)^
**
** [[CORTEX_LIMIT_EXPR_DEPTH]] ^(<dt>CORTEX_LIMIT_EXPR_DEPTH</dt>
** <dd>The maximum depth of the parse tree on any expression.</dd>)^
**
** [[CORTEX_LIMIT_COMPOUND_SELECT]] ^(<dt>CORTEX_LIMIT_COMPOUND_SELECT</dt>
** <dd>The maximum number of terms in a compound SELECT statement.</dd>)^
**
** [[CORTEX_LIMIT_VDBE_OP]] ^(<dt>CORTEX_LIMIT_VDBE_OP</dt>
** <dd>The maximum number of instructions in a virtual machine program
** used to implement an SQL statement.  If [cortex_prepare_v2()] or
** the equivalent tries to allocate space for more than this many opcodes
** in a single prepared statement, an CORTEX_NOMEM error is returned.</dd>)^
**
** [[CORTEX_LIMIT_FUNCTION_ARG]] ^(<dt>CORTEX_LIMIT_FUNCTION_ARG</dt>
** <dd>The maximum number of arguments on a function.</dd>)^
**
** [[CORTEX_LIMIT_ATTACHED]] ^(<dt>CORTEX_LIMIT_ATTACHED</dt>
** <dd>The maximum number of [ATTACH | attached databases].)^</dd>
**
** [[CORTEX_LIMIT_LIKE_PATTERN_LENGTH]]
** ^(<dt>CORTEX_LIMIT_LIKE_PATTERN_LENGTH</dt>
** <dd>The maximum length of the pattern argument to the [LIKE] or
** [GLOB] operators.</dd>)^
**
** [[CORTEX_LIMIT_VARIABLE_NUMBER]]
** ^(<dt>CORTEX_LIMIT_VARIABLE_NUMBER</dt>
** <dd>The maximum index number of any [parameter] in an SQL statement.)^
**
** [[CORTEX_LIMIT_TRIGGER_DEPTH]] ^(<dt>CORTEX_LIMIT_TRIGGER_DEPTH</dt>
** <dd>The maximum depth of recursion for triggers.</dd>)^
**
** [[CORTEX_LIMIT_WORKER_THREADS]] ^(<dt>CORTEX_LIMIT_WORKER_THREADS</dt>
** <dd>The maximum number of auxiliary worker threads that a single
** [prepared statement] may start.</dd>)^
** </dl>
*/
#define CORTEX_LIMIT_LENGTH                    0
#define CORTEX_LIMIT_SQL_LENGTH                1
#define CORTEX_LIMIT_COLUMN                    2
#define CORTEX_LIMIT_EXPR_DEPTH                3
#define CORTEX_LIMIT_COMPOUND_SELECT           4
#define CORTEX_LIMIT_VDBE_OP                   5
#define CORTEX_LIMIT_FUNCTION_ARG              6
#define CORTEX_LIMIT_ATTACHED                  7
#define CORTEX_LIMIT_LIKE_PATTERN_LENGTH       8
#define CORTEX_LIMIT_VARIABLE_NUMBER           9
#define CORTEX_LIMIT_TRIGGER_DEPTH            10
#define CORTEX_LIMIT_WORKER_THREADS           11

/*
** CAPI3REF: Prepare Flags
**
** These constants define various flags that can be passed into the
** "prepFlags" parameter of the [cortex_prepare_v3()] and
** [cortex_prepare16_v3()] interfaces.
**
** New flags may be added in future releases of SQLite.
**
** <dl>
** [[CORTEX_PREPARE_PERSISTENT]] ^(<dt>CORTEX_PREPARE_PERSISTENT</dt>
** <dd>The CORTEX_PREPARE_PERSISTENT flag is a hint to the query planner
** that the prepared statement will be retained for a long time and
** probably reused many times.)^ ^Without this flag, [cortex_prepare_v3()]
** and [cortex_prepare16_v3()] assume that the prepared statement will
** be used just once or at most a few times and then destroyed using
** [cortex_finalize()] relatively soon. The current implementation acts
** on this hint by avoiding the use of [lookaside memory] so as not to
** deplete the limited store of lookaside memory. Future versions of
** CORTEX may act on this hint differently.
**
** [[CORTEX_PREPARE_NORMALIZE]] <dt>CORTEX_PREPARE_NORMALIZE</dt>
** <dd>The CORTEX_PREPARE_NORMALIZE flag is a no-op. This flag used
** to be required for any prepared statement that wanted to use the
** [cortex_normalized_sql()] interface.  However, the
** [cortex_normalized_sql()] interface is now available to all
** prepared statements, regardless of whether or not they use this
** flag.
**
** [[CORTEX_PREPARE_NO_VTAB]] <dt>CORTEX_PREPARE_NO_VTAB</dt>
** <dd>The CORTEX_PREPARE_NO_VTAB flag causes the SQL compiler
** to return an error (error code CORTEX_ERROR) if the statement uses
** any virtual tables.
**
** [[CORTEX_PREPARE_DONT_LOG]] <dt>CORTEX_PREPARE_DONT_LOG</dt>
** <dd>The CORTEX_PREPARE_DONT_LOG flag prevents SQL compiler
** errors from being sent to the error log defined by
** [CORTEX_CONFIG_LOG].  This can be used, for example, to do test
** compiles to see if some SQL syntax is well-formed, without generating
** messages on the global error log when it is not.  If the test compile
** fails, the cortex_prepare_v3() call returns the same error indications
** with or without this flag; it just omits the call to [cortex_log()] that
** logs the error.
** </dl>
*/
#define CORTEX_PREPARE_PERSISTENT              0x01
#define CORTEX_PREPARE_NORMALIZE               0x02
#define CORTEX_PREPARE_NO_VTAB                 0x04
#define CORTEX_PREPARE_DONT_LOG                0x10

/*
** CAPI3REF: Compiling An SQL Statement
** KEYWORDS: {SQL statement compiler}
** METHOD: cortex
** CONSTRUCTOR: cortex_stmt
**
** To execute an SQL statement, it must first be compiled into a byte-code
** program using one of these routines.  Or, in other words, these routines
** are constructors for the [prepared statement] object.
**
** The preferred routine to use is [cortex_prepare_v2()].  The
** [cortex_prepare()] interface is legacy and should be avoided.
** [cortex_prepare_v3()] has an extra "prepFlags" option that is used
** for special purposes.
**
** The use of the UTF-8 interfaces is preferred, as CORTEX currently
** does all parsing using UTF-8.  The UTF-16 interfaces are provided
** as a convenience.  The UTF-16 interfaces work by converting the
** input text into UTF-8, then invoking the corresponding UTF-8 interface.
**
** The first argument, "db", is a [database connection] obtained from a
** prior successful call to [cortex_open()], [cortex_open_v2()] or
** [cortex_open16()].  The database connection must not have been closed.
**
** The second argument, "zSql", is the statement to be compiled, encoded
** as either UTF-8 or UTF-16.  The cortex_prepare(), cortex_prepare_v2(),
** and cortex_prepare_v3()
** interfaces use UTF-8, and cortex_prepare16(), cortex_prepare16_v2(),
** and cortex_prepare16_v3() use UTF-16.
**
** ^If the nByte argument is negative, then zSql is read up to the
** first zero terminator. ^If nByte is positive, then it is the maximum
** number of bytes read from zSql.  When nByte is positive, zSql is read
** up to the first zero terminator or until the nByte bytes have been read,
** whichever comes first.  ^If nByte is zero, then no prepared
** statement is generated.
** If the caller knows that the supplied string is nul-terminated, then
** there is a small performance advantage to passing an nByte parameter that
** is the number of bytes in the input string <i>including</i>
** the nul-terminator.
** Note that nByte measures the length of the input in bytes, not
** characters, even for the UTF-16 interfaces.
**
** ^If pzTail is not NULL then *pzTail is made to point to the first byte
** past the end of the first SQL statement in zSql.  These routines only
** compile the first statement in zSql, so *pzTail is left pointing to
** what remains uncompiled.
**
** ^*ppStmt is left pointing to a compiled [prepared statement] that can be
** executed using [cortex_step()].  ^If there is an error, *ppStmt is set
** to NULL.  ^If the input text contains no SQL (if the input is an empty
** string or a comment) then *ppStmt is set to NULL.
** The calling procedure is responsible for deleting the compiled
** SQL statement using [cortex_finalize()] after it has finished with it.
** ppStmt may not be NULL.
**
** ^On success, the cortex_prepare() family of routines return [CORTEX_OK];
** otherwise an [error code] is returned.
**
** The cortex_prepare_v2(), cortex_prepare_v3(), cortex_prepare16_v2(),
** and cortex_prepare16_v3() interfaces are recommended for all new programs.
** The older interfaces (cortex_prepare() and cortex_prepare16())
** are retained for backwards compatibility, but their use is discouraged.
** ^In the "vX" interfaces, the prepared statement
** that is returned (the [cortex_stmt] object) contains a copy of the
** original SQL text. This causes the [cortex_step()] interface to
** behave differently in three ways:
**
** <ol>
** <li>
** ^If the database schema changes, instead of returning [CORTEX_SCHEMA] as it
** always used to do, [cortex_step()] will automatically recompile the SQL
** statement and try to run it again. As many as [CORTEX_MAX_SCHEMA_RETRY]
** retries will occur before cortex_step() gives up and returns an error.
** </li>
**
** <li>
** ^When an error occurs, [cortex_step()] will return one of the detailed
** [error codes] or [extended error codes].  ^The legacy behavior was that
** [cortex_step()] would only return a generic [CORTEX_ERROR] result code
** and the application would have to make a second call to [cortex_reset()]
** in order to find the underlying cause of the problem. With the "v2" prepare
** interfaces, the underlying reason for the error is returned immediately.
** </li>
**
** <li>
** ^If the specific value bound to a [parameter | host parameter] in the
** WHERE clause might influence the choice of query plan for a statement,
** then the statement will be automatically recompiled, as if there had been
** a schema change, on the first [cortex_step()] call following any change
** to the [cortex_bind_text | bindings] of that [parameter].
** ^The specific value of a WHERE-clause [parameter] might influence the
** choice of query plan if the parameter is the left-hand side of a [LIKE]
** or [GLOB] operator or if the parameter is compared to an indexed column
** and the [CORTEX_ENABLE_STAT4] compile-time option is enabled.
** </li>
** </ol>
**
** <p>^cortex_prepare_v3() differs from cortex_prepare_v2() only in having
** the extra prepFlags parameter, which is a bit array consisting of zero or
** more of the [CORTEX_PREPARE_PERSISTENT|CORTEX_PREPARE_*] flags.  ^The
** cortex_prepare_v2() interface works exactly the same as
** cortex_prepare_v3() with a zero prepFlags parameter.
*/
CORTEX_API int cortex_prepare(
  cortex *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  cortex_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
CORTEX_API int cortex_prepare_v2(
  cortex *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  cortex_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
CORTEX_API int cortex_prepare_v3(
  cortex *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  unsigned int prepFlags, /* Zero or more CORTEX_PREPARE_ flags */
  cortex_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
CORTEX_API int cortex_prepare16(
  cortex *db,            /* Database handle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  cortex_stmt **ppStmt,  /* OUT: Statement handle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);
CORTEX_API int cortex_prepare16_v2(
  cortex *db,            /* Database handle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  cortex_stmt **ppStmt,  /* OUT: Statement handle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);
CORTEX_API int cortex_prepare16_v3(
  cortex *db,            /* Database handle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  unsigned int prepFlags, /* Zero or more CORTEX_PREPARE_ flags */
  cortex_stmt **ppStmt,  /* OUT: Statement handle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);

/*
** CAPI3REF: Retrieving Statement SQL
** METHOD: cortex_stmt
**
** ^The cortex_sql(P) interface returns a pointer to a copy of the UTF-8
** SQL text used to create [prepared statement] P if P was
** created by [cortex_prepare_v2()], [cortex_prepare_v3()],
** [cortex_prepare16_v2()], or [cortex_prepare16_v3()].
** ^The cortex_expanded_sql(P) interface returns a pointer to a UTF-8
** string containing the SQL text of prepared statement P with
** [bound parameters] expanded.
** ^The cortex_normalized_sql(P) interface returns a pointer to a UTF-8
** string containing the normalized SQL text of prepared statement P.  The
** semantics used to normalize a SQL statement are unspecified and subject
** to change.  At a minimum, literal values will be replaced with suitable
** placeholders.
**
** ^(For example, if a prepared statement is created using the SQL
** text "SELECT $abc,:xyz" and if parameter $abc is bound to integer 2345
** and parameter :xyz is unbound, then cortex_sql() will return
** the original string, "SELECT $abc,:xyz" but cortex_expanded_sql()
** will return "SELECT 2345,NULL".)^
**
** ^The cortex_expanded_sql() interface returns NULL if insufficient memory
** is available to hold the result, or if the result would exceed the
** maximum string length determined by the [CORTEX_LIMIT_LENGTH].
**
** ^The [CORTEX_TRACE_SIZE_LIMIT] compile-time option limits the size of
** bound parameter expansions.  ^The [CORTEX_OMIT_TRACE] compile-time
** option causes cortex_expanded_sql() to always return NULL.
**
** ^The strings returned by cortex_sql(P) and cortex_normalized_sql(P)
** are managed by CORTEX and are automatically freed when the prepared
** statement is finalized.
** ^The string returned by cortex_expanded_sql(P), on the other hand,
** is obtained from [cortex_malloc()] and must be freed by the application
** by passing it to [cortex_free()].
**
** ^The cortex_normalized_sql() interface is only available if
** the [CORTEX_ENABLE_NORMALIZE] compile-time option is defined.
*/
CORTEX_API const char *cortex_sql(cortex_stmt *pStmt);
CORTEX_API char *cortex_expanded_sql(cortex_stmt *pStmt);
#ifdef CORTEX_ENABLE_NORMALIZE
CORTEX_API const char *cortex_normalized_sql(cortex_stmt *pStmt);
#endif

/*
** CAPI3REF: Determine If An SQL Statement Writes The Database
** METHOD: cortex_stmt
**
** ^The cortex_stmt_readonly(X) interface returns true (non-zero) if
** and only if the [prepared statement] X makes no direct changes to
** the content of the database file.
**
** Note that [application-defined SQL functions] or
** [virtual tables] might change the database indirectly as a side effect.
** ^(For example, if an application defines a function "eval()" that
** calls [cortex_exec()], then the following SQL statement would
** change the database file through side-effects:
**
** <blockquote><pre>
**    SELECT eval('DELETE FROM t1') FROM t2;
** </pre></blockquote>
**
** But because the [SELECT] statement does not change the database file
** directly, cortex_stmt_readonly() would still return true.)^
**
** ^Transaction control statements such as [BEGIN], [COMMIT], [ROLLBACK],
** [SAVEPOINT], and [RELEASE] cause cortex_stmt_readonly() to return true,
** since the statements themselves do not actually modify the database but
** rather they control the timing of when other statements modify the
** database.  ^The [ATTACH] and [DETACH] statements also cause
** cortex_stmt_readonly() to return true since, while those statements
** change the configuration of a database connection, they do not make
** changes to the content of the database files on disk.
** ^The cortex_stmt_readonly() interface returns true for [BEGIN] since
** [BEGIN] merely sets internal flags, but the [BEGIN|BEGIN IMMEDIATE] and
** [BEGIN|BEGIN EXCLUSIVE] commands do touch the database and so
** cortex_stmt_readonly() returns false for those commands.
**
** ^This routine returns false if there is any possibility that the
** statement might change the database file.  ^A false return does
** not guarantee that the statement will change the database file.
** ^For example, an UPDATE statement might have a WHERE clause that
** makes it a no-op, but the cortex_stmt_readonly() result would still
** be false.  ^Similarly, a CREATE TABLE IF NOT EXISTS statement is a
** read-only no-op if the table already exists, but
** cortex_stmt_readonly() still returns false for such a statement.
**
** ^If prepared statement X is an [EXPLAIN] or [EXPLAIN QUERY PLAN]
** statement, then cortex_stmt_readonly(X) returns the same value as
** if the EXPLAIN or EXPLAIN QUERY PLAN prefix were omitted.
*/
CORTEX_API int cortex_stmt_readonly(cortex_stmt *pStmt);

/*
** CAPI3REF: Query The EXPLAIN Setting For A Prepared Statement
** METHOD: cortex_stmt
**
** ^The cortex_stmt_isexplain(S) interface returns 1 if the
** prepared statement S is an EXPLAIN statement, or 2 if the
** statement S is an EXPLAIN QUERY PLAN.
** ^The cortex_stmt_isexplain(S) interface returns 0 if S is
** an ordinary statement or a NULL pointer.
*/
CORTEX_API int cortex_stmt_isexplain(cortex_stmt *pStmt);

/*
** CAPI3REF: Change The EXPLAIN Setting For A Prepared Statement
** METHOD: cortex_stmt
**
** The cortex_stmt_explain(S,E) interface changes the EXPLAIN
** setting for [prepared statement] S.  If E is zero, then S becomes
** a normal prepared statement.  If E is 1, then S behaves as if
** its SQL text began with "[EXPLAIN]".  If E is 2, then S behaves as if
** its SQL text began with "[EXPLAIN QUERY PLAN]".
**
** Calling cortex_stmt_explain(S,E) might cause S to be reprepared.
** CORTEX tries to avoid a reprepare, but a reprepare might be necessary
** on the first transition into EXPLAIN or EXPLAIN QUERY PLAN mode.
**
** Because of the potential need to reprepare, a call to
** cortex_stmt_explain(S,E) will fail with CORTEX_ERROR if S cannot be
** reprepared because it was created using [cortex_prepare()] instead of
** the newer [cortex_prepare_v2()] or [cortex_prepare_v3()] interfaces and
** hence has no saved SQL text with which to reprepare.
**
** Changing the explain setting for a prepared statement does not change
** the original SQL text for the statement.  Hence, if the SQL text originally
** began with EXPLAIN or EXPLAIN QUERY PLAN, but cortex_stmt_explain(S,0)
** is called to convert the statement into an ordinary statement, the EXPLAIN
** or EXPLAIN QUERY PLAN keywords will still appear in the cortex_sql(S)
** output, even though the statement now acts like a normal SQL statement.
**
** This routine returns CORTEX_OK if the explain mode is successfully
** changed, or an error code if the explain mode could not be changed.
** The explain mode cannot be changed while a statement is active.
** Hence, it is good practice to call [cortex_reset(S)]
** immediately prior to calling cortex_stmt_explain(S,E).
*/
CORTEX_API int cortex_stmt_explain(cortex_stmt *pStmt, int eMode);

/*
** CAPI3REF: Determine If A Prepared Statement Has Been Reset
** METHOD: cortex_stmt
**
** ^The cortex_stmt_busy(S) interface returns true (non-zero) if the
** [prepared statement] S has been stepped at least once using
** [cortex_step(S)] but has neither run to completion (returned
** [CORTEX_DONE] from [cortex_step(S)]) nor
** been reset using [cortex_reset(S)].  ^The cortex_stmt_busy(S)
** interface returns false if S is a NULL pointer.  If S is not a
** NULL pointer and is not a pointer to a valid [prepared statement]
** object, then the behavior is undefined and probably undesirable.
**
** This interface can be used in combination [cortex_next_stmt()]
** to locate all prepared statements associated with a database
** connection that are in need of being reset.  This can be used,
** for example, in diagnostic routines to search for prepared
** statements that are holding a transaction open.
*/
CORTEX_API int cortex_stmt_busy(cortex_stmt*);

/*
** CAPI3REF: Dynamically Typed Value Object
** KEYWORDS: {protected cortex_value} {unprotected cortex_value}
**
** CORTEX uses the cortex_value object to represent all values
** that can be stored in a database table. CORTEX uses dynamic typing
** for the values it stores.  ^Values stored in cortex_value objects
** can be integers, floating point values, strings, BLOBs, or NULL.
**
** An cortex_value object may be either "protected" or "unprotected".
** Some interfaces require a protected cortex_value.  Other interfaces
** will accept either a protected or an unprotected cortex_value.
** Every interface that accepts cortex_value arguments specifies
** whether or not it requires a protected cortex_value.  The
** [cortex_value_dup()] interface can be used to construct a new
** protected cortex_value from an unprotected cortex_value.
**
** The terms "protected" and "unprotected" refer to whether or not
** a mutex is held.  An internal mutex is held for a protected
** cortex_value object but no mutex is held for an unprotected
** cortex_value object.  If CORTEX is compiled to be single-threaded
** (with [CORTEX_THREADSAFE=0] and with [cortex_threadsafe()] returning 0)
** or if CORTEX is run in one of reduced mutex modes
** [CORTEX_CONFIG_SINGLETHREAD] or [CORTEX_CONFIG_MULTITHREAD]
** then there is no distinction between protected and unprotected
** cortex_value objects and they can be used interchangeably.  However,
** for maximum code portability it is recommended that applications
** still make the distinction between protected and unprotected
** cortex_value objects even when not strictly required.
**
** ^The cortex_value objects that are passed as parameters into the
** implementation of [application-defined SQL functions] are protected.
** ^The cortex_value objects returned by [cortex_vtab_rhs_value()]
** are protected.
** ^The cortex_value object returned by
** [cortex_column_value()] is unprotected.
** Unprotected cortex_value objects may only be used as arguments
** to [cortex_result_value()], [cortex_bind_value()], and
** [cortex_value_dup()].
** The [cortex_value_blob | cortex_value_type()] family of
** interfaces require protected cortex_value objects.
*/
typedef struct cortex_value cortex_value;

/*
** CAPI3REF: SQL Function Context Object
**
** The context in which an SQL function executes is stored in an
** cortex_context object.  ^A pointer to an cortex_context object
** is always the first parameter to [application-defined SQL functions].
** The application-defined SQL function implementation will pass this
** pointer through into calls to [cortex_result_int | cortex_result()],
** [cortex_aggregate_context()], [cortex_user_data()],
** [cortex_context_db_handle()], [cortex_get_auxdata()],
** and/or [cortex_set_auxdata()].
*/
typedef struct cortex_context cortex_context;

/*
** CAPI3REF: Binding Values To Prepared Statements
** KEYWORDS: {host parameter} {host parameters} {host parameter name}
** KEYWORDS: {SQL parameter} {SQL parameters} {parameter binding}
** METHOD: cortex_stmt
**
** ^(In the SQL statement text input to [cortex_prepare_v2()] and its variants,
** literals may be replaced by a [parameter] that matches one of the following
** templates:
**
** <ul>
** <li>  ?
** <li>  ?NNN
** <li>  :VVV
** <li>  @VVV
** <li>  $VVV
** </ul>
**
** In the templates above, NNN represents an integer literal,
** and VVV represents an alphanumeric identifier.)^  ^The values of these
** parameters (also called "host parameter names" or "SQL parameters")
** can be set using the cortex_bind_*() routines defined here.
**
** ^The first argument to the cortex_bind_*() routines is always
** a pointer to the [cortex_stmt] object returned from
** [cortex_prepare_v2()] or its variants.
**
** ^The second argument is the index of the SQL parameter to be set.
** ^The leftmost SQL parameter has an index of 1.  ^When the same named
** SQL parameter is used more than once, second and subsequent
** occurrences have the same index as the first occurrence.
** ^The index for named parameters can be looked up using the
** [cortex_bind_parameter_index()] API if desired.  ^The index
** for "?NNN" parameters is the value of NNN.
** ^The NNN value must be between 1 and the [cortex_limit()]
** parameter [CORTEX_LIMIT_VARIABLE_NUMBER] (default value: 32766).
**
** ^The third argument is the value to bind to the parameter.
** ^If the third parameter to cortex_bind_text() or cortex_bind_text16()
** or cortex_bind_blob() is a NULL pointer then the fourth parameter
** is ignored and the end result is the same as cortex_bind_null().
** ^If the third parameter to cortex_bind_text() is not NULL, then
** it should be a pointer to well-formed UTF8 text.
** ^If the third parameter to cortex_bind_text16() is not NULL, then
** it should be a pointer to well-formed UTF16 text.
** ^If the third parameter to cortex_bind_text64() is not NULL, then
** it should be a pointer to a well-formed unicode string that is
** either UTF8 if the sixth parameter is CORTEX_UTF8, or UTF16
** otherwise.
**
** [[byte-order determination rules]] ^The byte-order of
** UTF16 input text is determined by the byte-order mark (BOM, U+FEFF)
** found in the first character, which is removed, or in the absence of a BOM
** the byte order is the native byte order of the host
** machine for cortex_bind_text16() or the byte order specified in
** the 6th parameter for cortex_bind_text64().)^
** ^If UTF16 input text contains invalid unicode
** characters, then CORTEX might change those invalid characters
** into the unicode replacement character: U+FFFD.
**
** ^(In those routines that have a fourth argument, its value is the
** number of bytes in the parameter.  To be clear: the value is the
** number of <u>bytes</u> in the value, not the number of characters.)^
** ^If the fourth parameter to cortex_bind_text() or cortex_bind_text16()
** is negative, then the length of the string is
** the number of bytes up to the first zero terminator.
** If the fourth parameter to cortex_bind_blob() is negative, then
** the behavior is undefined.
** If a non-negative fourth parameter is provided to cortex_bind_text()
** or cortex_bind_text16() or cortex_bind_text64() then
** that parameter must be the byte offset
** where the NUL terminator would occur assuming the string were NUL
** terminated.  If any NUL characters occur at byte offsets less than
** the value of the fourth parameter then the resulting string value will
** contain embedded NULs.  The result of expressions involving strings
** with embedded NULs is undefined.
**
** ^The fifth argument to the BLOB and string binding interfaces controls
** or indicates the lifetime of the object referenced by the third parameter.
** These three options exist:
** ^ (1) A destructor to dispose of the BLOB or string after CORTEX has finished
** with it may be passed. ^It is called to dispose of the BLOB or string even
** if the call to the bind API fails, except the destructor is not called if
** the third parameter is a NULL pointer or the fourth parameter is negative.
** ^ (2) The special constant, [CORTEX_STATIC], may be passed to indicate that
** the application remains responsible for disposing of the object. ^In this
** case, the object and the provided pointer to it must remain valid until
** either the prepared statement is finalized or the same SQL parameter is
** bound to something else, whichever occurs sooner.
** ^ (3) The constant, [CORTEX_TRANSIENT], may be passed to indicate that the
** object is to be copied prior to the return from cortex_bind_*(). ^The
** object and pointer to it must remain valid until then. ^SQLite will then
** manage the lifetime of its private copy.
**
** ^The sixth argument to cortex_bind_text64() must be one of
** [CORTEX_UTF8], [CORTEX_UTF16], [CORTEX_UTF16BE], or [CORTEX_UTF16LE]
** to specify the encoding of the text in the third parameter.  If
** the sixth argument to cortex_bind_text64() is not one of the
** allowed values shown above, or if the text encoding is different
** from the encoding specified by the sixth parameter, then the behavior
** is undefined.
**
** ^The cortex_bind_zeroblob() routine binds a BLOB of length N that
** is filled with zeroes.  ^A zeroblob uses a fixed amount of memory
** (just an integer to hold its size) while it is being processed.
** Zeroblobs are intended to serve as placeholders for BLOBs whose
** content is later written using
** [cortex_blob_open | incremental BLOB I/O] routines.
** ^A negative value for the zeroblob results in a zero-length BLOB.
**
** ^The cortex_bind_pointer(S,I,P,T,D) routine causes the I-th parameter in
** [prepared statement] S to have an SQL value of NULL, but to also be
** associated with the pointer P of type T.  ^D is either a NULL pointer or
** a pointer to a destructor function for P. ^SQLite will invoke the
** destructor D with a single argument of P when it is finished using
** P, even if the call to cortex_bind_pointer() fails.  Due to a
** historical design quirk, results are undefined if D is
** CORTEX_TRANSIENT. The T parameter should be a static string,
** preferably a string literal. The cortex_bind_pointer() routine is
** part of the [pointer passing interface] added for CORTEX 3.20.0.
**
** ^If any of the cortex_bind_*() routines are called with a NULL pointer
** for the [prepared statement] or with a prepared statement for which
** [cortex_step()] has been called more recently than [cortex_reset()],
** then the call will return [CORTEX_MISUSE].  If any cortex_bind_()
** routine is passed a [prepared statement] that has been finalized, the
** result is undefined and probably harmful.
**
** ^Bindings are not cleared by the [cortex_reset()] routine.
** ^Unbound parameters are interpreted as NULL.
**
** ^The cortex_bind_* routines return [CORTEX_OK] on success or an
** [error code] if anything goes wrong.
** ^[CORTEX_TOOBIG] might be returned if the size of a string or BLOB
** exceeds limits imposed by [cortex_limit]([CORTEX_LIMIT_LENGTH]) or
** [CORTEX_MAX_LENGTH].
** ^[CORTEX_RANGE] is returned if the parameter
** index is out of range.  ^[CORTEX_NOMEM] is returned if malloc() fails.
**
** See also: [cortex_bind_parameter_count()],
** [cortex_bind_parameter_name()], and [cortex_bind_parameter_index()].
*/
CORTEX_API int cortex_bind_blob(cortex_stmt*, int, const void*, int n, void(*)(void*));
CORTEX_API int cortex_bind_blob64(cortex_stmt*, int, const void*, cortex_uint64,
                        void(*)(void*));
CORTEX_API int cortex_bind_double(cortex_stmt*, int, double);
CORTEX_API int cortex_bind_int(cortex_stmt*, int, int);
CORTEX_API int cortex_bind_int64(cortex_stmt*, int, cortex_int64);
CORTEX_API int cortex_bind_null(cortex_stmt*, int);
CORTEX_API int cortex_bind_text(cortex_stmt*,int,const char*,int,void(*)(void*));
CORTEX_API int cortex_bind_text16(cortex_stmt*, int, const void*, int, void(*)(void*));
CORTEX_API int cortex_bind_text64(cortex_stmt*, int, const char*, cortex_uint64,
                         void(*)(void*), unsigned char encoding);
CORTEX_API int cortex_bind_value(cortex_stmt*, int, const cortex_value*);
CORTEX_API int cortex_bind_pointer(cortex_stmt*, int, void*, const char*,void(*)(void*));
CORTEX_API int cortex_bind_zeroblob(cortex_stmt*, int, int n);
CORTEX_API int cortex_bind_zeroblob64(cortex_stmt*, int, cortex_uint64);

/*
** CAPI3REF: Number Of SQL Parameters
** METHOD: cortex_stmt
**
** ^This routine can be used to find the number of [SQL parameters]
** in a [prepared statement].  SQL parameters are tokens of the
** form "?", "?NNN", ":AAA", "$AAA", or "@AAA" that serve as
** placeholders for values that are [cortex_bind_blob | bound]
** to the parameters at a later time.
**
** ^(This routine actually returns the index of the largest (rightmost)
** parameter. For all forms except ?NNN, this will correspond to the
** number of unique parameters.  If parameters of the ?NNN form are used,
** there may be gaps in the list.)^
**
** See also: [cortex_bind_blob|cortex_bind()],
** [cortex_bind_parameter_name()], and
** [cortex_bind_parameter_index()].
*/
CORTEX_API int cortex_bind_parameter_count(cortex_stmt*);

/*
** CAPI3REF: Name Of A Host Parameter
** METHOD: cortex_stmt
**
** ^The cortex_bind_parameter_name(P,N) interface returns
** the name of the N-th [SQL parameter] in the [prepared statement] P.
** ^(SQL parameters of the form "?NNN" or ":AAA" or "@AAA" or "$AAA"
** have a name which is the string "?NNN" or ":AAA" or "@AAA" or "$AAA"
** respectively.
** In other words, the initial ":" or "$" or "@" or "?"
** is included as part of the name.)^
** ^Parameters of the form "?" without a following integer have no name
** and are referred to as "nameless" or "anonymous parameters".
**
** ^The first host parameter has an index of 1, not 0.
**
** ^If the value N is out of range or if the N-th parameter is
** nameless, then NULL is returned.  ^The returned string is
** always in UTF-8 encoding even if the named parameter was
** originally specified as UTF-16 in [cortex_prepare16()],
** [cortex_prepare16_v2()], or [cortex_prepare16_v3()].
**
** See also: [cortex_bind_blob|cortex_bind()],
** [cortex_bind_parameter_count()], and
** [cortex_bind_parameter_index()].
*/
CORTEX_API const char *cortex_bind_parameter_name(cortex_stmt*, int);

/*
** CAPI3REF: Index Of A Parameter With A Given Name
** METHOD: cortex_stmt
**
** ^Return the index of an SQL parameter given its name.  ^The
** index value returned is suitable for use as the second
** parameter to [cortex_bind_blob|cortex_bind()].  ^A zero
** is returned if no matching parameter is found.  ^The parameter
** name must be given in UTF-8 even if the original statement
** was prepared from UTF-16 text using [cortex_prepare16_v2()] or
** [cortex_prepare16_v3()].
**
** See also: [cortex_bind_blob|cortex_bind()],
** [cortex_bind_parameter_count()], and
** [cortex_bind_parameter_name()].
*/
CORTEX_API int cortex_bind_parameter_index(cortex_stmt*, const char *zName);

/*
** CAPI3REF: Reset All Bindings On A Prepared Statement
** METHOD: cortex_stmt
**
** ^Contrary to the intuition of many, [cortex_reset()] does not reset
** the [cortex_bind_blob | bindings] on a [prepared statement].
** ^Use this routine to reset all host parameters to NULL.
*/
CORTEX_API int cortex_clear_bindings(cortex_stmt*);

/*
** CAPI3REF: Number Of Columns In A Result Set
** METHOD: cortex_stmt
**
** ^Return the number of columns in the result set returned by the
** [prepared statement]. ^If this routine returns 0, that means the
** [prepared statement] returns no data (for example an [UPDATE]).
** ^However, just because this routine returns a positive number does not
** mean that one or more rows of data will be returned.  ^A SELECT statement
** will always have a positive cortex_column_count() but depending on the
** WHERE clause constraints and the table content, it might return no rows.
**
** See also: [cortex_data_count()]
*/
CORTEX_API int cortex_column_count(cortex_stmt *pStmt);

/*
** CAPI3REF: Column Names In A Result Set
** METHOD: cortex_stmt
**
** ^These routines return the name assigned to a particular column
** in the result set of a [SELECT] statement.  ^The cortex_column_name()
** interface returns a pointer to a zero-terminated UTF-8 string
** and cortex_column_name16() returns a pointer to a zero-terminated
** UTF-16 string.  ^The first parameter is the [prepared statement]
** that implements the [SELECT] statement. ^The second parameter is the
** column number.  ^The leftmost column is number 0.
**
** ^The returned string pointer is valid until either the [prepared statement]
** is destroyed by [cortex_finalize()] or until the statement is automatically
** reprepared by the first call to [cortex_step()] for a particular run
** or until the next call to
** cortex_column_name() or cortex_column_name16() on the same column.
**
** ^If cortex_malloc() fails during the processing of either routine
** (for example during a conversion from UTF-8 to UTF-16) then a
** NULL pointer is returned.
**
** ^The name of a result column is the value of the "AS" clause for
** that column, if there is an AS clause.  If there is no AS clause
** then the name of the column is unspecified and may change from
** one release of CORTEX to the next.
*/
CORTEX_API const char *cortex_column_name(cortex_stmt*, int N);
CORTEX_API const void *cortex_column_name16(cortex_stmt*, int N);

/*
** CAPI3REF: Source Of Data In A Query Result
** METHOD: cortex_stmt
**
** ^These routines provide a means to determine the database, table, and
** table column that is the origin of a particular result column in a
** [SELECT] statement.
** ^The name of the database or table or column can be returned as
** either a UTF-8 or UTF-16 string.  ^The _database_ routines return
** the database name, the _table_ routines return the table name, and
** the origin_ routines return the column name.
** ^The returned string is valid until the [prepared statement] is destroyed
** using [cortex_finalize()] or until the statement is automatically
** reprepared by the first call to [cortex_step()] for a particular run
** or until the same information is requested
** again in a different encoding.
**
** ^The names returned are the original un-aliased names of the
** database, table, and column.
**
** ^The first argument to these interfaces is a [prepared statement].
** ^These functions return information about the Nth result column returned by
** the statement, where N is the second function argument.
** ^The left-most column is column 0 for these routines.
**
** ^If the Nth column returned by the statement is an expression or
** subquery and is not a column value, then all of these functions return
** NULL.  ^These routines might also return NULL if a memory allocation error
** occurs.  ^Otherwise, they return the name of the attached database, table,
** or column that query result column was extracted from.
**
** ^As with all other CORTEX APIs, those whose names end with "16" return
** UTF-16 encoded strings and the other functions return UTF-8.
**
** ^These APIs are only available if the library was compiled with the
** [CORTEX_ENABLE_COLUMN_METADATA] C-preprocessor symbol.
**
** If two or more threads call one or more
** [cortex_column_database_name | column metadata interfaces]
** for the same [prepared statement] and result column
** at the same time then the results are undefined.
*/
CORTEX_API const char *cortex_column_database_name(cortex_stmt*,int);
CORTEX_API const void *cortex_column_database_name16(cortex_stmt*,int);
CORTEX_API const char *cortex_column_table_name(cortex_stmt*,int);
CORTEX_API const void *cortex_column_table_name16(cortex_stmt*,int);
CORTEX_API const char *cortex_column_origin_name(cortex_stmt*,int);
CORTEX_API const void *cortex_column_origin_name16(cortex_stmt*,int);

/*
** CAPI3REF: Declared Datatype Of A Query Result
** METHOD: cortex_stmt
**
** ^(The first parameter is a [prepared statement].
** If this statement is a [SELECT] statement and the Nth column of the
** returned result set of that [SELECT] is a table column (not an
** expression or subquery) then the declared type of the table
** column is returned.)^  ^If the Nth column of the result set is an
** expression or subquery, then a NULL pointer is returned.
** ^The returned string is always UTF-8 encoded.
**
** ^(For example, given the database schema:
**
** CREATE TABLE t1(c1 VARIANT);
**
** and the following statement to be compiled:
**
** SELECT c1 + 1, c1 FROM t1;
**
** this routine would return the string "VARIANT" for the second result
** column (i==1), and a NULL pointer for the first result column (i==0).)^
**
** ^SQLite uses dynamic run-time typing.  ^So just because a column
** is declared to contain a particular type does not mean that the
** data stored in that column is of the declared type.  CORTEX is
** strongly typed, but the typing is dynamic not static.  ^Type
** is associated with individual values, not with the containers
** used to hold those values.
*/
CORTEX_API const char *cortex_column_decltype(cortex_stmt*,int);
CORTEX_API const void *cortex_column_decltype16(cortex_stmt*,int);

/*
** CAPI3REF: Evaluate An SQL Statement
** METHOD: cortex_stmt
**
** After a [prepared statement] has been prepared using any of
** [cortex_prepare_v2()], [cortex_prepare_v3()], [cortex_prepare16_v2()],
** or [cortex_prepare16_v3()] or one of the legacy
** interfaces [cortex_prepare()] or [cortex_prepare16()], this function
** must be called one or more times to evaluate the statement.
**
** The details of the behavior of the cortex_step() interface depend
** on whether the statement was prepared using the newer "vX" interfaces
** [cortex_prepare_v3()], [cortex_prepare_v2()], [cortex_prepare16_v3()],
** [cortex_prepare16_v2()] or the older legacy
** interfaces [cortex_prepare()] and [cortex_prepare16()].  The use of the
** new "vX" interface is recommended for new applications but the legacy
** interface will continue to be supported.
**
** ^In the legacy interface, the return value will be either [CORTEX_BUSY],
** [CORTEX_DONE], [CORTEX_ROW], [CORTEX_ERROR], or [CORTEX_MISUSE].
** ^With the "v2" interface, any of the other [result codes] or
** [extended result codes] might be returned as well.
**
** ^[CORTEX_BUSY] means that the database engine was unable to acquire the
** database locks it needs to do its job.  ^If the statement is a [COMMIT]
** or occurs outside of an explicit transaction, then you can retry the
** statement.  If the statement is not a [COMMIT] and occurs within an
** explicit transaction then you should rollback the transaction before
** continuing.
**
** ^[CORTEX_DONE] means that the statement has finished executing
** successfully.  cortex_step() should not be called again on this virtual
** machine without first calling [cortex_reset()] to reset the virtual
** machine back to its initial state.
**
** ^If the SQL statement being executed returns any data, then [CORTEX_ROW]
** is returned each time a new row of data is ready for processing by the
** caller. The values may be accessed using the [column access functions].
** cortex_step() is called again to retrieve the next row of data.
**
** ^[CORTEX_ERROR] means that a run-time error (such as a constraint
** violation) has occurred.  cortex_step() should not be called again on
** the VM. More information may be found by calling [cortex_errmsg()].
** ^With the legacy interface, a more specific error code (for example,
** [CORTEX_INTERRUPT], [CORTEX_SCHEMA], [CORTEX_CORRUPT], and so forth)
** can be obtained by calling [cortex_reset()] on the
** [prepared statement].  ^In the "v2" interface,
** the more specific error code is returned directly by cortex_step().
**
** [CORTEX_MISUSE] means that the this routine was called inappropriately.
** Perhaps it was called on a [prepared statement] that has
** already been [cortex_finalize | finalized] or on one that had
** previously returned [CORTEX_ERROR] or [CORTEX_DONE].  Or it could
** be the case that the same database connection is being used by two or
** more threads at the same moment in time.
**
** For all versions of CORTEX up to and including 3.6.23.1, a call to
** [cortex_reset()] was required after cortex_step() returned anything
** other than [CORTEX_ROW] before any subsequent invocation of
** cortex_step().  Failure to reset the prepared statement using
** [cortex_reset()] would result in an [CORTEX_MISUSE] return from
** cortex_step().  But after [version 3.6.23.1] ([dateof:3.6.23.1]),
** cortex_step() began
** calling [cortex_reset()] automatically in this circumstance rather
** than returning [CORTEX_MISUSE].  This is not considered a compatibility
** break because any application that ever receives an CORTEX_MISUSE error
** is broken by definition.  The [CORTEX_OMIT_AUTORESET] compile-time option
** can be used to restore the legacy behavior.
**
** <b>Goofy Interface Alert:</b> In the legacy interface, the cortex_step()
** API always returns a generic error code, [CORTEX_ERROR], following any
** error other than [CORTEX_BUSY] and [CORTEX_MISUSE].  You must call
** [cortex_reset()] or [cortex_finalize()] in order to find one of the
** specific [error codes] that better describes the error.
** We admit that this is a goofy design.  The problem has been fixed
** with the "v2" interface.  If you prepare all of your SQL statements
** using [cortex_prepare_v3()] or [cortex_prepare_v2()]
** or [cortex_prepare16_v2()] or [cortex_prepare16_v3()] instead
** of the legacy [cortex_prepare()] and [cortex_prepare16()] interfaces,
** then the more specific [error codes] are returned directly
** by cortex_step().  The use of the "vX" interfaces is recommended.
*/
CORTEX_API int cortex_step(cortex_stmt*);

/*
** CAPI3REF: Number of columns in a result set
** METHOD: cortex_stmt
**
** ^The cortex_data_count(P) interface returns the number of columns in the
** current row of the result set of [prepared statement] P.
** ^If prepared statement P does not have results ready to return
** (via calls to the [cortex_column_int | cortex_column()] family of
** interfaces) then cortex_data_count(P) returns 0.
** ^The cortex_data_count(P) routine also returns 0 if P is a NULL pointer.
** ^The cortex_data_count(P) routine returns 0 if the previous call to
** [cortex_step](P) returned [CORTEX_DONE].  ^The cortex_data_count(P)
** will return non-zero if previous call to [cortex_step](P) returned
** [CORTEX_ROW], except in the case of the [PRAGMA incremental_vacuum]
** where it always returns zero since each step of that multi-step
** pragma returns 0 columns of data.
**
** See also: [cortex_column_count()]
*/
CORTEX_API int cortex_data_count(cortex_stmt *pStmt);

/*
** CAPI3REF: Fundamental Datatypes
** KEYWORDS: CORTEX_TEXT
**
** ^(Every value in CORTEX has one of five fundamental datatypes:
**
** <ul>
** <li> 64-bit signed integer
** <li> 64-bit IEEE floating point number
** <li> string
** <li> BLOB
** <li> NULL
** </ul>)^
**
** These constants are codes for each of those types.
**
** Note that the CORTEX_TEXT constant was also used in CORTEX version 2
** for a completely different meaning.  Software that links against both
** CORTEX version 2 and CORTEX version 3 should use cortex_TEXT, not
** CORTEX_TEXT.
*/
#define CORTEX_INTEGER  1
#define CORTEX_FLOAT    2
#define CORTEX_BLOB     4
#define CORTEX_NULL     5
#ifdef CORTEX_TEXT
# undef CORTEX_TEXT
#else
# define CORTEX_TEXT     3
#endif
#define cortex_TEXT     3

/*
** CAPI3REF: Result Values From A Query
** KEYWORDS: {column access functions}
** METHOD: cortex_stmt
**
** <b>Summary:</b>
** <blockquote><table border=0 cellpadding=0 cellspacing=0>
** <tr><td><b>cortex_column_blob</b><td>&rarr;<td>BLOB result
** <tr><td><b>cortex_column_double</b><td>&rarr;<td>REAL result
** <tr><td><b>cortex_column_int</b><td>&rarr;<td>32-bit INTEGER result
** <tr><td><b>cortex_column_int64</b><td>&rarr;<td>64-bit INTEGER result
** <tr><td><b>cortex_column_text</b><td>&rarr;<td>UTF-8 TEXT result
** <tr><td><b>cortex_column_text16</b><td>&rarr;<td>UTF-16 TEXT result
** <tr><td><b>cortex_column_value</b><td>&rarr;<td>The result as an
** [cortex_value|unprotected cortex_value] object.
** <tr><td>&nbsp;<td>&nbsp;<td>&nbsp;
** <tr><td><b>cortex_column_bytes</b><td>&rarr;<td>Size of a BLOB
** or a UTF-8 TEXT result in bytes
** <tr><td><b>cortex_column_bytes16&nbsp;&nbsp;</b>
** <td>&rarr;&nbsp;&nbsp;<td>Size of UTF-16
** TEXT in bytes
** <tr><td><b>cortex_column_type</b><td>&rarr;<td>Default
** datatype of the result
** </table></blockquote>
**
** <b>Details:</b>
**
** ^These routines return information about a single column of the current
** result row of a query.  ^In every case the first argument is a pointer
** to the [prepared statement] that is being evaluated (the [cortex_stmt*]
** that was returned from [cortex_prepare_v2()] or one of its variants)
** and the second argument is the index of the column for which information
** should be returned. ^The leftmost column of the result set has the index 0.
** ^The number of columns in the result can be determined using
** [cortex_column_count()].
**
** If the SQL statement does not currently point to a valid row, or if the
** column index is out of range, the result is undefined.
** These routines may only be called when the most recent call to
** [cortex_step()] has returned [CORTEX_ROW] and neither
** [cortex_reset()] nor [cortex_finalize()] have been called subsequently.
** If any of these routines are called after [cortex_reset()] or
** [cortex_finalize()] or after [cortex_step()] has returned
** something other than [CORTEX_ROW], the results are undefined.
** If [cortex_step()] or [cortex_reset()] or [cortex_finalize()]
** are called from a different thread while any of these routines
** are pending, then the results are undefined.
**
** The first six interfaces (_blob, _double, _int, _int64, _text, and _text16)
** each return the value of a result column in a specific data format.  If
** the result column is not initially in the requested format (for example,
** if the query returns an integer but the cortex_column_text() interface
** is used to extract the value) then an automatic type conversion is performed.
**
** ^The cortex_column_type() routine returns the
** [CORTEX_INTEGER | datatype code] for the initial data type
** of the result column.  ^The returned value is one of [CORTEX_INTEGER],
** [CORTEX_FLOAT], [CORTEX_TEXT], [CORTEX_BLOB], or [CORTEX_NULL].
** The return value of cortex_column_type() can be used to decide which
** of the first six interface should be used to extract the column value.
** The value returned by cortex_column_type() is only meaningful if no
** automatic type conversions have occurred for the value in question.
** After a type conversion, the result of calling cortex_column_type()
** is undefined, though harmless.  Future
** versions of CORTEX may change the behavior of cortex_column_type()
** following a type conversion.
**
** If the result is a BLOB or a TEXT string, then the cortex_column_bytes()
** or cortex_column_bytes16() interfaces can be used to determine the size
** of that BLOB or string.
**
** ^If the result is a BLOB or UTF-8 string then the cortex_column_bytes()
** routine returns the number of bytes in that BLOB or string.
** ^If the result is a UTF-16 string, then cortex_column_bytes() converts
** the string to UTF-8 and then returns the number of bytes.
** ^If the result is a numeric value then cortex_column_bytes() uses
** [cortex_snprintf()] to convert that value to a UTF-8 string and returns
** the number of bytes in that string.
** ^If the result is NULL, then cortex_column_bytes() returns zero.
**
** ^If the result is a BLOB or UTF-16 string then the cortex_column_bytes16()
** routine returns the number of bytes in that BLOB or string.
** ^If the result is a UTF-8 string, then cortex_column_bytes16() converts
** the string to UTF-16 and then returns the number of bytes.
** ^If the result is a numeric value then cortex_column_bytes16() uses
** [cortex_snprintf()] to convert that value to a UTF-16 string and returns
** the number of bytes in that string.
** ^If the result is NULL, then cortex_column_bytes16() returns zero.
**
** ^The values returned by [cortex_column_bytes()] and
** [cortex_column_bytes16()] do not include the zero terminators at the end
** of the string.  ^For clarity: the values returned by
** [cortex_column_bytes()] and [cortex_column_bytes16()] are the number of
** bytes in the string, not the number of characters.
**
** ^Strings returned by cortex_column_text() and cortex_column_text16(),
** even empty strings, are always zero-terminated.  ^The return
** value from cortex_column_blob() for a zero-length BLOB is a NULL pointer.
**
** ^Strings returned by cortex_column_text16() always have the endianness
** which is native to the platform, regardless of the text encoding set
** for the database.
**
** <b>Warning:</b> ^The object returned by [cortex_column_value()] is an
** [unprotected cortex_value] object.  In a multithreaded environment,
** an unprotected cortex_value object may only be used safely with
** [cortex_bind_value()] and [cortex_result_value()].
** If the [unprotected cortex_value] object returned by
** [cortex_column_value()] is used in any other way, including calls
** to routines like [cortex_value_int()], [cortex_value_text()],
** or [cortex_value_bytes()], the behavior is not threadsafe.
** Hence, the cortex_column_value() interface
** is normally only useful within the implementation of
** [application-defined SQL functions] or [virtual tables], not within
** top-level application code.
**
** These routines may attempt to convert the datatype of the result.
** ^For example, if the internal representation is FLOAT and a text result
** is requested, [cortex_snprintf()] is used internally to perform the
** conversion automatically.  ^(The following table details the conversions
** that are applied:
**
** <blockquote>
** <table border="1">
** <tr><th> Internal<br>Type <th> Requested<br>Type <th>  Conversion
**
** <tr><td>  NULL    <td> INTEGER   <td> Result is 0
** <tr><td>  NULL    <td>  FLOAT    <td> Result is 0.0
** <tr><td>  NULL    <td>   TEXT    <td> Result is a NULL pointer
** <tr><td>  NULL    <td>   BLOB    <td> Result is a NULL pointer
** <tr><td> INTEGER  <td>  FLOAT    <td> Convert from integer to float
** <tr><td> INTEGER  <td>   TEXT    <td> ASCII rendering of the integer
** <tr><td> INTEGER  <td>   BLOB    <td> Same as INTEGER->TEXT
** <tr><td>  FLOAT   <td> INTEGER   <td> [CAST] to INTEGER
** <tr><td>  FLOAT   <td>   TEXT    <td> ASCII rendering of the float
** <tr><td>  FLOAT   <td>   BLOB    <td> [CAST] to BLOB
** <tr><td>  TEXT    <td> INTEGER   <td> [CAST] to INTEGER
** <tr><td>  TEXT    <td>  FLOAT    <td> [CAST] to REAL
** <tr><td>  TEXT    <td>   BLOB    <td> No change
** <tr><td>  BLOB    <td> INTEGER   <td> [CAST] to INTEGER
** <tr><td>  BLOB    <td>  FLOAT    <td> [CAST] to REAL
** <tr><td>  BLOB    <td>   TEXT    <td> [CAST] to TEXT, ensure zero terminator
** </table>
** </blockquote>)^
**
** Note that when type conversions occur, pointers returned by prior
** calls to cortex_column_blob(), cortex_column_text(), and/or
** cortex_column_text16() may be invalidated.
** Type conversions and pointer invalidations might occur
** in the following cases:
**
** <ul>
** <li> The initial content is a BLOB and cortex_column_text() or
**      cortex_column_text16() is called.  A zero-terminator might
**      need to be added to the string.</li>
** <li> The initial content is UTF-8 text and cortex_column_bytes16() or
**      cortex_column_text16() is called.  The content must be converted
**      to UTF-16.</li>
** <li> The initial content is UTF-16 text and cortex_column_bytes() or
**      cortex_column_text() is called.  The content must be converted
**      to UTF-8.</li>
** </ul>
**
** ^Conversions between UTF-16be and UTF-16le are always done in place and do
** not invalidate a prior pointer, though of course the content of the buffer
** that the prior pointer references will have been modified.  Other kinds
** of conversion are done in place when it is possible, but sometimes they
** are not possible and in those cases prior pointers are invalidated.
**
** The safest policy is to invoke these routines
** in one of the following ways:
**
** <ul>
**  <li>cortex_column_text() followed by cortex_column_bytes()</li>
**  <li>cortex_column_blob() followed by cortex_column_bytes()</li>
**  <li>cortex_column_text16() followed by cortex_column_bytes16()</li>
** </ul>
**
** In other words, you should call cortex_column_text(),
** cortex_column_blob(), or cortex_column_text16() first to force the result
** into the desired format, then invoke cortex_column_bytes() or
** cortex_column_bytes16() to find the size of the result.  Do not mix calls
** to cortex_column_text() or cortex_column_blob() with calls to
** cortex_column_bytes16(), and do not mix calls to cortex_column_text16()
** with calls to cortex_column_bytes().
**
** ^The pointers returned are valid until a type conversion occurs as
** described above, or until [cortex_step()] or [cortex_reset()] or
** [cortex_finalize()] is called.  ^The memory space used to hold strings
** and BLOBs is freed automatically.  Do not pass the pointers returned
** from [cortex_column_blob()], [cortex_column_text()], etc. into
** [cortex_free()].
**
** As long as the input parameters are correct, these routines will only
** fail if an out-of-memory error occurs during a format conversion.
** Only the following subset of interfaces are subject to out-of-memory
** errors:
**
** <ul>
** <li> cortex_column_blob()
** <li> cortex_column_text()
** <li> cortex_column_text16()
** <li> cortex_column_bytes()
** <li> cortex_column_bytes16()
** </ul>
**
** If an out-of-memory error occurs, then the return value from these
** routines is the same as if the column had contained an SQL NULL value.
** Valid SQL NULL returns can be distinguished from out-of-memory errors
** by invoking the [cortex_errcode()] immediately after the suspect
** return value is obtained and before any
** other CORTEX interface is called on the same [database connection].
*/
CORTEX_API const void *cortex_column_blob(cortex_stmt*, int iCol);
CORTEX_API double cortex_column_double(cortex_stmt*, int iCol);
CORTEX_API int cortex_column_int(cortex_stmt*, int iCol);
CORTEX_API cortex_int64 cortex_column_int64(cortex_stmt*, int iCol);
CORTEX_API const unsigned char *cortex_column_text(cortex_stmt*, int iCol);
CORTEX_API const void *cortex_column_text16(cortex_stmt*, int iCol);
CORTEX_API cortex_value *cortex_column_value(cortex_stmt*, int iCol);
CORTEX_API int cortex_column_bytes(cortex_stmt*, int iCol);
CORTEX_API int cortex_column_bytes16(cortex_stmt*, int iCol);
CORTEX_API int cortex_column_type(cortex_stmt*, int iCol);

/*
** CAPI3REF: Destroy A Prepared Statement Object
** DESTRUCTOR: cortex_stmt
**
** ^The cortex_finalize() function is called to delete a [prepared statement].
** ^If the most recent evaluation of the statement encountered no errors
** or if the statement has never been evaluated, then cortex_finalize() returns
** CORTEX_OK.  ^If the most recent evaluation of statement S failed, then
** cortex_finalize(S) returns the appropriate [error code] or
** [extended error code].
**
** ^The cortex_finalize(S) routine can be called at any point during
** the life cycle of [prepared statement] S:
** before statement S is ever evaluated, after
** one or more calls to [cortex_reset()], or after any call
** to [cortex_step()] regardless of whether or not the statement has
** completed execution.
**
** ^Invoking cortex_finalize() on a NULL pointer is a harmless no-op.
**
** The application must finalize every [prepared statement] in order to avoid
** resource leaks.  It is a grievous error for the application to try to use
** a prepared statement after it has been finalized.  Any use of a prepared
** statement after it has been finalized can result in undefined and
** undesirable behavior such as segfaults and heap corruption.
*/
CORTEX_API int cortex_finalize(cortex_stmt *pStmt);

/*
** CAPI3REF: Reset A Prepared Statement Object
** METHOD: cortex_stmt
**
** The cortex_reset() function is called to reset a [prepared statement]
** object back to its initial state, ready to be re-executed.
** ^Any SQL statement variables that had values bound to them using
** the [cortex_bind_blob | cortex_bind_*() API] retain their values.
** Use [cortex_clear_bindings()] to reset the bindings.
**
** ^The [cortex_reset(S)] interface resets the [prepared statement] S
** back to the beginning of its program.
**
** ^The return code from [cortex_reset(S)] indicates whether or not
** the previous evaluation of prepared statement S completed successfully.
** ^If [cortex_step(S)] has never before been called on S or if
** [cortex_step(S)] has not been called since the previous call
** to [cortex_reset(S)], then [cortex_reset(S)] will return
** [CORTEX_OK].
**
** ^If the most recent call to [cortex_step(S)] for the
** [prepared statement] S indicated an error, then
** [cortex_reset(S)] returns an appropriate [error code].
** ^The [cortex_reset(S)] interface might also return an [error code]
** if there were no prior errors but the process of resetting
** the prepared statement caused a new error. ^For example, if an
** [INSERT] statement with a [RETURNING] clause is only stepped one time,
** that one call to [cortex_step(S)] might return CORTEX_ROW but
** the overall statement might still fail and the [cortex_reset(S)] call
** might return CORTEX_BUSY if locking constraints prevent the
** database change from committing.  Therefore, it is important that
** applications check the return code from [cortex_reset(S)] even if
** no prior call to [cortex_step(S)] indicated a problem.
**
** ^The [cortex_reset(S)] interface does not change the values
** of any [cortex_bind_blob|bindings] on the [prepared statement] S.
*/
CORTEX_API int cortex_reset(cortex_stmt *pStmt);


/*
** CAPI3REF: Create Or Redefine SQL Functions
** KEYWORDS: {function creation routines}
** METHOD: cortex
**
** ^These functions (collectively known as "function creation routines")
** are used to add SQL functions or aggregates or to redefine the behavior
** of existing SQL functions or aggregates. The only differences between
** the three "cortex_create_function*" routines are the text encoding
** expected for the second parameter (the name of the function being
** created) and the presence or absence of a destructor callback for
** the application data pointer. Function cortex_create_window_function()
** is similar, but allows the user to supply the extra callback functions
** needed by [aggregate window functions].
**
** ^The first parameter is the [database connection] to which the SQL
** function is to be added.  ^If an application uses more than one database
** connection then application-defined SQL functions must be added
** to each database connection separately.
**
** ^The second parameter is the name of the SQL function to be created or
** redefined.  ^The length of the name is limited to 255 bytes in a UTF-8
** representation, exclusive of the zero-terminator.  ^Note that the name
** length limit is in UTF-8 bytes, not characters nor UTF-16 bytes.
** ^Any attempt to create a function with a longer name
** will result in [CORTEX_MISUSE] being returned.
**
** ^The third parameter (nArg)
** is the number of arguments that the SQL function or
** aggregate takes. ^If this parameter is -1, then the SQL function or
** aggregate may take any number of arguments between 0 and the limit
** set by [cortex_limit]([CORTEX_LIMIT_FUNCTION_ARG]).  If the third
** parameter is less than -1 or greater than 127 then the behavior is
** undefined.
**
** ^The fourth parameter, eTextRep, specifies what
** [CORTEX_UTF8 | text encoding] this SQL function prefers for
** its parameters.  The application should set this parameter to
** [CORTEX_UTF16LE] if the function implementation invokes
** [cortex_value_text16le()] on an input, or [CORTEX_UTF16BE] if the
** implementation invokes [cortex_value_text16be()] on an input, or
** [CORTEX_UTF16] if [cortex_value_text16()] is used, or [CORTEX_UTF8]
** otherwise.  ^The same SQL function may be registered multiple times using
** different preferred text encodings, with different implementations for
** each encoding.
** ^When multiple implementations of the same function are available, SQLite
** will pick the one that involves the least amount of data conversion.
**
** ^The fourth parameter may optionally be ORed with [CORTEX_DETERMINISTIC]
** to signal that the function will always return the same result given
** the same inputs within a single SQL statement.  Most SQL functions are
** deterministic.  The built-in [random()] SQL function is an example of a
** function that is not deterministic.  The CORTEX query planner is able to
** perform additional optimizations on deterministic functions, so use
** of the [CORTEX_DETERMINISTIC] flag is recommended where possible.
**
** ^The fourth parameter may also optionally include the [CORTEX_DIRECTONLY]
** flag, which if present prevents the function from being invoked from
** within VIEWs, TRIGGERs, CHECK constraints, generated column expressions,
** index expressions, or the WHERE clause of partial indexes.
**
** For best security, the [CORTEX_DIRECTONLY] flag is recommended for
** all application-defined SQL functions that do not need to be
** used inside of triggers, views, CHECK constraints, or other elements of
** the database schema.  This flag is especially recommended for SQL
** functions that have side effects or reveal internal application state.
** Without this flag, an attacker might be able to modify the schema of
** a database file to include invocations of the function with parameters
** chosen by the attacker, which the application will then execute when
** the database file is opened and read.
**
** ^(The fifth parameter is an arbitrary pointer.  The implementation of the
** function can gain access to this pointer using [cortex_user_data()].)^
**
** ^The sixth, seventh and eighth parameters passed to the three
** "cortex_create_function*" functions, xFunc, xStep and xFinal, are
** pointers to C-language functions that implement the SQL function or
** aggregate. ^A scalar SQL function requires an implementation of the xFunc
** callback only; NULL pointers must be passed as the xStep and xFinal
** parameters. ^An aggregate SQL function requires an implementation of xStep
** and xFinal and NULL pointer must be passed for xFunc. ^To delete an existing
** SQL function or aggregate, pass NULL pointers for all three function
** callbacks.
**
** ^The sixth, seventh, eighth and ninth parameters (xStep, xFinal, xValue
** and xInverse) passed to cortex_create_window_function are pointers to
** C-language callbacks that implement the new function. xStep and xFinal
** must both be non-NULL. xValue and xInverse may either both be NULL, in
** which case a regular aggregate function is created, or must both be
** non-NULL, in which case the new function may be used as either an aggregate
** or aggregate window function. More details regarding the implementation
** of aggregate window functions are
** [user-defined window functions|available here].
**
** ^(If the final parameter to cortex_create_function_v2() or
** cortex_create_window_function() is not NULL, then it is the destructor for
** the application data pointer. The destructor is invoked when the function
** is deleted, either by being overloaded or when the database connection
** closes.)^ ^The destructor is also invoked if the call to
** cortex_create_function_v2() fails.  ^When the destructor callback is
** invoked, it is passed a single argument which is a copy of the application
** data pointer which was the fifth parameter to cortex_create_function_v2().
**
** ^It is permitted to register multiple implementations of the same
** functions with the same name but with either differing numbers of
** arguments or differing preferred text encodings.  ^SQLite will use
** the implementation that most closely matches the way in which the
** SQL function is used.  ^A function implementation with a non-negative
** nArg parameter is a better match than a function implementation with
** a negative nArg.  ^A function where the preferred text encoding
** matches the database encoding is a better
** match than a function where the encoding is different.
** ^A function where the encoding difference is between UTF16le and UTF16be
** is a closer match than a function where the encoding difference is
** between UTF8 and UTF16.
**
** ^Built-in functions may be overloaded by new application-defined functions.
**
** ^An application-defined function is permitted to call other
** CORTEX interfaces.  However, such calls must not
** close the database connection nor finalize or reset the prepared
** statement in which the function is running.
*/
CORTEX_API int cortex_create_function(
  cortex *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(cortex_context*,int,cortex_value**),
  void (*xStep)(cortex_context*,int,cortex_value**),
  void (*xFinal)(cortex_context*)
);
CORTEX_API int cortex_create_function16(
  cortex *db,
  const void *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(cortex_context*,int,cortex_value**),
  void (*xStep)(cortex_context*,int,cortex_value**),
  void (*xFinal)(cortex_context*)
);
CORTEX_API int cortex_create_function_v2(
  cortex *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(cortex_context*,int,cortex_value**),
  void (*xStep)(cortex_context*,int,cortex_value**),
  void (*xFinal)(cortex_context*),
  void(*xDestroy)(void*)
);
CORTEX_API int cortex_create_window_function(
  cortex *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xStep)(cortex_context*,int,cortex_value**),
  void (*xFinal)(cortex_context*),
  void (*xValue)(cortex_context*),
  void (*xInverse)(cortex_context*,int,cortex_value**),
  void(*xDestroy)(void*)
);

/*
** CAPI3REF: Text Encodings
**
** These constants define integer codes that represent the various
** text encodings supported by SQLite.
*/
#define CORTEX_UTF8           1    /* IMP: R-37514-35566 */
#define CORTEX_UTF16LE        2    /* IMP: R-03371-37637 */
#define CORTEX_UTF16BE        3    /* IMP: R-51971-34154 */
#define CORTEX_UTF16          4    /* Use native byte order */
#define CORTEX_ANY            5    /* Deprecated */
#define CORTEX_UTF16_ALIGNED  8    /* cortex_create_collation only */

/*
** CAPI3REF: Function Flags
**
** These constants may be ORed together with the
** [CORTEX_UTF8 | preferred text encoding] as the fourth argument
** to [cortex_create_function()], [cortex_create_function16()], or
** [cortex_create_function_v2()].
**
** <dl>
** [[CORTEX_DETERMINISTIC]] <dt>CORTEX_DETERMINISTIC</dt><dd>
** The CORTEX_DETERMINISTIC flag means that the new function always gives
** the same output when the input parameters are the same.
** The [abs|abs() function] is deterministic, for example, but
** [randomblob|randomblob()] is not.  Functions must
** be deterministic in order to be used in certain contexts such as
** with the WHERE clause of [partial indexes] or in [generated columns].
** CORTEX might also optimize deterministic functions by factoring them
** out of inner loops.
** </dd>
**
** [[CORTEX_DIRECTONLY]] <dt>CORTEX_DIRECTONLY</dt><dd>
** The CORTEX_DIRECTONLY flag means that the function may only be invoked
** from top-level SQL, and cannot be used in VIEWs or TRIGGERs nor in
** schema structures such as [CHECK constraints], [DEFAULT clauses],
** [expression indexes], [partial indexes], or [generated columns].
** <p>
** The CORTEX_DIRECTONLY flag is recommended for any
** [application-defined SQL function]
** that has side-effects or that could potentially leak sensitive information.
** This will prevent attacks in which an application is tricked
** into using a database file that has had its schema surreptitiously
** modified to invoke the application-defined function in ways that are
** harmful.
** <p>
** Some people say it is good practice to set CORTEX_DIRECTONLY on all
** [application-defined SQL functions], regardless of whether or not they
** are security sensitive, as doing so prevents those functions from being used
** inside of the database schema, and thus ensures that the database
** can be inspected and modified using generic tools (such as the [CLI])
** that do not have access to the application-defined functions.
** </dd>
**
** [[CORTEX_INNOCUOUS]] <dt>CORTEX_INNOCUOUS</dt><dd>
** The CORTEX_INNOCUOUS flag means that the function is unlikely
** to cause problems even if misused.  An innocuous function should have
** no side effects and should not depend on any values other than its
** input parameters. The [abs|abs() function] is an example of an
** innocuous function.
** The [load_extension() SQL function] is not innocuous because of its
** side effects.
** <p> CORTEX_INNOCUOUS is similar to CORTEX_DETERMINISTIC, but is not
** exactly the same.  The [random|random() function] is an example of a
** function that is innocuous but not deterministic.
** <p>Some heightened security settings
** ([CORTEX_DBCONFIG_TRUSTED_SCHEMA] and [PRAGMA trusted_schema=OFF])
** disable the use of SQL functions inside views and triggers and in
** schema structures such as [CHECK constraints], [DEFAULT clauses],
** [expression indexes], [partial indexes], and [generated columns] unless
** the function is tagged with CORTEX_INNOCUOUS.  Most built-in functions
** are innocuous.  Developers are advised to avoid using the
** CORTEX_INNOCUOUS flag for application-defined functions unless the
** function has been carefully audited and found to be free of potentially
** security-adverse side-effects and information-leaks.
** </dd>
**
** [[CORTEX_SUBTYPE]] <dt>CORTEX_SUBTYPE</dt><dd>
** The CORTEX_SUBTYPE flag indicates to CORTEX that a function might call
** [cortex_value_subtype()] to inspect the sub-types of its arguments.
** This flag instructs CORTEX to omit some corner-case optimizations that
** might disrupt the operation of the [cortex_value_subtype()] function,
** causing it to return zero rather than the correct subtype().
** All SQL functions that invoke [cortex_value_subtype()] should have this
** property.  If the CORTEX_SUBTYPE property is omitted, then the return
** value from [cortex_value_subtype()] might sometimes be zero even though
** a non-zero subtype was specified by the function argument expression.
**
** [[CORTEX_RESULT_SUBTYPE]] <dt>CORTEX_RESULT_SUBTYPE</dt><dd>
** The CORTEX_RESULT_SUBTYPE flag indicates to CORTEX that a function might call
** [cortex_result_subtype()] to cause a sub-type to be associated with its
** result.
** Every function that invokes [cortex_result_subtype()] should have this
** property.  If it does not, then the call to [cortex_result_subtype()]
** might become a no-op if the function is used as a term in an
** [expression index].  On the other hand, SQL functions that never invoke
** [cortex_result_subtype()] should avoid setting this property, as the
** purpose of this property is to disable certain optimizations that are
** incompatible with subtypes.
**
** [[CORTEX_SELFORDER1]] <dt>CORTEX_SELFORDER1</dt><dd>
** The CORTEX_SELFORDER1 flag indicates that the function is an aggregate
** that internally orders the values provided to the first argument.  The
** ordered-set aggregate SQL notation with a single ORDER BY term can be
** used to invoke this function.  If the ordered-set aggregate notation is
** used on a function that lacks this flag, then an error is raised. Note
** that the ordered-set aggregate syntax is only available if CORTEX is
** built using the -DCORTEX_ENABLE_ORDERED_SET_AGGREGATES compile-time option.
** </dd>
** </dl>
*/
#define CORTEX_DETERMINISTIC    0x000000800
#define CORTEX_DIRECTONLY       0x000080000
#define CORTEX_SUBTYPE          0x000100000
#define CORTEX_INNOCUOUS        0x000200000
#define CORTEX_RESULT_SUBTYPE   0x001000000
#define CORTEX_SELFORDER1       0x002000000

/*
** CAPI3REF: Deprecated Functions
** DEPRECATED
**
** These functions are [deprecated].  In order to maintain
** backwards compatibility with older code, these functions continue
** to be supported.  However, new applications should avoid
** the use of these functions.  To encourage programmers to avoid
** these functions, we will not explain what they do.
*/
#ifndef CORTEX_OMIT_DEPRECATED
CORTEX_API CORTEX_DEPRECATED int cortex_aggregate_count(cortex_context*);
CORTEX_API CORTEX_DEPRECATED int cortex_expired(cortex_stmt*);
CORTEX_API CORTEX_DEPRECATED int cortex_transfer_bindings(cortex_stmt*, cortex_stmt*);
CORTEX_API CORTEX_DEPRECATED int cortex_global_recover(void);
CORTEX_API CORTEX_DEPRECATED void cortex_thread_cleanup(void);
CORTEX_API CORTEX_DEPRECATED int cortex_memory_alarm(void(*)(void*,cortex_int64,int),
                      void*,cortex_int64);
#endif

/*
** CAPI3REF: Obtaining SQL Values
** METHOD: cortex_value
**
** <b>Summary:</b>
** <blockquote><table border=0 cellpadding=0 cellspacing=0>
** <tr><td><b>cortex_value_blob</b><td>&rarr;<td>BLOB value
** <tr><td><b>cortex_value_double</b><td>&rarr;<td>REAL value
** <tr><td><b>cortex_value_int</b><td>&rarr;<td>32-bit INTEGER value
** <tr><td><b>cortex_value_int64</b><td>&rarr;<td>64-bit INTEGER value
** <tr><td><b>cortex_value_pointer</b><td>&rarr;<td>Pointer value
** <tr><td><b>cortex_value_text</b><td>&rarr;<td>UTF-8 TEXT value
** <tr><td><b>cortex_value_text16</b><td>&rarr;<td>UTF-16 TEXT value in
** the native byteorder
** <tr><td><b>cortex_value_text16be</b><td>&rarr;<td>UTF-16be TEXT value
** <tr><td><b>cortex_value_text16le</b><td>&rarr;<td>UTF-16le TEXT value
** <tr><td>&nbsp;<td>&nbsp;<td>&nbsp;
** <tr><td><b>cortex_value_bytes</b><td>&rarr;<td>Size of a BLOB
** or a UTF-8 TEXT in bytes
** <tr><td><b>cortex_value_bytes16&nbsp;&nbsp;</b>
** <td>&rarr;&nbsp;&nbsp;<td>Size of UTF-16
** TEXT in bytes
** <tr><td><b>cortex_value_type</b><td>&rarr;<td>Default
** datatype of the value
** <tr><td><b>cortex_value_numeric_type&nbsp;&nbsp;</b>
** <td>&rarr;&nbsp;&nbsp;<td>Best numeric datatype of the value
** <tr><td><b>cortex_value_nochange&nbsp;&nbsp;</b>
** <td>&rarr;&nbsp;&nbsp;<td>True if the column is unchanged in an UPDATE
** against a virtual table.
** <tr><td><b>cortex_value_frombind&nbsp;&nbsp;</b>
** <td>&rarr;&nbsp;&nbsp;<td>True if value originated from a [bound parameter]
** </table></blockquote>
**
** <b>Details:</b>
**
** These routines extract type, size, and content information from
** [protected cortex_value] objects.  Protected cortex_value objects
** are used to pass parameter information into the functions that
** implement [application-defined SQL functions] and [virtual tables].
**
** These routines work only with [protected cortex_value] objects.
** Any attempt to use these routines on an [unprotected cortex_value]
** is not threadsafe.
**
** ^These routines work just like the corresponding [column access functions]
** except that these routines take a single [protected cortex_value] object
** pointer instead of a [cortex_stmt*] pointer and an integer column number.
**
** ^The cortex_value_text16() interface extracts a UTF-16 string
** in the native byte-order of the host machine.  ^The
** cortex_value_text16be() and cortex_value_text16le() interfaces
** extract UTF-16 strings as big-endian and little-endian respectively.
**
** ^If [cortex_value] object V was initialized
** using [cortex_bind_pointer(S,I,P,X,D)] or [cortex_result_pointer(C,P,X,D)]
** and if X and Y are strings that compare equal according to strcmp(X,Y),
** then cortex_value_pointer(V,Y) will return the pointer P.  ^Otherwise,
** cortex_value_pointer(V,Y) returns a NULL. The cortex_bind_pointer()
** routine is part of the [pointer passing interface] added for CORTEX 3.20.0.
**
** ^(The cortex_value_type(V) interface returns the
** [CORTEX_INTEGER | datatype code] for the initial datatype of the
** [cortex_value] object V. The returned value is one of [CORTEX_INTEGER],
** [CORTEX_FLOAT], [CORTEX_TEXT], [CORTEX_BLOB], or [CORTEX_NULL].)^
** Other interfaces might change the datatype for an cortex_value object.
** For example, if the datatype is initially CORTEX_INTEGER and
** cortex_value_text(V) is called to extract a text value for that
** integer, then subsequent calls to cortex_value_type(V) might return
** CORTEX_TEXT.  Whether or not a persistent internal datatype conversion
** occurs is undefined and may change from one release of CORTEX to the next.
**
** ^(The cortex_value_numeric_type() interface attempts to apply
** numeric affinity to the value.  This means that an attempt is
** made to convert the value to an integer or floating point.  If
** such a conversion is possible without loss of information (in other
** words, if the value is a string that looks like a number)
** then the conversion is performed.  Otherwise no conversion occurs.
** The [CORTEX_INTEGER | datatype] after conversion is returned.)^
**
** ^Within the [xUpdate] method of a [virtual table], the
** cortex_value_nochange(X) interface returns true if and only if
** the column corresponding to X is unchanged by the UPDATE operation
** that the xUpdate method call was invoked to implement and if
** the prior [xColumn] method call that was invoked to extract
** the value for that column returned without setting a result (probably
** because it queried [cortex_vtab_nochange()] and found that the column
** was unchanging).  ^Within an [xUpdate] method, any value for which
** cortex_value_nochange(X) is true will in all other respects appear
** to be a NULL value.  If cortex_value_nochange(X) is invoked anywhere other
** than within an [xUpdate] method call for an UPDATE statement, then
** the return value is arbitrary and meaningless.
**
** ^The cortex_value_frombind(X) interface returns non-zero if the
** value X originated from one of the [cortex_bind_int|cortex_bind()]
** interfaces.  ^If X comes from an SQL literal value, or a table column,
** or an expression, then cortex_value_frombind(X) returns zero.
**
** Please pay particular attention to the fact that the pointer returned
** from [cortex_value_blob()], [cortex_value_text()], or
** [cortex_value_text16()] can be invalidated by a subsequent call to
** [cortex_value_bytes()], [cortex_value_bytes16()], [cortex_value_text()],
** or [cortex_value_text16()].
**
** These routines must be called from the same thread as
** the SQL function that supplied the [cortex_value*] parameters.
**
** As long as the input parameter is correct, these routines can only
** fail if an out-of-memory error occurs during a format conversion.
** Only the following subset of interfaces are subject to out-of-memory
** errors:
**
** <ul>
** <li> cortex_value_blob()
** <li> cortex_value_text()
** <li> cortex_value_text16()
** <li> cortex_value_text16le()
** <li> cortex_value_text16be()
** <li> cortex_value_bytes()
** <li> cortex_value_bytes16()
** </ul>
**
** If an out-of-memory error occurs, then the return value from these
** routines is the same as if the column had contained an SQL NULL value.
** Valid SQL NULL returns can be distinguished from out-of-memory errors
** by invoking the [cortex_errcode()] immediately after the suspect
** return value is obtained and before any
** other CORTEX interface is called on the same [database connection].
*/
CORTEX_API const void *cortex_value_blob(cortex_value*);
CORTEX_API double cortex_value_double(cortex_value*);
CORTEX_API int cortex_value_int(cortex_value*);
CORTEX_API cortex_int64 cortex_value_int64(cortex_value*);
CORTEX_API void *cortex_value_pointer(cortex_value*, const char*);
CORTEX_API const unsigned char *cortex_value_text(cortex_value*);
CORTEX_API const void *cortex_value_text16(cortex_value*);
CORTEX_API const void *cortex_value_text16le(cortex_value*);
CORTEX_API const void *cortex_value_text16be(cortex_value*);
CORTEX_API int cortex_value_bytes(cortex_value*);
CORTEX_API int cortex_value_bytes16(cortex_value*);
CORTEX_API int cortex_value_type(cortex_value*);
CORTEX_API int cortex_value_numeric_type(cortex_value*);
CORTEX_API int cortex_value_nochange(cortex_value*);
CORTEX_API int cortex_value_frombind(cortex_value*);

/*
** CAPI3REF: Report the internal text encoding state of an cortex_value object
** METHOD: cortex_value
**
** ^(The cortex_value_encoding(X) interface returns one of [CORTEX_UTF8],
** [CORTEX_UTF16BE], or [CORTEX_UTF16LE] according to the current text encoding
** of the value X, assuming that X has type TEXT.)^  If cortex_value_type(X)
** returns something other than CORTEX_TEXT, then the return value from
** cortex_value_encoding(X) is meaningless.  ^Calls to
** [cortex_value_text(X)], [cortex_value_text16(X)], [cortex_value_text16be(X)],
** [cortex_value_text16le(X)], [cortex_value_bytes(X)], or
** [cortex_value_bytes16(X)] might change the encoding of the value X and
** thus change the return from subsequent calls to cortex_value_encoding(X).
**
** This routine is intended for used by applications that test and validate
** the CORTEX implementation.  This routine is inquiring about the opaque
** internal state of an [cortex_value] object.  Ordinary applications should
** not need to know what the internal state of an cortex_value object is and
** hence should not need to use this interface.
*/
CORTEX_API int cortex_value_encoding(cortex_value*);

/*
** CAPI3REF: Finding The Subtype Of SQL Values
** METHOD: cortex_value
**
** The cortex_value_subtype(V) function returns the subtype for
** an [application-defined SQL function] argument V.  The subtype
** information can be used to pass a limited amount of context from
** one SQL function to another.  Use the [cortex_result_subtype()]
** routine to set the subtype for the return value of an SQL function.
**
** Every [application-defined SQL function] that invokes this interface
** should include the [CORTEX_SUBTYPE] property in the text
** encoding argument when the function is [cortex_create_function|registered].
** If the [CORTEX_SUBTYPE] property is omitted, then cortex_value_subtype()
** might return zero instead of the upstream subtype in some corner cases.
*/
CORTEX_API unsigned int cortex_value_subtype(cortex_value*);

/*
** CAPI3REF: Copy And Free SQL Values
** METHOD: cortex_value
**
** ^The cortex_value_dup(V) interface makes a copy of the [cortex_value]
** object V and returns a pointer to that copy.  ^The [cortex_value] returned
** is a [protected cortex_value] object even if the input is not.
** ^The cortex_value_dup(V) interface returns NULL if V is NULL or if a
** memory allocation fails. ^If V is a [pointer value], then the result
** of cortex_value_dup(V) is a NULL value.
**
** ^The cortex_value_free(V) interface frees an [cortex_value] object
** previously obtained from [cortex_value_dup()].  ^If V is a NULL pointer
** then cortex_value_free(V) is a harmless no-op.
*/
CORTEX_API cortex_value *cortex_value_dup(const cortex_value*);
CORTEX_API void cortex_value_free(cortex_value*);

/*
** CAPI3REF: Obtain Aggregate Function Context
** METHOD: cortex_context
**
** Implementations of aggregate SQL functions use this
** routine to allocate memory for storing their state.
**
** ^The first time the cortex_aggregate_context(C,N) routine is called
** for a particular aggregate function, CORTEX allocates
** N bytes of memory, zeroes out that memory, and returns a pointer
** to the new memory. ^On second and subsequent calls to
** cortex_aggregate_context() for the same aggregate function instance,
** the same buffer is returned.  cortex_aggregate_context() is normally
** called once for each invocation of the xStep callback and then one
** last time when the xFinal callback is invoked.  ^(When no rows match
** an aggregate query, the xStep() callback of the aggregate function
** implementation is never called and xFinal() is called exactly once.
** In those cases, cortex_aggregate_context() might be called for the
** first time from within xFinal().)^
**
** ^The cortex_aggregate_context(C,N) routine returns a NULL pointer
** when first called if N is less than or equal to zero or if a memory
** allocation error occurs.
**
** ^(The amount of space allocated by cortex_aggregate_context(C,N) is
** determined by the N parameter on the first successful call.  Changing the
** value of N in any subsequent call to cortex_aggregate_context() within
** the same aggregate function instance will not resize the memory
** allocation.)^  Within the xFinal callback, it is customary to set
** N=0 in calls to cortex_aggregate_context(C,N) so that no
** pointless memory allocations occur.
**
** ^SQLite automatically frees the memory allocated by
** cortex_aggregate_context() when the aggregate query concludes.
**
** The first parameter must be a copy of the
** [cortex_context | SQL function context] that is the first parameter
** to the xStep or xFinal callback routine that implements the aggregate
** function.
**
** This routine must be called from the same thread in which
** the aggregate SQL function is running.
*/
CORTEX_API void *cortex_aggregate_context(cortex_context*, int nBytes);

/*
** CAPI3REF: User Data For Functions
** METHOD: cortex_context
**
** ^The cortex_user_data() interface returns a copy of
** the pointer that was the pUserData parameter (the 5th parameter)
** of the [cortex_create_function()]
** and [cortex_create_function16()] routines that originally
** registered the application defined function.
**
** This routine must be called from the same thread in which
** the application-defined function is running.
*/
CORTEX_API void *cortex_user_data(cortex_context*);

/*
** CAPI3REF: Database Connection For Functions
** METHOD: cortex_context
**
** ^The cortex_context_db_handle() interface returns a copy of
** the pointer to the [database connection] (the 1st parameter)
** of the [cortex_create_function()]
** and [cortex_create_function16()] routines that originally
** registered the application defined function.
*/
CORTEX_API cortex *cortex_context_db_handle(cortex_context*);

/*
** CAPI3REF: Function Auxiliary Data
** METHOD: cortex_context
**
** These functions may be used by (non-aggregate) SQL functions to
** associate auxiliary data with argument values. If the same argument
** value is passed to multiple invocations of the same SQL function during
** query execution, under some circumstances the associated auxiliary data
** might be preserved.  An example of where this might be useful is in a
** regular-expression matching function. The compiled version of the regular
** expression can be stored as auxiliary data associated with the pattern string.
** Then as long as the pattern string remains the same,
** the compiled regular expression can be reused on multiple
** invocations of the same function.
**
** ^The cortex_get_auxdata(C,N) interface returns a pointer to the auxiliary data
** associated by the cortex_set_auxdata(C,N,P,X) function with the Nth argument
** value to the application-defined function.  ^N is zero for the left-most
** function argument.  ^If there is no auxiliary data
** associated with the function argument, the cortex_get_auxdata(C,N) interface
** returns a NULL pointer.
**
** ^The cortex_set_auxdata(C,N,P,X) interface saves P as auxiliary data for the
** N-th argument of the application-defined function.  ^Subsequent
** calls to cortex_get_auxdata(C,N) return P from the most recent
** cortex_set_auxdata(C,N,P,X) call if the auxiliary data is still valid or
** NULL if the auxiliary data has been discarded.
** ^After each call to cortex_set_auxdata(C,N,P,X) where X is not NULL,
** CORTEX will invoke the destructor function X with parameter P exactly
** once, when the auxiliary data is discarded.
** CORTEX is free to discard the auxiliary data at any time, including: <ul>
** <li> ^(when the corresponding function parameter changes)^, or
** <li> ^(when [cortex_reset()] or [cortex_finalize()] is called for the
**      SQL statement)^, or
** <li> ^(when cortex_set_auxdata() is invoked again on the same
**       parameter)^, or
** <li> ^(during the original cortex_set_auxdata() call when a memory
**      allocation error occurs.)^
** <li> ^(during the original cortex_set_auxdata() call if the function
**      is evaluated during query planning instead of during query execution,
**      as sometimes happens with [CORTEX_ENABLE_STAT4].)^ </ul>
**
** Note the last two bullets in particular.  The destructor X in
** cortex_set_auxdata(C,N,P,X) might be called immediately, before the
** cortex_set_auxdata() interface even returns.  Hence cortex_set_auxdata()
** should be called near the end of the function implementation and the
** function implementation should not make any use of P after
** cortex_set_auxdata() has been called.  Furthermore, a call to
** cortex_get_auxdata() that occurs immediately after a corresponding call
** to cortex_set_auxdata() might still return NULL if an out-of-memory
** condition occurred during the cortex_set_auxdata() call or if the
** function is being evaluated during query planning rather than during
** query execution.
**
** ^(In practice, auxiliary data is preserved between function calls for
** function parameters that are compile-time constants, including literal
** values and [parameters] and expressions composed from the same.)^
**
** The value of the N parameter to these interfaces should be non-negative.
** Future enhancements may make use of negative N values to define new
** kinds of function caching behavior.
**
** These routines must be called from the same thread in which
** the SQL function is running.
**
** See also: [cortex_get_clientdata()] and [cortex_set_clientdata()].
*/
CORTEX_API void *cortex_get_auxdata(cortex_context*, int N);
CORTEX_API void cortex_set_auxdata(cortex_context*, int N, void*, void (*)(void*));

/*
** CAPI3REF: Database Connection Client Data
** METHOD: cortex
**
** These functions are used to associate one or more named pointers
** with a [database connection].
** A call to cortex_set_clientdata(D,N,P,X) causes the pointer P
** to be attached to [database connection] D using name N.  Subsequent
** calls to cortex_get_clientdata(D,N) will return a copy of pointer P
** or a NULL pointer if there were no prior calls to
** cortex_set_clientdata() with the same values of D and N.
** Names are compared using strcmp() and are thus case sensitive.
** It returns 0 on success and CORTEX_NOMEM on allocation failure.
**
** If P and X are both non-NULL, then the destructor X is invoked with
** argument P on the first of the following occurrences:
** <ul>
** <li> An out-of-memory error occurs during the call to
**      cortex_set_clientdata() which attempts to register pointer P.
** <li> A subsequent call to cortex_set_clientdata(D,N,P,X) is made
**      with the same D and N parameters.
** <li> The database connection closes.  CORTEX does not make any guarantees
**      about the order in which destructors are called, only that all
**      destructors will be called exactly once at some point during the
**      database connection closing process.
** </ul>
**
** CORTEX does not do anything with client data other than invoke
** destructors on the client data at the appropriate time.  The intended
** use for client data is to provide a mechanism for wrapper libraries
** to store additional information about an CORTEX database connection.
**
** There is no limit (other than available memory) on the number of different
** client data pointers (with different names) that can be attached to a
** single database connection.  However, the implementation is optimized
** for the case of having only one or two different client data names.
** Applications and wrapper libraries are discouraged from using more than
** one client data name each.
**
** There is no way to enumerate the client data pointers
** associated with a database connection.  The N parameter can be thought
** of as a secret key such that only code that knows the secret key is able
** to access the associated data.
**
** Security Warning:  These interfaces should not be exposed in scripting
** languages or in other circumstances where it might be possible for an
** attacker to invoke them.  Any agent that can invoke these interfaces
** can probably also take control of the process.
**
** Database connection client data is only available for SQLite
** version 3.44.0 ([dateof:3.44.0]) and later.
**
** See also: [cortex_set_auxdata()] and [cortex_get_auxdata()].
*/
CORTEX_API void *cortex_get_clientdata(cortex*,const char*);
CORTEX_API int cortex_set_clientdata(cortex*, const char*, void*, void(*)(void*));

/*
** CAPI3REF: Constants Defining Special Destructor Behavior
**
** These are special values for the destructor that is passed in as the
** final argument to routines like [cortex_result_blob()].  ^If the destructor
** argument is CORTEX_STATIC, it means that the content pointer is constant
** and will never change.  It does not need to be destroyed.  ^The
** CORTEX_TRANSIENT value means that the content will likely change in
** the near future and that CORTEX should make its own private copy of
** the content before returning.
**
** The typedef is necessary to work around problems in certain
** C++ compilers.
*/
typedef void (*cortex_destructor_type)(void*);
#define CORTEX_STATIC      ((cortex_destructor_type)0)
#define CORTEX_TRANSIENT   ((cortex_destructor_type)-1)

/*
** CAPI3REF: Setting The Result Of An SQL Function
** METHOD: cortex_context
**
** These routines are used by the xFunc or xFinal callbacks that
** implement SQL functions and aggregates.  See
** [cortex_create_function()] and [cortex_create_function16()]
** for additional information.
**
** These functions work very much like the [parameter binding] family of
** functions used to bind values to host parameters in prepared statements.
** Refer to the [SQL parameter] documentation for additional information.
**
** ^The cortex_result_blob() interface sets the result from
** an application-defined function to be the BLOB whose content is pointed
** to by the second parameter and which is N bytes long where N is the
** third parameter.
**
** ^The cortex_result_zeroblob(C,N) and cortex_result_zeroblob64(C,N)
** interfaces set the result of the application-defined function to be
** a BLOB containing all zero bytes and N bytes in size.
**
** ^The cortex_result_double() interface sets the result from
** an application-defined function to be a floating point value specified
** by its 2nd argument.
**
** ^The cortex_result_error() and cortex_result_error16() functions
** cause the implemented SQL function to throw an exception.
** ^SQLite uses the string pointed to by the
** 2nd parameter of cortex_result_error() or cortex_result_error16()
** as the text of an error message.  ^SQLite interprets the error
** message string from cortex_result_error() as UTF-8. ^SQLite
** interprets the string from cortex_result_error16() as UTF-16 using
** the same [byte-order determination rules] as [cortex_bind_text16()].
** ^If the third parameter to cortex_result_error()
** or cortex_result_error16() is negative then CORTEX takes as the error
** message all text up through the first zero character.
** ^If the third parameter to cortex_result_error() or
** cortex_result_error16() is non-negative then CORTEX takes that many
** bytes (not characters) from the 2nd parameter as the error message.
** ^The cortex_result_error() and cortex_result_error16()
** routines make a private copy of the error message text before
** they return.  Hence, the calling function can deallocate or
** modify the text after they return without harm.
** ^The cortex_result_error_code() function changes the error code
** returned by CORTEX as a result of an error in a function.  ^By default,
** the error code is CORTEX_ERROR.  ^A subsequent call to cortex_result_error()
** or cortex_result_error16() resets the error code to CORTEX_ERROR.
**
** ^The cortex_result_error_toobig() interface causes CORTEX to throw an
** error indicating that a string or BLOB is too long to represent.
**
** ^The cortex_result_error_nomem() interface causes CORTEX to throw an
** error indicating that a memory allocation failed.
**
** ^The cortex_result_int() interface sets the return value
** of the application-defined function to be the 32-bit signed integer
** value given in the 2nd argument.
** ^The cortex_result_int64() interface sets the return value
** of the application-defined function to be the 64-bit signed integer
** value given in the 2nd argument.
**
** ^The cortex_result_null() interface sets the return value
** of the application-defined function to be NULL.
**
** ^The cortex_result_text(), cortex_result_text16(),
** cortex_result_text16le(), and cortex_result_text16be() interfaces
** set the return value of the application-defined function to be
** a text string which is represented as UTF-8, UTF-16 native byte order,
** UTF-16 little endian, or UTF-16 big endian, respectively.
** ^The cortex_result_text64() interface sets the return value of an
** application-defined function to be a text string in an encoding
** specified by the fifth (and last) parameter, which must be one
** of [CORTEX_UTF8], [CORTEX_UTF16], [CORTEX_UTF16BE], or [CORTEX_UTF16LE].
** ^SQLite takes the text result from the application from
** the 2nd parameter of the cortex_result_text* interfaces.
** ^If the 3rd parameter to any of the cortex_result_text* interfaces
** other than cortex_result_text64() is negative, then CORTEX computes
** the string length itself by searching the 2nd parameter for the first
** zero character.
** ^If the 3rd parameter to the cortex_result_text* interfaces
** is non-negative, then as many bytes (not characters) of the text
** pointed to by the 2nd parameter are taken as the application-defined
** function result.  If the 3rd parameter is non-negative, then it
** must be the byte offset into the string where the NUL terminator would
** appear if the string were NUL terminated.  If any NUL characters occur
** in the string at a byte offset that is less than the value of the 3rd
** parameter, then the resulting string will contain embedded NULs and the
** result of expressions operating on strings with embedded NULs is undefined.
** ^If the 4th parameter to the cortex_result_text* interfaces
** or cortex_result_blob is a non-NULL pointer, then CORTEX calls that
** function as the destructor on the text or BLOB result when it has
** finished using that result.
** ^If the 4th parameter to the cortex_result_text* interfaces or to
** cortex_result_blob is the special constant CORTEX_STATIC, then SQLite
** assumes that the text or BLOB result is in constant space and does not
** copy the content of the parameter nor call a destructor on the content
** when it has finished using that result.
** ^If the 4th parameter to the cortex_result_text* interfaces
** or cortex_result_blob is the special constant CORTEX_TRANSIENT
** then CORTEX makes a copy of the result into space obtained
** from [cortex_malloc()] before it returns.
**
** ^For the cortex_result_text16(), cortex_result_text16le(), and
** cortex_result_text16be() routines, and for cortex_result_text64()
** when the encoding is not UTF8, if the input UTF16 begins with a
** byte-order mark (BOM, U+FEFF) then the BOM is removed from the
** string and the rest of the string is interpreted according to the
** byte-order specified by the BOM.  ^The byte-order specified by
** the BOM at the beginning of the text overrides the byte-order
** specified by the interface procedure.  ^So, for example, if
** cortex_result_text16le() is invoked with text that begins
** with bytes 0xfe, 0xff (a big-endian byte-order mark) then the
** first two bytes of input are skipped and the remaining input
** is interpreted as UTF16BE text.
**
** ^For UTF16 input text to the cortex_result_text16(),
** cortex_result_text16be(), cortex_result_text16le(), and
** cortex_result_text64() routines, if the text contains invalid
** UTF16 characters, the invalid characters might be converted
** into the unicode replacement character, U+FFFD.
**
** ^The cortex_result_value() interface sets the result of
** the application-defined function to be a copy of the
** [unprotected cortex_value] object specified by the 2nd parameter.  ^The
** cortex_result_value() interface makes a copy of the [cortex_value]
** so that the [cortex_value] specified in the parameter may change or
** be deallocated after cortex_result_value() returns without harm.
** ^A [protected cortex_value] object may always be used where an
** [unprotected cortex_value] object is required, so either
** kind of [cortex_value] object can be used with this interface.
**
** ^The cortex_result_pointer(C,P,T,D) interface sets the result to an
** SQL NULL value, just like [cortex_result_null(C)], except that it
** also associates the host-language pointer P or type T with that
** NULL value such that the pointer can be retrieved within an
** [application-defined SQL function] using [cortex_value_pointer()].
** ^If the D parameter is not NULL, then it is a pointer to a destructor
** for the P parameter.  ^SQLite invokes D with P as its only argument
** when CORTEX is finished with P.  The T parameter should be a static
** string and preferably a string literal. The cortex_result_pointer()
** routine is part of the [pointer passing interface] added for CORTEX 3.20.0.
**
** If these routines are called from within a different thread
** than the one containing the application-defined function that received
** the [cortex_context] pointer, the results are undefined.
*/
CORTEX_API void cortex_result_blob(cortex_context*, const void*, int, void(*)(void*));
CORTEX_API void cortex_result_blob64(cortex_context*,const void*,
                           cortex_uint64,void(*)(void*));
CORTEX_API void cortex_result_double(cortex_context*, double);
CORTEX_API void cortex_result_error(cortex_context*, const char*, int);
CORTEX_API void cortex_result_error16(cortex_context*, const void*, int);
CORTEX_API void cortex_result_error_toobig(cortex_context*);
CORTEX_API void cortex_result_error_nomem(cortex_context*);
CORTEX_API void cortex_result_error_code(cortex_context*, int);
CORTEX_API void cortex_result_int(cortex_context*, int);
CORTEX_API void cortex_result_int64(cortex_context*, cortex_int64);
CORTEX_API void cortex_result_null(cortex_context*);
CORTEX_API void cortex_result_text(cortex_context*, const char*, int, void(*)(void*));
CORTEX_API void cortex_result_text64(cortex_context*, const char*,cortex_uint64,
                           void(*)(void*), unsigned char encoding);
CORTEX_API void cortex_result_text16(cortex_context*, const void*, int, void(*)(void*));
CORTEX_API void cortex_result_text16le(cortex_context*, const void*, int,void(*)(void*));
CORTEX_API void cortex_result_text16be(cortex_context*, const void*, int,void(*)(void*));
CORTEX_API void cortex_result_value(cortex_context*, cortex_value*);
CORTEX_API void cortex_result_pointer(cortex_context*, void*,const char*,void(*)(void*));
CORTEX_API void cortex_result_zeroblob(cortex_context*, int n);
CORTEX_API int cortex_result_zeroblob64(cortex_context*, cortex_uint64 n);


/*
** CAPI3REF: Setting The Subtype Of An SQL Function
** METHOD: cortex_context
**
** The cortex_result_subtype(C,T) function causes the subtype of
** the result from the [application-defined SQL function] with
** [cortex_context] C to be the value T.  Only the lower 8 bits
** of the subtype T are preserved in current versions of SQLite;
** higher order bits are discarded.
** The number of subtype bytes preserved by CORTEX might increase
** in future releases of SQLite.
**
** Every [application-defined SQL function] that invokes this interface
** should include the [CORTEX_RESULT_SUBTYPE] property in its
** text encoding argument when the SQL function is
** [cortex_create_function|registered].  If the [CORTEX_RESULT_SUBTYPE]
** property is omitted from the function that invokes cortex_result_subtype(),
** then in some cases the cortex_result_subtype() might fail to set
** the result subtype.
**
** If CORTEX is compiled with -DCORTEX_STRICT_SUBTYPE=1, then any
** SQL function that invokes the cortex_result_subtype() interface
** and that does not have the CORTEX_RESULT_SUBTYPE property will raise
** an error.  Future versions of CORTEX might enable -DCORTEX_STRICT_SUBTYPE=1
** by default.
*/
CORTEX_API void cortex_result_subtype(cortex_context*,unsigned int);

/*
** CAPI3REF: Define New Collating Sequences
** METHOD: cortex
**
** ^These functions add, remove, or modify a [collation] associated
** with the [database connection] specified as the first argument.
**
** ^The name of the collation is a UTF-8 string
** for cortex_create_collation() and cortex_create_collation_v2()
** and a UTF-16 string in native byte order for cortex_create_collation16().
** ^Collation names that compare equal according to [cortex_strnicmp()] are
** considered to be the same name.
**
** ^(The third argument (eTextRep) must be one of the constants:
** <ul>
** <li> [CORTEX_UTF8],
** <li> [CORTEX_UTF16LE],
** <li> [CORTEX_UTF16BE],
** <li> [CORTEX_UTF16], or
** <li> [CORTEX_UTF16_ALIGNED].
** </ul>)^
** ^The eTextRep argument determines the encoding of strings passed
** to the collating function callback, xCompare.
** ^The [CORTEX_UTF16] and [CORTEX_UTF16_ALIGNED] values for eTextRep
** force strings to be UTF16 with native byte order.
** ^The [CORTEX_UTF16_ALIGNED] value for eTextRep forces strings to begin
** on an even byte address.
**
** ^The fourth argument, pArg, is an application data pointer that is passed
** through as the first argument to the collating function callback.
**
** ^The fifth argument, xCompare, is a pointer to the collating function.
** ^Multiple collating functions can be registered using the same name but
** with different eTextRep parameters and CORTEX will use whichever
** function requires the least amount of data transformation.
** ^If the xCompare argument is NULL then the collating function is
** deleted.  ^When all collating functions having the same name are deleted,
** that collation is no longer usable.
**
** ^The collating function callback is invoked with a copy of the pArg
** application data pointer and with two strings in the encoding specified
** by the eTextRep argument.  The two integer parameters to the collating
** function callback are the length of the two strings, in bytes. The collating
** function must return an integer that is negative, zero, or positive
** if the first string is less than, equal to, or greater than the second,
** respectively.  A collating function must always return the same answer
** given the same inputs.  If two or more collating functions are registered
** to the same collation name (using different eTextRep values) then all
** must give an equivalent answer when invoked with equivalent strings.
** The collating function must obey the following properties for all
** strings A, B, and C:
**
** <ol>
** <li> If A==B then B==A.
** <li> If A==B and B==C then A==C.
** <li> If A&lt;B THEN B&gt;A.
** <li> If A&lt;B and B&lt;C then A&lt;C.
** </ol>
**
** If a collating function fails any of the above constraints and that
** collating function is registered and used, then the behavior of SQLite
** is undefined.
**
** ^The cortex_create_collation_v2() works like cortex_create_collation()
** with the addition that the xDestroy callback is invoked on pArg when
** the collating function is deleted.
** ^Collating functions are deleted when they are overridden by later
** calls to the collation creation functions or when the
** [database connection] is closed using [cortex_close()].
**
** ^The xDestroy callback is <u>not</u> called if the
** cortex_create_collation_v2() function fails.  Applications that invoke
** cortex_create_collation_v2() with a non-NULL xDestroy argument should
** check the return code and dispose of the application data pointer
** themselves rather than expecting CORTEX to deal with it for them.
** This is different from every other CORTEX interface.  The inconsistency
** is unfortunate but cannot be changed without breaking backwards
** compatibility.
**
** See also:  [cortex_collation_needed()] and [cortex_collation_needed16()].
*/
CORTEX_API int cortex_create_collation(
  cortex*,
  const char *zName,
  int eTextRep,
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*)
);
CORTEX_API int cortex_create_collation_v2(
  cortex*,
  const char *zName,
  int eTextRep,
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*),
  void(*xDestroy)(void*)
);
CORTEX_API int cortex_create_collation16(
  cortex*,
  const void *zName,
  int eTextRep,
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*)
);

/*
** CAPI3REF: Collation Needed Callbacks
** METHOD: cortex
**
** ^To avoid having to register all collation sequences before a database
** can be used, a single callback function may be registered with the
** [database connection] to be invoked whenever an undefined collation
** sequence is required.
**
** ^If the function is registered using the cortex_collation_needed() API,
** then it is passed the names of undefined collation sequences as strings
** encoded in UTF-8. ^If cortex_collation_needed16() is used,
** the names are passed as UTF-16 in machine native byte order.
** ^A call to either function replaces the existing collation-needed callback.
**
** ^(When the callback is invoked, the first argument passed is a copy
** of the second argument to cortex_collation_needed() or
** cortex_collation_needed16().  The second argument is the database
** connection.  The third argument is one of [CORTEX_UTF8], [CORTEX_UTF16BE],
** or [CORTEX_UTF16LE], indicating the most desirable form of the collation
** sequence function required.  The fourth parameter is the name of the
** required collation sequence.)^
**
** The callback function should register the desired collation using
** [cortex_create_collation()], [cortex_create_collation16()], or
** [cortex_create_collation_v2()].
*/
CORTEX_API int cortex_collation_needed(
  cortex*,
  void*,
  void(*)(void*,cortex*,int eTextRep,const char*)
);
CORTEX_API int cortex_collation_needed16(
  cortex*,
  void*,
  void(*)(void*,cortex*,int eTextRep,const void*)
);

#ifdef CORTEX_ENABLE_CEROD
/*
** Specify the activation key for a CEROD database.  Unless
** activated, none of the CEROD routines will work.
*/
CORTEX_API void cortex_activate_cerod(
  const char *zPassPhrase        /* Activation phrase */
);
#endif

/*
** CAPI3REF: Suspend Execution For A Short Time
**
** The cortex_sleep() function causes the current thread to suspend execution
** for at least a number of milliseconds specified in its parameter.
**
** If the operating system does not support sleep requests with
** millisecond time resolution, then the time will be rounded up to
** the nearest second. The number of milliseconds of sleep actually
** requested from the operating system is returned.
**
** ^SQLite implements this interface by calling the xSleep()
** method of the default [cortex_vfs] object.  If the xSleep() method
** of the default VFS is not implemented correctly, or not implemented at
** all, then the behavior of cortex_sleep() may deviate from the description
** in the previous paragraphs.
**
** If a negative argument is passed to cortex_sleep() the results vary by
** VFS and operating system.  Some system treat a negative argument as an
** instruction to sleep forever.  Others understand it to mean do not sleep
** at all. ^In CORTEX version 3.42.0 and later, a negative
** argument passed into cortex_sleep() is changed to zero before it is relayed
** down into the xSleep method of the VFS.
*/
CORTEX_API int cortex_sleep(int);

/*
** CAPI3REF: Name Of The Folder Holding Temporary Files
**
** ^(If this global variable is made to point to a string which is
** the name of a folder (a.k.a. directory), then all temporary files
** created by CORTEX when using a built-in [cortex_vfs | VFS]
** will be placed in that directory.)^  ^If this variable
** is a NULL pointer, then CORTEX performs a search for an appropriate
** temporary file directory.
**
** Applications are strongly discouraged from using this global variable.
** It is required to set a temporary folder on Windows Runtime (WinRT).
** But for all other platforms, it is highly recommended that applications
** neither read nor write this variable.  This global variable is a relic
** that exists for backwards compatibility of legacy applications and should
** be avoided in new projects.
**
** It is not safe to read or modify this variable in more than one
** thread at a time.  It is not safe to read or modify this variable
** if a [database connection] is being used at the same time in a separate
** thread.
** It is intended that this variable be set once
** as part of process initialization and before any CORTEX interface
** routines have been called and that this variable remain unchanged
** thereafter.
**
** ^The [temp_store_directory pragma] may modify this variable and cause
** it to point to memory obtained from [cortex_malloc].  ^Furthermore,
** the [temp_store_directory pragma] always assumes that any string
** that this variable points to is held in memory obtained from
** [cortex_malloc] and the pragma may attempt to free that memory
** using [cortex_free].
** Hence, if this variable is modified directly, either it should be
** made NULL or made to point to memory obtained from [cortex_malloc]
** or else the use of the [temp_store_directory pragma] should be avoided.
** Except when requested by the [temp_store_directory pragma], SQLite
** does not free the memory that cortex_temp_directory points to.  If
** the application wants that memory to be freed, it must do
** so itself, taking care to only do so after all [database connection]
** objects have been destroyed.
**
** <b>Note to Windows Runtime users:</b>  The temporary directory must be set
** prior to calling [cortex_open] or [cortex_open_v2].  Otherwise, various
** features that require the use of temporary files may fail.  Here is an
** example of how to do this using C++ with the Windows Runtime:
**
** <blockquote><pre>
** LPCWSTR zPath = Windows::Storage::ApplicationData::Current->
** &nbsp;     TemporaryFolder->Path->Data();
** char zPathBuf&#91;MAX_PATH + 1&#93;;
** memset(zPathBuf, 0, sizeof(zPathBuf));
** WideCharToMultiByte(CP_UTF8, 0, zPath, -1, zPathBuf, sizeof(zPathBuf),
** &nbsp;     NULL, NULL);
** cortex_temp_directory = cortex_mprintf("%s", zPathBuf);
** </pre></blockquote>
*/
CORTEX_API CORTEX_EXTERN char *cortex_temp_directory;

/*
** CAPI3REF: Name Of The Folder Holding Database Files
**
** ^(If this global variable is made to point to a string which is
** the name of a folder (a.k.a. directory), then all database files
** specified with a relative pathname and created or accessed by
** CORTEX when using a built-in windows [cortex_vfs | VFS] will be assumed
** to be relative to that directory.)^ ^If this variable is a NULL
** pointer, then CORTEX assumes that all database files specified
** with a relative pathname are relative to the current directory
** for the process.  Only the windows VFS makes use of this global
** variable; it is ignored by the unix VFS.
**
** Changing the value of this variable while a database connection is
** open can result in a corrupt database.
**
** It is not safe to read or modify this variable in more than one
** thread at a time.  It is not safe to read or modify this variable
** if a [database connection] is being used at the same time in a separate
** thread.
** It is intended that this variable be set once
** as part of process initialization and before any CORTEX interface
** routines have been called and that this variable remain unchanged
** thereafter.
**
** ^The [data_store_directory pragma] may modify this variable and cause
** it to point to memory obtained from [cortex_malloc].  ^Furthermore,
** the [data_store_directory pragma] always assumes that any string
** that this variable points to is held in memory obtained from
** [cortex_malloc] and the pragma may attempt to free that memory
** using [cortex_free].
** Hence, if this variable is modified directly, either it should be
** made NULL or made to point to memory obtained from [cortex_malloc]
** or else the use of the [data_store_directory pragma] should be avoided.
*/
CORTEX_API CORTEX_EXTERN char *cortex_data_directory;

/*
** CAPI3REF: Win32 Specific Interface
**
** These interfaces are available only on Windows.  The
** [cortex_win32_set_directory] interface is used to set the value associated
** with the [cortex_temp_directory] or [cortex_data_directory] variable, to
** zValue, depending on the value of the type parameter.  The zValue parameter
** should be NULL to cause the previous value to be freed via [cortex_free];
** a non-NULL value will be copied into memory obtained from [cortex_malloc]
** prior to being used.  The [cortex_win32_set_directory] interface returns
** [CORTEX_OK] to indicate success, [CORTEX_ERROR] if the type is unsupported,
** or [CORTEX_NOMEM] if memory could not be allocated.  The value of the
** [cortex_data_directory] variable is intended to act as a replacement for
** the current directory on the sub-platforms of Win32 where that concept is
** not present, e.g. WinRT and UWP.  The [cortex_win32_set_directory8] and
** [cortex_win32_set_directory16] interfaces behave exactly the same as the
** cortex_win32_set_directory interface except the string parameter must be
** UTF-8 or UTF-16, respectively.
*/
CORTEX_API int cortex_win32_set_directory(
  unsigned long type, /* Identifier for directory being set or reset */
  void *zValue        /* New value for directory being set or reset */
);
CORTEX_API int cortex_win32_set_directory8(unsigned long type, const char *zValue);
CORTEX_API int cortex_win32_set_directory16(unsigned long type, const void *zValue);

/*
** CAPI3REF: Win32 Directory Types
**
** These macros are only available on Windows.  They define the allowed values
** for the type argument to the [cortex_win32_set_directory] interface.
*/
#define CORTEX_WIN32_DATA_DIRECTORY_TYPE  1
#define CORTEX_WIN32_TEMP_DIRECTORY_TYPE  2

/*
** CAPI3REF: Test For Auto-Commit Mode
** KEYWORDS: {autocommit mode}
** METHOD: cortex
**
** ^The cortex_get_autocommit() interface returns non-zero or
** zero if the given database connection is or is not in autocommit mode,
** respectively.  ^Autocommit mode is on by default.
** ^Autocommit mode is disabled by a [BEGIN] statement.
** ^Autocommit mode is re-enabled by a [COMMIT] or [ROLLBACK].
**
** If certain kinds of errors occur on a statement within a multi-statement
** transaction (errors including [CORTEX_FULL], [CORTEX_IOERR],
** [CORTEX_NOMEM], [CORTEX_BUSY], and [CORTEX_INTERRUPT]) then the
** transaction might be rolled back automatically.  The only way to
** find out whether CORTEX automatically rolled back the transaction after
** an error is to use this function.
**
** If another thread changes the autocommit status of the database
** connection while this routine is running, then the return value
** is undefined.
*/
CORTEX_API int cortex_get_autocommit(cortex*);

/*
** CAPI3REF: Find The Database Handle Of A Prepared Statement
** METHOD: cortex_stmt
**
** ^The cortex_db_handle interface returns the [database connection] handle
** to which a [prepared statement] belongs.  ^The [database connection]
** returned by cortex_db_handle is the same [database connection]
** that was the first argument
** to the [cortex_prepare_v2()] call (or its variants) that was used to
** create the statement in the first place.
*/
CORTEX_API cortex *cortex_db_handle(cortex_stmt*);

/*
** CAPI3REF: Return The Schema Name For A Database Connection
** METHOD: cortex
**
** ^The cortex_db_name(D,N) interface returns a pointer to the schema name
** for the N-th database on database connection D, or a NULL pointer if N is
** out of range.  An N value of 0 means the main database file.  An N of 1 is
** the "temp" schema.  Larger values of N correspond to various ATTACH-ed
** databases.
**
** Space to hold the string that is returned by cortex_db_name() is managed
** by CORTEX itself.  The string might be deallocated by any operation that
** changes the schema, including [ATTACH] or [DETACH] or calls to
** [cortex_serialize()] or [cortex_deserialize()], even operations that
** occur on a different thread.  Applications that need to
** remember the string long-term should make their own copy.  Applications that
** are accessing the same database connection simultaneously on multiple
** threads should mutex-protect calls to this API and should make their own
** private copy of the result prior to releasing the mutex.
*/
CORTEX_API const char *cortex_db_name(cortex *db, int N);

/*
** CAPI3REF: Return The Filename For A Database Connection
** METHOD: cortex
**
** ^The cortex_db_filename(D,N) interface returns a pointer to the filename
** associated with database N of connection D.
** ^If there is no attached database N on the database
** connection D, or if database N is a temporary or in-memory database, then
** this function will return either a NULL pointer or an empty string.
**
** ^The string value returned by this routine is owned and managed by
** the database connection.  ^The value will be valid until the database N
** is [DETACH]-ed or until the database connection closes.
**
** ^The filename returned by this function is the output of the
** xFullPathname method of the [VFS].  ^In other words, the filename
** will be an absolute pathname, even if the filename used
** to open the database originally was a URI or relative pathname.
**
** If the filename pointer returned by this routine is not NULL, then it
** can be used as the filename input parameter to these routines:
** <ul>
** <li> [cortex_uri_parameter()]
** <li> [cortex_uri_boolean()]
** <li> [cortex_uri_int64()]
** <li> [cortex_filename_database()]
** <li> [cortex_filename_journal()]
** <li> [cortex_filename_wal()]
** </ul>
*/
CORTEX_API cortex_filename cortex_db_filename(cortex *db, const char *zDbName);

/*
** CAPI3REF: Determine if a database is read-only
** METHOD: cortex
**
** ^The cortex_db_readonly(D,N) interface returns 1 if the database N
** of connection D is read-only, 0 if it is read/write, or -1 if N is not
** the name of a database on connection D.
*/
CORTEX_API int cortex_db_readonly(cortex *db, const char *zDbName);

/*
** CAPI3REF: Determine the transaction state of a database
** METHOD: cortex
**
** ^The cortex_txn_state(D,S) interface returns the current
** [transaction state] of schema S in database connection D.  ^If S is NULL,
** then the highest transaction state of any schema on database connection D
** is returned.  Transaction states are (in order of lowest to highest):
** <ol>
** <li value="0"> CORTEX_TXN_NONE
** <li value="1"> CORTEX_TXN_READ
** <li value="2"> CORTEX_TXN_WRITE
** </ol>
** ^If the S argument to cortex_txn_state(D,S) is not the name of
** a valid schema, then -1 is returned.
*/
CORTEX_API int cortex_txn_state(cortex*,const char *zSchema);

/*
** CAPI3REF: Allowed return values from cortex_txn_state()
** KEYWORDS: {transaction state}
**
** These constants define the current transaction state of a database file.
** ^The [cortex_txn_state(D,S)] interface returns one of these
** constants in order to describe the transaction state of schema S
** in [database connection] D.
**
** <dl>
** [[CORTEX_TXN_NONE]] <dt>CORTEX_TXN_NONE</dt>
** <dd>The CORTEX_TXN_NONE state means that no transaction is currently
** pending.</dd>
**
** [[CORTEX_TXN_READ]] <dt>CORTEX_TXN_READ</dt>
** <dd>The CORTEX_TXN_READ state means that the database is currently
** in a read transaction.  Content has been read from the database file
** but nothing in the database file has changed.  The transaction state
** will be advanced to CORTEX_TXN_WRITE if any changes occur and there are
** no other conflicting concurrent write transactions.  The transaction
** state will revert to CORTEX_TXN_NONE following a [ROLLBACK] or
** [COMMIT].</dd>
**
** [[CORTEX_TXN_WRITE]] <dt>CORTEX_TXN_WRITE</dt>
** <dd>The CORTEX_TXN_WRITE state means that the database is currently
** in a write transaction.  Content has been written to the database file
** but has not yet committed.  The transaction state will change to
** CORTEX_TXN_NONE at the next [ROLLBACK] or [COMMIT].</dd>
*/
#define CORTEX_TXN_NONE  0
#define CORTEX_TXN_READ  1
#define CORTEX_TXN_WRITE 2

/*
** CAPI3REF: Find the next prepared statement
** METHOD: cortex
**
** ^This interface returns a pointer to the next [prepared statement] after
** pStmt associated with the [database connection] pDb.  ^If pStmt is NULL
** then this interface returns a pointer to the first prepared statement
** associated with the database connection pDb.  ^If no prepared statement
** satisfies the conditions of this routine, it returns NULL.
**
** The [database connection] pointer D in a call to
** [cortex_next_stmt(D,S)] must refer to an open database
** connection and in particular must not be a NULL pointer.
*/
CORTEX_API cortex_stmt *cortex_next_stmt(cortex *pDb, cortex_stmt *pStmt);

/*
** CAPI3REF: Commit And Rollback Notification Callbacks
** METHOD: cortex
**
** ^The cortex_commit_hook() interface registers a callback
** function to be invoked whenever a transaction is [COMMIT | committed].
** ^Any callback set by a previous call to cortex_commit_hook()
** for the same database connection is overridden.
** ^The cortex_rollback_hook() interface registers a callback
** function to be invoked whenever a transaction is [ROLLBACK | rolled back].
** ^Any callback set by a previous call to cortex_rollback_hook()
** for the same database connection is overridden.
** ^The pArg argument is passed through to the callback.
** ^If the callback on a commit hook function returns non-zero,
** then the commit is converted into a rollback.
**
** ^The cortex_commit_hook(D,C,P) and cortex_rollback_hook(D,C,P) functions
** return the P argument from the previous call of the same function
** on the same [database connection] D, or NULL for
** the first call for each function on D.
**
** The commit and rollback hook callbacks are not reentrant.
** The callback implementation must not do anything that will modify
** the database connection that invoked the callback.  Any actions
** to modify the database connection must be deferred until after the
** completion of the [cortex_step()] call that triggered the commit
** or rollback hook in the first place.
** Note that running any other SQL statements, including SELECT statements,
** or merely calling [cortex_prepare_v2()] and [cortex_step()] will modify
** the database connections for the meaning of "modify" in this paragraph.
**
** ^Registering a NULL function disables the callback.
**
** ^When the commit hook callback routine returns zero, the [COMMIT]
** operation is allowed to continue normally.  ^If the commit hook
** returns non-zero, then the [COMMIT] is converted into a [ROLLBACK].
** ^The rollback hook is invoked on a rollback that results from a commit
** hook returning non-zero, just as it would be with any other rollback.
**
** ^For the purposes of this API, a transaction is said to have been
** rolled back if an explicit "ROLLBACK" statement is executed, or
** an error or constraint causes an implicit rollback to occur.
** ^The rollback callback is not invoked if a transaction is
** automatically rolled back because the database connection is closed.
**
** See also the [cortex_update_hook()] interface.
*/
CORTEX_API void *cortex_commit_hook(cortex*, int(*)(void*), void*);
CORTEX_API void *cortex_rollback_hook(cortex*, void(*)(void *), void*);

/*
** CAPI3REF: Autovacuum Compaction Amount Callback
** METHOD: cortex
**
** ^The cortex_autovacuum_pages(D,C,P,X) interface registers a callback
** function C that is invoked prior to each autovacuum of the database
** file.  ^The callback is passed a copy of the generic data pointer (P),
** the schema-name of the attached database that is being autovacuumed,
** the size of the database file in pages, the number of free pages,
** and the number of bytes per page, respectively.  The callback should
** return the number of free pages that should be removed by the
** autovacuum.  ^If the callback returns zero, then no autovacuum happens.
** ^If the value returned is greater than or equal to the number of
** free pages, then a complete autovacuum happens.
**
** <p>^If there are multiple ATTACH-ed database files that are being
** modified as part of a transaction commit, then the autovacuum pages
** callback is invoked separately for each file.
**
** <p><b>The callback is not reentrant.</b> The callback function should
** not attempt to invoke any other CORTEX interface.  If it does, bad
** things may happen, including segmentation faults and corrupt database
** files.  The callback function should be a simple function that
** does some arithmetic on its input parameters and returns a result.
**
** ^The X parameter to cortex_autovacuum_pages(D,C,P,X) is an optional
** destructor for the P parameter.  ^If X is not NULL, then X(P) is
** invoked whenever the database connection closes or when the callback
** is overwritten by another invocation of cortex_autovacuum_pages().
**
** <p>^There is only one autovacuum pages callback per database connection.
** ^Each call to the cortex_autovacuum_pages() interface overrides all
** previous invocations for that database connection.  ^If the callback
** argument (C) to cortex_autovacuum_pages(D,C,P,X) is a NULL pointer,
** then the autovacuum steps callback is canceled.  The return value
** from cortex_autovacuum_pages() is normally CORTEX_OK, but might
** be some other error code if something goes wrong.  The current
** implementation will only return CORTEX_OK or CORTEX_MISUSE, but other
** return codes might be added in future releases.
**
** <p>If no autovacuum pages callback is specified (the usual case) or
** a NULL pointer is provided for the callback,
** then the default behavior is to vacuum all free pages.  So, in other
** words, the default behavior is the same as if the callback function
** were something like this:
**
** <blockquote><pre>
** &nbsp;   unsigned int demonstration_autovac_pages_callback(
** &nbsp;     void *pClientData,
** &nbsp;     const char *zSchema,
** &nbsp;     unsigned int nDbPage,
** &nbsp;     unsigned int nFreePage,
** &nbsp;     unsigned int nBytePerPage
** &nbsp;   ){
** &nbsp;     return nFreePage;
** &nbsp;   }
** </pre></blockquote>
*/
CORTEX_API int cortex_autovacuum_pages(
  cortex *db,
  unsigned int(*)(void*,const char*,unsigned int,unsigned int,unsigned int),
  void*,
  void(*)(void*)
);


/*
** CAPI3REF: Data Change Notification Callbacks
** METHOD: cortex
**
** ^The cortex_update_hook() interface registers a callback function
** with the [database connection] identified by the first argument
** to be invoked whenever a row is updated, inserted or deleted in
** a [rowid table].
** ^Any callback set by a previous call to this function
** for the same database connection is overridden.
**
** ^The second argument is a pointer to the function to invoke when a
** row is updated, inserted or deleted in a rowid table.
** ^The update hook is disabled by invoking cortex_update_hook()
** with a NULL pointer as the second parameter.
** ^The first argument to the callback is a copy of the third argument
** to cortex_update_hook().
** ^The second callback argument is one of [CORTEX_INSERT], [CORTEX_DELETE],
** or [CORTEX_UPDATE], depending on the operation that caused the callback
** to be invoked.
** ^The third and fourth arguments to the callback contain pointers to the
** database and table name containing the affected row.
** ^The final callback parameter is the [rowid] of the row.
** ^In the case of an update, this is the [rowid] after the update takes place.
**
** ^(The update hook is not invoked when internal system tables are
** modified (i.e. CORTEX_sequence).)^
** ^The update hook is not invoked when [WITHOUT ROWID] tables are modified.
**
** ^In the current implementation, the update hook
** is not invoked when conflicting rows are deleted because of an
** [ON CONFLICT | ON CONFLICT REPLACE] clause.  ^Nor is the update hook
** invoked when rows are deleted using the [truncate optimization].
** The exceptions defined in this paragraph might change in a future
** release of SQLite.
**
** Whether the update hook is invoked before or after the
** corresponding change is currently unspecified and may differ
** depending on the type of change. Do not rely on the order of the
** hook call with regards to the final result of the operation which
** triggers the hook.
**
** The update hook implementation must not do anything that will modify
** the database connection that invoked the update hook.  Any actions
** to modify the database connection must be deferred until after the
** completion of the [cortex_step()] call that triggered the update hook.
** Note that [cortex_prepare_v2()] and [cortex_step()] both modify their
** database connections for the meaning of "modify" in this paragraph.
**
** ^The cortex_update_hook(D,C,P) function
** returns the P argument from the previous call
** on the same [database connection] D, or NULL for
** the first call on D.
**
** See also the [cortex_commit_hook()], [cortex_rollback_hook()],
** and [cortex_preupdate_hook()] interfaces.
*/
CORTEX_API void *cortex_update_hook(
  cortex*,
  void(*)(void *,int ,char const *,char const *,cortex_int64),
  void*
);

/*
** CAPI3REF: Enable Or Disable Shared Pager Cache
**
** ^(This routine enables or disables the sharing of the database cache
** and schema data structures between [database connection | connections]
** to the same database. Sharing is enabled if the argument is true
** and disabled if the argument is false.)^
**
** This interface is omitted if CORTEX is compiled with
** [-DCORTEX_OMIT_SHARED_CACHE].  The [-DCORTEX_OMIT_SHARED_CACHE]
** compile-time option is recommended because the
** [use of shared cache mode is discouraged].
**
** ^Cache sharing is enabled and disabled for an entire process.
** This is a change as of CORTEX [version 3.5.0] ([dateof:3.5.0]).
** In prior versions of SQLite,
** sharing was enabled or disabled for each thread separately.
**
** ^(The cache sharing mode set by this interface effects all subsequent
** calls to [cortex_open()], [cortex_open_v2()], and [cortex_open16()].
** Existing database connections continue to use the sharing mode
** that was in effect at the time they were opened.)^
**
** ^(This routine returns [CORTEX_OK] if shared cache was enabled or disabled
** successfully.  An [error code] is returned otherwise.)^
**
** ^Shared cache is disabled by default. It is recommended that it stay
** that way.  In other words, do not use this routine.  This interface
** continues to be provided for historical compatibility, but its use is
** discouraged.  Any use of shared cache is discouraged.  If shared cache
** must be used, it is recommended that shared cache only be enabled for
** individual database connections using the [cortex_open_v2()] interface
** with the [CORTEX_OPEN_SHAREDCACHE] flag.
**
** Note: This method is disabled on MacOS X 10.7 and iOS version 5.0
** and will always return CORTEX_MISUSE. On those systems,
** shared cache mode should be enabled per-database connection via
** [cortex_open_v2()] with [CORTEX_OPEN_SHAREDCACHE].
**
** This interface is threadsafe on processors where writing a
** 32-bit integer is atomic.
**
** See Also:  [SQLite Shared-Cache Mode]
*/
CORTEX_API int cortex_enable_shared_cache(int);

/*
** CAPI3REF: Attempt To Free Heap Memory
**
** ^The cortex_release_memory() interface attempts to free N bytes
** of heap memory by deallocating non-essential memory allocations
** held by the database library.   Memory used to cache database
** pages to improve performance is an example of non-essential memory.
** ^cortex_release_memory() returns the number of bytes actually freed,
** which might be more or less than the amount requested.
** ^The cortex_release_memory() routine is a no-op returning zero
** if CORTEX is not compiled with [CORTEX_ENABLE_MEMORY_MANAGEMENT].
**
** See also: [cortex_db_release_memory()]
*/
CORTEX_API int cortex_release_memory(int);

/*
** CAPI3REF: Free Memory Used By A Database Connection
** METHOD: cortex
**
** ^The cortex_db_release_memory(D) interface attempts to free as much heap
** memory as possible from database connection D. Unlike the
** [cortex_release_memory()] interface, this interface is in effect even
** when the [CORTEX_ENABLE_MEMORY_MANAGEMENT] compile-time option is
** omitted.
**
** See also: [cortex_release_memory()]
*/
CORTEX_API int cortex_db_release_memory(cortex*);

/*
** CAPI3REF: Impose A Limit On Heap Size
**
** These interfaces impose limits on the amount of heap memory that will be
** used by all database connections within a single process.
**
** ^The cortex_soft_heap_limit64() interface sets and/or queries the
** soft limit on the amount of heap memory that may be allocated by SQLite.
** ^SQLite strives to keep heap memory utilization below the soft heap
** limit by reducing the number of pages held in the page cache
** as heap memory usages approaches the limit.
** ^The soft heap limit is "soft" because even though CORTEX strives to stay
** below the limit, it will exceed the limit rather than generate
** an [CORTEX_NOMEM] error.  In other words, the soft heap limit
** is advisory only.
**
** ^The cortex_hard_heap_limit64(N) interface sets a hard upper bound of
** N bytes on the amount of memory that will be allocated.  ^The
** cortex_hard_heap_limit64(N) interface is similar to
** cortex_soft_heap_limit64(N) except that memory allocations will fail
** when the hard heap limit is reached.
**
** ^The return value from both cortex_soft_heap_limit64() and
** cortex_hard_heap_limit64() is the size of
** the heap limit prior to the call, or negative in the case of an
** error.  ^If the argument N is negative
** then no change is made to the heap limit.  Hence, the current
** size of heap limits can be determined by invoking
** cortex_soft_heap_limit64(-1) or cortex_hard_heap_limit(-1).
**
** ^Setting the heap limits to zero disables the heap limiter mechanism.
**
** ^The soft heap limit may not be greater than the hard heap limit.
** ^If the hard heap limit is enabled and if cortex_soft_heap_limit(N)
** is invoked with a value of N that is greater than the hard heap limit,
** the soft heap limit is set to the value of the hard heap limit.
** ^The soft heap limit is automatically enabled whenever the hard heap
** limit is enabled. ^When cortex_hard_heap_limit64(N) is invoked and
** the soft heap limit is outside the range of 1..N, then the soft heap
** limit is set to N.  ^Invoking cortex_soft_heap_limit64(0) when the
** hard heap limit is enabled makes the soft heap limit equal to the
** hard heap limit.
**
** The memory allocation limits can also be adjusted using
** [PRAGMA soft_heap_limit] and [PRAGMA hard_heap_limit].
**
** ^(The heap limits are not enforced in the current implementation
** if one or more of following conditions are true:
**
** <ul>
** <li> The limit value is set to zero.
** <li> Memory accounting is disabled using a combination of the
**      [cortex_config]([CORTEX_CONFIG_MEMSTATUS],...) start-time option and
**      the [CORTEX_DEFAULT_MEMSTATUS] compile-time option.
** <li> An alternative page cache implementation is specified using
**      [cortex_config]([CORTEX_CONFIG_PCACHE2],...).
** <li> The page cache allocates from its own memory pool supplied
**      by [cortex_config]([CORTEX_CONFIG_PAGECACHE],...) rather than
**      from the heap.
** </ul>)^
**
** The circumstances under which CORTEX will enforce the heap limits may
** change in future releases of SQLite.
*/
CORTEX_API cortex_int64 cortex_soft_heap_limit64(cortex_int64 N);
CORTEX_API cortex_int64 cortex_hard_heap_limit64(cortex_int64 N);

/*
** CAPI3REF: Deprecated Soft Heap Limit Interface
** DEPRECATED
**
** This is a deprecated version of the [cortex_soft_heap_limit64()]
** interface.  This routine is provided for historical compatibility
** only.  All new applications should use the
** [cortex_soft_heap_limit64()] interface rather than this one.
*/
CORTEX_API CORTEX_DEPRECATED void cortex_soft_heap_limit(int N);


/*
** CAPI3REF: Extract Metadata About A Column Of A Table
** METHOD: cortex
**
** ^(The cortex_table_column_metadata(X,D,T,C,....) routine returns
** information about column C of table T in database D
** on [database connection] X.)^  ^The cortex_table_column_metadata()
** interface returns CORTEX_OK and fills in the non-NULL pointers in
** the final five arguments with appropriate values if the specified
** column exists.  ^The cortex_table_column_metadata() interface returns
** CORTEX_ERROR if the specified column does not exist.
** ^If the column-name parameter to cortex_table_column_metadata() is a
** NULL pointer, then this routine simply checks for the existence of the
** table and returns CORTEX_OK if the table exists and CORTEX_ERROR if it
** does not.  If the table name parameter T in a call to
** cortex_table_column_metadata(X,D,T,C,...) is NULL then the result is
** undefined behavior.
**
** ^The column is identified by the second, third and fourth parameters to
** this function. ^(The second parameter is either the name of the database
** (i.e. "main", "temp", or an attached database) containing the specified
** table or NULL.)^ ^If it is NULL, then all attached databases are searched
** for the table using the same algorithm used by the database engine to
** resolve unqualified table references.
**
** ^The third and fourth parameters to this function are the table and column
** name of the desired column, respectively.
**
** ^Metadata is returned by writing to the memory locations passed as the 5th
** and subsequent parameters to this function. ^Any of these arguments may be
** NULL, in which case the corresponding element of metadata is omitted.
**
** ^(<blockquote>
** <table border="1">
** <tr><th> Parameter <th> Output<br>Type <th>  Description
**
** <tr><td> 5th <td> const char* <td> Data type
** <tr><td> 6th <td> const char* <td> Name of default collation sequence
** <tr><td> 7th <td> int         <td> True if column has a NOT NULL constraint
** <tr><td> 8th <td> int         <td> True if column is part of the PRIMARY KEY
** <tr><td> 9th <td> int         <td> True if column is [AUTOINCREMENT]
** </table>
** </blockquote>)^
**
** ^The memory pointed to by the character pointers returned for the
** declaration type and collation sequence is valid until the next
** call to any CORTEX API function.
**
** ^If the specified table is actually a view, an [error code] is returned.
**
** ^If the specified column is "rowid", "oid" or "_rowid_" and the table
** is not a [WITHOUT ROWID] table and an
** [INTEGER PRIMARY KEY] column has been explicitly declared, then the output
** parameters are set for the explicitly declared column. ^(If there is no
** [INTEGER PRIMARY KEY] column, then the outputs
** for the [rowid] are set as follows:
**
** <pre>
**     data type: "INTEGER"
**     collation sequence: "BINARY"
**     not null: 0
**     primary key: 1
**     auto increment: 0
** </pre>)^
**
** ^This function causes all database schemas to be read from disk and
** parsed, if that has not already been done, and returns an error if
** any errors are encountered while loading the schema.
*/
CORTEX_API int cortex_table_column_metadata(
  cortex *db,                /* Connection handle */
  const char *zDbName,        /* Database name or NULL */
  const char *zTableName,     /* Table name */
  const char *zColumnName,    /* Column name */
  char const **pzDataType,    /* OUTPUT: Declared data type */
  char const **pzCollSeq,     /* OUTPUT: Collation sequence name */
  int *pNotNull,              /* OUTPUT: True if NOT NULL constraint exists */
  int *pPrimaryKey,           /* OUTPUT: True if column part of PK */
  int *pAutoinc               /* OUTPUT: True if column is auto-increment */
);

/*
** CAPI3REF: Load An Extension
** METHOD: cortex
**
** ^This interface loads an CORTEX extension library from the named file.
**
** ^The cortex_load_extension() interface attempts to load an
** [SQLite extension] library contained in the file zFile.  If
** the file cannot be loaded directly, attempts are made to load
** with various operating-system specific extensions added.
** So for example, if "samplelib" cannot be loaded, then names like
** "samplelib.so" or "samplelib.dylib" or "samplelib.dll" might
** be tried also.
**
** ^The entry point is zProc.
** ^(zProc may be 0, in which case CORTEX will try to come up with an
** entry point name on its own.  It first tries "cortex_extension_init".
** If that does not work, it constructs a name "cortex_X_init" where
** X consists of the lower-case equivalent of all ASCII alphabetic
** characters in the filename from the last "/" to the first following
** "." and omitting any initial "lib".)^
** ^The cortex_load_extension() interface returns
** [CORTEX_OK] on success and [CORTEX_ERROR] if something goes wrong.
** ^If an error occurs and pzErrMsg is not 0, then the
** [cortex_load_extension()] interface shall attempt to
** fill *pzErrMsg with error message text stored in memory
** obtained from [cortex_malloc()]. The calling function
** should free this memory by calling [cortex_free()].
**
** ^Extension loading must be enabled using
** [cortex_enable_load_extension()] or
** [cortex_db_config](db,[CORTEX_DBCONFIG_ENABLE_LOAD_EXTENSION],1,NULL)
** prior to calling this API,
** otherwise an error will be returned.
**
** <b>Security warning:</b> It is recommended that the
** [CORTEX_DBCONFIG_ENABLE_LOAD_EXTENSION] method be used to enable only this
** interface.  The use of the [cortex_enable_load_extension()] interface
** should be avoided.  This will keep the SQL function [load_extension()]
** disabled and prevent SQL injections from giving attackers
** access to extension loading capabilities.
**
** See also the [load_extension() SQL function].
*/
CORTEX_API int cortex_load_extension(
  cortex *db,          /* Load the extension into this database connection */
  const char *zFile,    /* Name of the shared library containing extension */
  const char *zProc,    /* Entry point.  Derived from zFile if 0 */
  char **pzErrMsg       /* Put error message here if not 0 */
);

/*
** CAPI3REF: Enable Or Disable Extension Loading
** METHOD: cortex
**
** ^So as not to open security holes in older applications that are
** unprepared to deal with [extension loading], and as a means of disabling
** [extension loading] while evaluating user-entered SQL, the following API
** is provided to turn the [cortex_load_extension()] mechanism on and off.
**
** ^Extension loading is off by default.
** ^Call the cortex_enable_load_extension() routine with onoff==1
** to turn extension loading on and call it with onoff==0 to turn
** it back off again.
**
** ^This interface enables or disables both the C-API
** [cortex_load_extension()] and the SQL function [load_extension()].
** ^(Use [cortex_db_config](db,[CORTEX_DBCONFIG_ENABLE_LOAD_EXTENSION],..)
** to enable or disable only the C-API.)^
**
** <b>Security warning:</b> It is recommended that extension loading
** be enabled using the [CORTEX_DBCONFIG_ENABLE_LOAD_EXTENSION] method
** rather than this interface, so the [load_extension()] SQL function
** remains disabled. This will prevent SQL injections from giving attackers
** access to extension loading capabilities.
*/
CORTEX_API int cortex_enable_load_extension(cortex *db, int onoff);

/*
** CAPI3REF: Automatically Load Statically Linked Extensions
**
** ^This interface causes the xEntryPoint() function to be invoked for
** each new [database connection] that is created.  The idea here is that
** xEntryPoint() is the entry point for a statically linked [SQLite extension]
** that is to be automatically loaded into all new database connections.
**
** ^(Even though the function prototype shows that xEntryPoint() takes
** no arguments and returns void, CORTEX invokes xEntryPoint() with three
** arguments and expects an integer result as if the signature of the
** entry point were as follows:
**
** <blockquote><pre>
** &nbsp;  int xEntryPoint(
** &nbsp;    cortex *db,
** &nbsp;    const char **pzErrMsg,
** &nbsp;    const struct cortex_api_routines *pThunk
** &nbsp;  );
** </pre></blockquote>)^
**
** If the xEntryPoint routine encounters an error, it should make *pzErrMsg
** point to an appropriate error message (obtained from [cortex_mprintf()])
** and return an appropriate [error code].  ^SQLite ensures that *pzErrMsg
** is NULL before calling the xEntryPoint().  ^SQLite will invoke
** [cortex_free()] on *pzErrMsg after xEntryPoint() returns.  ^If any
** xEntryPoint() returns an error, the [cortex_open()], [cortex_open16()],
** or [cortex_open_v2()] call that provoked the xEntryPoint() will fail.
**
** ^Calling cortex_auto_extension(X) with an entry point X that is already
** on the list of automatic extensions is a harmless no-op. ^No entry point
** will be called more than once for each database connection that is opened.
**
** See also: [cortex_reset_auto_extension()]
** and [cortex_cancel_auto_extension()]
*/
CORTEX_API int cortex_auto_extension(void(*xEntryPoint)(void));

/*
** CAPI3REF: Cancel Automatic Extension Loading
**
** ^The [cortex_cancel_auto_extension(X)] interface unregisters the
** initialization routine X that was registered using a prior call to
** [cortex_auto_extension(X)].  ^The [cortex_cancel_auto_extension(X)]
** routine returns 1 if initialization routine X was successfully
** unregistered and it returns 0 if X was not on the list of initialization
** routines.
*/
CORTEX_API int cortex_cancel_auto_extension(void(*xEntryPoint)(void));

/*
** CAPI3REF: Reset Automatic Extension Loading
**
** ^This interface disables all automatic extensions previously
** registered using [cortex_auto_extension()].
*/
CORTEX_API void cortex_reset_auto_extension(void);

/*
** Structures used by the virtual table interface
*/
typedef struct cortex_vtab cortex_vtab;
typedef struct cortex_index_info cortex_index_info;
typedef struct cortex_vtab_cursor cortex_vtab_cursor;
typedef struct cortex_module cortex_module;

/*
** CAPI3REF: Virtual Table Object
** KEYWORDS: cortex_module {virtual table module}
**
** This structure, sometimes called a "virtual table module",
** defines the implementation of a [virtual table].
** This structure consists mostly of methods for the module.
**
** ^A virtual table module is created by filling in a persistent
** instance of this structure and passing a pointer to that instance
** to [cortex_create_module()] or [cortex_create_module_v2()].
** ^The registration remains valid until it is replaced by a different
** module or until the [database connection] closes.  The content
** of this structure must not change while it is registered with
** any database connection.
*/
struct cortex_module {
  int iVersion;
  int (*xCreate)(cortex*, void *pAux,
               int argc, const char *const*argv,
               cortex_vtab **ppVTab, char**);
  int (*xConnect)(cortex*, void *pAux,
               int argc, const char *const*argv,
               cortex_vtab **ppVTab, char**);
  int (*xBestIndex)(cortex_vtab *pVTab, cortex_index_info*);
  int (*xDisconnect)(cortex_vtab *pVTab);
  int (*xDestroy)(cortex_vtab *pVTab);
  int (*xOpen)(cortex_vtab *pVTab, cortex_vtab_cursor **ppCursor);
  int (*xClose)(cortex_vtab_cursor*);
  int (*xFilter)(cortex_vtab_cursor*, int idxNum, const char *idxStr,
                int argc, cortex_value **argv);
  int (*xNext)(cortex_vtab_cursor*);
  int (*xEof)(cortex_vtab_cursor*);
  int (*xColumn)(cortex_vtab_cursor*, cortex_context*, int);
  int (*xRowid)(cortex_vtab_cursor*, cortex_int64 *pRowid);
  int (*xUpdate)(cortex_vtab *, int, cortex_value **, cortex_int64 *);
  int (*xBegin)(cortex_vtab *pVTab);
  int (*xSync)(cortex_vtab *pVTab);
  int (*xCommit)(cortex_vtab *pVTab);
  int (*xRollback)(cortex_vtab *pVTab);
  int (*xFindFunction)(cortex_vtab *pVtab, int nArg, const char *zName,
                       void (**pxFunc)(cortex_context*,int,cortex_value**),
                       void **ppArg);
  int (*xRename)(cortex_vtab *pVtab, const char *zNew);
  /* The methods above are in version 1 of the CORTEX_module object. Those
  ** below are for version 2 and greater. */
  int (*xSavepoint)(cortex_vtab *pVTab, int);
  int (*xRelease)(cortex_vtab *pVTab, int);
  int (*xRollbackTo)(cortex_vtab *pVTab, int);
  /* The methods above are in versions 1 and 2 of the CORTEX_module object.
  ** Those below are for version 3 and greater. */
  int (*xShadowName)(const char*);
  /* The methods above are in versions 1 through 3 of the CORTEX_module object.
  ** Those below are for version 4 and greater. */
  int (*xIntegrity)(cortex_vtab *pVTab, const char *zSchema,
                    const char *zTabName, int mFlags, char **pzErr);
};

/*
** CAPI3REF: Virtual Table Indexing Information
** KEYWORDS: cortex_index_info
**
** The cortex_index_info structure and its substructures is used as part
** of the [virtual table] interface to
** pass information into and receive the reply from the [xBestIndex]
** method of a [virtual table module].  The fields under **Inputs** are the
** inputs to xBestIndex and are read-only.  xBestIndex inserts its
** results into the **Outputs** fields.
**
** ^(The aConstraint[] array records WHERE clause constraints of the form:
**
** <blockquote>column OP expr</blockquote>
**
** where OP is =, &lt;, &lt;=, &gt;, or &gt;=.)^  ^(The particular operator is
** stored in aConstraint[].op using one of the
** [CORTEX_INDEX_CONSTRAINT_EQ | CORTEX_INDEX_CONSTRAINT_ values].)^
** ^(The index of the column is stored in
** aConstraint[].iColumn.)^  ^(aConstraint[].usable is TRUE if the
** expr on the right-hand side can be evaluated (and thus the constraint
** is usable) and false if it cannot.)^
**
** ^The optimizer automatically inverts terms of the form "expr OP column"
** and makes other simplifications to the WHERE clause in an attempt to
** get as many WHERE clause terms into the form shown above as possible.
** ^The aConstraint[] array only reports WHERE clause terms that are
** relevant to the particular virtual table being queried.
**
** ^Information about the ORDER BY clause is stored in aOrderBy[].
** ^Each term of aOrderBy records a column of the ORDER BY clause.
**
** The colUsed field indicates which columns of the virtual table may be
** required by the current scan. Virtual table columns are numbered from
** zero in the order in which they appear within the CREATE TABLE statement
** passed to cortex_declare_vtab(). For the first 63 columns (columns 0-62),
** the corresponding bit is set within the colUsed mask if the column may be
** required by SQLite. If the table has at least 64 columns and any column
** to the right of the first 63 is required, then bit 63 of colUsed is also
** set. In other words, column iCol may be required if the expression
** (colUsed & ((cortex_uint64)1 << (iCol>=63 ? 63 : iCol))) evaluates to
** non-zero.
**
** The [xBestIndex] method must fill aConstraintUsage[] with information
** about what parameters to pass to xFilter.  ^If argvIndex>0 then
** the right-hand side of the corresponding aConstraint[] is evaluated
** and becomes the argvIndex-th entry in argv.  ^(If aConstraintUsage[].omit
** is true, then the constraint is assumed to be fully handled by the
** virtual table and might not be checked again by the byte code.)^ ^(The
** aConstraintUsage[].omit flag is an optimization hint. When the omit flag
** is left in its default setting of false, the constraint will always be
** checked separately in byte code.  If the omit flag is changed to true, then
** the constraint may or may not be checked in byte code.  In other words,
** when the omit flag is true there is no guarantee that the constraint will
** not be checked again using byte code.)^
**
** ^The idxNum and idxStr values are recorded and passed into the
** [xFilter] method.
** ^[cortex_free()] is used to free idxStr if and only if
** needToFreeIdxStr is true.
**
** ^The orderByConsumed means that output from [xFilter]/[xNext] will occur in
** the correct order to satisfy the ORDER BY clause so that no separate
** sorting step is required.
**
** ^The estimatedCost value is an estimate of the cost of a particular
** strategy. A cost of N indicates that the cost of the strategy is similar
** to a linear scan of an CORTEX table with N rows. A cost of log(N)
** indicates that the expense of the operation is similar to that of a
** binary search on a unique indexed field of an CORTEX table with N rows.
**
** ^The estimatedRows value is an estimate of the number of rows that
** will be returned by the strategy.
**
** The xBestIndex method may optionally populate the idxFlags field with a
** mask of CORTEX_INDEX_SCAN_* flags. One such flag is
** [CORTEX_INDEX_SCAN_HEX], which if set causes the [EXPLAIN QUERY PLAN]
** output to show the idxNum as hex instead of as decimal.  Another flag is
** CORTEX_INDEX_SCAN_UNIQUE, which if set indicates that the query plan will
** return at most one row.
**
** Additionally, if xBestIndex sets the CORTEX_INDEX_SCAN_UNIQUE flag, then
** CORTEX also assumes that if a call to the xUpdate() method is made as
** part of the same statement to delete or update a virtual table row and the
** implementation returns CORTEX_CONSTRAINT, then there is no need to rollback
** any database changes. In other words, if the xUpdate() returns
** CORTEX_CONSTRAINT, the database contents must be exactly as they were
** before xUpdate was called. By contrast, if CORTEX_INDEX_SCAN_UNIQUE is not
** set and xUpdate returns CORTEX_CONSTRAINT, any database changes made by
** the xUpdate method are automatically rolled back by SQLite.
**
** IMPORTANT: The estimatedRows field was added to the cortex_index_info
** structure for CORTEX [version 3.8.2] ([dateof:3.8.2]).
** If a virtual table extension is
** used with an CORTEX version earlier than 3.8.2, the results of attempting
** to read or write the estimatedRows field are undefined (but are likely
** to include crashing the application). The estimatedRows field should
** therefore only be used if [cortex_libversion_number()] returns a
** value greater than or equal to 3008002. Similarly, the idxFlags field
** was added for [version 3.9.0] ([dateof:3.9.0]).
** It may therefore only be used if
** cortex_libversion_number() returns a value greater than or equal to
** 3009000.
*/
struct cortex_index_info {
  /* Inputs */
  int nConstraint;           /* Number of entries in aConstraint */
  struct cortex_index_constraint {
     int iColumn;              /* Column constrained.  -1 for ROWID */
     unsigned char op;         /* Constraint operator */
     unsigned char usable;     /* True if this constraint is usable */
     int iTermOffset;          /* Used internally - xBestIndex should ignore */
  } *aConstraint;            /* Table of WHERE clause constraints */
  int nOrderBy;              /* Number of terms in the ORDER BY clause */
  struct cortex_index_orderby {
     int iColumn;              /* Column number */
     unsigned char desc;       /* True for DESC.  False for ASC. */
  } *aOrderBy;               /* The ORDER BY clause */
  /* Outputs */
  struct cortex_index_constraint_usage {
    int argvIndex;           /* if >0, constraint is part of argv to xFilter */
    unsigned char omit;      /* Do not code a test for this constraint */
  } *aConstraintUsage;
  int idxNum;                /* Number used to identify the index */
  char *idxStr;              /* String, possibly obtained from cortex_malloc */
  int needToFreeIdxStr;      /* Free idxStr using cortex_free() if true */
  int orderByConsumed;       /* True if output is already ordered */
  double estimatedCost;           /* Estimated cost of using this index */
  /* Fields below are only available in CORTEX 3.8.2 and later */
  cortex_int64 estimatedRows;    /* Estimated number of rows returned */
  /* Fields below are only available in CORTEX 3.9.0 and later */
  int idxFlags;              /* Mask of CORTEX_INDEX_SCAN_* flags */
  /* Fields below are only available in CORTEX 3.10.0 and later */
  cortex_uint64 colUsed;    /* Input: Mask of columns used by statement */
};

/*
** CAPI3REF: Virtual Table Scan Flags
**
** Virtual table implementations are allowed to set the
** [cortex_index_info].idxFlags field to some combination of
** these bits.
*/
#define CORTEX_INDEX_SCAN_UNIQUE 0x00000001 /* Scan visits at most 1 row */
#define CORTEX_INDEX_SCAN_HEX    0x00000002 /* Display idxNum as hex */
                                            /* in EXPLAIN QUERY PLAN */

/*
** CAPI3REF: Virtual Table Constraint Operator Codes
**
** These macros define the allowed values for the
** [cortex_index_info].aConstraint[].op field.  Each value represents
** an operator that is part of a constraint term in the WHERE clause of
** a query that uses a [virtual table].
**
** ^The left-hand operand of the operator is given by the corresponding
** aConstraint[].iColumn field.  ^An iColumn of -1 indicates the left-hand
** operand is the rowid.
** The CORTEX_INDEX_CONSTRAINT_LIMIT and CORTEX_INDEX_CONSTRAINT_OFFSET
** operators have no left-hand operand, and so for those operators the
** corresponding aConstraint[].iColumn is meaningless and should not be
** used.
**
** All operator values from CORTEX_INDEX_CONSTRAINT_FUNCTION through
** value 255 are reserved to represent functions that are overloaded
** by the [xFindFunction|xFindFunction method] of the virtual table
** implementation.
**
** The right-hand operands for each constraint might be accessible using
** the [cortex_vtab_rhs_value()] interface.  Usually the right-hand
** operand is only available if it appears as a single constant literal
** in the input SQL.  If the right-hand operand is another column or an
** expression (even a constant expression) or a parameter, then the
** cortex_vtab_rhs_value() probably will not be able to extract it.
** ^The CORTEX_INDEX_CONSTRAINT_ISNULL and
** CORTEX_INDEX_CONSTRAINT_ISNOTNULL operators have no right-hand operand
** and hence calls to cortex_vtab_rhs_value() for those operators will
** always return CORTEX_NOTFOUND.
**
** The collating sequence to be used for comparison can be found using
** the [cortex_vtab_collation()] interface.  For most real-world virtual
** tables, the collating sequence of constraints does not matter (for example
** because the constraints are numeric) and so the cortex_vtab_collation()
** interface is not commonly needed.
*/
#define CORTEX_INDEX_CONSTRAINT_EQ          2
#define CORTEX_INDEX_CONSTRAINT_GT          4
#define CORTEX_INDEX_CONSTRAINT_LE          8
#define CORTEX_INDEX_CONSTRAINT_LT         16
#define CORTEX_INDEX_CONSTRAINT_GE         32
#define CORTEX_INDEX_CONSTRAINT_MATCH      64
#define CORTEX_INDEX_CONSTRAINT_LIKE       65
#define CORTEX_INDEX_CONSTRAINT_GLOB       66
#define CORTEX_INDEX_CONSTRAINT_REGEXP     67
#define CORTEX_INDEX_CONSTRAINT_NE         68
#define CORTEX_INDEX_CONSTRAINT_ISNOT      69
#define CORTEX_INDEX_CONSTRAINT_ISNOTNULL  70
#define CORTEX_INDEX_CONSTRAINT_ISNULL     71
#define CORTEX_INDEX_CONSTRAINT_IS         72
#define CORTEX_INDEX_CONSTRAINT_LIMIT      73
#define CORTEX_INDEX_CONSTRAINT_OFFSET     74
#define CORTEX_INDEX_CONSTRAINT_FUNCTION  150

/*
** CAPI3REF: Register A Virtual Table Implementation
** METHOD: cortex
**
** ^These routines are used to register a new [virtual table module] name.
** ^Module names must be registered before
** creating a new [virtual table] using the module and before using a
** preexisting [virtual table] for the module.
**
** ^The module name is registered on the [database connection] specified
** by the first parameter.  ^The name of the module is given by the
** second parameter.  ^The third parameter is a pointer to
** the implementation of the [virtual table module].   ^The fourth
** parameter is an arbitrary client data pointer that is passed through
** into the [xCreate] and [xConnect] methods of the virtual table module
** when a new virtual table is being created or reinitialized.
**
** ^The cortex_create_module_v2() interface has a fifth parameter which
** is a pointer to a destructor for the pClientData.  ^SQLite will
** invoke the destructor function (if it is not NULL) when SQLite
** no longer needs the pClientData pointer.  ^The destructor will also
** be invoked if the call to cortex_create_module_v2() fails.
** ^The cortex_create_module()
** interface is equivalent to cortex_create_module_v2() with a NULL
** destructor.
**
** ^If the third parameter (the pointer to the cortex_module object) is
** NULL then no new module is created and any existing modules with the
** same name are dropped.
**
** See also: [cortex_drop_modules()]
*/
CORTEX_API int cortex_create_module(
  cortex *db,               /* CORTEX connection to register module with */
  const char *zName,         /* Name of the module */
  const cortex_module *p,   /* Methods for the module */
  void *pClientData          /* Client data for xCreate/xConnect */
);
CORTEX_API int cortex_create_module_v2(
  cortex *db,               /* CORTEX connection to register module with */
  const char *zName,         /* Name of the module */
  const cortex_module *p,   /* Methods for the module */
  void *pClientData,         /* Client data for xCreate/xConnect */
  void(*xDestroy)(void*)     /* Module destructor function */
);

/*
** CAPI3REF: Remove Unnecessary Virtual Table Implementations
** METHOD: cortex
**
** ^The cortex_drop_modules(D,L) interface removes all virtual
** table modules from database connection D except those named on list L.
** The L parameter must be either NULL or a pointer to an array of pointers
** to strings where the array is terminated by a single NULL pointer.
** ^If the L parameter is NULL, then all virtual table modules are removed.
**
** See also: [cortex_create_module()]
*/
CORTEX_API int cortex_drop_modules(
  cortex *db,                /* Remove modules from this connection */
  const char **azKeep         /* Except, do not remove the ones named here */
);

/*
** CAPI3REF: Virtual Table Instance Object
** KEYWORDS: cortex_vtab
**
** Every [virtual table module] implementation uses a subclass
** of this object to describe a particular instance
** of the [virtual table].  Each subclass will
** be tailored to the specific needs of the module implementation.
** The purpose of this superclass is to define certain fields that are
** common to all module implementations.
**
** ^Virtual tables methods can set an error message by assigning a
** string obtained from [cortex_mprintf()] to zErrMsg.  The method should
** take care that any prior string is freed by a call to [cortex_free()]
** prior to assigning a new string to zErrMsg.  ^After the error message
** is delivered up to the client application, the string will be automatically
** freed by cortex_free() and the zErrMsg field will be zeroed.
*/
struct cortex_vtab {
  const cortex_module *pModule;  /* The module for this virtual table */
  int nRef;                       /* Number of open cursors */
  char *zErrMsg;                  /* Error message from cortex_mprintf() */
  /* Virtual table implementations will typically add additional fields */
};

/*
** CAPI3REF: Virtual Table Cursor Object
** KEYWORDS: cortex_vtab_cursor {virtual table cursor}
**
** Every [virtual table module] implementation uses a subclass of the
** following structure to describe cursors that point into the
** [virtual table] and are used
** to loop through the virtual table.  Cursors are created using the
** [cortex_module.xOpen | xOpen] method of the module and are destroyed
** by the [cortex_module.xClose | xClose] method.  Cursors are used
** by the [xFilter], [xNext], [xEof], [xColumn], and [xRowid] methods
** of the module.  Each module implementation will define
** the content of a cursor structure to suit its own needs.
**
** This superclass exists in order to define fields of the cursor that
** are common to all implementations.
*/
struct cortex_vtab_cursor {
  cortex_vtab *pVtab;      /* Virtual table of this cursor */
  /* Virtual table implementations will typically add additional fields */
};

/*
** CAPI3REF: Declare The Schema Of A Virtual Table
**
** ^The [xCreate] and [xConnect] methods of a
** [virtual table module] call this interface
** to declare the format (the names and datatypes of the columns) of
** the virtual tables they implement.
*/
CORTEX_API int cortex_declare_vtab(cortex*, const char *zSQL);

/*
** CAPI3REF: Overload A Function For A Virtual Table
** METHOD: cortex
**
** ^(Virtual tables can provide alternative implementations of functions
** using the [xFindFunction] method of the [virtual table module].
** But global versions of those functions
** must exist in order to be overloaded.)^
**
** ^(This API makes sure a global version of a function with a particular
** name and number of parameters exists.  If no such function exists
** before this API is called, a new function is created.)^  ^The implementation
** of the new function always causes an exception to be thrown.  So
** the new function is not good for anything by itself.  Its only
** purpose is to be a placeholder function that can be overloaded
** by a [virtual table].
*/
CORTEX_API int cortex_overload_function(cortex*, const char *zFuncName, int nArg);

/*
** CAPI3REF: A Handle To An Open BLOB
** KEYWORDS: {BLOB handle} {BLOB handles}
**
** An instance of this object represents an open BLOB on which
** [cortex_blob_open | incremental BLOB I/O] can be performed.
** ^Objects of this type are created by [cortex_blob_open()]
** and destroyed by [cortex_blob_close()].
** ^The [cortex_blob_read()] and [cortex_blob_write()] interfaces
** can be used to read or write small subsections of the BLOB.
** ^The [cortex_blob_bytes()] interface returns the size of the BLOB in bytes.
*/
typedef struct cortex_blob cortex_blob;

/*
** CAPI3REF: Open A BLOB For Incremental I/O
** METHOD: cortex
** CONSTRUCTOR: cortex_blob
**
** ^(This interfaces opens a [BLOB handle | handle] to the BLOB located
** in row iRow, column zColumn, table zTable in database zDb;
** in other words, the same BLOB that would be selected by:
**
** <pre>
**     SELECT zColumn FROM zDb.zTable WHERE [rowid] = iRow;
** </pre>)^
**
** ^(Parameter zDb is not the filename that contains the database, but
** rather the symbolic name of the database. For attached databases, this is
** the name that appears after the AS keyword in the [ATTACH] statement.
** For the main database file, the database name is "main". For TEMP
** tables, the database name is "temp".)^
**
** ^If the flags parameter is non-zero, then the BLOB is opened for read
** and write access. ^If the flags parameter is zero, the BLOB is opened for
** read-only access.
**
** ^(On success, [CORTEX_OK] is returned and the new [BLOB handle] is stored
** in *ppBlob. Otherwise an [error code] is returned and, unless the error
** code is CORTEX_MISUSE, *ppBlob is set to NULL.)^ ^This means that, provided
** the API is not misused, it is always safe to call [cortex_blob_close()]
** on *ppBlob after this function returns.
**
** This function fails with CORTEX_ERROR if any of the following are true:
** <ul>
**   <li> ^(Database zDb does not exist)^,
**   <li> ^(Table zTable does not exist within database zDb)^,
**   <li> ^(Table zTable is a WITHOUT ROWID table)^,
**   <li> ^(Column zColumn does not exist)^,
**   <li> ^(Row iRow is not present in the table)^,
**   <li> ^(The specified column of row iRow contains a value that is not
**         a TEXT or BLOB value)^,
**   <li> ^(Column zColumn is part of an index, PRIMARY KEY or UNIQUE
**         constraint and the blob is being opened for read/write access)^,
**   <li> ^([foreign key constraints | Foreign key constraints] are enabled,
**         column zColumn is part of a [child key] definition and the blob is
**         being opened for read/write access)^.
** </ul>
**
** ^Unless it returns CORTEX_MISUSE, this function sets the
** [database connection] error code and message accessible via
** [cortex_errcode()] and [cortex_errmsg()] and related functions.
**
** A BLOB referenced by cortex_blob_open() may be read using the
** [cortex_blob_read()] interface and modified by using
** [cortex_blob_write()].  The [BLOB handle] can be moved to a
** different row of the same table using the [cortex_blob_reopen()]
** interface.  However, the column, table, or database of a [BLOB handle]
** cannot be changed after the [BLOB handle] is opened.
**
** ^(If the row that a BLOB handle points to is modified by an
** [UPDATE], [DELETE], or by [ON CONFLICT] side-effects
** then the BLOB handle is marked as "expired".
** This is true if any column of the row is changed, even a column
** other than the one the BLOB handle is open on.)^
** ^Calls to [cortex_blob_read()] and [cortex_blob_write()] for
** an expired BLOB handle fail with a return code of [CORTEX_ABORT].
** ^(Changes written into a BLOB prior to the BLOB expiring are not
** rolled back by the expiration of the BLOB.  Such changes will eventually
** commit if the transaction continues to completion.)^
**
** ^Use the [cortex_blob_bytes()] interface to determine the size of
** the opened blob.  ^The size of a blob may not be changed by this
** interface.  Use the [UPDATE] SQL command to change the size of a
** blob.
**
** ^The [cortex_bind_zeroblob()] and [cortex_result_zeroblob()] interfaces
** and the built-in [zeroblob] SQL function may be used to create a
** zero-filled blob to read or write using the incremental-blob interface.
**
** To avoid a resource leak, every open [BLOB handle] should eventually
** be released by a call to [cortex_blob_close()].
**
** See also: [cortex_blob_close()],
** [cortex_blob_reopen()], [cortex_blob_read()],
** [cortex_blob_bytes()], [cortex_blob_write()].
*/
CORTEX_API int cortex_blob_open(
  cortex*,
  const char *zDb,
  const char *zTable,
  const char *zColumn,
  cortex_int64 iRow,
  int flags,
  cortex_blob **ppBlob
);

/*
** CAPI3REF: Move a BLOB Handle to a New Row
** METHOD: cortex_blob
**
** ^This function is used to move an existing [BLOB handle] so that it points
** to a different row of the same database table. ^The new row is identified
** by the rowid value passed as the second argument. Only the row can be
** changed. ^The database, table and column on which the blob handle is open
** remain the same. Moving an existing [BLOB handle] to a new row is
** faster than closing the existing handle and opening a new one.
**
** ^(The new row must meet the same criteria as for [cortex_blob_open()] -
** it must exist and there must be either a blob or text value stored in
** the nominated column.)^ ^If the new row is not present in the table, or if
** it does not contain a blob or text value, or if another error occurs, an
** CORTEX error code is returned and the blob handle is considered aborted.
** ^All subsequent calls to [cortex_blob_read()], [cortex_blob_write()] or
** [cortex_blob_reopen()] on an aborted blob handle immediately return
** CORTEX_ABORT. ^Calling [cortex_blob_bytes()] on an aborted blob handle
** always returns zero.
**
** ^This function sets the database handle error code and message.
*/
CORTEX_API int cortex_blob_reopen(cortex_blob *, cortex_int64);

/*
** CAPI3REF: Close A BLOB Handle
** DESTRUCTOR: cortex_blob
**
** ^This function closes an open [BLOB handle]. ^(The BLOB handle is closed
** unconditionally.  Even if this routine returns an error code, the
** handle is still closed.)^
**
** ^If the blob handle being closed was opened for read-write access, and if
** the database is in auto-commit mode and there are no other open read-write
** blob handles or active write statements, the current transaction is
** committed. ^If an error occurs while committing the transaction, an error
** code is returned and the transaction rolled back.
**
** Calling this function with an argument that is not a NULL pointer or an
** open blob handle results in undefined behavior. ^Calling this routine
** with a null pointer (such as would be returned by a failed call to
** [cortex_blob_open()]) is a harmless no-op. ^Otherwise, if this function
** is passed a valid open blob handle, the values returned by the
** cortex_errcode() and cortex_errmsg() functions are set before returning.
*/
CORTEX_API int cortex_blob_close(cortex_blob *);

/*
** CAPI3REF: Return The Size Of An Open BLOB
** METHOD: cortex_blob
**
** ^Returns the size in bytes of the BLOB accessible via the
** successfully opened [BLOB handle] in its only argument.  ^The
** incremental blob I/O routines can only read or overwrite existing
** blob content; they cannot change the size of a blob.
**
** This routine only works on a [BLOB handle] which has been created
** by a prior successful call to [cortex_blob_open()] and which has not
** been closed by [cortex_blob_close()].  Passing any other pointer in
** to this routine results in undefined and probably undesirable behavior.
*/
CORTEX_API int cortex_blob_bytes(cortex_blob *);

/*
** CAPI3REF: Read Data From A BLOB Incrementally
** METHOD: cortex_blob
**
** ^(This function is used to read data from an open [BLOB handle] into a
** caller-supplied buffer. N bytes of data are copied into buffer Z
** from the open BLOB, starting at offset iOffset.)^
**
** ^If offset iOffset is less than N bytes from the end of the BLOB,
** [CORTEX_ERROR] is returned and no data is read.  ^If N or iOffset is
** less than zero, [CORTEX_ERROR] is returned and no data is read.
** ^The size of the blob (and hence the maximum value of N+iOffset)
** can be determined using the [cortex_blob_bytes()] interface.
**
** ^An attempt to read from an expired [BLOB handle] fails with an
** error code of [CORTEX_ABORT].
**
** ^(On success, cortex_blob_read() returns CORTEX_OK.
** Otherwise, an [error code] or an [extended error code] is returned.)^
**
** This routine only works on a [BLOB handle] which has been created
** by a prior successful call to [cortex_blob_open()] and which has not
** been closed by [cortex_blob_close()].  Passing any other pointer in
** to this routine results in undefined and probably undesirable behavior.
**
** See also: [cortex_blob_write()].
*/
CORTEX_API int cortex_blob_read(cortex_blob *, void *Z, int N, int iOffset);

/*
** CAPI3REF: Write Data Into A BLOB Incrementally
** METHOD: cortex_blob
**
** ^(This function is used to write data into an open [BLOB handle] from a
** caller-supplied buffer. N bytes of data are copied from the buffer Z
** into the open BLOB, starting at offset iOffset.)^
**
** ^(On success, cortex_blob_write() returns CORTEX_OK.
** Otherwise, an  [error code] or an [extended error code] is returned.)^
** ^Unless CORTEX_MISUSE is returned, this function sets the
** [database connection] error code and message accessible via
** [cortex_errcode()] and [cortex_errmsg()] and related functions.
**
** ^If the [BLOB handle] passed as the first argument was not opened for
** writing (the flags parameter to [cortex_blob_open()] was zero),
** this function returns [CORTEX_READONLY].
**
** This function may only modify the contents of the BLOB; it is
** not possible to increase the size of a BLOB using this API.
** ^If offset iOffset is less than N bytes from the end of the BLOB,
** [CORTEX_ERROR] is returned and no data is written. The size of the
** BLOB (and hence the maximum value of N+iOffset) can be determined
** using the [cortex_blob_bytes()] interface. ^If N or iOffset are less
** than zero [CORTEX_ERROR] is returned and no data is written.
**
** ^An attempt to write to an expired [BLOB handle] fails with an
** error code of [CORTEX_ABORT].  ^Writes to the BLOB that occurred
** before the [BLOB handle] expired are not rolled back by the
** expiration of the handle, though of course those changes might
** have been overwritten by the statement that expired the BLOB handle
** or by other independent statements.
**
** This routine only works on a [BLOB handle] which has been created
** by a prior successful call to [cortex_blob_open()] and which has not
** been closed by [cortex_blob_close()].  Passing any other pointer in
** to this routine results in undefined and probably undesirable behavior.
**
** See also: [cortex_blob_read()].
*/
CORTEX_API int cortex_blob_write(cortex_blob *, const void *z, int n, int iOffset);

/*
** CAPI3REF: Virtual File System Objects
**
** A virtual filesystem (VFS) is an [cortex_vfs] object
** that CORTEX uses to interact
** with the underlying operating system.  Most CORTEX builds come with a
** single default VFS that is appropriate for the host computer.
** New VFSes can be registered and existing VFSes can be unregistered.
** The following interfaces are provided.
**
** ^The cortex_vfs_find() interface returns a pointer to a VFS given its name.
** ^Names are case sensitive.
** ^Names are zero-terminated UTF-8 strings.
** ^If there is no match, a NULL pointer is returned.
** ^If zVfsName is NULL then the default VFS is returned.
**
** ^New VFSes are registered with cortex_vfs_register().
** ^Each new VFS becomes the default VFS if the makeDflt flag is set.
** ^The same VFS can be registered multiple times without injury.
** ^To make an existing VFS into the default VFS, register it again
** with the makeDflt flag set.  If two different VFSes with the
** same name are registered, the behavior is undefined.  If a
** VFS is registered with a name that is NULL or an empty string,
** then the behavior is undefined.
**
** ^Unregister a VFS with the cortex_vfs_unregister() interface.
** ^(If the default VFS is unregistered, another VFS is chosen as
** the default.  The choice for the new VFS is arbitrary.)^
*/
CORTEX_API cortex_vfs *cortex_vfs_find(const char *zVfsName);
CORTEX_API int cortex_vfs_register(cortex_vfs*, int makeDflt);
CORTEX_API int cortex_vfs_unregister(cortex_vfs*);

/*
** CAPI3REF: Mutexes
**
** The CORTEX core uses these routines for thread
** synchronization. Though they are intended for internal
** use by SQLite, code that links against CORTEX is
** permitted to use any of these routines.
**
** The CORTEX source code contains multiple implementations
** of these mutex routines.  An appropriate implementation
** is selected automatically at compile-time.  The following
** implementations are available in the CORTEX core:
**
** <ul>
** <li>   CORTEX_MUTEX_PTHREADS
** <li>   CORTEX_MUTEX_W32
** <li>   CORTEX_MUTEX_NOOP
** </ul>
**
** The CORTEX_MUTEX_NOOP implementation is a set of routines
** that does no real locking and is appropriate for use in
** a single-threaded application.  The CORTEX_MUTEX_PTHREADS and
** CORTEX_MUTEX_W32 implementations are appropriate for use on Unix
** and Windows.
**
** If CORTEX is compiled with the CORTEX_MUTEX_APPDEF preprocessor
** macro defined (with "-DCORTEX_MUTEX_APPDEF=1"), then no mutex
** implementation is included with the library. In this case the
** application must supply a custom mutex implementation using the
** [CORTEX_CONFIG_MUTEX] option of the cortex_config() function
** before calling cortex_initialize() or any other public cortex_
** function that calls cortex_initialize().
**
** ^The cortex_mutex_alloc() routine allocates a new
** mutex and returns a pointer to it. ^The cortex_mutex_alloc()
** routine returns NULL if it is unable to allocate the requested
** mutex.  The argument to cortex_mutex_alloc() must be one of these
** integer constants:
**
** <ul>
** <li>  CORTEX_MUTEX_FAST
** <li>  CORTEX_MUTEX_RECURSIVE
** <li>  CORTEX_MUTEX_STATIC_MAIN
** <li>  CORTEX_MUTEX_STATIC_MEM
** <li>  CORTEX_MUTEX_STATIC_OPEN
** <li>  CORTEX_MUTEX_STATIC_PRNG
** <li>  CORTEX_MUTEX_STATIC_LRU
** <li>  CORTEX_MUTEX_STATIC_PMEM
** <li>  CORTEX_MUTEX_STATIC_APP1
** <li>  CORTEX_MUTEX_STATIC_APP2
** <li>  CORTEX_MUTEX_STATIC_APP3
** <li>  CORTEX_MUTEX_STATIC_VFS1
** <li>  CORTEX_MUTEX_STATIC_VFS2
** <li>  CORTEX_MUTEX_STATIC_VFS3
** </ul>
**
** ^The first two constants (CORTEX_MUTEX_FAST and CORTEX_MUTEX_RECURSIVE)
** cause cortex_mutex_alloc() to create
** a new mutex.  ^The new mutex is recursive when CORTEX_MUTEX_RECURSIVE
** is used but not necessarily so when CORTEX_MUTEX_FAST is used.
** The mutex implementation does not need to make a distinction
** between CORTEX_MUTEX_RECURSIVE and CORTEX_MUTEX_FAST if it does
** not want to.  CORTEX will only request a recursive mutex in
** cases where it really needs one.  If a faster non-recursive mutex
** implementation is available on the host platform, the mutex subsystem
** might return such a mutex in response to CORTEX_MUTEX_FAST.
**
** ^The other allowed parameters to cortex_mutex_alloc() (anything other
** than CORTEX_MUTEX_FAST and CORTEX_MUTEX_RECURSIVE) each return
** a pointer to a static preexisting mutex.  ^Nine static mutexes are
** used by the current version of SQLite.  Future versions of SQLite
** may add additional static mutexes.  Static mutexes are for internal
** use by CORTEX only.  Applications that use CORTEX mutexes should
** use only the dynamic mutexes returned by CORTEX_MUTEX_FAST or
** CORTEX_MUTEX_RECURSIVE.
**
** ^Note that if one of the dynamic mutex parameters (CORTEX_MUTEX_FAST
** or CORTEX_MUTEX_RECURSIVE) is used then cortex_mutex_alloc()
** returns a different mutex on every call.  ^For the static
** mutex types, the same mutex is returned on every call that has
** the same type number.
**
** ^The cortex_mutex_free() routine deallocates a previously
** allocated dynamic mutex.  Attempting to deallocate a static
** mutex results in undefined behavior.
**
** ^The cortex_mutex_enter() and cortex_mutex_try() routines attempt
** to enter a mutex.  ^If another thread is already within the mutex,
** cortex_mutex_enter() will block and cortex_mutex_try() will return
** CORTEX_BUSY.  ^The cortex_mutex_try() interface returns [CORTEX_OK]
** upon successful entry.  ^(Mutexes created using
** CORTEX_MUTEX_RECURSIVE can be entered multiple times by the same thread.
** In such cases, the
** mutex must be exited an equal number of times before another thread
** can enter.)^  If the same thread tries to enter any mutex other
** than an CORTEX_MUTEX_RECURSIVE more than once, the behavior is undefined.
**
** ^(Some systems (for example, Windows 95) do not support the operation
** implemented by cortex_mutex_try().  On those systems, cortex_mutex_try()
** will always return CORTEX_BUSY. In most cases the CORTEX core only uses
** cortex_mutex_try() as an optimization, so this is acceptable
** behavior. The exceptions are unix builds that set the
** CORTEX_ENABLE_SETLK_TIMEOUT build option. In that case a working
** cortex_mutex_try() is required.)^
**
** ^The cortex_mutex_leave() routine exits a mutex that was
** previously entered by the same thread.   The behavior
** is undefined if the mutex is not currently entered by the
** calling thread or is not currently allocated.
**
** ^If the argument to cortex_mutex_enter(), cortex_mutex_try(),
** cortex_mutex_leave(), or cortex_mutex_free() is a NULL pointer,
** then any of the four routines behaves as a no-op.
**
** See also: [cortex_mutex_held()] and [cortex_mutex_notheld()].
*/
CORTEX_API cortex_mutex *cortex_mutex_alloc(int);
CORTEX_API void cortex_mutex_free(cortex_mutex*);
CORTEX_API void cortex_mutex_enter(cortex_mutex*);
CORTEX_API int cortex_mutex_try(cortex_mutex*);
CORTEX_API void cortex_mutex_leave(cortex_mutex*);

/*
** CAPI3REF: Mutex Methods Object
**
** An instance of this structure defines the low-level routines
** used to allocate and use mutexes.
**
** Usually, the default mutex implementations provided by CORTEX are
** sufficient, however the application has the option of substituting a custom
** implementation for specialized deployments or systems for which SQLite
** does not provide a suitable implementation. In this case, the application
** creates and populates an instance of this structure to pass
** to cortex_config() along with the [CORTEX_CONFIG_MUTEX] option.
** Additionally, an instance of this structure can be used as an
** output variable when querying the system for the current mutex
** implementation, using the [CORTEX_CONFIG_GETMUTEX] option.
**
** ^The xMutexInit method defined by this structure is invoked as
** part of system initialization by the cortex_initialize() function.
** ^The xMutexInit routine is called by CORTEX exactly once for each
** effective call to [cortex_initialize()].
**
** ^The xMutexEnd method defined by this structure is invoked as
** part of system shutdown by the cortex_shutdown() function. The
** implementation of this method is expected to release all outstanding
** resources obtained by the mutex methods implementation, especially
** those obtained by the xMutexInit method.  ^The xMutexEnd()
** interface is invoked exactly once for each call to [cortex_shutdown()].
**
** ^(The remaining seven methods defined by this structure (xMutexAlloc,
** xMutexFree, xMutexEnter, xMutexTry, xMutexLeave, xMutexHeld and
** xMutexNotheld) implement the following interfaces (respectively):
**
** <ul>
**   <li>  [cortex_mutex_alloc()] </li>
**   <li>  [cortex_mutex_free()] </li>
**   <li>  [cortex_mutex_enter()] </li>
**   <li>  [cortex_mutex_try()] </li>
**   <li>  [cortex_mutex_leave()] </li>
**   <li>  [cortex_mutex_held()] </li>
**   <li>  [cortex_mutex_notheld()] </li>
** </ul>)^
**
** The only difference is that the public cortex_XXX functions enumerated
** above silently ignore any invocations that pass a NULL pointer instead
** of a valid mutex handle. The implementations of the methods defined
** by this structure are not required to handle this case. The results
** of passing a NULL pointer instead of a valid mutex handle are undefined
** (i.e. it is acceptable to provide an implementation that segfaults if
** it is passed a NULL pointer).
**
** The xMutexInit() method must be threadsafe.  It must be harmless to
** invoke xMutexInit() multiple times within the same process and without
** intervening calls to xMutexEnd().  Second and subsequent calls to
** xMutexInit() must be no-ops.
**
** xMutexInit() must not use CORTEX memory allocation ([cortex_malloc()]
** and its associates).  Similarly, xMutexAlloc() must not use CORTEX memory
** allocation for a static mutex.  ^However xMutexAlloc() may use SQLite
** memory allocation for a fast or recursive mutex.
**
** ^SQLite will invoke the xMutexEnd() method when [cortex_shutdown()] is
** called, but only if the prior call to xMutexInit returned CORTEX_OK.
** If xMutexInit fails in any way, it is expected to clean up after itself
** prior to returning.
*/
typedef struct cortex_mutex_methods cortex_mutex_methods;
struct cortex_mutex_methods {
  int (*xMutexInit)(void);
  int (*xMutexEnd)(void);
  cortex_mutex *(*xMutexAlloc)(int);
  void (*xMutexFree)(cortex_mutex *);
  void (*xMutexEnter)(cortex_mutex *);
  int (*xMutexTry)(cortex_mutex *);
  void (*xMutexLeave)(cortex_mutex *);
  int (*xMutexHeld)(cortex_mutex *);
  int (*xMutexNotheld)(cortex_mutex *);
};

/*
** CAPI3REF: Mutex Verification Routines
**
** The cortex_mutex_held() and cortex_mutex_notheld() routines
** are intended for use inside assert() statements.  The CORTEX core
** never uses these routines except inside an assert() and applications
** are advised to follow the lead of the core.  The CORTEX core only
** provides implementations for these routines when it is compiled
** with the CORTEX_DEBUG flag.  External mutex implementations
** are only required to provide these routines if CORTEX_DEBUG is
** defined and if NDEBUG is not defined.
**
** These routines should return true if the mutex in their argument
** is held or not held, respectively, by the calling thread.
**
** The implementation is not required to provide versions of these
** routines that actually work. If the implementation does not provide working
** versions of these routines, it should at least provide stubs that always
** return true so that one does not get spurious assertion failures.
**
** If the argument to cortex_mutex_held() is a NULL pointer then
** the routine should return 1.   This seems counter-intuitive since
** clearly the mutex cannot be held if it does not exist.  But
** the reason the mutex does not exist is because the build is not
** using mutexes.  And we do not want the assert() containing the
** call to cortex_mutex_held() to fail, so a non-zero return is
** the appropriate thing to do.  The cortex_mutex_notheld()
** interface should also return 1 when given a NULL pointer.
*/
#ifndef NDEBUG
CORTEX_API int cortex_mutex_held(cortex_mutex*);
CORTEX_API int cortex_mutex_notheld(cortex_mutex*);
#endif

/*
** CAPI3REF: Mutex Types
**
** The [cortex_mutex_alloc()] interface takes a single argument
** which is one of these integer constants.
**
** The set of static mutexes may change from one CORTEX release to the
** next.  Applications that override the built-in mutex logic must be
** prepared to accommodate additional static mutexes.
*/
#define CORTEX_MUTEX_FAST             0
#define CORTEX_MUTEX_RECURSIVE        1
#define CORTEX_MUTEX_STATIC_MAIN      2
#define CORTEX_MUTEX_STATIC_MEM       3  /* cortex_malloc() */
#define CORTEX_MUTEX_STATIC_MEM2      4  /* NOT USED */
#define CORTEX_MUTEX_STATIC_OPEN      4  /* cortexBtreeOpen() */
#define CORTEX_MUTEX_STATIC_PRNG      5  /* cortex_randomness() */
#define CORTEX_MUTEX_STATIC_LRU       6  /* lru page list */
#define CORTEX_MUTEX_STATIC_LRU2      7  /* NOT USED */
#define CORTEX_MUTEX_STATIC_PMEM      7  /* cortexPageMalloc() */
#define CORTEX_MUTEX_STATIC_APP1      8  /* For use by application */
#define CORTEX_MUTEX_STATIC_APP2      9  /* For use by application */
#define CORTEX_MUTEX_STATIC_APP3     10  /* For use by application */
#define CORTEX_MUTEX_STATIC_VFS1     11  /* For use by built-in VFS */
#define CORTEX_MUTEX_STATIC_VFS2     12  /* For use by extension VFS */
#define CORTEX_MUTEX_STATIC_VFS3     13  /* For use by application VFS */

/* Legacy compatibility: */
#define CORTEX_MUTEX_STATIC_MASTER    2


/*
** CAPI3REF: Retrieve the mutex for a database connection
** METHOD: cortex
**
** ^This interface returns a pointer to the [cortex_mutex] object that
** serializes access to the [database connection] given in the argument
** when the [threading mode] is Serialized.
** ^If the [threading mode] is Single-thread or Multi-thread then this
** routine returns a NULL pointer.
*/
CORTEX_API cortex_mutex *cortex_db_mutex(cortex*);

/*
** CAPI3REF: Low-Level Control Of Database Files
** METHOD: cortex
** KEYWORDS: {file control}
**
** ^The [cortex_file_control()] interface makes a direct call to the
** xFileControl method for the [cortex_io_methods] object associated
** with a particular database identified by the second argument. ^The
** name of the database is "main" for the main database or "temp" for the
** TEMP database, or the name that appears after the AS keyword for
** databases that are added using the [ATTACH] SQL command.
** ^A NULL pointer can be used in place of "main" to refer to the
** main database file.
** ^The third and fourth parameters to this routine
** are passed directly through to the second and third parameters of
** the xFileControl method.  ^The return value of the xFileControl
** method becomes the return value of this routine.
**
** A few opcodes for [cortex_file_control()] are handled directly
** by the CORTEX core and never invoke the
** cortex_io_methods.xFileControl method.
** ^The [CORTEX_FCNTL_FILE_POINTER] value for the op parameter causes
** a pointer to the underlying [cortex_file] object to be written into
** the space pointed to by the 4th parameter.  The
** [CORTEX_FCNTL_JOURNAL_POINTER] works similarly except that it returns
** the [cortex_file] object associated with the journal file instead of
** the main database.  The [CORTEX_FCNTL_VFS_POINTER] opcode returns
** a pointer to the underlying [cortex_vfs] object for the file.
** The [CORTEX_FCNTL_DATA_VERSION] returns the data version counter
** from the pager.
**
** ^If the second parameter (zDbName) does not match the name of any
** open database file, then CORTEX_ERROR is returned.  ^This error
** code is not remembered and will not be recalled by [cortex_errcode()]
** or [cortex_errmsg()].  The underlying xFileControl method might
** also return CORTEX_ERROR.  There is no way to distinguish between
** an incorrect zDbName and an CORTEX_ERROR return from the underlying
** xFileControl method.
**
** See also: [file control opcodes]
*/
CORTEX_API int cortex_file_control(cortex*, const char *zDbName, int op, void*);

/*
** CAPI3REF: Testing Interface
**
** ^The cortex_test_control() interface is used to read out internal
** state of CORTEX and to inject faults into CORTEX for testing
** purposes.  ^The first parameter is an operation code that determines
** the number, meaning, and operation of all subsequent parameters.
**
** This interface is not for use by applications.  It exists solely
** for verifying the correct operation of the CORTEX library.  Depending
** on how the CORTEX library is compiled, this interface might not exist.
**
** The details of the operation codes, their meanings, the parameters
** they take, and what they do are all subject to change without notice.
** Unlike most of the CORTEX API, this function is not guaranteed to
** operate consistently from one release to the next.
*/
CORTEX_API int cortex_test_control(int op, ...);

/*
** CAPI3REF: Testing Interface Operation Codes
**
** These constants are the valid operation code parameters used
** as the first argument to [cortex_test_control()].
**
** These parameters and their meanings are subject to change
** without notice.  These values are for testing purposes only.
** Applications should not use any of these parameters or the
** [cortex_test_control()] interface.
*/
#define CORTEX_TESTCTRL_FIRST                    5
#define CORTEX_TESTCTRL_PRNG_SAVE                5
#define CORTEX_TESTCTRL_PRNG_RESTORE             6
#define CORTEX_TESTCTRL_PRNG_RESET               7  /* NOT USED */
#define CORTEX_TESTCTRL_FK_NO_ACTION             7
#define CORTEX_TESTCTRL_BITVEC_TEST              8
#define CORTEX_TESTCTRL_FAULT_INSTALL            9
#define CORTEX_TESTCTRL_BENIGN_MALLOC_HOOKS     10
#define CORTEX_TESTCTRL_PENDING_BYTE            11
#define CORTEX_TESTCTRL_ASSERT                  12
#define CORTEX_TESTCTRL_ALWAYS                  13
#define CORTEX_TESTCTRL_RESERVE                 14  /* NOT USED */
#define CORTEX_TESTCTRL_JSON_SELFCHECK          14
#define CORTEX_TESTCTRL_OPTIMIZATIONS           15
#define CORTEX_TESTCTRL_ISKEYWORD               16  /* NOT USED */
#define CORTEX_TESTCTRL_GETOPT                  16
#define CORTEX_TESTCTRL_SCRATCHMALLOC           17  /* NOT USED */
#define CORTEX_TESTCTRL_INTERNAL_FUNCTIONS      17
#define CORTEX_TESTCTRL_LOCALTIME_FAULT         18
#define CORTEX_TESTCTRL_EXPLAIN_STMT            19  /* NOT USED */
#define CORTEX_TESTCTRL_ONCE_RESET_THRESHOLD    19
#define CORTEX_TESTCTRL_NEVER_CORRUPT           20
#define CORTEX_TESTCTRL_VDBE_COVERAGE           21
#define CORTEX_TESTCTRL_BYTEORDER               22
#define CORTEX_TESTCTRL_ISINIT                  23
#define CORTEX_TESTCTRL_SORTER_MMAP             24
#define CORTEX_TESTCTRL_IMPOSTER                25
#define CORTEX_TESTCTRL_PARSER_COVERAGE         26
#define CORTEX_TESTCTRL_RESULT_INTREAL          27
#define CORTEX_TESTCTRL_PRNG_SEED               28
#define CORTEX_TESTCTRL_EXTRA_SCHEMA_CHECKS     29
#define CORTEX_TESTCTRL_SEEK_COUNT              30
#define CORTEX_TESTCTRL_TRACEFLAGS              31
#define CORTEX_TESTCTRL_TUNE                    32
#define CORTEX_TESTCTRL_LOGEST                  33
#define CORTEX_TESTCTRL_USELONGDOUBLE           34  /* NOT USED */
#define CORTEX_TESTCTRL_LAST                    34  /* Largest TESTCTRL */

/*
** CAPI3REF: SQL Keyword Checking
**
** These routines provide access to the set of SQL language keywords
** recognized by SQLite.  Applications can use these routines to determine
** whether or not a specific identifier needs to be escaped (for example,
** by enclosing in double-quotes) so as not to confuse the parser.
**
** The cortex_keyword_count() interface returns the number of distinct
** keywords understood by SQLite.
**
** The cortex_keyword_name(N,Z,L) interface finds the 0-based N-th keyword and
** makes *Z point to that keyword expressed as UTF8 and writes the number
** of bytes in the keyword into *L.  The string that *Z points to is not
** zero-terminated.  The cortex_keyword_name(N,Z,L) routine returns
** CORTEX_OK if N is within bounds and CORTEX_ERROR if not. If either Z
** or L are NULL or invalid pointers then calls to
** cortex_keyword_name(N,Z,L) result in undefined behavior.
**
** The cortex_keyword_check(Z,L) interface checks to see whether or not
** the L-byte UTF8 identifier that Z points to is a keyword, returning non-zero
** if it is and zero if not.
**
** The parser used by CORTEX is forgiving.  It is often possible to use
** a keyword as an identifier as long as such use does not result in a
** parsing ambiguity.  For example, the statement
** "CREATE TABLE BEGIN(REPLACE,PRAGMA,END);" is accepted by SQLite, and
** creates a new table named "BEGIN" with three columns named
** "REPLACE", "PRAGMA", and "END".  Nevertheless, best practice is to avoid
** using keywords as identifiers.  Common techniques used to avoid keyword
** name collisions include:
** <ul>
** <li> Put all identifier names inside double-quotes.  This is the official
**      SQL way to escape identifier names.
** <li> Put identifier names inside &#91;...&#93;.  This is not standard SQL,
**      but it is what SQL Server does and so lots of programmers use this
**      technique.
** <li> Begin every identifier with the letter "Z" as no SQL keywords start
**      with "Z".
** <li> Include a digit somewhere in every identifier name.
** </ul>
**
** Note that the number of keywords understood by CORTEX can depend on
** compile-time options.  For example, "VACUUM" is not a keyword if
** CORTEX is compiled with the [-DCORTEX_OMIT_VACUUM] option.  Also,
** new keywords may be added to future releases of SQLite.
*/
CORTEX_API int cortex_keyword_count(void);
CORTEX_API int cortex_keyword_name(int,const char**,int*);
CORTEX_API int cortex_keyword_check(const char*,int);

/*
** CAPI3REF: Dynamic String Object
** KEYWORDS: {dynamic string}
**
** An instance of the cortex_str object contains a dynamically-sized
** string under construction.
**
** The lifecycle of an cortex_str object is as follows:
** <ol>
** <li> ^The cortex_str object is created using [cortex_str_new()].
** <li> ^Text is appended to the cortex_str object using various
** methods, such as [cortex_str_appendf()].
** <li> ^The cortex_str object is destroyed and the string it created
** is returned using the [cortex_str_finish()] interface.
** </ol>
*/
typedef struct cortex_str cortex_str;

/*
** CAPI3REF: Create A New Dynamic String Object
** CONSTRUCTOR: cortex_str
**
** ^The [cortex_str_new(D)] interface allocates and initializes
** a new [cortex_str] object.  To avoid memory leaks, the object returned by
** [cortex_str_new()] must be freed by a subsequent call to
** [cortex_str_finish(X)].
**
** ^The [cortex_str_new(D)] interface always returns a pointer to a
** valid [cortex_str] object, though in the event of an out-of-memory
** error the returned object might be a special singleton that will
** silently reject new text, always return CORTEX_NOMEM from
** [cortex_str_errcode()], always return 0 for
** [cortex_str_length()], and always return NULL from
** [cortex_str_finish(X)].  It is always safe to use the value
** returned by [cortex_str_new(D)] as the cortex_str parameter
** to any of the other [cortex_str] methods.
**
** The D parameter to [cortex_str_new(D)] may be NULL.  If the
** D parameter in [cortex_str_new(D)] is not NULL, then the maximum
** length of the string contained in the [cortex_str] object will be
** the value set for [cortex_limit](D,[CORTEX_LIMIT_LENGTH]) instead
** of [CORTEX_MAX_LENGTH].
*/
CORTEX_API cortex_str *cortex_str_new(cortex*);

/*
** CAPI3REF: Finalize A Dynamic String
** DESTRUCTOR: cortex_str
**
** ^The [cortex_str_finish(X)] interface destroys the cortex_str object X
** and returns a pointer to a memory buffer obtained from [cortex_malloc64()]
** that contains the constructed string.  The calling application should
** pass the returned value to [cortex_free()] to avoid a memory leak.
** ^The [cortex_str_finish(X)] interface may return a NULL pointer if any
** errors were encountered during construction of the string.  ^The
** [cortex_str_finish(X)] interface will also return a NULL pointer if the
** string in [cortex_str] object X is zero bytes long.
*/
CORTEX_API char *cortex_str_finish(cortex_str*);

/*
** CAPI3REF: Add Content To A Dynamic String
** METHOD: cortex_str
**
** These interfaces add content to an cortex_str object previously obtained
** from [cortex_str_new()].
**
** ^The [cortex_str_appendf(X,F,...)] and
** [cortex_str_vappendf(X,F,V)] interfaces uses the [built-in printf]
** functionality of CORTEX to append formatted text onto the end of
** [cortex_str] object X.
**
** ^The [cortex_str_append(X,S,N)] method appends exactly N bytes from string S
** onto the end of the [cortex_str] object X.  N must be non-negative.
** S must contain at least N non-zero bytes of content.  To append a
** zero-terminated string in its entirety, use the [cortex_str_appendall()]
** method instead.
**
** ^The [cortex_str_appendall(X,S)] method appends the complete content of
** zero-terminated string S onto the end of [cortex_str] object X.
**
** ^The [cortex_str_appendchar(X,N,C)] method appends N copies of the
** single-byte character C onto the end of [cortex_str] object X.
** ^This method can be used, for example, to add whitespace indentation.
**
** ^The [cortex_str_reset(X)] method resets the string under construction
** inside [cortex_str] object X back to zero bytes in length.
**
** These methods do not return a result code.  ^If an error occurs, that fact
** is recorded in the [cortex_str] object and can be recovered by a
** subsequent call to [cortex_str_errcode(X)].
*/
CORTEX_API void cortex_str_appendf(cortex_str*, const char *zFormat, ...);
CORTEX_API void cortex_str_vappendf(cortex_str*, const char *zFormat, va_list);
CORTEX_API void cortex_str_append(cortex_str*, const char *zIn, int N);
CORTEX_API void cortex_str_appendall(cortex_str*, const char *zIn);
CORTEX_API void cortex_str_appendchar(cortex_str*, int N, char C);
CORTEX_API void cortex_str_reset(cortex_str*);

/*
** CAPI3REF: Status Of A Dynamic String
** METHOD: cortex_str
**
** These interfaces return the current status of an [cortex_str] object.
**
** ^If any prior errors have occurred while constructing the dynamic string
** in cortex_str X, then the [cortex_str_errcode(X)] method will return
** an appropriate error code.  ^The [cortex_str_errcode(X)] method returns
** [CORTEX_NOMEM] following any out-of-memory error, or
** [CORTEX_TOOBIG] if the size of the dynamic string exceeds
** [CORTEX_MAX_LENGTH], or [CORTEX_OK] if there have been no errors.
**
** ^The [cortex_str_length(X)] method returns the current length, in bytes,
** of the dynamic string under construction in [cortex_str] object X.
** ^The length returned by [cortex_str_length(X)] does not include the
** zero-termination byte.
**
** ^The [cortex_str_value(X)] method returns a pointer to the current
** content of the dynamic string under construction in X.  The value
** returned by [cortex_str_value(X)] is managed by the cortex_str object X
** and might be freed or altered by any subsequent method on the same
** [cortex_str] object.  Applications must not use the pointer returned by
** [cortex_str_value(X)] after any subsequent method call on the same
** object.  ^Applications may change the content of the string returned
** by [cortex_str_value(X)] as long as they do not write into any bytes
** outside the range of 0 to [cortex_str_length(X)] and do not read or
** write any byte after any subsequent cortex_str method call.
*/
CORTEX_API int cortex_str_errcode(cortex_str*);
CORTEX_API int cortex_str_length(cortex_str*);
CORTEX_API char *cortex_str_value(cortex_str*);

/*
** CAPI3REF: CORTEX Runtime Status
**
** ^These interfaces are used to retrieve runtime status information
** about the performance of SQLite, and optionally to reset various
** highwater marks.  ^The first argument is an integer code for
** the specific parameter to measure.  ^(Recognized integer codes
** are of the form [status parameters | CORTEX_STATUS_...].)^
** ^The current value of the parameter is returned into *pCurrent.
** ^The highest recorded value is returned in *pHighwater.  ^If the
** resetFlag is true, then the highest record value is reset after
** *pHighwater is written.  ^(Some parameters do not record the highest
** value.  For those parameters
** nothing is written into *pHighwater and the resetFlag is ignored.)^
** ^(Other parameters record only the highwater mark and not the current
** value.  For these latter parameters nothing is written into *pCurrent.)^
**
** ^The cortex_status() and cortex_status64() routines return
** CORTEX_OK on success and a non-zero [error code] on failure.
**
** If either the current value or the highwater mark is too large to
** be represented by a 32-bit integer, then the values returned by
** cortex_status() are undefined.
**
** See also: [cortex_db_status()]
*/
CORTEX_API int cortex_status(int op, int *pCurrent, int *pHighwater, int resetFlag);
CORTEX_API int cortex_status64(
  int op,
  cortex_int64 *pCurrent,
  cortex_int64 *pHighwater,
  int resetFlag
);


/*
** CAPI3REF: Status Parameters
** KEYWORDS: {status parameters}
**
** These integer constants designate various run-time status parameters
** that can be returned by [cortex_status()].
**
** <dl>
** [[CORTEX_STATUS_MEMORY_USED]] ^(<dt>CORTEX_STATUS_MEMORY_USED</dt>
** <dd>This parameter is the current amount of memory checked out
** using [cortex_malloc()], either directly or indirectly.  The
** figure includes calls made to [cortex_malloc()] by the application
** and internal memory usage by the CORTEX library.  Auxiliary page-cache
** memory controlled by [CORTEX_CONFIG_PAGECACHE] is not included in
** this parameter.  The amount returned is the sum of the allocation
** sizes as reported by the xSize method in [cortex_mem_methods].</dd>)^
**
** [[CORTEX_STATUS_MALLOC_SIZE]] ^(<dt>CORTEX_STATUS_MALLOC_SIZE</dt>
** <dd>This parameter records the largest memory allocation request
** handed to [cortex_malloc()] or [cortex_realloc()] (or their
** internal equivalents).  Only the value returned in the
** *pHighwater parameter to [cortex_status()] is of interest.
** The value written into the *pCurrent parameter is undefined.</dd>)^
**
** [[CORTEX_STATUS_MALLOC_COUNT]] ^(<dt>CORTEX_STATUS_MALLOC_COUNT</dt>
** <dd>This parameter records the number of separate memory allocations
** currently checked out.</dd>)^
**
** [[CORTEX_STATUS_PAGECACHE_USED]] ^(<dt>CORTEX_STATUS_PAGECACHE_USED</dt>
** <dd>This parameter returns the number of pages used out of the
** [pagecache memory allocator] that was configured using
** [CORTEX_CONFIG_PAGECACHE].  The
** value returned is in pages, not in bytes.</dd>)^
**
** [[CORTEX_STATUS_PAGECACHE_OVERFLOW]]
** ^(<dt>CORTEX_STATUS_PAGECACHE_OVERFLOW</dt>
** <dd>This parameter returns the number of bytes of page cache
** allocation which could not be satisfied by the [CORTEX_CONFIG_PAGECACHE]
** buffer and where forced to overflow to [cortex_malloc()].  The
** returned value includes allocations that overflowed because they
** were too large (they were larger than the "sz" parameter to
** [CORTEX_CONFIG_PAGECACHE]) and allocations that overflowed because
** no space was left in the page cache.</dd>)^
**
** [[CORTEX_STATUS_PAGECACHE_SIZE]] ^(<dt>CORTEX_STATUS_PAGECACHE_SIZE</dt>
** <dd>This parameter records the largest memory allocation request
** handed to the [pagecache memory allocator].  Only the value returned in the
** *pHighwater parameter to [cortex_status()] is of interest.
** The value written into the *pCurrent parameter is undefined.</dd>)^
**
** [[CORTEX_STATUS_SCRATCH_USED]] <dt>CORTEX_STATUS_SCRATCH_USED</dt>
** <dd>No longer used.</dd>
**
** [[CORTEX_STATUS_SCRATCH_OVERFLOW]] ^(<dt>CORTEX_STATUS_SCRATCH_OVERFLOW</dt>
** <dd>No longer used.</dd>
**
** [[CORTEX_STATUS_SCRATCH_SIZE]] <dt>CORTEX_STATUS_SCRATCH_SIZE</dt>
** <dd>No longer used.</dd>
**
** [[CORTEX_STATUS_PARSER_STACK]] ^(<dt>CORTEX_STATUS_PARSER_STACK</dt>
** <dd>The *pHighwater parameter records the deepest parser stack.
** The *pCurrent value is undefined.  The *pHighwater value is only
** meaningful if CORTEX is compiled with [YYTRACKMAXSTACKDEPTH].</dd>)^
** </dl>
**
** New status parameters may be added from time to time.
*/
#define CORTEX_STATUS_MEMORY_USED          0
#define CORTEX_STATUS_PAGECACHE_USED       1
#define CORTEX_STATUS_PAGECACHE_OVERFLOW   2
#define CORTEX_STATUS_SCRATCH_USED         3  /* NOT USED */
#define CORTEX_STATUS_SCRATCH_OVERFLOW     4  /* NOT USED */
#define CORTEX_STATUS_MALLOC_SIZE          5
#define CORTEX_STATUS_PARSER_STACK         6
#define CORTEX_STATUS_PAGECACHE_SIZE       7
#define CORTEX_STATUS_SCRATCH_SIZE         8  /* NOT USED */
#define CORTEX_STATUS_MALLOC_COUNT         9

/*
** CAPI3REF: Database Connection Status
** METHOD: cortex
**
** ^This interface is used to retrieve runtime status information
** about a single [database connection].  ^The first argument is the
** database connection object to be interrogated.  ^The second argument
** is an integer constant, taken from the set of
** [CORTEX_DBSTATUS options], that
** determines the parameter to interrogate.  The set of
** [CORTEX_DBSTATUS options] is likely
** to grow in future releases of SQLite.
**
** ^The current value of the requested parameter is written into *pCur
** and the highest instantaneous value is written into *pHiwtr.  ^If
** the resetFlg is true, then the highest instantaneous value is
** reset back down to the current value.
**
** ^The cortex_db_status() routine returns CORTEX_OK on success and a
** non-zero [error code] on failure.
**
** ^The cortex_db_status64(D,O,C,H,R) routine works exactly the same
** way as cortex_db_status(D,O,C,H,R) routine except that the C and H
** parameters are pointer to 64-bit integers (type: cortex_int64) instead
** of pointers to 32-bit integers, which allows larger status values
** to be returned.  If a status value exceeds 2,147,483,647 then
** cortex_db_status() will truncate the value whereas cortex_db_status64()
** will return the full value.
**
** See also: [cortex_status()] and [cortex_stmt_status()].
*/
CORTEX_API int cortex_db_status(cortex*, int op, int *pCur, int *pHiwtr, int resetFlg);
CORTEX_API int cortex_db_status64(cortex*,int,cortex_int64*,cortex_int64*,int);

/*
** CAPI3REF: Status Parameters for database connections
** KEYWORDS: {CORTEX_DBSTATUS options}
**
** These constants are the available integer "verbs" that can be passed as
** the second argument to the [cortex_db_status()] interface.
**
** New verbs may be added in future releases of SQLite. Existing verbs
** might be discontinued. Applications should check the return code from
** [cortex_db_status()] to make sure that the call worked.
** The [cortex_db_status()] interface will return a non-zero error code
** if a discontinued or unsupported verb is invoked.
**
** <dl>
** [[CORTEX_DBSTATUS_LOOKASIDE_USED]] ^(<dt>CORTEX_DBSTATUS_LOOKASIDE_USED</dt>
** <dd>This parameter returns the number of lookaside memory slots currently
** checked out.</dd>)^
**
** [[CORTEX_DBSTATUS_LOOKASIDE_HIT]] ^(<dt>CORTEX_DBSTATUS_LOOKASIDE_HIT</dt>
** <dd>This parameter returns the number of malloc attempts that were
** satisfied using lookaside memory. Only the high-water value is meaningful;
** the current value is always zero.</dd>)^
**
** [[CORTEX_DBSTATUS_LOOKASIDE_MISS_SIZE]]
** ^(<dt>CORTEX_DBSTATUS_LOOKASIDE_MISS_SIZE</dt>
** <dd>This parameter returns the number of malloc attempts that might have
** been satisfied using lookaside memory but failed due to the amount of
** memory requested being larger than the lookaside slot size.
** Only the high-water value is meaningful;
** the current value is always zero.</dd>)^
**
** [[CORTEX_DBSTATUS_LOOKASIDE_MISS_FULL]]
** ^(<dt>CORTEX_DBSTATUS_LOOKASIDE_MISS_FULL</dt>
** <dd>This parameter returns the number of malloc attempts that might have
** been satisfied using lookaside memory but failed due to all lookaside
** memory already being in use.
** Only the high-water value is meaningful;
** the current value is always zero.</dd>)^
**
** [[CORTEX_DBSTATUS_CACHE_USED]] ^(<dt>CORTEX_DBSTATUS_CACHE_USED</dt>
** <dd>This parameter returns the approximate number of bytes of heap
** memory used by all pager caches associated with the database connection.)^
** ^The highwater mark associated with CORTEX_DBSTATUS_CACHE_USED is always 0.
** </dd>
**
** [[CORTEX_DBSTATUS_CACHE_USED_SHARED]]
** ^(<dt>CORTEX_DBSTATUS_CACHE_USED_SHARED</dt>
** <dd>This parameter is similar to DBSTATUS_CACHE_USED, except that if a
** pager cache is shared between two or more connections the bytes of heap
** memory used by that pager cache is divided evenly between the attached
** connections.)^  In other words, if none of the pager caches associated
** with the database connection are shared, this request returns the same
** value as DBSTATUS_CACHE_USED. Or, if one or more of the pager caches are
** shared, the value returned by this call will be smaller than that returned
** by DBSTATUS_CACHE_USED. ^The highwater mark associated with
** CORTEX_DBSTATUS_CACHE_USED_SHARED is always 0.</dd>
**
** [[CORTEX_DBSTATUS_SCHEMA_USED]] ^(<dt>CORTEX_DBSTATUS_SCHEMA_USED</dt>
** <dd>This parameter returns the approximate number of bytes of heap
** memory used to store the schema for all databases associated
** with the connection - main, temp, and any [ATTACH]-ed databases.)^
** ^The full amount of memory used by the schemas is reported, even if the
** schema memory is shared with other database connections due to
** [shared cache mode] being enabled.
** ^The highwater mark associated with CORTEX_DBSTATUS_SCHEMA_USED is always 0.
** </dd>
**
** [[CORTEX_DBSTATUS_STMT_USED]] ^(<dt>CORTEX_DBSTATUS_STMT_USED</dt>
** <dd>This parameter returns the approximate number of bytes of heap
** and lookaside memory used by all prepared statements associated with
** the database connection.)^
** ^The highwater mark associated with CORTEX_DBSTATUS_STMT_USED is always 0.
** </dd>
**
** [[CORTEX_DBSTATUS_CACHE_HIT]] ^(<dt>CORTEX_DBSTATUS_CACHE_HIT</dt>
** <dd>This parameter returns the number of pager cache hits that have
** occurred.)^ ^The highwater mark associated with CORTEX_DBSTATUS_CACHE_HIT
** is always 0.
** </dd>
**
** [[CORTEX_DBSTATUS_CACHE_MISS]] ^(<dt>CORTEX_DBSTATUS_CACHE_MISS</dt>
** <dd>This parameter returns the number of pager cache misses that have
** occurred.)^ ^The highwater mark associated with CORTEX_DBSTATUS_CACHE_MISS
** is always 0.
** </dd>
**
** [[CORTEX_DBSTATUS_CACHE_WRITE]] ^(<dt>CORTEX_DBSTATUS_CACHE_WRITE</dt>
** <dd>This parameter returns the number of dirty cache entries that have
** been written to disk. Specifically, the number of pages written to the
** wal file in wal mode databases, or the number of pages written to the
** database file in rollback mode databases. Any pages written as part of
** transaction rollback or database recovery operations are not included.
** If an IO or other error occurs while writing a page to disk, the effect
** on subsequent CORTEX_DBSTATUS_CACHE_WRITE requests is undefined.)^ ^The
** highwater mark associated with CORTEX_DBSTATUS_CACHE_WRITE is always 0.
** <p>
** ^(There is overlap between the quantities measured by this parameter
** (CORTEX_DBSTATUS_CACHE_WRITE) and CORTEX_DBSTATUS_TEMPBUF_SPILL.
** Resetting one will reduce the other.)^
** </dd>
**
** [[CORTEX_DBSTATUS_CACHE_SPILL]] ^(<dt>CORTEX_DBSTATUS_CACHE_SPILL</dt>
** <dd>This parameter returns the number of dirty cache entries that have
** been written to disk in the middle of a transaction due to the page
** cache overflowing. Transactions are more efficient if they are written
** to disk all at once. When pages spill mid-transaction, that introduces
** additional overhead. This parameter can be used to help identify
** inefficiencies that can be resolved by increasing the cache size.
** </dd>
**
** [[CORTEX_DBSTATUS_DEFERRED_FKS]] ^(<dt>CORTEX_DBSTATUS_DEFERRED_FKS</dt>
** <dd>This parameter returns zero for the current value if and only if
** all foreign key constraints (deferred or immediate) have been
** resolved.)^  ^The highwater mark is always 0.
**
** [[CORTEX_DBSTATUS_TEMPBUF_SPILL] ^(<dt>CORTEX_DBSTATUS_TEMPBUF_SPILL</dt>
** <dd>^(This parameter returns the number of bytes written to temporary
** files on disk that could have been kept in memory had sufficient memory
** been available.  This value includes writes to intermediate tables that
** are part of complex queries, external sorts that spill to disk, and
** writes to TEMP tables.)^
** ^The highwater mark is always 0.
** <p>
** ^(There is overlap between the quantities measured by this parameter
** (CORTEX_DBSTATUS_TEMPBUF_SPILL) and CORTEX_DBSTATUS_CACHE_WRITE.
** Resetting one will reduce the other.)^
** </dd>
** </dl>
*/
#define CORTEX_DBSTATUS_LOOKASIDE_USED       0
#define CORTEX_DBSTATUS_CACHE_USED           1
#define CORTEX_DBSTATUS_SCHEMA_USED          2
#define CORTEX_DBSTATUS_STMT_USED            3
#define CORTEX_DBSTATUS_LOOKASIDE_HIT        4
#define CORTEX_DBSTATUS_LOOKASIDE_MISS_SIZE  5
#define CORTEX_DBSTATUS_LOOKASIDE_MISS_FULL  6
#define CORTEX_DBSTATUS_CACHE_HIT            7
#define CORTEX_DBSTATUS_CACHE_MISS           8
#define CORTEX_DBSTATUS_CACHE_WRITE          9
#define CORTEX_DBSTATUS_DEFERRED_FKS        10
#define CORTEX_DBSTATUS_CACHE_USED_SHARED   11
#define CORTEX_DBSTATUS_CACHE_SPILL         12
#define CORTEX_DBSTATUS_TEMPBUF_SPILL       13
#define CORTEX_DBSTATUS_MAX                 13   /* Largest defined DBSTATUS */


/*
** CAPI3REF: Prepared Statement Status
** METHOD: cortex_stmt
**
** ^(Each prepared statement maintains various
** [CORTEX_STMTSTATUS counters] that measure the number
** of times it has performed specific operations.)^  These counters can
** be used to monitor the performance characteristics of the prepared
** statements.  For example, if the number of table steps greatly exceeds
** the number of table searches or result rows, that would tend to indicate
** that the prepared statement is using a full table scan rather than
** an index.
**
** ^(This interface is used to retrieve and reset counter values from
** a [prepared statement].  The first argument is the prepared statement
** object to be interrogated.  The second argument
** is an integer code for a specific [CORTEX_STMTSTATUS counter]
** to be interrogated.)^
** ^The current value of the requested counter is returned.
** ^If the resetFlg is true, then the counter is reset to zero after this
** interface call returns.
**
** See also: [cortex_status()] and [cortex_db_status()].
*/
CORTEX_API int cortex_stmt_status(cortex_stmt*, int op,int resetFlg);

/*
** CAPI3REF: Status Parameters for prepared statements
** KEYWORDS: {CORTEX_STMTSTATUS counter} {CORTEX_STMTSTATUS counters}
**
** These preprocessor macros define integer codes that name counter
** values associated with the [cortex_stmt_status()] interface.
** The meanings of the various counters are as follows:
**
** <dl>
** [[CORTEX_STMTSTATUS_FULLSCAN_STEP]] <dt>CORTEX_STMTSTATUS_FULLSCAN_STEP</dt>
** <dd>^This is the number of times that CORTEX has stepped forward in
** a table as part of a full table scan.  Large numbers for this counter
** may indicate opportunities for performance improvement through
** careful use of indices.</dd>
**
** [[CORTEX_STMTSTATUS_SORT]] <dt>CORTEX_STMTSTATUS_SORT</dt>
** <dd>^This is the number of sort operations that have occurred.
** A non-zero value in this counter may indicate an opportunity to
** improve performance through careful use of indices.</dd>
**
** [[CORTEX_STMTSTATUS_AUTOINDEX]] <dt>CORTEX_STMTSTATUS_AUTOINDEX</dt>
** <dd>^This is the number of rows inserted into transient indices that
** were created automatically in order to help joins run faster.
** A non-zero value in this counter may indicate an opportunity to
** improve performance by adding permanent indices that do not
** need to be reinitialized each time the statement is run.</dd>
**
** [[CORTEX_STMTSTATUS_VM_STEP]] <dt>CORTEX_STMTSTATUS_VM_STEP</dt>
** <dd>^This is the number of virtual machine operations executed
** by the prepared statement if that number is less than or equal
** to 2147483647.  The number of virtual machine operations can be
** used as a proxy for the total work done by the prepared statement.
** If the number of virtual machine operations exceeds 2147483647
** then the value returned by this statement status code is undefined.</dd>
**
** [[CORTEX_STMTSTATUS_REPREPARE]] <dt>CORTEX_STMTSTATUS_REPREPARE</dt>
** <dd>^This is the number of times that the prepare statement has been
** automatically regenerated due to schema changes or changes to
** [bound parameters] that might affect the query plan.</dd>
**
** [[CORTEX_STMTSTATUS_RUN]] <dt>CORTEX_STMTSTATUS_RUN</dt>
** <dd>^This is the number of times that the prepared statement has
** been run.  A single "run" for the purposes of this counter is one
** or more calls to [cortex_step()] followed by a call to [cortex_reset()].
** The counter is incremented on the first [cortex_step()] call of each
** cycle.</dd>
**
** [[CORTEX_STMTSTATUS_FILTER_MISS]]
** [[CORTEX_STMTSTATUS_FILTER HIT]]
** <dt>CORTEX_STMTSTATUS_FILTER_HIT<br>
** CORTEX_STMTSTATUS_FILTER_MISS</dt>
** <dd>^CORTEX_STMTSTATUS_FILTER_HIT is the number of times that a join
** step was bypassed because a Bloom filter returned not-found.  The
** corresponding CORTEX_STMTSTATUS_FILTER_MISS value is the number of
** times that the Bloom filter returned a find, and thus the join step
** had to be processed as normal.</dd>
**
** [[CORTEX_STMTSTATUS_MEMUSED]] <dt>CORTEX_STMTSTATUS_MEMUSED</dt>
** <dd>^This is the approximate number of bytes of heap memory
** used to store the prepared statement.  ^This value is not actually
** a counter, and so the resetFlg parameter to cortex_stmt_status()
** is ignored when the opcode is CORTEX_STMTSTATUS_MEMUSED.
** </dd>
** </dl>
*/
#define CORTEX_STMTSTATUS_FULLSCAN_STEP     1
#define CORTEX_STMTSTATUS_SORT              2
#define CORTEX_STMTSTATUS_AUTOINDEX         3
#define CORTEX_STMTSTATUS_VM_STEP           4
#define CORTEX_STMTSTATUS_REPREPARE         5
#define CORTEX_STMTSTATUS_RUN               6
#define CORTEX_STMTSTATUS_FILTER_MISS       7
#define CORTEX_STMTSTATUS_FILTER_HIT        8
#define CORTEX_STMTSTATUS_MEMUSED           99

/*
** CAPI3REF: Custom Page Cache Object
**
** The cortex_pcache type is opaque.  It is implemented by
** the pluggable module.  The CORTEX core has no knowledge of
** its size or internal structure and never deals with the
** cortex_pcache object except by holding and passing pointers
** to the object.
**
** See [cortex_pcache_methods2] for additional information.
*/
typedef struct cortex_pcache cortex_pcache;

/*
** CAPI3REF: Custom Page Cache Object
**
** The cortex_pcache_page object represents a single page in the
** page cache.  The page cache will allocate instances of this
** object.  Various methods of the page cache use pointers to instances
** of this object as parameters or as their return value.
**
** See [cortex_pcache_methods2] for additional information.
*/
typedef struct cortex_pcache_page cortex_pcache_page;
struct cortex_pcache_page {
  void *pBuf;        /* The content of the page */
  void *pExtra;      /* Extra information associated with the page */
};

/*
** CAPI3REF: Application Defined Page Cache.
** KEYWORDS: {page cache}
**
** ^(The [cortex_config]([CORTEX_CONFIG_PCACHE2], ...) interface can
** register an alternative page cache implementation by passing in an
** instance of the cortex_pcache_methods2 structure.)^
** In many applications, most of the heap memory allocated by
** CORTEX is used for the page cache.
** By implementing a
** custom page cache using this API, an application can better control
** the amount of memory consumed by SQLite, the way in which
** that memory is allocated and released, and the policies used to
** determine exactly which parts of a database file are cached and for
** how long.
**
** The alternative page cache mechanism is an
** extreme measure that is only needed by the most demanding applications.
** The built-in page cache is recommended for most uses.
**
** ^(The contents of the cortex_pcache_methods2 structure are copied to an
** internal buffer by CORTEX within the call to [cortex_config].  Hence
** the application may discard the parameter after the call to
** [cortex_config()] returns.)^
**
** [[the xInit() page cache method]]
** ^(The xInit() method is called once for each effective
** call to [cortex_initialize()])^
** (usually only once during the lifetime of the process). ^(The xInit()
** method is passed a copy of the cortex_pcache_methods2.pArg value.)^
** The intent of the xInit() method is to set up global data structures
** required by the custom page cache implementation.
** ^(If the xInit() method is NULL, then the
** built-in default page cache is used instead of the application defined
** page cache.)^
**
** [[the xShutdown() page cache method]]
** ^The xShutdown() method is called by [cortex_shutdown()].
** It can be used to clean up
** any outstanding resources before process shutdown, if required.
** ^The xShutdown() method may be NULL.
**
** ^SQLite automatically serializes calls to the xInit method,
** so the xInit method need not be threadsafe.  ^The
** xShutdown method is only called from [cortex_shutdown()] so it does
** not need to be threadsafe either.  All other methods must be threadsafe
** in multithreaded applications.
**
** ^SQLite will never invoke xInit() more than once without an intervening
** call to xShutdown().
**
** [[the xCreate() page cache methods]]
** ^SQLite invokes the xCreate() method to construct a new cache instance.
** CORTEX will typically create one cache instance for each open database file,
** though this is not guaranteed. ^The
** first parameter, szPage, is the size in bytes of the pages that must
** be allocated by the cache.  ^szPage will always be a power of two.  ^The
** second parameter szExtra is a number of bytes of extra storage
** associated with each page cache entry.  ^The szExtra parameter will be
** a number less than 250.  CORTEX will use the
** extra szExtra bytes on each page to store metadata about the underlying
** database page on disk.  The value passed into szExtra depends
** on the CORTEX version, the target platform, and how CORTEX was compiled.
** ^The third argument to xCreate(), bPurgeable, is true if the cache being
** created will be used to cache database pages of a file stored on disk, or
** false if it is used for an in-memory database. The cache implementation
** does not have to do anything special based upon the value of bPurgeable;
** it is purely advisory.  ^On a cache where bPurgeable is false, CORTEX will
** never invoke xUnpin() except to deliberately delete a page.
** ^In other words, calls to xUnpin() on a cache with bPurgeable set to
** false will always have the "discard" flag set to true.
** ^Hence, a cache created with bPurgeable set to false will
** never contain any unpinned pages.
**
** [[the xCachesize() page cache method]]
** ^(The xCachesize() method may be called at any time by CORTEX to set the
** suggested maximum cache-size (number of pages stored) for the cache
** instance passed as the first argument. This is the value configured using
** the CORTEX "[PRAGMA cache_size]" command.)^  As with the bPurgeable
** parameter, the implementation is not required to do anything with this
** value; it is advisory only.
**
** [[the xPagecount() page cache methods]]
** The xPagecount() method must return the number of pages currently
** stored in the cache, both pinned and unpinned.
**
** [[the xFetch() page cache methods]]
** The xFetch() method locates a page in the cache and returns a pointer to
** an cortex_pcache_page object associated with that page, or a NULL pointer.
** The pBuf element of the returned cortex_pcache_page object will be a
** pointer to a buffer of szPage bytes used to store the content of a
** single database page.  The pExtra element of cortex_pcache_page will be
** a pointer to the szExtra bytes of extra storage that CORTEX has requested
** for each entry in the page cache.
**
** The page to be fetched is determined by the key. ^The minimum key value
** is 1.  After it has been retrieved using xFetch, the page is considered
** to be "pinned".
**
** If the requested page is already in the page cache, then the page cache
** implementation must return a pointer to the page buffer with its content
** intact.  If the requested page is not already in the cache, then the
** cache implementation should use the value of the createFlag
** parameter to help it determine what action to take:
**
** <table border=1 width=85% align=center>
** <tr><th> createFlag <th> Behavior when page is not already in cache
** <tr><td> 0 <td> Do not allocate a new page.  Return NULL.
** <tr><td> 1 <td> Allocate a new page if it is easy and convenient to do so.
**                 Otherwise return NULL.
** <tr><td> 2 <td> Make every effort to allocate a new page.  Only return
**                 NULL if allocating a new page is effectively impossible.
** </table>
**
** ^(SQLite will normally invoke xFetch() with a createFlag of 0 or 1.  SQLite
** will only use a createFlag of 2 after a prior call with a createFlag of 1
** failed.)^  In between the xFetch() calls, CORTEX may
** attempt to unpin one or more cache pages by spilling the content of
** pinned pages to disk and synching the operating system disk cache.
**
** [[the xUnpin() page cache method]]
** ^xUnpin() is called by CORTEX with a pointer to a currently pinned page
** as its second argument.  If the third parameter, discard, is non-zero,
** then the page must be evicted from the cache.
** ^If the discard parameter is
** zero, then the page may be discarded or retained at the discretion of the
** page cache implementation. ^The page cache implementation
** may choose to evict unpinned pages at any time.
**
** The cache must not perform any reference counting. A single
** call to xUnpin() unpins the page regardless of the number of prior calls
** to xFetch().
**
** [[the xRekey() page cache methods]]
** The xRekey() method is used to change the key value associated with the
** page passed as the second argument. If the cache
** previously contains an entry associated with newKey, it must be
** discarded. ^Any prior cache entry associated with newKey is guaranteed not
** to be pinned.
**
** When CORTEX calls the xTruncate() method, the cache must discard all
** existing cache entries with page numbers (keys) greater than or equal
** to the value of the iLimit parameter passed to xTruncate(). If any
** of these pages are pinned, they become implicitly unpinned, meaning that
** they can be safely discarded.
**
** [[the xDestroy() page cache method]]
** ^The xDestroy() method is used to delete a cache allocated by xCreate().
** All resources associated with the specified cache should be freed. ^After
** calling the xDestroy() method, CORTEX considers the [cortex_pcache*]
** handle invalid, and will not use it with any other cortex_pcache_methods2
** functions.
**
** [[the xShrink() page cache method]]
** ^SQLite invokes the xShrink() method when it wants the page cache to
** free up as much of heap memory as possible.  The page cache implementation
** is not obligated to free any memory, but well-behaved implementations should
** do their best.
*/
typedef struct cortex_pcache_methods2 cortex_pcache_methods2;
struct cortex_pcache_methods2 {
  int iVersion;
  void *pArg;
  int (*xInit)(void*);
  void (*xShutdown)(void*);
  cortex_pcache *(*xCreate)(int szPage, int szExtra, int bPurgeable);
  void (*xCachesize)(cortex_pcache*, int nCachesize);
  int (*xPagecount)(cortex_pcache*);
  cortex_pcache_page *(*xFetch)(cortex_pcache*, unsigned key, int createFlag);
  void (*xUnpin)(cortex_pcache*, cortex_pcache_page*, int discard);
  void (*xRekey)(cortex_pcache*, cortex_pcache_page*,
      unsigned oldKey, unsigned newKey);
  void (*xTruncate)(cortex_pcache*, unsigned iLimit);
  void (*xDestroy)(cortex_pcache*);
  void (*xShrink)(cortex_pcache*);
};

/*
** This is the obsolete pcache_methods object that has now been replaced
** by cortex_pcache_methods2.  This object is not used by SQLite.  It is
** retained in the header file for backwards compatibility only.
*/
typedef struct cortex_pcache_methods cortex_pcache_methods;
struct cortex_pcache_methods {
  void *pArg;
  int (*xInit)(void*);
  void (*xShutdown)(void*);
  cortex_pcache *(*xCreate)(int szPage, int bPurgeable);
  void (*xCachesize)(cortex_pcache*, int nCachesize);
  int (*xPagecount)(cortex_pcache*);
  void *(*xFetch)(cortex_pcache*, unsigned key, int createFlag);
  void (*xUnpin)(cortex_pcache*, void*, int discard);
  void (*xRekey)(cortex_pcache*, void*, unsigned oldKey, unsigned newKey);
  void (*xTruncate)(cortex_pcache*, unsigned iLimit);
  void (*xDestroy)(cortex_pcache*);
};


/*
** CAPI3REF: Online Backup Object
**
** The cortex_backup object records state information about an ongoing
** online backup operation.  ^The cortex_backup object is created by
** a call to [cortex_backup_init()] and is destroyed by a call to
** [cortex_backup_finish()].
**
** See Also: [Using the CORTEX Online Backup API]
*/
typedef struct cortex_backup cortex_backup;

/*
** CAPI3REF: Online Backup API.
**
** The backup API copies the content of one database into another.
** It is useful either for creating backups of databases or
** for copying in-memory databases to or from persistent files.
**
** See Also: [Using the CORTEX Online Backup API]
**
** ^SQLite holds a write transaction open on the destination database file
** for the duration of the backup operation.
** ^The source database is read-locked only while it is being read;
** it is not locked continuously for the entire backup operation.
** ^Thus, the backup may be performed on a live source database without
** preventing other database connections from
** reading or writing to the source database while the backup is underway.
**
** ^(To perform a backup operation:
**   <ol>
**     <li><b>cortex_backup_init()</b> is called once to initialize the
**         backup,
**     <li><b>cortex_backup_step()</b> is called one or more times to transfer
**         the data between the two databases, and finally
**     <li><b>cortex_backup_finish()</b> is called to release all resources
**         associated with the backup operation.
**   </ol>)^
** There should be exactly one call to cortex_backup_finish() for each
** successful call to cortex_backup_init().
**
** [[cortex_backup_init()]] <b>cortex_backup_init()</b>
**
** ^The D and N arguments to cortex_backup_init(D,N,S,M) are the
** [database connection] associated with the destination database
** and the database name, respectively.
** ^The database name is "main" for the main database, "temp" for the
** temporary database, or the name specified after the AS keyword in
** an [ATTACH] statement for an attached database.
** ^The S and M arguments passed to
** cortex_backup_init(D,N,S,M) identify the [database connection]
** and database name of the source database, respectively.
** ^The source and destination [database connections] (parameters S and D)
** must be different or else cortex_backup_init(D,N,S,M) will fail with
** an error.
**
** ^A call to cortex_backup_init() will fail, returning NULL, if
** there is already a read or read-write transaction open on the
** destination database.
**
** ^If an error occurs within cortex_backup_init(D,N,S,M), then NULL is
** returned and an error code and error message are stored in the
** destination [database connection] D.
** ^The error code and message for the failed call to cortex_backup_init()
** can be retrieved using the [cortex_errcode()], [cortex_errmsg()], and/or
** [cortex_errmsg16()] functions.
** ^A successful call to cortex_backup_init() returns a pointer to an
** [cortex_backup] object.
** ^The [cortex_backup] object may be used with the cortex_backup_step() and
** cortex_backup_finish() functions to perform the specified backup
** operation.
**
** [[cortex_backup_step()]] <b>cortex_backup_step()</b>
**
** ^Function cortex_backup_step(B,N) will copy up to N pages between
** the source and destination databases specified by [cortex_backup] object B.
** ^If N is negative, all remaining source pages are copied.
** ^If cortex_backup_step(B,N) successfully copies N pages and there
** are still more pages to be copied, then the function returns [CORTEX_OK].
** ^If cortex_backup_step(B,N) successfully finishes copying all pages
** from source to destination, then it returns [CORTEX_DONE].
** ^If an error occurs while running cortex_backup_step(B,N),
** then an [error code] is returned. ^As well as [CORTEX_OK] and
** [CORTEX_DONE], a call to cortex_backup_step() may return [CORTEX_READONLY],
** [CORTEX_NOMEM], [CORTEX_BUSY], [CORTEX_LOCKED], or an
** [CORTEX_IOERR_ACCESS | CORTEX_IOERR_XXX] extended error code.
**
** ^(The cortex_backup_step() might return [CORTEX_READONLY] if
** <ol>
** <li> the destination database was opened read-only, or
** <li> the destination database is using write-ahead-log journaling
** and the destination and source page sizes differ, or
** <li> the destination database is an in-memory database and the
** destination and source page sizes differ.
** </ol>)^
**
** ^If cortex_backup_step() cannot obtain a required file-system lock, then
** the [cortex_busy_handler | busy-handler function]
** is invoked (if one is specified). ^If the
** busy-handler returns non-zero before the lock is available, then
** [CORTEX_BUSY] is returned to the caller. ^In this case the call to
** cortex_backup_step() can be retried later. ^If the source
** [database connection]
** is being used to write to the source database when cortex_backup_step()
** is called, then [CORTEX_LOCKED] is returned immediately. ^Again, in this
** case the call to cortex_backup_step() can be retried later on. ^(If
** [CORTEX_IOERR_ACCESS | CORTEX_IOERR_XXX], [CORTEX_NOMEM], or
** [CORTEX_READONLY] is returned, then
** there is no point in retrying the call to cortex_backup_step(). These
** errors are considered fatal.)^  The application must accept
** that the backup operation has failed and pass the backup operation handle
** to the cortex_backup_finish() to release associated resources.
**
** ^The first call to cortex_backup_step() obtains an exclusive lock
** on the destination file. ^The exclusive lock is not released until either
** cortex_backup_finish() is called or the backup operation is complete
** and cortex_backup_step() returns [CORTEX_DONE].  ^Every call to
** cortex_backup_step() obtains a [shared lock] on the source database that
** lasts for the duration of the cortex_backup_step() call.
** ^Because the source database is not locked between calls to
** cortex_backup_step(), the source database may be modified mid-way
** through the backup process.  ^If the source database is modified by an
** external process or via a database connection other than the one being
** used by the backup operation, then the backup will be automatically
** restarted by the next call to cortex_backup_step(). ^If the source
** database is modified by using the same database connection as is used
** by the backup operation, then the backup database is automatically
** updated at the same time.
**
** [[cortex_backup_finish()]] <b>cortex_backup_finish()</b>
**
** When cortex_backup_step() has returned [CORTEX_DONE], or when the
** application wishes to abandon the backup operation, the application
** should destroy the [cortex_backup] by passing it to cortex_backup_finish().
** ^The cortex_backup_finish() interfaces releases all
** resources associated with the [cortex_backup] object.
** ^If cortex_backup_step() has not yet returned [CORTEX_DONE], then any
** active write-transaction on the destination database is rolled back.
** The [cortex_backup] object is invalid
** and may not be used following a call to cortex_backup_finish().
**
** ^The value returned by cortex_backup_finish is [CORTEX_OK] if no
** cortex_backup_step() errors occurred, regardless of whether or not
** cortex_backup_step() completed.
** ^If an out-of-memory condition or IO error occurred during any prior
** cortex_backup_step() call on the same [cortex_backup] object, then
** cortex_backup_finish() returns the corresponding [error code].
**
** ^A return of [CORTEX_BUSY] or [CORTEX_LOCKED] from cortex_backup_step()
** is not a permanent error and does not affect the return value of
** cortex_backup_finish().
**
** [[cortex_backup_remaining()]] [[cortex_backup_pagecount()]]
** <b>cortex_backup_remaining() and cortex_backup_pagecount()</b>
**
** ^The cortex_backup_remaining() routine returns the number of pages still
** to be backed up at the conclusion of the most recent cortex_backup_step().
** ^The cortex_backup_pagecount() routine returns the total number of pages
** in the source database at the conclusion of the most recent
** cortex_backup_step().
** ^(The values returned by these functions are only updated by
** cortex_backup_step(). If the source database is modified in a way that
** changes the size of the source database or the number of pages remaining,
** those changes are not reflected in the output of cortex_backup_pagecount()
** and cortex_backup_remaining() until after the next
** cortex_backup_step().)^
**
** <b>Concurrent Usage of Database Handles</b>
**
** ^The source [database connection] may be used by the application for other
** purposes while a backup operation is underway or being initialized.
** ^If CORTEX is compiled and configured to support threadsafe database
** connections, then the source database connection may be used concurrently
** from within other threads.
**
** However, the application must guarantee that the destination
** [database connection] is not passed to any other API (by any thread) after
** cortex_backup_init() is called and before the corresponding call to
** cortex_backup_finish().  CORTEX does not currently check to see
** if the application incorrectly accesses the destination [database connection]
** and so no error code is reported, but the operations may malfunction
** nevertheless.  Use of the destination database connection while a
** backup is in progress might also cause a mutex deadlock.
**
** If running in [shared cache mode], the application must
** guarantee that the shared cache used by the destination database
** is not accessed while the backup is running. In practice this means
** that the application must guarantee that the disk file being
** backed up to is not accessed by any connection within the process,
** not just the specific connection that was passed to cortex_backup_init().
**
** The [cortex_backup] object itself is partially threadsafe. Multiple
** threads may safely make multiple concurrent calls to cortex_backup_step().
** However, the cortex_backup_remaining() and cortex_backup_pagecount()
** APIs are not strictly speaking threadsafe. If they are invoked at the
** same time as another thread is invoking cortex_backup_step() it is
** possible that they return invalid values.
**
** <b>Alternatives To Using The Backup API</b>
**
** Other techniques for safely creating a consistent backup of an SQLite
** database include:
**
** <ul>
** <li> The [VACUUM INTO] command.
** <li> The [cortex_rsync] utility program.
** </ul>
*/
CORTEX_API cortex_backup *cortex_backup_init(
  cortex *pDest,                        /* Destination database handle */
  const char *zDestName,                 /* Destination database name */
  cortex *pSource,                      /* Source database handle */
  const char *zSourceName                /* Source database name */
);
CORTEX_API int cortex_backup_step(cortex_backup *p, int nPage);
CORTEX_API int cortex_backup_finish(cortex_backup *p);
CORTEX_API int cortex_backup_remaining(cortex_backup *p);
CORTEX_API int cortex_backup_pagecount(cortex_backup *p);

/*
** CAPI3REF: Unlock Notification
** METHOD: cortex
**
** ^When running in shared-cache mode, a database operation may fail with
** an [CORTEX_LOCKED] error if the required locks on the shared-cache or
** individual tables within the shared-cache cannot be obtained. See
** [SQLite Shared-Cache Mode] for a description of shared-cache locking.
** ^This API may be used to register a callback that CORTEX will invoke
** when the connection currently holding the required lock relinquishes it.
** ^This API is only available if the library was compiled with the
** [CORTEX_ENABLE_UNLOCK_NOTIFY] C-preprocessor symbol defined.
**
** See Also: [Using the CORTEX Unlock Notification Feature].
**
** ^Shared-cache locks are released when a database connection concludes
** its current transaction, either by committing it or rolling it back.
**
** ^When a connection (known as the blocked connection) fails to obtain a
** shared-cache lock and CORTEX_LOCKED is returned to the caller, the
** identity of the database connection (the blocking connection) that
** has locked the required resource is stored internally. ^After an
** application receives an CORTEX_LOCKED error, it may call the
** cortex_unlock_notify() method with the blocked connection handle as
** the first argument to register for a callback that will be invoked
** when the blocking connection's current transaction is concluded. ^The
** callback is invoked from within the [cortex_step] or [cortex_close]
** call that concludes the blocking connection's transaction.
**
** ^(If cortex_unlock_notify() is called in a multi-threaded application,
** there is a chance that the blocking connection will have already
** concluded its transaction by the time cortex_unlock_notify() is invoked.
** If this happens, then the specified callback is invoked immediately,
** from within the call to cortex_unlock_notify().)^
**
** ^If the blocked connection is attempting to obtain a write-lock on a
** shared-cache table, and more than one other connection currently holds
** a read-lock on the same table, then CORTEX arbitrarily selects one of
** the other connections to use as the blocking connection.
**
** ^(There may be at most one unlock-notify callback registered by a
** blocked connection. If cortex_unlock_notify() is called when the
** blocked connection already has a registered unlock-notify callback,
** then the new callback replaces the old.)^ ^If cortex_unlock_notify() is
** called with a NULL pointer as its second argument, then any existing
** unlock-notify callback is canceled. ^The blocked connection's
** unlock-notify callback may also be canceled by closing the blocked
** connection using [cortex_close()].
**
** The unlock-notify callback is not reentrant. If an application invokes
** any cortex_xxx API functions from within an unlock-notify callback, a
** crash or deadlock may be the result.
**
** ^Unless deadlock is detected (see below), cortex_unlock_notify() always
** returns CORTEX_OK.
**
** <b>Callback Invocation Details</b>
**
** When an unlock-notify callback is registered, the application provides a
** single void* pointer that is passed to the callback when it is invoked.
** However, the signature of the callback function allows CORTEX to pass
** it an array of void* context pointers. The first argument passed to
** an unlock-notify callback is a pointer to an array of void* pointers,
** and the second is the number of entries in the array.
**
** When a blocking connection's transaction is concluded, there may be
** more than one blocked connection that has registered for an unlock-notify
** callback. ^If two or more such blocked connections have specified the
** same callback function, then instead of invoking the callback function
** multiple times, it is invoked once with the set of void* context pointers
** specified by the blocked connections bundled together into an array.
** This gives the application an opportunity to prioritize any actions
** related to the set of unblocked database connections.
**
** <b>Deadlock Detection</b>
**
** Assuming that after registering for an unlock-notify callback a
** database waits for the callback to be issued before taking any further
** action (a reasonable assumption), then using this API may cause the
** application to deadlock. For example, if connection X is waiting for
** connection Y's transaction to be concluded, and similarly connection
** Y is waiting on connection X's transaction, then neither connection
** will proceed and the system may remain deadlocked indefinitely.
**
** To avoid this scenario, the cortex_unlock_notify() performs deadlock
** detection. ^If a given call to cortex_unlock_notify() would put the
** system in a deadlocked state, then CORTEX_LOCKED is returned and no
** unlock-notify callback is registered. The system is said to be in
** a deadlocked state if connection A has registered for an unlock-notify
** callback on the conclusion of connection B's transaction, and connection
** B has itself registered for an unlock-notify callback when connection
** A's transaction is concluded. ^Indirect deadlock is also detected, so
** the system is also considered to be deadlocked if connection B has
** registered for an unlock-notify callback on the conclusion of connection
** C's transaction, where connection C is waiting on connection A. ^Any
** number of levels of indirection are allowed.
**
** <b>The "DROP TABLE" Exception</b>
**
** When a call to [cortex_step()] returns CORTEX_LOCKED, it is almost
** always appropriate to call cortex_unlock_notify(). There is however,
** one exception. When executing a "DROP TABLE" or "DROP INDEX" statement,
** CORTEX checks if there are any currently executing SELECT statements
** that belong to the same connection. If there are, CORTEX_LOCKED is
** returned. In this case there is no "blocking connection", so invoking
** cortex_unlock_notify() results in the unlock-notify callback being
** invoked immediately. If the application then re-attempts the "DROP TABLE"
** or "DROP INDEX" query, an infinite loop might be the result.
**
** One way around this problem is to check the extended error code returned
** by an cortex_step() call. ^(If there is a blocking connection, then the
** extended error code is set to CORTEX_LOCKED_SHAREDCACHE. Otherwise, in
** the special "DROP TABLE/INDEX" case, the extended error code is just
** CORTEX_LOCKED.)^
*/
CORTEX_API int cortex_unlock_notify(
  cortex *pBlocked,                          /* Waiting connection */
  void (*xNotify)(void **apArg, int nArg),    /* Callback function to invoke */
  void *pNotifyArg                            /* Argument to pass to xNotify */
);


/*
** CAPI3REF: String Comparison
**
** ^The [cortex_stricmp()] and [cortex_strnicmp()] APIs allow applications
** and extensions to compare the contents of two buffers containing UTF-8
** strings in a case-independent fashion, using the same definition of "case
** independence" that CORTEX uses internally when comparing identifiers.
*/
CORTEX_API int cortex_stricmp(const char *, const char *);
CORTEX_API int cortex_strnicmp(const char *, const char *, int);

/*
** CAPI3REF: String Globbing
*
** ^The [cortex_strglob(P,X)] interface returns zero if and only if
** string X matches the [GLOB] pattern P.
** ^The definition of [GLOB] pattern matching used in
** [cortex_strglob(P,X)] is the same as for the "X GLOB P" operator in the
** SQL dialect understood by SQLite.  ^The [cortex_strglob(P,X)] function
** is case sensitive.
**
** Note that this routine returns zero on a match and non-zero if the strings
** do not match, the same as [cortex_stricmp()] and [cortex_strnicmp()].
**
** See also: [cortex_strlike()].
*/
CORTEX_API int cortex_strglob(const char *zGlob, const char *zStr);

/*
** CAPI3REF: String LIKE Matching
*
** ^The [cortex_strlike(P,X,E)] interface returns zero if and only if
** string X matches the [LIKE] pattern P with escape character E.
** ^The definition of [LIKE] pattern matching used in
** [cortex_strlike(P,X,E)] is the same as for the "X LIKE P ESCAPE E"
** operator in the SQL dialect understood by SQLite.  ^For "X LIKE P" without
** the ESCAPE clause, set the E parameter of [cortex_strlike(P,X,E)] to 0.
** ^As with the LIKE operator, the [cortex_strlike(P,X,E)] function is case
** insensitive - equivalent upper and lower case ASCII characters match
** one another.
**
** ^The [cortex_strlike(P,X,E)] function matches Unicode characters, though
** only ASCII characters are case folded.
**
** Note that this routine returns zero on a match and non-zero if the strings
** do not match, the same as [cortex_stricmp()] and [cortex_strnicmp()].
**
** See also: [cortex_strglob()].
*/
CORTEX_API int cortex_strlike(const char *zGlob, const char *zStr, unsigned int cEsc);

/*
** CAPI3REF: Error Logging Interface
**
** ^The [cortex_log()] interface writes a message into the [error log]
** established by the [CORTEX_CONFIG_LOG] option to [cortex_config()].
** ^If logging is enabled, the zFormat string and subsequent arguments are
** used with [cortex_snprintf()] to generate the final output string.
**
** The cortex_log() interface is intended for use by extensions such as
** virtual tables, collating functions, and SQL functions.  While there is
** nothing to prevent an application from calling cortex_log(), doing so
** is considered bad form.
**
** The zFormat string must not be NULL.
**
** To avoid deadlocks and other threading problems, the cortex_log() routine
** will not use dynamically allocated memory.  The log message is stored in
** a fixed-length buffer on the stack.  If the log message is longer than
** a few hundred characters, it will be truncated to the length of the
** buffer.
*/
CORTEX_API void cortex_log(int iErrCode, const char *zFormat, ...);

/*
** CAPI3REF: Write-Ahead Log Commit Hook
** METHOD: cortex
**
** ^The [cortex_wal_hook()] function is used to register a callback that
** is invoked each time data is committed to a database in wal mode.
**
** ^(The callback is invoked by CORTEX after the commit has taken place and
** the associated write-lock on the database released)^, so the implementation
** may read, write or [checkpoint] the database as required.
**
** ^The first parameter passed to the callback function when it is invoked
** is a copy of the third parameter passed to cortex_wal_hook() when
** registering the callback. ^The second is a copy of the database handle.
** ^The third parameter is the name of the database that was written to -
** either "main" or the name of an [ATTACH]-ed database. ^The fourth parameter
** is the number of pages currently in the write-ahead log file,
** including those that were just committed.
**
** ^The callback function should normally return [CORTEX_OK].  ^If an error
** code is returned, that error will propagate back up through the
** CORTEX code base to cause the statement that provoked the callback
** to report an error, though the commit will have still occurred. If the
** callback returns [CORTEX_ROW] or [CORTEX_DONE], or if it returns a value
** that does not correspond to any valid CORTEX error code, the results
** are undefined.
**
** ^A single database handle may have at most a single write-ahead log
** callback registered at one time. ^Calling [cortex_wal_hook()]
** replaces the default behavior or previously registered write-ahead
** log callback.
**
** ^The return value is a copy of the third parameter from the
** previous call, if any, or 0.
**
** ^The [cortex_wal_autocheckpoint()] interface and the
** [wal_autocheckpoint pragma] both invoke [cortex_wal_hook()] and
** will overwrite any prior [cortex_wal_hook()] settings.
**
** ^If a write-ahead log callback is set using this function then
** [cortex_wal_checkpoint_v2()] or [PRAGMA wal_checkpoint]
** should be invoked periodically to keep the write-ahead log file
** from growing without bound.
**
** ^Passing a NULL pointer for the callback disables automatic
** checkpointing entirely. To re-enable the default behavior, call
** cortex_wal_autocheckpoint(db,1000) or use [PRAGMA wal_checkpoint].
*/
CORTEX_API void *cortex_wal_hook(
  cortex*,
  int(*)(void *,cortex*,const char*,int),
  void*
);

/*
** CAPI3REF: Configure an auto-checkpoint
** METHOD: cortex
**
** ^The [cortex_wal_autocheckpoint(D,N)] is a wrapper around
** [cortex_wal_hook()] that causes any database on [database connection] D
** to automatically [checkpoint]
** after committing a transaction if there are N or
** more frames in the [write-ahead log] file.  ^Passing zero or
** a negative value as the N parameter disables automatic
** checkpoints entirely.
**
** ^The callback registered by this function replaces any existing callback
** registered using [cortex_wal_hook()].  ^Likewise, registering a callback
** using [cortex_wal_hook()] disables the automatic checkpoint mechanism
** configured by this function.
**
** ^The [wal_autocheckpoint pragma] can be used to invoke this interface
** from SQL.
**
** ^Checkpoints initiated by this mechanism are
** [cortex_wal_checkpoint_v2|PASSIVE].
**
** ^Every new [database connection] defaults to having the auto-checkpoint
** enabled with a threshold of 1000 or [CORTEX_DEFAULT_WAL_AUTOCHECKPOINT]
** pages.
**
** ^The use of this interface is only necessary if the default setting
** is found to be suboptimal for a particular application.
*/
CORTEX_API int cortex_wal_autocheckpoint(cortex *db, int N);

/*
** CAPI3REF: Checkpoint a database
** METHOD: cortex
**
** ^(The cortex_wal_checkpoint(D,X) is equivalent to
** [cortex_wal_checkpoint_v2](D,X,[CORTEX_CHECKPOINT_PASSIVE],0,0).)^
**
** In brief, cortex_wal_checkpoint(D,X) causes the content in the
** [write-ahead log] for database X on [database connection] D to be
** transferred into the database file and for the write-ahead log to
** be reset.  See the [checkpointing] documentation for addition
** information.
**
** This interface used to be the only way to cause a checkpoint to
** occur.  But then the newer and more powerful [cortex_wal_checkpoint_v2()]
** interface was added.  This interface is retained for backwards
** compatibility and as a convenience for applications that need to manually
** start a callback but which do not need the full power (and corresponding
** complication) of [cortex_wal_checkpoint_v2()].
*/
CORTEX_API int cortex_wal_checkpoint(cortex *db, const char *zDb);

/*
** CAPI3REF: Checkpoint a database
** METHOD: cortex
**
** ^(The cortex_wal_checkpoint_v2(D,X,M,L,C) interface runs a checkpoint
** operation on database X of [database connection] D in mode M.  Status
** information is written back into integers pointed to by L and C.)^
** ^(The M parameter must be a valid [checkpoint mode]:)^
**
** <dl>
** <dt>CORTEX_CHECKPOINT_PASSIVE<dd>
**   ^Checkpoint as many frames as possible without waiting for any database
**   readers or writers to finish, then sync the database file if all frames
**   in the log were checkpointed. ^The [busy-handler callback]
**   is never invoked in the CORTEX_CHECKPOINT_PASSIVE mode.
**   ^On the other hand, passive mode might leave the checkpoint unfinished
**   if there are concurrent readers or writers.
**
** <dt>CORTEX_CHECKPOINT_FULL<dd>
**   ^This mode blocks (it invokes the
**   [cortex_busy_handler|busy-handler callback]) until there is no
**   database writer and all readers are reading from the most recent database
**   snapshot. ^It then checkpoints all frames in the log file and syncs the
**   database file. ^This mode blocks new database writers while it is pending,
**   but new database readers are allowed to continue unimpeded.
**
** <dt>CORTEX_CHECKPOINT_RESTART<dd>
**   ^This mode works the same way as CORTEX_CHECKPOINT_FULL with the addition
**   that after checkpointing the log file it blocks (calls the
**   [busy-handler callback])
**   until all readers are reading from the database file only. ^This ensures
**   that the next writer will restart the log file from the beginning.
**   ^Like CORTEX_CHECKPOINT_FULL, this mode blocks new
**   database writer attempts while it is pending, but does not impede readers.
**
** <dt>CORTEX_CHECKPOINT_TRUNCATE<dd>
**   ^This mode works the same way as CORTEX_CHECKPOINT_RESTART with the
**   addition that it also truncates the log file to zero bytes just prior
**   to a successful return.
**
** <dt>CORTEX_CHECKPOINT_NOOP<dd>
**   ^This mode always checkpoints zero frames. The only reason to invoke
**   a NOOP checkpoint is to access the values returned by
**   cortex_wal_checkpoint_v2() via output parameters *pnLog and *pnCkpt.
** </dl>
**
** ^If pnLog is not NULL, then *pnLog is set to the total number of frames in
** the log file or to -1 if the checkpoint could not run because
** of an error or because the database is not in [WAL mode]. ^If pnCkpt is not
** NULL,then *pnCkpt is set to the total number of checkpointed frames in the
** log file (including any that were already checkpointed before the function
** was called) or to -1 if the checkpoint could not run due to an error or
** because the database is not in WAL mode. ^Note that upon successful
** completion of an CORTEX_CHECKPOINT_TRUNCATE, the log file will have been
** truncated to zero bytes and so both *pnLog and *pnCkpt will be set to zero.
**
** ^All calls obtain an exclusive "checkpoint" lock on the database file. ^If
** any other process is running a checkpoint operation at the same time, the
** lock cannot be obtained and CORTEX_BUSY is returned. ^Even if there is a
** busy-handler configured, it will not be invoked in this case.
**
** ^The CORTEX_CHECKPOINT_FULL, RESTART and TRUNCATE modes also obtain the
** exclusive "writer" lock on the database file. ^If the writer lock cannot be
** obtained immediately, and a busy-handler is configured, it is invoked and
** the writer lock retried until either the busy-handler returns 0 or the lock
** is successfully obtained. ^The busy-handler is also invoked while waiting for
** database readers as described above. ^If the busy-handler returns 0 before
** the writer lock is obtained or while waiting for database readers, the
** checkpoint operation proceeds from that point in the same way as
** CORTEX_CHECKPOINT_PASSIVE - checkpointing as many frames as possible
** without blocking any further. ^CORTEX_BUSY is returned in this case.
**
** ^If parameter zDb is NULL or points to a zero length string, then the
** specified operation is attempted on all WAL databases [attached] to
** [database connection] db.  In this case the
** values written to output parameters *pnLog and *pnCkpt are undefined. ^If
** an CORTEX_BUSY error is encountered when processing one or more of the
** attached WAL databases, the operation is still attempted on any remaining
** attached databases and CORTEX_BUSY is returned at the end. ^If any other
** error occurs while processing an attached database, processing is abandoned
** and the error code is returned to the caller immediately. ^If no error
** (CORTEX_BUSY or otherwise) is encountered while processing the attached
** databases, CORTEX_OK is returned.
**
** ^If database zDb is the name of an attached database that is not in WAL
** mode, CORTEX_OK is returned and both *pnLog and *pnCkpt set to -1. ^If
** zDb is not NULL (or a zero length string) and is not the name of any
** attached database, CORTEX_ERROR is returned to the caller.
**
** ^Unless it returns CORTEX_MISUSE,
** the cortex_wal_checkpoint_v2() interface
** sets the error information that is queried by
** [cortex_errcode()] and [cortex_errmsg()].
**
** ^The [PRAGMA wal_checkpoint] command can be used to invoke this interface
** from SQL.
*/
CORTEX_API int cortex_wal_checkpoint_v2(
  cortex *db,                    /* Database handle */
  const char *zDb,                /* Name of attached database (or NULL) */
  int eMode,                      /* CORTEX_CHECKPOINT_* value */
  int *pnLog,                     /* OUT: Size of WAL log in frames */
  int *pnCkpt                     /* OUT: Total number of frames checkpointed */
);

/*
** CAPI3REF: Checkpoint Mode Values
** KEYWORDS: {checkpoint mode}
**
** These constants define all valid values for the "checkpoint mode" passed
** as the third parameter to the [cortex_wal_checkpoint_v2()] interface.
** See the [cortex_wal_checkpoint_v2()] documentation for details on the
** meaning of each of these checkpoint modes.
*/
#define CORTEX_CHECKPOINT_NOOP    -1  /* Do no work at all */
#define CORTEX_CHECKPOINT_PASSIVE  0  /* Do as much as possible w/o blocking */
#define CORTEX_CHECKPOINT_FULL     1  /* Wait for writers, then checkpoint */
#define CORTEX_CHECKPOINT_RESTART  2  /* Like FULL but wait for readers */
#define CORTEX_CHECKPOINT_TRUNCATE 3  /* Like RESTART but also truncate WAL */

/*
** CAPI3REF: Virtual Table Interface Configuration
**
** This function may be called by either the [xConnect] or [xCreate] method
** of a [virtual table] implementation to configure
** various facets of the virtual table interface.
**
** If this interface is invoked outside the context of an xConnect or
** xCreate virtual table method then the behavior is undefined.
**
** In the call cortex_vtab_config(D,C,...) the D parameter is the
** [database connection] in which the virtual table is being created and
** which is passed in as the first argument to the [xConnect] or [xCreate]
** method that is invoking cortex_vtab_config().  The C parameter is one
** of the [virtual table configuration options].  The presence and meaning
** of parameters after C depend on which [virtual table configuration option]
** is used.
*/
CORTEX_API int cortex_vtab_config(cortex*, int op, ...);

/*
** CAPI3REF: Virtual Table Configuration Options
** KEYWORDS: {virtual table configuration options}
** KEYWORDS: {virtual table configuration option}
**
** These macros define the various options to the
** [cortex_vtab_config()] interface that [virtual table] implementations
** can use to customize and optimize their behavior.
**
** <dl>
** [[CORTEX_VTAB_CONSTRAINT_SUPPORT]]
** <dt>CORTEX_VTAB_CONSTRAINT_SUPPORT</dt>
** <dd>Calls of the form
** [cortex_vtab_config](db,CORTEX_VTAB_CONSTRAINT_SUPPORT,X) are supported,
** where X is an integer.  If X is zero, then the [virtual table] whose
** [xCreate] or [xConnect] method invoked [cortex_vtab_config()] does not
** support constraints.  In this configuration (which is the default) if
** a call to the [xUpdate] method returns [CORTEX_CONSTRAINT], then the entire
** statement is rolled back as if [ON CONFLICT | OR ABORT] had been
** specified as part of the user's SQL statement, regardless of the actual
** ON CONFLICT mode specified.
**
** If X is non-zero, then the virtual table implementation guarantees
** that if [xUpdate] returns [CORTEX_CONSTRAINT], it will do so before
** any modifications to internal or persistent data structures have been made.
** If the [ON CONFLICT] mode is ABORT, FAIL, IGNORE or ROLLBACK, SQLite
** is able to roll back a statement or database transaction, and abandon
** or continue processing the current SQL statement as appropriate.
** If the ON CONFLICT mode is REPLACE and the [xUpdate] method returns
** [CORTEX_CONSTRAINT], CORTEX handles this as if the ON CONFLICT mode
** had been ABORT.
**
** Virtual table implementations that are required to handle OR REPLACE
** must do so within the [xUpdate] method. If a call to the
** [cortex_vtab_on_conflict()] function indicates that the current ON
** CONFLICT policy is REPLACE, the virtual table implementation should
** silently replace the appropriate rows within the xUpdate callback and
** return CORTEX_OK. Or, if this is not possible, it may return
** CORTEX_CONSTRAINT, in which case CORTEX falls back to OR ABORT
** constraint handling.
** </dd>
**
** [[CORTEX_VTAB_DIRECTONLY]]<dt>CORTEX_VTAB_DIRECTONLY</dt>
** <dd>Calls of the form
** [cortex_vtab_config](db,CORTEX_VTAB_DIRECTONLY) from within the
** the [xConnect] or [xCreate] methods of a [virtual table] implementation
** prohibits that virtual table from being used from within triggers and
** views.
** </dd>
**
** [[CORTEX_VTAB_INNOCUOUS]]<dt>CORTEX_VTAB_INNOCUOUS</dt>
** <dd>Calls of the form
** [cortex_vtab_config](db,CORTEX_VTAB_INNOCUOUS) from within the
** [xConnect] or [xCreate] methods of a [virtual table] implementation
** identify that virtual table as being safe to use from within triggers
** and views.  Conceptually, the CORTEX_VTAB_INNOCUOUS tag means that the
** virtual table can do no serious harm even if it is controlled by a
** malicious hacker.  Developers should avoid setting the CORTEX_VTAB_INNOCUOUS
** flag unless absolutely necessary.
** </dd>
**
** [[CORTEX_VTAB_USES_ALL_SCHEMAS]]<dt>CORTEX_VTAB_USES_ALL_SCHEMAS</dt>
** <dd>Calls of the form
** [cortex_vtab_config](db,CORTEX_VTAB_USES_ALL_SCHEMA) from within the
** the [xConnect] or [xCreate] methods of a [virtual table] implementation
** instruct the query planner to begin at least a read transaction on
** all schemas ("main", "temp", and any ATTACH-ed databases) whenever the
** virtual table is used.
** </dd>
** </dl>
*/
#define CORTEX_VTAB_CONSTRAINT_SUPPORT 1
#define CORTEX_VTAB_INNOCUOUS          2
#define CORTEX_VTAB_DIRECTONLY         3
#define CORTEX_VTAB_USES_ALL_SCHEMAS   4

/*
** CAPI3REF: Determine The Virtual Table Conflict Policy
**
** This function may only be called from within a call to the [xUpdate] method
** of a [virtual table] implementation for an INSERT or UPDATE operation. ^The
** value returned is one of [CORTEX_ROLLBACK], [CORTEX_IGNORE], [CORTEX_FAIL],
** [CORTEX_ABORT], or [CORTEX_REPLACE], according to the [ON CONFLICT] mode
** of the SQL statement that triggered the call to the [xUpdate] method of the
** [virtual table].
*/
CORTEX_API int cortex_vtab_on_conflict(cortex *);

/*
** CAPI3REF: Determine If Virtual Table Column Access Is For UPDATE
**
** If the cortex_vtab_nochange(X) routine is called within the [xColumn]
** method of a [virtual table], then it might return true if the
** column is being fetched as part of an UPDATE operation during which the
** column value will not change.  The virtual table implementation can use
** this hint as permission to substitute a return value that is less
** expensive to compute and that the corresponding
** [xUpdate] method understands as a "no-change" value.
**
** If the [xColumn] method calls cortex_vtab_nochange() and finds that
** the column is not changed by the UPDATE statement, then the xColumn
** method can optionally return without setting a result, without calling
** any of the [cortex_result_int|cortex_result_xxxxx() interfaces].
** In that case, [cortex_value_nochange(X)] will return true for the
** same column in the [xUpdate] method.
**
** The cortex_vtab_nochange() routine is an optimization.  Virtual table
** implementations should continue to give a correct answer even if the
** cortex_vtab_nochange() interface were to always return false.  In the
** current implementation, the cortex_vtab_nochange() interface does always
** returns false for the enhanced [UPDATE FROM] statement.
*/
CORTEX_API int cortex_vtab_nochange(cortex_context*);

/*
** CAPI3REF: Determine The Collation For a Virtual Table Constraint
** METHOD: cortex_index_info
**
** This function may only be called from within a call to the [xBestIndex]
** method of a [virtual table].  This function returns a pointer to a string
** that is the name of the appropriate collation sequence to use for text
** comparisons on the constraint identified by its arguments.
**
** The first argument must be the pointer to the [cortex_index_info] object
** that is the first parameter to the xBestIndex() method. The second argument
** must be an index into the aConstraint[] array belonging to the
** cortex_index_info structure passed to xBestIndex.
**
** Important:
** The first parameter must be the same pointer that is passed into the
** xBestMethod() method.  The first parameter may not be a pointer to a
** different [cortex_index_info] object, even an exact copy.
**
** The return value is computed as follows:
**
** <ol>
** <li><p> If the constraint comes from a WHERE clause expression that contains
**         a [COLLATE operator], then the name of the collation specified by
**         that COLLATE operator is returned.
** <li><p> If there is no COLLATE operator, but the column that is the subject
**         of the constraint specifies an alternative collating sequence via
**         a [COLLATE clause] on the column definition within the CREATE TABLE
**         statement that was passed into [cortex_declare_vtab()], then the
**         name of that alternative collating sequence is returned.
** <li><p> Otherwise, "BINARY" is returned.
** </ol>
*/
CORTEX_API const char *cortex_vtab_collation(cortex_index_info*,int);

/*
** CAPI3REF: Determine if a virtual table query is DISTINCT
** METHOD: cortex_index_info
**
** This API may only be used from within an [xBestIndex|xBestIndex method]
** of a [virtual table] implementation. The result of calling this
** interface from outside of xBestIndex() is undefined and probably harmful.
**
** ^The cortex_vtab_distinct() interface returns an integer between 0 and
** 3.  The integer returned by cortex_vtab_distinct()
** gives the virtual table additional information about how the query
** planner wants the output to be ordered. As long as the virtual table
** can meet the ordering requirements of the query planner, it may set
** the "orderByConsumed" flag.
**
** <ol><li value="0"><p>
** ^If the cortex_vtab_distinct() interface returns 0, that means
** that the query planner needs the virtual table to return all rows in the
** sort order defined by the "nOrderBy" and "aOrderBy" fields of the
** [cortex_index_info] object.  This is the default expectation.  If the
** virtual table outputs all rows in sorted order, then it is always safe for
** the xBestIndex method to set the "orderByConsumed" flag, regardless of
** the return value from cortex_vtab_distinct().
** <li value="1"><p>
** ^(If the cortex_vtab_distinct() interface returns 1, that means
** that the query planner does not need the rows to be returned in sorted order
** as long as all rows with the same values in all columns identified by the
** "aOrderBy" field are adjacent.)^  This mode is used when the query planner
** is doing a GROUP BY.
** <li value="2"><p>
** ^(If the cortex_vtab_distinct() interface returns 2, that means
** that the query planner does not need the rows returned in any particular
** order, as long as rows with the same values in all columns identified
** by "aOrderBy" are adjacent.)^  ^(Furthermore, when two or more rows
** contain the same values for all columns identified by "colUsed", all but
** one such row may optionally be omitted from the result.)^
** The virtual table is not required to omit rows that are duplicates
** over the "colUsed" columns, but if the virtual table can do that without
** too much extra effort, it could potentially help the query to run faster.
** This mode is used for a DISTINCT query.
** <li value="3"><p>
** ^(If the cortex_vtab_distinct() interface returns 3, that means the
** virtual table must return rows in the order defined by "aOrderBy" as
** if the cortex_vtab_distinct() interface had returned 0.  However if
** two or more rows in the result have the same values for all columns
** identified by "colUsed", then all but one such row may optionally be
** omitted.)^  Like when the return value is 2, the virtual table
** is not required to omit rows that are duplicates over the "colUsed"
** columns, but if the virtual table can do that without
** too much extra effort, it could potentially help the query to run faster.
** This mode is used for queries
** that have both DISTINCT and ORDER BY clauses.
** </ol>
**
** <p>The following table summarizes the conditions under which the
** virtual table is allowed to set the "orderByConsumed" flag based on
** the value returned by cortex_vtab_distinct().  This table is a
** restatement of the previous four paragraphs:
**
** <table border=1 cellspacing=0 cellpadding=10 width="90%">
** <tr>
** <td valign="top">cortex_vtab_distinct() return value
** <td valign="top">Rows are returned in aOrderBy order
** <td valign="top">Rows with the same value in all aOrderBy columns are adjacent
** <td valign="top">Duplicates over all colUsed columns may be omitted
** <tr><td>0<td>yes<td>yes<td>no
** <tr><td>1<td>no<td>yes<td>no
** <tr><td>2<td>no<td>yes<td>yes
** <tr><td>3<td>yes<td>yes<td>yes
** </table>
**
** ^For the purposes of comparing virtual table output values to see if the
** values are the same value for sorting purposes, two NULL values are considered
** to be the same.  In other words, the comparison operator is "IS"
** (or "IS NOT DISTINCT FROM") and not "==".
**
** If a virtual table implementation is unable to meet the requirements
** specified above, then it must not set the "orderByConsumed" flag in the
** [cortex_index_info] object or an incorrect answer may result.
**
** ^A virtual table implementation is always free to return rows in any order
** it wants, as long as the "orderByConsumed" flag is not set.  ^When the
** "orderByConsumed" flag is unset, the query planner will add extra
** [bytecode] to ensure that the final results returned by the SQL query are
** ordered correctly.  The use of the "orderByConsumed" flag and the
** cortex_vtab_distinct() interface is merely an optimization.  ^Careful
** use of the cortex_vtab_distinct() interface and the "orderByConsumed"
** flag might help queries against a virtual table to run faster.  Being
** overly aggressive and setting the "orderByConsumed" flag when it is not
** valid to do so, on the other hand, might cause CORTEX to return incorrect
** results.
*/
CORTEX_API int cortex_vtab_distinct(cortex_index_info*);

/*
** CAPI3REF: Identify and handle IN constraints in xBestIndex
**
** This interface may only be used from within an
** [xBestIndex|xBestIndex() method] of a [virtual table] implementation.
** The result of invoking this interface from any other context is
** undefined and probably harmful.
**
** ^(A constraint on a virtual table of the form
** "[IN operator|column IN (...)]" is
** communicated to the xBestIndex method as a
** [CORTEX_INDEX_CONSTRAINT_EQ] constraint.)^  If xBestIndex wants to use
** this constraint, it must set the corresponding
** aConstraintUsage[].argvIndex to a positive integer.  ^(Then, under
** the usual mode of handling IN operators, CORTEX generates [bytecode]
** that invokes the [xFilter|xFilter() method] once for each value
** on the right-hand side of the IN operator.)^  Thus the virtual table
** only sees a single value from the right-hand side of the IN operator
** at a time.
**
** In some cases, however, it would be advantageous for the virtual
** table to see all values on the right-hand of the IN operator all at
** once.  The cortex_vtab_in() interfaces facilitates this in two ways:
**
** <ol>
** <li><p>
**   ^A call to cortex_vtab_in(P,N,-1) will return true (non-zero)
**   if and only if the [cortex_index_info|P->aConstraint][N] constraint
**   is an [IN operator] that can be processed all at once.  ^In other words,
**   cortex_vtab_in() with -1 in the third argument is a mechanism
**   by which the virtual table can ask CORTEX if all-at-once processing
**   of the IN operator is even possible.
**
** <li><p>
**   ^A call to cortex_vtab_in(P,N,F) with F==1 or F==0 indicates
**   to CORTEX that the virtual table does or does not want to process
**   the IN operator all-at-once, respectively.  ^Thus when the third
**   parameter (F) is non-negative, this interface is the mechanism by
**   which the virtual table tells CORTEX how it wants to process the
**   IN operator.
** </ol>
**
** ^The cortex_vtab_in(P,N,F) interface can be invoked multiple times
** within the same xBestIndex method call.  ^For any given P,N pair,
** the return value from cortex_vtab_in(P,N,F) will always be the same
** within the same xBestIndex call.  ^If the interface returns true
** (non-zero), that means that the constraint is an IN operator
** that can be processed all-at-once.  ^If the constraint is not an IN
** operator or cannot be processed all-at-once, then the interface returns
** false.
**
** ^(All-at-once processing of the IN operator is selected if both of the
** following conditions are met:
**
** <ol>
** <li><p> The P->aConstraintUsage[N].argvIndex value is set to a positive
** integer.  This is how the virtual table tells CORTEX that it wants to
** use the N-th constraint.
**
** <li><p> The last call to cortex_vtab_in(P,N,F) for which F was
** non-negative had F>=1.
** </ol>)^
**
** ^If either or both of the conditions above are false, then CORTEX uses
** the traditional one-at-a-time processing strategy for the IN constraint.
** ^If both conditions are true, then the argvIndex-th parameter to the
** xFilter method will be an [cortex_value] that appears to be NULL,
** but which can be passed to [cortex_vtab_in_first()] and
** [cortex_vtab_in_next()] to find all values on the right-hand side
** of the IN constraint.
*/
CORTEX_API int cortex_vtab_in(cortex_index_info*, int iCons, int bHandle);

/*
** CAPI3REF: Find all elements on the right-hand side of an IN constraint.
**
** These interfaces are only useful from within the
** [xFilter|xFilter() method] of a [virtual table] implementation.
** The result of invoking these interfaces from any other context
** is undefined and probably harmful.
**
** The X parameter in a call to cortex_vtab_in_first(X,P) or
** cortex_vtab_in_next(X,P) should be one of the parameters to the
** xFilter method which invokes these routines, and specifically
** a parameter that was previously selected for all-at-once IN constraint
** processing using the [cortex_vtab_in()] interface in the
** [xBestIndex|xBestIndex method].  ^(If the X parameter is not
** an xFilter argument that was selected for all-at-once IN constraint
** processing, then these routines return [CORTEX_ERROR].)^
**
** ^(Use these routines to access all values on the right-hand side
** of the IN constraint using code like the following:
**
** <blockquote><pre>
** &nbsp;  for(rc=cortex_vtab_in_first(pList, &pVal);
** &nbsp;      rc==CORTEX_OK && pVal;
** &nbsp;      rc=cortex_vtab_in_next(pList, &pVal)
** &nbsp;  ){
** &nbsp;    // do something with pVal
** &nbsp;  }
** &nbsp;  if( rc!=CORTEX_DONE ){
** &nbsp;    // an error has occurred
** &nbsp;  }
** </pre></blockquote>)^
**
** ^On success, the cortex_vtab_in_first(X,P) and cortex_vtab_in_next(X,P)
** routines return CORTEX_OK and set *P to point to the first or next value
** on the RHS of the IN constraint.  ^If there are no more values on the
** right hand side of the IN constraint, then *P is set to NULL and these
** routines return [CORTEX_DONE].  ^The return value might be
** some other value, such as CORTEX_NOMEM, in the event of a malfunction.
**
** The *ppOut values returned by these routines are only valid until the
** next call to either of these routines or until the end of the xFilter
** method from which these routines were called.  If the virtual table
** implementation needs to retain the *ppOut values for longer, it must make
** copies.  The *ppOut values are [protected cortex_value|protected].
*/
CORTEX_API int cortex_vtab_in_first(cortex_value *pVal, cortex_value **ppOut);
CORTEX_API int cortex_vtab_in_next(cortex_value *pVal, cortex_value **ppOut);

/*
** CAPI3REF: Constraint values in xBestIndex()
** METHOD: cortex_index_info
**
** This API may only be used from within the [xBestIndex|xBestIndex method]
** of a [virtual table] implementation. The result of calling this interface
** from outside of an xBestIndex method are undefined and probably harmful.
**
** ^When the cortex_vtab_rhs_value(P,J,V) interface is invoked from within
** the [xBestIndex] method of a [virtual table] implementation, with P being
** a copy of the [cortex_index_info] object pointer passed into xBestIndex and
** J being a 0-based index into P->aConstraint[], then this routine
** attempts to set *V to the value of the right-hand operand of
** that constraint if the right-hand operand is known.  ^If the
** right-hand operand is not known, then *V is set to a NULL pointer.
** ^The cortex_vtab_rhs_value(P,J,V) interface returns CORTEX_OK if
** and only if *V is set to a value.  ^The cortex_vtab_rhs_value(P,J,V)
** inteface returns CORTEX_NOTFOUND if the right-hand side of the J-th
** constraint is not available.  ^The cortex_vtab_rhs_value() interface
** can return a result code other than CORTEX_OK or CORTEX_NOTFOUND if
** something goes wrong.
**
** The cortex_vtab_rhs_value() interface is usually only successful if
** the right-hand operand of a constraint is a literal value in the original
** SQL statement.  If the right-hand operand is an expression or a reference
** to some other column or a [host parameter], then cortex_vtab_rhs_value()
** will probably return [CORTEX_NOTFOUND].
**
** ^(Some constraints, such as [CORTEX_INDEX_CONSTRAINT_ISNULL] and
** [CORTEX_INDEX_CONSTRAINT_ISNOTNULL], have no right-hand operand.  For such
** constraints, cortex_vtab_rhs_value() always returns CORTEX_NOTFOUND.)^
**
** ^The [cortex_value] object returned in *V is a protected cortex_value
** and remains valid for the duration of the xBestIndex method call.
** ^When xBestIndex returns, the cortex_value object returned by
** cortex_vtab_rhs_value() is automatically deallocated.
**
** The "_rhs_" in the name of this routine is an abbreviation for
** "Right-Hand Side".
*/
CORTEX_API int cortex_vtab_rhs_value(cortex_index_info*, int, cortex_value **ppVal);

/*
** CAPI3REF: Conflict resolution modes
** KEYWORDS: {conflict resolution mode}
**
** These constants are returned by [cortex_vtab_on_conflict()] to
** inform a [virtual table] implementation of the [ON CONFLICT] mode
** for the SQL statement being evaluated.
**
** Note that the [CORTEX_IGNORE] constant is also used as a potential
** return value from the [cortex_set_authorizer()] callback and that
** [CORTEX_ABORT] is also a [result code].
*/
#define CORTEX_ROLLBACK 1
/* #define CORTEX_IGNORE 2 // Also used by cortex_authorizer() callback */
#define CORTEX_FAIL     3
/* #define CORTEX_ABORT 4  // Also an error code */
#define CORTEX_REPLACE  5

/*
** CAPI3REF: Prepared Statement Scan Status Opcodes
** KEYWORDS: {scanstatus options}
**
** The following constants can be used for the T parameter to the
** [cortex_stmt_scanstatus(S,X,T,V)] interface.  Each constant designates a
** different metric for cortex_stmt_scanstatus() to return.
**
** When the value returned to V is a string, space to hold that string is
** managed by the prepared statement S and will be automatically freed when
** S is finalized.
**
** Not all values are available for all query elements. When a value is
** not available, the output variable is set to -1 if the value is numeric,
** or to NULL if it is a string (CORTEX_SCANSTAT_NAME).
**
** <dl>
** [[CORTEX_SCANSTAT_NLOOP]] <dt>CORTEX_SCANSTAT_NLOOP</dt>
** <dd>^The [cortex_int64] variable pointed to by the V parameter will be
** set to the total number of times that the X-th loop has run.</dd>
**
** [[CORTEX_SCANSTAT_NVISIT]] <dt>CORTEX_SCANSTAT_NVISIT</dt>
** <dd>^The [cortex_int64] variable pointed to by the V parameter will be set
** to the total number of rows examined by all iterations of the X-th loop.</dd>
**
** [[CORTEX_SCANSTAT_EST]] <dt>CORTEX_SCANSTAT_EST</dt>
** <dd>^The "double" variable pointed to by the V parameter will be set to the
** query planner's estimate for the average number of rows output from each
** iteration of the X-th loop.  If the query planner's estimate was accurate,
** then this value will approximate the quotient NVISIT/NLOOP and the
** product of this value for all prior loops with the same SELECTID will
** be the NLOOP value for the current loop.</dd>
**
** [[CORTEX_SCANSTAT_NAME]] <dt>CORTEX_SCANSTAT_NAME</dt>
** <dd>^The "const char *" variable pointed to by the V parameter will be set
** to a zero-terminated UTF-8 string containing the name of the index or table
** used for the X-th loop.</dd>
**
** [[CORTEX_SCANSTAT_EXPLAIN]] <dt>CORTEX_SCANSTAT_EXPLAIN</dt>
** <dd>^The "const char *" variable pointed to by the V parameter will be set
** to a zero-terminated UTF-8 string containing the [EXPLAIN QUERY PLAN]
** description for the X-th loop.</dd>
**
** [[CORTEX_SCANSTAT_SELECTID]] <dt>CORTEX_SCANSTAT_SELECTID</dt>
** <dd>^The "int" variable pointed to by the V parameter will be set to the
** id for the X-th query plan element. The id value is unique within the
** statement. The select-id is the same value as is output in the first
** column of an [EXPLAIN QUERY PLAN] query.</dd>
**
** [[CORTEX_SCANSTAT_PARENTID]] <dt>CORTEX_SCANSTAT_PARENTID</dt>
** <dd>The "int" variable pointed to by the V parameter will be set to the
** id of the parent of the current query element, if applicable, or
** to zero if the query element has no parent. This is the same value as
** returned in the second column of an [EXPLAIN QUERY PLAN] query.</dd>
**
** [[CORTEX_SCANSTAT_NCYCLE]] <dt>CORTEX_SCANSTAT_NCYCLE</dt>
** <dd>The cortex_int64 output value is set to the number of cycles,
** according to the processor time-stamp counter, that elapsed while the
** query element was being processed. This value is not available for
** all query elements - if it is unavailable the output variable is
** set to -1.</dd>
** </dl>
*/
#define CORTEX_SCANSTAT_NLOOP    0
#define CORTEX_SCANSTAT_NVISIT   1
#define CORTEX_SCANSTAT_EST      2
#define CORTEX_SCANSTAT_NAME     3
#define CORTEX_SCANSTAT_EXPLAIN  4
#define CORTEX_SCANSTAT_SELECTID 5
#define CORTEX_SCANSTAT_PARENTID 6
#define CORTEX_SCANSTAT_NCYCLE   7

/*
** CAPI3REF: Prepared Statement Scan Status
** METHOD: cortex_stmt
**
** These interfaces return information about the predicted and measured
** performance for pStmt.  Advanced applications can use this
** interface to compare the predicted and the measured performance and
** issue warnings and/or rerun [ANALYZE] if discrepancies are found.
**
** Since this interface is expected to be rarely used, it is only
** available if CORTEX is compiled using the [CORTEX_ENABLE_STMT_SCANSTATUS]
** compile-time option.
**
** The "iScanStatusOp" parameter determines which status information to return.
** The "iScanStatusOp" must be one of the [scanstatus options] or the behavior
** of this interface is undefined. ^The requested measurement is written into
** a variable pointed to by the "pOut" parameter.
**
** The "flags" parameter must be passed a mask of flags. At present only
** one flag is defined - CORTEX_SCANSTAT_COMPLEX. If CORTEX_SCANSTAT_COMPLEX
** is specified, then status information is available for all elements
** of a query plan that are reported by "EXPLAIN QUERY PLAN" output. If
** CORTEX_SCANSTAT_COMPLEX is not specified, then only query plan elements
** that correspond to query loops (the "SCAN..." and "SEARCH..." elements of
** the EXPLAIN QUERY PLAN output) are available. Invoking API
** cortex_stmt_scanstatus() is equivalent to calling
** cortex_stmt_scanstatus_v2() with a zeroed flags parameter.
**
** Parameter "idx" identifies the specific query element to retrieve statistics
** for. Query elements are numbered starting from zero. A value of -1 may
** retrieve statistics for the entire query. ^If idx is out of range
** - less than -1 or greater than or equal to the total number of query
** elements used to implement the statement - a non-zero value is returned and
** the variable that pOut points to is unchanged.
**
** See also: [cortex_stmt_scanstatus_reset()]
*/
CORTEX_API int cortex_stmt_scanstatus(
  cortex_stmt *pStmt,      /* Prepared statement for which info desired */
  int idx,                  /* Index of loop to report on */
  int iScanStatusOp,        /* Information desired.  CORTEX_SCANSTAT_* */
  void *pOut                /* Result written here */
);
CORTEX_API int cortex_stmt_scanstatus_v2(
  cortex_stmt *pStmt,      /* Prepared statement for which info desired */
  int idx,                  /* Index of loop to report on */
  int iScanStatusOp,        /* Information desired.  CORTEX_SCANSTAT_* */
  int flags,                /* Mask of flags defined below */
  void *pOut                /* Result written here */
);

/*
** CAPI3REF: Prepared Statement Scan Status
** KEYWORDS: {scan status flags}
*/
#define CORTEX_SCANSTAT_COMPLEX 0x0001

/*
** CAPI3REF: Zero Scan-Status Counters
** METHOD: cortex_stmt
**
** ^Zero all [cortex_stmt_scanstatus()] related event counters.
**
** This API is only available if the library is built with pre-processor
** symbol [CORTEX_ENABLE_STMT_SCANSTATUS] defined.
*/
CORTEX_API void cortex_stmt_scanstatus_reset(cortex_stmt*);

/*
** CAPI3REF: Flush caches to disk mid-transaction
** METHOD: cortex
**
** ^If a write-transaction is open on [database connection] D when the
** [cortex_db_cacheflush(D)] interface is invoked, any dirty
** pages in the pager-cache that are not currently in use are written out
** to disk. A dirty page may be in use if a database cursor created by an
** active SQL statement is reading from it, or if it is page 1 of a database
** file (page 1 is always "in use").  ^The [cortex_db_cacheflush(D)]
** interface flushes caches for all schemas - "main", "temp", and
** any [attached] databases.
**
** ^If this function needs to obtain extra database locks before dirty pages
** can be flushed to disk, it does so. ^If those locks cannot be obtained
** immediately and there is a busy-handler callback configured, it is invoked
** in the usual manner. ^If the required lock still cannot be obtained, then
** the database is skipped and an attempt made to flush any dirty pages
** belonging to the next (if any) database. ^If any databases are skipped
** because locks cannot be obtained, but no other error occurs, this
** function returns CORTEX_BUSY.
**
** ^If any other error occurs while flushing dirty pages to disk (for
** example an IO error or out-of-memory condition), then processing is
** abandoned and an CORTEX [error code] is returned to the caller immediately.
**
** ^Otherwise, if no error occurs, [cortex_db_cacheflush()] returns CORTEX_OK.
**
** ^This function does not set the database handle error code or message
** returned by the [cortex_errcode()] and [cortex_errmsg()] functions.
*/
CORTEX_API int cortex_db_cacheflush(cortex*);

/*
** CAPI3REF: The pre-update hook.
** METHOD: cortex
**
** ^These interfaces are only available if CORTEX is compiled using the
** [CORTEX_ENABLE_PREUPDATE_HOOK] compile-time option.
**
** ^The [cortex_preupdate_hook()] interface registers a callback function
** that is invoked prior to each [INSERT], [UPDATE], and [DELETE] operation
** on a database table.
** ^At most one preupdate hook may be registered at a time on a single
** [database connection]; each call to [cortex_preupdate_hook()] overrides
** the previous setting.
** ^The preupdate hook is disabled by invoking [cortex_preupdate_hook()]
** with a NULL pointer as the second parameter.
** ^The third parameter to [cortex_preupdate_hook()] is passed through as
** the first parameter to callbacks.
**
** ^The preupdate hook only fires for changes to real database tables; the
** preupdate hook is not invoked for changes to [virtual tables] or to
** system tables like CORTEX_sequence or CORTEX_stat1.
**
** ^The second parameter to the preupdate callback is a pointer to
** the [database connection] that registered the preupdate hook.
** ^The third parameter to the preupdate callback is one of the constants
** [CORTEX_INSERT], [CORTEX_DELETE], or [CORTEX_UPDATE] to identify the
** kind of update operation that is about to occur.
** ^(The fourth parameter to the preupdate callback is the name of the
** database within the database connection that is being modified.  This
** will be "main" for the main database or "temp" for TEMP tables or
** the name given after the AS keyword in the [ATTACH] statement for attached
** databases.)^
** ^The fifth parameter to the preupdate callback is the name of the
** table that is being modified.
**
** For an UPDATE or DELETE operation on a [rowid table], the sixth
** parameter passed to the preupdate callback is the initial [rowid] of the
** row being modified or deleted. For an INSERT operation on a rowid table,
** or any operation on a WITHOUT ROWID table, the value of the sixth
** parameter is undefined. For an INSERT or UPDATE on a rowid table the
** seventh parameter is the final rowid value of the row being inserted
** or updated. The value of the seventh parameter passed to the callback
** function is not defined for operations on WITHOUT ROWID tables, or for
** DELETE operations on rowid tables.
**
** ^The cortex_preupdate_hook(D,C,P) function returns the P argument from
** the previous call on the same [database connection] D, or NULL for
** the first call on D.
**
** The [cortex_preupdate_old()], [cortex_preupdate_new()],
** [cortex_preupdate_count()], and [cortex_preupdate_depth()] interfaces
** provide additional information about a preupdate event. These routines
** may only be called from within a preupdate callback.  Invoking any of
** these routines from outside of a preupdate callback or with a
** [database connection] pointer that is different from the one supplied
** to the preupdate callback results in undefined and probably undesirable
** behavior.
**
** ^The [cortex_preupdate_count(D)] interface returns the number of columns
** in the row that is being inserted, updated, or deleted.
**
** ^The [cortex_preupdate_old(D,N,P)] interface writes into P a pointer to
** a [protected cortex_value] that contains the value of the Nth column of
** the table row before it is updated.  The N parameter must be between 0
** and one less than the number of columns or the behavior will be
** undefined. This must only be used within CORTEX_UPDATE and CORTEX_DELETE
** preupdate callbacks; if it is used by an CORTEX_INSERT callback then the
** behavior is undefined.  The [cortex_value] that P points to
** will be destroyed when the preupdate callback returns.
**
** ^The [cortex_preupdate_new(D,N,P)] interface writes into P a pointer to
** a [protected cortex_value] that contains the value of the Nth column of
** the table row after it is updated.  The N parameter must be between 0
** and one less than the number of columns or the behavior will be
** undefined. This must only be used within CORTEX_INSERT and CORTEX_UPDATE
** preupdate callbacks; if it is used by an CORTEX_DELETE callback then the
** behavior is undefined.  The [cortex_value] that P points to
** will be destroyed when the preupdate callback returns.
**
** ^The [cortex_preupdate_depth(D)] interface returns 0 if the preupdate
** callback was invoked as a result of a direct insert, update, or delete
** operation; or 1 for inserts, updates, or deletes invoked by top-level
** triggers; or 2 for changes resulting from triggers called by top-level
** triggers; and so forth.
**
** When the [cortex_blob_write()] API is used to update a blob column,
** the pre-update hook is invoked with CORTEX_DELETE, because
** the new values are not yet available. In this case, when a
** callback made with op==CORTEX_DELETE is actually a write using the
** cortex_blob_write() API, the [cortex_preupdate_blobwrite()] returns
** the index of the column being written. In other cases, where the
** pre-update hook is being invoked for some other reason, including a
** regular DELETE, cortex_preupdate_blobwrite() returns -1.
**
** See also:  [cortex_update_hook()]
*/
#if defined(CORTEX_ENABLE_PREUPDATE_HOOK)
CORTEX_API void *cortex_preupdate_hook(
  cortex *db,
  void(*xPreUpdate)(
    void *pCtx,                   /* Copy of third arg to preupdate_hook() */
    cortex *db,                  /* Database handle */
    int op,                       /* CORTEX_UPDATE, DELETE or INSERT */
    char const *zDb,              /* Database name */
    char const *zName,            /* Table name */
    cortex_int64 iKey1,          /* Rowid of row about to be deleted/updated */
    cortex_int64 iKey2           /* New rowid value (for a rowid UPDATE) */
  ),
  void*
);
CORTEX_API int cortex_preupdate_old(cortex *, int, cortex_value **);
CORTEX_API int cortex_preupdate_count(cortex *);
CORTEX_API int cortex_preupdate_depth(cortex *);
CORTEX_API int cortex_preupdate_new(cortex *, int, cortex_value **);
CORTEX_API int cortex_preupdate_blobwrite(cortex *);
#endif

/*
** CAPI3REF: Low-level system error code
** METHOD: cortex
**
** ^Attempt to return the underlying operating system error code or error
** number that caused the most recent I/O error or failure to open a file.
** The return value is OS-dependent.  For example, on unix systems, after
** [cortex_open_v2()] returns [CORTEX_CANTOPEN], this interface could be
** called to get back the underlying "errno" that caused the problem, such
** as ENOSPC, EAUTH, EISDIR, and so forth.
*/
CORTEX_API int cortex_system_errno(cortex*);

/*
** CAPI3REF: Database Snapshot
** KEYWORDS: {snapshot} {cortex_snapshot}
**
** An instance of the snapshot object records the state of a [WAL mode]
** database for some specific point in history.
**
** In [WAL mode], multiple [database connections] that are open on the
** same database file can each be reading a different historical version
** of the database file.  When a [database connection] begins a read
** transaction, that connection sees an unchanging copy of the database
** as it existed for the point in time when the transaction first started.
** Subsequent changes to the database from other connections are not seen
** by the reader until a new read transaction is started.
**
** The cortex_snapshot object records state information about an historical
** version of the database file so that it is possible to later open a new read
** transaction that sees that historical version of the database rather than
** the most recent version.
*/
typedef struct cortex_snapshot {
  unsigned char hidden[48];
} cortex_snapshot;

/*
** CAPI3REF: Record A Database Snapshot
** CONSTRUCTOR: cortex_snapshot
**
** ^The [cortex_snapshot_get(D,S,P)] interface attempts to make a
** new [cortex_snapshot] object that records the current state of
** schema S in database connection D.  ^On success, the
** [cortex_snapshot_get(D,S,P)] interface writes a pointer to the newly
** created [cortex_snapshot] object into *P and returns CORTEX_OK.
** If there is not already a read-transaction open on schema S when
** this function is called, one is opened automatically.
**
** If a read-transaction is opened by this function, then it is guaranteed
** that the returned snapshot object may not be invalidated by a database
** writer or checkpointer until after the read-transaction is closed. This
** is not guaranteed if a read-transaction is already open when this
** function is called. In that case, any subsequent write or checkpoint
** operation on the database may invalidate the returned snapshot handle,
** even while the read-transaction remains open.
**
** The following must be true for this function to succeed. If any of
** the following statements are false when cortex_snapshot_get() is
** called, CORTEX_ERROR is returned. The final value of *P is undefined
** in this case.
**
** <ul>
**   <li> The database handle must not be in [autocommit mode].
**
**   <li> Schema S of [database connection] D must be a [WAL mode] database.
**
**   <li> There must not be a write transaction open on schema S of database
**        connection D.
**
**   <li> One or more transactions must have been written to the current wal
**        file since it was created on disk (by any connection). This means
**        that a snapshot cannot be taken on a wal mode database with no wal
**        file immediately after it is first opened. At least one transaction
**        must be written to it first.
** </ul>
**
** This function may also return CORTEX_NOMEM.  If it is called with the
** database handle in autocommit mode but fails for some other reason,
** whether or not a read transaction is opened on schema S is undefined.
**
** The [cortex_snapshot] object returned from a successful call to
** [cortex_snapshot_get()] must be freed using [cortex_snapshot_free()]
** to avoid a memory leak.
**
** The [cortex_snapshot_get()] interface is only available when the
** [CORTEX_ENABLE_SNAPSHOT] compile-time option is used.
*/
CORTEX_API int cortex_snapshot_get(
  cortex *db,
  const char *zSchema,
  cortex_snapshot **ppSnapshot
);

/*
** CAPI3REF: Start a read transaction on an historical snapshot
** METHOD: cortex_snapshot
**
** ^The [cortex_snapshot_open(D,S,P)] interface either starts a new read
** transaction or upgrades an existing one for schema S of
** [database connection] D such that the read transaction refers to
** historical [snapshot] P, rather than the most recent change to the
** database. ^The [cortex_snapshot_open()] interface returns CORTEX_OK
** on success or an appropriate [error code] if it fails.
**
** ^In order to succeed, the database connection must not be in
** [autocommit mode] when [cortex_snapshot_open(D,S,P)] is called. If there
** is already a read transaction open on schema S, then the database handle
** must have no active statements (SELECT statements that have been passed
** to cortex_step() but not cortex_reset() or cortex_finalize()).
** CORTEX_ERROR is returned if either of these conditions is violated, or
** if schema S does not exist, or if the snapshot object is invalid.
**
** ^A call to cortex_snapshot_open() will fail to open if the specified
** snapshot has been overwritten by a [checkpoint]. In this case
** CORTEX_ERROR_SNAPSHOT is returned.
**
** If there is already a read transaction open when this function is
** invoked, then the same read transaction remains open (on the same
** database snapshot) if CORTEX_ERROR, CORTEX_BUSY or CORTEX_ERROR_SNAPSHOT
** is returned. If another error code - for example CORTEX_PROTOCOL or an
** CORTEX_IOERR error code - is returned, then the final state of the
** read transaction is undefined. If CORTEX_OK is returned, then the
** read transaction is now open on database snapshot P.
**
** ^(A call to [cortex_snapshot_open(D,S,P)] will fail if the
** database connection D does not know that the database file for
** schema S is in [WAL mode].  A database connection might not know
** that the database file is in [WAL mode] if there has been no prior
** I/O on that database connection, or if the database entered [WAL mode]
** after the most recent I/O on the database connection.)^
** (Hint: Run "[PRAGMA application_id]" against a newly opened
** database connection in order to make it ready to use snapshots.)
**
** The [cortex_snapshot_open()] interface is only available when the
** [CORTEX_ENABLE_SNAPSHOT] compile-time option is used.
*/
CORTEX_API int cortex_snapshot_open(
  cortex *db,
  const char *zSchema,
  cortex_snapshot *pSnapshot
);

/*
** CAPI3REF: Destroy a snapshot
** DESTRUCTOR: cortex_snapshot
**
** ^The [cortex_snapshot_free(P)] interface destroys [cortex_snapshot] P.
** The application must eventually free every [cortex_snapshot] object
** using this routine to avoid a memory leak.
**
** The [cortex_snapshot_free()] interface is only available when the
** [CORTEX_ENABLE_SNAPSHOT] compile-time option is used.
*/
CORTEX_API void cortex_snapshot_free(cortex_snapshot*);

/*
** CAPI3REF: Compare the ages of two snapshot handles.
** METHOD: cortex_snapshot
**
** The cortex_snapshot_cmp(P1, P2) interface is used to compare the ages
** of two valid snapshot handles.
**
** If the two snapshot handles are not associated with the same database
** file, the result of the comparison is undefined.
**
** Additionally, the result of the comparison is only valid if both of the
** snapshot handles were obtained by calling cortex_snapshot_get() since the
** last time the wal file was deleted. The wal file is deleted when the
** database is changed back to rollback mode or when the number of database
** clients drops to zero. If either snapshot handle was obtained before the
** wal file was last deleted, the value returned by this function
** is undefined.
**
** Otherwise, this API returns a negative value if P1 refers to an older
** snapshot than P2, zero if the two handles refer to the same database
** snapshot, and a positive value if P1 is a newer snapshot than P2.
**
** This interface is only available if CORTEX is compiled with the
** [CORTEX_ENABLE_SNAPSHOT] option.
*/
CORTEX_API int cortex_snapshot_cmp(
  cortex_snapshot *p1,
  cortex_snapshot *p2
);

/*
** CAPI3REF: Recover snapshots from a wal file
** METHOD: cortex_snapshot
**
** If a [WAL file] remains on disk after all database connections close
** (either through the use of the [CORTEX_FCNTL_PERSIST_WAL] [file control]
** or because the last process to have the database opened exited without
** calling [cortex_close()]) and a new connection is subsequently opened
** on that database and [WAL file], the [cortex_snapshot_open()] interface
** will only be able to open the last transaction added to the WAL file
** even though the WAL file contains other valid transactions.
**
** This function attempts to scan the WAL file associated with database zDb
** of database handle db and make all valid snapshots available to
** cortex_snapshot_open(). It is an error if there is already a read
** transaction open on the database, or if the database is not a WAL mode
** database.
**
** CORTEX_OK is returned if successful, or an CORTEX error code otherwise.
**
** This interface is only available if CORTEX is compiled with the
** [CORTEX_ENABLE_SNAPSHOT] option.
*/
CORTEX_API int cortex_snapshot_recover(cortex *db, const char *zDb);

/*
** CAPI3REF: Serialize a database
**
** The cortex_serialize(D,S,P,F) interface returns a pointer to
** memory that is a serialization of the S database on
** [database connection] D.  If S is a NULL pointer, the main database is used.
** If P is not a NULL pointer, then the size of the database in bytes
** is written into *P.
**
** For an ordinary on-disk database file, the serialization is just a
** copy of the disk file.  For an in-memory database or a "TEMP" database,
** the serialization is the same sequence of bytes which would be written
** to disk if that database were backed up to disk.
**
** The usual case is that cortex_serialize() copies the serialization of
** the database into memory obtained from [cortex_malloc64()] and returns
** a pointer to that memory.  The caller is responsible for freeing the
** returned value to avoid a memory leak.  However, if the F argument
** contains the CORTEX_SERIALIZE_NOCOPY bit, then no memory allocations
** are made, and the cortex_serialize() function will return a pointer
** to the contiguous memory representation of the database that SQLite
** is currently using for that database, or NULL if no such contiguous
** memory representation of the database exists.  A contiguous memory
** representation of the database will usually only exist if there has
** been a prior call to [cortex_deserialize(D,S,...)] with the same
** values of D and S.
** The size of the database is written into *P even if the
** CORTEX_SERIALIZE_NOCOPY bit is set but no contiguous copy
** of the database exists.
**
** After the call, if the CORTEX_SERIALIZE_NOCOPY bit had been set,
** the returned buffer content will remain accessible and unchanged
** until either the next write operation on the connection or when
** the connection is closed, and applications must not modify the
** buffer. If the bit had been clear, the returned buffer will not
** be accessed by CORTEX after the call.
**
** A call to cortex_serialize(D,S,P,F) might return NULL even if the
** CORTEX_SERIALIZE_NOCOPY bit is omitted from argument F if a memory
** allocation error occurs.
**
** This interface is omitted if CORTEX is compiled with the
** [CORTEX_OMIT_DESERIALIZE] option.
*/
CORTEX_API unsigned char *cortex_serialize(
  cortex *db,           /* The database connection */
  const char *zSchema,   /* Which DB to serialize. ex: "main", "temp", ... */
  cortex_int64 *piSize, /* Write size of the DB here, if not NULL */
  unsigned int mFlags    /* Zero or more CORTEX_SERIALIZE_* flags */
);

/*
** CAPI3REF: Flags for cortex_serialize
**
** Zero or more of the following constants can be OR-ed together for
** the F argument to [cortex_serialize(D,S,P,F)].
**
** CORTEX_SERIALIZE_NOCOPY means that [cortex_serialize()] will return
** a pointer to contiguous in-memory database that it is currently using,
** without making a copy of the database.  If CORTEX is not currently using
** a contiguous in-memory database, then this option causes
** [cortex_serialize()] to return a NULL pointer.  CORTEX will only be
** using a contiguous in-memory database if it has been initialized by a
** prior call to [cortex_deserialize()].
*/
#define CORTEX_SERIALIZE_NOCOPY 0x001   /* Do no memory allocations */

/*
** CAPI3REF: Deserialize a database
**
** The cortex_deserialize(D,S,P,N,M,F) interface causes the
** [database connection] D to disconnect from database S and then
** reopen S as an in-memory database based on the serialization
** contained in P.  If S is a NULL pointer, the main database is
** used. The serialized database P is N bytes in size.  M is the size
** of the buffer P, which might be larger than N.  If M is larger than
** N, and the CORTEX_DESERIALIZE_READONLY bit is not set in F, then
** CORTEX is permitted to add content to the in-memory database as
** long as the total size does not exceed M bytes.
**
** If the CORTEX_DESERIALIZE_FREEONCLOSE bit is set in F, then CORTEX will
** invoke cortex_free() on the serialization buffer when the database
** connection closes.  If the CORTEX_DESERIALIZE_RESIZEABLE bit is set, then
** CORTEX will try to increase the buffer size using cortex_realloc64()
** if writes on the database cause it to grow larger than M bytes.
**
** Applications must not modify the buffer P or invalidate it before
** the database connection D is closed.
**
** The cortex_deserialize() interface will fail with CORTEX_BUSY if the
** database is currently in a read transaction or is involved in a backup
** operation.
**
** It is not possible to deserialize into the TEMP database.  If the
** S argument to cortex_deserialize(D,S,P,N,M,F) is "temp" then the
** function returns CORTEX_ERROR.
**
** The deserialized database should not be in [WAL mode].  If the database
** is in WAL mode, then any attempt to use the database file will result
** in an [CORTEX_CANTOPEN] error.  The application can set the
** [file format version numbers] (bytes 18 and 19) of the input database P
** to 0x01 prior to invoking cortex_deserialize(D,S,P,N,M,F) to force the
** database file into rollback mode and work around this limitation.
**
** If cortex_deserialize(D,S,P,N,M,F) fails for any reason and if the
** CORTEX_DESERIALIZE_FREEONCLOSE bit is set in argument F, then
** [cortex_free()] is invoked on argument P prior to returning.
**
** This interface is omitted if CORTEX is compiled with the
** [CORTEX_OMIT_DESERIALIZE] option.
*/
CORTEX_API int cortex_deserialize(
  cortex *db,            /* The database connection */
  const char *zSchema,    /* Which DB to reopen with the deserialization */
  unsigned char *pData,   /* The serialized database content */
  cortex_int64 szDb,     /* Number of bytes in the deserialization */
  cortex_int64 szBuf,    /* Total size of buffer pData[] */
  unsigned mFlags         /* Zero or more CORTEX_DESERIALIZE_* flags */
);

/*
** CAPI3REF: Flags for cortex_deserialize()
**
** The following are allowed values for the 6th argument (the F argument) to
** the [cortex_deserialize(D,S,P,N,M,F)] interface.
**
** The CORTEX_DESERIALIZE_FREEONCLOSE means that the database serialization
** in the P argument is held in memory obtained from [cortex_malloc64()]
** and that CORTEX should take ownership of this memory and automatically
** free it when it has finished using it.  Without this flag, the caller
** is responsible for freeing any dynamically allocated memory.
**
** The CORTEX_DESERIALIZE_RESIZEABLE flag means that CORTEX is allowed to
** grow the size of the database using calls to [cortex_realloc64()].  This
** flag should only be used if CORTEX_DESERIALIZE_FREEONCLOSE is also used.
** Without this flag, the deserialized database cannot increase in size beyond
** the number of bytes specified by the M parameter.
**
** The CORTEX_DESERIALIZE_READONLY flag means that the deserialized database
** should be treated as read-only.
*/
#define CORTEX_DESERIALIZE_FREEONCLOSE 1 /* Call cortex_free() on close */
#define CORTEX_DESERIALIZE_RESIZEABLE  2 /* Resize using cortex_realloc64() */
#define CORTEX_DESERIALIZE_READONLY    4 /* Database is read-only */

/*
** CAPI3REF: Bind array values to the CARRAY table-valued function
**
** The cortex_carray_bind(S,I,P,N,F,X) interface binds an array value to
** one of the first argument of the [carray() table-valued function].  The
** S parameter is a pointer to the [prepared statement] that uses the carray()
** functions.  I is the parameter index to be bound.  P is a pointer to the
** array to be bound, and N is the number of eements in the array.  The
** F argument is one of constants [CORTEX_CARRAY_INT32], [CORTEX_CARRAY_INT64],
** [CORTEX_CARRAY_DOUBLE], [CORTEX_CARRAY_TEXT], or [CORTEX_CARRAY_BLOB] to
** indicate the datatype of the array being bound.  The X argument is not a
** NULL pointer, then CORTEX will invoke the function X on the P parameter
** after it has finished using P, even if the call to
** cortex_carray_bind() fails. The special-case finalizer
** CORTEX_TRANSIENT has no effect here.
*/
CORTEX_API int cortex_carray_bind(
  cortex_stmt *pStmt,        /* Statement to be bound */
  int i,                      /* Parameter index */
  void *aData,                /* Pointer to array data */
  int nData,                  /* Number of data elements */
  int mFlags,                 /* CARRAY flags */
  void (*xDel)(void*)         /* Destructor for aData */
);

/*
** CAPI3REF: Datatypes for the CARRAY table-valued function
**
** The fifth argument to the [cortex_carray_bind()] interface musts be
** one of the following constants, to specify the datatype of the array
** that is being bound into the [carray table-valued function].
*/
#define CORTEX_CARRAY_INT32     0    /* Data is 32-bit signed integers */
#define CORTEX_CARRAY_INT64     1    /* Data is 64-bit signed integers */
#define CORTEX_CARRAY_DOUBLE    2    /* Data is doubles */
#define CORTEX_CARRAY_TEXT      3    /* Data is char* */
#define CORTEX_CARRAY_BLOB      4    /* Data is struct iovec */

/*
** Versions of the above #defines that omit the initial CORTEX_, for
** legacy compatibility.
*/
#define CARRAY_INT32     0    /* Data is 32-bit signed integers */
#define CARRAY_INT64     1    /* Data is 64-bit signed integers */
#define CARRAY_DOUBLE    2    /* Data is doubles */
#define CARRAY_TEXT      3    /* Data is char* */
#define CARRAY_BLOB      4    /* Data is struct iovec */

/*
** Undo the hack that converts floating point types to integer for
** builds on processors without floating point support.
*/
#ifdef CORTEX_OMIT_FLOATING_POINT
# undef double
#endif

#if defined(__wasi__)
# undef CORTEX_WASI
# define CORTEX_WASI 1
# ifndef CORTEX_OMIT_LOAD_EXTENSION
#  define CORTEX_OMIT_LOAD_EXTENSION
# endif
# ifndef CORTEX_THREADSAFE
#  define CORTEX_THREADSAFE 0
# endif
#endif

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
/* #endif for cortex_H will be added by mkcortex.tcl */

/******** Begin file cortexrtree.h *********/
/*
** 2010 August 30
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
*/

#ifndef _cortexRTREE_H_
#define _cortexRTREE_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct cortex_rtree_geometry cortex_rtree_geometry;
typedef struct cortex_rtree_query_info cortex_rtree_query_info;

/* The double-precision datatype used by RTree depends on the
** CORTEX_RTREE_INT_ONLY compile-time option.
*/
#ifdef CORTEX_RTREE_INT_ONLY
  typedef cortex_int64 cortex_rtree_dbl;
#else
  typedef double cortex_rtree_dbl;
#endif

/*
** Register a geometry callback named zGeom that can be used as part of an
** R-Tree geometry query as follows:
**
**   SELECT ... FROM <rtree> WHERE <rtree col> MATCH $zGeom(... params ...)
*/
CORTEX_API int cortex_rtree_geometry_callback(
  cortex *db,
  const char *zGeom,
  int (*xGeom)(cortex_rtree_geometry*, int, cortex_rtree_dbl*,int*),
  void *pContext
);


/*
** A pointer to a structure of the following type is passed as the first
** argument to callbacks registered using rtree_geometry_callback().
*/
struct cortex_rtree_geometry {
  void *pContext;                 /* Copy of pContext passed to s_r_g_c() */
  int nParam;                     /* Size of array aParam[] */
  cortex_rtree_dbl *aParam;      /* Parameters passed to SQL geom function */
  void *pUser;                    /* Callback implementation user data */
  void (*xDelUser)(void *);       /* Called by CORTEX to clean up pUser */
};

/*
** Register a 2nd-generation geometry callback named zScore that can be
** used as part of an R-Tree geometry query as follows:
**
**   SELECT ... FROM <rtree> WHERE <rtree col> MATCH $zQueryFunc(... params ...)
*/
CORTEX_API int cortex_rtree_query_callback(
  cortex *db,
  const char *zQueryFunc,
  int (*xQueryFunc)(cortex_rtree_query_info*),
  void *pContext,
  void (*xDestructor)(void*)
);


/*
** A pointer to a structure of the following type is passed as the
** argument to scored geometry callback registered using
** cortex_rtree_query_callback().
**
** Note that the first 5 fields of this structure are identical to
** cortex_rtree_geometry.  This structure is a subclass of
** cortex_rtree_geometry.
*/
struct cortex_rtree_query_info {
  void *pContext;                   /* pContext from when function registered */
  int nParam;                       /* Number of function parameters */
  cortex_rtree_dbl *aParam;        /* value of function parameters */
  void *pUser;                      /* callback can use this, if desired */
  void (*xDelUser)(void*);          /* function to free pUser */
  cortex_rtree_dbl *aCoord;        /* Coordinates of node or entry to check */
  unsigned int *anQueue;            /* Number of pending entries in the queue */
  int nCoord;                       /* Number of coordinates */
  int iLevel;                       /* Level of current node or entry */
  int mxLevel;                      /* The largest iLevel value in the tree */
  cortex_int64 iRowid;             /* Rowid for current entry */
  cortex_rtree_dbl rParentScore;   /* Score of parent node */
  int eParentWithin;                /* Visibility of parent node */
  int eWithin;                      /* OUT: Visibility */
  cortex_rtree_dbl rScore;         /* OUT: Write the score here */
  /* The following fields are only available in 3.8.11 and later */
  cortex_value **apSqlParam;       /* Original SQL values of parameters */
};

/*
** Allowed values for cortex_rtree_query.eWithin and .eParentWithin.
*/
#define NOT_WITHIN       0   /* Object completely outside of query region */
#define PARTLY_WITHIN    1   /* Object partially overlaps query region */
#define FULLY_WITHIN     2   /* Object fully contained within query region */


#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif  /* ifndef _cortexRTREE_H_ */

/******** End of cortexrtree.h *********/
/******** Begin file cortexsession.h *********/

#if !defined(__SQLITESESSION_H_) && defined(CORTEX_ENABLE_SESSION)
#define __SQLITESESSION_H_ 1

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


/*
** CAPI3REF: Session Object Handle
**
** An instance of this object is a [session] that can be used to
** record changes to a database.
*/
typedef struct cortex_session cortex_session;

/*
** CAPI3REF: Changeset Iterator Handle
**
** An instance of this object acts as a cursor for iterating
** over the elements of a [changeset] or [patchset].
*/
typedef struct cortex_changeset_iter cortex_changeset_iter;

/*
** CAPI3REF: Create A New Session Object
** CONSTRUCTOR: cortex_session
**
** Create a new session object attached to database handle db. If successful,
** a pointer to the new object is written to *ppSession and CORTEX_OK is
** returned. If an error occurs, *ppSession is set to NULL and an SQLite
** error code (e.g. CORTEX_NOMEM) is returned.
**
** It is possible to create multiple session objects attached to a single
** database handle.
**
** Session objects created using this function should be deleted using the
** [cortexsession_delete()] function before the database handle that they
** are attached to is itself closed. If the database handle is closed before
** the session object is deleted, then the results of calling any session
** module function, including [cortexsession_delete()] on the session object
** are undefined.
**
** Because the session module uses the [cortex_preupdate_hook()] API, it
** is not possible for an application to register a pre-update hook on a
** database handle that has one or more session objects attached. Nor is
** it possible to create a session object attached to a database handle for
** which a pre-update hook is already defined. The results of attempting
** either of these things are undefined.
**
** The session object will be used to create changesets for tables in
** database zDb, where zDb is either "main", or "temp", or the name of an
** attached database. It is not an error if database zDb is not attached
** to the database when the session object is created.
*/
CORTEX_API int cortexsession_create(
  cortex *db,                    /* Database handle */
  const char *zDb,                /* Name of db (e.g. "main") */
  cortex_session **ppSession     /* OUT: New session object */
);

/*
** CAPI3REF: Delete A Session Object
** DESTRUCTOR: cortex_session
**
** Delete a session object previously allocated using
** [cortexsession_create()]. Once a session object has been deleted, the
** results of attempting to use pSession with any other session module
** function are undefined.
**
** Session objects must be deleted before the database handle to which they
** are attached is closed. Refer to the documentation for
** [cortexsession_create()] for details.
*/
CORTEX_API void cortexsession_delete(cortex_session *pSession);

/*
** CAPI3REF: Configure a Session Object
** METHOD: cortex_session
**
** This method is used to configure a session object after it has been
** created. At present the only valid values for the second parameter are
** [CORTEX_SESSION_OBJCONFIG_SIZE] and [CORTEX_SESSION_OBJCONFIG_ROWID].
**
*/
CORTEX_API int cortexsession_object_config(cortex_session*, int op, void *pArg);

/*
** CAPI3REF: Options for cortexsession_object_config
**
** The following values may passed as the the 2nd parameter to
** cortexsession_object_config().
**
** <dt>CORTEX_SESSION_OBJCONFIG_SIZE <dd>
**   This option is used to set, clear or query the flag that enables
**   the [cortexsession_changeset_size()] API. Because it imposes some
**   computational overhead, this API is disabled by default. Argument
**   pArg must point to a value of type (int). If the value is initially
**   0, then the cortexsession_changeset_size() API is disabled. If it
**   is greater than 0, then the same API is enabled. Or, if the initial
**   value is less than zero, no change is made. In all cases the (int)
**   variable is set to 1 if the cortexsession_changeset_size() API is
**   enabled following the current call, or 0 otherwise.
**
**   It is an error (CORTEX_MISUSE) to attempt to modify this setting after
**   the first table has been attached to the session object.
**
** <dt>CORTEX_SESSION_OBJCONFIG_ROWID <dd>
**   This option is used to set, clear or query the flag that enables
**   collection of data for tables with no explicit PRIMARY KEY.
**
**   Normally, tables with no explicit PRIMARY KEY are simply ignored
**   by the sessions module. However, if this flag is set, it behaves
**   as if such tables have a column "_rowid_ INTEGER PRIMARY KEY" inserted
**   as their leftmost columns.
**
**   It is an error (CORTEX_MISUSE) to attempt to modify this setting after
**   the first table has been attached to the session object.
*/
#define CORTEX_SESSION_OBJCONFIG_SIZE  1
#define CORTEX_SESSION_OBJCONFIG_ROWID 2

/*
** CAPI3REF: Enable Or Disable A Session Object
** METHOD: cortex_session
**
** Enable or disable the recording of changes by a session object. When
** enabled, a session object records changes made to the database. When
** disabled - it does not. A newly created session object is enabled.
** Refer to the documentation for [cortexsession_changeset()] for further
** details regarding how enabling and disabling a session object affects
** the eventual changesets.
**
** Passing zero to this function disables the session. Passing a value
** greater than zero enables it. Passing a value less than zero is a
** no-op, and may be used to query the current state of the session.
**
** The return value indicates the final state of the session object: 0 if
** the session is disabled, or 1 if it is enabled.
*/
CORTEX_API int cortexsession_enable(cortex_session *pSession, int bEnable);

/*
** CAPI3REF: Set Or Clear the Indirect Change Flag
** METHOD: cortex_session
**
** Each change recorded by a session object is marked as either direct or
** indirect. A change is marked as indirect if either:
**
** <ul>
**   <li> The session object "indirect" flag is set when the change is
**        made, or
**   <li> The change is made by an SQL trigger or foreign key action
**        instead of directly as a result of a users SQL statement.
** </ul>
**
** If a single row is affected by more than one operation within a session,
** then the change is considered indirect if all operations meet the criteria
** for an indirect change above, or direct otherwise.
**
** This function is used to set, clear or query the session object indirect
** flag.  If the second argument passed to this function is zero, then the
** indirect flag is cleared. If it is greater than zero, the indirect flag
** is set. Passing a value less than zero does not modify the current value
** of the indirect flag, and may be used to query the current state of the
** indirect flag for the specified session object.
**
** The return value indicates the final state of the indirect flag: 0 if
** it is clear, or 1 if it is set.
*/
CORTEX_API int cortexsession_indirect(cortex_session *pSession, int bIndirect);

/*
** CAPI3REF: Attach A Table To A Session Object
** METHOD: cortex_session
**
** If argument zTab is not NULL, then it is the name of a table to attach
** to the session object passed as the first argument. All subsequent changes
** made to the table while the session object is enabled will be recorded. See
** documentation for [cortexsession_changeset()] for further details.
**
** Or, if argument zTab is NULL, then changes are recorded for all tables
** in the database. If additional tables are added to the database (by
** executing "CREATE TABLE" statements) after this call is made, changes for
** the new tables are also recorded.
**
** Changes can only be recorded for tables that have a PRIMARY KEY explicitly
** defined as part of their CREATE TABLE statement. It does not matter if the
** PRIMARY KEY is an "INTEGER PRIMARY KEY" (rowid alias) or not. The PRIMARY
** KEY may consist of a single column, or may be a composite key.
**
** It is not an error if the named table does not exist in the database. Nor
** is it an error if the named table does not have a PRIMARY KEY. However,
** no changes will be recorded in either of these scenarios.
**
** Changes are not recorded for individual rows that have NULL values stored
** in one or more of their PRIMARY KEY columns.
**
** CORTEX_OK is returned if the call completes without error. Or, if an error
** occurs, an CORTEX error code (e.g. CORTEX_NOMEM) is returned.
**
** <h3>Special CORTEX_stat1 Handling</h3>
**
** As of CORTEX version 3.22.0, the "CORTEX_stat1" table is an exception to
** some of the rules above. In SQLite, the schema of CORTEX_stat1 is:
**  <pre>
**  &nbsp;     CREATE TABLE CORTEX_stat1(tbl,idx,stat)
**  </pre>
**
** Even though CORTEX_stat1 does not have a PRIMARY KEY, changes are
** recorded for it as if the PRIMARY KEY is (tbl,idx). Additionally, changes
** are recorded for rows for which (idx IS NULL) is true. However, for such
** rows a zero-length blob (SQL value X'') is stored in the changeset or
** patchset instead of a NULL value. This allows such changesets to be
** manipulated by legacy implementations of cortexchangeset_invert(),
** concat() and similar.
**
** The cortexchangeset_apply() function automatically converts the
** zero-length blob back to a NULL value when updating the CORTEX_stat1
** table. However, if the application calls cortexchangeset_new(),
** cortexchangeset_old() or cortexchangeset_conflict on a changeset
** iterator directly (including on a changeset iterator passed to a
** conflict-handler callback) then the X'' value is returned. The application
** must translate X'' to NULL itself if required.
**
** Legacy (older than 3.22.0) versions of the sessions module cannot capture
** changes made to the CORTEX_stat1 table. Legacy versions of the
** cortexchangeset_apply() function silently ignore any modifications to the
** CORTEX_stat1 table that are part of a changeset or patchset.
*/
CORTEX_API int cortexsession_attach(
  cortex_session *pSession,      /* Session object */
  const char *zTab                /* Table name */
);

/*
** CAPI3REF: Set a table filter on a Session Object.
** METHOD: cortex_session
**
** The second argument (xFilter) is the "filter callback". For changes to rows
** in tables that are not attached to the Session object, the filter is called
** to determine whether changes to the table's rows should be tracked or not.
** If xFilter returns 0, changes are not tracked. Note that once a table is
** attached, xFilter will not be called again.
*/
CORTEX_API void cortexsession_table_filter(
  cortex_session *pSession,      /* Session object */
  int(*xFilter)(
    void *pCtx,                   /* Copy of third arg to _filter_table() */
    const char *zTab              /* Table name */
  ),
  void *pCtx                      /* First argument passed to xFilter */
);

/*
** CAPI3REF: Generate A Changeset From A Session Object
** METHOD: cortex_session
**
** Obtain a changeset containing changes to the tables attached to the
** session object passed as the first argument. If successful,
** set *ppChangeset to point to a buffer containing the changeset
** and *pnChangeset to the size of the changeset in bytes before returning
** CORTEX_OK. If an error occurs, set both *ppChangeset and *pnChangeset to
** zero and return an CORTEX error code.
**
** A changeset consists of zero or more INSERT, UPDATE and/or DELETE changes,
** each representing a change to a single row of an attached table. An INSERT
** change contains the values of each field of a new database row. A DELETE
** contains the original values of each field of a deleted database row. An
** UPDATE change contains the original values of each field of an updated
** database row along with the updated values for each updated non-primary-key
** column. It is not possible for an UPDATE change to represent a change that
** modifies the values of primary key columns. If such a change is made, it
** is represented in a changeset as a DELETE followed by an INSERT.
**
** Changes are not recorded for rows that have NULL values stored in one or
** more of their PRIMARY KEY columns. If such a row is inserted or deleted,
** no corresponding change is present in the changesets returned by this
** function. If an existing row with one or more NULL values stored in
** PRIMARY KEY columns is updated so that all PRIMARY KEY columns are non-NULL,
** only an INSERT is appears in the changeset. Similarly, if an existing row
** with non-NULL PRIMARY KEY values is updated so that one or more of its
** PRIMARY KEY columns are set to NULL, the resulting changeset contains a
** DELETE change only.
**
** The contents of a changeset may be traversed using an iterator created
** using the [cortexchangeset_start()] API. A changeset may be applied to
** a database with a compatible schema using the [cortexchangeset_apply()]
** API.
**
** Within a changeset generated by this function, all changes related to a
** single table are grouped together. In other words, when iterating through
** a changeset or when applying a changeset to a database, all changes related
** to a single table are processed before moving on to the next table. Tables
** are sorted in the same order in which they were attached (or auto-attached)
** to the cortex_session object. The order in which the changes related to
** a single table are stored is undefined.
**
** Following a successful call to this function, it is the responsibility of
** the caller to eventually free the buffer that *ppChangeset points to using
** [cortex_free()].
**
** <h3>Changeset Generation</h3>
**
** Once a table has been attached to a session object, the session object
** records the primary key values of all new rows inserted into the table.
** It also records the original primary key and other column values of any
** deleted or updated rows. For each unique primary key value, data is only
** recorded once - the first time a row with said primary key is inserted,
** updated or deleted in the lifetime of the session.
**
** There is one exception to the previous paragraph: when a row is inserted,
** updated or deleted, if one or more of its primary key columns contain a
** NULL value, no record of the change is made.
**
** The session object therefore accumulates two types of records - those
** that consist of primary key values only (created when the user inserts
** a new record) and those that consist of the primary key values and the
** original values of other table columns (created when the users deletes
** or updates a record).
**
** When this function is called, the requested changeset is created using
** both the accumulated records and the current contents of the database
** file. Specifically:
**
** <ul>
**   <li> For each record generated by an insert, the database is queried
**        for a row with a matching primary key. If one is found, an INSERT
**        change is added to the changeset. If no such row is found, no change
**        is added to the changeset.
**
**   <li> For each record generated by an update or delete, the database is
**        queried for a row with a matching primary key. If such a row is
**        found and one or more of the non-primary key fields have been
**        modified from their original values, an UPDATE change is added to
**        the changeset. Or, if no such row is found in the table, a DELETE
**        change is added to the changeset. If there is a row with a matching
**        primary key in the database, but all fields contain their original
**        values, no change is added to the changeset.
** </ul>
**
** This means, amongst other things, that if a row is inserted and then later
** deleted while a session object is active, neither the insert nor the delete
** will be present in the changeset. Or if a row is deleted and then later a
** row with the same primary key values inserted while a session object is
** active, the resulting changeset will contain an UPDATE change instead of
** a DELETE and an INSERT.
**
** When a session object is disabled (see the [cortexsession_enable()] API),
** it does not accumulate records when rows are inserted, updated or deleted.
** This may appear to have some counter-intuitive effects if a single row
** is written to more than once during a session. For example, if a row
** is inserted while a session object is enabled, then later deleted while
** the same session object is disabled, no INSERT record will appear in the
** changeset, even though the delete took place while the session was disabled.
** Or, if one field of a row is updated while a session is enabled, and
** then another field of the same row is updated while the session is disabled,
** the resulting changeset will contain an UPDATE change that updates both
** fields.
*/
CORTEX_API int cortexsession_changeset(
  cortex_session *pSession,      /* Session object */
  int *pnChangeset,               /* OUT: Size of buffer at *ppChangeset */
  void **ppChangeset              /* OUT: Buffer containing changeset */
);

/*
** CAPI3REF: Return An Upper-limit For The Size Of The Changeset
** METHOD: cortex_session
**
** By default, this function always returns 0. For it to return
** a useful result, the cortex_session object must have been configured
** to enable this API using cortexsession_object_config() with the
** CORTEX_SESSION_OBJCONFIG_SIZE verb.
**
** When enabled, this function returns an upper limit, in bytes, for the size
** of the changeset that might be produced if cortexsession_changeset() were
** called. The final changeset size might be equal to or smaller than the
** size in bytes returned by this function.
*/
CORTEX_API cortex_int64 cortexsession_changeset_size(cortex_session *pSession);

/*
** CAPI3REF: Load The Difference Between Tables Into A Session
** METHOD: cortex_session
**
** If it is not already attached to the session object passed as the first
** argument, this function attaches table zTbl in the same manner as the
** [cortexsession_attach()] function. If zTbl does not exist, or if it
** does not have a primary key, this function is a no-op (but does not return
** an error).
**
** Argument zFromDb must be the name of a database ("main", "temp" etc.)
** attached to the same database handle as the session object that contains
** a table compatible with the table attached to the session by this function.
** A table is considered compatible if it:
**
** <ul>
**   <li> Has the same name,
**   <li> Has the same set of columns declared in the same order, and
**   <li> Has the same PRIMARY KEY definition.
** </ul>
**
** If the tables are not compatible, CORTEX_SCHEMA is returned. If the tables
** are compatible but do not have any PRIMARY KEY columns, it is not an error
** but no changes are added to the session object. As with other session
** APIs, tables without PRIMARY KEYs are simply ignored.
**
** This function adds a set of changes to the session object that could be
** used to update the table in database zFrom (call this the "from-table")
** so that its content is the same as the table attached to the session
** object (call this the "to-table"). Specifically:
**
** <ul>
**   <li> For each row (primary key) that exists in the to-table but not in
**     the from-table, an INSERT record is added to the session object.
**
**   <li> For each row (primary key) that exists in the to-table but not in
**     the from-table, a DELETE record is added to the session object.
**
**   <li> For each row (primary key) that exists in both tables, but features
**     different non-PK values in each, an UPDATE record is added to the
**     session.
** </ul>
**
** To clarify, if this function is called and then a changeset constructed
** using [cortexsession_changeset()], then after applying that changeset to
** database zFrom the contents of the two compatible tables would be
** identical.
**
** Unless the call to this function is a no-op as described above, it is an
** error if database zFrom does not exist or does not contain the required
** compatible table.
**
** If the operation is successful, CORTEX_OK is returned. Otherwise, an SQLite
** error code. In this case, if argument pzErrMsg is not NULL, *pzErrMsg
** may be set to point to a buffer containing an English language error
** message. It is the responsibility of the caller to free this buffer using
** cortex_free().
*/
CORTEX_API int cortexsession_diff(
  cortex_session *pSession,
  const char *zFromDb,
  const char *zTbl,
  char **pzErrMsg
);


/*
** CAPI3REF: Generate A Patchset From A Session Object
** METHOD: cortex_session
**
** The differences between a patchset and a changeset are that:
**
** <ul>
**   <li> DELETE records consist of the primary key fields only. The
**        original values of other fields are omitted.
**   <li> The original values of any modified fields are omitted from
**        UPDATE records.
** </ul>
**
** A patchset blob may be used with up to date versions of all
** cortexchangeset_xxx API functions except for cortexchangeset_invert(),
** which returns CORTEX_CORRUPT if it is passed a patchset. Similarly,
** attempting to use a patchset blob with old versions of the
** cortexchangeset_xxx APIs also provokes an CORTEX_CORRUPT error.
**
** Because the non-primary key "old.*" fields are omitted, no
** CORTEX_CHANGESET_DATA conflicts can be detected or reported if a patchset
** is passed to the cortexchangeset_apply() API. Other conflict types work
** in the same way as for changesets.
**
** Changes within a patchset are ordered in the same way as for changesets
** generated by the cortexsession_changeset() function (i.e. all changes for
** a single table are grouped together, tables appear in the order in which
** they were attached to the session object).
*/
CORTEX_API int cortexsession_patchset(
  cortex_session *pSession,      /* Session object */
  int *pnPatchset,                /* OUT: Size of buffer at *ppPatchset */
  void **ppPatchset               /* OUT: Buffer containing patchset */
);

/*
** CAPI3REF: Test if a changeset has recorded any changes.
**
** Return non-zero if no changes to attached tables have been recorded by
** the session object passed as the first argument. Otherwise, if one or
** more changes have been recorded, return zero.
**
** Even if this function returns zero, it is possible that calling
** [cortexsession_changeset()] on the session handle may still return a
** changeset that contains no changes. This can happen when a row in
** an attached table is modified and then later on the original values
** are restored. However, if this function returns non-zero, then it is
** guaranteed that a call to cortexsession_changeset() will return a
** changeset containing zero changes.
*/
CORTEX_API int cortexsession_isempty(cortex_session *pSession);

/*
** CAPI3REF: Query for the amount of heap memory used by a session object.
**
** This API returns the total amount of heap memory in bytes currently
** used by the session object passed as the only argument.
*/
CORTEX_API cortex_int64 cortexsession_memory_used(cortex_session *pSession);

/*
** CAPI3REF: Create An Iterator To Traverse A Changeset
** CONSTRUCTOR: cortex_changeset_iter
**
** Create an iterator used to iterate through the contents of a changeset.
** If successful, *pp is set to point to the iterator handle and CORTEX_OK
** is returned. Otherwise, if an error occurs, *pp is set to zero and an
** CORTEX error code is returned.
**
** The following functions can be used to advance and query a changeset
** iterator created by this function:
**
** <ul>
**   <li> [cortexchangeset_next()]
**   <li> [cortexchangeset_op()]
**   <li> [cortexchangeset_new()]
**   <li> [cortexchangeset_old()]
** </ul>
**
** It is the responsibility of the caller to eventually destroy the iterator
** by passing it to [cortexchangeset_finalize()]. The buffer containing the
** changeset (pChangeset) must remain valid until after the iterator is
** destroyed.
**
** Assuming the changeset blob was created by one of the
** [cortexsession_changeset()], [cortexchangeset_concat()] or
** [cortexchangeset_invert()] functions, all changes within the changeset
** that apply to a single table are grouped together. This means that when
** an application iterates through a changeset using an iterator created by
** this function, all changes that relate to a single table are visited
** consecutively. There is no chance that the iterator will visit a change
** the applies to table X, then one for table Y, and then later on visit
** another change for table X.
**
** The behavior of cortexchangeset_start_v2() and its streaming equivalent
** may be modified by passing a combination of
** [CORTEX_CHANGESETSTART_INVERT | supported flags] as the 4th parameter.
**
** Note that the cortexchangeset_start_v2() API is still <b>experimental</b>
** and therefore subject to change.
*/
CORTEX_API int cortexchangeset_start(
  cortex_changeset_iter **pp,    /* OUT: New changeset iterator handle */
  int nChangeset,                 /* Size of changeset blob in bytes */
  void *pChangeset                /* Pointer to blob containing changeset */
);
CORTEX_API int cortexchangeset_start_v2(
  cortex_changeset_iter **pp,    /* OUT: New changeset iterator handle */
  int nChangeset,                 /* Size of changeset blob in bytes */
  void *pChangeset,               /* Pointer to blob containing changeset */
  int flags                       /* SESSION_CHANGESETSTART_* flags */
);

/*
** CAPI3REF: Flags for cortexchangeset_start_v2
**
** The following flags may passed via the 4th parameter to
** [cortexchangeset_start_v2] and [cortexchangeset_start_v2_strm]:
**
** <dt>CORTEX_CHANGESETSTART_INVERT <dd>
**   Invert the changeset while iterating through it. This is equivalent to
**   inverting a changeset using cortexchangeset_invert() before applying it.
**   It is an error to specify this flag with a patchset.
*/
#define CORTEX_CHANGESETSTART_INVERT        0x0002


/*
** CAPI3REF: Advance A Changeset Iterator
** METHOD: cortex_changeset_iter
**
** This function may only be used with iterators created by the function
** [cortexchangeset_start()]. If it is called on an iterator passed to
** a conflict-handler callback by [cortexchangeset_apply()], CORTEX_MISUSE
** is returned and the call has no effect.
**
** Immediately after an iterator is created by cortexchangeset_start(), it
** does not point to any change in the changeset. Assuming the changeset
** is not empty, the first call to this function advances the iterator to
** point to the first change in the changeset. Each subsequent call advances
** the iterator to point to the next change in the changeset (if any). If
** no error occurs and the iterator points to a valid change after a call
** to cortexchangeset_next() has advanced it, CORTEX_ROW is returned.
** Otherwise, if all changes in the changeset have already been visited,
** CORTEX_DONE is returned.
**
** If an error occurs, an CORTEX error code is returned. Possible error
** codes include CORTEX_CORRUPT (if the changeset buffer is corrupt) or
** CORTEX_NOMEM.
*/
CORTEX_API int cortexchangeset_next(cortex_changeset_iter *pIter);

/*
** CAPI3REF: Obtain The Current Operation From A Changeset Iterator
** METHOD: cortex_changeset_iter
**
** The pIter argument passed to this function may either be an iterator
** passed to a conflict-handler by [cortexchangeset_apply()], or an iterator
** created by [cortexchangeset_start()]. In the latter case, the most recent
** call to [cortexchangeset_next()] must have returned [CORTEX_ROW]. If this
** is not the case, this function returns [CORTEX_MISUSE].
**
** Arguments pOp, pnCol and pzTab may not be NULL. Upon return, three
** outputs are set through these pointers:
**
** *pOp is set to one of [CORTEX_INSERT], [CORTEX_DELETE] or [CORTEX_UPDATE],
** depending on the type of change that the iterator currently points to;
**
** *pnCol is set to the number of columns in the table affected by the change; and
**
** *pzTab is set to point to a nul-terminated utf-8 encoded string containing
** the name of the table affected by the current change. The buffer remains
** valid until either cortexchangeset_next() is called on the iterator
** or until the conflict-handler function returns.
**
** If pbIndirect is not NULL, then *pbIndirect is set to true (1) if the change
** is an indirect change, or false (0) otherwise. See the documentation for
** [cortexsession_indirect()] for a description of direct and indirect
** changes.
**
** If no error occurs, CORTEX_OK is returned. If an error does occur, an
** CORTEX error code is returned. The values of the output variables may not
** be trusted in this case.
*/
CORTEX_API int cortexchangeset_op(
  cortex_changeset_iter *pIter,  /* Iterator object */
  const char **pzTab,             /* OUT: Pointer to table name */
  int *pnCol,                     /* OUT: Number of columns in table */
  int *pOp,                       /* OUT: CORTEX_INSERT, DELETE or UPDATE */
  int *pbIndirect                 /* OUT: True for an 'indirect' change */
);

/*
** CAPI3REF: Obtain The Primary Key Definition Of A Table
** METHOD: cortex_changeset_iter
**
** For each modified table, a changeset includes the following:
**
** <ul>
**   <li> The number of columns in the table, and
**   <li> Which of those columns make up the tables PRIMARY KEY.
** </ul>
**
** This function is used to find which columns comprise the PRIMARY KEY of
** the table modified by the change that iterator pIter currently points to.
** If successful, *pabPK is set to point to an array of nCol entries, where
** nCol is the number of columns in the table. Elements of *pabPK are set to
** 0x01 if the corresponding column is part of the tables primary key, or
** 0x00 if it is not.
**
** If argument pnCol is not NULL, then *pnCol is set to the number of columns
** in the table.
**
** If this function is called when the iterator does not point to a valid
** entry, CORTEX_MISUSE is returned and the output variables zeroed. Otherwise,
** CORTEX_OK is returned and the output variables populated as described
** above.
*/
CORTEX_API int cortexchangeset_pk(
  cortex_changeset_iter *pIter,  /* Iterator object */
  unsigned char **pabPK,          /* OUT: Array of boolean - true for PK cols */
  int *pnCol                      /* OUT: Number of entries in output array */
);

/*
** CAPI3REF: Obtain old.* Values From A Changeset Iterator
** METHOD: cortex_changeset_iter
**
** The pIter argument passed to this function may either be an iterator
** passed to a conflict-handler by [cortexchangeset_apply()], or an iterator
** created by [cortexchangeset_start()]. In the latter case, the most recent
** call to [cortexchangeset_next()] must have returned CORTEX_ROW.
** Furthermore, it may only be called if the type of change that the iterator
** currently points to is either [CORTEX_DELETE] or [CORTEX_UPDATE]. Otherwise,
** this function returns [CORTEX_MISUSE] and sets *ppValue to NULL.
**
** Argument iVal must be greater than or equal to 0, and less than the number
** of columns in the table affected by the current change. Otherwise,
** [CORTEX_RANGE] is returned and *ppValue is set to NULL.
**
** If successful, this function sets *ppValue to point to a protected
** cortex_value object containing the iVal'th value from the vector of
** original row values stored as part of the UPDATE or DELETE change and
** returns CORTEX_OK. The name of the function comes from the fact that this
** is similar to the "old.*" columns available to update or delete triggers.
**
** If some other error occurs (e.g. an OOM condition), an CORTEX error code
** is returned and *ppValue is set to NULL.
*/
CORTEX_API int cortexchangeset_old(
  cortex_changeset_iter *pIter,  /* Changeset iterator */
  int iVal,                       /* Column number */
  cortex_value **ppValue         /* OUT: Old value (or NULL pointer) */
);

/*
** CAPI3REF: Obtain new.* Values From A Changeset Iterator
** METHOD: cortex_changeset_iter
**
** The pIter argument passed to this function may either be an iterator
** passed to a conflict-handler by [cortexchangeset_apply()], or an iterator
** created by [cortexchangeset_start()]. In the latter case, the most recent
** call to [cortexchangeset_next()] must have returned CORTEX_ROW.
** Furthermore, it may only be called if the type of change that the iterator
** currently points to is either [CORTEX_UPDATE] or [CORTEX_INSERT]. Otherwise,
** this function returns [CORTEX_MISUSE] and sets *ppValue to NULL.
**
** Argument iVal must be greater than or equal to 0, and less than the number
** of columns in the table affected by the current change. Otherwise,
** [CORTEX_RANGE] is returned and *ppValue is set to NULL.
**
** If successful, this function sets *ppValue to point to a protected
** cortex_value object containing the iVal'th value from the vector of
** new row values stored as part of the UPDATE or INSERT change and
** returns CORTEX_OK. If the change is an UPDATE and does not include
** a new value for the requested column, *ppValue is set to NULL and
** CORTEX_OK returned. The name of the function comes from the fact that
** this is similar to the "new.*" columns available to update or delete
** triggers.
**
** If some other error occurs (e.g. an OOM condition), an CORTEX error code
** is returned and *ppValue is set to NULL.
*/
CORTEX_API int cortexchangeset_new(
  cortex_changeset_iter *pIter,  /* Changeset iterator */
  int iVal,                       /* Column number */
  cortex_value **ppValue         /* OUT: New value (or NULL pointer) */
);

/*
** CAPI3REF: Obtain Conflicting Row Values From A Changeset Iterator
** METHOD: cortex_changeset_iter
**
** This function should only be used with iterator objects passed to a
** conflict-handler callback by [cortexchangeset_apply()] with either
** [CORTEX_CHANGESET_DATA] or [CORTEX_CHANGESET_CONFLICT]. If this function
** is called on any other iterator, [CORTEX_MISUSE] is returned and *ppValue
** is set to NULL.
**
** Argument iVal must be greater than or equal to 0, and less than the number
** of columns in the table affected by the current change. Otherwise,
** [CORTEX_RANGE] is returned and *ppValue is set to NULL.
**
** If successful, this function sets *ppValue to point to a protected
** cortex_value object containing the iVal'th value from the
** "conflicting row" associated with the current conflict-handler callback
** and returns CORTEX_OK.
**
** If some other error occurs (e.g. an OOM condition), an CORTEX error code
** is returned and *ppValue is set to NULL.
*/
CORTEX_API int cortexchangeset_conflict(
  cortex_changeset_iter *pIter,  /* Changeset iterator */
  int iVal,                       /* Column number */
  cortex_value **ppValue         /* OUT: Value from conflicting row */
);

/*
** CAPI3REF: Determine The Number Of Foreign Key Constraint Violations
** METHOD: cortex_changeset_iter
**
** This function may only be called with an iterator passed to an
** CORTEX_CHANGESET_FOREIGN_KEY conflict handler callback. In this case
** it sets the output variable to the total number of known foreign key
** violations in the destination database and returns CORTEX_OK.
**
** In all other cases this function returns CORTEX_MISUSE.
*/
CORTEX_API int cortexchangeset_fk_conflicts(
  cortex_changeset_iter *pIter,  /* Changeset iterator */
  int *pnOut                      /* OUT: Number of FK violations */
);


/*
** CAPI3REF: Finalize A Changeset Iterator
** METHOD: cortex_changeset_iter
**
** This function is used to finalize an iterator allocated with
** [cortexchangeset_start()].
**
** This function should only be called on iterators created using the
** [cortexchangeset_start()] function. If an application calls this
** function with an iterator passed to a conflict-handler by
** [cortexchangeset_apply()], [CORTEX_MISUSE] is immediately returned and the
** call has no effect.
**
** If an error was encountered within a call to an cortexchangeset_xxx()
** function (for example an [CORTEX_CORRUPT] in [cortexchangeset_next()] or an
** [CORTEX_NOMEM] in [cortexchangeset_new()]) then an error code corresponding
** to that error is returned by this function. Otherwise, CORTEX_OK is
** returned. This is to allow the following pattern (pseudo-code):
**
** <pre>
**   cortexchangeset_start();
**   while( CORTEX_ROW==cortexchangeset_next() ){
**     // Do something with change.
**   }
**   rc = cortexchangeset_finalize();
**   if( rc!=CORTEX_OK ){
**     // An error has occurred
**   }
** </pre>
*/
CORTEX_API int cortexchangeset_finalize(cortex_changeset_iter *pIter);

/*
** CAPI3REF: Invert A Changeset
**
** This function is used to "invert" a changeset object. Applying an inverted
** changeset to a database reverses the effects of applying the uninverted
** changeset. Specifically:
**
** <ul>
**   <li> Each DELETE change is changed to an INSERT, and
**   <li> Each INSERT change is changed to a DELETE, and
**   <li> For each UPDATE change, the old.* and new.* values are exchanged.
** </ul>
**
** This function does not change the order in which changes appear within
** the changeset. It merely reverses the sense of each individual change.
**
** If successful, a pointer to a buffer containing the inverted changeset
** is stored in *ppOut, the size of the same buffer is stored in *pnOut, and
** CORTEX_OK is returned. If an error occurs, both *pnOut and *ppOut are
** zeroed and an CORTEX error code returned.
**
** It is the responsibility of the caller to eventually call cortex_free()
** on the *ppOut pointer to free the buffer allocation following a successful
** call to this function.
**
** WARNING/TODO: This function currently assumes that the input is a valid
** changeset. If it is not, the results are undefined.
*/
CORTEX_API int cortexchangeset_invert(
  int nIn, const void *pIn,       /* Input changeset */
  int *pnOut, void **ppOut        /* OUT: Inverse of input */
);

/*
** CAPI3REF: Concatenate Two Changeset Objects
**
** This function is used to concatenate two changesets, A and B, into a
** single changeset. The result is a changeset equivalent to applying
** changeset A followed by changeset B.
**
** This function combines the two input changesets using an
** cortex_changegroup object. Calling it produces similar results as the
** following code fragment:
**
** <pre>
**   cortex_changegroup *pGrp;
**   rc = cortex_changegroup_new(&pGrp);
**   if( rc==CORTEX_OK ) rc = cortexchangegroup_add(pGrp, nA, pA);
**   if( rc==CORTEX_OK ) rc = cortexchangegroup_add(pGrp, nB, pB);
**   if( rc==CORTEX_OK ){
**     rc = cortexchangegroup_output(pGrp, pnOut, ppOut);
**   }else{
**     *ppOut = 0;
**     *pnOut = 0;
**   }
** </pre>
**
** Refer to the cortex_changegroup documentation below for details.
*/
CORTEX_API int cortexchangeset_concat(
  int nA,                         /* Number of bytes in buffer pA */
  void *pA,                       /* Pointer to buffer containing changeset A */
  int nB,                         /* Number of bytes in buffer pB */
  void *pB,                       /* Pointer to buffer containing changeset B */
  int *pnOut,                     /* OUT: Number of bytes in output changeset */
  void **ppOut                    /* OUT: Buffer containing output changeset */
);

/*
** CAPI3REF: Changegroup Handle
**
** A changegroup is an object used to combine two or more
** [changesets] or [patchsets]
*/
typedef struct cortex_changegroup cortex_changegroup;

/*
** CAPI3REF: Create A New Changegroup Object
** CONSTRUCTOR: cortex_changegroup
**
** An cortex_changegroup object is used to combine two or more changesets
** (or patchsets) into a single changeset (or patchset). A single changegroup
** object may combine changesets or patchsets, but not both. The output is
** always in the same format as the input.
**
** If successful, this function returns CORTEX_OK and populates (*pp) with
** a pointer to a new cortex_changegroup object before returning. The caller
** should eventually free the returned object using a call to
** cortexchangegroup_delete(). If an error occurs, an CORTEX error code
** (i.e. CORTEX_NOMEM) is returned and *pp is set to NULL.
**
** The usual usage pattern for an cortex_changegroup object is as follows:
**
** <ul>
**   <li> It is created using a call to cortexchangegroup_new().
**
**   <li> Zero or more changesets (or patchsets) are added to the object
**        by calling cortexchangegroup_add().
**
**   <li> The result of combining all input changesets together is obtained
**        by the application via a call to cortexchangegroup_output().
**
**   <li> The object is deleted using a call to cortexchangegroup_delete().
** </ul>
**
** Any number of calls to add() and output() may be made between the calls to
** new() and delete(), and in any order.
**
** As well as the regular cortexchangegroup_add() and
** cortexchangegroup_output() functions, also available are the streaming
** versions cortexchangegroup_add_strm() and cortexchangegroup_output_strm().
*/
CORTEX_API int cortexchangegroup_new(cortex_changegroup **pp);

/*
** CAPI3REF: Add a Schema to a Changegroup
** METHOD: cortex_changegroup_schema
**
** This method may be used to optionally enforce the rule that the changesets
** added to the changegroup handle must match the schema of database zDb
** ("main", "temp", or the name of an attached database). If
** cortexchangegroup_add() is called to add a changeset that is not compatible
** with the configured schema, CORTEX_SCHEMA is returned and the changegroup
** object is left in an undefined state.
**
** A changeset schema is considered compatible with the database schema in
** the same way as for cortexchangeset_apply(). Specifically, for each
** table in the changeset, there exists a database table with:
**
** <ul>
**   <li> The name identified by the changeset, and
**   <li> at least as many columns as recorded in the changeset, and
**   <li> the primary key columns in the same position as recorded in
**        the changeset.
** </ul>
**
** The output of the changegroup object always has the same schema as the
** database nominated using this function. In cases where changesets passed
** to cortexchangegroup_add() have fewer columns than the corresponding table
** in the database schema, these are filled in using the default column
** values from the database schema. This makes it possible to combined
** changesets that have different numbers of columns for a single table
** within a changegroup, provided that they are otherwise compatible.
*/
CORTEX_API int cortexchangegroup_schema(cortex_changegroup*, cortex*, const char *zDb);

/*
** CAPI3REF: Add A Changeset To A Changegroup
** METHOD: cortex_changegroup
**
** Add all changes within the changeset (or patchset) in buffer pData (size
** nData bytes) to the changegroup.
**
** If the buffer contains a patchset, then all prior calls to this function
** on the same changegroup object must also have specified patchsets. Or, if
** the buffer contains a changeset, so must have the earlier calls to this
** function. Otherwise, CORTEX_ERROR is returned and no changes are added
** to the changegroup.
**
** Rows within the changeset and changegroup are identified by the values in
** their PRIMARY KEY columns. A change in the changeset is considered to
** apply to the same row as a change already present in the changegroup if
** the two rows have the same primary key.
**
** Changes to rows that do not already appear in the changegroup are
** simply copied into it. Or, if both the new changeset and the changegroup
** contain changes that apply to a single row, the final contents of the
** changegroup depends on the type of each change, as follows:
**
** <table border=1 style="margin-left:8ex;margin-right:8ex">
**   <tr><th style="white-space:pre">Existing Change  </th>
**       <th style="white-space:pre">New Change       </th>
**       <th>Output Change
**   <tr><td>INSERT <td>INSERT <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
**   <tr><td>INSERT <td>UPDATE <td>
**       The INSERT change remains in the changegroup. The values in the
**       INSERT change are modified as if the row was inserted by the
**       existing change and then updated according to the new change.
**   <tr><td>INSERT <td>DELETE <td>
**       The existing INSERT is removed from the changegroup. The DELETE is
**       not added.
**   <tr><td>UPDATE <td>INSERT <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
**   <tr><td>UPDATE <td>UPDATE <td>
**       The existing UPDATE remains within the changegroup. It is amended
**       so that the accompanying values are as if the row was updated once
**       by the existing change and then again by the new change.
**   <tr><td>UPDATE <td>DELETE <td>
**       The existing UPDATE is replaced by the new DELETE within the
**       changegroup.
**   <tr><td>DELETE <td>INSERT <td>
**       If one or more of the column values in the row inserted by the
**       new change differ from those in the row deleted by the existing
**       change, the existing DELETE is replaced by an UPDATE within the
**       changegroup. Otherwise, if the inserted row is exactly the same
**       as the deleted row, the existing DELETE is simply discarded.
**   <tr><td>DELETE <td>UPDATE <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
**   <tr><td>DELETE <td>DELETE <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
** </table>
**
** If the new changeset contains changes to a table that is already present
** in the changegroup, then the number of columns and the position of the
** primary key columns for the table must be consistent. If this is not the
** case, this function fails with CORTEX_SCHEMA. Except, if the changegroup
** object has been configured with a database schema using the
** cortexchangegroup_schema() API, then it is possible to combine changesets
** with different numbers of columns for a single table, provided that
** they are otherwise compatible.
**
** If the input changeset appears to be corrupt and the corruption is
** detected, CORTEX_CORRUPT is returned. Or, if an out-of-memory condition
** occurs during processing, this function returns CORTEX_NOMEM.
**
** In all cases, if an error occurs the state of the final contents of the
** changegroup is undefined. If no error occurs, CORTEX_OK is returned.
*/
CORTEX_API int cortexchangegroup_add(cortex_changegroup*, int nData, void *pData);

/*
** CAPI3REF: Add A Single Change To A Changegroup
** METHOD: cortex_changegroup
**
** This function adds the single change currently indicated by the iterator
** passed as the second argument to the changegroup object. The rules for
** adding the change are just as described for [cortexchangegroup_add()].
**
** If the change is successfully added to the changegroup, CORTEX_OK is
** returned. Otherwise, an CORTEX error code is returned.
**
** The iterator must point to a valid entry when this function is called.
** If it does not, CORTEX_ERROR is returned and no change is added to the
** changegroup. Additionally, the iterator must not have been opened with
** the CORTEX_CHANGESETAPPLY_INVERT flag. In this case CORTEX_ERROR is also
** returned.
*/
CORTEX_API int cortexchangegroup_add_change(
  cortex_changegroup*,
  cortex_changeset_iter*
);



/*
** CAPI3REF: Obtain A Composite Changeset From A Changegroup
** METHOD: cortex_changegroup
**
** Obtain a buffer containing a changeset (or patchset) representing the
** current contents of the changegroup. If the inputs to the changegroup
** were themselves changesets, the output is a changeset. Or, if the
** inputs were patchsets, the output is also a patchset.
**
** As with the output of the cortexsession_changeset() and
** cortexsession_patchset() functions, all changes related to a single
** table are grouped together in the output of this function. Tables appear
** in the same order as for the very first changeset added to the changegroup.
** If the second or subsequent changesets added to the changegroup contain
** changes for tables that do not appear in the first changeset, they are
** appended onto the end of the output changeset, again in the order in
** which they are first encountered.
**
** If an error occurs, an CORTEX error code is returned and the output
** variables (*pnData) and (*ppData) are set to 0. Otherwise, CORTEX_OK
** is returned and the output variables are set to the size of and a
** pointer to the output buffer, respectively. In this case it is the
** responsibility of the caller to eventually free the buffer using a
** call to cortex_free().
*/
CORTEX_API int cortexchangegroup_output(
  cortex_changegroup*,
  int *pnData,                    /* OUT: Size of output buffer in bytes */
  void **ppData                   /* OUT: Pointer to output buffer */
);

/*
** CAPI3REF: Delete A Changegroup Object
** DESTRUCTOR: cortex_changegroup
*/
CORTEX_API void cortexchangegroup_delete(cortex_changegroup*);

/*
** CAPI3REF: Apply A Changeset To A Database
**
** Apply a changeset or patchset to a database. These functions attempt to
** update the "main" database attached to handle db with the changes found in
** the changeset passed via the second and third arguments.
**
** All changes made by these functions are enclosed in a savepoint transaction.
** If any other error (aside from a constraint failure when attempting to
** write to the target database) occurs, then the savepoint transaction is
** rolled back, restoring the target database to its original state, and an
** CORTEX error code returned. Additionally, starting with version 3.51.0,
** an error code and error message that may be accessed using the
** [cortex_errcode()] and [cortex_errmsg()] APIs are left in the database
** handle.
**
** The fourth argument (xFilter) passed to these functions is the "filter
** callback". This may be passed NULL, in which case all changes in the
** changeset are applied to the database. For cortexchangeset_apply() and
** cortex_changeset_apply_v2(), if it is not NULL, then it is invoked once
** for each table affected by at least one change in the changeset. In this
** case the table name is passed as the second argument, and a copy of
** the context pointer passed as the sixth argument to apply() or apply_v2()
** as the first. If the "filter callback" returns zero, then no attempt is
** made to apply any changes to the table. Otherwise, if the return value is
** non-zero, all changes related to the table are attempted.
**
** For cortex_changeset_apply_v3(), the xFilter callback is invoked once
** per change. The second argument in this case is an cortex_changeset_iter
** that may be queried using the usual APIs for the details of the current
** change. If the "filter callback" returns zero in this case, then no attempt
** is made to apply the current change. If it returns non-zero, the change
** is applied.
**
** For each table that is not excluded by the filter callback, this function
** tests that the target database contains a compatible table. A table is
** considered compatible if all of the following are true:
**
** <ul>
**   <li> The table has the same name as the name recorded in the
**        changeset, and
**   <li> The table has at least as many columns as recorded in the
**        changeset, and
**   <li> The table has primary key columns in the same position as
**        recorded in the changeset.
** </ul>
**
** If there is no compatible table, it is not an error, but none of the
** changes associated with the table are applied. A warning message is issued
** via the cortex_log() mechanism with the error code CORTEX_SCHEMA. At most
** one such warning is issued for each table in the changeset.
**
** For each change for which there is a compatible table, an attempt is made
** to modify the table contents according to each UPDATE, INSERT or DELETE
** change that is not excluded by a filter callback. If a change cannot be
** applied cleanly, the conflict handler function passed as the fifth argument
** to cortexchangeset_apply() may be invoked. A description of exactly when
** the conflict handler is invoked for each type of change is below.
**
** Unlike the xFilter argument, xConflict may not be passed NULL. The results
** of passing anything other than a valid function pointer as the xConflict
** argument are undefined.
**
** Each time the conflict handler function is invoked, it must return one
** of [CORTEX_CHANGESET_OMIT], [CORTEX_CHANGESET_ABORT] or
** [CORTEX_CHANGESET_REPLACE]. CORTEX_CHANGESET_REPLACE may only be returned
** if the second argument passed to the conflict handler is either
** CORTEX_CHANGESET_DATA or CORTEX_CHANGESET_CONFLICT. If the conflict-handler
** returns an illegal value, any changes already made are rolled back and
** the call to cortexchangeset_apply() returns CORTEX_MISUSE. Different
** actions are taken by cortexchangeset_apply() depending on the value
** returned by each invocation of the conflict-handler function. Refer to
** the documentation for the three
** [CORTEX_CHANGESET_OMIT|available return values] for details.
**
** <dl>
** <dt>DELETE Changes<dd>
**   For each DELETE change, the function checks if the target database
**   contains a row with the same primary key value (or values) as the
**   original row values stored in the changeset. If it does, and the values
**   stored in all non-primary key columns also match the values stored in
**   the changeset the row is deleted from the target database.
**
**   If a row with matching primary key values is found, but one or more of
**   the non-primary key fields contains a value different from the original
**   row value stored in the changeset, the conflict-handler function is
**   invoked with [CORTEX_CHANGESET_DATA] as the second argument. If the
**   database table has more columns than are recorded in the changeset,
**   only the values of those non-primary key fields are compared against
**   the current database contents - any trailing database table columns
**   are ignored.
**
**   If no row with matching primary key values is found in the database,
**   the conflict-handler function is invoked with [CORTEX_CHANGESET_NOTFOUND]
**   passed as the second argument.
**
**   If the DELETE operation is attempted, but CORTEX returns CORTEX_CONSTRAINT
**   (which can only happen if a foreign key constraint is violated), the
**   conflict-handler function is invoked with [CORTEX_CHANGESET_CONSTRAINT]
**   passed as the second argument. This includes the case where the DELETE
**   operation is attempted because an earlier call to the conflict handler
**   function returned [CORTEX_CHANGESET_REPLACE].
**
** <dt>INSERT Changes<dd>
**   For each INSERT change, an attempt is made to insert the new row into
**   the database. If the changeset row contains fewer fields than the
**   database table, the trailing fields are populated with their default
**   values.
**
**   If the attempt to insert the row fails because the database already
**   contains a row with the same primary key values, the conflict handler
**   function is invoked with the second argument set to
**   [CORTEX_CHANGESET_CONFLICT].
**
**   If the attempt to insert the row fails because of some other constraint
**   violation (e.g. NOT NULL or UNIQUE), the conflict handler function is
**   invoked with the second argument set to [CORTEX_CHANGESET_CONSTRAINT].
**   This includes the case where the INSERT operation is re-attempted because
**   an earlier call to the conflict handler function returned
**   [CORTEX_CHANGESET_REPLACE].
**
** <dt>UPDATE Changes<dd>
**   For each UPDATE change, the function checks if the target database
**   contains a row with the same primary key value (or values) as the
**   original row values stored in the changeset. If it does, and the values
**   stored in all modified non-primary key columns also match the values
**   stored in the changeset the row is updated within the target database.
**
**   If a row with matching primary key values is found, but one or more of
**   the modified non-primary key fields contains a value different from an
**   original row value stored in the changeset, the conflict-handler function
**   is invoked with [CORTEX_CHANGESET_DATA] as the second argument. Since
**   UPDATE changes only contain values for non-primary key fields that are
**   to be modified, only those fields need to match the original values to
**   avoid the CORTEX_CHANGESET_DATA conflict-handler callback.
**
**   If no row with matching primary key values is found in the database,
**   the conflict-handler function is invoked with [CORTEX_CHANGESET_NOTFOUND]
**   passed as the second argument.
**
**   If the UPDATE operation is attempted, but CORTEX returns
**   CORTEX_CONSTRAINT, the conflict-handler function is invoked with
**   [CORTEX_CHANGESET_CONSTRAINT] passed as the second argument.
**   This includes the case where the UPDATE operation is attempted after
**   an earlier call to the conflict handler function returned
**   [CORTEX_CHANGESET_REPLACE].
** </dl>
**
** It is safe to execute SQL statements, including those that write to the
** table that the callback related to, from within the xConflict callback.
** This can be used to further customize the application's conflict
** resolution strategy.
**
** If the output parameters (ppRebase) and (pnRebase) are non-NULL and
** the input is a changeset (not a patchset), then cortexchangeset_apply_v2()
** may set (*ppRebase) to point to a "rebase" that may be used with the
** cortex_rebaser APIs buffer before returning. In this case (*pnRebase)
** is set to the size of the buffer in bytes. It is the responsibility of the
** caller to eventually free any such buffer using cortex_free(). The buffer
** is only allocated and populated if one or more conflicts were encountered
** while applying the patchset. See comments surrounding the cortex_rebaser
** APIs for further details.
**
** The behavior of cortexchangeset_apply_v2() and its streaming equivalent
** may be modified by passing a combination of
** [CORTEX_CHANGESETAPPLY_NOSAVEPOINT | supported flags] as the 9th parameter.
**
** Note that the cortexchangeset_apply_v2() API is still <b>experimental</b>
** and therefore subject to change.
*/
CORTEX_API int cortexchangeset_apply(
  cortex *db,                    /* Apply change to "main" db of this handle */
  int nChangeset,                 /* Size of changeset in bytes */
  void *pChangeset,               /* Changeset blob */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    cortex_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx                      /* First argument passed to xConflict */
);
CORTEX_API int cortexchangeset_apply_v2(
  cortex *db,                    /* Apply change to "main" db of this handle */
  int nChangeset,                 /* Size of changeset in bytes */
  void *pChangeset,               /* Changeset blob */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    cortex_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx,                     /* First argument passed to xConflict */
  void **ppRebase, int *pnRebase, /* OUT: Rebase data */
  int flags                       /* SESSION_CHANGESETAPPLY_* flags */
);
CORTEX_API int cortexchangeset_apply_v3(
  cortex *db,                    /* Apply change to "main" db of this handle */
  int nChangeset,                 /* Size of changeset in bytes */
  void *pChangeset,               /* Changeset blob */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    cortex_changeset_iter *p     /* Handle describing change */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    cortex_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx,                     /* First argument passed to xConflict */
  void **ppRebase, int *pnRebase, /* OUT: Rebase data */
  int flags                       /* SESSION_CHANGESETAPPLY_* flags */
);

/*
** CAPI3REF: Flags for cortexchangeset_apply_v2
**
** The following flags may passed via the 9th parameter to
** [cortexchangeset_apply_v2] and [cortexchangeset_apply_v2_strm]:
**
** <dl>
** <dt>CORTEX_CHANGESETAPPLY_NOSAVEPOINT <dd>
**   Usually, the sessions module encloses all operations performed by
**   a single call to apply_v2() or apply_v2_strm() in a [SAVEPOINT]. The
**   SAVEPOINT is committed if the changeset or patchset is successfully
**   applied, or rolled back if an error occurs. Specifying this flag
**   causes the sessions module to omit this savepoint. In this case, if the
**   caller has an open transaction or savepoint when apply_v2() is called,
**   it may revert the partially applied changeset by rolling it back.
**
** <dt>CORTEX_CHANGESETAPPLY_INVERT <dd>
**   Invert the changeset before applying it. This is equivalent to inverting
**   a changeset using cortexchangeset_invert() before applying it. It is
**   an error to specify this flag with a patchset.
**
** <dt>CORTEX_CHANGESETAPPLY_IGNORENOOP <dd>
**   Do not invoke the conflict handler callback for any changes that
**   would not actually modify the database even if they were applied.
**   Specifically, this means that the conflict handler is not invoked
**   for:
**    <ul>
**    <li>a delete change if the row being deleted cannot be found,
**    <li>an update change if the modified fields are already set to
**        their new values in the conflicting row, or
**    <li>an insert change if all fields of the conflicting row match
**        the row being inserted.
**    </ul>
**
** <dt>CORTEX_CHANGESETAPPLY_FKNOACTION <dd>
**   If this flag it set, then all foreign key constraints in the target
**   database behave as if they were declared with "ON UPDATE NO ACTION ON
**   DELETE NO ACTION", even if they are actually CASCADE, RESTRICT, SET NULL
**   or SET DEFAULT.
*/
#define CORTEX_CHANGESETAPPLY_NOSAVEPOINT   0x0001
#define CORTEX_CHANGESETAPPLY_INVERT        0x0002
#define CORTEX_CHANGESETAPPLY_IGNORENOOP    0x0004
#define CORTEX_CHANGESETAPPLY_FKNOACTION    0x0008

/*
** CAPI3REF: Constants Passed To The Conflict Handler
**
** Values that may be passed as the second argument to a conflict-handler.
**
** <dl>
** <dt>CORTEX_CHANGESET_DATA<dd>
**   The conflict handler is invoked with CHANGESET_DATA as the second argument
**   when processing a DELETE or UPDATE change if a row with the required
**   PRIMARY KEY fields is present in the database, but one or more other
**   (non primary-key) fields modified by the update do not contain the
**   expected "before" values.
**
**   The conflicting row, in this case, is the database row with the matching
**   primary key.
**
** <dt>CORTEX_CHANGESET_NOTFOUND<dd>
**   The conflict handler is invoked with CHANGESET_NOTFOUND as the second
**   argument when processing a DELETE or UPDATE change if a row with the
**   required PRIMARY KEY fields is not present in the database.
**
**   There is no conflicting row in this case. The results of invoking the
**   cortexchangeset_conflict() API are undefined.
**
** <dt>CORTEX_CHANGESET_CONFLICT<dd>
**   CHANGESET_CONFLICT is passed as the second argument to the conflict
**   handler while processing an INSERT change if the operation would result
**   in duplicate primary key values.
**
**   The conflicting row in this case is the database row with the matching
**   primary key.
**
** <dt>CORTEX_CHANGESET_FOREIGN_KEY<dd>
**   If foreign key handling is enabled, and applying a changeset leaves the
**   database in a state containing foreign key violations, the conflict
**   handler is invoked with CHANGESET_FOREIGN_KEY as the second argument
**   exactly once before the changeset is committed. If the conflict handler
**   returns CHANGESET_OMIT, the changes, including those that caused the
**   foreign key constraint violation, are committed. Or, if it returns
**   CHANGESET_ABORT, the changeset is rolled back.
**
**   No current or conflicting row information is provided. The only function
**   it is possible to call on the supplied cortex_changeset_iter handle
**   is cortexchangeset_fk_conflicts().
**
** <dt>CORTEX_CHANGESET_CONSTRAINT<dd>
**   If any other constraint violation occurs while applying a change (i.e.
**   a UNIQUE, CHECK or NOT NULL constraint), the conflict handler is
**   invoked with CHANGESET_CONSTRAINT as the second argument.
**
**   There is no conflicting row in this case. The results of invoking the
**   cortexchangeset_conflict() API are undefined.
**
** </dl>
*/
#define CORTEX_CHANGESET_DATA        1
#define CORTEX_CHANGESET_NOTFOUND    2
#define CORTEX_CHANGESET_CONFLICT    3
#define CORTEX_CHANGESET_CONSTRAINT  4
#define CORTEX_CHANGESET_FOREIGN_KEY 5

/*
** CAPI3REF: Constants Returned By The Conflict Handler
**
** A conflict handler callback must return one of the following three values.
**
** <dl>
** <dt>CORTEX_CHANGESET_OMIT<dd>
**   If a conflict handler returns this value no special action is taken. The
**   change that caused the conflict is not applied. The session module
**   continues to the next change in the changeset.
**
** <dt>CORTEX_CHANGESET_REPLACE<dd>
**   This value may only be returned if the second argument to the conflict
**   handler was CORTEX_CHANGESET_DATA or CORTEX_CHANGESET_CONFLICT. If this
**   is not the case, any changes applied so far are rolled back and the
**   call to cortexchangeset_apply() returns CORTEX_MISUSE.
**
**   If CHANGESET_REPLACE is returned by an CORTEX_CHANGESET_DATA conflict
**   handler, then the conflicting row is either updated or deleted, depending
**   on the type of change.
**
**   If CHANGESET_REPLACE is returned by an CORTEX_CHANGESET_CONFLICT conflict
**   handler, then the conflicting row is removed from the database and a
**   second attempt to apply the change is made. If this second attempt fails,
**   the original row is restored to the database before continuing.
**
** <dt>CORTEX_CHANGESET_ABORT<dd>
**   If this value is returned, any changes applied so far are rolled back
**   and the call to cortexchangeset_apply() returns CORTEX_ABORT.
** </dl>
*/
#define CORTEX_CHANGESET_OMIT       0
#define CORTEX_CHANGESET_REPLACE    1
#define CORTEX_CHANGESET_ABORT      2

/*
** CAPI3REF: Rebasing changesets
** EXPERIMENTAL
**
** Suppose there is a site hosting a database in state S0. And that
** modifications are made that move that database to state S1 and a
** changeset recorded (the "local" changeset). Then, a changeset based
** on S0 is received from another site (the "remote" changeset) and
** applied to the database. The database is then in state
** (S1+"remote"), where the exact state depends on any conflict
** resolution decisions (OMIT or REPLACE) made while applying "remote".
** Rebasing a changeset is to update it to take those conflict
** resolution decisions into account, so that the same conflicts
** do not have to be resolved elsewhere in the network.
**
** For example, if both the local and remote changesets contain an
** INSERT of the same key on "CREATE TABLE t1(a PRIMARY KEY, b)":
**
**   local:  INSERT INTO t1 VALUES(1, 'v1');
**   remote: INSERT INTO t1 VALUES(1, 'v2');
**
** and the conflict resolution is REPLACE, then the INSERT change is
** removed from the local changeset (it was overridden). Or, if the
** conflict resolution was "OMIT", then the local changeset is modified
** to instead contain:
**
**           UPDATE t1 SET b = 'v2' WHERE a=1;
**
** Changes within the local changeset are rebased as follows:
**
** <dl>
** <dt>Local INSERT<dd>
**   This may only conflict with a remote INSERT. If the conflict
**   resolution was OMIT, then add an UPDATE change to the rebased
**   changeset. Or, if the conflict resolution was REPLACE, add
**   nothing to the rebased changeset.
**
** <dt>Local DELETE<dd>
**   This may conflict with a remote UPDATE or DELETE. In both cases the
**   only possible resolution is OMIT. If the remote operation was a
**   DELETE, then add no change to the rebased changeset. If the remote
**   operation was an UPDATE, then the old.* fields of change are updated
**   to reflect the new.* values in the UPDATE.
**
** <dt>Local UPDATE<dd>
**   This may conflict with a remote UPDATE or DELETE. If it conflicts
**   with a DELETE, and the conflict resolution was OMIT, then the update
**   is changed into an INSERT. Any undefined values in the new.* record
**   from the update change are filled in using the old.* values from
**   the conflicting DELETE. Or, if the conflict resolution was REPLACE,
**   the UPDATE change is simply omitted from the rebased changeset.
**
**   If conflict is with a remote UPDATE and the resolution is OMIT, then
**   the old.* values are rebased using the new.* values in the remote
**   change. Or, if the resolution is REPLACE, then the change is copied
**   into the rebased changeset with updates to columns also updated by
**   the conflicting remote UPDATE removed. If this means no columns would
**   be updated, the change is omitted.
** </dl>
**
** A local change may be rebased against multiple remote changes
** simultaneously. If a single key is modified by multiple remote
** changesets, they are combined as follows before the local changeset
** is rebased:
**
** <ul>
**    <li> If there has been one or more REPLACE resolutions on a
**         key, it is rebased according to a REPLACE.
**
**    <li> If there have been no REPLACE resolutions on a key, then
**         the local changeset is rebased according to the most recent
**         of the OMIT resolutions.
** </ul>
**
** Note that conflict resolutions from multiple remote changesets are
** combined on a per-field basis, not per-row. This means that in the
** case of multiple remote UPDATE operations, some fields of a single
** local change may be rebased for REPLACE while others are rebased for
** OMIT.
**
** In order to rebase a local changeset, the remote changeset must first
** be applied to the local database using cortexchangeset_apply_v2() and
** the buffer of rebase information captured. Then:
**
** <ol>
**   <li> An cortex_rebaser object is created by calling
**        cortexrebaser_create().
**   <li> The new object is configured with the rebase buffer obtained from
**        cortexchangeset_apply_v2() by calling cortexrebaser_configure().
**        If the local changeset is to be rebased against multiple remote
**        changesets, then cortexrebaser_configure() should be called
**        multiple times, in the same order that the multiple
**        cortexchangeset_apply_v2() calls were made.
**   <li> Each local changeset is rebased by calling cortexrebaser_rebase().
**   <li> The cortex_rebaser object is deleted by calling
**        cortexrebaser_delete().
** </ol>
*/
typedef struct cortex_rebaser cortex_rebaser;

/*
** CAPI3REF: Create a changeset rebaser object.
** EXPERIMENTAL
**
** Allocate a new changeset rebaser object. If successful, set (*ppNew) to
** point to the new object and return CORTEX_OK. Otherwise, if an error
** occurs, return an CORTEX error code (e.g. CORTEX_NOMEM) and set (*ppNew)
** to NULL.
*/
CORTEX_API int cortexrebaser_create(cortex_rebaser **ppNew);

/*
** CAPI3REF: Configure a changeset rebaser object.
** EXPERIMENTAL
**
** Configure the changeset rebaser object to rebase changesets according
** to the conflict resolutions described by buffer pRebase (size nRebase
** bytes), which must have been obtained from a previous call to
** cortexchangeset_apply_v2().
*/
CORTEX_API int cortexrebaser_configure(
  cortex_rebaser*,
  int nRebase, const void *pRebase
);

/*
** CAPI3REF: Rebase a changeset
** EXPERIMENTAL
**
** Argument pIn must point to a buffer containing a changeset nIn bytes
** in size. This function allocates and populates a buffer with a copy
** of the changeset rebased according to the configuration of the
** rebaser object passed as the first argument. If successful, (*ppOut)
** is set to point to the new buffer containing the rebased changeset and
** (*pnOut) to its size in bytes and CORTEX_OK returned. It is the
** responsibility of the caller to eventually free the new buffer using
** cortex_free(). Otherwise, if an error occurs, (*ppOut) and (*pnOut)
** are set to zero and an CORTEX error code returned.
*/
CORTEX_API int cortexrebaser_rebase(
  cortex_rebaser*,
  int nIn, const void *pIn,
  int *pnOut, void **ppOut
);

/*
** CAPI3REF: Delete a changeset rebaser object.
** EXPERIMENTAL
**
** Delete the changeset rebaser object and all associated resources. There
** should be one call to this function for each successful invocation
** of cortexrebaser_create().
*/
CORTEX_API void cortexrebaser_delete(cortex_rebaser *p);

/*
** CAPI3REF: Streaming Versions of API functions.
**
** The six streaming API xxx_strm() functions serve similar purposes to the
** corresponding non-streaming API functions:
**
** <table border=1 style="margin-left:8ex;margin-right:8ex">
**   <tr><th>Streaming function<th>Non-streaming equivalent</th>
**   <tr><td>cortexchangeset_apply_strm<td>[cortexchangeset_apply]
**   <tr><td>cortexchangeset_apply_strm_v2<td>[cortexchangeset_apply_v2]
**   <tr><td>cortexchangeset_concat_strm<td>[cortexchangeset_concat]
**   <tr><td>cortexchangeset_invert_strm<td>[cortexchangeset_invert]
**   <tr><td>cortexchangeset_start_strm<td>[cortexchangeset_start]
**   <tr><td>cortexsession_changeset_strm<td>[cortexsession_changeset]
**   <tr><td>cortexsession_patchset_strm<td>[cortexsession_patchset]
** </table>
**
** Non-streaming functions that accept changesets (or patchsets) as input
** require that the entire changeset be stored in a single buffer in memory.
** Similarly, those that return a changeset or patchset do so by returning
** a pointer to a single large buffer allocated using cortex_malloc().
** Normally this is convenient. However, if an application running in a
** low-memory environment is required to handle very large changesets, the
** large contiguous memory allocations required can become onerous.
**
** In order to avoid this problem, instead of a single large buffer, input
** is passed to a streaming API functions by way of a callback function that
** the sessions module invokes to incrementally request input data as it is
** required. In all cases, a pair of API function parameters such as
**
**  <pre>
**  &nbsp;     int nChangeset,
**  &nbsp;     void *pChangeset,
**  </pre>
**
** Is replaced by:
**
**  <pre>
**  &nbsp;     int (*xInput)(void *pIn, void *pData, int *pnData),
**  &nbsp;     void *pIn,
**  </pre>
**
** Each time the xInput callback is invoked by the sessions module, the first
** argument passed is a copy of the supplied pIn context pointer. The second
** argument, pData, points to a buffer (*pnData) bytes in size. Assuming no
** error occurs the xInput method should copy up to (*pnData) bytes of data
** into the buffer and set (*pnData) to the actual number of bytes copied
** before returning CORTEX_OK. If the input is completely exhausted, (*pnData)
** should be set to zero to indicate this. Or, if an error occurs, an SQLite
** error code should be returned. In all cases, if an xInput callback returns
** an error, all processing is abandoned and the streaming API function
** returns a copy of the error code to the caller.
**
** In the case of cortexchangeset_start_strm(), the xInput callback may be
** invoked by the sessions module at any point during the lifetime of the
** iterator. If such an xInput callback returns an error, the iterator enters
** an error state, whereby all subsequent calls to iterator functions
** immediately fail with the same error code as returned by xInput.
**
** Similarly, streaming API functions that return changesets (or patchsets)
** return them in chunks by way of a callback function instead of via a
** pointer to a single large buffer. In this case, a pair of parameters such
** as:
**
**  <pre>
**  &nbsp;     int *pnChangeset,
**  &nbsp;     void **ppChangeset,
**  </pre>
**
** Is replaced by:
**
**  <pre>
**  &nbsp;     int (*xOutput)(void *pOut, const void *pData, int nData),
**  &nbsp;     void *pOut
**  </pre>
**
** The xOutput callback is invoked zero or more times to return data to
** the application. The first parameter passed to each call is a copy of the
** pOut pointer supplied by the application. The second parameter, pData,
** points to a buffer nData bytes in size containing the chunk of output
** data being returned. If the xOutput callback successfully processes the
** supplied data, it should return CORTEX_OK to indicate success. Otherwise,
** it should return some other CORTEX error code. In this case processing
** is immediately abandoned and the streaming API function returns a copy
** of the xOutput error code to the application.
**
** The sessions module never invokes an xOutput callback with the third
** parameter set to a value less than or equal to zero. Other than this,
** no guarantees are made as to the size of the chunks of data returned.
*/
CORTEX_API int cortexchangeset_apply_strm(
  cortex *db,                    /* Apply change to "main" db of this handle */
  int (*xInput)(void *pIn, void *pData, int *pnData), /* Input function */
  void *pIn,                                          /* First arg for xInput */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    cortex_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx                      /* First argument passed to xConflict */
);
CORTEX_API int cortexchangeset_apply_v2_strm(
  cortex *db,                    /* Apply change to "main" db of this handle */
  int (*xInput)(void *pIn, void *pData, int *pnData), /* Input function */
  void *pIn,                                          /* First arg for xInput */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    cortex_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx,                     /* First argument passed to xConflict */
  void **ppRebase, int *pnRebase,
  int flags
);
CORTEX_API int cortexchangeset_apply_v3_strm(
  cortex *db,                    /* Apply change to "main" db of this handle */
  int (*xInput)(void *pIn, void *pData, int *pnData), /* Input function */
  void *pIn,                                          /* First arg for xInput */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    cortex_changeset_iter *p
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    cortex_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx,                     /* First argument passed to xConflict */
  void **ppRebase, int *pnRebase,
  int flags
);
CORTEX_API int cortexchangeset_concat_strm(
  int (*xInputA)(void *pIn, void *pData, int *pnData),
  void *pInA,
  int (*xInputB)(void *pIn, void *pData, int *pnData),
  void *pInB,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
CORTEX_API int cortexchangeset_invert_strm(
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
CORTEX_API int cortexchangeset_start_strm(
  cortex_changeset_iter **pp,
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn
);
CORTEX_API int cortexchangeset_start_v2_strm(
  cortex_changeset_iter **pp,
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn,
  int flags
);
CORTEX_API int cortexsession_changeset_strm(
  cortex_session *pSession,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
CORTEX_API int cortexsession_patchset_strm(
  cortex_session *pSession,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
CORTEX_API int cortexchangegroup_add_strm(cortex_changegroup*,
    int (*xInput)(void *pIn, void *pData, int *pnData),
    void *pIn
);
CORTEX_API int cortexchangegroup_output_strm(cortex_changegroup*,
    int (*xOutput)(void *pOut, const void *pData, int nData),
    void *pOut
);
CORTEX_API int cortexrebaser_rebase_strm(
  cortex_rebaser *pRebaser,
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);

/*
** CAPI3REF: Configure global parameters
**
** The cortexsession_config() interface is used to make global configuration
** changes to the sessions module in order to tune it to the specific needs
** of the application.
**
** The cortexsession_config() interface is not threadsafe. If it is invoked
** while any other thread is inside any other sessions method then the
** results are undefined. Furthermore, if it is invoked after any sessions
** related objects have been created, the results are also undefined.
**
** The first argument to the cortexsession_config() function must be one
** of the CORTEX_SESSION_CONFIG_XXX constants defined below. The
** interpretation of the (void*) value passed as the second parameter and
** the effect of calling this function depends on the value of the first
** parameter.
**
** <dl>
** <dt>CORTEX_SESSION_CONFIG_STRMSIZE<dd>
**    By default, the sessions module streaming interfaces attempt to input
**    and output data in approximately 1 KiB chunks. This operand may be used
**    to set and query the value of this configuration setting. The pointer
**    passed as the second argument must point to a value of type (int).
**    If this value is greater than 0, it is used as the new streaming data
**    chunk size for both input and output. Before returning, the (int) value
**    pointed to by pArg is set to the final value of the streaming interface
**    chunk size.
** </dl>
**
** This function returns CORTEX_OK if successful, or an CORTEX error code
** otherwise.
*/
CORTEX_API int cortexsession_config(int op, void *pArg);

/*
** CAPI3REF: Values for cortexsession_config().
*/
#define CORTEX_SESSION_CONFIG_STRMSIZE 1

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
}
#endif

#endif  /* !defined(__SQLITESESSION_H_) && defined(CORTEX_ENABLE_SESSION) */

/******** End of cortexsession.h *********/
/******** Begin file fts5.h *********/
/*
** 2014 May 31
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** Interfaces to extend FTS5. Using the interfaces defined in this file,
** FTS5 may be extended with:
**
**     * custom tokenizers, and
**     * custom auxiliary functions.
*/


#ifndef _FTS5_H
#define _FTS5_H


#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************
** CUSTOM AUXILIARY FUNCTIONS
**
** Virtual table implementations may overload SQL functions by implementing
** the cortex_module.xFindFunction() method.
*/

typedef struct Fts5ExtensionApi Fts5ExtensionApi;
typedef struct Fts5Context Fts5Context;
typedef struct Fts5PhraseIter Fts5PhraseIter;

typedef void (*fts5_extension_function)(
  const Fts5ExtensionApi *pApi,   /* API offered by current FTS version */
  Fts5Context *pFts,              /* First arg to pass to pApi functions */
  cortex_context *pCtx,          /* Context for returning result/error */
  int nVal,                       /* Number of values in apVal[] array */
  cortex_value **apVal           /* Array of trailing arguments */
);

struct Fts5PhraseIter {
  const unsigned char *a;
  const unsigned char *b;
};

/*
** EXTENSION API FUNCTIONS
**
** xUserData(pFts):
**   Return a copy of the pUserData pointer passed to the xCreateFunction()
**   API when the extension function was registered.
**
** xColumnTotalSize(pFts, iCol, pnToken):
**   If parameter iCol is less than zero, set output variable *pnToken
**   to the total number of tokens in the FTS5 table. Or, if iCol is
**   non-negative but less than the number of columns in the table, return
**   the total number of tokens in column iCol, considering all rows in
**   the FTS5 table.
**
**   If parameter iCol is greater than or equal to the number of columns
**   in the table, CORTEX_RANGE is returned. Or, if an error occurs (e.g.
**   an OOM condition or IO error), an appropriate CORTEX error code is
**   returned.
**
** xColumnCount(pFts):
**   Return the number of columns in the table.
**
** xColumnSize(pFts, iCol, pnToken):
**   If parameter iCol is less than zero, set output variable *pnToken
**   to the total number of tokens in the current row. Or, if iCol is
**   non-negative but less than the number of columns in the table, set
**   *pnToken to the number of tokens in column iCol of the current row.
**
**   If parameter iCol is greater than or equal to the number of columns
**   in the table, CORTEX_RANGE is returned. Or, if an error occurs (e.g.
**   an OOM condition or IO error), an appropriate CORTEX error code is
**   returned.
**
**   This function may be quite inefficient if used with an FTS5 table
**   created with the "columnsize=0" option.
**
** xColumnText:
**   If parameter iCol is less than zero, or greater than or equal to the
**   number of columns in the table, CORTEX_RANGE is returned.
**
**   Otherwise, this function attempts to retrieve the text of column iCol of
**   the current document. If successful, (*pz) is set to point to a buffer
**   containing the text in utf-8 encoding, (*pn) is set to the size in bytes
**   (not characters) of the buffer and CORTEX_OK is returned. Otherwise,
**   if an error occurs, an CORTEX error code is returned and the final values
**   of (*pz) and (*pn) are undefined.
**
** xPhraseCount:
**   Returns the number of phrases in the current query expression.
**
** xPhraseSize:
**   If parameter iCol is less than zero, or greater than or equal to the
**   number of phrases in the current query, as returned by xPhraseCount,
**   0 is returned. Otherwise, this function returns the number of tokens in
**   phrase iPhrase of the query. Phrases are numbered starting from zero.
**
** xInstCount:
**   Set *pnInst to the total number of occurrences of all phrases within
**   the query within the current row. Return CORTEX_OK if successful, or
**   an error code (i.e. CORTEX_NOMEM) if an error occurs.
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" or "detail=column" option. If the FTS5 table is created
**   with either "detail=none" or "detail=column" and "content=" option
**   (i.e. if it is a contentless table), then this API always returns 0.
**
** xInst:
**   Query for the details of phrase match iIdx within the current row.
**   Phrase matches are numbered starting from zero, so the iIdx argument
**   should be greater than or equal to zero and smaller than the value
**   output by xInstCount(). If iIdx is less than zero or greater than
**   or equal to the value returned by xInstCount(), CORTEX_RANGE is returned.
**
**   Otherwise, output parameter *piPhrase is set to the phrase number, *piCol
**   to the column in which it occurs and *piOff the token offset of the
**   first token of the phrase. CORTEX_OK is returned if successful, or an
**   error code (i.e. CORTEX_NOMEM) if an error occurs.
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" or "detail=column" option.
**
** xRowid:
**   Returns the rowid of the current row.
**
** xTokenize:
**   Tokenize text using the tokenizer belonging to the FTS5 table.
**
** xQueryPhrase(pFts5, iPhrase, pUserData, xCallback):
**   This API function is used to query the FTS table for phrase iPhrase
**   of the current query. Specifically, a query equivalent to:
**
**       ... FROM ftstable WHERE ftstable MATCH $p ORDER BY rowid
**
**   with $p set to a phrase equivalent to the phrase iPhrase of the
**   current query is executed. Any column filter that applies to
**   phrase iPhrase of the current query is included in $p. For each
**   row visited, the callback function passed as the fourth argument
**   is invoked. The context and API objects passed to the callback
**   function may be used to access the properties of each matched row.
**   Invoking Api.xUserData() returns a copy of the pointer passed as
**   the third argument to pUserData.
**
**   If parameter iPhrase is less than zero, or greater than or equal to
**   the number of phrases in the query, as returned by xPhraseCount(),
**   this function returns CORTEX_RANGE.
**
**   If the callback function returns any value other than CORTEX_OK, the
**   query is abandoned and the xQueryPhrase function returns immediately.
**   If the returned value is CORTEX_DONE, xQueryPhrase returns CORTEX_OK.
**   Otherwise, the error code is propagated upwards.
**
**   If the query runs to completion without incident, CORTEX_OK is returned.
**   Or, if some error occurs before the query completes or is aborted by
**   the callback, an CORTEX error code is returned.
**
**
** xSetAuxdata(pFts5, pAux, xDelete)
**
**   Save the pointer passed as the second argument as the extension function's
**   "auxiliary data". The pointer may then be retrieved by the current or any
**   future invocation of the same fts5 extension function made as part of
**   the same MATCH query using the xGetAuxdata() API.
**
**   Each extension function is allocated a single auxiliary data slot for
**   each FTS query (MATCH expression). If the extension function is invoked
**   more than once for a single FTS query, then all invocations share a
**   single auxiliary data context.
**
**   If there is already an auxiliary data pointer when this function is
**   invoked, then it is replaced by the new pointer. If an xDelete callback
**   was specified along with the original pointer, it is invoked at this
**   point.
**
**   The xDelete callback, if one is specified, is also invoked on the
**   auxiliary data pointer after the FTS5 query has finished.
**
**   If an error (e.g. an OOM condition) occurs within this function,
**   the auxiliary data is set to NULL and an error code returned. If the
**   xDelete parameter was not NULL, it is invoked on the auxiliary data
**   pointer before returning.
**
**
** xGetAuxdata(pFts5, bClear)
**
**   Returns the current auxiliary data pointer for the fts5 extension
**   function. See the xSetAuxdata() method for details.
**
**   If the bClear argument is non-zero, then the auxiliary data is cleared
**   (set to NULL) before this function returns. In this case the xDelete,
**   if any, is not invoked.
**
**
** xRowCount(pFts5, pnRow)
**
**   This function is used to retrieve the total number of rows in the table.
**   In other words, the same value that would be returned by:
**
**        SELECT count(*) FROM ftstable;
**
** xPhraseFirst()
**   This function is used, along with type Fts5PhraseIter and the xPhraseNext
**   method, to iterate through all instances of a single query phrase within
**   the current row. This is the same information as is accessible via the
**   xInstCount/xInst APIs. While the xInstCount/xInst APIs are more convenient
**   to use, this API may be faster under some circumstances. To iterate
**   through instances of phrase iPhrase, use the following code:
**
**       Fts5PhraseIter iter;
**       int iCol, iOff;
**       for(pApi->xPhraseFirst(pFts, iPhrase, &iter, &iCol, &iOff);
**           iCol>=0;
**           pApi->xPhraseNext(pFts, &iter, &iCol, &iOff)
**       ){
**         // An instance of phrase iPhrase at offset iOff of column iCol
**       }
**
**   The Fts5PhraseIter structure is defined above. Applications should not
**   modify this structure directly - it should only be used as shown above
**   with the xPhraseFirst() and xPhraseNext() API methods (and by
**   xPhraseFirstColumn() and xPhraseNextColumn() as illustrated below).
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" or "detail=column" option. If the FTS5 table is created
**   with either "detail=none" or "detail=column" and "content=" option
**   (i.e. if it is a contentless table), then this API always iterates
**   through an empty set (all calls to xPhraseFirst() set iCol to -1).
**
**   In all cases, matches are visited in (column ASC, offset ASC) order.
**   i.e. all those in column 0, sorted by offset, followed by those in
**   column 1, etc.
**
** xPhraseNext()
**   See xPhraseFirst above.
**
** xPhraseFirstColumn()
**   This function and xPhraseNextColumn() are similar to the xPhraseFirst()
**   and xPhraseNext() APIs described above. The difference is that instead
**   of iterating through all instances of a phrase in the current row, these
**   APIs are used to iterate through the set of columns in the current row
**   that contain one or more instances of a specified phrase. For example:
**
**       Fts5PhraseIter iter;
**       int iCol;
**       for(pApi->xPhraseFirstColumn(pFts, iPhrase, &iter, &iCol);
**           iCol>=0;
**           pApi->xPhraseNextColumn(pFts, &iter, &iCol)
**       ){
**         // Column iCol contains at least one instance of phrase iPhrase
**       }
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" option. If the FTS5 table is created with either
**   "detail=none" "content=" option (i.e. if it is a contentless table),
**   then this API always iterates through an empty set (all calls to
**   xPhraseFirstColumn() set iCol to -1).
**
**   The information accessed using this API and its companion
**   xPhraseFirstColumn() may also be obtained using xPhraseFirst/xPhraseNext
**   (or xInst/xInstCount). The chief advantage of this API is that it is
**   significantly more efficient than those alternatives when used with
**   "detail=column" tables.
**
** xPhraseNextColumn()
**   See xPhraseFirstColumn above.
**
** xQueryToken(pFts5, iPhrase, iToken, ppToken, pnToken)
**   This is used to access token iToken of phrase iPhrase of the current
**   query. Before returning, output parameter *ppToken is set to point
**   to a buffer containing the requested token, and *pnToken to the
**   size of this buffer in bytes.
**
**   If iPhrase or iToken are less than zero, or if iPhrase is greater than
**   or equal to the number of phrases in the query as reported by
**   xPhraseCount(), or if iToken is equal to or greater than the number of
**   tokens in the phrase, CORTEX_RANGE is returned and *ppToken and *pnToken
     are both zeroed.
**
**   The output text is not a copy of the query text that specified the
**   token. It is the output of the tokenizer module. For tokendata=1
**   tables, this includes any embedded 0x00 and trailing data.
**
** xInstToken(pFts5, iIdx, iToken, ppToken, pnToken)
**   This is used to access token iToken of phrase hit iIdx within the
**   current row. If iIdx is less than zero or greater than or equal to the
**   value returned by xInstCount(), CORTEX_RANGE is returned.  Otherwise,
**   output variable (*ppToken) is set to point to a buffer containing the
**   matching document token, and (*pnToken) to the size of that buffer in
**   bytes.
**
**   The output text is not a copy of the document text that was tokenized.
**   It is the output of the tokenizer module. For tokendata=1 tables, this
**   includes any embedded 0x00 and trailing data.
**
**   This API may be slow in some cases if the token identified by parameters
**   iIdx and iToken matched a prefix token in the query. In most cases, the
**   first call to this API for each prefix token in the query is forced
**   to scan the portion of the full-text index that matches the prefix
**   token to collect the extra data required by this API. If the prefix
**   token matches a large number of token instances in the document set,
**   this may be a performance problem.
**
**   If the user knows in advance that a query may use this API for a
**   prefix token, FTS5 may be configured to collect all required data as part
**   of the initial querying of the full-text index, avoiding the second scan
**   entirely. This also causes prefix queries that do not use this API to
**   run more slowly and use more memory. FTS5 may be configured in this way
**   either on a per-table basis using the [FTS5 insttoken | 'insttoken']
**   option, or on a per-query basis using the
**   [fts5_insttoken | fts5_insttoken()] user function.
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" or "detail=column" option.
**
** xColumnLocale(pFts5, iIdx, pzLocale, pnLocale)
**   If parameter iCol is less than zero, or greater than or equal to the
**   number of columns in the table, CORTEX_RANGE is returned.
**
**   Otherwise, this function attempts to retrieve the locale associated
**   with column iCol of the current row. Usually, there is no associated
**   locale, and output parameters (*pzLocale) and (*pnLocale) are set
**   to NULL and 0, respectively. However, if the fts5_locale() function
**   was used to associate a locale with the value when it was inserted
**   into the fts5 table, then (*pzLocale) is set to point to a nul-terminated
**   buffer containing the name of the locale in utf-8 encoding. (*pnLocale)
**   is set to the size in bytes of the buffer, not including the
**   nul-terminator.
**
**   If successful, CORTEX_OK is returned. Or, if an error occurs, an
**   CORTEX error code is returned. The final value of the output parameters
**   is undefined in this case.
**
** xTokenize_v2:
**   Tokenize text using the tokenizer belonging to the FTS5 table. This
**   API is the same as the xTokenize() API, except that it allows a tokenizer
**   locale to be specified.
*/
struct Fts5ExtensionApi {
  int iVersion;                   /* Currently always set to 4 */

  void *(*xUserData)(Fts5Context*);

  int (*xColumnCount)(Fts5Context*);
  int (*xRowCount)(Fts5Context*, cortex_int64 *pnRow);
  int (*xColumnTotalSize)(Fts5Context*, int iCol, cortex_int64 *pnToken);

  int (*xTokenize)(Fts5Context*,
    const char *pText, int nText, /* Text to tokenize */
    void *pCtx,                   /* Context passed to xToken() */
    int (*xToken)(void*, int, const char*, int, int, int)       /* Callback */
  );

  int (*xPhraseCount)(Fts5Context*);
  int (*xPhraseSize)(Fts5Context*, int iPhrase);

  int (*xInstCount)(Fts5Context*, int *pnInst);
  int (*xInst)(Fts5Context*, int iIdx, int *piPhrase, int *piCol, int *piOff);

  cortex_int64 (*xRowid)(Fts5Context*);
  int (*xColumnText)(Fts5Context*, int iCol, const char **pz, int *pn);
  int (*xColumnSize)(Fts5Context*, int iCol, int *pnToken);

  int (*xQueryPhrase)(Fts5Context*, int iPhrase, void *pUserData,
    int(*)(const Fts5ExtensionApi*,Fts5Context*,void*)
  );
  int (*xSetAuxdata)(Fts5Context*, void *pAux, void(*xDelete)(void*));
  void *(*xGetAuxdata)(Fts5Context*, int bClear);

  int (*xPhraseFirst)(Fts5Context*, int iPhrase, Fts5PhraseIter*, int*, int*);
  void (*xPhraseNext)(Fts5Context*, Fts5PhraseIter*, int *piCol, int *piOff);

  int (*xPhraseFirstColumn)(Fts5Context*, int iPhrase, Fts5PhraseIter*, int*);
  void (*xPhraseNextColumn)(Fts5Context*, Fts5PhraseIter*, int *piCol);

  /* Below this point are iVersion>=3 only */
  int (*xQueryToken)(Fts5Context*,
      int iPhrase, int iToken,
      const char **ppToken, int *pnToken
  );
  int (*xInstToken)(Fts5Context*, int iIdx, int iToken, const char**, int*);

  /* Below this point are iVersion>=4 only */
  int (*xColumnLocale)(Fts5Context*, int iCol, const char **pz, int *pn);
  int (*xTokenize_v2)(Fts5Context*,
    const char *pText, int nText,      /* Text to tokenize */
    const char *pLocale, int nLocale,  /* Locale to pass to tokenizer */
    void *pCtx,                        /* Context passed to xToken() */
    int (*xToken)(void*, int, const char*, int, int, int)       /* Callback */
  );
};

/*
** CUSTOM AUXILIARY FUNCTIONS
*************************************************************************/

/*************************************************************************
** CUSTOM TOKENIZERS
**
** Applications may also register custom tokenizer types. A tokenizer
** is registered by providing fts5 with a populated instance of the
** following structure. All structure methods must be defined, setting
** any member of the fts5_tokenizer struct to NULL leads to undefined
** behaviour. The structure methods are expected to function as follows:
**
** xCreate:
**   This function is used to allocate and initialize a tokenizer instance.
**   A tokenizer instance is required to actually tokenize text.
**
**   The first argument passed to this function is a copy of the (void*)
**   pointer provided by the application when the fts5_tokenizer_v2 object
**   was registered with FTS5 (the third argument to xCreateTokenizer()).
**   The second and third arguments are an array of nul-terminated strings
**   containing the tokenizer arguments, if any, specified following the
**   tokenizer name as part of the CREATE VIRTUAL TABLE statement used
**   to create the FTS5 table.
**
**   The final argument is an output variable. If successful, (*ppOut)
**   should be set to point to the new tokenizer handle and CORTEX_OK
**   returned. If an error occurs, some value other than CORTEX_OK should
**   be returned. In this case, fts5 assumes that the final value of *ppOut
**   is undefined.
**
** xDelete:
**   This function is invoked to delete a tokenizer handle previously
**   allocated using xCreate(). Fts5 guarantees that this function will
**   be invoked exactly once for each successful call to xCreate().
**
** xTokenize:
**   This function is expected to tokenize the nText byte string indicated
**   by argument pText. pText may or may not be nul-terminated. The first
**   argument passed to this function is a pointer to an Fts5Tokenizer object
**   returned by an earlier call to xCreate().
**
**   The third argument indicates the reason that FTS5 is requesting
**   tokenization of the supplied text. This is always one of the following
**   four values:
**
**   <ul><li> <b>FTS5_TOKENIZE_DOCUMENT</b> - A document is being inserted into
**            or removed from the FTS table. The tokenizer is being invoked to
**            determine the set of tokens to add to (or delete from) the
**            FTS index.
**
**       <li> <b>FTS5_TOKENIZE_QUERY</b> - A MATCH query is being executed
**            against the FTS index. The tokenizer is being called to tokenize
**            a bareword or quoted string specified as part of the query.
**
**       <li> <b>(FTS5_TOKENIZE_QUERY | FTS5_TOKENIZE_PREFIX)</b> - Same as
**            FTS5_TOKENIZE_QUERY, except that the bareword or quoted string is
**            followed by a "*" character, indicating that the last token
**            returned by the tokenizer will be treated as a token prefix.
**
**       <li> <b>FTS5_TOKENIZE_AUX</b> - The tokenizer is being invoked to
**            satisfy an fts5_api.xTokenize() request made by an auxiliary
**            function. Or an fts5_api.xColumnSize() request made by the same
**            on a columnsize=0 database.
**   </ul>
**
**   The sixth and seventh arguments passed to xTokenize() - pLocale and
**   nLocale - are a pointer to a buffer containing the locale to use for
**   tokenization (e.g. "en_US") and its size in bytes, respectively. The
**   pLocale buffer is not nul-terminated. pLocale may be passed NULL (in
**   which case nLocale is always 0) to indicate that the tokenizer should
**   use its default locale.
**
**   For each token in the input string, the supplied callback xToken() must
**   be invoked. The first argument to it should be a copy of the pointer
**   passed as the second argument to xTokenize(). The third and fourth
**   arguments are a pointer to a buffer containing the token text, and the
**   size of the token in bytes. The 4th and 5th arguments are the byte offsets
**   of the first byte of and first byte immediately following the text from
**   which the token is derived within the input.
**
**   The second argument passed to the xToken() callback ("tflags") should
**   normally be set to 0. The exception is if the tokenizer supports
**   synonyms. In this case see the discussion below for details.
**
**   FTS5 assumes the xToken() callback is invoked for each token in the
**   order that they occur within the input text.
**
**   If an xToken() callback returns any value other than CORTEX_OK, then
**   the tokenization should be abandoned and the xTokenize() method should
**   immediately return a copy of the xToken() return value. Or, if the
**   input buffer is exhausted, xTokenize() should return CORTEX_OK. Finally,
**   if an error occurs with the xTokenize() implementation itself, it
**   may abandon the tokenization and return any error code other than
**   CORTEX_OK or CORTEX_DONE.
**
**   If the tokenizer is registered using an fts5_tokenizer_v2 object,
**   then the xTokenize() method has two additional arguments - pLocale
**   and nLocale. These specify the locale that the tokenizer should use
**   for the current request. If pLocale and nLocale are both 0, then the
**   tokenizer should use its default locale. Otherwise, pLocale points to
**   an nLocale byte buffer containing the name of the locale to use as utf-8
**   text. pLocale is not nul-terminated.
**
** FTS5_TOKENIZER
**
** There is also an fts5_tokenizer object. This is an older, deprecated,
** version of fts5_tokenizer_v2. It is similar except that:
**
**  <ul>
**    <li> There is no "iVersion" field, and
**    <li> The xTokenize() method does not take a locale argument.
**  </ul>
**
** Legacy fts5_tokenizer tokenizers must be registered using the
** legacy xCreateTokenizer() function, instead of xCreateTokenizer_v2().
**
** Tokenizer implementations registered using either API may be retrieved
** using both xFindTokenizer() and xFindTokenizer_v2().
**
** SYNONYM SUPPORT
**
**   Custom tokenizers may also support synonyms. Consider a case in which a
**   user wishes to query for a phrase such as "first place". Using the
**   built-in tokenizers, the FTS5 query 'first + place' will match instances
**   of "first place" within the document set, but not alternative forms
**   such as "1st place". In some applications, it would be better to match
**   all instances of "first place" or "1st place" regardless of which form
**   the user specified in the MATCH query text.
**
**   There are several ways to approach this in FTS5:
**
**   <ol><li> By mapping all synonyms to a single token. In this case, using
**            the above example, this means that the tokenizer returns the
**            same token for inputs "first" and "1st". Say that token is in
**            fact "first", so that when the user inserts the document "I won
**            1st place" entries are added to the index for tokens "i", "won",
**            "first" and "place". If the user then queries for '1st + place',
**            the tokenizer substitutes "first" for "1st" and the query works
**            as expected.
**
**       <li> By querying the index for all synonyms of each query term
**            separately. In this case, when tokenizing query text, the
**            tokenizer may provide multiple synonyms for a single term
**            within the document. FTS5 then queries the index for each
**            synonym individually. For example, faced with the query:
**
**   <codeblock>
**     ... MATCH 'first place'</codeblock>
**
**            the tokenizer offers both "1st" and "first" as synonyms for the
**            first token in the MATCH query and FTS5 effectively runs a query
**            similar to:
**
**   <codeblock>
**     ... MATCH '(first OR 1st) place'</codeblock>
**
**            except that, for the purposes of auxiliary functions, the query
**            still appears to contain just two phrases - "(first OR 1st)"
**            being treated as a single phrase.
**
**       <li> By adding multiple synonyms for a single term to the FTS index.
**            Using this method, when tokenizing document text, the tokenizer
**            provides multiple synonyms for each token. So that when a
**            document such as "I won first place" is tokenized, entries are
**            added to the FTS index for "i", "won", "first", "1st" and
**            "place".
**
**            This way, even if the tokenizer does not provide synonyms
**            when tokenizing query text (it should not - to do so would be
**            inefficient), it doesn't matter if the user queries for
**            'first + place' or '1st + place', as there are entries in the
**            FTS index corresponding to both forms of the first token.
**   </ol>
**
**   Whether it is parsing document or query text, any call to xToken that
**   specifies a <i>tflags</i> argument with the FTS5_TOKEN_COLOCATED bit
**   is considered to supply a synonym for the previous token. For example,
**   when parsing the document "I won first place", a tokenizer that supports
**   synonyms would call xToken() 5 times, as follows:
**
**   <codeblock>
**       xToken(pCtx, 0, "i",                      1,  0,  1);
**       xToken(pCtx, 0, "won",                    3,  2,  5);
**       xToken(pCtx, 0, "first",                  5,  6, 11);
**       xToken(pCtx, FTS5_TOKEN_COLOCATED, "1st", 3,  6, 11);
**       xToken(pCtx, 0, "place",                  5, 12, 17);
**</codeblock>
**
**   It is an error to specify the FTS5_TOKEN_COLOCATED flag the first time
**   xToken() is called. Multiple synonyms may be specified for a single token
**   by making multiple calls to xToken(FTS5_TOKEN_COLOCATED) in sequence.
**   There is no limit to the number of synonyms that may be provided for a
**   single token.
**
**   In many cases, method (1) above is the best approach. It does not add
**   extra data to the FTS index or require FTS5 to query for multiple terms,
**   so it is efficient in terms of disk space and query speed. However, it
**   does not support prefix queries very well. If, as suggested above, the
**   token "first" is substituted for "1st" by the tokenizer, then the query:
**
**   <codeblock>
**     ... MATCH '1s*'</codeblock>
**
**   will not match documents that contain the token "1st" (as the tokenizer
**   will probably not map "1s" to any prefix of "first").
**
**   For full prefix support, method (3) may be preferred. In this case,
**   because the index contains entries for both "first" and "1st", prefix
**   queries such as 'fi*' or '1s*' will match correctly. However, because
**   extra entries are added to the FTS index, this method uses more space
**   within the database.
**
**   Method (2) offers a midpoint between (1) and (3). Using this method,
**   a query such as '1s*' will match documents that contain the literal
**   token "1st", but not "first" (assuming the tokenizer is not able to
**   provide synonyms for prefixes). However, a non-prefix query like '1st'
**   will match against "1st" and "first". This method does not require
**   extra disk space, as no extra entries are added to the FTS index.
**   On the other hand, it may require more CPU cycles to run MATCH queries,
**   as separate queries of the FTS index are required for each synonym.
**
**   When using methods (2) or (3), it is important that the tokenizer only
**   provide synonyms when tokenizing document text (method (3)) or query
**   text (method (2)), not both. Doing so will not cause any errors, but is
**   inefficient.
*/
typedef struct Fts5Tokenizer Fts5Tokenizer;
typedef struct fts5_tokenizer_v2 fts5_tokenizer_v2;
struct fts5_tokenizer_v2 {
  int iVersion;             /* Currently always 2 */

  int (*xCreate)(void*, const char **azArg, int nArg, Fts5Tokenizer **ppOut);
  void (*xDelete)(Fts5Tokenizer*);
  int (*xTokenize)(Fts5Tokenizer*,
      void *pCtx,
      int flags,            /* Mask of FTS5_TOKENIZE_* flags */
      const char *pText, int nText,
      const char *pLocale, int nLocale,
      int (*xToken)(
        void *pCtx,         /* Copy of 2nd argument to xTokenize() */
        int tflags,         /* Mask of FTS5_TOKEN_* flags */
        const char *pToken, /* Pointer to buffer containing token */
        int nToken,         /* Size of token in bytes */
        int iStart,         /* Byte offset of token within input text */
        int iEnd            /* Byte offset of end of token within input text */
      )
  );
};

/*
** New code should use the fts5_tokenizer_v2 type to define tokenizer
** implementations. The following type is included for legacy applications
** that still use it.
*/
typedef struct fts5_tokenizer fts5_tokenizer;
struct fts5_tokenizer {
  int (*xCreate)(void*, const char **azArg, int nArg, Fts5Tokenizer **ppOut);
  void (*xDelete)(Fts5Tokenizer*);
  int (*xTokenize)(Fts5Tokenizer*,
      void *pCtx,
      int flags,            /* Mask of FTS5_TOKENIZE_* flags */
      const char *pText, int nText,
      int (*xToken)(
        void *pCtx,         /* Copy of 2nd argument to xTokenize() */
        int tflags,         /* Mask of FTS5_TOKEN_* flags */
        const char *pToken, /* Pointer to buffer containing token */
        int nToken,         /* Size of token in bytes */
        int iStart,         /* Byte offset of token within input text */
        int iEnd            /* Byte offset of end of token within input text */
      )
  );
};


/* Flags that may be passed as the third argument to xTokenize() */
#define FTS5_TOKENIZE_QUERY     0x0001
#define FTS5_TOKENIZE_PREFIX    0x0002
#define FTS5_TOKENIZE_DOCUMENT  0x0004
#define FTS5_TOKENIZE_AUX       0x0008

/* Flags that may be passed by the tokenizer implementation back to FTS5
** as the third argument to the supplied xToken callback. */
#define FTS5_TOKEN_COLOCATED    0x0001      /* Same position as prev. token */

/*
** END OF CUSTOM TOKENIZERS
*************************************************************************/

/*************************************************************************
** FTS5 EXTENSION REGISTRATION API
*/
typedef struct fts5_api fts5_api;
struct fts5_api {
  int iVersion;                   /* Currently always set to 3 */

  /* Create a new tokenizer */
  int (*xCreateTokenizer)(
    fts5_api *pApi,
    const char *zName,
    void *pUserData,
    fts5_tokenizer *pTokenizer,
    void (*xDestroy)(void*)
  );

  /* Find an existing tokenizer */
  int (*xFindTokenizer)(
    fts5_api *pApi,
    const char *zName,
    void **ppUserData,
    fts5_tokenizer *pTokenizer
  );

  /* Create a new auxiliary function */
  int (*xCreateFunction)(
    fts5_api *pApi,
    const char *zName,
    void *pUserData,
    fts5_extension_function xFunction,
    void (*xDestroy)(void*)
  );

  /* APIs below this point are only available if iVersion>=3 */

  /* Create a new tokenizer */
  int (*xCreateTokenizer_v2)(
    fts5_api *pApi,
    const char *zName,
    void *pUserData,
    fts5_tokenizer_v2 *pTokenizer,
    void (*xDestroy)(void*)
  );

  /* Find an existing tokenizer */
  int (*xFindTokenizer_v2)(
    fts5_api *pApi,
    const char *zName,
    void **ppUserData,
    fts5_tokenizer_v2 **ppTokenizer
  );
};

/*
** END OF REGISTRATION API
*************************************************************************/

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif /* _FTS5_H */

/******** End of fts5.h *********/
#endif /* cortex_H */
