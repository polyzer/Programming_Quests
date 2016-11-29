#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <map>
#include <vector>

using namespace std;
typedef double(*func_type)(stack<double>&);



map<string, int> PriorityMap = { 
	{ "+", 45 },
	{ "-", 45 },
	{ "*", 50 },
	{ "/", 50 },
	{ "^", 60 },
	{ "||", 10},
	{ "&&", 20},
	{ ">>", 30},
	{ "<<", 40},
	{ "or", 10},
	{ "xor", 10},
	{ "and", 20},
	{ "div", 50},
	{ "mod", 50}
};

enum tokenType
{
	variable, operation, openBr, closeBr, function, numConst
};

struct token
{
	string name;
	tokenType tokType;
	token(string str, tokenType type)
	{
		name = str;
		tokType = type;
	}
	token(){}
};
/*разбивает выражение из потока на составляющие
	в &expr - записывается вычисляемое выражение;
	в map<string, double> &vars - записываются названия переменных и их значения;
*/
void getExpression(ifstream &in, string &expr, map<string, double>&vars)
{
	getline(in, expr);
	while (!in.eof())
	{
		string tmp = "";
		getline(in, tmp);
		if (tmp == "")
			break;
		int n = tmp.find('=');
		string var = tmp.substr(0, n);
		while (var.back() == ' ')
			var.pop_back();
		string val = tmp.substr(n + 1, tmp.size() - n);
		double k = atof(val.c_str());
		vars[var] = k;
	}
}
/*
	Проверяет, являеся ли данная подстрока разделителем
*/
string getIfIsDelimiter(string str, int pos)
{
	vector<string>delimeters_v_s = {" ", "(", ")", "+", "-", "*", "/", "^", "||", "&&", ">>", "<<", "or", "xor", "and", "div", "mod"};
	for (int i = 0; i < delimeters_v_s.size(); i++)
	{
		if (((str.substr(pos).length() - delimeters_v_s[i].length()) >= 0) && (str.substr(pos, delimeters_v_s[i].length()) == delimeters_v_s[i]))
		{
			return delimeters_v_s[i];
		}
	}
	return "";
}

bool isDelimiter(string str, unsigned int pos)
{
	const vector<string>delimeters_v_s = { " ", "(", ")", "+", "-", "*", "/", "^", "||", "&&", ">>", "<<", "or", "xor", "and", "div", "mod" };
	for (auto i = 0; i < delimeters_v_s.size(); i++)
	{
		if (((str.substr(pos).length() - delimeters_v_s[i].length()) >= 0) && (str.substr(pos, delimeters_v_s[i].length()) == delimeters_v_s[i]))
		{
			return true;
		}
	}
	return false;
}

vector <token> parseExpression(const string &expression)
{
	vector<token>res;
	string name, expr = expression;
	size_t i = 0;
	expr += ' ';
	while (i < expr.size())
	{
		name = getIfIsDelimiter(expr, i);
		if (name != "")
		{
			if (expr[i] == ' ')
			{
				i++;
				continue;
			}
			i += name.length();
		}
		else
			while (!isDelimiter(expr, i))
			{
				name += expr[i];
				i++;
			}
		res.push_back(token(name, variable));
	}
	for (size_t i = 0; i < res.size(); i++)
	{
		if (res[i].name == "(")
		{
			res[i].tokType = openBr;
			continue;
		}
		if (res[i].name == ")")
		{
			res[i].tokType = closeBr;
			continue;
		}
		if (isdigit(res[i].name[0]))
		{
			res[i].tokType = numConst;
			continue;
		}
		if (isalpha(res[i].name[0]) && !isDelimiter(res[i].name, 0))
		{
			if (i < res.size() && res[i+1].name == "(")
				res[i].tokType = function;
			continue;
		}
		res[i].tokType = operation;
	}
	return res;
}

// map<string, int>

int returnPrior(string op_name)
{
	if (op_name == "+")
		return 45;
	if (op_name == "-")
		return 45;
	if (op_name == "*")
		return 50;
	if (op_name == "/")
		return 50;
	if (op_name == "^")
		return 60;
	if (op_name == "||")
		return 10;
	if (op_name == "&&")
		return 20;
	if (op_name == ">>")
		return 30;
	if (op_name == "<<")
		return 30;
	if (op_name == "or")
		return 10;
	if (op_name == "xor")
		return 10;
	if (op_name == "and")
		return 20;
	if (op_name == "div")
		return 50;
	if (op_name == "mod")
		return 50;

	return 0;
}

vector <token> createPostfixExpression(vector<token> &tokens)
{
	vector <token> result;
	stack <token> tStack;

	for (size_t i = 0; i < tokens.size(); i++)
	{
		switch (tokens[i].tokType)
		{
		case variable:
		case numConst:
			result.push_back(tokens[i]);
			break;
		case openBr:
			tStack.push(tokens[i]);
			break;
		case closeBr:
			while (tStack.top().tokType != openBr)
			{
				result.push_back(tStack.top());
				tStack.pop();
			}
			tStack.pop();
			break;
		case operation:
			if (tStack.size())
			{
				while	(tStack.size() &&
						((tStack.top().tokType == operation && returnPrior(tokens[i].name) <= returnPrior(tStack.top().name))
						|| tStack.top().tokType == function))
				{
					result.push_back(tStack.top());
					tStack.pop();
				}
				tStack.push(tokens[i]);
				break;
			}
		case function:
			while (tStack.size() && tStack.top().tokType == function)
			{
				result.push_back(tStack.top());
				tStack.pop();
			}
			tStack.push(tokens[i]);
			break;
		}
	}
	while (tStack.size())
	{
		result.push_back(tStack.top());
		tStack.pop();
	}
	return result;
}


using dStack = stack < double > ;

double pop(dStack &s)
{
	double res = s.top();
	s.pop();
	return res;
}

map<string, double(*)(dStack&)> func;

double op_plus(dStack &s)
{
	return pop(s) + pop(s);
}

double op_minus(dStack &s)
{
	return -pop(s) + pop(s);
}

double op_mul(dStack &s)
{
	return pop(s) * pop(s);
}

double op_div(dStack &s)
{
	return (1 / pop(s))*pop(s);
}

double op_pow(dStack &s)
{
	double t = pop(s);
	return pow(pop(s), t);
}

double op_and(dStack &s)
{
	return (double)(pop(s) && pop(s));
}

double op_or(dStack &s)
{
	return (double)(pop(s) || pop(s));
}

double op_mod(dStack &s)
{
	return (1 % (int)pop(s))*pop(s);
}

double op_cos(dStack &s)
{
	return cos(pop(s));
}

double op_sin(dStack &s)
{
	return sin(pop(s));
}

double op_ln(dStack &s)
{
	return log(pop(s));
}

void initFunctionMap()
{
	func["+"] = op_plus;
	func["-"] = op_minus;
	func["*"] = op_mul;
	func["/"] = op_div;
	func["^"] = op_pow;
	func["div"] = op_div;
	func["mod"] = op_mod;
	func["and"] = op_and;
	func["or"] = op_or;
	func["mul"] = op_mul;
	func["cos"] = op_cos;
	func["sin"] = op_sin;
	func["ln"] = op_ln;
}

double calcExpr(vector<token> &expr, map<string, double> &vars)
{
	stack<double> S;
	initFunctionMap();
	for (int i = 0; i < expr.size(); i++)
	{
		switch (expr[i].tokType)
		{
		case numConst: S.push(stod(expr[i].name)); break;
		case variable:	S.push(vars[expr[i].name]); break;
		case operation:
		case function:
			S.push(func[expr[i].name](S));
		}
	}
	return S.top();
}

int main()
{
	map<string, double> vars;
	string expr;
	getExpression(ifstream("file.txt"), expr, vars);
	vector<token> toks = parseExpression(expr);
	cout <<"Expression: " << endl << expr << endl;
	cout << endl <<"Variables: " << endl;
	for (auto i = vars.begin(); i != vars.end(); i++)
		cout << i->first << " = " << i->second << endl;
	cout << endl << "Tokens: " << endl;
	for (size_t i = 0; i < toks.size(); i++)
	{
		cout << toks[i].name << " ";
		switch (toks[i].tokType)
		{
		case 0: cout << "variable" << endl; break;
		case 1: cout << "operation" << endl; break;
		case 2: cout << "openBr" << endl; break;
		case 3: cout << "closeBr" << endl; break;
		case 4: cout << "function" << endl; break;
		case 5: cout << "numConst" << endl; break;
		}
	}
	vector<token> postfixExpr = createPostfixExpression(toks);
	cout << endl << "Postfix expression: " << endl;
	for (size_t i = 0; i < postfixExpr.size(); i++)
		cout << postfixExpr[i].name << " ";
	cout << endl;

	cout << calcExpr(postfixExpr, vars) << endl;

	double var1 = 1;
	double x = 10;
	double y = 50.5;
	double z = 1;
	cout << (var1 + (pow(cos(x + y && z*x),2) - log(var1*x / (y - x - z))) / 2.15) << endl;

	system("pause");
	return 0;
}
