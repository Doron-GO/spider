#include <DxLib.h>
#include "Transform.h"
#include "Capsule.h"

Capsule::Capsule(Transform* parent)
{
	transformParent_ = parent;
	radius_ = 0.0f;
	relativePosTop_ = { 0.0f, 0.0f, 0.0f };
	relativePosDown_ = { 0.0f, 0.0f, 0.0f };
}

Capsule::~Capsule(void)
{
}

void Capsule::Draw(void)
{

	// ã‚Ì‹…‘Ì
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, 5, COLOR, COLOR, false);

	// ‰º‚Ì‹…‘Ì
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, 5, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// ‹…‘Ì‚ðŒq‚®ü(X+)
	dir = transformParent_->GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ðŒq‚®ü(X-)
	dir = transformParent_->GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ðŒq‚®ü(Z+)
	dir = transformParent_->GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ðŒq‚®ü(Z-)
	dir = transformParent_->GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ƒJƒvƒZƒ‹‚Ì’†S
	DrawSphere3D(GetCenter(), 5.0f, 10, COLOR, COLOR, true);

}

void Capsule::SetRelativePosTop(const VECTOR& pos)
{
	relativePosTop_ = pos;
}

void Capsule::SetRelativePosDown(const VECTOR& pos)
{
	relativePosDown_ = pos;
}

VECTOR Capsule::GetPosTop(void) const
{
	return GetRelativePos(relativePosTop_);
}

VECTOR Capsule::GetPosDown(void) const
{
	return GetRelativePos(relativePosDown_);
}

VECTOR Capsule::GetRelativePos(const VECTOR& pos) const
{
	VECTOR relativePos = transformParent_->quaRot.PosAxis(pos);
	return VAdd(transformParent_->pos, relativePos);
}

float Capsule::GetRadius(void) const
{
	return radius_;
}

void Capsule::SetRadius(float radius)
{
	radius_ = radius;
}

float Capsule::GetHeight(void) const
{
	return relativePosTop_.y;
}

VECTOR Capsule::GetCenter(void) const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}

Capsule Capsule::Copy(Transform* base)
{
	Capsule ret = Capsule(base);
	ret.SetRelativePosDown(relativePosDown_);
	ret.SetRelativePosTop(relativePosTop_);
	ret.SetRadius(radius_);
	return ret;
}
