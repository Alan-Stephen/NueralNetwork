#include <cmath>
#include <fstream>
#include "../include/matrix.h"
#include "../include/MNISTLabel.h"

Matrix::Matrix(int rows, int columns) {
	data.reserve(rows * columns);
	this->numRows = rows; // numRows
	this->numCols = columns; // numCols
	for (int i = 0; i < rows * columns; i++){
		data.emplace_back(0);
	}
}

void Matrix::printMatrix() const {
    for(int row = 0; row < this->numRows; row++){
        for(int cols = 0; cols < this->numCols; cols++){
            std::cout << getElement(cols,row) << " ";
        }
        std::cout << "\n";
    }
}

double Matrix::getRawElement(int i) const
{
	return data[i];
}

void Matrix::setRawElement(const int i,const double value)
{
	data[i] = value;
}

int Matrix::coordsToRaw(const int x,const int y,const int coll)
{
	return (((y - 1) * coll) + x) - 1;
}

double Matrix::getElement(int cols, int rows) const {
    int maxCols = this->numCols;
    if(isTransposed){
        std::swap(cols,rows);
        maxCols = this->numRows;
    }

	double value = getRawElement(rows * maxCols + cols);

    return value;
}

inline void Matrix::setElement(int cols, int rows, double value) {
    int maxCols = this->numCols;
    if(isTransposed){
        std::swap(cols,rows);
        maxCols = this->numRows;
    }

	setRawElement(rows * maxCols + cols, value);
}

Matrix::Matrix(const Matrix& other){
	this->numRows = other.numRows;
	this->numCols = other.numCols;
	this->data = std::vector<double>();
    data.reserve(other.numRows * other.numCols);
	for(double value: other.data){
        this->data.emplace_back(value);
	}	
}

Matrix &Matrix::operator=(const Matrix &matrix) {
    this->numRows = matrix.numRows;
    this->numCols = matrix.numCols;

    this->data = std::vector<double>();
    data.reserve(matrix.numRows * matrix.numCols);
    for(double value: matrix.data){
        data.emplace_back(value);
    }
}

void Matrix::add(const Matrix &a) {
    if(not isSameDimensions(*this,a)){
        std::cout << "[ERROR] wrong matrix dimensions for inline add Matrixes\n";
        exit(1);
    }

    for(int i = 0; i < a.numCols * a.numRows; i++){
        double value = a.getRawElement(i) + this->getRawElement(i);
        this->setRawElement(i,value);
    }
}
void Matrix::minus(const Matrix &a) {
    if(not isSameDimensions(*this,a)){
        std::cout << "[ERROR] wrong matrix dimensions for inline minus Matrixes\n";
        exit(1);
    }

    for(int i = 0; i < a.numCols * a.numRows; i++){
        double value =this->getRawElement(i) - a.getRawElement(i);
        this->setRawElement(i,value);
    }
}

void Matrix::transpose() {
    std::swap(this->numCols, this->numRows);
    isTransposed = ! isTransposed;
}

size_t Matrix::size() const {
    return data.size();
}

void copyMatrix(const Matrix &from, Matrix &to) {
    to.numRows = from.numRows;
    to.numCols = from.numCols;

    to.data =  std::vector<double>();
    to.data.reserve(from.numRows * from.numCols);
    for(double values: from.data) {
        to.data.emplace_back(values);
    }
}

void addMatrix(const Matrix &a, const Matrix &b, Matrix &out) {
    if (!isSameDimensions(a,b)) {
        std::cout << "[ERROR] cannot add matrix invalid shapes";
        exit(1);
    }

    for(int i = 0; i < a.size(); i++){
        double value = a.getRawElement(i) + b.getRawElement(i);
        out.setRawElement(i,value);
    }
}

 Matrix multiplyMatrix(const Matrix &a, const Matrix &b){
    if(a.numCols != b.numRows){
        std::cout << "ERROR: invalid size of matrix for multiplication a :" << a.numRows << " " << a.numCols <<
                  " b: " << b.numRows << " " << b.numCols << "\n";
        std::exit(1);
    }

    Matrix out = Matrix(a.numRows, b.numCols);
    for(int row = 0; row < a.numRows; row++){
        for(int col = 0; col < b.numCols; col++){
            double sum = 0;
            for(int k = 0; k < a.numCols; k++){
                sum += a.getElement(k,row) * b.getElement(col,k);
            }
            out.setElement(col,row,sum);
        }
    }

    return out;
}

/**
 * Sets values in matrix to a random number bewteen 0 < x < 1
 * Matrix must be initlaised otherwise seg fault will be thrown.
 * */
void randomizeMatrix(Matrix &a){
    for(double &value: a.data){
        value = ((double) rand() / RAND_MAX);
    }
}

/**
 * Matrix minus in the form a - b;
 *
 * */
Matrix minusMatrix(const Matrix &a, const Matrix &b){
    if(a.numRows != b.numRows || a.numCols != b.numCols){
        std::cout << "invalid shapes for minus matrix\n";
        exit(1);
    }

    Matrix result = Matrix(a.numRows, a.numCols);
    for(int i = 0; i < a.size(); i++){
        result.setRawElement(i,a.getRawElement(i) - b.getRawElement(i));
    }
    return result;
}

Matrix multiplyMatrix(const double scalar, const Matrix &a) {
   Matrix matrix = Matrix(a.numRows, a.numCols);
   for(int i = 0; i < a.size(); i++){
       matrix.setRawElement(i,a.getRawElement(i) * scalar);
   }
   return matrix;
}

Matrix addMatrix(const Matrix &a, const Matrix &b){
    if(a.numCols != b.numCols || a.numRows != b.numRows){
        std::cout << "INVALID SIZES FOR addMatrix()";
        exit(1);
    }

    Matrix output = Matrix(a.numRows,a.numCols);
    for(int i = 0; i < a.size(); i++){
        double value = a.getRawElement(i) + b.getRawElement(i);
        output.setRawElement(i,value);
    }
    return output;
}

Matrix hadamardProduct(const Matrix &a, const Matrix &b){
    if(a.numCols != b.numCols || a.numRows != b.numRows){
        std::cout << "INVALID SIZES FOR hadamardProduct()";
        exit(1);
    }

    Matrix output = Matrix(a.numRows,a.numCols);
    for(int i = 0; i < a.numCols * a.numRows; i++){
        double value = a.getRawElement(i) * b.getRawElement(i);
        output.setRawElement(i,value);
    }
    return output;
}

Matrix mseLoss(const Matrix &pred, const Matrix &actual){
    if(pred.numCols != actual.numCols || pred.numRows != actual.numRows){
        std::cout << "INVALID SIZES FOR mseLoss()";
        exit(1);
    }

    Matrix temp = Matrix(pred.numRows,pred.numCols);
    for(size_t i = 0; i < pred.size(); i++) {
        double value = pow(pred.getRawElement(i) - actual.getRawElement(i),2.0F);
        temp.setRawElement(i,value);
    }
    return temp;
}

bool isSameDimensions(const Matrix &a, const Matrix &b) {
    if(a.numCols != b.numCols || a.numRows != b.numRows){
        return false;
    }

    return true;
}

Matrix mseLossDerivitive(const Matrix &pred, const Matrix &actual) {
    return multiplyMatrix(2, minusMatrix(pred,actual));
}

/**
 * not finished yet
 * */
Matrix dot(const Matrix &a, const Matrix &b) {
    // todo : finish this
    if(a.numCols != 1 || b.numCols != 1){
        std::cout << "[ERROR] a or b are not proper vectors for dot product";
        exit(1);
    }
    if(a.numRows != b.numRows){
        std::cout << "[ERROR] size of a and b not same for dot product";
        exit(1);
    }
}