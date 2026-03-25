#version 330

uniform float u_Time;

in vec3 a_Position;
in float a_Mass;
in vec2 a_Vel;
in float a_RV;
in float a_RV1;
in float a_RV2;

//out float v_Grey;
out vec3 v_Color;

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

float pseudoRandom(float n) {
	return fract(sin(n) * 43758.5453);
}

void Particle2(){
	// emitTime
	float newTime = u_Time - a_RV1;

	if(newTime > 0.0){
		float t = mod(newTime, 1.0); //newtime을 1로 나눈 나머지
		float tt = t * t;

		float vx = a_Vel.x/10;
		float vy = a_Vel.y/10;

		float initPosX = a_Position.x;
		float initPosY = a_Position.y;

		vec4 newPos = vec4(0.0);
		newPos.x = initPosX + vx * t + 0.5 * C_G.x * tt;
		newPos.y = initPosY + vy * t + 0.5 * C_G.y * tt;
		newPos.z = 0.0;
		newPos.w = 1.0;

		gl_Position = newPos;
		gl_PointSize = 5.0;
	}
	else{
		gl_Position = vec4(-10.0, -10.0, 0.0, 1.0);
		gl_PointSize = 0.0;
	}
    gl_PointSize = 5.0;
}

void Particle3(){
	// emitTime
	float life = a_RV2+0.5;
    //float newTime = u_Time + a_RV1*life;
	float newTime = u_Time - a_RV1;

if(newTime > 0.0){
		//float scale = a_RV;
		float t = mod(newTime, life);
		float tt = t * t;
		float scale = pseudoRandom(a_RV1)*(life-t)/life;
		float initPosX = a_Position.x*scale + sin(a_RV * 2.0 * C_PI);
		float initPosY = a_Position.y*scale + cos(a_RV * 2.0 * C_PI);

		float vx = a_Vel.x/10;
		float vy = a_Vel.y/10;

		vec4 newPos = vec4(0.0);
		newPos.x = initPosX + vx * t;
		newPos.y = initPosY + vy * t + 0.5 * C_G.y * 0.08 * tt;
		newPos.z = 0.0;
		newPos.w = 1.0;

		gl_Position = newPos;
		gl_PointSize = 2.0;
	}
	else{
		gl_Position = vec4(-10.0, -10.0, 0.0, 1.0);
		gl_PointSize = 0.0;
	}
}

void Thrust(){
	//float life = a_RV2+0.5; 
	//float newTime = u_Time + a_RV1*life;
	float newTime = u_Time - a_RV1;
	if(newTime>0.0){
		float t = mod(newTime,1.0); 
		float ampscale = t*0.5;	//0.5-t*0.5; - 0.5 -> 0
		float amp = 2*(a_RV-0.5)*ampscale;
		float period = a_RV2;
		float sizeScale = t*2; //2-t*2;  - 2 -> 0
		vec4 newPosition = vec4(a_Position, 1.0);

		newPosition.x = a_Position.x*sizeScale -1 + t*2; // -1~1
		newPosition.y = a_Position.y*sizeScale + sin(t*2*C_PI*period)*amp;

		newPosition.z = a_Position.z;
		gl_Position = newPosition;
		gl_PointSize = 5.0;
		//v_Grey = 1.0 - t; //시간이 지날수록 어두워짐

		vec3 red    = vec3(1.0, 0.1, 0.0);
        vec3 orange = vec3(1.0, 0.45, 0.0);
        vec3 yellow = vec3(1.0, 0.9, 0.1);

        if(t < 0.5){
            float k = t / 0.5;
            v_Color = mix(red, orange, k);
        }
        else{
            float k = (t - 0.5) / 0.5;
            v_Color = mix(orange, yellow, k);
        }

	}else{
		gl_Position = vec4(-10.0, -10.0, 0.0, 1.0);
		gl_PointSize = 0.0;
		//v_Grey = 0.0;
		v_Color = vec3(0.0, 0.0, 0.0);
	}
}

void main()
{
	Thrust();
}
