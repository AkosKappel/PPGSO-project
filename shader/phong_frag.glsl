#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform vec3 lightPos;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

in vec3 FragPos;

// The final color
out vec4 FragmentColor;

// camera position
uniform vec3 viewPos;

void main() {
  // Compute ambient lighting
  float ambientStrength = 0.1;
  float ambient = ambientStrength;
  //  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
  //  vec3 ambient = ambientStrength * lightColor;

  // Compute diffuse lighting
  vec3 lightDir = normalize(lightPos - FragPos);
  float diffuse = max(dot(normal, vec4(lightDir, 1.0f)), 0.0f);
//  vec3 diffuse = diff * lightColor;
//  float diffuse = max(dot(normal, vec4(normalize(LightDirection), 1.0f)), 0.0f);

  // Compute specular lighting
  float specularStrength = 0.2;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, vec3(normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  float specular = specularStrength * spec;
  //  vec3 specular = specularStrength * spec * lightColor;

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * (ambient + diffuse + specular);
  FragmentColor.a = Transparency;
}
