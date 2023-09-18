#include "Framework.h"
#include "Animator.h"

AnimationClip::AnimationClip(wstring clipName, Texture2D* srcTex, UINT frameCount, Vector2 startPos, Vector2 endPos, float playRate, bool bLoop, bool bReversed)
	:clipName(clipName), frameCount(frameCount), playRate(playRate), bLoop(bLoop), bReversed(bReversed)
{
	srv = srcTex->GetSRV();

	// 텍스처의 너비와 높이를 가져와서 이미지에 적용
	float imageWidth  = srcTex->GetWidth();
	float imageHeight = srcTex->GetHeight();

	// 클립 사이즈 (애니메이션이 그려져있는 텍스처의 길이)
	Vector2 clipSize = endPos - startPos;

	// 프레임 사이즈 (애니메이션 중 그림 하나)
	Vector2 frameSize;
	frameSize.x = clipSize.x / frameCount;
	frameSize.y = clipSize.y;

	// 텍셀 프레임 너비와 높이 계산 (UV 좌표에서의 크기 계산)
	texelFrameSize.x = frameSize.x / imageWidth;
	texelFrameSize.y = frameSize.y / imageHeight;

	// 텍셀 시작 위치
	Vector2 texelStartPos;
	texelStartPos.x = startPos.x / imageWidth;
	texelStartPos.y = startPos.y / imageHeight;

	// 현재 재생되는 프레임
	Vector2 keyframe;

	for (UINT i = 0; i < frameCount; i++) // 프레임에 대해 반복
	{
		// 키 프레임의 x, y 좌표 계산
		keyframe.x = texelStartPos.x + (texelFrameSize.x * i);
		keyframe.y = texelStartPos.y;
		// 키 프레임을 keyFrame 백터에 추가
		keyFrames.push_back(keyframe);
	}
}

Animator::Animator()
{
}

Animator::~Animator()
{
	for (auto clip : animClips)
		SAFE_DELETE(clip.second);
}

void Animator::Update()
{
	if (deltaTime > currentClip->playRate)
	{
		// 역재생이 아닌 경우
		if (currentClip->bReversed == false)
		{
			currentFrameIndex++; // 프레임 인덱스 증가

			// 현재 프레임 인덱스가 프레임 수와 같은 경우
			if (currentFrameIndex == currentClip->frameCount)
				if (currentClip->bLoop)
					currentFrameIndex = 0; // 프레임 인덱스를 0으로 설정
				else
					currentFrameIndex--;

			// 현재 프레임을 업데이트
			currentFrame = currentClip->keyFrames[currentFrameIndex];
		}
		else // 역재생인 경우
		{
			currentFrameIndex--; // 프레임 인덱스 감소

			// 현재 프레임 인덱스가 -1인 경우
			if (currentFrameIndex == -1)
				if (currentClip->bLoop)
					currentFrameIndex = currentClip->frameCount - 1; // 현재 프레임 인덱스를 프레임 수 - 1 로 설정
				else
					currentFrameIndex++;

			// 현재 프레임을 업데이트
			currentFrame = currentClip->keyFrames[currentFrameIndex];
		}
		deltaTime = 0.0f; // deltaTime을 초기화
	}
	else
		deltaTime += Time::Delta(); // deltaTime에 경과 시간을 더한다
}

void Animator::AddAnimClip(AnimationClip* animClip)
{
	animClips.insert(make_pair(animClip->clipName, animClip));
}

void Animator::SetCurrentAnimClip(wstring clipName)
{
	// 현재 클립이 없고, 애니메이션은 존재할 때
	if (currentClip == nullptr && CheckExist(clipName) == true)
		currentClip = animClips.find(clipName)->second; // 현재 클립을 clipName으로 변경
	// 이미 클립이 있고, 애니메이션이 지금 재생되는 것과 같을 때
	else if (currentClip != nullptr && currentClip->clipName == clipName)
		return;

	// 클립이 존재한다면
	if (CheckExist(clipName))
	{
		// 현재 클립을 clipName으로 변경
		currentClip = animClips.find(clipName)->second;
		deltaTime = 0.0f;

		// 역재생이 활성화 되어 있다면
		if (currentClip->bReversed == true)
			// 현재 프레임 인덱스를 프레임 수 - 1로 설정
			currentFrameIndex = currentClip->frameCount - 1;
		else
			// 현재 프레임 인덱스를 0으로 설정
			currentFrameIndex = 0;

		// 현재 프레임을 업데이트
		currentFrame = currentClip->keyFrames[currentFrameIndex];
	}
}

#pragma region Set Default Clip
void Animator::SetClip(AnimationClip* clip)
{
	defaultClip[Default] = clip;
	AddAnimClip(clip);
}

void Animator::SetIdleClip(AnimationClip* clip)
{
	defaultClip[IDLE] = clip;
	AddAnimClip(clip);
}

void Animator::SetAttackClip(AnimationClip* clip)
{
	defaultClip[ATTACK] = clip;
	AddAnimClip(clip);
}

void Animator::SetWalkClip(AnimationClip* clip)
{
	defaultClip[WALK] = clip;
	AddAnimClip(clip);
}

void Animator::SetRunClip(AnimationClip* clip)
{
	defaultClip[RUN] = clip;
	AddAnimClip(clip);
}

void Animator::SetJumpClip(AnimationClip* clip)
{
	defaultClip[JUMP] = clip;
	AddAnimClip(clip);
}

void Animator::SetDownClip(AnimationClip* clip)
{
	defaultClip[DOWN] = clip;
	AddAnimClip(clip);
}
#pragma endregion