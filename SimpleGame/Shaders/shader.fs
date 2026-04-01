#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

void main()
{
	if(v_Tex.x < 0.5)
		FragColor = vec4(0,0,0,1);
	else
		FragColor = vec4(1,1,1,1);
	//FragColor = vec4(v_Tex,0,1);
	FragColor = vec4(sin(v_Tex.x*20*3.141592));
}
