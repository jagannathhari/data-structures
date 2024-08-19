#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MEMEORY_ERROR "Unable to allocate Memory"

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char decode_table[] = {
    62, 255, 255, 255, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255, 255, 255, 255,
    255,255, 0,   1,   2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,  14,  15,  16,  17,
    18, 19,  20,  21,  22, 23, 24, 25, 255,255,255,255,255,255,26, 27,  28,  29,  30,  31,
    32, 33,  34,  35,  36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,  48,  49,  50,  51
};

char *base64_encode(const unsigned char *src, size_t input_length, char **outptr);
unsigned char *base64_decode(const char *src, size_t input_length, unsigned char **outptr);

static void error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, format, args);
    va_end(args);
}

bool is_valid_base64(const char *base64,size_t input_length){
    if(!input_length || input_length % 4){
        return false;
    }

    size_t len = input_length;
    unsigned char lookup_table[256];
    memset(lookup_table,255,sizeof(lookup_table));
    memcpy(&lookup_table['+'],decode_table,sizeof(decode_table));

    if(base64[input_length-1]=='=') len--;
    if(base64[input_length-2]=='=') len--;

    while(len--){
        if(lookup_table[base64[len]]==0xff){
            return false;
        }
    }

    return true;
}

char *base64_encode(const unsigned char *src, size_t input_length, char **outptr) {

    char *result = NULL;
    char *output = NULL;
    result = output = malloc((input_length + 2) / 3 * 4 + 1);

    if (!output) {
        error(MEMEORY_ERROR);
        return NULL;
    }

    int i = input_length;
    const unsigned char *input = src;
    while (i >= 3) {
        *output++ = base64_table[input[0] >> 2];
        *output++ = base64_table[((input[0] & 0x3) << 4) | input[1] >> 4];
        *output++ = base64_table[((input[1] & 0xf) << 2) | input[2] >> 6];
        *output++ = base64_table[input[2] & 0x3f];
        i -= 3;
        input += 3;
    }

    if (i) {
        *output++ = base64_table[input[0] >> 2];
        if (i == 2) {
            *output++ = base64_table[((input[0] & 0x3) << 4) | input[1] >> 4];
            *output++ = base64_table[((input[1] & 0xf) << 2)];
            *output++ = '=';
        } else {
            *output++ = base64_table[(input[0] & 0x3) << 4];
            *output++ = '=';
            *output++ = '=';
        }
    }
    *output = '\0';
    *outptr = result;
    return result;
}

unsigned char *base64_decode(const char *src, size_t input_length, unsigned char **outptr){
    
    if(!is_valid_base64(src,input_length)){
        error("Not a valid base64 content.");
        return NULL;
    }

    size_t padding = 0;
    size_t i = input_length;
    size_t decoded_length = 0;
    if(src[input_length-1] == '='){
       padding++;
       i--;
    }

    if(src[input_length-2] == '='){
       padding++;
       i--;
    }

    decoded_length = (input_length/4)*3 - padding + 1;

    unsigned char *output = NULL;
    unsigned char *result= NULL;
    result = output = malloc(sizeof(*result)*decoded_length + 1);
    if(result == NULL){
        error(MEMEORY_ERROR);
        return NULL;
    }
    unsigned char temp[4];
    while(i>=4){
        for(int j=0;j<4;j++){
            temp[j] = decode_table[*src++ - 43] << 2;
        }
        *output++ = temp[0] | (temp[1] >> 6);
        *output++ = ((temp[1]&0x3c)<<2)| ((temp[2] & 0xf0)>>4);
        *output++ = ((temp[2]&0xc)<<4)| (temp[3] >> 2);
        i-=4;
    }
    if(padding){

        temp[0] = decode_table[*src++ - 43] << 2;
        temp[1] = decode_table[*src++ - 43] << 2;

        *output++ = temp[0] | (temp[1] >> 6);
        if(padding == 1){ // 2 byte avilable

            temp[2] = decode_table[*src++ - 43] << 2;
            *output++ = ((temp[1]&0x3c)<<2)| ((temp[2] & 0xf0)>>4);
        }            
    }
    *output = '\0';
    *outptr = result;
    return result;
}

#ifdef TEST
#include <assert.h>
int main() {
    char *output;
    assert(strcmp("NHA4Q1pdUzNLOU1URSgLMVB9flZAZiNLbD5FTSNrYU0NIiZ7c15WPDkpSTwgQlgJRjZcdiVlNmphU3dnM2JKI21BdE1zSH1CZkc=", base64_encode("\x34\x70\x38\x43\x5a\x5d\x53\x33\x4b\x39\x4d\x54\x45\x28\x0b\x31\x50\x7d\x7e\x56\x40\x66\x23\x4b\x6c\x3e\x45\x4d\x23\x6b\x61\x4d\x0d\x22\x26\x7b\x73\x5e\x56\x3c\x39\x29\x49\x3c\x20\x42\x58\x09\x46\x36\x5c\x76\x25\x65\x36\x6a\x61\x53\x77\x67\x33\x62\x4a\x23\x6d\x41\x74\x4d\x73\x48\x7d\x42\x66\x47", 74, &output)) == 0);
    assert(strcmp("Smo1bHtGaWhfMEQ+UkpIIE52dCt2TFcnXzBxeFpWN1gwOXkid0pgLmZCP3JNUi9xfjVdN24hYXs4LW9zLShXPFxqMiFKbiUJKgwzdTp1fkpWcUYxQFtgelE+Rw==", base64_encode("\x4a\x6a\x35\x6c\x7b\x46\x69\x68\x5f\x30\x44\x3e\x52\x4a\x48\x20\x4e\x76\x74\x2b\x76\x4c\x57\x27\x5f\x30\x71\x78\x5a\x56\x37\x58\x30\x39\x79\x22\x77\x4a\x60\x2e\x66\x42\x3f\x72\x4d\x52\x2f\x71\x7e\x35\x5d\x37\x6e\x21\x61\x7b\x38\x2d\x6f\x73\x2d\x28\x57\x3c\x5c\x6a\x32\x21\x4a\x6e\x25\x09\x2a\x0c\x33\x75\x3a\x75\x7e\x4a\x56\x71\x46\x31\x40\x5b\x60\x7a\x51\x3e\x47", 91, &output)) == 0);
    assert(strcmp("PGttKlZdPi9tCWsNaS82XD4gMA0+LwkvTmQsaVhaL1RSRiF3W0ZPYG0jaloLYjY/aCggQGpbDUwNQWAxNCIvWS5mKg==", base64_encode("\x3c\x6b\x6d\x2a\x56\x5d\x3e\x2f\x6d\x09\x6b\x0d\x69\x2f\x36\x5c\x3e\x20\x30\x0d\x3e\x2f\x09\x2f\x4e\x64\x2c\x69\x58\x5a\x2f\x54\x52\x46\x21\x77\x5b\x46\x4f\x60\x6d\x23\x6a\x5a\x0b\x62\x36\x3f\x68\x28\x20\x40\x6a\x5b\x0d\x4c\x0d\x41\x60\x31\x34\x22\x2f\x59\x2e\x66\x2a", 67, &output)) == 0);
    assert(strcmp("DCR4ZQ0lMj5XMXFJX3NPUXcuU3FdZ2lSVXdack5ZSjxBYVt+IU9jNzVBNkl5JlsMPUBYIy5JQ1dQXw==", base64_encode("\x0c\x24\x78\x65\x0d\x25\x32\x3e\x57\x31\x71\x49\x5f\x73\x4f\x51\x77\x2e\x53\x71\x5d\x67\x69\x52\x55\x77\x5a\x72\x4e\x59\x4a\x3c\x41\x61\x5b\x7e\x21\x4f\x63\x37\x35\x41\x36\x49\x79\x26\x5b\x0c\x3d\x40\x58\x23\x2e\x49\x43\x57\x50\x5f", 58, &output)) == 0);
    assert(strcmp("RC4qC1BXdXYs", base64_encode("\x44\x2e\x2a\x0b\x50\x57\x75\x76\x2c", 9, &output)) == 0);
    assert(strcmp("dys1", base64_encode("\x77\x2b\x35", 3, &output)) == 0);
    assert(strcmp("cCpRIUxCWU1DMk9jOV1UWUBMWD0kNEA9TUVSNWpWQjxnQEZ6N1h6QHF0fV1kS21DcAokQGtmSyg2PCp5RgtHMw==", base64_encode("\x70\x2a\x51\x21\x4c\x42\x59\x4d\x43\x32\x4f\x63\x39\x5d\x54\x59\x40\x4c\x58\x3d\x24\x34\x40\x3d\x4d\x45\x52\x35\x6a\x56\x42\x3c\x67\x40\x46\x7a\x37\x58\x7a\x40\x71\x74\x7d\x5d\x64\x4b\x6d\x43\x70\x0a\x24\x40\x6b\x66\x4b\x28\x36\x3c\x2a\x79\x46\x0b\x47\x33", 64, &output)) == 0);
    assert(strcmp("akgqMyhZPnslWTZ3WX1LPTEvVmAqQklsPEhOLltNZyVsIjd1QmVpPGJ5Zn1yC2RjIno0JFNvfEEjNjokcHlMK0VkP2UqSFxtPA==", base64_encode("\x6a\x48\x2a\x33\x28\x59\x3e\x7b\x25\x59\x36\x77\x59\x7d\x4b\x3d\x31\x2f\x56\x60\x2a\x42\x49\x6c\x3c\x48\x4e\x2e\x5b\x4d\x67\x25\x6c\x22\x37\x75\x42\x65\x69\x3c\x62\x79\x66\x7d\x72\x0b\x64\x63\x22\x7a\x34\x24\x53\x6f\x7c\x41\x23\x36\x3a\x24\x70\x79\x4c\x2b\x45\x64\x3f\x65\x2a\x48\x5c\x6d\x3c", 73, &output)) == 0);
    assert(strcmp("LnVzQ19IWVc5YltpQ3oiLCdyNDk9Z0lONjFVRURaRz15LFhWIiI2ckw4dlxv", base64_encode("\x2e\x75\x73\x43\x5f\x48\x59\x57\x39\x62\x5b\x69\x43\x7a\x22\x2c\x27\x72\x34\x39\x3d\x67\x49\x4e\x36\x31\x55\x45\x44\x5a\x47\x3d\x79\x2c\x58\x56\x22\x22\x36\x72\x4c\x38\x76\x5c\x6f", 45, &output)) == 0);
    assert(strcmp("PjNFIEtVTEAwckBQP3dsK0tgJTtsRDs0Rmd8KTVvRwpbPzFqdWdsOylKdCxMYUdBTkcsNyl1TlpDTnNucl9vMnhbWFw=", base64_encode("\x3e\x33\x45\x20\x4b\x55\x4c\x40\x30\x72\x40\x50\x3f\x77\x6c\x2b\x4b\x60\x25\x3b\x6c\x44\x3b\x34\x46\x67\x7c\x29\x35\x6f\x47\x0a\x5b\x3f\x31\x6a\x75\x67\x6c\x3b\x29\x4a\x74\x2c\x4c\x61\x47\x41\x4e\x47\x2c\x37\x29\x75\x4e\x5a\x43\x4e\x73\x6e\x72\x5f\x6f\x32\x78\x5b\x58\x5c", 68, &output)) == 0);
    return 0;
}
#endif
