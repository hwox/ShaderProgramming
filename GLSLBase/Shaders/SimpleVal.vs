#version 450

// attributes. 
// in 말고 attribute로 바꿔도 됨 
in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp;
in float a_value;

in vec4 a_Color;



// varying --> fragment shader input
out vec4 v_Color;
out vec2 v_OriXY; 
out float v_Radius;


uniform float u_Time;

const float PI = 3.141592;
// 행렬 opengl은 행렬일 세로부터 들어간다.
const mat3 c_RP = mat3(0,-1,0,1,0,0,0,0,0);
const vec3 c_Gravity = vec3(0,-30,0); // 중력가속도



void main()
{
	vec3 newPos = a_Position.xyz;
	 v_OriXY = a_Position.xy;
	 v_Radius = abs(a_Position.x);
	vec3 newVel = a_Vel.xyz;
	float start = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w; // 폭
	vec4 newColor = a_Color.xyzw;

	float newAlpha;
	float newVal = a_value;

	float newTime = u_Time - start;
	if(newTime > 0 ){
		newPos.x += sin(a_value*PI*2);
		newPos.y += cos(a_value*PI*2);
		newTime = mod(newTime, lifeTime);
		newVel = newVel + c_Gravity * newTime;
		newPos = newPos + a_Vel * newTime + 0.5*c_Gravity*newTime*newTime;
		vec3 vsin = a_Vel*c_RP;
		
		newAlpha  = pow(1- newTime/lifeTime,9);
		
	
	}
	else {
		newPos = vec3(10000,10000,10000);
	}

	v_Color = a_Color;
	v_Color.a = newAlpha;
	gl_Position = vec4(newPos.xyz , 1);

}

