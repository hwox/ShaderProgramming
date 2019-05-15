#version 450

uniform sampler2D u_Texture;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
out vec4 FragColor;
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
	/*

	vec2 newTex;
	newTex.x = v_Tex.x;;
	newTex.y = (2-floor(v_Tex.y * 3.0)); 
	newTex.y += ((v_Tex.y) / 3.0);
	newTex.y = v_Tex.y;
	vec4 newColor;
	
	newColor1 = texture(u_Texture, newTex);
	FragColor = vec4(newColor.rgb, 1);
	*/

	// multiple Textures
	vec4 newColor;

	vec2 newTexPos = vec2(v_Tex.x, u_Time/6.0 + v_Tex.y/6.0);
	FragColor = texture(u_Texture, newTexPos);
	// 1.0 - v_Tex.y 해주면 된다.
	/*	
	vec2 newTex = vec2(v_Tex.x, 1.0-v_Tex.y); // 텍스쳐 불러오면 위아래가 반전되는 문제가 생기니까
	if(newTex.x < 0.5){
		if(newTex.y < 0.5){
			newColor = texture(u_Texture1, vec2(newTex.x*2, fract(newTex.y*2)));
		}
		else{
			newColor = texture(u_Texture2, vec2(newTex.x*2, newTex.y));
			}
	} else{
		if(newTex.y < 0.5){
	
		newColor = texture(u_Texture3, vec2(fract(newTex.x*2), fract(newTex.y*2)));
		}
		else{
		newColor = texture(u_Texture, vec2(fract(newTex.x*2), newTex.y));
		}
	}
	FragColor = vec4(newColor);
	*/

	




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