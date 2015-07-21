struct Monomial
{
	double factor;
	int degree;
};
struct Polynomial{
	struct Monomial *monomials;
	char *name;
};

