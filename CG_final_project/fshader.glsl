#version 330 core

in vec3 N;
in vec3 V;

in  vec4 color;
out vec4 fColor;
uniform vec3 lightPos;

void main() 
{ 
	// TODO 设置三维物体的材质属性
	vec3 ambiColor = vec3(0.3, 0.3, 0.3);
	vec3 diffColor = vec3(0.6, 0.6, 0.6);
	vec3 specColor = vec3(0.4, 0.4, 0.4);

	// TODO 计算N，L，V，R四个向量并归一化
	vec3 N_norm = normalize(N);
	vec3 L_norm = normalize(lightPos - V);
	vec3 V_norm = normalize(-V);
	vec3 R_norm = reflect(L_norm, N_norm);

	// TODO 计算漫反射系数和镜面反射系数
	float lambertian = clamp(dot(L_norm, N_norm), 0.0, 1.0);
	float specular = clamp(dot(R_norm, V_norm), 0.0, 1.0);
	
	// TODO 计算最终每个片元的输出颜色
 	vec4 fragmentColor = vec4(ambiColor + 
				   diffColor * lambertian +
				   specColor * pow(specular, 5.0), 1.0);

    fColor = color * fragmentColor;	
} 

