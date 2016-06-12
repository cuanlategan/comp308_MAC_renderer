#version 120

uniform samplerCube gCubemapTexture;
varying vec2 vTextureCoord0;
varying vec3 vNormal;

void main (void)
{

      vec4 ambiColor = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;
      vec4 total = ambiColor;

	  gl_FragColor = texture2D(gCubemapTexture, vTextureCoord0.xy);

}

/*
#version 330

in vec3 TexCoord0;

out vec4 FragColor;

uniform samplerCube gCubemapTexture;

void main()
{
    FragColor = texture(gCubemapTexture, TexCoord0);
}
*/