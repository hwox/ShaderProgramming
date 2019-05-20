#version 450

uniform sampler2D u_Texture;
out vec4 FragColor;

in vec2 v_Tex;


uniform float u_Resolx;
uniform float u_Resoly;
uniform float u_Number;

void main()
{

vec2 newTex;
float x_index = int(floor(fract(u_Number / u_Resolx)*u_Resolx));
float y_index = int(floor(floor(u_Number / u_Resolx)));

newTex.x = v_Tex.x/u_Resolx + x_index/u_Resolx;
newTex.y = v_Tex.y/u_Resoly + y_index/u_Resoly;


vec4 newColor = texture(u_Texture, newTex);
FragColor = newColor;


}
