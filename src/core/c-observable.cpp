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

#include "c-observable.hpp"

CObservable::CObservable() {
	observers = 0;
}

CObservable::~CObservable() {
	observerChain *entry = observers;
	observerChain *nextentry;
	while (entry) {
		nextentry = entry->next;
		delete entry;
		entry = nextentry;
	}
}

void CObservable::subscribe(CObserver *obs) {
	observerChain *entry = new observerChain;

	// Add observer to the chained list
	entry->next = observers;
	entry->obs = obs;
	observers = entry;
}

void CObservable::unsubscribe(CObserver *obs) {
	observerChain *entry = observers;
	observerChain *lastentry = 0;

	while (entry) {
		if (entry->obs==obs)
			if (lastentry) {
				lastentry->next = entry->next;
				delete entry;
				entry = lastentry->next;
			} else {
				observers = entry->next;
				delete entry;
				entry = observers;
			}
		else
			entry = entry->next;
	}
}

void CObservable::notifyUpdate() {
	observerChain *entry = observers;
	while (entry) {
		entry->obs->notify(this);
		entry = entry->next;
	}
}

