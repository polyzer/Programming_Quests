struct Monomial
{
	double factor;
	unsigned int degree;
};

struct Polynomial
{
	struct Monomial *monomials;
	char *name;
	unsigned int monomialsCount;
};

struct PolynomialsArray
{
	struct Polynomial **Polynomials;
	unsigned int count;
};

struct Polynomial *cleapUp(struct Polynomial **poly);
void sortToBiggerPolynomial(struct Polynomial **poly);
void sortToLessPolynomial(struct Polynomial **poly);
int findMaxDegree(struct Polynomial *poly);
struct Polynomial *GCD(struct Polynomial *poly1, struct Polynomial *poly2);
struct Polynomial *getModPolynomials(struct Polynomial *poly1, struct Polynomial *poly2);
struct Polynomial *getDivPolynomials(struct Polynomial *poly1, struct Polynomial *poly2);
struct Polynomial *mulPolyMono(struct Polynomial *poly1, struct Monomial *mono1);
struct Polynomial *mulPolynomials(struct Polynomial *poly1, struct Polynomial *poly2);
struct Polynomial *subPolynomials(struct Polynomial *poly1, struct Polynomial *poly2);
struct Polynomial *addPolynomials(struct Polynomial *poly1, struct Polynomial *poly2);
struct Polynomial **findPolynomialByName(struct PolynomialsArray *PolyArr, const char *name);
void deletePolyArray (struct PolynomialsArray *PolyArray);
void appendPolynomialToArray(struct PolynomialsArray *PolyArray, struct Polynomial **polynomial);
int identificationVariable(char *expression);
void printPolynomial(struct Polynomial* poly);
struct Polynomial *getPolynomialFromString(char *strPoly);
char **parseAction(const char *action);
struct Polynomial *justDoIt(struct Polynomial *poly1, unsigned short int operation, struct Polynomial *poly2);