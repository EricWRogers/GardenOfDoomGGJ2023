#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D specular;
    sampler2D diffuse;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 color;
uniform int numDirLights;
uniform DirLight dirLight;
uniform Material material;



float ShadowCalculation(vec4 fragPosLightSpace);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{           
    // properties
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 dirResult;
    if (numDirLights > 0)
        dirResult = CalcDirLight(dirLight, norm, viewDir);
    
    vec3 result = dirResult;
    float gamma = 2.2;
    result = pow(result, vec3(1.0/gamma));

    // alpha
    float alpha = min(color.a, texture(material.diffuse, fs_in.TexCoords).a);
    
    FragColor = vec4(result, alpha);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.002);
    //float bias = 0.001;
    // check whether current frag pos is in shadow
    //return currentDepth - 0.0000000001 > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * (vec3(color) * vec3(texture(material.diffuse, fs_in.TexCoords)));
    //vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * (vec3(color) * vec3(texture(material.diffuse, fs_in.TexCoords)));
    //vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    //vec3 specular = light.specular * spec * material.specular;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
    //return (ambient + diffuse + specular);
}