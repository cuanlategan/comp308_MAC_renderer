#version 120


attribute float height;

uniform float wavelength, amplitude, steepnes, speed;
uniform vec2 direction;
uniform float time;


//float wavelength = 4.567;
//float amplitude = 0.3;
//float steepnes = 0.8;
//float speed = 1.5;

const float PI = 3.14159265359;
vec4 calcGerstnerWave(float frequency, float QA,   vec2 direction,float phase_const);
void main(void)
{
   gl_TexCoord[0] = gl_MultiTexCoord0;


   if(gl_TexCoord[0].y > .9){
      gl_Position = ftransform();
   }/*else {
      vec4 p;
      p.x = gl_Vertex.x;
      p.y = gl_Vertex.y;
      p.z = gl_Vertex.z + sin(time);
      p.w = 1.0;
      gl_Position = gl_ModelViewProjectionMatrix * p;
   }*/ else {
      //vec2 dir = normalize(direction);
      vec2 dir = vec2(-0.1,1.0);
      float freq = 2*PI/wavelength;
      float phase_con = freq*speed;
      float QA = steepnes*amplitude;
      vec4 result = calcGerstnerWave(freq, QA,dir,phase_con);

      gl_Position = gl_ModelViewProjectionMatrix * (result* 0.1 +gl_Vertex);
   }



}

vec4 calcGerstnerWave(float frequency, float QA,   vec2 direction,float phase_const)
    {

        vec2 dir = normalize(direction);

        float wave_phase = frequency * dot(dir,gl_Vertex.xy) + (time*phase_const);

        float c = cos(wave_phase);
        float s = sin(wave_phase);

        return  vec4(QA*gl_Vertex.x*c, QA*gl_Vertex.y*c, amplitude*s, 0.0);
    }
