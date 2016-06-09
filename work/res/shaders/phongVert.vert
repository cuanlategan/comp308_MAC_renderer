#version 120


varying vec2 vTextureCoord0;

attribute vec3 attr_center;

uniform float wavelength, amplitude, steepnes, speed, dirX, dirY;
//uniform vec2 direction;
uniform float time;

const float PI = 3.14159265359;

vec4 calcGerstnerWave(float frequency, float QA,   vec2 direction,float phase_const);
vec4 calcGerstnerWave(float frequency, float QA, float amplitude, vec3 position, vec2 direction,
                                 float time, float phase_const);

void main(void)
{
    //gl_TexCoord[0] = gl_MultiTexCoord0;
    vTextureCoord0 = gl_MultiTexCoord0.xy;


   if(gl_MultiTexCoord0.y > .9)
   {
      gl_Position = ftransform();
   } else
   {

      vec2 dir = vec2(dirX,dirY);

      float freq = 2*PI/wavelength;
      float phase_con = freq*speed;
      float QA = steepnes*amplitude;

      //vec4 result = calcGerstnerWave(freq, QA,dir,phase_con);
      vec4 result = calcGerstnerWave(freq, QA,amplitude,attr_center, dir, time, phase_con);

      gl_Position = gl_ModelViewProjectionMatrix * ((result*0.1) +gl_Vertex);
      //gl_Position = gl_ModelViewProjectionMatrix * (result +gl_Vertex);

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



vec4 calcGerstnerWave(float frequency, float QA,   vec2 direction,float phase_const)
    {


        vec3 test = attr_center;

        vec2 dir = normalize(direction);
        //vec2 dir = direction;

        float wave_phase = frequency * dot(dir,test.xy) + (time*phase_const);

        float c = cos(wave_phase);
        float s = sin(wave_phase);

        return  vec4(QA*gl_Vertex.x*c, QA*gl_Vertex.y*c, amplitude*s, 0.0);
    }
