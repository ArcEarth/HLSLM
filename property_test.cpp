struct PropertyTest
{
	int m_k;

	const int& get_k() const { return m_k; }
	int& get_k() { return m_k; }

	void put_k(int value) { m_k = value; }
	void put_k(int* value) { m_k = reinterpret_cast<int>(value); }

	__declspec(property(get = get_k, put = put_k)) int k;
};

int __cdecl main( int argc, char *argv[] )
{
	PropertyTest p;
	auto& s = p.k;

	const auto& cp = p;

	auto& cs = p.k;

	s = 5;
	p.k = 5;

	cs = 5;
	cp.k = 5;
}
