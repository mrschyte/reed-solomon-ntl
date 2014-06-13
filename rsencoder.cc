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
#include "rsencoder.h"

// k specifies the number of parity bits
// m specifies extension field exponent ie. GF(2^m)
RSEncoder::RSEncoder(long n, long k) {
	NTL::GF2E::init(NTL::BuildSparseIrred_GF2X(n));
	this->parityLength = k;
	buildGeneratorPoly();
}

RSEncoder::~RSEncoder() {

}

void RSEncoder::encode(const std::vector<unsigned char>& message, std::vector<unsigned char>& buffer) {
	NTL::GF2E alpha, power;
	NTL::GF2EX msg, code;

	if (message.size() > maxLength())
		throw RSException("message is too long");

	/*if (message.size() < minLength())
		throw RSException("message is too short");*/

	for (int i = 0; i < message.size(); i++) {
		NTL::SetCoeff(msg, i, convLongToGF2E(message[i]));
	}

	msg = msg << this->parityLength;
	code = msg + (msg % generator);

	//std::cout << code << std::endl;

	//buffer.clear();
	for (int i = 0; i < message.size() + this->parityLength; i++) {
		buffer.push_back((unsigned char) convGF2EToLong(const_cast<NTL::GF2E&>(NTL::coeff(code, i))));
	}
}

void RSEncoder::decode(const std::vector<unsigned char>& message, std::vector<unsigned char>& buffer) {
	NTL::GF2EX code, lambda, error;
	NTL::vec_GF2E syndromes;

	bool corrupted = false;

	for (int i = 0; i < message.size(); i++) { // set coefficients from coded message
		NTL::SetCoeff(code, i, convLongToGF2E(message[i]));
	}
	//std::cout << code << std::endl;
	
	if (calculateSyndromes(code, syndromes)) { // calculate syndrome values
		berlekamp_massey(lambda, syndromes); // calculate error locator polynomial

		if (!forney_error_magnitudes(error, lambda, syndromes)) { // calculate error polynomial
			throw RSException("message cannot be decoded"); // lambda is not irreducible
			// message cannot be decoded
		}
		code = code + error; // correct error
	}

	//buffer.clear();
	for (int i = this->parityLength; i < message.size(); i++) {
		// copy message to buffer
		buffer.push_back(convGF2EToLong(const_cast<NTL::GF2E&>(NTL::coeff(code, i))));
	}
}

bool RSEncoder::calculateSyndromes(NTL::GF2EX& code, NTL::vec_GF2E& syndromes) {
	NTL::GF2E power, alpha;
	bool corrupted = false;

	syndromes.SetLength(this->parityLength);
	power = alpha = convLongToGF2E(2); // set alpha and power to alpha^1

	for (int i = 0; i < this->parityLength; i++) {
		syndromes[i] = NTL::eval(code, power);

		if (syndromes[i] != 0) // non zero syndrome found, message is corrupted
			corrupted = true;

		power = power * alpha; // power = alpha * alpha^i
	}

	return corrupted;
}

void RSEncoder::buildGeneratorPoly() {
	NTL::GF2E alpha, power;
	NTL::vec_GF2E elems;

	power = alpha = convLongToGF2E(2); // calculate alpha^i i = 1 .. parityLength
	elems.SetLength(this->parityLength);
	for (long i = 0; i < this->parityLength; i++) {
		elems[i] = power;
		power = power * alpha;
	}

	NTL::BuildFromRoots(this->generator, elems); // build generator polynomial prod(x - alpha^i)
}

long RSEncoder::getParityLength() {
	return parityLength;
}

long RSEncoder::maxErrors() {
	return parityLength / 2;
}

long RSEncoder::maxLength() {
	return (1 << NTL::GF2EInfo->p.n) / (8 * sizeof(unsigned char));
}

