//Version number
#version 400

//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;

//Uniform variable
uniform sampler2D diffuse;

in vec2 TexCoords;
 
void main()
{
    fragcolor = texture2D(diffuse, TexCoords);
}