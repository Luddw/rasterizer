#shader vertex
#version 430
layout(location=0) in vec3 pos;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 vert_normal;
layout(location=0) uniform mat4 m;
layout(location=1) uniform mat4 vp;
out vec3 fragment_pos;
out vec3 fragment_norm;
out vec2 UV;

void main()
{
	fragment_pos = vec3(m * vec4(pos , 1.0)); // world space vertex
	fragment_norm = inverse(transpose(mat3(m))) * vert_normal; // worldspace normal 
	gl_Position = vp*m*vec4(pos, 1); //vertex pos
	
	UV = uv;
};

#shader fragment
#version 430
layout(location=1) in vec2 UV;
uniform sampler2D text;
uniform vec4 lightcolor;
uniform vec4 lightpos; // the other objects color that is being lit by the lighting.
uniform vec4 intensity;
uniform vec4 camerapos;
in vec3 fragment_pos;
in vec3 fragment_norm;
out vec4 Color;
void main()
{
	const float mipmaplvl = 1.0;
	vec3 textur = texture2D(text,UV,mipmaplvl).xyz;
	textur = vec3(0.1, 0.1, 0.1);


	
	//ambient
	vec3 ambient = intensity.x*lightcolor.xyz;
	//Color = vec4((lightcolor.xyz*textur) , 1.0);



	//diffuse
	vec3 norm = normalize(fragment_norm);
	vec3 lighting_direction = normalize(lightpos.xyz - fragment_pos);
	float diffuse_intensity = max(dot(norm, lighting_direction), 0.0);
	vec3 diffuse = diffuse_intensity * lightcolor.xyz;
	


	//specular  Phong
//	float specular_intensity = 0.5;
//	vec3 view_direction = normalize(camerapos.xyz - fragment_pos);
//	vec3 reflection_direction = reflect(-lighting_direction, norm);
//	float specular_value = pow(max(dot(view_direction, reflection_direction), 0.0), 32); //higher exponent --> smaller highlights



	float specular_intensity = 0.5;
	vec3 view_direction = normalize(camerapos.xyz - fragment_pos);

	vec3 halfway_direction = normalize(lighting_direction + view_direction);

	float specular_value = pow(max(dot(norm, halfway_direction), 0.0), 32); //higher exponent --> smaller highlights

	
	vec3 specular_final =  specular_intensity*specular_value *  lightcolor.xyz;


	vec3 final = (ambient + diffuse + specular_final) * textur;
	Color = vec4(final, 1.0);



};
