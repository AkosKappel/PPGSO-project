#version 330
in vec3 vertexColor;

// The final color
out vec4 FragmentColor;

// Additional overall color when not using per-vertex Color input
uniform vec3 OverallColor;

// Transparency
uniform float Transparency;

void main() {
  // Just pass the color to the output
  FragmentColor = vec4(vertexColor + OverallColor, Transparency);
}
