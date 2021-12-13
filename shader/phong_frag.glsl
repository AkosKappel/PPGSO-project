#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

uniform sampler2D shadowMap;

// Direction of light
uniform vec3 LightDirection;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

in vec4 FragPosLightSpace;

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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005) - 0.005f;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0){
        shadow = 0.0;
    }
    return shadow;
}

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

  float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

  return (ambient + (1.0 - shadow) * (diffuse + specular));
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

  ambient *= intensity;
  diffuse  *= intensity;
  specular *= intensity;

  return ambient + diffuse + specular;
}

#define NR_POINT_LIGHTS 3
#define NR_SPOT_LIGHTS 5

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform bool isOutside;

void main() {
  vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lightStrength = vec3(0);

    if (isOutside) {
        lightStrength += calculateDirectionalLight(directionalLight, normal, viewDir, material);
    } else {
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            lightStrength +=  calculatePointLight(pointLights[i], normal, FragPos, viewDir, material);
        }

        for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
            lightStrength += calculateSpotLight(spotLights[i], normal, FragPos, viewDir, material);
        }
    }

  vec3 result = lightStrength * vec3(texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset));
  FragmentColor = vec4(result, 1.0);
}
