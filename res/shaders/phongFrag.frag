#version 120

uniform sampler2D tex;


void main (void)
{


	  gl_FragColor = texture2D(tex, gl_TexCoord[0].st)/* * ( gl_FrontLightModelProduct.sceneColor + total)*/;

}
