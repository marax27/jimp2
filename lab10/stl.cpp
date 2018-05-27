#include <iostream>
#include <utility>

using std::cout;

//------------------------------

template<typename T1, typename T2>
struct pair{
	typedef T1 first_type;    // Sposob na przechowanie typow
	typedef T2 second_type;   // wewnatrz struktury.

	pair() : first(T1()), second(T2()) {}
	pair(const T1 &a, const T2 &b) : first(a), second(b) {}
	
	template<typename U1, typename U2>
	pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}

	T1 first;
	T2 second;
};

template<typename T1, typename T2>
bool operator==(const pair<T1, T2> &a, const pair<T1, T2> &b){
	return a.first == b.first && a.second == b.second;
}

template<typename T1, typename T2>
pair<T1, T2> make_pair(const T1 &a, const T2 &b){
	return pair<T1, T2>(a, b);
}

//------------------------------

// Jesli nie byloby const w zwracanym typie,
// nie moznaby okreslic argumentow jako const.
template<typename T> const T& min(const T &a, const T &b){
	return a<b ? a : b;
}

// STL jest tak przygotowany, ze preferuje '<' ponad '>'.
template<typename T> const T& max(const T &a, const T &b){
	return a<b ? b : a;
}

template<typename T> void swap(T &a, T &b){
	T tmp = a;
	a = b;
	b = tmp;
}

//------------------------------

int main(){
	// pair<int, char> ic(321, 'x');
	auto ic = make_pair(321, 'x');
	pair<float, int> fi(ic);

	struct{int dx, dy;} v{3, -1}, u{0, 7};
	auto vv = make_pair(v, u);
	std::cout << vv.first.dx << "/" << vv.second.dy << '\n';

	float q1(3.14159), q2(2.7182818);
	std::cout << "min: " << min(q1, q2) << '\n';
	std::cout << "max: " << max(q1, q2) << '\n';
	std::cout << "Watchout " << max<double>(1, 2.5) << '\n';
	swap(q1, q2);
	std::cout << "pi = " << q1 << ", e = " << q2 << '\n';
}
