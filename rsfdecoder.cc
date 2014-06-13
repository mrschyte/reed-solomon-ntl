/*
 *  This file is part of reed-solomon-ntl
 *  Copyright (C) 2009   Peter Kasza
 *
 *  reed-solomon-ntl is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  reed-solomon-ntl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with reed-solomon-ntl.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "rsfencoder.h"

size_t read(std::ifstream& stream, std::vector<unsigned char>& buffer, size_t n) {
	size_t padding = 0;

	buffer.clear();
	for (size_t i = 0; i < n && !(stream.eof()); i++) {
		if (!stream.eof())
			buffer.push_back(stream.get());
		else {
			buffer.push_back(0);
			padding++;
		}
	}

	return padding;
}

void write(std::ofstream& stream, std::vector<unsigned char>& buffer) {
	for (std::vector<unsigned char>::iterator it = buffer.begin(); it != buffer.end(); it++)
		stream.put(*it);
}

void decode(RSEncoder& enc, const char* in_path, const char* out_path) {
	std::vector<unsigned char> message;
	std::vector<unsigned char> encbuf;
	unsigned long padding = 0;
	unsigned long i = 0;

	std::ifstream input(in_path);
	std::ofstream output(out_path);

	input >> padding;
	padding++;

	while (!input.eof()) {
		read(input, message, enc.maxLength() + enc.getParityLength());
		enc.decode(message, encbuf);
		printf("decoded block %d (%d bytes)\n", i, message.size());
		i++;
	}

	for (unsigned int i = 0; i < padding; i++) {
		encbuf.pop_back();
	}

	printf("data was padded by %d bytes\n", padding);
	write(output, encbuf);
}

void help(const char* arg0) {
	printf("Usage: \n\t %s [input] [output]\n", arg0);
}

long const parity = 8;
int main (int argc, char const *argv[]) {
	if (argc > 2) {
		RSEncoder enc(8, parity);
		decode(enc, argv[1], argv[2]);
	} else {
		help(argv[0]);
	}

	return 0;
}

