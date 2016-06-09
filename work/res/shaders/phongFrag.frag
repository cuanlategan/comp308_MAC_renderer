#version 120

uniform sampler2D tex;
varying vec2 vTextureCoord0;

void main (void)
{


	  gl_FragColor = texture2D(tex, vTextureCoord0.xy)/* * ( gl_FrontLightModelProduct.sceneColor + total)*/;

}
