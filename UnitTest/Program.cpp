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
	//	// view, projection ���� ����
	//	vpb = new VPBuffer();
	//
	//	// View ��� ����
	//	{
	//		D3DXMatrixLookAtLH
	//		(
	//			&view,
	//			&Vector3(0, 0, 0), // ī�޶� ��ġ (����)
	//			&Vector3(0, 0, 1), // ī�޶� �ٶ󺸴� ����
	//			&Vector3(0, 1, 0)  // ī�޶��� ���� ����
	//		);
	//	}
	//
	//	// Projection ��� ����
	//	{
	//		// ���� ���� ���
	//		D3DXMatrixOrthoOffCenterLH
	//		(
	//			&proj,
	//			0.0f, 					// ���� ���� ��ġ
	//			(float)WinMaxWidth, 	// ������ ���� ��ġ
	//			0.0f, 					// �Ʒ��� ���� ��ġ
	//			(float)WinMaxHeight, 	// ���� ���� ��ġ
	//			0, 						// Near Plane	(����� ��)
	//			1						// Far Plane	(�ָ� �ִ� ��)
	//		);
	//	}
	//
	//	// ���� ����
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
