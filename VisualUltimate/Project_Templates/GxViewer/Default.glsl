#pragma shader(vertex) ///!
#version 150
//---------------------------
in vec2 a_Pos;
//---------------------------
void main(){
	gl_Position = vec4(a_Pos, 0, 1);
}
//---------------------------

#pragma shader(fragment) ///!
//------------------------------------------------
void main(void){
	if(gl_FragCoord.x>400.0)
		gl_FragColor = vec4(1.0,1.0,0.0,1.0);
	else
		gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
//------------------------------------------------
