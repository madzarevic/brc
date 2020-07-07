#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <QSharedDataPointer>
#include <QVector>
#include <QString>

using namespace std;

template <typename T>
class MatrixData : public QSharedData
{
public:
	MatrixData() : mRowSize(0), mColumnSize(0) {}
	MatrixData(int rowSize, int columnSize) : mRowSize(rowSize), mColumnSize(columnSize), mElements(rowSize * columnSize, 0) {}
	MatrixData(const MatrixData<T> &other) : QSharedData(other), mRowSize(other.mRowSize), mColumnSize(other.mColumnSize), mElements(other.mElements) {}
	~MatrixData() {}
	int rowSize() const { return mRowSize; }
	int columnSize() const { return mColumnSize; }
	const T element(int row, int column) const { return mElements.at(index(row, column)); }
	void setElement(int row, int column, const T& value) { mElements[index(row, column)] = value; }
	
private:
	int index(int row, int column) const { return row * this->columnSize() + column; }

private:
	int mRowSize;
	int mColumnSize;
	QVector<T> mElements;
};

template <typename T>
class Matrix
{
public:
	Matrix() { d = new MatrixData<T>(); }
	Matrix(int rowSize, int columnSize) { d = new MatrixData<T>(rowSize, columnSize); }
    Matrix(const Matrix<T> &other) : d(other.d) {}
	int rowSize() const { return d->rowSize(); }
	int columnSize() const { return d->columnSize(); }
	const T element(int row, int column) const { return d->element(row, column); }
	void setElement(int row, int column, const T& value) { d->setElement(row, column, value); }
	
	QString toString() const;

private:
	QSharedDataPointer< MatrixData<T> > d;
};

template <typename T>
QString Matrix<T>::toString() const
{
	Matrix<QString> leftStrings(rowSize(), columnSize());
	Matrix<QString> rightStrings(rowSize(), columnSize());
	QVector<int> leftWidths(columnSize(), 0);
	QVector<int> rightWidths(columnSize(), 0);
	
	for (int i = 0; i < rowSize(); i++)
	{
		for (int j = 0; j < columnSize(); j++)
		{
			QString string = QString::number(element(i, j));
			QString leftString;
			QString rightString;
			int decimalPoint = string.indexOf('.');
			if (decimalPoint >= 0)
			{
				leftString = string.left(decimalPoint);
				rightString = string.right(string.size() - decimalPoint);
			}
			else
				leftString = string;
			
			leftStrings.setElement(i, j, leftString);
			rightStrings.setElement(i, j, rightString);
			
			if (leftString.size() > leftWidths.at(j))
				leftWidths[j] = leftString.size();
			
			if (rightString.size() > rightWidths.at(j))
				rightWidths[j] = rightString.size();

		}
	}
	
	QString string = QString("[%1 x %2]\n").arg(rowSize()).arg(columnSize());	
	for (int i = 0; i < rowSize(); i++)
	{
		string.append("[ ");
		for (int j = 0; j < columnSize(); j++)
		{
			string.append(QString("%1%2").arg(leftStrings.element(i, j), leftWidths.at(j), ' ').arg(rightStrings.element(i, j), -rightWidths.at(j), ' '));
			string.append(" ");
		}
		string.append("]\n");
	}
	return string;
}

#endif // #ifndef _MATRIX_H_