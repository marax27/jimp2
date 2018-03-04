#include <math.h>
#include <stdio.h>

typedef struct{
	double x, y, z;
} Vector3;

// Dlugosc wektora.
double length(const Vector3 *a){
	return sqrt(a->x*a->x + a->y*a->y + a->z*a->z);
}

// Dodawanie wektorow.
Vector3 add(const Vector3 *a, const Vector3 *b){
	Vector3 result = {
		a->x + b->x,
		a->y + b->y,
		a->z + b->z
	};
	return result;
}

// Mnozenie wektora przez skalar.
Vector3 multiply(const Vector3 *a, double n){
	Vector3 result = {
		a->x * n,
		a->y * n,
		a->z * n
	};
	return result;
}

// Iloczyn wektorowy.
// https://en.wikipedia.org/wiki/Cross_product#Lagrange's_identity
Vector3 cross(const Vector3 *a, const Vector3 *b){
	Vector3 result = {
		a->y*b->z - a->z*b->y,
		a->z*b->x - a->x*b->z,
		a->x*b->y - a->y*b->x
	};
	return result;
}

// Iloczyn skalarny.
// https://en.wikipedia.org/wiki/Dot_product#Algebraic_definition
double dot(const Vector3 *a, const Vector3 *b){
	return a->x*b->x + a->y*b->y + a->z*b->z;
}

// Normalizacja: zwraca znormalizowany wektor.
Vector3 normalized(const Vector3 *a){
	double len = length(a);
	Vector3 result = {
		a->x / len,
		a->y / len,
		a->z / len
	};
	return result;
}

// Normalizacja: normalizuje podany wektor.
void normalize(Vector3 *a){
	*a = normalized(a);
}

// Porownanie wektorow. Ze wzgledu na niedoskonalosci
// operacji zmiennoprzecinkowych, areEqual() zignoruje
// bledy mniejsze niz EPSILON, chyba za flaga
// EXACT_COMPARE jest ustawiona.

//#define EXACT_COMPARE
#define EPSILON 10e-9
int areEqual(Vector3 *a, Vector3 *b){
	#ifdef EXACT_COMPARE
		return a->x == b->x &&
		       a->y == b->y &&
		       a->z == b->z;
	#else
		return fabs(a->x - b->x) < EPSILON &&
		       fabs(a->y - b->y) < EPSILON &&
			   fabs(a->z - b->z) < EPSILON;
	#endif
}

// Wypisanie wektora do bufora znakowego.
void VectorToString(char *buf, const Vector3 *a){
	sprintf(buf, "[%f, %f, %f]", a->x, a->y, a->z);
}

int main(void){
	Vector3 a = {3.0, 2.0, 0.0},
	        b = {0.0, -2.5, 4.5};

	char buffer[128];
	Vector3 c;
	
	c = add(&a, &b);
	VectorToString(buffer, &c);
	printf("a + b = %s\n", buffer);

	c = multiply(&a, -2.0);
	VectorToString(buffer, &c);
	printf("-2a = %s\n", buffer);

	c = cross(&a, &b);
	VectorToString(buffer, &c);
	printf("a x b = %s\n", buffer);

	printf("a dot b = %f\n", dot(&a, &b));

	c = normalized(&a);
	VectorToString(buffer, &c);
	printf("Normalized a: %s\n", buffer);

	Vector3 u = {-1, tan(0.2), sqrt(3)};
	Vector3 v = {-2, 2*sin(0.2)/cos(0.2), sqrt(12)};  //v == 2*u
	normalize(&u);
	normalize(&v);
	// Wynik zalezny od flagi EXACT_COMPARE.
	printf("Wektory u i v %ssa rowne.\n", areEqual(&u, &v) ? "" : "nie ");
}