#version 330 core

in  vec4 vPosition;
in  vec4 vColor;
in  vec4 vNormal;

out vec4 color;
out vec3 N;
out vec3 V;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 shadowMatrix;
uniform vec4 draw_color;
uniform int isShadow;

void main() 
{
    //color = vColor;
	color = draw_color;
	vec4 t = Projection * ModelView * vPosition;
    //gl_Position = vec4(t.xyz / t.w, 1.0);

	// 判断是否是阴影
	if (isShadow == 1.0)
	{
		color = vec4(0,0,0,1.0);
		gl_Position = vec4(t.xyz / t.w, 1.0);
	}
	else
	{
		gl_Position = t;
		// TODO 将顶点变换到相机坐标系下
		vec4 vertPos_cameraspace = ModelView * vPosition;
	
		// TODO 将法向量变换到相机坐标系下并传入片元着色器
		N = (ModelView * vNormal).xyz;
	
		// 对顶点坐标做透视投影
		V = vertPos_cameraspace.xyz / vertPos_cameraspace.w;
	}	
} 
