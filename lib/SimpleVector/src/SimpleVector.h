#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

template <class T> class SimpleVector {
  public:
    SimpleVector(int row):row(row), col(1)
    {
      sVector = new T*[row];
      for(int i= 0; i<row; i++)
      {
        sVector[i] = new T[col];
      }
    }

    SimpleVector(int row, int col):row(row), col(col)
    {
      sVector = new T*[row];
      for(int i= 0; i<row; i++)
      {
        sVector[i] = new T[col];
      }
    }

    ~SimpleVector()
    {
      for(int i= 0; i<row; i++)
      {
        delete[] sVector[i];
      }
      delete[] sVector;
    }

    T* get(int row)
    {
      return sVector[row];
    }

    T* get(int row, int col)
    {
      return &sVector[row][col];
    }

    T* set(int row, int col, T value)
    {
      sVector[row][col] = value;
      return &sVector[row][col];
    }

    int rows()
    {
      return row;
    }

    int cols()
    {
      return col;
    }

    T* operator [](int row)
    {
      return sVector[row];
    }

  private:
    int row, col;
    T** sVector;

};
#endif
