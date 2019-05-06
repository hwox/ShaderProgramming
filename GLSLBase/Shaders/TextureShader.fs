#version 450

uniform sampler2D u_Texture;
out vec4 FragColor;
//uniform sampler2D uTexSampler;
in vec2 v_Tex;

uniform float u_Time;

void main()
{
	// default
	/*
	vec4 newColor = vec4(newTex, 0, 1);
	vec2 newTex =abs(v_Tex - vec2(0, 0.5)) + vec2(0, 0.5) ;
	newColor = vec4(newTex, 0, 1);
	
	FragColor = newColor;
	*/
	// simple mean blur

	/*
	vec2 newTex[5];
	vec4 newColor[5];
	newTex[0] = vec2(v_Tex.x, v_Tex.y);
	newTex[1] = vec2(v_Tex.x - 1.0/8.0, v_Tex.y);
	newTex[2] = vec2(v_Tex.x + 1.0/8.0, v_Tex.y);
	newTex[3] = vec2(v_Tex.x, v_Tex.y- 1/4.0);
	newTex[4] = vec2(v_Tex.x, v_Tex.y+ 1/4.0);


	newColor[0] = texture(u_Texture, newTex[0]);
	newColor[1] = texture(u_Texture, newTex[1]);
	newColor[2] = texture(u_Texture, newTex[2]);
	newColor[3] = texture(u_Texture, newTex[3]);
	newColor[4] = texture(u_Texture, newTex[4]);

	vec4 finalColor = newColor[0]+newColor[1]+newColor[2]+newColor[3]+newColor[4];
	finalColor /= 5;
	FragColor = finalColor;
	*/


	vec2 newTex;
	newTex.x = v_Tex.x;;
	newTex.y = (2-floor(v_Tex.y * 3.0)); 
	newTex.y += ((v_Tex.y) / 3.0);

	vec4 newColor;

	newColor = texture(u_Texture, newTex);
	FragColor = newColor;


}
 
 /*
 
    vec4 newColor = vec4(0);

   // v_tex  : x 0~1 . --> 0~2 PI

   // 흐르는 곡선 하려면 x값만 증가시켜주면 돼! 
   // sin 함수 곡선에 x축으로 translation 되는 거-> sin곡선이 움직이는것처럼 보이는거

   float x = v_Tex.x *2 * 3.14 + u_Time;
   float sinResult = sin(x);

   if(sinResult > v_Tex.y && sinResult - 0.01 < v_Tex.y){
   	   newColor = vec4(1);
   }

   FragColor = newColor;
 
 */