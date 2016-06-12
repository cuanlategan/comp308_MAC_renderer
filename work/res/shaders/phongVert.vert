#version 120


varying vec2 vTextureCoord0;
varying vec3 normal, spotLightDir, pointLightDir;
varying vec3 dirLightDir, eyeVec;

const float cos_inner_cone_angle = 0.99619469809174553229501040247389;

attribute vec3 attr_center;

uniform float time, wavelength, amplitude, steepnes, speed, dirX, dirY;

const float PI = 3.14159265359;

vec4 calcGerstnerWave(float frequency, float QA,   vec2 direction,float phase_const);
vec4 calcGerstnerWave(float frequency, float QA, float amplitude, vec3 position, vec2 direction,
                                 float time, float phase_const);

void main(void)
{

   vTextureCoord0 = gl_MultiTexCoord0.xy;
   normal = normalize(gl_NormalMatrix * gl_Normal);

   vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
   eyeVec = -vVertex;

   spotLightDir = vec3(gl_LightSource[3].position.xyz - vVertex);
   dirLightDir = vec3(gl_LightSource[2].position.xyz);
   pointLightDir = vec3(gl_LightSource[3].position.xyz - vVertex);

   if(gl_MultiTexCoord0.y > .9)
   {
      gl_Position = ftransform();
   } else
   {

      vec2 dir = vec2(dirX,dirY);
      float freq = 2*PI/wavelength;
      float phase_con = freq*speed;
      float QA = steepnes*amplitude;
      vec4 result = calcGerstnerWave(freq, QA,amplitude,attr_center, dir, time, phase_con);

      dir = vec2(dirX*0.77,dirY*0.77);
      freq = 2*PI/(wavelength);
      phase_con = freq*speed;
      QA = (steepnes*0.77)*(amplitude*0.7);
      result += calcGerstnerWave(freq, QA,amplitude,attr_center, dir, time, phase_con);

      dir = vec2(dirX*1.33, dirY*1.33);
      freq = 2*PI/(wavelength);
      phase_con = freq*speed;
      QA = (steepnes*1.33)*(amplitude*1.33);
      result += calcGerstnerWave(freq, QA,amplitude,attr_center, dir, time, phase_con);

      result /= 3;




      gl_Position = gl_ModelViewProjectionMatrix * ((result*.5  +gl_Vertex) );

      vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
         eyeVec = -vVertex;

         spotLightDir = vec3(gl_LightSource[3].position.xyz - vVertex);
         dirLightDir = vec3(gl_LightSource[2].position.xyz);
         pointLightDir = vec3(gl_LightSource[3].position.xyz - vVertex);

   }


}


vec4 calcGerstnerWave(float frequency, float QA, float amplitude, vec3 position, vec2 direction,
                                 float time, float phase_const)
    {


        float wave_phase = frequency * dot(direction.xy,position.xy) + (time*phase_const);

        float c = cos(wave_phase);
        float s = sin(wave_phase);

        return  vec4(QA*direction.x*c, QA*direction.y*c, amplitude*s, 0.0);
    }