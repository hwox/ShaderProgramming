#version 450

layout(location=0) out vec4 FragColor;


in vec4 v_Color;
in vec2 v_OriXY; 
in float v_Radius;



void main(){
	
	vec4 newColor;
	if(sqrt(v_OriXY.x * v_OriXY.x + v_OriXY.y*v_OriXY.y) < v_Radius){
		newColor = v_Color;
	}
	else{
		newColor = vec4(0,0,0,0);
	}


	FragColor = newColor;

}