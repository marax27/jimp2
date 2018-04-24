class Base{
	int x;
protected:
	int y;
public:
	int z;

	Base(int _x, int _y, int _z)
		: x(_x), y(_y), z(_z) {}
};

class A : protected Base {};
class B : private Base {};

int main(){}