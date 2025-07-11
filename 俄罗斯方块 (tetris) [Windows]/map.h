#ifndef MAP_H
#define MAP_H

#include "config.h"
#include <cassert>
#include <cstring>

struct map {
	struct line {
		int rows[mapWidth];

		line() { memset(rows, 0xff, sizeof(rows)); }

		int &operator[](const int &pos) {
			assert(0 <= pos && pos < mapWidth);
			return rows[pos];
		}
		void clear(){
			memset(rows, 0xff, sizeof(rows));
		}
	};

	line mapP[mapHeightP], mapN[mapHeightN];

	line &operator[](const int &pos) {
		assert(-mapHeightN <= pos);
		assert(pos < mapHeightP);
		if (pos >= 0) {
			return mapP[pos];
		}
		return mapN[-pos - 1];
	}
	void clear(){
		for(int i=0;i<mapHeightP;i++) mapP[i].clear();
		for(int i=0;i<mapHeightN;i++) mapN[i].clear();
	}
};

static map board;

#endif
