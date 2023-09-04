#include "stdafx.h"
#include "Program.h"

#include "Systems/Window.h"

#include "Demos/RectDemo.h"
#include "Demos/TextureDemo.h"
#include "Demos/ShadedDemo.h"
#include "Demos/BlurDemo.h"
#include "Demos/RenderingTargetDemo.h"
#include "Demos/CollisionDemo.h"
#include "Demos/AnimationDemo.h"
#include "Demos/PlayerDemo.h"
#include "Demos/TileMapDemo.h"

void Program::Init()
{
	States::Create();
	Camera::Create();

	//// Create Buffer
	//{
	//	// view, projection 버퍼 선언
	//	vpb = new VPBuffer();
	//
	//	// View 행렬 생성
	//	{
	//		D3DXMatrixLookAtLH
	//		(
	//			&view,
	//			&Vector3(0, 0, 0), // 카메라 위치 (원점)
	//			&Vector3(0, 0, 1), // 카메라가 바라보는 방향
	//			&Vector3(0, 1, 0)  // 카메라의 위쪽 방향
	//		);
	//	}
	//
	//	// Projection 행렬 생성
	//	{
	//		// 직교 투영 행렬
	//		D3DXMatrixOrthoOffCenterLH
	//		(
	//			&proj,
	//			0.0f, 					// 왼쪽 면의 위치
	//			(float)WinMaxWidth, 	// 오른쪽 면의 위치
	//			0.0f, 					// 아래쪽 면의 위치
	//			(float)WinMaxHeight, 	// 위쪽 면의 위치
	//			0, 						// Near Plane	(가까운 면)
	//			1						// Far Plane	(멀리 있는 면)
	//		);
	//	}
	//
	//	// 버퍼 세팅
	//	vpb->SetView(view);
	//	vpb->SetProj(proj);
	//}

	//Push(new RectDemo);
	//Push(new TextureDemo);
	//Push(new ShadedDemo);
	//Push(new BlurDemo);
	//Push(new RenderingTargetDemo);
	//Push(new CollisionDemo);
	//Push(new AnimationDemo);
	Push(new PlayerDemo);
	//Push(new TileMapDemo);
}

void Program::Destroy()
{
	//SAFE_DELETE(vpb);

	for (IObject* obj : objs)
	{
		obj->Destroy();
		SAFE_DELETE(obj);
	}

	Camera::Delete();
}

void Program::Update()
{
	for (IObject* obj : objs)
		obj->Update();

	Camera::Get()->Move();
}

void Program::Render()
{
	for (IObject* obj : objs)
		obj->Render();

	Camera::Get()->UpdateView();
	Camera::Get()->Render();
}

void Program::PostRender()
{
	for (IObject* obj : objs)
		obj->PostRender();
}

void Program::GUI()
{
	for (IObject* obj : objs)
	{
		obj->GUI();
	}
}

void Program::Push(IObject* obj)
{
	objs.push_back(obj);
	obj->Init();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	srand((UINT)time(NULL));

	DXDesc desc;
	desc.AppName = L"D2DGame";
	desc.instance = instance;
	desc.handle = NULL;
	desc.width = WinMaxWidth;
	desc.height = WinMaxHeight;

	Program* program = new Program();
	Window* window = new Window(desc);
	WPARAM wParam = window->Run(program);

	SAFE_DELETE(window);
	SAFE_DELETE(program);

	return wParam;
}
