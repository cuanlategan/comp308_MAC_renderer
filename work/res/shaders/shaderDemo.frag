//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#version 120

// Constant across both shaders
uniform sampler2D texture0;

uniform float amplitude;

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;

void main() {

	// Can do all sorts of cool stuff here

	vec3 color = texture2D(texture0, vTextureCoord0).rgb;
	gl_FragColor = vec4(0,color.g, 0,1);

	//float color = texture2D(texture0, vTextureCoord0).r;
	//float dis = amplitude * color;
	// IMPORTANT tell OpenGL what the final color of the fragment is (vec4)

	//if (dis > 0.1) {
	//	gl_FragColor = vec4(1,1,1,1);
	//} else {
	//	gl_FragColor = vec4(color, color, color, 1);
	//}
}