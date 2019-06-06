// Matrix.h : Matrix class headers.
//	 C++ implementation of Python code.
//   From Udacity CS 373 Artificial Intelliegence for Robots.
//   Taught by Sebastian Thrun.
//   C++ version by Tyler Folsom; March 6, 2012.

#define TOLERANCE (1.0e-5)
//#define ARDUINO 1
#define MAX_MATRIX_SIZE 36
#define REAL double

void Show(char* x);
void Show(REAL x);

class matrix
{
 // implements basic operations of a matrix class
private:
#ifdef ARDUINO
        REAL value[MAX_MATRIX_SIZE];
#else
	REAL* value;
#endif
	int rows;  // dimx in Python
	int columns; // dimy
	int error;
	matrix __add__(matrix& other);
	matrix __sub__(matrix& other);
	matrix __mul__(matrix& other);
	bool   equal(matrix& other, REAL tolerance=TOLERANCE);
	REAL* operator[](int i);
	REAL element(int i, int k);
public:
	matrix(int Rows, int Columns); // zero constructor
	matrix(int dim); // identity constructor
	matrix(int Rows, int Columns, REAL* values); // contruct from list of values
//	matrix(matrix& value); // constructor from a matrix
	~matrix()
	{
#ifndef ARDUINO
		delete[] value;
#endif

	}
	void show();
	matrix inverse();
	matrix operator+(matrix& other);
	matrix operator-(matrix& other);
	matrix operator*(matrix& other);
	matrix operator/(matrix& other);
	void   operator=(matrix& other);
	bool   operator==(matrix& other);
	bool   operator!=(matrix& other);
	matrix transpose();
	bool  symmetric(REAL tolerance=TOLERANCE);
	matrix Cholesky(REAL ztol=TOLERANCE);
	matrix CholeskyInverse();
        void values(REAL* dest);
};

