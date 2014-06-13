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
#include "reed-solomon.h"

void printUCharVector(const std::vector<unsigned char>& v, long left, long right) {
	if (v.size() != 0) {
		if (v.size() < right)
			right = v.size();

		for (long i = left; (i < right - 1); i++) {
			printf("%02x", v[i]);
		}

		printf("%02x\n", v[right - 1]);
	}
}

void printUCharVector(const std::vector<unsigned char>& v) {
	printUCharVector(v, 0, v.size());
}

void generateMessage(std::vector<unsigned char>& message, size_t n) {
	message.clear();
	for (size_t i = 0; i < n; i++)
		message.push_back(rand() % (1 << (8 * sizeof(unsigned char))));
}

void corruptMessage(std::vector<unsigned char>& message, size_t n) {
	size_t index = rand() % (message.size() - n);
	for (size_t i = 0; i < n; i++) {
		message[index + i] = rand() % (1 << (8 * sizeof(unsigned char)));
	}
}

bool verifyMessage(std::vector<unsigned char>& omsg, std::vector<unsigned char>& dmsg) {
	if (omsg.size() != dmsg.size())
		return false;

	for (size_t i = 0; i < omsg.size(); i++) {
		if (omsg[i] != dmsg[i]) {
			printf("%05d [%02x : %02x]\n", i, omsg[i], dmsg[i]);
			return false;
		}
	}
	return true;
}

int main (int argc, char const *argv[]) {
	std::vector<unsigned char> message;
	std::vector<unsigned char> encbuf;
	std::vector<unsigned char> decbuf;
	RSEncoder enc(8, 8);

	srand(time(NULL));

	generateMessage(message, enc.maxLength());
	printf("generated message: \n\t");
	printUCharVector(message);
	printf("\n");

	enc.encode(message, encbuf);
	printf("encoded message: \n\t");
	printUCharVector(encbuf);
	printf("\n");

	corruptMessage(encbuf, enc.maxErrors());
	printf("corrupted message: (%d symbols) \n\t", enc.maxErrors());
	printUCharVector(encbuf);
	printf("\n");

	enc.decode(encbuf, decbuf);
	printf("decoded message: \n\t");
	printUCharVector(decbuf);
	printf("\n");

	if (verifyMessage(message, decbuf)) {
		std::cout << "message decoded successfully" << std::endl;
	} else {
		std::cout << "decoding error (this shouldn't happen)" << std::endl;
	}
	return 0;
}
