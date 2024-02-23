#version 330 

uniform mat4 o2w; // object to world 
uniform mat4 w2c; // world to clip 
uniform vec3 lightPosition; // world space

layout (location = 0) in vec3 vertexPosition; // object space
layout (location = 1) in vec3 vertexNormal;

out vec4 vertexColour; 

void main() { 
   gl_Position = w2c * o2w * vec4(vertexPosition, 1.0); 
   
   // vec3 lightDirection = normalize(lightPosition - vertexPosition);
   // float brightness = dot(vertexNormal, lightDirection);
   // vertexColour = brightness * vec4(1.0, 1.0, 1.0, 1.0);

   // vertexColour = o2w * vec4(vertexNormal, 0.0);
   vertexColour = vec4(vertexNormal, 0.0);
} 