#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() : rows_(0), cols_(0), matrix_(nullptr) {}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows <= 0 || cols <= 0) {
    throw std::out_of_range("Invalid matrix size");
  }
  matrix_ = new double* [rows_] {};
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_]{};
  }
}

S21Matrix::S21Matrix(const S21Matrix& other) { CopyMatrix(other); }

S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.cols_ = 0;
  other.rows_ = 0;
  other.matrix_ = nullptr;
}

S21Matrix::~S21Matrix() { DeleteMatrix(); }

bool S21Matrix::EqMatrix(const S21Matrix& other) const noexcept {
  bool res = true;
  if (!SizeEq(other)) {
    res = false;
  }
  for (int i = 0; (i < rows_) && (res); i++) {
    for (int j = 0; (j < cols_) && (res); j++) {
      if (std::fabs(matrix_[i][j] - other.matrix_[i][j]) > 1e-7) res = false;
    }
  }
  return res;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (!SizeEq(other)) {
    throw std::invalid_argument("Matrix sizes are not equal");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (!SizeEq(other)) {
    throw std::invalid_argument("Matrix sizes are not equal");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) noexcept {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument(
        "The number of columns of the first matrix is not equal to the number "
        "of rows of the second matrix");
  }
  S21Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      for (int k = 0; k < cols_; k++) {
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = result;
}

S21Matrix S21Matrix::Transpose() const noexcept {
  S21Matrix res(cols_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      res.matrix_[j][i] = matrix_[i][j];
    }
  }
  return res;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (cols_ != rows_) {
    throw std::invalid_argument("Rows != cols");
  }
  S21Matrix res(rows_, cols_), tmp;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      tmp = MakeMinor(i, j);
      res.matrix_[i][j] = tmp.Determinant() * pow(-1.0, (double)(i + j));
    }
  }
  return res;
}

double S21Matrix::Determinant() const {
  if (cols_ != rows_) {
    throw std::invalid_argument("Rows != cols");
  }
  double res = 0.0;
  S21Matrix tmp;
  if (rows_ == 1) {
    res = matrix_[0][0];
  }
  if (rows_ == 2) {
    res = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  }
  if (rows_ >= 3) {
    for (int i = 0; i < rows_; i++) {
      tmp = MakeMinor(i, 0);
      res += tmp.Determinant() * pow(-1.0, (double)(i)) * matrix_[i][0];
    }
  }
  return res;
}

S21Matrix S21Matrix::InverseMatrix() const {
  double det = Determinant();
  if (std::fabs(det) < 1e-7) {
    throw std::invalid_argument("Zero Determinant");
  }
  S21Matrix res(rows_, cols_);
  if (rows_ == 1) {
    res.matrix_[0][0] = 1. / matrix_[0][0];
  } else {
    res = CalcComplements();
    res.MulNumber(1. / det);
    res = res.Transpose();
  }
  return res;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SumMatrix(other);
  return tmp;
}
S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SubMatrix(other);
  return tmp;
}
S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.MulMatrix(other);
  return tmp;
}

S21Matrix operator*(const double num, const S21Matrix& other) {
  S21Matrix temp(other);
  temp.MulNumber(num);
  return temp;
}

S21Matrix operator*(const S21Matrix& other, const double num) {
  S21Matrix temp(other);
  temp.MulNumber(num);
  return temp;
}
bool S21Matrix::operator==(const S21Matrix& other) const noexcept {
  return EqMatrix(other);
}
S21Matrix& S21Matrix::operator=(const S21Matrix& other) noexcept {
  DeleteMatrix();
  CopyMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}
S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}
S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}
S21Matrix& S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}
double& S21Matrix::operator()(int i, int j) {
  if (i >= rows_ || j >= cols_ || i < 0 || j < 0) {
    throw std::out_of_range("Incorrect index!");
  }
  return matrix_[i][j];
}

int S21Matrix::GetRows() const noexcept { return rows_; }

int S21Matrix::GetCols() const noexcept { return cols_; }

void S21Matrix::SetRows(int rows) {
  if (rows <= 0) {
    throw std::out_of_range("The number of rows have to be > 0");
  }
  S21Matrix res(rows, cols_);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols_; j++) {
      res.matrix_[i][j] = (i >= rows_) ? 0 : matrix_[i][j];
    }
  }
  *this = res;
}

void S21Matrix::SetCols(int cols) {
  if (cols <= 0) {
    throw std::out_of_range("The number of cols have to be > 0");
  }
  S21Matrix res(rows_, cols);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols; j++) {
      res.matrix_[i][j] = (j >= cols_) ? 0 : matrix_[i][j];
    }
  }
  *this = res;
}

void S21Matrix::CopyMatrix(const S21Matrix& other) noexcept {
  cols_ = other.cols_;
  rows_ = other.rows_;
  CreateMatrix();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

void S21Matrix::CreateMatrix() noexcept {
  matrix_ = new double*[rows_]();
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_]();
  }
}

void S21Matrix::DeleteMatrix() noexcept {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }
}

bool S21Matrix::SizeEq(const S21Matrix& other) const noexcept {
  return cols_ == other.cols_ && rows_ == other.rows_;
}

S21Matrix S21Matrix::MakeMinor(int row, int col) const noexcept {
  S21Matrix res(rows_ - 1, cols_ - 1);
  int m = 0, n = 0;
  for (int i = 0; i < rows_; i++) {
    if (i == row) continue;
    for (int j = 0; j < cols_; j++) {
      if (j == col) continue;
      res.matrix_[n][m] = matrix_[i][j];
      m++;
    }
    m = 0;
    n++;
  }
  return res;
}