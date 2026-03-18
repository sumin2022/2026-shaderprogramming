#version 330

uniform float u_Time;

in vec3 a_Position;
in float a_Mass;
in vec2 a_Vel;
in float a_RV;

const float C_PI = 3.141592;
const vec2 C_G = vec2(0,-9.8);

void Basic(){
    float t = mod(u_Time * 10.0, 1.0);
    vec4 newPosition = vec4(a_Position, 1.0);
    newPosition.y = a_Position.y + t * 0.5;
    gl_Position = newPosition;
}

void Sin1(){
	float t = mod(u_Time*10,1.0); // 0~1
	vec4 newPosition = vec4(a_Position, 1.0);
	newPosition.x = a_Position.x -1 + t*2; // -1~1
	newPosition.y = a_Position.y + sin(t*4*3.141592)*0.5;
	newPosition.z = a_Position.z;
	gl_Position = newPosition;
}

void Cycle(){
	float t = mod(u_Time*10,1.0); // 0~1
	vec4 newPosition = vec4(a_Position, 1.0);
	newPosition.x = a_Position.x + cos(t*2*3.141592);
	newPosition.y = a_Position.y + sin(t*2*3.141592);
	newPosition.z = a_Position.z;
	gl_Position = newPosition;
}

void Cycle2(){
	float t = abs(sin(u_Time * 3.0));   // 0 -> 1 -> 0 반복

    vec4 newPosition = vec4(a_Position, 1.0);
    newPosition.x = a_Position.x;
    newPosition.y = a_Position.y + 0.8 - t * 1.2;   // 위에서 시작해서 아래로 내려감

    gl_Position = newPosition;
}

void Falling(){
	float t = mod(u_Time,1.0);
	vec4 newPos;
	newPos.x = a_Position.x + a_Vel.x*t + 0.5*C_G.x*t*t;
	newPos.y = a_Position.y + a_Vel.y*t + 0.5*C_G.y*t*t;
	newPos.z = 0;
	newPos.w = 1;

	gl_Position = newPos;
}

void Particle(){
	float t = mod(u_Time, 3.0);

    vec4 newPos = vec4(0.0);
    newPos.x = a_Position.x + a_Vel.x * t;
    newPos.y = a_Position.y + a_Vel.y * t + 0.5 * C_G.y * a_Mass * t * t;
    newPos.z = a_Position.z;
    newPos.w = 1.0;

    gl_Position = newPos;
    gl_PointSize = 5.0;
}
void main()
{
	Particle();
}
