#pragma once
#include <cmath>
#include <iostream>
#define A _matrix_values

/** A 4-dimensional homogenous vector*/
class Vector4D
{
public:

	//default construct
	Vector4D()
	{
		_vect_values[0] = 0;
		_vect_values[1] = 0;
		_vect_values[2] = 0;
		_vect_values[3] = 1;
	}
	
	Vector4D(const Vector4D &o)
	{
		_vect_values[0] = o[0];
		_vect_values[1] = o[1];
		_vect_values[2] = o[2];
		_vect_values[3] = o[3];
	}
	float GetX() { return _vect_values[0]; }
	float GetY() { return _vect_values[1]; }
	float GetZ() { return _vect_values[2]; }
	float GetW() { return _vect_values[3]; }
	/**
	* custom construct for a 4D-vector
	* @param x : x-value
	* @param y : y-value
	* @param z : z-value
	* @param w : w-value
	* @return : 4D-vector
	*/
	Vector4D(const float x, const float y, const float z, const float w) //custom construct
	{
		this->_vect_values[0] = x;
		this->_vect_values[1] = y;
		this->_vect_values[2] = z;
		this->_vect_values[3] = w;
	}

	Vector4D(float x, float y, float z)
	{
		this->_vect_values[0] = x;
		this->_vect_values[1] = y;
		this->_vect_values[2] = z;
		this->_vect_values[3] = 0;
	}

	Vector4D(float x, float y)
	{
		this->_vect_values[0] = x;
		this->_vect_values[1] = y;
		this->_vect_values[2] = 0;
		this->_vect_values[3] = 0;
	}


	/**
	* Performs a dotproduction of two vectors
	* @param vect4 : the 2nd vector used in the dotproduct
	* @return Returns a Scalar 
	*/
	float DotProduct(const Vector4D& vect2)
	{
		const float result =
			this->_vect_values[0] * vect2._vect_values[0] +
			this->_vect_values[1] * vect2._vect_values[1] +
			this->_vect_values[2] * vect2._vect_values[2];

		return result;
	}
	/**
	* Performs crossproduct with another vector
	* @param vect2
	* @param vect4 : the 2nd vector used in the crossproduct
	* @return Returns a new vector 
	*/
	Vector4D CrossProduct(const Vector4D& vect2)
	{
		Vector4D result;
		result._vect_values[0] = (this->_vect_values[1] * vect2._vect_values[2]) - (this->_vect_values[2] * vect2._vect_values[1]);
		result._vect_values[1] = -((this->_vect_values[0] * vect2._vect_values[2]) - (this->_vect_values[2] * vect2._vect_values[0]));
		result._vect_values[2] = (this->_vect_values[0] * vect2._vect_values[1]) - (this->_vect_values[1] * vect2._vect_values[0]);


		return result;
	}

	/**
	* Calculates the lenght of a vector
	* @return Returns the lenght of the vector
	*/
	float LenghtOf()
	{
		const float lenght = sqrt(pow(this->_vect_values[0], 2) + pow(this->_vect_values[1], 2) + pow(this->_vect_values[2], 2)); // square root of (x^2 + y^2)
		return lenght;
	}
	/**
	* Normalizes the vector
	* @return Returns a new normalized vector
	*/
	Vector4D Normalize()
	{
		Vector4D new_vect;
		new_vect._vect_values[0] = this->_vect_values[0] / this->LenghtOf();
		new_vect._vect_values[1] = this->_vect_values[1] / this->LenghtOf();
		new_vect._vect_values[2] = this->_vect_values[2] / this->LenghtOf();

		return new_vect;
	}

	//-----overloading operators-----

	/**
	* Allows Addition between two vectors
	*/
	Vector4D operator+(const Vector4D& other)
	{
		Vector4D new_vect;
		new_vect._vect_values[0] = this->_vect_values[0] + other._vect_values[0];
		new_vect._vect_values[1] = this->_vect_values[1] + other._vect_values[1];
		new_vect._vect_values[2] = this->_vect_values[2] + other._vect_values[2];

		return new_vect;
	}
	/**
	* Allows Subtraction between two vectors
	*/
	Vector4D operator-(const Vector4D& other)
	{
		Vector4D new_vect;
		new_vect._vect_values[0] = this->_vect_values[0] - other._vect_values[0];
		new_vect._vect_values[1] = this->_vect_values[1] - other._vect_values[1];
		new_vect._vect_values[2] = this->_vect_values[2] - other._vect_values[2];

		return new_vect;
	}
	/**
	* Allows multiplication with a vector and a number, Scalar
	*/
	Vector4D operator*(const float& other)
	{
		Vector4D new_vect;
		new_vect._vect_values[0] = this->_vect_values[0] * (other);
		new_vect._vect_values[1] = this->_vect_values[1] * (other);
		new_vect._vect_values[2] = this->_vect_values[2] * (other);

		return new_vect;
	}
	/**
	* Allows assigning a vector with another vector
	*/
	Vector4D& operator=(const Vector4D& other)
	{
		_vect_values[0] = other._vect_values[0];
		_vect_values[1] = other._vect_values[1];
		_vect_values[2] = other._vect_values[2];

		return *this;
	}
	/**
	* Compares a vector with another vector. Checks if the two vectors are the same.
	* @return True or False 
	*/
	bool operator==(const Vector4D& v)
	{
		return (this->_vect_values[0] == v._vect_values[0] &&
			this->_vect_values[1] == v._vect_values[1]) &&
			this->_vect_values[2] == v._vect_values[2];
	}

	/**
	* Compares a vector with another vector. Checks if the two vectors are not the same.
	* @return True or False
	*/
	bool operator!=(const Vector4D& v)
	{
		return (this->_vect_values[0] != v._vect_values[0] &&
			this->_vect_values[1] != v._vect_values[1] &&
			this->_vect_values[2] != v._vect_values[2]);
	}
	/**
	* Allows access to the vectors private data. Reading the data
	*/
	const float& operator[] (int index) const
	{
		if (index <= 3 && index >= 0)
		{
			return this->_vect_values[index];
		}
		else
		{
			throw "INDEX OUT OF BOUNDS";
		}
	}
	/**
	* Allows access to the vectors private data. Writing data to the array of data
	*/
	float& operator[] (int index)
	{
		if (index <= 3 && index >= 0)
		{
			return this->_vect_values[index];
		}
		else
		{
			throw "INDEX OUT OF BOUNDS";
		}
	}

private:
	float _vect_values[4]{};
};




/** A 4-dimensional homogenous matrix*/
class Matrix4D
{
public:
	/** Default construct, creates a 4x4 identity matrix*/
	Matrix4D()
	{
		this->_matrix_values[0] = 1;
		this->_matrix_values[1] = 0;
		this->_matrix_values[2] = 0;
		this->_matrix_values[3] = 0;

		this->_matrix_values[4] = 0;
		this->_matrix_values[5] = 1;
		this->_matrix_values[6] = 0;
		this->_matrix_values[7] = 0;

		this->_matrix_values[8] = 0;
		this->_matrix_values[9] = 0;
		this->_matrix_values[10] = 1;
		this->_matrix_values[11] = 0;

		this->_matrix_values[12] = 0;
		this->_matrix_values[13] = 0;
		this->_matrix_values[14] = 0;
		this->_matrix_values[15] = 1;
	}
	/**
	* Custom constructor for a 4x4 matrix
	*/
	Matrix4D(float x1, float y1, float z1, float w1,
		float x2, float y2, float z2, float w2,
		float x3, float y3, float z3, float w3,
		float x4, float y4, float z4, float w4)
		
	{
		_matrix_values[0] = x1;
		_matrix_values[4] = x2;
		_matrix_values[8] = x3;
		_matrix_values[12] = x4;

		_matrix_values[1] = y1;
		_matrix_values[5] = y2;
		_matrix_values[9] = y3;
		_matrix_values[13] = y4;

		_matrix_values[2] = z1;
		_matrix_values[6] = z2;
		_matrix_values[10] = z3;
		_matrix_values[14] = z4;

		_matrix_values[3] = w1;
		_matrix_values[7] = w2;
		_matrix_values[11] = w3;
		_matrix_values[15] = w4;


	}

	
	/** static member function that creates a Rotation Matrix for the X-axis
	* @param angle : the angle in radians
	* @return rotation matrix around the x axis
	*/
	static Matrix4D RotX_axis(float rad)
	{
		return Matrix4D(
			1, 0, 0, 0,
			0, cos(rad), -sin(rad), 0,
			0, sin(rad), cos(rad), 0,
			0, 0, 0, 1);
	}
	/** static member function that creates a Rotation Matrix for the X-axis
	* @param angle : the angle in radians
	* @return rotation matrix around the x axis
	*/
	static Matrix4D RotY_axis(float rad)
	{
		return Matrix4D(
			cos(rad), 0, -sin(rad), 0,
			0, 1, 0, 0,
			sin(rad), 0, cos(rad), 0,
			0, 0, 0, 1);
	}
	/** static member function that creates a Rotation Matrix for the Z-axis
	* @param angle : the angle in radians
	* @return rotation matrix around the Z-axis
	*/
	static Matrix4D RotZ_axis(float rad)
	{
		return Matrix4D(
			cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
	/** static member function that creates a Rotation Matrix for a vectors axis
	* @param angle : the angle in radians
	* @param vect : vector which axis is rotated around
	* @return rotation matrix around the given vector
	*/
	static Matrix4D RotVect_axis(Vector4D vect, float rad)
	{
		const float co = cos(rad);
		const float tcos = 1 - cos(rad);
		const float si = sin(rad);
		const float x = vect[0];
		const float y = vect[1];
		const float z = vect[2];


		return Matrix4D(
			co + x * x * tcos, x * y * tcos - z * si, x * z * tcos + y * si, 0,
			y * x * tcos + z * si, co + y * y * tcos, y * z * tcos - x * si, 0,
			z * x * tcos - y * si, x * y * tcos + x * si, co + z * z * tcos, 0,
			0, 0, 0, 0);
	}

	/** static member function that calculates the transpose of a given matrix
	* @param mat : matrix to transpose
	* @return Transposed matrix
	*/
	Matrix4D Transpose(Matrix4D mat)
	{
		mat[1] = mat[4];
		mat[2] = mat[8];
		mat[3] = mat[12];

		mat[4] = mat[1];
		mat[5] = mat[5];
		mat[6] = mat[6];
		mat[7] = mat[7];

		mat[8] = mat[2];
		mat[9] = mat[6];
		mat[10] = mat[10];
		mat[11] = mat[14];

		mat[12] = mat[3];
		mat[13] = mat[7];
		mat[14] = mat[11];
		mat[15] = mat[15];
		return mat;
	}

	/**
	* Static member function That calculates the inverse of the given matrix
	* @param matrix
	* @return inverse of the matrix
	*/
	static Matrix4D Inverse(Matrix4D mat)
	{
		// if the determinant is 0, there's no possible inverse
		const float det = mat.Det();
		if (det == 0)
			mat;


		// create 16 3x3-submatrices from the 4x4 matrix
		float submat1[9] = { mat.A[5], mat.A[6], mat.A[7],
							mat.A[9], mat.A[10], mat.A[11],
							mat.A[13], mat.A[14], mat.A[15] };
		float submat2[9] = { mat.A[4], mat.A[6], mat.A[7],
						mat.A[8], mat.A[10], mat.A[11],
						mat.A[12], mat.A[14], mat.A[15] };
		float submat3[9] = { mat.A[4], mat.A[5], mat.A[7],
						mat.A[8], mat.A[9], mat.A[11],
						mat.A[12], mat.A[13], mat.A[15] };
		float submat4[9] = { mat.A[4], mat.A[5], mat.A[6],
						mat.A[8], mat.A[9], mat.A[10],
						mat.A[12], mat.A[13], mat.A[14] };

		float submat5[9] = { mat.A[1], mat.A[2], mat.A[3],
						mat.A[9], mat.A[10], mat.A[11],
						mat.A[13], mat.A[14], mat.A[15] };
		float submat6[9] = { mat.A[0], mat.A[2], mat.A[3],
						mat.A[8], mat.A[10], mat.A[11],
						mat.A[12], mat.A[14], mat.A[15] };
		float submat7[9] = { mat.A[0], mat.A[1], mat.A[3],
						mat.A[8], mat.A[9], mat.A[11],
						mat.A[12], mat.A[13], mat.A[15] };
		float submat8[9] = { mat.A[0], mat.A[1], mat.A[2],
						mat.A[8], mat.A[9], mat.A[10],
						mat.A[12], mat.A[13], mat.A[14] };

		float submat9[9] = { mat.A[1], mat.A[2], mat.A[3],
						mat.A[5], mat.A[6], mat.A[7],
						mat.A[13], mat.A[14], mat.A[15] };
		float submat10[9] = { mat.A[0], mat.A[2], mat.A[3],
						mat.A[4], mat.A[6], mat.A[7],
						mat.A[12], mat.A[14], mat.A[15] };
		float submat11[9] = { mat.A[0], mat.A[1], mat.A[3],
						mat.A[4], mat.A[5], mat.A[7],
						mat.A[12], mat.A[13], mat.A[14] };
		float submat12[9] = { mat.A[0], mat.A[1], mat.A[2],
						mat.A[4], mat.A[5], mat.A[6],
						mat.A[12], mat.A[13], mat.A[14] };

		float submat13[9] = { mat.A[1], mat.A[2], mat.A[3],
						mat.A[5], mat.A[6], mat.A[7],
						mat.A[9], mat.A[10], mat.A[11] };
		float submat14[9] = { mat.A[0], mat.A[2], mat.A[3],
						mat.A[4], mat.A[6], mat.A[7],
						mat.A[8], mat.A[10], mat.A[11] };
		float submat15[9] = { mat.A[0], mat.A[1], mat.A[3],
						mat.A[4], mat.A[5], mat.A[7],
						mat.A[8], mat.A[9], mat.A[11] };
		float submat16[9] = { mat.A[0], mat.A[1], mat.A[2],
						mat.A[4], mat.A[5], mat.A[6],
						mat.A[8], mat.A[9], mat.A[10] };

		// define the fraction used in calculating the inverse
		const float detdiv = (1 / det);

		//calculating each cell. each cell's value is the determinant of it's 3x3-submatrix
		const float x1 = Matrix4D::det3x3(submat1);
		const float x2 = Matrix4D::det3x3(submat2);
		const float x3 = Matrix4D::det3x3(submat3);
		const float x4 = Matrix4D::det3x3(submat4);

		const float y1 = Matrix4D::det3x3(submat5);
		const float y2 = Matrix4D::det3x3(submat6);
		const float y3 = Matrix4D::det3x3(submat7);
		const float y4 = Matrix4D::det3x3(submat8);

		const float z1 = Matrix4D::det3x3(submat9);
		const float z2 = Matrix4D::det3x3(submat10);
		const float z3 = Matrix4D::det3x3(submat11);
		const float z4 = Matrix4D::det3x3(submat12);

		const float w1 = Matrix4D::det3x3(submat13);
		const float w2 = Matrix4D::det3x3(submat14);
		const float w3 = Matrix4D::det3x3(submat15);
		const float w4 = Matrix4D::det3x3(submat16);

		//construct the 4x4 matrix
		Matrix4D adj(x1, -y1, z1, -w1, -x2, y2, -z2, w2, x3, -y3, z3, -w3, -x4, y4, -z4, w4);
		// transpose the new 4x4-matrix
		adj.Transpose();
		//multiply the determinant fraction into each cell to complete the inversion. 
		adj = adj * detdiv;
		//adj will be the inverse of the origina 4x4 matrix
		return adj;
	}


	/**
	* Calculates the determinant of a 4x4 matrix
	* @return Determinant as a float
	*/
	float Det()
	{
		return
			A[3] * A[6] * A[9] * A[12] - A[2] * A[7] * A[9] * A[12] -
			A[3] * A[5] * A[10] * A[12] + A[1] * A[7] * A[10] * A[12] +
			A[2] * A[5] * A[11] * A[12] - A[1] * A[6] * A[11] * A[12] -
			A[3] * A[6] * A[8] * A[13] + A[2] * A[7] * A[8] * A[13] +
			A[3] * A[4] * A[10] * A[13] - A[0] * A[7] * A[10] * A[13] -
			A[2] * A[4] * A[11] * A[13] + A[0] * A[6] * A[11] * A[13] +
			A[3] * A[5] * A[8] * A[14] - A[1] * A[7] * A[8] * A[14] -
			A[3] * A[4] * A[9] * A[14] + A[0] * A[7] * A[9] * A[14] +
			A[1] * A[4] * A[11] * A[14] - A[0] * A[5] * A[11] * A[14] -
			A[2] * A[5] * A[8] * A[15] + A[1] * A[6] * A[8] * A[15] +
			A[2] * A[4] * A[9] * A[15] - A[0] * A[6] * A[9] * A[15] -
			A[1] * A[4] * A[10] * A[15] + A[0] * A[5] * A[10] * A[15];
	}
	/**
	* Calculates the inverse of a 4x4 matrix
	* @return A New matrix which is the inverse of matrix calling the method.
	*/
	Matrix4D Inverse()
	{
		const float det = this->Det();
		if (det == 0)
			this;

		float submat1[9] = { this->A[5], this->A[6], this->A[7],
							this->A[9], this->A[10], this->A[11],
							this->A[13], this->A[14], this->A[15] };
		float submat2[9] = { this->A[4], this->A[6], this->A[7],
						this->A[8], this->A[10], this->A[11],
						this->A[12], this->A[14], this->A[15] };
		float submat3[9] = { this->A[4], this->A[5], this->A[7],
						this->A[8], this->A[9], this->A[11],
						this->A[12], this->A[13], this->A[15] };
		float submat4[9] = { this->A[4], this->A[5], this->A[6],
						this->A[8], this->A[9], this->A[10],
						this->A[12], this->A[13], this->A[14] };

		float submat5[9] = { this->A[1], this->A[2], this->A[3],
						this->A[9], this->A[10], this->A[11],
						this->A[13], this->A[14], this->A[15] };
		float submat6[9] = { this->A[0], this->A[2], this->A[3],
						this->A[8], this->A[10], this->A[11],
						this->A[12], this->A[14], this->A[15] };
		float submat7[9] = { this->A[0], this->A[1], this->A[3],
						this->A[8], this->A[9], this->A[11],
						this->A[12], this->A[13], this->A[15] };
		float submat8[9] = { this->A[0], this->A[1], this->A[2],
						this->A[8], this->A[9], this->A[10],
						this->A[12], this->A[13], this->A[14] };

		float submat9[9] = { this->A[1], this->A[2], this->A[3],
						this->A[5], this->A[6], this->A[7],
						this->A[13], this->A[14], this->A[15] };
		float submat10[9] = { this->A[0], this->A[2], this->A[3],
						this->A[4], this->A[6], this->A[7],
						this->A[12], this->A[14], this->A[15] };
		float submat11[9] = { this->A[0], this->A[1], this->A[3],
						this->A[4], this->A[5], this->A[7],
						this->A[12], this->A[13], this->A[14] };
		float submat12[9] = { this->A[0], this->A[1], this->A[2],
						this->A[4], this->A[5], this->A[6],
						this->A[12], this->A[13], this->A[14] };

		float submat13[9] = { this->A[1], this->A[2], this->A[3],
						this->A[5], this->A[6], this->A[7],
						this->A[9], this->A[10], this->A[11] };
		float submat14[9] = { this->A[0], this->A[2], this->A[3],
						this->A[4], this->A[6], this->A[7],
						this->A[8], this->A[10], this->A[11] };
		float submat15[9] = { this->A[0], this->A[1], this->A[3],
						this->A[4], this->A[5], this->A[7],
						this->A[8], this->A[9], this->A[11] };
		float submat16[9] = { this->A[0], this->A[1], this->A[2],
						this->A[4], this->A[5], this->A[6],
						this->A[8], this->A[9], this->A[10] };

		const float detdiv = (1 / det);

		const float x1 = Matrix4D::det3x3(submat1);
		const float x2 = Matrix4D::det3x3(submat2);
		const float x3 = Matrix4D::det3x3(submat3);
		const float x4 = Matrix4D::det3x3(submat4);

		const float y1 = Matrix4D::det3x3(submat5);
		const float y2 = Matrix4D::det3x3(submat6);
		const float y3 = Matrix4D::det3x3(submat7);
		const float y4 = Matrix4D::det3x3(submat8);

		const float z1 = Matrix4D::det3x3(submat9);
		const float z2 = Matrix4D::det3x3(submat10);
		const float z3 = Matrix4D::det3x3(submat11);
		const float z4 = Matrix4D::det3x3(submat12);

		const float w1 = Matrix4D::det3x3(submat13);
		const float w2 = Matrix4D::det3x3(submat14);
		const float w3 = Matrix4D::det3x3(submat15);
		const float w4 = Matrix4D::det3x3(submat16);

		Matrix4D adj(x1, -y1, z1, -w1, -x2, y2, -z2, w2, x3, -y3, z3, -w3, -x4, y4, -z4, w4);
		adj.Transpose();
		adj = adj * detdiv;

		return adj;
	}

	/** Changes the matrix into it's transpose*/
	void Transpose()
	{
		this->A[1] = this->A[4];
		this->A[2] = this->A[8];
		this->A[3] = this->A[12];
		this->A[4] = this->A[1];
		this->A[6] = this->A[9];
		this->A[7] = this->A[13];
		this->A[8] = this->A[2];
		this->A[9] = this->A[6];
		this->A[11] = this->A[14];
		this->A[12] = this->A[3];
		this->A[13] = this->A[7];
		this->A[14] = this->A[11];

	}

	/** 4x4 matrix multiplication with a 4x4 matrix
	* @return a new matrix
	*/
	Matrix4D operator*(const Matrix4D& other)
	{
		// performs multiplication between 2 4x4-matrices
		Matrix4D new_mat;
		new_mat.A[0] = this->A[0] * other.A[0] + this->A[1] * other.A[4] + this->A[2] * other.A[8] + this->A[3] * other.A[12];
		new_mat.A[1] = this->A[0] * other.A[1] + this->A[1] * other.A[5] + this->A[2] * other.A[9] + this->A[3] * other.A[13];
		new_mat.A[2] = this->A[0] * other.A[2] + this->A[1] * other.A[6] + this->A[2] * other.A[10] + this->A[3] * other.A[14];
		new_mat.A[3] = this->A[0] * other.A[3] + this->A[1] * other.A[7] + this->A[2] * other.A[11] + this->A[3] * other.A[15];

		new_mat.A[4] = this->A[4] * other.A[0] + this->A[5] * other.A[4] + this->A[6] * other.A[8] + this->A[7] * other.A[12];
		new_mat.A[5] = this->A[4] * other.A[1] + this->A[5] * other.A[5] + this->A[6] * other.A[9] + this->A[7] * other.A[13];
		new_mat.A[6] = this->A[4] * other.A[2] + this->A[5] * other.A[6] + this->A[6] * other.A[10] + this->A[7] * other.A[14];
		new_mat.A[7] = this->A[4] * other.A[3] + this->A[5] * other.A[7] + this->A[6] * other.A[11] + this->A[7] * other.A[15];

		new_mat.A[8] = this->A[8] * other.A[0] + this->A[9] * other.A[4] + this->A[10] * other.A[8] + this->A[11] * other.A[12];
		new_mat.A[9] = this->A[8] * other.A[1] + this->A[9] * other.A[5] + this->A[10] * other.A[9] + this->A[11] * other.A[13];
		new_mat.A[10] = this->A[8] * other.A[2] + this->A[9] * other.A[6] + this->A[10] * other.A[10] + this->A[11] * other.A[14];
		new_mat.A[11] = this->A[8] * other.A[3] + this->A[9] * other.A[7] + this->A[10] * other.A[11] + this->A[11] * other.A[15];

		new_mat.A[12] = this->A[12] * other.A[0] + this->A[13] * other.A[4] + this->A[14] * other.A[8] + this->A[15] * other.A[12];
		new_mat.A[13] = this->A[12] * other.A[1] + this->A[13] * other.A[5] + this->A[14] * other.A[9] + this->A[15] * other.A[13];
		new_mat.A[14] = this->A[12] * other.A[2] + this->A[13] * other.A[6] + this->A[14] * other.A[10] + this->A[15] * other.A[14];
		new_mat.A[15] = this->A[12] * other.A[3] + this->A[13] * other.A[7] + this->A[14] * other.A[11] + this->A[15] * other.A[15];

		return new_mat;
	}
	/**
	* Operator overloading, allowing scalar with a float and 4x4 matrix
	* @return A new matrix with the cells multiplied with the given float.
	*/

	Matrix4D operator*(const float& other)
	{
		// multiplies a matrix's cells with a float
		Matrix4D new_mat;

		for (int i = 0; i < 16; i++)
		{
			new_mat._matrix_values[i] = this->_matrix_values[i] * other;
		}
		return new_mat;
	}

	/** 4x4 matrix multiplication with a 4D vector 
	* @return a new vector
	*/

	Vector4D operator*(const Vector4D& other)
	{
		Vector4D new_vect;
		new_vect[0] = this->A[0] * other[0] + this->A[1] * other[1] + this->A[2] * other[2] + this->A[3] * other[3];
		new_vect[1] = this->A[4] * other[0] + this->A[5] * other[1] + this->A[6] * other[2] + this->A[7] * other[3];
		new_vect[2] = this->A[8] * other[0] + this->A[9] * other[1] + this->A[10] * other[2] + this->A[11] * other[3];
		new_vect[3] = this->A[12] * other[0] + this->A[13] * other[1] + this->A[14] * other[2] + this->A[15] * other[3];

		return new_vect;
	}

	/** allows access to the matrixs private data, reading */
	float& operator[] (int index)
	{
		if (index <= 15 && index >= 0)
		{
			return this->_matrix_values[index];
		}
		else
		{
			throw "INDEX OUT OF BOUNDS";
		}
	}
	/** allows access to the matrixs private data, writing */
	const float& operator[] (int index) const
	{
		if (index <= 15 && index >= 0)
		{
			return this->_matrix_values[index];
		}
		else
		{
			throw "INDEX OUT OF BOUNDS";
		}
	}

	static Matrix4D PerspProj(float fov, float aspect, float near, float far)
	{
		const float r = tan(fov/2)*near;
		const float t = r/aspect;
		const float l = r*-1;
		const float b = t*-1;
		const float n2 = near*2;
		const Matrix4D proj(
			n2/(r-l),	0,			(r + l) / (r - l),	0,
			0,			n2/(t-b),	 (t + b) / (t - b),	0,
			0,			0,			-((far+near)/(far-near)), -((2 * far * near) / (far - near)),
			0,			0,			-1,					0
			);
		return proj;
	}

	void Print()
	{
		std::cout <<
			A[0] << "\t" << A[1] << "\t" << A[2] << "\t" << A[3] << "\n" <<
			A[4] << "\t" << A[5] << "\t" << A[6] << "\t" << A[7] << "\n" <<
			A[8] << "\t" << A[9] << "\t" << A[10] << "\t" << A[11] << "\n" <<
			A[12] << "\t" << A[13] << "\t" << A[14] << "\t" << A[15] << std::endl;
		std::cout << "---------------------------------------"<<std::endl;
	}
private:
	/**
	* private member function used for the calculation of a 3x3 matrix Determinant used in the calculation of 4x4 Matrix inverse.
	* @param submat[9] : a submatrix of a 4x4 matrix
	* @return determinant of a 3x3 matrix
	*/
	static float det3x3(const float submat[9])
	{

		return submat[0] * (submat[4] * submat[8] - submat[5] * submat[7]) -
			submat[1] * (submat[3] * submat[8] - submat[5] * submat[6]) +
			submat[2] * (submat[3] * submat[7] - submat[4] * submat[6]);
	}
	float _matrix_values[16]{}; /*[ x1[0]  y1[1]  z1[2]  w1[3]  ]
								[ x2[4]  y2[5]  z2[6]  w2[7]  ]
								[ x3[8]  y3[9]  z3[10] w3[11] ]
								[ x4[12] y4[13] z4[14  w4[15] ] */
																
};

#undef A