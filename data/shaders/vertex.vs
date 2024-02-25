#version 330 

uniform mat4 o2w; // object to world 
uniform mat4 w2c; // world to clip 
uniform vec3 lightDirection; // world space

layout (location = 0) in vec3 vertexPosition; // object space
layout (location = 1) in vec3 vertexNormal;

out vec4 vertexColour; 

void main() { 
   gl_Position = w2c * o2w * vec4(vertexPosition, 1.0); 
   
   vec3 clipLight = -1.0 * normalize(vec3(w2c * vec4(lightDirection, 0.0)));
   vec3 clipNormal = normalize(vec3(w2c * o2w * vec4(vertexNormal, 0.0)));
   float brightness = dot(clipNormal, lightDirection);

   if(brightness < 0.1)
      brightness = 0.1;

   vertexColour = brightness * vec4(1.0, 1.0, 1.0, 1.0);
} 