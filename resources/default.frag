#version 450 core

// inputs do vertex shader
in vec3 FragPos;  // a posicao exata de cada vertice
in vec3 Normal;   // a direcao de cada vetor normal a cada vertice
in vec2 TexCoord; // as coordenadas da textura.

// --- definicoes de estruturas  ---

// struct Material
struct Material {
    vec3 ambient;  // a cor ambiente do material (base, sem luz).
    vec3 diffuse;  // a cor difusa do material (como reflete a luz).
    vec3 specular; // a cor especular do material (como brilha).
    float shininess; // o "brilho" do material, afeta o tamanho do ponto de luz especular.
};

// struct da luz ambiente
struct AmbientLight {
    bool enabled; // se esta luz esta ligada ou desligada.
    vec3 color;   // a cor da luz ambiente.
};

// struct da luz direcional
struct DirectionalLight {
    bool enabled;  // se esta luz esta ligada ou desligada.
    vec3 direction; // a direcao de onde a luz esta a vir.

    vec3 ambient;  // a cor da componente ambiente que esta luz emite.
    vec3 diffuse;  // a cor da componente difusa que esta luz emite.
    vec3 specular; // a cor da componente especular que esta luz emite.
};

// struct da point light
struct PointLight {
    bool enabled;  // se esta luz esta ligada ou desligada.
    vec3 position; // a posicao exata desta lampada no mundo 3d.

    vec3 ambient;  // a cor da componente ambiente que esta luz emite.
    vec3 diffuse;  // a cor da componente difusa que esta luz emite.
    vec3 specular; // a cor da componente especular que esta luz emite.

    // parametros de atenuacao: dizem o quao rapido a luz enfraquece com a distancia.
    float kc_atenuation; // constante
    float kl_atenuation; // linear
    float kq_atenuation; // quadratica
};

// struct da spot light
struct SpotLight {
    bool enabled;  // se esta luz esta ligada ou desligada.
    vec3 position; // a posicao de onde este foco de luz esta a vir.
    vec3 direction; // a direcao para onde este foco de luz esta a apontar.

    vec3 ambient;  // a cor da componente ambiente que esta luz emite.
    vec3 diffuse;  // a cor da componente difusa que esta luz emite.
    vec3 specular; // a cor da componente especular que esta luz emite.

    // parametros de atenuacao (como na luz de ponto).
    float kc_atenuation;
    float kl_atenuation;
    float kq_atenuation;

    float s_exponent;    // afeta a intensidade da luz dentro do cone.
    float cutOff;        // o angulo do cone interno (luz mais forte).
    float outerCutOff;   // o angulo do cone externo (onde a luz comeca a desvanecer).
};

// --- variaveis uniform  ---

uniform Material material;          // uniform material para definir as propriedades do material do objeto.

uniform AmbientLight ambientLight;  // os detalhes da luz ambiente da cena.
uniform DirectionalLight directionalLight; // os detalhes da luz direcional da cena.
uniform PointLight pointLight;      // os detalhes da luz de ponto da cena.
uniform SpotLight spotLight;        // os detalhes da luz de foco da cena.

uniform sampler2D texture1;         // a imagem (textura) que da cor e detalhes ao objeto.

uniform vec3 viewPos;               // a posicao da nossa camera (observador) no mundo 3d.

// --- saida do resultado final do fragment ---
out vec4 FragColor; // a cor final que este pontinho vai ter no ecra.

// --- funcoes  ---

// calcula a contribuicao da luz ambiente.
vec4 IAmbientLight() {
    if (!ambientLight.enabled) // se a luz ambiente estiver desligada, nao contribui com nada.
        return vec4(0.0f);

    // cor final da luz ambiente = cor ambiente do material * cor da luz ambiente.
    return vec4(material.ambient * ambientLight.color, 1.0f);
}

// calcula a contribuicao da luz direcional.
vec4 IDirectionalLight() {
    if(!directionalLight.enabled) // se a luz direcional estiver desligada, nao contribui com nada.
        return vec4(0.0f);

    // 1. componente ambiente: luz base, sem depender da direcao.
    vec4 iambient = vec4(material.ambient * directionalLight.ambient, 1.0f);

    // 2. componente difusa: o quao diretamente a luz bate na superficie.
    vec3 L = normalize(-directionalLight.direction); // a direcao da luz (aponta para a luz).
    vec3 N = normalize(Normal.xyz);                 // a direcao da superficie.

    // dot product: se n e l apontam na mesma direcao, e 1 (luz forte). se opostas, 0 (sombra).
    float NdotL = max(dot(N, L), 0.0f);

    // cor difusa = cor difusa do material * cor difusa da luz * intensidade (ndotl).
    vec4 idiffuse = vec4(material.diffuse * directionalLight.diffuse, 1.0f) * NdotL;

    // 3. componente especular (o brilho):
    vec3 V = normalize(viewPos - FragPos); // a direcao da camera para este pontinho.
    vec3 H = normalize(L + V);             // a direcao "intermedia" entre a luz e a camera (halfway vector).

    // dot product: ve o quao bem o brilho esta alinhado com a camera.
    float HdotN = max(dot(H, N), 0.0f);

    // cor especular = quao brilhante o material e * intensidade (ndotl) * cor da luz especular.
    vec4 ispecular = pow(HdotN, material.shininess) * NdotL * vec4(directionalLight.specular * material.specular, 1.0f);

    // soma as tres componentes para a cor final desta luz.
    return iambient + idiffuse + ispecular;
}

// calcula a contribuicao da luz de ponto.
vec4 IPointLight() {
    if(!pointLight.enabled) // se a luz de ponto estiver desligada, nao contribui com nada.
        return vec4(0.0f);

    // 1. componente ambiente.
    vec4 iambient = vec4(material.ambient * pointLight.ambient, 1.0f);

    // 2. componente difusa.
    vec3 L = normalize(pointLight.position - FragPos); // a direcao da lampada para este pontinho.
    vec3 N = normalize(Normal.xyz);                   // a direcao da superficie.

    float NdotL = max(dot(N, L), 0.0f);

    vec4 idiffuse = vec4(material.diffuse * pointLight.diffuse, 1.0f) * NdotL;

    // 3. componente especular.
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V);

    float HdotN = max(dot(H, N), 0.0f);

    vec4 ispecular = pow(HdotN, material.shininess) * NdotL * vec4(pointLight.specular * material.specular, 1.0f);

    // 4. atenuacao pela distancia: a luz enfraquece a medida que te afastas.
    float distance = length(pointLight.position - FragPos); // calcula a distancia.
    float attenuation = 1.0f / (pointLight.kc_atenuation + pointLight.kl_atenuation * distance + pointLight.kq_atenuation * (distance * distance));

    // soma as componentes e aplica a atenuacao.
    return (iambient + idiffuse + ispecular) * attenuation;
}

// calcula a contribuicao da luz de foco.
vec4 ISpotLight() {
    if(!spotLight.enabled) // se a luz de foco estiver desligada, nao contribui com nada.
        return vec4(0.0f);

    // 1. componente ambiente.
    vec4 iambient = vec4(material.ambient * spotLight.ambient, 1.0f);

    // 2. componente difusa.
    vec3 L = normalize(spotLight.position - FragPos); // a direcao do foco para este pontinho.
    vec3 N = normalize(Normal.xyz);                   // a direcao da superficie.

    float NdotL = max(dot(N, L), 0.0f);

    vec4 idiffuse = vec4(material.diffuse * spotLight.diffuse, 1.0f) * NdotL;

    // 3. componente especular.
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V);

    float HdotN = max(dot(H, N), 0.0f); 

    vec4 ispecular = pow(HdotN, material.shininess) * vec4(spotLight.specular * material.specular, 1.0f);

    // 4. atenuacao pela distancia.
    float distance = length(spotLight.position - FragPos);

    // 5. calculo do cone do foco:
    float DdotV = dot(L, normalize(-spotLight.direction)); // ve o quao perto o pontinho esta do centro do foco.

    // 'se' e um fator de suavizacao que faz a luz desvanecer nas bordas do cone.
    float SE = clamp((DdotV - spotLight.outerCutOff) / (spotLight.cutOff - spotLight.outerCutOff), 0.0f, 1.0f);

    float attenuation = 0.0f;
    // se o pontinho estiver dentro do cone exterior do foco:
    if (DdotV > spotLight.outerCutOff) {
        // calcula a atenuacao, que e mais intensa no centro e diminui para as bordas.
        attenuation = (1.0f * pow(DdotV, spotLight.s_exponent)) / 
            (spotLight.kc_atenuation + spotLight.kl_atenuation * distance + spotLight.kq_atenuation * pow(distance, 2.0f));
    }

    // soma as componentes, aplica a atenuacao e o fator de suavizacao do cone.
    return (iambient + idiffuse + ispecular) * attenuation * SE;
}

// --- a "mistura final" (a funcao principal) ---
void main() {
    // vai buscar a cor do pontinho na imagem (textura).
    vec4 texColor = texture(texture1, TexCoord);

    // soma a contribuicao de todas as luzes e multiplica pela cor da textura.
    // isto e, a luz decide a intensidade e a cor geral, e a textura da o padrao e os detalhes.
    FragColor = (IAmbientLight() + IDirectionalLight() + IPointLight() + ISpotLight()) * texColor;
}