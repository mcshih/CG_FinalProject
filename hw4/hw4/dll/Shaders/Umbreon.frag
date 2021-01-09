#version 430

uniform sampler2D Texture;
uniform sampler2D normalMap;

uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 ambientIntensity;
uniform vec3 diffuseIntensity;
uniform vec3 specularIntensity;
uniform float gloss;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

in vec3 f_normal;
in vec4 f_worldPos;
in vec2 texCoords;

out vec4 color;

void main()
{
	//vec3 f_normal = texture(normalMap, texCoords).rgb;
    //f_normal = normalize(f_normal * 2.0 - 1.0);

	// ambient
    vec3 ambient = ambientIntensity * Ka;

    // diffuse
    vec3 norm = normalize(f_normal);
    vec3 lightDir = normalize(lightPos - vec3(f_worldPos[0], f_worldPos[1],f_worldPos[2]));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseIntensity * Kd * diff;

    // specular
    vec3 viewDir = normalize(viewPos - vec3(f_worldPos[0], f_worldPos[1], f_worldPos[2]));
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), gloss);
    vec3 specular = specularIntensity * Ks * spec;  

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f) * texture2D(Texture, texCoords);
	
	//color = texture2D(Texture, texCoords);
} 
