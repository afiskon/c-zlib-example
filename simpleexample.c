#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <zlib.h>

int main(int argc, char* argv[])
{
	int res;

	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s <fname>\n", argv[0]);
		return 1;
	}

	char* fname = argv[1];

	struct stat file_stat;
	res = stat(fname, &file_stat);
	if(res == -1)
	{
		fprintf(stderr, "stat(...) failed, errno = %d\n", errno);
		return 1;
	}

	size_t temp_file_size = (size_t)file_stat.st_size;
	if(temp_file_size >= INT_MAX)
	{
		fprintf(stderr, "Error: filze_size >= INT_MAX (%d)\n", INT_MAX);
		return 1;
	}

	int file_size = (int)temp_file_size;
	int buff_size = file_size + 1;
	void* file_buff = malloc(buff_size);
	if(file_buff == NULL)
	{
		fprintf(stderr, "malloc(buff_size) failed, buff_size = %d\n",
			file_size);
		return 1;
	}

	int fid = open(fname, O_RDONLY);
	if(fid == -1)
	{
		fprintf(stderr, "open(...) failed, errno = %d\n", errno);
		free(file_buff);
		return 1;
	}

	if(read(fid, file_buff, file_size) != file_size)
	{
		fprintf(stderr, "read(...) failed, errno = %d\n", errno);
		free(file_buff);
		close(fid);
		return 1;
	}

	close(fid);

	uLongf compress_buff_size = compressBound(file_size);
	void* compress_buff = malloc(compress_buff_size);
	if(compress_buff == NULL)
	{
		fprintf(stderr,
			"malloc(compress_buff_size) failed, "
			"compress_buff_size = %lu\n",
			compress_buff_size);
		free(file_buff);
		return 1;
	}

	uLongf compressed_size = compress_buff_size;
	res = compress(compress_buff, &compressed_size, file_buff, file_size);
	if(res != Z_OK)
	{
		fprintf(stderr, "compress(...) failed, res = %d\n", res);
		free(compress_buff);
		free(file_buff);
		return 1;
	}

	memset(file_buff, 0, buff_size);
	uLongf decompressed_size = (uLongf)file_size;
	res = uncompress(file_buff, &decompressed_size,
		compress_buff, compressed_size);
	if(res != Z_OK)
	{
		fprintf(stderr, "uncompress(...) failed, res = %d\n", res);
		free(compress_buff);
		free(file_buff);
		return 1;
	}

	printf(
		"%s\n----------------\n"
		"File size: %d, compress_buff_size: %lu, compressed_size: %lu, "
		"decompressed_size: %lu\n",
		(char*)file_buff, file_size, compress_buff_size, compressed_size,
		decompressed_size);

	free(compress_buff);
	free(file_buff);
}