#version 450

uniform sampler2D u_Texture;

out vec4 FragColor;
in vec2 v_Tex;


uniform int[3] u_Number;
void main()
{

vec2 newTex = v_Tex;

// x_tex coord??
newTex.x  = fract(newTex.x * 3.0);

// cal index
int newIndex = int(floor(v_Tex.x*3.0));

float newy = 9.0 - u_Number[newIndex];
newTex.y = newTex.y /10.0 + newy/10.0;
vec4 newColor = texture(u_Texture, newTex);
FragColor = newColor;


}
 
  /* �̷����ϸ� 9�� ũ�� ����
    vec2 newTex = v_Tex;
   newTex.y = newTex.y / 10.0; 
   vec4 newColor = texture(u_Texture, newTex);
   FragColor = newColor;


      newTex.y = newTex.y / 10.0 + 1.0/ 10.0; 

	  �̷��� offset�� 1/10��ŭ �÷��ָ� 8,
	  2/10 ��ŭ �÷��ָ� 7 �̷���..
   */