#version 450



in vec3 a_Position;
in vec3 a_Vel;//  �ӵ� �޾ƿ��� 
in vec2 a_StartLife;

uniform float u_Time; // 0..... ����Ǵ� ���� 0�������� �����ϴ� ��
uniform bool u_Repeat; //
const vec3 c_Gravity = vec3(0,-5,0);


void main()
{
	vec3 newPos = a_Position.xyz;



/*
	
	float startTime = a_StartLife.x;
	float lifeTime= a_StartLife.y;
	//float newTime = fract(u_Time); // fract �Ҽ��� �κи� ����� ��. �׷��� ���  0~1 ���̸� �ݺ�.
	float newTime = u_Time - startTime; // ������ ������ �ȵȰ�


	// �׷��ϱ� �������϶��� �׷��� ��

	if(newTime>0){
	
		float life = newTime;
		float remainingLife = lifeTime - life;

		if(u_Repeat == true){
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}
		
		if(remainingLife < 0){
			newPos = vec3(10000,10000,10000); // �ٸ����� ��������

		}
		else{
			newPos += a_Vel*newTime+0.5*c_Gravity*newTime*newTime;
		}

	
	}
	else{
		newPos = vec3(10000,10000,10000); // �ٸ����� ��������
	}



	// ���ؽ��� �÷����Τ��� ������ 8���� �ٲ� (���� 6������)
	// ��? ���� �߰��� �ΰ��� ���ߵǴϱ�

	//newPos += a_Vel * newTime; // �Ѵ� vec3�ϱ� 
	*/

	gl_Position = vec4(newPos , 1);
}


