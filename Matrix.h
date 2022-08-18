#pragma once
#include <cmath>
#include <algorithm>
#include <GL/glew.h>

class Matrix
{
public:
	Matrix(){}

	// �z��̓��e�ŏ���������R���X�g���N�^
	//	 a�FGLfloat�^��16�v�f�̔z��
	Matrix(const GLfloat* a)
	{
		std::copy(a, a + 16, m_matrix);
	}

	// �s��̗v�f���E�Ӓl�Ƃ��ĎQ��
	const GLfloat& operator[](std::size_t i) const
	{
		return m_matrix[i];
	}

	// �s��̗v�f�����Ӓl�Ƃ��ĎQ��
	GLfloat& operator[](std::size_t i)
	{
		return m_matrix[i];
	}

	// �ϊ��s��̔z���Ԃ�
	const GLfloat* Data() const
	{
		return m_matrix;
	}

	// �@���x�N�g���̕ϊ��s������߂�
	void GetNormalMatrix(GLfloat* m) const
	{
		m[0] = m_matrix[5]  * m_matrix[10] - m_matrix[6]  * m_matrix[9];
		m[1] = m_matrix[6]  * m_matrix[8]  - m_matrix[4]  * m_matrix[10];
		m[2] = m_matrix[4]  * m_matrix[9]  - m_matrix[5]  * m_matrix[8];
		m[3] = m_matrix[9]  * m_matrix[2]  - m_matrix[10] * m_matrix[1];
		m[4] = m_matrix[10] * m_matrix[0]  - m_matrix[8]  * m_matrix[2];
		m[5] = m_matrix[8]  * m_matrix[1]  - m_matrix[9]  * m_matrix[0];
		m[6] = m_matrix[1]  * m_matrix[6]  - m_matrix[2]  * m_matrix[5];
		m[7] = m_matrix[2]  * m_matrix[4]  - m_matrix[0]  * m_matrix[6];
		m[8] = m_matrix[0]  * m_matrix[5]  - m_matrix[1]  * m_matrix[4];
	}

	// �P�ʍs���ݒ�
	void LoadIdentity()
	{
		std::fill(m_matrix, m_matrix + 16, 0.0f);
		m_matrix[0] = m_matrix[5] = m_matrix[10] = m_matrix[15] = 1.0f;
	}

	// �P�ʍs����쐬
	static Matrix Identity()
	{
		Matrix temp;
		temp.LoadIdentity();
		return temp;
	}

	// (x, y, z)�������s�ړ�����ϊ��s����쐬����
	static Matrix Translate(GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix temp;
		temp.LoadIdentity();
		temp[12] = x;
		temp[13] = y;
		temp[14] = z;

		return temp;
	}

	// (x, y, z)�{�Ɋg��k������ϊ��s����쐬����
	static Matrix Scale(GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix temp;
		temp.LoadIdentity();
		temp[0]  = x;
		temp[5]  = y;
		temp[10] = z;

		return temp;
	}

	// (x, y, z)������a��]����ϊ��s����쐬����
	static Matrix Rotate(GLfloat a, GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix temp;
		const GLfloat d(sqrt(x * x + y * y + z * z));

		if(d > 0.0f)
		{
			const GLfloat l(x / d), m(y / d), n(z / d);
			const GLfloat l2(l * l), m2(m * m), n2(n * n);
			const GLfloat lm(l * m), mn(m * n), nl(n * l);
			const GLfloat c(cos(a)), c1(1.0f - c), s(sin(a));

			temp.LoadIdentity();
			temp[0]  = (1.0f - l2) * c + l2;
			temp[1]  = lm * c1 + n * s;
			temp[2]  = nl * c1 - m * s;
			temp[4]  = lm * c1 - n * s;
			temp[5]  = (1.0f - m2) * c + m2;
			temp[6]  = mn * c1 + l * s;
			temp[8]  = nl * c1 + m * s;
			temp[9]  = mn * c1 - n * s;
			temp[10] = (1.0f - n2) * c + n2;
		}

		return temp;
	}

	// ��Z
	Matrix operator*(const Matrix& m) const
	{
		Matrix temp;

		for(int i = 0; i < 16; ++i)
		{
			const int j(i & 3), k(i & ~3);
			temp[i] = m_matrix[0  + j] * m[k + 0] +
					  m_matrix[4  + j] * m[k + 1] +
					  m_matrix[8  + j] * m[k + 2] +
					  m_matrix[12 + j] * m[k + 3];
		}

		return temp;
	}

	static Matrix LookAt(GLfloat ex, GLfloat ey, GLfloat ez,	// ���_�̈ʒu
						 GLfloat gx, GLfloat gy, GLfloat gz,	// �ڕW�_�̈ʒu
						 GLfloat ux, GLfloat uy, GLfloat uz)	// ������x�N�g��
	{
		const Matrix tv = Translate(-ex, -ey, -ez);

		// t�� = e - g;
		const GLfloat tx = ex - gx;
		const GLfloat ty = ey - gy;
		const GLfloat tz = ez - gz;

		// r�� = u x t��
		const GLfloat rx = uy * tz - uz * ty;
		const GLfloat ry = uz * tx - ux * tz;
		const GLfloat rz = ux * ty - uy * tx;

		// s�� = t�� x r��
		const GLfloat sx = ty * rz - tz * ry;
		const GLfloat sy = tz * rx - tx * rz;
		const GLfloat sz = tz * ry - ty * rx;

		// s���̒����`�F�b�N
		const GLfloat s2 = sx * sx + sy * sy + sz * sz;
		if(s2 == 0.0f){ return tv; }
		
		// ��]�̕ϊ��s��
		Matrix rv;
		rv.LoadIdentity();

		// r���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat r = sqrt(rx * rx + ry * ry + rz * rz);
		rv[0] = rx / r;
		rv[4] = ry / r;
		rv[8] = rz / r;

		// s���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat s = sqrt(s2);
		rv[1] = sx / s;
		rv[5] = sy / s;
		rv[9] = sz / s;

		// t���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat t = sqrt(tx * tx + ty * ty + tz * tz);
		rv[2]  = tx / t;
		rv[6]  = ty / t;
		rv[10] = tz / t;

		// ���_�̕��s�ړ��̕ϊ��s��Ɏ��_�̉�]�̕ϊ��s����悶��
		return rv * tv;
	}

	// ���s���e�ϊ��s����쐬����
	static Matrix Orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
	{
		Matrix temp;
		const GLfloat dx = right - left;
		const GLfloat dy = top - bottom;
		const GLfloat dz = zFar - zNear;

		if(dx != 0.0f && dy != 0.0f && dz != 0.0f)
		{
			temp.LoadIdentity();
			temp[0]  = 2.0f / dx;
			temp[5]  = 2.0f / dy;
			temp[10] = -2.0f / dz;
			temp[12] = -(right + left) / dx;
			temp[13] = -(top + bottom) / dy;
			temp[14] = -(zFar + zNear) / dz;
		}

		return temp;
	}

	// �������e�ϊ��s����쐬����
	static Matrix Frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
	{
		Matrix temp;
		const GLfloat dx = right - left;
		const GLfloat dy = top - bottom;
		const GLfloat dz = zFar - zNear;

		if(dx != 0.0f && dy != 0.0f && dz != 0.0f)
		{
			temp.LoadIdentity();
			temp[0]  = 2.0f * zNear / dx;
			temp[5]  = 2.0f * zNear / dy;
			temp[8]  = (right + left) / dx;
			temp[9]  = (top + bottom) / dy;
			temp[10] = -(zFar + zNear) / dz;
			temp[11] = -1.0f;
			temp[14] = -2.0f * zFar * zNear / dz;
			temp[15] = 0.0f;
		}

		return temp;
	}

	// ��p���w�肵�ē������e�ϊ��s����쐬����
	static Matrix Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
	{
		Matrix temp;
		const GLfloat dz = zFar - zNear;

		if(dz != 0.0f)
		{
			temp.LoadIdentity();
			temp[5] = 1.0f / tan(fovy * 0.5f);
			temp[0] = temp[5] / aspect;
			temp[10] = -(zFar + zNear) / dz;
			temp[11] = -1.0f;
			temp[14] = -2.0f * zFar * zNear / dz;
			temp[15] = 0.0f;
		}

		return temp;
	}

private:
	GLfloat m_matrix[16];	// �ϊ��s��̗v�f
};