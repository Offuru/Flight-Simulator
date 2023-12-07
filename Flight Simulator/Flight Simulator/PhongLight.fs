#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;


uniform float Ka; //ambience constant
uniform float Kd; //diffusion constant
uniform float Ks; //specular constant
uniform float spec;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = Kd * max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 specular = Ks * lightColor * pow(max(dot(viewDir, reflectDir), 0.0), spec);

    FragColor = vec4((Ka + diffuse + specular) * objectColor, 1.0);

} 