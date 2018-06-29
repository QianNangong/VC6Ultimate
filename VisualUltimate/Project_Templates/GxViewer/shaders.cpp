#include <GLSLShaderData.h>
#define NULL 0
namespace GLSL{
	//-----------------------------------------
	char const _Default_Vs_[]=
		"#version 150\r\n" // 2
		"in vec2 a_Pos;\r\n" // 3
		"void main(){\r\n" // 4
		"	gl_Position = vec4(a_Pos, 0, 1);\r\n" // 5
		"}\r\n" // 6
	;
	//-----------------------------------------
	char const _Default_Fs_[]=
		"void main(void){\r\n" // 12
		"	if(gl_FragCoord.x>400.0)\r\n" // 13
		"		gl_FragColor = vec4(1.0,1.0,0.0,1.0);\r\n" // 14
		"	else\r\n" // 15
		"		gl_FragColor = vec4(1.0,0.0,0.0,1.0);\r\n" // 16
		"}\r\n" // 17
	;
	//-----------------------------------------
	;
	//-----------------------------------------
#if 0
#	include "./Default.glsl"
#endif
	GLSL::CREATE_SHADER_DATA Default={
	"Default",
		_Default_Vs_,
		NULL,
		NULL,
		NULL,
		_Default_Fs_,
		NULL,
		0,0,0,
		"C:\\temprd\\_Default_Vs_.glsl",
		NULL,
		NULL,
		NULL,
		"C:\\temprd\\_Default_Fs_.glsl",
		NULL,
	};
	//-----------------------------------------
}
//-------------------------
