#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 InColor;
layout (location = 2) in vec3 move;
vec3 anormal= vec3(0.0f,  1.0f, 0.0f);


uniform mat4 gWVP;
uniform mat4 gTranslation;

out vec3 Color;

out vec3 v_normal;

out vec3 FragPos;

void main()
{
    if(move.x==1.0f){
    gl_Position = gWVP * gTranslation * vec4(Position, 1.0) ;
    Color = InColor;
    v_normal=anormal;
    FragPos= vec3(gWVP * vec4(Position, 1.0));}

    else{
    gl_Position = gWVP * vec4(Position, 1.0) ;
    Color = InColor;
    v_normal=anormal;
    FragPos= vec3(gWVP * vec4(Position, 1.0));}
    }

