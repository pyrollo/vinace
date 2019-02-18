/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * Vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _C_OBSERVABLE_HPP_
#define _C_OBSERVABLE_HPP_
#include "c-observer.hpp"

class CObserver;

struct observerChain {
	CObserver *obs;
	observerChain *next;
};

class CObservable
{
public:
	CObservable();
	~CObservable();
	void subscribe(CObserver *obs);
	void unsubscribe(CObserver *obs);
	void notifyUpdate();
	observerChain *observers;
protected:
};


#endif // _C_OBSERVABLE_HPP_
