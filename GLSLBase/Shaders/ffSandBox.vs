#version 450

in vec3 a_Position;
in vec2 a_uv;

out vec4 v_Color;

//out vec4 v_Temp;

void main()
{
   gl_Position = vec4(a_Position, 1);


   v_Color = vec4(a_Position.xy,0,1);
   v_Color = vec4(a_Position.xy + vec2(0.5, 0.5),0,1);


}
