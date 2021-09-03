#include"p0_starter.h"
#include<iostream>
using namespace scudb;
using namespace std;

template<typename T>
Matrix<T>::Matrix(int rows, int cols){
	rows_ = rows;
	cols_ = cols;
	linear_ = new T[rows * cols];
}

template<typename T>
int Matrix<T>::GetRowCount()const {
	return rows_;
}

template<typename T>
int Matrix<T>::GetColumnCount()const {
	return cols_;
}

template<typename T>
T Matrix<T>::GetElement(int i, int j) const {

	int rows = GetRowCount();
	int cols = GetColumnCount();

	if (i > rows || j > cols) {
		cout << "OUT_OF_RANGE " << endl;
	}
	else {
		return linear_[i * cols + j];
	}
	
}

template<typename T>
void Matrix<T>::SetElement(int i, int j, T val) {

	int rows = GetRowCount();
	int cols = GetColumnCount();

	if (i > rows || j > cols) {
		cout << "OUT_OF_RANGE " << endl;
		return;
	}
	linear_[i * cols + j] = val;

}

template<typename T>
void Matrix<T>::FillFrom(const std::vector<T>& source) {

	int rows = GetRowCount();
	int cols = GetColumnCount();
	if (int(source.size()) != sizeof(linear_) / sizeof(linear_[0])) {
		cout << "OUT_OF_RANGE " << endl;
		return;
	}
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			linear_[i * cols + j] = source[i * cols + j];
		}
	}

}
template<typename T>
Matrix<T>::~Matrix() {
	rows_ = NULL;
	cols_ = NULL;
	linear_ = NULL;
}

template<typename T>
RowMatrix<T>::RowMatrix(int rows, int cols):Matrix<T>(rows, cols) {
	this->rows_ = rows;
	this->cols_ = cols;

	data_ = new T * [rows];
	for (int i = 0; i < rows; i++) {
		data_[i] = new T[cols];
	}
}

template<typename T>
int  RowMatrix<T>::GetRowCount()const  {

	return this->rows_;
}

template<typename T>
int RowMatrix<T>::GetColumnCount()const {

	return this->cols_;
}

template<typename T>
T RowMatrix<T>::GetElement(int i, int j) const {

	int rows = GetRowCount();
	int cols = GetColumnCount();

	if (i > rows || j > cols) {
		cout << "OUT_OF_RANGE " << endl;
	}
	else {
		return data_[i][j];
	}

}
template<typename T>
void RowMatrix<T>::SetElement(int i, int j, T val) {

	int rows = GetRowCount();
	int cols = GetColumnCount();

	if (i > rows || j > cols) {
		cout << "OUT_OF_RANGE " << endl;
		return;
	}

	data_[i][j] = val; 
}

template<typename T>
void RowMatrix<T>::FillFrom(const std::vector<T>& source) {

	int rows = GetRowCount();
	int cols = GetColumnCount();
	
	if (int(source.size()) !=  rows*cols ) {

		cout << "OUT_OF_RANGE " << endl;
		return;
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			data_[i][j] = source[i * cols + j];
		}
	}

}

template<typename T>
RowMatrix<T>::~RowMatrix() {
	
	data_ = NULL;
}

template <typename T>
unique_ptr<RowMatrix<T>> RowMatrixOperations<T>::Add(const RowMatrix<T>* matrixA, const RowMatrix<T>* matrixB) {

	int a_row = matrixA->GetRowCount();
	int a_col = matrixA->GetColumnCount();

	int b_row = matrixB->GetRowCount();
	int b_col = matrixB->GetColumnCount();



	if (a_row != b_row || a_col != b_col) return unique_ptr<RowMatrix<T>>(nullptr);
	
	
	RowMatrix<T>* matrixC;
	matrixC = new RowMatrix<T>(a_row, b_col);
	for (int i = 0; i < a_row; i++) {
		for (int j = 0; j < b_col; j++) {
			matrixC->SetElement(i,j, matrixA->GetElement(i, j) + matrixB->GetElement(i, j)) ;
			
		}
	}
	
	unique_ptr<RowMatrix<T>> takePtr(matrixC);

	return takePtr;
}

template <typename T>
unique_ptr<RowMatrix<T>> RowMatrixOperations<T>::Multiply(const RowMatrix<T>* matrixA, const RowMatrix<T>* matrixB) {
	int a_row = matrixA->GetRowCount();
	int a_col = matrixA->GetColumnCount();

	int b_row = matrixB->GetRowCount();
	int b_col = matrixB->GetColumnCount();


	
	if (a_col != b_row) return unique_ptr<RowMatrix<T>>(nullptr);


	RowMatrix<T>* matrixC;
	matrixC = new RowMatrix<T>(a_row, b_col);
	for (int i = 0; i < a_row; i++) {
		for (int j = 0; j < b_col; j++) {
			int val = 0;
			
			for (int a = 0; a < a_col; a++) {
				val += (matrixA->GetElement(i, a) * matrixB->GetElement(a, j));
			}
			matrixC->SetElement(i, j, val);

		}
	}

	
	unique_ptr<RowMatrix<T>> takePtr(matrixC);

	return takePtr;
}
template <typename T>
unique_ptr<RowMatrix<T>> RowMatrixOperations<T>::GEMM(const RowMatrix<T>* matrixA, const RowMatrix<T>* matrixB,
	const RowMatrix<T>* matrixC) {
	int a_row = matrixA->GetRowCount();
	int a_col = matrixA->GetColumnCount();

	int b_row = matrixB->GetRowCount();
	int b_col = matrixB->GetColumnCount();

	int c_row = matrixC->GetRowCount();
	int c_col = matrixC->GetColumnCount();



	if (a_col != b_row || a_row != c_row || b_col!=c_col) return unique_ptr<RowMatrix<T>>(nullptr);

	// *
	RowMatrix<T>* matrixD;
	matrixD = new RowMatrix<T>(a_row, b_col);
	for (int i = 0; i < a_row; i++) {
		for (int j = 0; j < b_col; j++) {
			int val = 0;

			for (int a = 0; a < a_col; a++) {
				val += (matrixA->GetElement(i, a) * matrixB->GetElement(a, j));
			}
			matrixD->SetElement(i, j, val);

		}
	}

	// +
	RowMatrix<T>* matrixE;
	matrixE = new RowMatrix<T>(a_row, b_col);
	for (int i = 0; i < a_row; i++) {
		for (int j = 0; j < b_col; j++) {
			matrixE->SetElement(i, j, matrixD->GetElement(i, j) + matrixC->GetElement(i, j));

		}
	}

	unique_ptr<RowMatrix<T>> takePtr(matrixE);

	return takePtr;
}

int main() {

	RowMatrix<int>* A = new RowMatrix<int>(3, 4);
	RowMatrix<int>* B = new RowMatrix<int>(3, 4);
	RowMatrix<int>* C = new RowMatrix<int>(4, 3);
	RowMatrix<int>* D = new RowMatrix<int>(3, 3);
	vector<int> a,b,c,d;
	//装填3个矩阵
	for (int i = 0; i < 12; i++) {
		a.push_back(i);
	}
	for (int i = 0; i < 12; i++) {
		b.push_back(1);
	}
	for (int i = 0; i < 12; i++) {
		c.push_back(2);
	}
	for (int i = 0; i < 9; i++) {
		d.push_back(3);
	}
	A->FillFrom(a);
	B->FillFrom(b);
	C->FillFrom(c);
	D->FillFrom(d);
	//打印三个矩阵
	cout << "A is :" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			cout << A->GetElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << "B is :" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			cout << B->GetElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << "C is :" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			cout << C->GetElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << "D is :" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << D->GetElement(i, j) << " ";
		}
		cout << endl;
	}

	
	auto E = RowMatrixOperations<int>::Add(A, B);
	cout << "A + B is :" << endl;
	
	for (int i = 0; i < A->GetRowCount(); i++) {
		for (int j = 0; j < A->GetColumnCount(); j++) {
			
			cout << E->GetElement(i, j) << " ";
		}
		cout << endl;
	}
	
	auto F = RowMatrixOperations<int>::Multiply(B, C);
	cout << "B * C is :" << endl;
	
	for (int i = 0; i < B->GetRowCount(); i++) {
		for (int j = 0; j < C->GetColumnCount(); j++) {

			cout << F->GetElement(i, j) << " ";
		}
		cout << endl;
	}
	auto G = RowMatrixOperations<int>::GEMM(B, C, D);
	cout << "B * C + D is :" << endl;

	for (int i = 0; i < B->GetRowCount(); i++) {
		for (int j = 0; j < C->GetColumnCount(); j++) {

			cout << G->GetElement(i, j) << " ";
		}
		cout << endl;
	}
	
	return 0;
}