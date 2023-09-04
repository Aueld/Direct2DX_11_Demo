#pragma once

enum DefaultClip
{
	IDLE = 0,
	ATTACK,
	MOVE_L,
	MOVE_R,
	MOVE_U,
	MOVE_D,
};

class AnimationClip
{
public:
	friend class Animator;

	AnimationClip(wstring clipName, Texture2D* srcTex, UINT frameCount, Vector2 startPos, Vector2 endPos, float playRate, bool bReversed = false);

protected:
	wstring clipName = L"";
	vector<Vector2> keyFrames;
	UINT frameCount = 0;
	ID3D11ShaderResourceView* srv = nullptr;
	Vector2 texelFrameSize = Values::ZeroVec2;
	float playRate = 0.f;
	bool bReversed = false;
};

class Animator
{
public:
	Animator();
	~Animator();

	void Update();
	Vector2 GetCurrentFrame() { return currentFrame; }
	Vector2 GetTexelFrameSize() { return currentClip->texelFrameSize; }
	ID3D11ShaderResourceView* GetCurrentSRV() { return currentClip->srv; }

	void AddAnimClip(AnimationClip* animClip);
	void SetCurrentAnimClip(wstring clipName);

	void SetIdleClip(AnimationClip* clip);
	void SetAttackClip(AnimationClip* clip);
	void SetMoveLClip(AnimationClip* clip);
	void SetMoveRClip(AnimationClip* clip);
	void SetMoveUClip(AnimationClip* clip);
	void SetMoveDClip(AnimationClip* clip);

	void PlayIdle() { SetCurrentAnimClip(defaultClip[IDLE]->clipName); }
	void PlayAttack() { SetCurrentAnimClip(defaultClip[ATTACK]->clipName); }
	void PlayMoveL() { SetCurrentAnimClip(defaultClip[MOVE_L]->clipName); }
	void PlayMoveR() { SetCurrentAnimClip(defaultClip[MOVE_R]->clipName); }
	void PlayMoveU() { SetCurrentAnimClip(defaultClip[MOVE_U]->clipName); }
	void PlayMoveD() { SetCurrentAnimClip(defaultClip[MOVE_D]->clipName); }

private:
	unordered_map<wstring, AnimationClip*> animClips;
	bool CheckExist(wstring clipName) { return animClips.find(clipName) != animClips.end(); }

	AnimationClip* currentClip = nullptr;
	UINT currentFrameIndex = 0;
	Vector2 currentFrame = Values::ZeroVec2;

	float deltaTime = 0.0f;

	AnimationClip* defaultClip[6] = { 0 };
};