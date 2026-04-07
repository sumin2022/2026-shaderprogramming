#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;
const float C_PI = 3.141592;
const vec4 C_Points[2] = vec4[2](vec4(0.5,0.5,0,0.5),vec4(0.5,0.7,0.5,1));

uniform float u_Time;
uniform vec4 u_Points[500];

void Simple()
{
	if(v_Tex.x < 0.5)
		FragColor = vec4(0,0,0,1);
	else
		FragColor = vec4(1,1,1,1);
	//FragColor = vec4(v_Tex,0,1);
	//FragColor = vec4(sin(v_Tex.x*10*3.141592));
}

//시험 예상 문제
void Line()
{
	float trans = C_PI/2;
	float periodx =( v_Tex.x*2*C_PI - trans)*5;
	float periody = (v_Tex.y*2*C_PI - trans)*5;
	float valuex =  pow(abs(sin(periodx)),16);
	float valuey = pow(abs(sin(periody)),16);
	FragColor = vec4(max(valuex,valuey));
}

void Circle()
{
	vec2 center = vec2(0.5,0.5);
	vec2 currPos = v_Tex;
	float d = distance(center,currPos);
	float width = 0.01;
	float raidus = 0.5;
	
	if(d>raidus-width&&d<raidus)
		FragColor = vec4(1);
	else
		FragColor = vec4(0);
}

void Circles()
{
	vec2 center = vec2(0.5,0.5);
	vec2 currPos = v_Tex;
	float d = distance(center,currPos);
	float count = 5;
	float grey = pow(
		abs(sin(d*4*C_PI*count-u_Time*10)),
		8);

	FragColor = vec4(grey);
}

void RainDrop()
{
	float accum;
	for(int i=0;i<500;++i){
		float sTime = u_Points[i].z;
		float lTime = u_Points[i].w;
		float newTime = u_Time-sTime; //0~1
		if(newTime>0)
		{//lTime으로 fract를 바꿔서 빗방울을 좀더 자연스럽게 해준다.
			float t = fract(newTime/lTime);
			float oneMinus = 1-t;
			t = t*lTime;
			vec2 center = u_Points[i].xy;
			vec2 currPos = v_Tex;
			float count = 5;
			float range = t/5;

			float d = distance(center,currPos);
			float fade = 10*clamp(range-d,0,1);
			float grey = pow(
				abs(sin(d*4*C_PI*count-t*20)),
				8);

			accum += grey*fade*oneMinus;
		}
	}
	FragColor = vec4(accum);
}

void main()
{
	RainDrop();
}
