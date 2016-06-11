#version 120

uniform sampler2D tex;
varying vec2 vTextureCoord0;
varying vec3 vNormal;

void main (void)
{

      vec4 ambiColor = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;
      vec4 total = ambiColor;

	  gl_FragColor = texture2D(tex, vTextureCoord0.xy) /** ( gl_FrontLightModelProduct.sceneColor + total)*/;

}
