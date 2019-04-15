#version 450

layout(location=0) out vec4 FragColor;


in vec4 v_Color;

uniform float u_Time;      //서서히 증가하는 time
uniform vec2 u_Points[5];  //외부에서 입력받도록. 움직이는 좌표

//const vec2 c_Points[2]={vec2(0,0), vec2(0.1,0.1)};

void main(){
   
   vec2 newUV = v_Color.xy - vec2(0.5,0.5); //-0.5~0.5 사이로 xy값이 입력됨
   
   float pointGray = 0;

   
   float distance =length(newUV);  // sqrt(newVec.x * newVec.x + newVec.y * newVec.y)이거를 해주는것
   float newTime = fract(u_Time);
   float ringWidth = 0.1;
   if(distance < newTime +ringWidth && distance > newTime) //반지름의 크기를 ringWidth해서 얘보다 작을경우, 클경우
   {
      float temp = (distance - newTime)/ringWidth;
      pointGray = temp;

      //링 안에 있을때만 point를 밝게 해줘라
      for(int i = 0; i <5; i++)   //포인트 5개
      {
         vec2 newPoint = u_Points[i];
         vec2 newVec = newPoint - newUV;
         float distance = sqrt(newVec.x * newVec.x + newVec.y * newVec.y);

         if(distance <0.1)
         {
            //pointGray += 0.5 * (distance/0.1);
            pointGray += 0.5 * pow((1-(distance/0.1)),5);
         }
      }
   }
   
   
   FragColor = vec4(pointGray);

   //float distance = sqrt(newUV.x * newUV.x + newUV.y * newUV.y);
   //float gray = sin(distance * 3.141592 * 2 * 20);


   //if(distance < 0.5 && distance > 0.48 ){
   //   FragColor = vec4(1);
   //}
   //else{
   //   FragColor = vec4(0);
   //
   //}

   //FragColor = vec4(newUV,0,1);
   //FragColor = vec4(gray);

   //FragColor =  v_Color;

}