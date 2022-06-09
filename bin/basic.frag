#version 330 core

uniform vec4 lightColor;

out vec4 fragColor;

void main() {
   fragColor = lightColor;
};