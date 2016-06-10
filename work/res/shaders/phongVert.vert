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
      vec4 result = calcGerstnerWave(freq, QA,amplitude,attr_center, dir, time, phase_con);

      dir = vec2(dirX+0.3,dirY+0.3);
      freq = 2*PI/(wavelength*.7);
      phase_con = freq*speed;
      QA = (steepnes+.2)*(amplitude+0.3);
      result += calcGerstnerWave(freq, QA,amplitude,attr_center, dir, time, phase_con);

      dir = vec2(dirY-0.333,dirX-0.333);
      freq = 2*PI/(wavelength*1.3);
      phase_con = freq*speed;
      QA = (steepnes-.23)*(amplitude-0.3);
      result += calcGerstnerWave(freq, QA,amplitude,attr_center, dir, time, phase_con);

      result /= 3;





      gl_Position = gl_ModelViewProjectionMatrix * ((result*0.3) +gl_Vertex);
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



