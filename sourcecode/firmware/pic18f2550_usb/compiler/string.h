#ifndef __STRING_H
#define __STRING_H

#ifndef __STDDEF_H
#include <stddef.h>
#endif

/* The compiler calling convention varies depending on the target
 * processor family; therefore, there often needs to be separate
 * prototypes depending on which compiler is being used.
 *
 * MPLAB-C18 provides the __18CXX environment variable which will
 * allow us to know that we're compiling for an 18c part. Future
 * versions of MPLAB-C17 will have a corresponding __17CXX environment
 * variable, but as of v2.30.03, it does not.
 *
 * Since the only two Microchip compilers currently available are
 * MPLAB-C17 and MPLAB-C18, the presence or absence of __18CXX
 * is sufficient to determine the target platform.
 */
#if __18CXX

/* Change this to near (or omit altogether) if building small memory model
 * versions of the libraries
 */
#define MEM_MODEL far

/* The ANSI specified versions */
/** @name memcpy
 * ``The {\bf memcpy} funciton copies {\bf n} characters from the object
 * pointed to by {\bf s2} into the object pointed to by {\bf s1}. If
 * copying takes place between objects that overlap, the behaviour is
 * undefined.''
 * Stack usage: 8 bytes. Re-entrant.
 * @param s1 pointer to destination
 * @param s2 pointer to source
 * @param n count of bytes to copy
 * @return ``The {\bf memcpy} function returns the value of {\bf s1}.''
 */
void *memcpy (auto void *s1, auto const void *s2, auto size_t n);

/** @name memmove
 * ``The {\bf memmove} function copies {\bf n} characters from the object
 * pointed to by {\bf s2} into the object pointed to by {\bf s1}. Copying
 * takes place as if the {\bf n} characters from the object pointed to
 * by {\bf s2} are first copied into a temporary array of {\bf n}
 * characters that does not overlap the objects pointed to by {\bf s1}
 * and {\bf s2}, and then the {\bf n} characters from the temporary array
 * are copied into the object pointed to by {\bf s1}.''
 * Stack usage: 8 bytes. Re-entrant.
 * @param s1 pointer to destination
 * @param s2 pointer to source
 * @param n count of bytes to copy
 * @return ``The {\bf memmove} function returns the value of {\bf s1}.''
 */
void *memmove (auto void *s1, auto const void *s2, auto size_t n);

/** @name strcpy
 * `` The {\bf strcpy} function copies the string pointed to by {\bf s2}
 * (including the terminating null character) into the array pointed to
 * by {\bf s1}. If copying takes place between objects that overlap,
 * the behaviour is undefined.''
 * Stack usage: 6 bytes. Re-entrant.
 * @param s1 pointer to destination
 * @param s2 pointer to source
 * @return ``The {\bf strcpy} function returns the value of {\bf s1}.''
 */
char *strcpy (auto char *s1, auto const char *s2);

/** @name strncpy
 * ``The {\bf strncpy} function copies not more than {\bf n} characters
 * (auto characters that follow a null character are not copied) from the
 * array pointed to by {\bf s2} to the array pointed to by {\bf s1}.
 * If {\bf n} characters are copies and no null character is found then
 * {\bf s1} will not be terminated.
 * If copying takes place between objects that overlap, the behaviour
 * is undefined.''
 * Stack usage: 8 bytes. Re-entrant.
 * @param s1 pointer to destination
 * @param s2 pointer to source
 * @param n count of maximum characters to copy
 * @return ``The {\bf strncpy} function returns the value of {\bf s1}.''
 */
char *strncpy (auto char *s1, auto const char *s2, auto size_t n);

/** @name strcat
 * ``The {\bf strcat} function appends a copy of the string pointed to
 * by {\bf s2} (including the terminating null character) to the end
 * of the string pointed to by {\bf s1}. The initial character of
 * {\bf s2} overwrites the null character at the end of {\bf s1}. If
 * copying takes place between objects that overlap, the behaviour is
 * undefined.''
 * Stack usage: 6 bytes. Re-entrant.
 * @param s1 pointer to destination
 * @param s2 pointer to source
 * @return ``The {\bf strcat} function returns the value of {\bf s1}.''
 */
char *strcat (auto char *s1, auto const char *s2);

/** @name strncat
 * ``The {\bf strncat} function appends not more than {\bf n} characters
 * (a null character and characters that follow it are not appended)
 * from the array pointed to by {\bf s2} to the end of the string
 * pointed to by {\bf s1}. The initial character of {\bf s2} overwrites
 * the null character at the end of {\bf s1}. A terminating null 
 * character is always appended to the result. If copying takes place
 * between objects that overlap, the behaviour is undefined.''
 * Stack usage: 8 bytes. Re-entrant.
 * @param s1 pointer to destination
 * @param s2 pointer to source
 * @param n count of maximum characters to copy
 * @return ``The {\bf strncat} function returns the value of {\bf s1}.''
 */
char *strncat (auto char *s1, auto const char *s2, auto size_t n);

/** @name memcmp
 * ``The {\bf memcmp} function compares the first {\bf n} characters of the
 * object pointed to by {\bf s1} to the first {\bf n} characters pointed
 * to by {\bf s2}.''
 * Stack usage: 6 bytes. Re-entrant.
 * @param s1 pointer to object one
 * @param s2 pointer to object two
 * @param n count of characters to compare
 * @return ``The {\bf memcmp} function returns a signed char greater than,
 * equal to, or less than zero, accordingly as the object pointed to by
 * {\bf s1} is greater than, equal to, or less than the object pointed to by
 * {\bf s2}.''
 */
signed char memcmp (auto const void *s1, auto const void *s2, auto size_t n);

/** @name strcmp
 * ``The {\bf strcmp} function compares the string pointed to by {\bf s1} to
 * the string pointed to by {\bf s2}.''
 * Stack usage: 6 bytes. Re-entrant.
 * @param s1 pointer to string one
 * @param s2 pointer to string two
 * @return ``The {\bf strcmp} function returns a signed char greater than,
 * equal to, or less than zero, accordingly as the string pointed to by
 * {\bf s1} is greater than, equal to, or less than the string pointed to
 * by {\bf s2}.''
 */
signed char strcmp (auto const char *s1, auto const char *s2);

/** @name strcoll
 * Locale-aware string comparison. As MPLAB-C18 does not currently support
 * locales, the {\bf strcoll} function is not required and is unimplemented.
 */

/** @name strncmp
 * ``The {\bf strncmp} function compares not more than {\bf n} characters
 * (auto characters that follow a null character are not compared) from the 
 * array pointed to by {\bf s1} to the array pointed to by {\bf s2}.''
 * Stack usage: 8 bytes. Re-entrant.
 * @param s1 pointer to string one
 * @param s2 pointer to string two
 * @param n count of characters to compare
 * @return ``The {\bf strncmp} function returns a signed char greater than,
 * equal to, or less than zero, accordiongly as the possibly null-terminated
 * array pointed to by {\bf s1} is greater than, equal to, or less than the
 * possibly null-terminated array pointed to by {\bf s2}.''
 */
signed char strncmp (auto const char *s1, auto const char *s2, auto size_t n);

/** @name strxfrm
 * As MPLAB-C18 does not currently support locales, the {\bf strxfrm}
 * function is not required and is unimplemented.
 */

/** @name memchr
 * ``The {\bf memchr} function locates the first occurence of {\bf c} [...]
 * in the initial {\bf n} characters [...] of the object pointed to by
 * {\bf s}.
 *
 * The MPLAB-C18 version of the {\bf memchr} function differs from the ANSI
 * specified function in that {\bf c} is defined as an {\bf unsigned char}
 * parameter rather than an {\bf int} parameter.''
 * Stack usage: 5 bytes. Re-entrant.
 * @param s pointer to object to search
 * @param c character to search for
 * @param n maximum number of chararacters to search
 * @return ``The {\bf memchr} function returns a pointer to the located character,
 * or a null pointer if the character does not occur in the object.''
 */
void *memchr (auto const void *s, auto unsigned char c, auto size_t n);

/** @name strchr
 * ``The {\bf strchr} function locates the first occurence of {\bf c} [...]
 * in the string pointed to by {\bf s}. The terminating null character is
 * considered to be part of the string.''
 *
 * The MPLAB-C18 version of the {\bf strchr} function differs from the ANSI
 * specified function in that {\bf c} is defined as an {\bf unsigned char}
 * parameter rather than an {\bf int} parameter.
 * Stack usage: 3 bytes. Re-entrant.
 * @param s pointer to string to search
 * @param c character to search for
 * @return ``The {\bf strchr} function returns a pointer to the located character,
 * or a null pointer if the character does not occur in the string.''
 */
char *strchr (auto const char *s, auto unsigned char c);

/** @name strcspn
 * ``The {\bf strcspn} function computes the length of the maximum initial
 * segment of the string pointed to by {\bf s1} which consists entirely
 * of characters {\it not} from the string pointed to by {\bf s2}.''
 * Stack usage: 6 bytes. Re-entrant.
 * @param s1 pointer to string to span
 * @param s2 pointer to set of characters
 * @return ``The {\bf strcspn} function returns the length of the segment.''
 */
size_t strcspn (auto const char *s1, auto const char *s2);

/** @name strpbrk
 * ``The {\bf strpbrk} function locates the first occurrence in the string
 * pointed to by {\bf s1} of any character from the string pointed to by
 * {\bf s2}.''
 * Stack usage: 6 bytes. Re-entrant.
 * @param s1 pointer to string to search
 * @param s2 pointer to set of characters
 * @return ``The {\bf strpbrk} function returns a pointer to the character,
 * or a null-pointer if no character from {\bf s2} occurs in {\bf s1}.''
 */
char *strpbrk (auto const char *s1, auto const char *s2);

/** @name strrchr
 * ``The {\bf strrchr} function locates the last occurrence of {\bf c} [...]
 * in the string pointed to by {\bf s}. The terminating null character is 
 * considered to be part of the string.''
 *
 * The MPLAB-C18 version of the {\bf strrchr} function differs from the ANSI
 * specified function in that {\bf c} is defined as an {\bf unsigned char}
 * parameter rather than an {\bf int} parameter.
 * Stack usage: 3 bytes. Re-entrant.
 * @param s pointer to string to search
 * @param c character to search for
 * @return ``The {\bf strrchr} function returns a pointer to the character,
 * or a null pointer if {\bf c} does not occur in the string.''
 */
char *strrchr (auto const char *s, auto unsigned char c);

/** @name strspn
 * ``The {\bf strspn} function computes the length of the maximum initial
 * segment of the string pointed to by {\bf s1} which consists entirely
 * of characters from the string pointed to by {\bf s2}.''
 * Stack usage: 6 bytes. Re-entrant.
 * @param s1 pointer to string to span
 * @param s2 pointer to set of characters
 * @return ``The {\bf strspn} function returns the length of the segment.''
 */
size_t strspn (auto const char *s1, auto const char *s2);

/** @name strstr
 * ``The {\bf strstr} function locates the first occurrence in the string 
 * pointed to by {\bf s1} of the sequence of characters (excluding the
 * null terminator) in the string pointed to by {\bf s2}.''
 * Stack usage: 8 bytes. Re-entrant.
 * @param s1 pointer to the string to search
 * @param s2 pointer to sequence to search for
 * @return ``The {\bf strstr} function returns a pointer to the located
 * string, or a null pointer if the string is not found. If {\bf s2}
 * points to a string with zero length, the function returns {\bf s1}.''
 */
char *strstr (auto const char *s1, auto const char *s2);

/** @name strtok
 * ``A sequence of calls to the {\bf strtok} function breaks the
 * string pointed to by {\bf s1} into a sequence of tokens, each of
 * which is delimited by a character from the string pointed to by
 * {\bf s2}. The first call in the sequence has {\bf s1} as its
 * first argument, and is followed by calls with a null pointer
 * as their first argument. The separator string pointed to by {\bf s2}
 * may be different from call to call.
 *
 * ``The first call in the sequence searches the string pointed to
 * by {\bf s1} for the first character that is {\it not} contained in
 * the current separator string {\bf s2}. If no such character is found,
 * then there are no tokens in the string pointed to by {\bf s1} and the
 * {\bf strtok} function returns a null pointer. If such a character is
 * found, it is the start of the first token.
 *
 * ``The {\bf strtok} function then searches from there for a character
 * that {\it is} contained in the current separator string. If no such
 * character is found, the current token extends to the end of the
 * string pointed to by {\bf s1}, and subsequent searches for a token
 * will return a null pointer. If such a character is found, it is
 * overwritten by a null character, which terminates the current token.
 * The {\bf strtok} function saves a pointer to the following character,
 * from which the next search for a token will start.
 *
 * ``Each subsequent call, with a null pointer as the first argument, 
 * starts searching from the saved pointer and behaves as described
 * above.
 *
 * ``The implementation shall behave as if no library function calls the
 * {\bf strtok} function.
 *
 * ``This function is implemented in C, is not re-entrant and calls the
 * functions: strspn, strpbrk, memchr.''
 *
 * @param s1 pointer to a string to begin searching, or null to continue
 * searching a prior string
 * @param s2 pointer to a string containing the set of separator characters
 * @return ``The {\bf strtok} function returns a pointer to the first
 * character of a token, or a null pointer if there is no token.'' 
 */
char *strtok (auto char *s1, auto const char *s2);

/** @name memset
 * ``The {\bf memset} function copies the value of {\bf c} [...] into
 * each of the first {\bf n} characters of the object pointed to by
 * {\bf s}.''
 *
 * The MPLAB-C18 version of the {\bf memset} function differs from the ANSI
 * specified function in that {\bf c} is defined as an {\bf unsigned char}
 * parameter rather than an {\bf int} parameter.
 * Stack usage: 5 bytes. Re-entrant.
 * @param s pointer to object
 * @param c character to copy into object
 * @param n number of bytes of object to copy {\bf c} into
 * @return ``The {\bf memset} function returns the value of {\bf s}.''
 */
void *memset (auto void *s, auto unsigned char c, auto size_t n);

/** @name strerror
 * ``The {\bf strerror} function maps the error number in {\bf errnum} to
 * an error message string.
 *
 * ``The implementation shall behave as if no library function calls the
 * {\bf strerror} function.''
 *
 * MPLAB-C18 currently defines the {\bf strerror} function to return
 * an empty string for all values of {\bf errnum}. Future versions may
 * implement this function in a more meaningful manner.
 * @param errnum error number
 * @return ``The {\bf strerror} function returns a pointer to the string,
 * the contents of which are implmentation defined. The array pointed to
 * shall not be modified by the program, but may be overwritten by a
 * subsequent call to the {\bf strerror} function.''
 */
#define strerror(n) ((n),"")

/** @name strlen
 * ``The {\bf strlen} function computes the length of the string pointed
 * to by {\bf s}.''
 * Stack usage: 2 bytes. Re-entrant.
 * @param s pointer to the string
 * @return ``The {\bf strlen} function returns the number of characters
 * that precede the terminating null character.''
 */
size_t strlen (auto const char *s);

/** @name strupr
 * The {\bf strupr} function converts each lower case character in the
 * string pointed to by {\bf s} to the corresponding upper case character.
 * Stack usage: 2 bytes. Re-entrant.
 * @param s pointer to string
 * @return The {\bf strupr} function returns the value of {\bf s}.
 */
char *strupr (auto char *s);

/** @name strlwr
 * The {\bf strlwr} function converts each upper case character in the
 * string pointed to by {\bf s} to the corresponding lower case character.
 * Stack usage: 2 bytes. Re-entrant.
 * @param s pointer to string
 * @return The {\bf strlwr} function returns the value of {\bf s}.
 */
char *strlwr (auto char *s);



/* The versions which deal with program memory data */
/** @name memcpypgm
 * The {\bf memcpypgm} function performs a {\bf memcpy} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 * @param n number of characters to copy
 */
MEM_MODEL rom void *memcpypgm (auto MEM_MODEL rom void *s1, auto const MEM_MODEL rom void *s2, auto sizerom_t n);

/** @name memcpypgm2ram
 * The {\bf memcpypgm2ram} function performs a {\bf memcpy} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 * @param n number of characters to copy
 */
void *memcpypgm2ram (auto void *s1, auto const MEM_MODEL rom void *s2, auto sizeram_t n);

/** @name memcpyram2pgm
 * The {\bf memcpyram2pgm} function performs a {\bf memcpy} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 * @param n number of characters to copy
 */
MEM_MODEL rom void *memcpyram2pgm (auto MEM_MODEL rom void *s1, auto const void *s2, auto sizeram_t n);

/** @name memmovepgm
 * The {\bf memmovepgm} function performs a {\bf memmove} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 * @param n number of characters to copy
 */
MEM_MODEL rom void *memmovepgm (auto MEM_MODEL rom void *s1, auto const MEM_MODEL rom void *s2, auto sizerom_t n);

/** @name memmovepgm2ram
 * The {\bf memmovepgm2ram} function performs a {\bf memmove} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 * @param n number of characters to copy
 */
void *memmovepgm2ram (auto void *s1, auto const MEM_MODEL rom void *s2, auto sizeram_t n);

/** @name memmoveram2pgm
 * The {\bf memmoveram2pgm} function performs a {\bf memmove} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 * @param n number of characters to copy
 */
MEM_MODEL rom void *memmoveram2pgm (auto MEM_MODEL rom void *s1, auto const void *s2, auto sizeram_t n);

/** @name strcpypgm
 * The {\bf strcpypgm} function performs a {\bf strcpy} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 */
MEM_MODEL rom char *strcpypgm (auto MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strcpypgm2ram
 * The {\bf strcpypgm2ram} function performs a {\bf strcpy} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 */
char *strcpypgm2ram (auto char *s1, auto const MEM_MODEL rom char *s2);

/** @name strcpyram2pgm
 * The {\bf strcpyram2pgm} function performs a {\bf strcpy} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 */
MEM_MODEL rom char *strcpyram2pgm (auto MEM_MODEL rom char *s1, auto const char *s2);

/** @name strncpypgm
 * The {\bf strncpypgm} function performs a {\bf strncpy} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 * @param n maximum number of characters to copy
 */
MEM_MODEL rom char *strncpypgm (auto MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2, auto sizerom_t n);

/** @name strncpypgm2ram
 * The {\bf strncpypgm2ram} function performs a {\bf strncpy} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 * @param n maximum number of characters to copy
 */
char *strncpypgm2ram (auto char *s1, auto const MEM_MODEL rom char *s2, auto sizeram_t n);

/** @name strncpyram2pgm
 * The {\bf strncpyram2pgm} function performs a {\bf strncpy} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 * @param n maximum number of characters to copy
 */
MEM_MODEL rom char *strncpyram2pgm (auto MEM_MODEL rom char *s1, auto const char *s2, auto sizeram_t n);

/** @name strcatpgm
 * The {\bf strcatpgm} function performs a {\bf strcat} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 */
MEM_MODEL rom char *strcatpgm (auto MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strcatpgm2ram
 * The {\bf strcatpgm2ram} function performs a {\bf strcat} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 */
char *strcatpgm2ram (auto char *s1, auto const MEM_MODEL rom char *s2);

/** @name strcatram2pgm
 * The {\bf strcatram2pgm} function performs a {\bf strcat} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 */
MEM_MODEL rom char *strcatram2pgm (auto MEM_MODEL rom char *s1, auto const char *s2);

/** @name strncatpgm
 * The {\bf strncatpgm} function performs a {\bf strncat} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 * @param n maximum number of characters to copy
 */
MEM_MODEL rom char *strncatpgm (auto MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2, auto sizerom_t n);

/** @name strncatpgm2ram
 * The {\bf strncatpgm2ram} function performs a {\bf strncat} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 * @param n maximum number of characters to copy
 */
char *strncatpgm2ram (auto char *s1, auto const MEM_MODEL rom char *s2, auto sizeram_t n);

/** @name strncatram2pgm
 * The {\bf strncatram2pgm} function performs a {\bf strncat} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 * @param n maximum number of characters to copy
 */
MEM_MODEL rom char *strncatram2pgm (auto MEM_MODEL rom char *s1, auto const char *s2, auto sizeram_t n);

/** @name memcmppgm
 * The {\bf memcmppgm} function performs a {\bf memcmp} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in program memory
 * @param n number of characters to compare
 */
signed char memcmppgm (auto MEM_MODEL rom void *s1, auto const MEM_MODEL rom void *s2, auto sizerom_t n);

/** @name memcmppgm2ram
 * The {\bf memcmppgm2ram} function performs a {\bf memcmp} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to string in data memory
 * @param s2 pointer to string in program memory
 * @param n number of characters to compare
 */
signed char memcmppgm2ram (auto void *s1, auto const MEM_MODEL rom void *s2, auto sizeram_t n);

/** @name memcmpram2pgm
 * The {\bf memcmpram2pgm} function performs a {\bf memcmp} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in data memory
 * @param n number of characters to compare
 */
signed char memcmpram2pgm (auto MEM_MODEL rom void *s1, auto const void *s2, auto sizeram_t n);

/** @name strcmppgm
 * The {\bf strcmppgm} function performs a {\bf strcmp} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in program memory
 */
signed char strcmppgm (auto const MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strcmppgm2ram
 * The {\bf strcmppgm2ram} function performs a {\bf strcmp} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to string in data memory
 * @param s2 pointer to string in program memory
 */
signed char strcmppgm2ram (auto const char *s1, auto const MEM_MODEL rom char *s2);

/** @name strcmpram2pgm
 * The {\bf strcmpram2pgm} function performs a {\bf strcmp} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in data memory
 */
signed char strcmpram2pgm (auto const MEM_MODEL rom char *s1, auto const char *s2);

/** @name strncmppgm
 * The {\bf strncmppgm} function performs a {\bf strncmp} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in program memory
 * @param n number of characters to compare
 */
signed char strncmppgm (auto const MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2, auto sizerom_t n);

/** @name strncmppgm2ram
 * The {\bf strncmppgm2ram} function performs a {\bf strncmp} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to string in data memory
 * @param s2 pointer to string in program memory
 * @param n number of characters to compare
 */
signed char strncmppgm2ram (auto char *s1, auto const MEM_MODEL rom char *s2, auto sizeram_t n);

/** @name strncmpram2pgm
 * The {\bf strncmpram2pgm} function performs a {\bf strncmp} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in data memory
 * @param n number of characters to compare
 */
signed char strncmpram2pgm (auto MEM_MODEL rom char *s1, auto const char *s2, auto sizeram_t n);

/** @name memchrpgm
 * The {\bf memchrpgm} function performs a {\bf memchr} where {\bf s} 
 * points to program memory.
 * @param s pointer to object to search
 * @param c character to search for
 * @param n maximum number of chararacters to search
 */
MEM_MODEL rom char *memchrpgm (auto const MEM_MODEL rom char *s, auto const unsigned char c, auto sizerom_t n);

/** @name strchrpgm
 * The {\bf strchrpgm} function performs a {\bf strchr} where {\bf s} 
 * points to program memory.
 * @param s pointer to object to search
 * @param c character to search for
 * @param n maximum number of chararacters to search
 */
MEM_MODEL rom char *strchrpgm (auto const MEM_MODEL rom char *s, auto unsigned char c);

/** @name strcspnpgm
 * The {\bf strcspnpgm} function performs a {\bf strcspn} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in program memory
 */
sizerom_t strcspnpgm (auto const MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strcspnpgmram
 * The {\bf strcspnpgmram} function performs a {\bf strcspn} where {\bf s1} 
 * points to program memory and {\bf s2} points to data memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in data memory
 */
sizerom_t strcspnpgmram (auto const MEM_MODEL rom char *s1, auto const char *s2);

/** @name strcspnrampgm
 * The {\bf strcspnrampgm} function performs a {\bf strcspn} where {\bf s1} 
 * points to program memory and {\bf s2} points to data memory.
 * @param s1 pointer to string in data memory
 * @param s2 pointer to string in program memory
 */
sizeram_t strcspnrampgm (auto const char *s1, auto const MEM_MODEL rom char *s2);

/** @name strpbrkpgm
 * The {\bf strpbrkpgm} function performs a {\bf strpbrk} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in program memory
 */
MEM_MODEL rom char *strpbrkpgm (auto const MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strpbrkpgmram
 * The {\bf strpbrkpgmram} function performs a {\bf strpbrk} where {\bf s1} 
 * points to program memory and {\bf s2} points to data memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in data memory
 */
MEM_MODEL rom char *strpbrkpgmram (auto const MEM_MODEL rom char *s1, auto const char *s2);

/** @name strpbrkrampgm
 * The {\bf strpbrkrampgm} function performs a {\bf strpbrk} where {\bf s1} 
 * points to program memory and {\bf s2} points to data memory.
 * @param s1 pointer to string in data memory
 * @param s2 pointer to string in program memory
 */
char *strpbrkrampgm (auto const char *s1, auto const MEM_MODEL rom char *s2);

/** @name strrchrpgm
 * The {\bf strchrpgm} function performs a {\bf strchr} where {\bf s} 
 * points to program memory.
 * @param s pointer to object to search
 * @param c character to search for
 * @param n maximum number of chararacters to search
 */
/* MEM_MODEL rom char *strrchrpgm (auto MEM_MODEL rom char *s, auto unsigned char c); */

/** @name strspnpgm
 * The {\bf strspnpgm} function performs a {\bf strspn} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in program memory
 */
sizerom_t strspnpgm (auto const MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strspnpgmram
 * The {\bf strspnpgmram} function performs a {\bf strspn} where {\bf s1} 
 * points to program memory and {\bf s2} points to data memory.
 * @param s1 pointer to string in program memory
 * @param s2 pointer to string in data memory
 */
sizerom_t strspnpgmram (auto const MEM_MODEL rom char *s1, auto const char *s2);

/** @name strspnrampgm
 * The {\bf strspnrampgm} function performs a {\bf strspn} where {\bf s1} 
 * points to program memory and {\bf s2} points to data memory.
 * @param s1 pointer to string in data memory
 * @param s2 pointer to string in program memory
 */
sizeram_t strspnrampgm (auto const char *s1, auto const MEM_MODEL rom char *s2);

/** @name strstrpgm
 * The {\bf strstrpgm} function performs a {\bf strstr} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 */
MEM_MODEL rom char *strstrpgm (auto const MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strstrpgmram
 * The {\bf strstrpgmram} function performs a {\bf strstr} where
 * {\bf s1} points to program memory and {\bf s2} points to data
 * memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 */
MEM_MODEL rom char *strstrpgmram (auto const MEM_MODEL rom char *s1, auto const char *s2);

/** @name strstrrampgm
 * The {\bf strstrrampgm} function performs a {\bf strstr} where {\bf s1} 
 * points to data memory and {\bf s2} points to program memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 */
char *strstrrampgm (auto const char *s1, auto const MEM_MODEL rom char *s2);

/** @name strtokpgm
 * The {\bf strtokpgm} function performs a {\bf strtok} where both
 * {\bf s1} and {\bf s2} point to program memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in program memory
 */
MEM_MODEL rom char *strtokpgm (auto MEM_MODEL rom char *s1, auto const MEM_MODEL rom char *s2);

/** @name strtokpgmram
 * The {\bf strtokpgmram} function performs a {\bf strtok} where
 * {\bf s1} points to data memory and {\bf s2} points to program
 * memory.
 * @param s1 pointer to destination in data memory
 * @param s2 pointer to source in program memory
 */
char *strtokpgmram (auto char *s1, auto const MEM_MODEL rom char *s2);

/** @name strtokrampgm
 * The {\bf strtokrampgm} function performs a {\bf strtok} where {\bf s1} 
 * points to program memory and {\bf s2} point to data memory.
 * @param s1 pointer to destination in program memory
 * @param s2 pointer to source in data memory
 */
MEM_MODEL rom char *strtokrampgm (auto MEM_MODEL rom char *s1, auto const char *s2);

/** @name memsetpgm
 * The {\bf memsetpgm} function performs a {\bf memset} where {\bf s} 
 * points to program memory.
 * @param s pointer to object in program memory
 * @param c character to copy into object
 * @param n number of characters of object to copy {\bf c} into
 */
MEM_MODEL rom void *memsetpgm (auto MEM_MODEL rom void *s, auto unsigned char c, auto sizerom_t n);

/** @name struprpgm
 * The {\bf struprpgm} function performs a {\bf strupr} where {\bf s} 
 * points to program memory.
 * @param s pointer to object in program memory
 */
MEM_MODEL rom char *struprpgm (auto MEM_MODEL rom char *s);

/** @name strlwrpgm
 * The {\bf strlwrpgm} function performs a {\bf strlwr} where {\bf s} 
 * points to program memory.
 * @param s pointer to object in program memory
 */
MEM_MODEL rom char *strlwrpgm (auto MEM_MODEL rom char *s);

/** @name strlenpgm
 * The {\bf strlenpgm} function performs a {\bf strlen} where {\bf s} 
 * points to program memory.
 * @param s pointer to object in program memory
 */
sizerom_t strlenpgm (auto const MEM_MODEL rom char *s);

#undef MEM_MODEL

#else
/* The versions of the libraries shipped with MPLAB-C17 are currently
 * not completely correct. Specifically, the functions should return
 * a pointer to the destination buffer as the MPLAB-C18 versions do.
 * This will be corrected in a future version of MPLAB-C17.
 *
 * There are also a number of functions missing from the MPLAB-C17
 * implementation.
 */
void strcat (static char *, static char *);
signed char strcmp (static char *, static char *);
void strcpy (static char *, static char *);
unsigned char strlen (static char *);
void strlwr (static char *);
void strupr (static char *);
void strset (static char *, static char);
#endif

#endif
