#include "Framework.h"
#include "AnimationRect.h"


AnimationRect::AnimationRect(Vector3 position, Vector3 size)
	: TextureRect(position, size, .0f)
{
	SetShader(ShaderPath + L"Animation.hlsl");

	animator = new Animator();
	movement = new Movement();

	Texture2D* srcTex = new Texture2D(TexturePath + L"player.png");
	AnimationClip* RunR = new AnimationClip(L"RunR", srcTex, 10, Values::ZeroVec2, { srcTex->GetWidth(), srcTex->GetHeight() * 0.5f }, 1.f / 15.f);
	AnimationClip* RunL = new AnimationClip(L"RunL", srcTex, 10, { 0, srcTex->GetHeight() * 0.5f }, { srcTex->GetWidth(), srcTex->GetHeight() }, 1.f / 15.f, true);

	// 애니메이션 저장
	animator->AddAnimClip(RunR);
	animator->AddAnimClip(RunL);
	animator->SetCurrentAnimClip(L"RunL");

	SAFE_DELETE(srcTex);

	// Sampler
	{
		// 선형 보간
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);
		States::CreateSampler(&desc, &point[0]);

		// point sampling
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		States::CreateSampler(&desc, &point[1]);
	}

	// Blend
	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &bPoint[0]);
		
		desc.RenderTarget[0].BlendEnable = true;
		States::CreateBlend(&desc, &bPoint[1]);
	}
}

AnimationRect::~AnimationRect()
{

}

void AnimationRect::Update()
{
	animator->Update();

	MapVertexBuffer();
	{
		// uv 좌표로 봤을 때 3번이 0,0지점
		vertices[0].uv.x = animator->GetCurrentFrame().x;
		vertices[0].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

		vertices[1].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
		vertices[1].uv.y = animator->GetCurrentFrame().y;

		vertices[2].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
		vertices[2].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

		vertices[3].uv.x = animator->GetCurrentFrame().x;
		vertices[3].uv.y = animator->GetCurrentFrame().y;
	}
	UnmapVertexBuffer();

	__super::Update();
}

void AnimationRect::Render()
{
	srv = animator->GetCurrentSRV();

	DC->PSSetSamplers(0, 1, &point[1]);
	DC->OMSetBlendState(bPoint[1], nullptr, (UINT)0xFFFFFFFF);

	__super::Render();

	//DC->PSSetSamplers(0, 1, &point[0]);
	//DC->OMSetBlendState(bPoint[0], nullptr, (UINT)0xFFFFFFFF);
}

void AnimationRect::Move()
{
	movement->Update(position);

	if (Keyboard::Get()->Press('W') && Keyboard::Get()->Press('D'))
	{
		position.y += 25 * Time::Delta();
		position.x += 100 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunU");
	}
	else if (Keyboard::Get()->Press('W') && Keyboard::Get()->Press('A'))
	{
		position.y += 25 * Time::Delta();
		position.x -= 100 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunU");
	}
	else if (Keyboard::Get()->Press('S') && Keyboard::Get()->Press('A'))
	{
		position.y -= 25 * Time::Delta();
		position.x -= 100 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunD");
	}
	else if (Keyboard::Get()->Press('S') && Keyboard::Get()->Press('D'))
	{
		position.y -= 25 * Time::Delta();
		position.x += 100 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunD");
	}
	else if (Keyboard::Get()->Press('A'))
	{
		position.x -= 128 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunL");
	}
	else if (Keyboard::Get()->Press('D'))
	{
		position.x += 128 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunR");
	}

	if (Keyboard::Get()->Press('W'))
	{
		position.y += 72 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunU");
	}
	else if (Keyboard::Get()->Press('S'))
	{
		position.y -= 72 * Time::Delta();
		animator->SetCurrentAnimClip(L"RunD");
	}
}
