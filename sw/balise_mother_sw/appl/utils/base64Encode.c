#include <stdint.h>
#include <stdlib.h>
#include "base64Encode.h"

static const char encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};
static const int mod_table[] = { 0, 2, 1 };

int base64_encode( const unsigned char *src,
                   unsigned char *dest,
                   size_t input_length, size_t output_length )
{
    int err = 0;
    int i = 0, j = 0;
    
    for( ; i < input_length; )
    {
        uint32_t octet_a = i < input_length ? (unsigned char)src[++i] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)src[++i] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)src[++i] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        dest[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        dest[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        dest[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        dest[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    
    int k = 0;

    for( ; k < mod_table[input_length % 3]; ++k )
    {
        dest[output_length - 1 - k] = '=';
    }
    
    dest[output_length - 1] = '\0';
    
    return err;
}