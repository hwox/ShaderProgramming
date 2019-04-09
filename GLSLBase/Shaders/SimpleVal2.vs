#version 450



in vec3 a_Position;
in vec3 a_Vel;//  속도 받아오기 
in vec2 a_StartLife;

uniform float u_Time; // 0..... 예상되는 값은 0에서부터 증가하는 값
uniform bool u_Repeat; //
const vec3 c_Gravity = vec3(0,-5,0);


void main()
{
	vec3 newPos = a_Position.xyz;



/*
	
	float startTime = a_StartLife.x;
	float lifeTime= a_StartLife.y;
	//float newTime = fract(u_Time); // fract 소숫점 부분만 떼어내는 것. 그러면 얘는  0~1 사이를 반복.
	float newTime = u_Time - startTime; // 음수면 생성이 안된거


	// 그러니깡 ㅑㅇ수일때만 그려야 함

	if(newTime>0){
	
		float life = newTime;
		float remainingLife = lifeTime - life;

		if(u_Repeat == true){
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}
		
		if(remainingLife < 0){
			newPos = vec3(10000,10000,10000); // 다른데로 날려버려

		}
		else{
			newPos += a_Vel*newTime+0.5*c_Gravity*newTime*newTime;
		}

	
	}
	else{
		newPos = vec3(10000,10000,10000); // 다른데로 날려버려
	}



	// 버텍스당 플롯포인ㅌ의 갯수가 8개로 바뀜 (원래 6개에서)
	// 왜? 지금 추가한 두개가 들어가야되니까

	//newPos += a_Vel * newTime; // 둘다 vec3니까 
	*/

	gl_Position = vec4(newPos , 1);
}


