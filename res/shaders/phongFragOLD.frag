#version 120

uniform sampler2D tex;
uniform int hasTex;

varying vec3 normal, spotLightDir, pointLightDir;
varying vec3 dirLightDir, eyeVec;

const float cos_inner_cone_angle = 0.99619469809174553229501040247389;

void main (void)
{
   vec4 total = vec4(0.f, 0.f, 0.f, 1.f);
   vec4 ambiColor = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;
   vec3 N = normalize(normal);

   // Point light
   vec3 L = normalize(pointLightDir);
   vec3 E = normalize(eyeVec);
   vec3 R = normalize(-reflect(L,N));
   vec4 Idiff = gl_FrontLightProduct[3].diffuse * max(dot(N,L), 0.0);
   vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   Idiff = clamp(Idiff, 0.0, 1.0);
   Ispec = clamp(Ispec, 0.0, 1.0);
   total = ambiColor + Idiff + Ispec;

   //Spot light
   L = normalize(spotLightDir);
   vec3 D = normalize(gl_LightSource[1].spotDirection);

   float cos_cur_angle = dot(-L, D);
   float cos_outer_cone_angle = cos(radians(gl_LightSource[1].spotCutoff));
   float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;
   float spot = 0.0;
   spot = clamp((cos_cur_angle - cos_outer_cone_angle) / cos_inner_minus_outer_angle, 0.0, 1.0);
   float lambert = max(dot(N,L), 0.0);
   if (lambert > 0.0){
      total += clamp(gl_LightSource[1].diffuse * gl_FrontMaterial.diffuse * lambert * spot, 0.0, 1.0);
	  R = normalize(-reflect(L,N));

	  float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
	  total += clamp(gl_LightSource[1].specular * gl_FrontMaterial.specular * specular * spot, 0.0, 1.0);
   }



   // Directional light
   L = normalize(dirLightDir);
   Idiff = gl_FrontLightProduct[2].diffuse * max(dot(N,L), 0.0);
   Ispec = gl_FrontLightProduct[2].specular * pow(max(dot(R,E),0.0),gl_FrontMaterial.shininess);
   total += Idiff + Ispec;

   // draw
   gl_FragColor = gl_FrontLightModelProduct.sceneColor + total;
   if (hasTex == 1){
	  gl_FragColor = texture2D(tex, gl_TexCoord[0].st) * ( gl_FrontLightModelProduct.sceneColor + total);
   }
}
