#shader vertex
#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
out vec2 UV;
out vec2 POS;
//uniform mat4 MVP;


void main()
{
    gl_Position =  vec4(pos, 1);
    UV = uv;
}

#shader fragment
#version 430
in vec2 UV;
in vec2 POS;
out vec3 color;
uniform sampler2D tex;
void main()
{
    color = texture(tex, UV).rgb;
    //color = vec3(0,0,1);
    //color = vec3(UV.x, UV.y,0);
}