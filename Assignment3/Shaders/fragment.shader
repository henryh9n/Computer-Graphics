#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec3 lightPosition;

out vec4 color;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 0.0);
    vec3 objectColor = vec3(0.6, 1.0, 1.0);

    vec3 ambient = 0.5 * lightColor;

    vec3 norm = normalize(fragmentPosition);
    vec3 lightDir = normalize(lightPosition - fragmentPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;

    vec3 result = (ambient + specular + diffuse) * objectColor;
    color = vec4(result, 1.0);
}
