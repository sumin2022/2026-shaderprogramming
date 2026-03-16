#version 330

uniform float u_Time;

in vec3 a_Position;

void Basic(){
	float t = mod(u_Time*10,1.0);
	vec4 newPosition;
	newPosition.x = a_Position.x;
	newPosition.y = a_Position.y + t * 0.5;
	newPosition.z = a_Position.z;
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

void main()
{
	Cycle2();
}
