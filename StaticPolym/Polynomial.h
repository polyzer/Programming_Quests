struct Monomial
{
	double factor;
	int degree;
};

struct Polynomial
{
	struct Monomial *monomials;
	char *name;
	int monomialsCount;
};

struct PolynomialsArray
{
	struct Polynomial **Polynomials;
	int count;
};
