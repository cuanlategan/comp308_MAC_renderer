#version 120

//varying vec3 t;
//varying vec3 b;

varying vec3 normal, spotLightDir, pointLightDir;
varying vec3 dirLightDir, eyeVec;


void main(void)
{
   normal = normalize(gl_NormalMatrix * gl_Normal);

   vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
   eyeVec = -vVertex;



   spotLightDir = vec3(gl_LightSource[3].position.xyz - vVertex);
   dirLightDir = vec3(gl_LightSource[2].position.xyz);
   pointLightDir = vec3(gl_LightSource[3].position.xyz - vVertex);

   //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_Position = ftransform();

   gl_TexCoord[0] = gl_MultiTexCoord0;
}