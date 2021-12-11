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
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform DirectionalLight directionalLight;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material material) {
  vec3 lightDir = normalize(-light.direction);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 ambient = light.ambient * material.ambient;
  vec3 diffuse = light.diffuse * diff * material.diffuse;
  vec3 specular = light.specular * spec * material.specular;

  return (ambient + diffuse + specular);
}


struct PointLight {
  vec3 position;
  vec3 color;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform PointLight pointLight;

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, Material material) {
  vec3 lightDir = normalize(light.position - FragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 ambient = light.ambient * light.color * material.ambient;
  vec3 diffuse = light.diffuse * light.color * diff * material.diffuse;
  vec3 specular = light.specular * light.color * spec * material.specular;

  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}


struct SpotLight {
  vec3 position;
  vec3 direction;
  vec3 color;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform SpotLight spotLight;

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir, Material material) {
  vec3 lightDir = normalize(light.position - FragPos);
  float theta = dot(lightDir, normalize(-light.direction));

  float epsilon   = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  vec3 ambient = material.ambient * light.color * light.ambient;

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


//float ShadowCalculation(vec4 fragPosLightSpace)
//{
//  // perform perspective divide
//  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//  // transform to [0,1] range
//  projCoords = projCoords * 0.5 + 0.5;
//  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//  float closestDepth = texture(shadowMap, projCoords.xy).r;
//  // get depth of current fragment from light's perspective
//  float currentDepth = projCoords.z;
//  // check whether current frag pos is in shadow
//  float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
//
//  return shadow;
//}


void main() {
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 lightStrength = calculateDirectionalLight(directionalLight, normal, viewDir, material);

  lightStrength += calculatePointLight(pointLight, normal, FragPos, viewDir, material);

  lightStrength += calculateSpotLight(spotLight, normal, FragPos, viewDir, material);

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  vec3 result = lightStrength * vec3(texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset));
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
