#version 330 core

in vec3 fragColor;
flat in int faceID;

out vec4 FragColor;

void main()
{
    // Flat shading with bright lighting
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    
    // Approximate face normals based on faceID
    vec3 faceNormal;
    if (faceID == 0 || faceID == 1) faceNormal = vec3(0.0, 0.0, -1.0);
    else if (faceID == 2 || faceID == 3) faceNormal = vec3(0.0, 0.0, 1.0);
    else if (faceID == 4 || faceID == 5) faceNormal = vec3(-1.0, 0.0, 0.0);
    else if (faceID == 6 || faceID == 7) faceNormal = vec3(1.0, 0.0, 0.0);
    else if (faceID == 8 || faceID == 9) faceNormal = vec3(0.0, -1.0, 0.0);
    else faceNormal = vec3(0.0, 1.0, 0.0);
    
    float diff = max(dot(faceNormal, lightDir), 0.2);
    vec3 lit = fragColor * (diff + 0.5);  // Added ambient light (0.5)
    FragColor = vec4(lit, 1.0);
}
