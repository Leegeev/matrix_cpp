#ifndef CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_
#define CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_

#include <algorithm>
#include <cmath>
#include <stdexcept>

class S21Matrix {
 public:
  S21Matrix();
  explicit S21Matrix(const int rows, const int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  bool EqMatrix(const S21Matrix& other) const noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const noexcept;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix operator*(const S21Matrix& other) const;
  friend S21Matrix operator*(const double num, const S21Matrix& other);
  friend S21Matrix operator*(const S21Matrix& other, const double num);
  bool operator==(const S21Matrix& other) const noexcept;
  S21Matrix& operator=(const S21Matrix& other) noexcept;
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double num);
  double& operator()(int i, int j);

  int GetRows() const noexcept;
  int GetCols() const noexcept;
  void SetRows(int rows);
  void SetCols(int cols);

 private:
  void CopyMatrix(const S21Matrix& other) noexcept;
  void CreateMatrix() noexcept;
  void DeleteMatrix() noexcept;
  bool SizeEq(const S21Matrix& other) const noexcept;
  S21Matrix MakeMinor(int row, int col) const noexcept;

  int rows_, cols_;
  double** matrix_;
};
#endif  // CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_