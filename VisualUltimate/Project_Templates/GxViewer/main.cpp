#include <windows.h>
#include <glx.h>
#include <gxBaseViewer.h>
#include <GLSLShaders.h>
#include "../Shaders/$$root$$_Shaders.h"
#include <PreciseTimer.h>
//-------------------------------------------------------------------------------------------------------------
struct POS2D{
	int x;
	int y;
};
//-------------------------------------------------------------------------------------------------------------
struct VIEWER:GX_VIEWER{
	//----------------------------------
	SPR::MATH::VEC2Df										OrthoQuadPos[6];
	GLuint															OrthoQuadPosID;
	//-- SHADERS -----------------------
	GLSL::SHADER_PROGRAM								DefaultShader;
	//----------------------------------
	void InitShaders(){
		VIEWER& m = *this;

		// Création QuadVBO
		{
			float b = -1;
			float t = 1;
			float l = -1;
			float r = 1;

			OrthoQuadPos[0].Set(r, t);
			OrthoQuadPos[1].Set(l, t);
			OrthoQuadPos[2].Set(l, b);
			OrthoQuadPos[3].Set(r, t);
			OrthoQuadPos[4].Set(l, b);
			OrthoQuadPos[5].Set(r, b);
			OrthoQuadPosID=gl().GenBuffer();
			{
				glBindBuffer(GL_ARRAY_BUFFER, OrthoQuadPosID);
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(*OrthoQuadPos), OrthoQuadPos, GL_STATIC_DRAW);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		//GLSL::GX_SHADER_MAKER::GenerateShader(m.gl, OUT m.CheckerShader, GLSL::CheckerShader);
		GLSL::GX_SHADER_MAKER::GenerateShader(m.gl, OUT m.DefaultShader, GLSL::Default);
	}
	//----------------------------------
	void ReleaseShaders(){
		VIEWER& m = *this;
		m.DefaultShader.Release(m.gl);
	}
	//----------------------------------
// 	/*virtual*/ void __fastcall OnIdle();
// 	/*virtual*/ void __fastcall OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bSkip);
 	/*virtual*/ void __fastcall OnRender();
	/*virtual*/ void __fastcall OnMouseMove(UINT msg, WPARAM wParam, int MouseX, int MouseY, BOOL& bSkip){
		VIEWER& m = *this;

		if(wParam&MK_LBUTTON){
// 			m.Shader.MousePos.x = MouseX;
// 			m.Shader.MousePos.y = m.ViewportHeight-MouseY;
		}
	}
	//----------------------------------
 	/*virtual*/ void __fastcall OnMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bSkip){
		VIEWER& m = *this;

		if(msg==WM_LBUTTONDOWN){
			int MouseX = GET_X_LPARAM(lParam);
			int MouseY = GET_Y_LPARAM(lParam);
// 			m.Shader.MouseClick.x = MouseX;
// 			m.Shader.MouseClick.y = m.ViewportHeight-MouseY;
		}
	}
	//----------------------------------
	///*virtual*/ void __fastcall OnMouseWheel(UINT msg, WPARAM wParam, LPARAM lParam,BOOL& bSkip);
	//----------------------------------
 	/*virtual*/ void __fastcall OnWindowChangeSize(int width, int height){
		VIEWER& m = *this;

		//m.Shader.Resolution.Set(width, height, float(width)/float(height));

		GX_VIEWER::OnWindowChangeSize(width, height);
	}
	//----------------------------------
// 	/*virtual*/ void __fastcall OnKeyboardEvent(UINT Msg, KEY_MSG_DATA wParam, LPARAM lParam, BOOL& bSkip);
// 	/*virtual*/ void __fastcall OnJoystick(int iControllerID);
// 	/*virtual*/ void __fastcall OnCommand(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bSkip);
// 	/*virtual*/ void __fastcall OnFocus(HWND hLoseFocusWnd, BOOL& bSkip);
// 	/*virtual*/ void __fastcall OnMoveWindow();
// 	/*virtual*/ void __fastcall NotifyChange();
// 	/*virtual*/ void __fastcall OnClose();
};
//-------------------------------------------------------------------------------------------------------------
void VIEWER::OnRender(){
	VIEWER& m=*this;

	float TimeSeconds = TimerCountToSecond(GetTimerCount());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GX_RGBA_t Color1 = gl().GetClearColor();
	GX_RGBA_t Color2 = { Color1.r*0.8f, Color1.g*0.8f, Color1.b*0.8f, 1.f};
	//gl.DrawBackgroundChecker(m.CheckerShader, Color1, Color2);
	GX_PUSH_MODEL_MATRIX(){
		GX_PUSH_VIEW_MATRIX(){
			SetCamera();
		}
	}

	GX_START_PROGRAM(m.DefaultShader){
		//-------------
		m.NextViewData.SetOrthoView(GX_VIEW_DATA::CAMERA_TOP);
		SetCamera();

		gl().EnableDepthBufferTesting();
 		gl().Model.Identity();
 		gl().View.Identity();
		gl().Fragment.SetSmoothShadingModel(false);

		GLSL::PROGRAM_ARRAYS ProgramArrays;
		ProgramArrays.VertexAttribPointerAndBind2f(m.DefaultShader.Locations.Position, OrthoQuadPosID);
		gl().DrawArrays(GL_TRIANGLES, 0, 6, true);
	}

	m.NotifyChange();
}
//-------------------------------------------------------------------------------------------------------------
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, char* lpCmdLine, int nCmdShow){
  VIEWER Viewer;
	//---
  Viewer.DisplayMode=GX_DOUBLEBUFFER|GX_DEPTHBUFFER|GX_FSAA;
  Viewer.MaxFSAA_Sample=6;
  Viewer.Init(100, 100, 800, 600, GX_BASE_VIEWER::CENTERED_WP, NULL, L"ViewerTest");

	GX_CONTEXT& gx = Viewer.gl();
	GL_CONTEXT& gl = Viewer.gl;

	gxActivateVerticalSync(true);
	Viewer.InitShaders();
	GX_RGBA_t ClearColor = {1.0,0.0,1.0,1.0};
	gx.SetClearColor(ClearColor);
	//---
  Viewer.OnLoop();
	//---
	Viewer.ReleaseShaders();
	//---
	return 0;
}
//-------------------------------------------------------------------------------------------------------------