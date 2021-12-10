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
in vec3 normal;

in vec3 FragPos;

// The final color
out vec4 FragmentColor;

// camera position
uniform vec3 viewPos;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material material;

struct DirectionalLight {
  vec3 position;
  vec3 color;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform DirectionalLight directionalLight;

struct PointLight {
  vec3 position;
  vec3 color;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};
uniform PointLight pointLight;

struct SpotLight {
  vec3 position;
  vec3  direction;
  vec3 color;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform SpotLight spotLight;

vec3 calculatePointLight(PointLight light, vec3 viewPos, vec3 FragPos, Material material, vec3 normal) {
  vec3 ambient = material.ambient * light.color * light.ambient;

  vec3 lightDir = normalize(light.position - FragPos);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);

  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.color * (diff * material.diffuse) * light.diffuse;

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.color * (material.specular * spec) * light.specular;

  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 viewPos, vec3 FragPos, Material material, vec3 normal) {
  vec3 lightDir = normalize(light.position - FragPos);
  float theta = dot(lightDir, normalize(-light.direction));

  if (theta > light.outerCutOff)
  {
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = material.ambient * light.color * light.ambient;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse) * light.diffuse;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * (material.specular * spec) * light.specular;

    // we'll leave ambient unaffected so we always have a little light.
    diffuse  *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
  }
  return vec3(0);
}

void main() {

  vec3 lightStrength = calculatePointLight(pointLight, viewPos, FragPos, material, normal);
  lightStrength += calculateSpotLight(spotLight, viewPos, FragPos, material, normal);

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  vec3 result =  lightStrength * vec3(texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset));
  FragmentColor = vec4(result, 1.0);


//  // Compute ambient lighting
//  float ambientStrength = 0.1;
////  float ambient = ambientStrength;
//  //  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
//    vec3 ambient = ambientStrength * light.color;
//
//  // Compute diffuse lighting
//  vec3 lightDir = normalize(lightPos - FragPos);
//  float diff = max(dot(normal, vec4(lightDir, 1.0f)), 0.0f);
////  float diffuse = max(dot(normal, vec4(normalize(LightDirection), 1.0f)), 0.0f);
//  vec3 diffuse = diff * light.color;
//
//  // Compute specular lighting
//  float specularStrength = 0.3;
//  vec3 viewDir = normalize(viewPos - FragPos);
//  vec3 reflectDir = reflect(-lightDir, vec3(normal));
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
////  float specular = specularStrength * spec;
//    vec3 specular = specularStrength * spec * light.color;
//
//  // Lookup the color in Texture on coordinates given by texCoord
//  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
//  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * (ambient + diffuse + specular);
//  FragmentColor.a = Transparency;
}
