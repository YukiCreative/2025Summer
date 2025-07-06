#pragma once
#include <memory>
#include <unordered_map>
#include "Vector3.h"

enum class ColKind3D
{
	kNone, // None�̓G���[��f�����߂̑���
	kSphere,
	kBox,
	kPolygon,
	kCapsule,
};

// �����蔻��N���X
// �Փ˂ɕK�v�ȏ�������
class Collider3D abstract
{
public:
	Collider3D();
	virtual ~Collider3D();

	void Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic);

	virtual void Draw() const abstract;

	const ColKind3D GetColKind() const { return m_colKind; }
	virtual void SetPos(const Vector3& pos);
	const Vector3& GetPos() const { return m_pos; }
	const int  GetWeight() const { return m_weight; }
	const bool IsThrough() const { return m_isThrough; }
	const bool IsStatic()  const { return m_isStatic; }

	void SetIsThrough(const bool isThrough) { m_isThrough = isThrough; }
	void SetIsStatic(const bool isStatic) { m_isStatic = isStatic; }

	bool IsValid() const { return m_isValid; }
	void ValidCol() { m_isValid = true; }
	void InvalidCol() { m_isValid = false; }

protected:
	ColKind3D m_colKind;
	Vector3 m_pos;
	// ���̃t���[���ɓ��������ǂ���
	bool m_isMove;
	// �������������߂��������s�����ǂ���
	// �ǂ��炩��IsThrough�Ȃ�s��Ȃ�
	bool m_isThrough;
	// �����߂�����邩�ǂ���
	bool m_isStatic;
	// �d��
	// ���������ΏۂƂ̉����߂��̔z���Ɏg��
	int m_weight;
	// �L�����ǂ���
	bool m_isValid;
};