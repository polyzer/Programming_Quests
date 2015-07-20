#ifdef INT
typedef int OperationType;
#define ADD_FUNCTION AddInt
#endif   // INT

#ifdef DOUBLE
typedef double OperationType;
#define ADD_FUNCTION AddDouble
#endif  // DOUBLE

OperationType ADD_FUNCTION (OperationType a, OperationType b) {
	
    return a + b;
}

OperationType MUL_FUNCTION (OperationType a, OperationType b) {
	
	return a * b;
}

OperationType DIV_FUNCTION (OperationType a, OperationType b) {
	
	return a / b;
}

OperationType MOD_FUNCTION (OperationType a, OperationType b) {

	return a % b;
}

