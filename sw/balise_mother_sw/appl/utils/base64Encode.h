#ifndef _BASE64ENCODE_H
#define	_BASE64ENCODE_H

#include <stdlib.h>

int base64_encode( const unsigned char *src,
                   unsigned char *dest,
                   size_t input_length, size_t output_length );

#endif //_BASE64ENCODE_H