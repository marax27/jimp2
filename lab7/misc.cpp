#include "misc.h"

// Rozne pomniejsze funkcje.

// Konwersja stopni na radiany.
fp_t deg2Rad(fp_t degrees){
	return degrees * M_PI / 180.0;
}

// Funkcja znaku.
bool sign(int x){
	return x ? (x > 0 ? 1 : -1) : 0;
}

// Sprawdzenie, czy czesc wycinka/luku ograniczonego
// katami alfa1, alfa2 lezy w danej cwiartce.
bool isInFirstQuarter(fp_t alfa1, fp_t){
	return alfa1 < 90.0f;
}
bool isInSecondQuarter(fp_t alfa1, fp_t alfa2){
	return alfa1 < 180.0f && (alfa1 >= 90.0f || alfa2 >= 90.0f);
}
bool isInThirdQuarter(fp_t alfa1, fp_t alfa2){
	return alfa1 < 270.0f && (alfa1 >= 180.0f || alfa2 >= 180.0f);
}
bool isInFourthQuarter(fp_t, fp_t alfa2){
	return alfa2 >= 270.0f;
}

int linearInterpolation(Point A, Point B, int y){
	//if(A.x != B.x)
	if(A.y != B.y)
		return B.x + (y-B.y)*(A.x-B.x)/(A.y-B.y);
	else
		return B.x;
}