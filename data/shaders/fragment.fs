#version 330 
out mediump vec4 fragmentColour; 
in vec4 vertexColour; 
void main() { 
   fragmentColour = vertexColour; 
} 