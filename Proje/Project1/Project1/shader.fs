#version 460

in vec3 Color;
in vec3 FragPos;
in vec3 v_normal;

out vec4 FragColor;

void main()
{
  vec3 ambient = vec3(0.5f, 0.5f, 0.5f); // color - grey

  // diffuse (lambertian) lighting
  // lightColor, lightSource, normal, diffuseStrength
  vec3 normal =normalize( v_normal);
  vec3 lightColor = vec3(1.0f,1.0f, 1.0f); // color - white
  vec3 lightSource = vec3(0.0f, 6.0f, 0.0f); // coord - (1, 0, 0)
  vec3 lightDir = normalize(lightSource - FragPos); 
  float diffuseStrength = max(0.0, dot(lightDir, normal));
  vec3 diffuse = diffuseStrength * lightColor;

  // specular light
  // lightColor, lightSource, normal, specularStrength, viewSource
  vec3 cameraSource = vec3(3.0f, 3.0f, 3.0f);
  vec3 viewSource = normalize(cameraSource);

  vec3 reflectSource = normalize(reflect(-lightDir, normal));
  float specularStrength = max(0.0, dot(viewSource, reflectSource));
  specularStrength = pow(specularStrength, 64.0);
  vec3 specular = specularStrength * lightColor;

  // lighting = ambient + diffuse + specular
  vec3 lighting = vec3(1.0f, 0.0f, 0.0f); // color - black
  // lighting = ambient;
  //lighting = ambient * 0.0 + diffuse;
  //lighting = ambient * 0.0 + diffuse * 0.0 + specular*5000.0f;
  lighting = ambient * 0.4f + diffuse * 0.5f + specular * 0.5f;

  // color = modelColor * lighting
  vec3 modelColor = Color;
  vec3 color1 = modelColor * lighting;

  //gl_FragColor = vec4(color, 1.0);
    FragColor = vec4(color1,1.0f);
}
