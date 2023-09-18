#pragma once

enum DefaultClip
{
	Default = 0,
	IDLE,
	WALK,
	RUN,
	RUNATTACK,
	ATTACK,
	JUMP,
	DOWN
};

class AnimationClip
{
public:
	friend class Animator;

	AnimationClip(wstring clipName, Texture2D* srcTex, UINT frameCount, Vector2 startPos, Vector2 endPos, float playRate, bool bLoop, bool bReversed = false);

protected:
	wstring clipName = L"";
	vector<Vector2> keyFrames;
	UINT frameCount = 0;
	ID3D11ShaderResourceView* srv = nullptr;
	Vector2 texelFrameSize = Values::ZeroVec2;
	float playRate = 0.f;
	bool bLoop = true;
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

	void SetClip(AnimationClip* clip);

	void SetIdleClip(AnimationClip* clip);
	void SetAttackClip(AnimationClip* clip);
	void SetWalkClip(AnimationClip* clip);
	void SetRunClip(AnimationClip* clip);
	void SetJumpClip(AnimationClip* clip);
	void SetDownClip(AnimationClip* clip);

	void PlayAnimation() { SetCurrentAnimClip(defaultClip[Default]->clipName); }
	void PlayIdle() { SetCurrentAnimClip(defaultClip[IDLE]->clipName); }
	void PlayAttack() { SetCurrentAnimClip(defaultClip[ATTACK]->clipName); }
	void PlayWalk() { SetCurrentAnimClip(defaultClip[WALK]->clipName); }
	void PlayRun() { SetCurrentAnimClip(defaultClip[RUN]->clipName); }
	void PlayJump() { SetCurrentAnimClip(defaultClip[JUMP]->clipName); }
	void PlayDown() { SetCurrentAnimClip(defaultClip[DOWN]->clipName); }

private:
	unordered_map<wstring, AnimationClip*> animClips;
	bool CheckExist(wstring clipName) { return animClips.find(clipName) != animClips.end(); }

	AnimationClip* currentClip = nullptr;
	UINT currentFrameIndex = 0;
	Vector2 currentFrame = Values::ZeroVec2;

	float deltaTime = 0.0f;

	AnimationClip* defaultClip[10] = { 0 };
};