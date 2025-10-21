#ifndef MAP_H
#define MAP_H

#include "config.h"
#include <cassert>
#include <cstring>
#include <algorithm>

struct map {
	struct line {
		int rows[105];

		line() { memset(rows, 0xff, sizeof(rows)); }

		int &operator[](const int &pos) {
			// assert(0 <= pos);
			// assert(pos < mapWidth);
			return rows[pos];
		}
		line &operator=(const line& t) {
			for(int i=0;i<mapWidth;++i)rows[i] = t.rows[i];
			return *this;
		}
		void clear(){
			memset(rows, 0xff, sizeof(rows));
		}
		~line() = default;
	};

	line mapP[105], mapN[105];
		~map() = default;

	line &operator[](const int &pos) {
		// assert(-mapHeightN <= pos);
		// assert(pos < mapHeight);
		if (pos >= 0) {
			return mapP[pos];
		}
		return mapN[-pos - 1];
	}
	map &operator=(const map& t) {
		for(int i=0;i<mapHeight;++i)mapP[i] = t.mapP[i];
		for(int i=0;i<mapHeightN;++i)mapN[i] = t.mapN[i];
		return *this;
	}
	void clear(){
		for(int i=0;i<mapHeight;i++) mapP[i].clear();
		for(int i=0;i<mapHeightN;i++) mapN[i].clear();
	}
};

static map board;

#endif
