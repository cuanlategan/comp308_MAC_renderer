#version 120


varying vec2 vTextureCoord0;
varying vec3 vNormal;



void main(void)
{

    vTextureCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * ((result*.5) +gl_Vertex);



}

