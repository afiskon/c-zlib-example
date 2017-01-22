#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <zlib.h>

#define CHUNK_SIZE 4096
#define COMPRESSION_LEVEL 9

bool compress_file(FILE *src, FILE *dst)
{
    uint8_t inbuff[CHUNK_SIZE];
    uint8_t outbuff[CHUNK_SIZE];
    z_stream stream = {0};

    if (deflateInit(&stream, COMPRESSION_LEVEL) != Z_OK)
    {
    	fprintf(stderr, "deflateInit(...) failed!\n");
        return false;
    }

	int flush;
    do {
        stream.avail_in = fread(inbuff, 1, CHUNK_SIZE, src);
        if (ferror(src))
        {
        	fprintf(stderr, "fread(...) failed!\n");
            deflateEnd(&stream);
            return false;
        }

        flush = feof(src) ? Z_FINISH : Z_NO_FLUSH;
        stream.next_in = inbuff;

        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = outbuff;
            deflate(&stream, flush);
            uint32_t nbytes = CHUNK_SIZE - stream.avail_out;

            if (fwrite(outbuff, 1, nbytes, dst) != nbytes || ferror(dst))
            {
            	fprintf(stderr, "fwrite(...) failed!\n");
                deflateEnd(&stream);
                return false;
            }
        } while (stream.avail_out == 0);
    } while (flush != Z_FINISH);

    deflateEnd(&stream);
    return true;
}

int main(int argc, char* argv[])
{
	if(argc >= 2 && strcmp(argv[1], "-h") == 0)
	{
		fprintf(stderr, "Usage: cat infile | %s > outfile\n", argv[0]);
		exit(1);
	}

	bool ok = compress_file(stdin, stdout);
	return ok ? 0 : 1;
}