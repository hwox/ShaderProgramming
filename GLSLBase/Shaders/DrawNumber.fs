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
 
  /* 이렇게하면 9반 크게 나옴
    vec2 newTex = v_Tex;
   newTex.y = newTex.y / 10.0; 
   vec4 newColor = texture(u_Texture, newTex);
   FragColor = newColor;


      newTex.y = newTex.y / 10.0 + 1.0/ 10.0; 

	  이렇게 offset을 1/10만큼 늘려주면 8,
	  2/10 만큼 늘려주면 7 이런식..
   */