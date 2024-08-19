#ifndef BASE64_H
#define BASE64_H

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
bool is_valid_base64(const char *base64,size_t input_length);
static void error(const char *format, ...);
#endif //BASE64_H

#ifdef IMPLEMENT_BASE64
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
        if(lookup_table[(size_t)base64[len]]==0xff){
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
#endif //IMPLEMENT_BASE64

#ifdef TEST
#include <assert.h>
int main() {
    char *encoded_data;
    unsigned char *decoded_data;

    assert(strcmp("IUBQRFJmMHkkLGdtKwpAV3AqMk80Ug1RNDlzLQkxOUNAXSpLYnZsWyd4VFssCiVMS0lFIDlYO0I2ezREaXplLlRRJClscFJZOmI0RX4yQFNZRTsLaQ==", base64_encode((unsigned char*)"\x21\x40\x50\x44\x52\x66\x30\x79\x24\x2c\x67\x6d\x2b\x0a\x40\x57\x70\x2a\x32\x4f\x34\x52\x0d\x51\x34\x39\x73\x2d\x09\x31\x39\x43\x40\x5d\x2a\x4b\x62\x76\x6c\x5b\x27\x78\x54\x5b\x2c\x0a\x25\x4c\x4b\x49\x45\x20\x39\x58\x3b\x42\x36\x7b\x34\x44\x69\x7a\x65\x2e\x54\x51\x24\x29\x6c\x70\x52\x59\x3a\x62\x34\x45\x7e\x32\x40\x53\x59\x45\x3b\x0b\x69", 85, &encoded_data)) == 0);
    assert(strcmp("\x21\x40\x50\x44\x52\x66\x30\x79\x24\x2c\x67\x6d\x2b\x0a\x40\x57\x70\x2a\x32\x4f\x34\x52\x0d\x51\x34\x39\x73\x2d\x09\x31\x39\x43\x40\x5d\x2a\x4b\x62\x76\x6c\x5b\x27\x78\x54\x5b\x2c\x0a\x25\x4c\x4b\x49\x45\x20\x39\x58\x3b\x42\x36\x7b\x34\x44\x69\x7a\x65\x2e\x54\x51\x24\x29\x6c\x70\x52\x59\x3a\x62\x34\x45\x7e\x32\x40\x53\x59\x45\x3b\x0b\x69", (char *)base64_decode("IUBQRFJmMHkkLGdtKwpAV3AqMk80Ug1RNDlzLQkxOUNAXSpLYnZsWyd4VFssCiVMS0lFIDlYO0I2ezREaXplLlRRJClscFJZOmI0RX4yQFNZRTsLaQ==",116,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("Q3NgMVRPfFtiNyYlYXFRTjxBQ2lyfGZQNUV+eS4kbTt8OUttQSNhXVJzIzg6XWsnPUB2KUNMRy5+Q09BOA0oVw0JZFtbZVNaMiB0Xk5gbGloLTBcWUQ=", base64_encode((unsigned char*)"\x43\x73\x60\x31\x54\x4f\x7c\x5b\x62\x37\x26\x25\x61\x71\x51\x4e\x3c\x41\x43\x69\x72\x7c\x66\x50\x35\x45\x7e\x79\x2e\x24\x6d\x3b\x7c\x39\x4b\x6d\x41\x23\x61\x5d\x52\x73\x23\x38\x3a\x5d\x6b\x27\x3d\x40\x76\x29\x43\x4c\x47\x2e\x7e\x43\x4f\x41\x38\x0d\x28\x57\x0d\x09\x64\x5b\x5b\x65\x53\x5a\x32\x20\x74\x5e\x4e\x60\x6c\x69\x68\x2d\x30\x5c\x59\x44", 86, &encoded_data)) == 0);
    assert(strcmp("\x43\x73\x60\x31\x54\x4f\x7c\x5b\x62\x37\x26\x25\x61\x71\x51\x4e\x3c\x41\x43\x69\x72\x7c\x66\x50\x35\x45\x7e\x79\x2e\x24\x6d\x3b\x7c\x39\x4b\x6d\x41\x23\x61\x5d\x52\x73\x23\x38\x3a\x5d\x6b\x27\x3d\x40\x76\x29\x43\x4c\x47\x2e\x7e\x43\x4f\x41\x38\x0d\x28\x57\x0d\x09\x64\x5b\x5b\x65\x53\x5a\x32\x20\x74\x5e\x4e\x60\x6c\x69\x68\x2d\x30\x5c\x59\x44", (char *)base64_decode("Q3NgMVRPfFtiNyYlYXFRTjxBQ2lyfGZQNUV+eS4kbTt8OUttQSNhXVJzIzg6XWsnPUB2KUNMRy5+Q09BOA0oVw0JZFtbZVNaMiB0Xk5gbGloLTBcWUQ=",116,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("YgtwKz9lRjFaSmgpS2UtDCp4PmhCYwszSTFzNzpXV0BfaWQ7SDN1YGk8bFNTNAk9VFVUQiYvKFRbPWA/bTxpW2Zm", base64_encode((unsigned char*)"\x62\x0b\x70\x2b\x3f\x65\x46\x31\x5a\x4a\x68\x29\x4b\x65\x2d\x0c\x2a\x78\x3e\x68\x42\x63\x0b\x33\x49\x31\x73\x37\x3a\x57\x57\x40\x5f\x69\x64\x3b\x48\x33\x75\x60\x69\x3c\x6c\x53\x53\x34\x09\x3d\x54\x55\x54\x42\x26\x2f\x28\x54\x5b\x3d\x60\x3f\x6d\x3c\x69\x5b\x66\x66", 66, &encoded_data)) == 0);
    assert(strcmp("\x62\x0b\x70\x2b\x3f\x65\x46\x31\x5a\x4a\x68\x29\x4b\x65\x2d\x0c\x2a\x78\x3e\x68\x42\x63\x0b\x33\x49\x31\x73\x37\x3a\x57\x57\x40\x5f\x69\x64\x3b\x48\x33\x75\x60\x69\x3c\x6c\x53\x53\x34\x09\x3d\x54\x55\x54\x42\x26\x2f\x28\x54\x5b\x3d\x60\x3f\x6d\x3c\x69\x5b\x66\x66", (char *)base64_decode("YgtwKz9lRjFaSmgpS2UtDCp4PmhCYwszSTFzNzpXV0BfaWQ7SDN1YGk8bFNTNAk9VFVUQiYvKFRbPWA/bTxpW2Zm",88,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("LXBWSFJPK1d2YExze3IkakNfYG8rX2hHQ0cLTGVMRkJrYE8vZygt", base64_encode((unsigned char*)"\x2d\x70\x56\x48\x52\x4f\x2b\x57\x76\x60\x4c\x73\x7b\x72\x24\x6a\x43\x5f\x60\x6f\x2b\x5f\x68\x47\x43\x47\x0b\x4c\x65\x4c\x46\x42\x6b\x60\x4f\x2f\x67\x28\x2d", 39, &encoded_data)) == 0);
    assert(strcmp("\x2d\x70\x56\x48\x52\x4f\x2b\x57\x76\x60\x4c\x73\x7b\x72\x24\x6a\x43\x5f\x60\x6f\x2b\x5f\x68\x47\x43\x47\x0b\x4c\x65\x4c\x46\x42\x6b\x60\x4f\x2f\x67\x28\x2d", (char *)base64_decode("LXBWSFJPK1d2YExze3IkakNfYG8rX2hHQ0cLTGVMRkJrYE8vZygt",52,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("CnIMSUcj", base64_encode((unsigned char*)"\x0a\x72\x0c\x49\x47\x23", 6, &encoded_data)) == 0);
    assert(strcmp("\x0a\x72\x0c\x49\x47\x23", (char *)base64_decode("CnIMSUcj",8,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("d1IhVT9TY0lPaDElZV85czgxVnFuX1BJO04nMm08eA==", base64_encode((unsigned char*)"\x77\x52\x21\x55\x3f\x53\x63\x49\x4f\x68\x31\x25\x65\x5f\x39\x73\x38\x31\x56\x71\x6e\x5f\x50\x49\x3b\x4e\x27\x32\x6d\x3c\x78", 31, &encoded_data)) == 0);
    assert(strcmp("\x77\x52\x21\x55\x3f\x53\x63\x49\x4f\x68\x31\x25\x65\x5f\x39\x73\x38\x31\x56\x71\x6e\x5f\x50\x49\x3b\x4e\x27\x32\x6d\x3c\x78", (char *)base64_decode("d1IhVT9TY0lPaDElZV85czgxVnFuX1BJO04nMm08eA==",44,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("ZXclXk99dEFSO2FHJUx8cjd2Szh1JWAleCkJCV5iRWIqeGxfJXVjdlQ=", base64_encode((unsigned char*)"\x65\x77\x25\x5e\x4f\x7d\x74\x41\x52\x3b\x61\x47\x25\x4c\x7c\x72\x37\x76\x4b\x38\x75\x25\x60\x25\x78\x29\x09\x09\x5e\x62\x45\x62\x2a\x78\x6c\x5f\x25\x75\x63\x76\x54", 41, &encoded_data)) == 0);
    assert(strcmp("\x65\x77\x25\x5e\x4f\x7d\x74\x41\x52\x3b\x61\x47\x25\x4c\x7c\x72\x37\x76\x4b\x38\x75\x25\x60\x25\x78\x29\x09\x09\x5e\x62\x45\x62\x2a\x78\x6c\x5f\x25\x75\x63\x76\x54", (char *)base64_decode("ZXclXk99dEFSO2FHJUx8cjd2Szh1JWAleCkJCV5iRWIqeGxfJXVjdlQ=",56,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("QW9bZi4zKmZkI09PLFUvTy5lZmZUZHR2MSsxTncjYVMMIlVaeWNoTEJ+ZWpOeGpqTnluUS5nTUJIc09JJj1VYSBUCkxtKkAgejlKRGB7", base64_encode((unsigned char*)"\x41\x6f\x5b\x66\x2e\x33\x2a\x66\x64\x23\x4f\x4f\x2c\x55\x2f\x4f\x2e\x65\x66\x66\x54\x64\x74\x76\x31\x2b\x31\x4e\x77\x23\x61\x53\x0c\x22\x55\x5a\x79\x63\x68\x4c\x42\x7e\x65\x6a\x4e\x78\x6a\x6a\x4e\x79\x6e\x51\x2e\x67\x4d\x42\x48\x73\x4f\x49\x26\x3d\x55\x61\x20\x54\x0a\x4c\x6d\x2a\x40\x20\x7a\x39\x4a\x44\x60\x7b", 78, &encoded_data)) == 0);
    assert(strcmp("\x41\x6f\x5b\x66\x2e\x33\x2a\x66\x64\x23\x4f\x4f\x2c\x55\x2f\x4f\x2e\x65\x66\x66\x54\x64\x74\x76\x31\x2b\x31\x4e\x77\x23\x61\x53\x0c\x22\x55\x5a\x79\x63\x68\x4c\x42\x7e\x65\x6a\x4e\x78\x6a\x6a\x4e\x79\x6e\x51\x2e\x67\x4d\x42\x48\x73\x4f\x49\x26\x3d\x55\x61\x20\x54\x0a\x4c\x6d\x2a\x40\x20\x7a\x39\x4a\x44\x60\x7b", (char *)base64_decode("QW9bZi4zKmZkI09PLFUvTy5lZmZUZHR2MSsxTncjYVMMIlVaeWNoTEJ+ZWpOeGpqTnluUS5nTUJIc09JJj1VYSBUCkxtKkAgejlKRGB7",104,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("NTRFaiV0bGgiXjhZQnJ5DCMoYFJfVFU4fgwuYFAuVVFqcDwtJGJucHI5NFtbQnFQREAsPnRWSmp1dnVRR0hxT3BjWT0yV1Z1P0x0X0dn", base64_encode((unsigned char*)"\x35\x34\x45\x6a\x25\x74\x6c\x68\x22\x5e\x38\x59\x42\x72\x79\x0c\x23\x28\x60\x52\x5f\x54\x55\x38\x7e\x0c\x2e\x60\x50\x2e\x55\x51\x6a\x70\x3c\x2d\x24\x62\x6e\x70\x72\x39\x34\x5b\x5b\x42\x71\x50\x44\x40\x2c\x3e\x74\x56\x4a\x6a\x75\x76\x75\x51\x47\x48\x71\x4f\x70\x63\x59\x3d\x32\x57\x56\x75\x3f\x4c\x74\x5f\x47\x67", 78, &encoded_data)) == 0);
    assert(strcmp("\x35\x34\x45\x6a\x25\x74\x6c\x68\x22\x5e\x38\x59\x42\x72\x79\x0c\x23\x28\x60\x52\x5f\x54\x55\x38\x7e\x0c\x2e\x60\x50\x2e\x55\x51\x6a\x70\x3c\x2d\x24\x62\x6e\x70\x72\x39\x34\x5b\x5b\x42\x71\x50\x44\x40\x2c\x3e\x74\x56\x4a\x6a\x75\x76\x75\x51\x47\x48\x71\x4f\x70\x63\x59\x3d\x32\x57\x56\x75\x3f\x4c\x74\x5f\x47\x67", (char *)base64_decode("NTRFaiV0bGgiXjhZQnJ5DCMoYFJfVFU4fgwuYFAuVVFqcDwtJGJucHI5NFtbQnFQREAsPnRWSmp1dnVRR0hxT3BjWT0yV1Z1P0x0X0dn",104,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);

    assert(strcmp("eXkqal8jJ0JRclVSUkJYYiloUUVXRXk8NjdNZTJcK3JgIXY=", base64_encode((unsigned char*)"\x79\x79\x2a\x6a\x5f\x23\x27\x42\x51\x72\x55\x52\x52\x42\x58\x62\x29\x68\x51\x45\x57\x45\x79\x3c\x36\x37\x4d\x65\x32\x5c\x2b\x72\x60\x21\x76", 35, &encoded_data)) == 0);
    assert(strcmp("\x79\x79\x2a\x6a\x5f\x23\x27\x42\x51\x72\x55\x52\x52\x42\x58\x62\x29\x68\x51\x45\x57\x45\x79\x3c\x36\x37\x4d\x65\x32\x5c\x2b\x72\x60\x21\x76", (char *)base64_decode("eXkqal8jJ0JRclVSUkJYYiloUUVXRXk8NjdNZTJcK3JgIXY=",48,&decoded_data)) == 0);
    free(encoded_data);
    free(decoded_data);
    return 0;
}
#endif //TEST

