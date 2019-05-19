#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000000

size_t encode_varint(uint32_t value, uint8_t* buf);
uint32_t generate_number();
uint32_t decode_varint(const uint8_t** bufp);

int main() {
	//system("clear");
	srand(time(NULL));

	FILE* uncompressed_file;
	FILE* compressed_file;
	uncompressed_file = fopen("uncompressed.dat", "w+b");
	if (uncompressed_file == NULL) {
		printf("Error opening file\n");
		return -1;
	}
	compressed_file = fopen("compressed.dat", "w+b");
	if (compressed_file == NULL) {
		printf("Error opening file\n");
		return -1;
	}

	uint32_t numb;
	uint8_t* buf;
	size_t size;
	for (size_t i = 0; i < MAX; i++) {
		numb = generate_number();
		fwrite(&numb, sizeof(uint32_t), 1, uncompressed_file);
		buf = (uint8_t*)&numb;
		size = encode_varint(numb, buf);
		fwrite(buf, sizeof(uint8_t), size, compressed_file);
	}

	rewind(uncompressed_file);// Функция rewind() перемещает указатель положения в файле на начало указанного потока.
	rewind(compressed_file);

	uint32_t garb;
	uint32_t decode;
	buf = (uint8_t*)&garb;
	const uint8_t** bufp;
	int j, k;

	for (k = 0, j = 0; j < MAX; k++, j++) {
		fseek(compressed_file, k, SEEK_SET);
		fread(buf, sizeof(uint8_t), 4, compressed_file);//указатель на область памяти которая получает данные из файла
		
		bufp = (const uint8_t**)&buf;
		decode = decode_varint(bufp);
		fread(&numb, sizeof(uint32_t), 1, uncompressed_file);

		if (decode > 127) k++;
		if (decode > 16383) k++;
		if (decode > 2097151) k++;

		if (numb != decode) {
			printf("Decode Error\n");
			break;
		}
	}
	fclose(uncompressed_file);
	fclose(compressed_file);

	return 0;
}

size_t encode_varint(uint32_t value, uint8_t* buf) {
	assert(buf != NULL);// макросс для проверки корректности программы 
	uint8_t* cur = buf;
	while (value >= 0x80) {
		const uint8_t byte = (value & 0x7f) | 0x80;
		*cur = byte;
		value >>= 7;
		++cur;// указатель смещаем на следующий бит
	}
	*cur = value;
	++cur;
	return cur - buf;
}

uint32_t decode_varint(const uint8_t** bufp) {
	const uint8_t* cur = *bufp;
	uint8_t byte = *cur++;
	uint32_t value = byte & 0x7f;
	size_t shift = 7;
	while (byte >= 0x80) {
		byte = *cur++;
		value += (byte & 0x7f) << shift;
		shift += 7;
	}
	return value;
}

uint32_t generate_number() {
	const int r = rand();
	const int p = r % 100;
	if (p < 90) {
		return r % 128;
	}
	if (p < 95) {
		return r % 16384;
	}
	if (p < 99) {
		return r % 2097152;
	}
	return r % 268435455;
}
