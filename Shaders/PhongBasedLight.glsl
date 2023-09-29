precision lowp float;

varying vec4 v_normal;

void main() 
{
    // ambient lighting (global illuminance)
    vec3 ambient = vec3(0.6, 0.6, 0.6); // color - grey

    // diffuse (lambertian) lighting
    // lightColor, lightSource, normal, diffuseStrength
    vec3 normal = normalize(v_normal.xyz);
    vec3 lightColor = vec3(1.0, 1.0, 1.0); // color - white
    vec3 lightSource = vec3(1.0, 1.0, 1.0); // coord - (1, 0, 0)
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    vec3 diffuse = diffuseStrength * lightColor;

    // specular light
    // lightColor, lightSource, normal, specularStrength, viewSource
    vec3 cameraSource = vec3(0.1, 0.1, 1.0);
    vec3 viewSource = normalize(cameraSource);
    vec3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    specularStrength = pow(specularStrength, 0.5);
    vec3 specular = specularStrength * lightColor;

    // lighting = ambient + diffuse + specular
    vec3 lighting = vec3(0.3686, 0.3608, 0.3608); // color - black
    // lighting = ambient;
    // lighting = ambient * 0.0 + diffuse;
    // lighting = ambient * 0.0 + diffuse * 0.0 + specular;
    lighting = ambient - .1 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;

    // color = modelColor * lighting
    vec3 modelColor = vec3(0.82, 0.3, 0.3);
    vec3 color = modelColor * lighting;

    gl_FragColor = vec4(color, 1.0);
}