#version 450 core


in vec3 FragPos;  
in vec3 Normal;   
in vec2 TexCoord; 




struct Material {
    vec3 ambient;  
    vec3 diffuse;  
    vec3 specular; 
    float shininess; 
};


struct AmbientLight {
    bool enabled; 
    vec3 color;   
};


struct DirectionalLight {
    bool enabled;  
    vec3 direction; 

    vec3 ambient;  
    vec3 diffuse;  
    vec3 specular; 
};


struct PointLight {
    bool enabled;  
    vec3 position; 

    vec3 ambient;  
    vec3 diffuse;  
    vec3 specular; 

    
    float kc_atenuation; 
    float kl_atenuation; 
    float kq_atenuation; 
};


struct SpotLight {
    bool enabled;  
    vec3 position; 
    vec3 direction; 

    vec3 ambient;  
    vec3 diffuse;  
    vec3 specular; 

    
    float kc_atenuation;
    float kl_atenuation;
    float kq_atenuation;

    float s_exponent;    
    float cutOff;        
    float outerCutOff;   
};



uniform Material material;          

uniform AmbientLight ambientLight;  
uniform DirectionalLight directionalLight; 
uniform PointLight pointLight;      
uniform SpotLight spotLight;        

uniform sampler2D texture1;         

uniform vec3 viewPos;               


out vec4 FragColor; 




vec4 IAmbientLight() {
    if (!ambientLight.enabled) 
        return vec4(0.0f);

    
    return vec4(material.ambient * ambientLight.color, 1.0f);
}


vec4 IDirectionalLight() {
    if(!directionalLight.enabled) 
        return vec4(0.0f);

    
    vec4 iambient = vec4(material.ambient * directionalLight.ambient, 1.0f);

    
    vec3 L = normalize(-directionalLight.direction); 
    vec3 N = normalize(Normal.xyz);                 

    
    float NdotL = max(dot(N, L), 0.0f);

    
    vec4 idiffuse = vec4(material.diffuse * directionalLight.diffuse, 1.0f) * NdotL;

    
    vec3 V = normalize(viewPos - FragPos); 
    vec3 H = normalize(L + V);             

    
    float HdotN = max(dot(H, N), 0.0f);

    
    vec4 ispecular = pow(HdotN, material.shininess) * NdotL * vec4(directionalLight.specular * material.specular, 1.0f);

    
    return iambient + idiffuse + ispecular;
}


vec4 IPointLight() {
    if(!pointLight.enabled) 
        return vec4(0.0f);

    
    vec4 iambient = vec4(material.ambient * pointLight.ambient, 1.0f);

    
    vec3 L = normalize(pointLight.position - FragPos); 
    vec3 N = normalize(Normal.xyz);                   

    float NdotL = max(dot(N, L), 0.0f);

    vec4 idiffuse = vec4(material.diffuse * pointLight.diffuse, 1.0f) * NdotL;

    
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V);

    float HdotN = max(dot(H, N), 0.0f);

    vec4 ispecular = pow(HdotN, material.shininess) * NdotL * vec4(pointLight.specular * material.specular, 1.0f);

    
    float distance = length(pointLight.position - FragPos); 
    float attenuation = 1.0f / (pointLight.kc_atenuation + pointLight.kl_atenuation * distance + pointLight.kq_atenuation * (distance * distance));

    
    return (iambient + idiffuse + ispecular) * attenuation;
}


vec4 ISpotLight() {
    if(!spotLight.enabled) 
        return vec4(0.0f);

    
    vec4 iambient = vec4(material.ambient * spotLight.ambient, 1.0f);

    
    vec3 L = normalize(spotLight.position - FragPos); 
    vec3 N = normalize(Normal.xyz);                   

    float NdotL = max(dot(N, L), 0.0f);

    vec4 idiffuse = vec4(material.diffuse * spotLight.diffuse, 1.0f) * NdotL;

    
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V);

    float HdotN = max(dot(H, N), 0.0f); 

    vec4 ispecular = pow(HdotN, material.shininess) * vec4(spotLight.specular * material.specular, 1.0f);

    
    float distance = length(spotLight.position - FragPos);

    
    float DdotV = dot(L, normalize(-spotLight.direction)); 

    
    float SE = clamp((DdotV - spotLight.outerCutOff) / (spotLight.cutOff - spotLight.outerCutOff), 0.0f, 1.0f);

    float attenuation = 0.0f;
    
    if (DdotV > spotLight.outerCutOff) {
        
        attenuation = (1.0f * pow(DdotV, spotLight.s_exponent)) / 
            (spotLight.kc_atenuation + spotLight.kl_atenuation * distance + spotLight.kq_atenuation * pow(distance, 2.0f));
    }

    
    return (iambient + idiffuse + ispecular) * attenuation * SE;
}


void main() {
    
    vec4 texColor = texture(texture1, TexCoord);

    
    
    FragColor = (IAmbientLight() + IDirectionalLight() + IPointLight() + ISpotLight()) * texColor;
}