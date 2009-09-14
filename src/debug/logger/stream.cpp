// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
// ----------------------------------------------------------------------------

#include "stream.hpp"
#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 8

xpcc::Stream::Stream() :
	container ( new char[DEFAULT_SIZE] ),
	size ( DEFAULT_SIZE ),
	pos ( 0 )
{
}

// -----------------------------------------------------------------------------

xpcc::Stream::~Stream()
{
	delete[] this->container;
}

// -----------------------------------------------------------------------------

xpcc::Stream&
xpcc::Stream::operator<< ( uint8_t value )
{
	while( this->pos + 4 > this->size ) {
		if( !this->allocate() ) {
			return *this;
		}
	}

	this->pos += sprintf(&this->container[this->pos], "%u", value);

	return *this;
}

//------------------------------------------------------------------------------

void
xpcc::Stream::addString( const char* str, uint8_t need )
{
	while( this->pos + need > this->size ) {
		if( !this->allocate() ) {
			return;
		}
	}

	memcpy( &this->container[this->pos], str, need );
}

// -----------------------------------------------------------------------------

bool
xpcc::Stream::allocate()
{
	char* tmp = new char[2*this->size];
	// TODO: check that allocation was successful

	memcpy( tmp, &this->container[0], this->size );

	delete[] this->container;

	this->container = tmp;
	this->size = 2*this->size;

	return true;
}

// -----------------------------------------------------------------------------

std::ostream&
xpcc::operator<<(std::ostream& os, const xpcc::Stream& c)
{
	os << std::string(c.container);

	return os;
}
