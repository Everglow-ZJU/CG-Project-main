#include "helicopter.h"

const std::string modelPath = "../../../media/Seahawk.obj";

const std::string Seahawk = {
        "../../../media/Seahawk2.jpg"
};

Helicopter::Helicopter() {
    // init model
    _model.reset(new Model(modelPath));
    _model->scale = glm::vec3(0.02f, 0.02f, 0.02f);

    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    float rotationAngle = 3.14f;
    _rotation = glm::axisAngleMatrix(rotationAxis, rotationAngle);

    // init texture
    this->_texture.reset(new Texture2D(Seahawk));

    // init shader
    initShader();

    _Box = new AABBBox();

}

Helicopter::~Helicopter() {

}

void Helicopter::move(glm::vec3 offset) {
    _position += offset;
}

void Helicopter::draw(Shader* _heliShader,glm::mat4 projection, glm::mat4 view) {

    if (_viewPattern == ViewPattern::FPS) return;

    // translation
    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, _position);
    // rotation
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = _rotation * _rotation2 * rotation;
    // scale
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(scale, _scale);

    glm::mat4 model = translation * rotation * scale;

    _heliShader->use();

    _heliShader->setMat4("projection", projection);
    _heliShader->setMat4("view", view);
    _heliShader->setMat4("model", model);

    // glActiveTexture(GL_TEXTURE0);
    // _texture->bind();
		glActiveTexture(GL_TEXTURE0);
		_texture->bind();
		glActiveTexture(GL_TEXTURE1);
		_texture->bind();
    _model->draw();

    if (_firstDraw) {
        _Box->findMinAndMax(_model->getVertex(),model);//* projection* view);
        _firstDraw = false;
    }

}

void Helicopter::initShader() {
    const char* vertCode =
            "#version 330 core\n"
            "layout(location = 0) in vec3 aPosition;\n"
            "layout(location = 1) in vec3 aNormal;\n"
            "layout(location = 2) in vec2 aTexCoord;\n"
            "out vec2 TexCoord;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main() {\n"
            "	TexCoord = aTexCoord;\n"
            "	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
            "}\n";

    const char* fragCode =
            "#version 330 core\n"
            "in vec2 TexCoord;\n"
            "out vec4 color;\n"
            "uniform sampler2D mapKd;\n"
            "void main() {\n"
            "	color = texture(mapKd, TexCoord);\n"
            "}\n";

    _shader.reset(new Shader(vertCode, fragCode));

}

void Helicopter::bindPosition(glm::vec3& cameraPosition) {
    glm::vec3 front = glm::normalize(-cameraPosition);
    if (_viewPattern == ViewPattern::TPS) {
        _position = cameraPosition + glm::vec3(0.f, -5.f, -10.f);
    } else {
        _position = cameraPosition + glm::vec3(0.f, -0.5f, 0.f);
    }

}

void Helicopter::rotationX(glm::vec3 axis, float angle) {
    _rotation *= glm::axisAngleMatrix(axis, angle);
}

void Helicopter::tiltLeft() {
    glm::vec3 axis = glm::vec3(0.f, 0.f, -1.f);
    if (false == _left) {
        _rotation2 = glm::axisAngleMatrix(axis, 3.14f / 6.f);
        _left = true;
    }

}

void Helicopter::tiltRight() {
    glm::vec3 axis = glm::vec3(0.f, 0.f, 1.f);
    if (false == _right) {
        _rotation2 = glm::axisAngleMatrix(axis, 3.14f / 6.f);
        _right = true;
    }
}

void Helicopter::tiltFront() {
    glm::vec3 axis = glm::vec3(1.f, 0.f, 0.f);
    if (false == _front) {
        _rotation2 = glm::axisAngleMatrix(axis, 3.14f / 6.f);
        _front = true;
    }
}

void Helicopter::tiltBack() {
    glm::vec3 axis = glm::vec3(-1.f, 0.f, 0.f);
    if (false == _back) {
        _rotation2 = glm::axisAngleMatrix(axis, 3.14f / 6.f);
        _back = true;
    }
}

void Helicopter::resetTilt() {
    _rotation2 = glm::mat4(1.0f);
    _left = false;
    _right = false;
    _front = false;
    _back = false;
}

//Normal is the plane normal direction, position is the plane position, 
//planeposition is the aircraft position, and direction is the flight direction of the aircraft
int Helicopter::collisionDetection(glm::vec3& Normal, glm::vec3& position, glm::vec3& PlanePosition, glm::vec3& direction) {
    double dotProduct = dot(Normal,direction);//Calculate the dot of plane and plane
    //If the dot is multiplied by 0, it means that it is parallel and will never collide
    if (dotProduct<0.0001 && dotProduct>-0.0001) {
        return 0;
    }
    //Calculate the distance from the aircraft point to the plane
    double l2 = (dot(Normal, position - PlanePosition)) / dotProduct;
    //If L2 is less than 0, the plane is behind the aircraft
    if (l2 < -0.0001) {
        return 0;
    }
    //If L2 is greater than 0, it means it hasn't hit
    else if (l2 > 0.001) {
        return 0;
    }
    //else means hit
    else {
        return 1;
    }
}

void Helicopter::draw(glm::mat4 projection, glm::mat4 view, glm::vec3 lightPosition, glm::vec3 lightDirection, glm::vec3 viewPos) {
    if (_viewPattern == ViewPattern::FPS) return;

    this->initShader2();


    // translation
    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, _position);
    // rotation
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = _rotation * _rotation2 * rotation;
    // scale
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(scale, _scale);

    glm::mat4 model = translation * rotation * scale;

    _shader->use();

    _shader->setMat4("projection", projection);
    _shader->setMat4("view", view);
    _shader->setMat4("model", model);

    _shader->setVec3("viewPos", viewPos);

    _shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    _shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

    // light properties
    _shader->setVec3("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    _shader->setVec3("SpotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    _shader->setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    _shader->setFloat("spotLight.constant", 1.0f);
    _shader->setFloat("spotLight.linear", 0.09f);
    _shader->setFloat("spotLight.quadratic", 0.032f);
    _shader->setVec3("spotLight.position", lightPosition);
    _shader->setVec3("spotLight.direction", lightDirection);

    // material properties
    _shader->setFloat("material.shininess", 32.0f);


    // directional light
    _shader->setVec3("dirLight.direction",glm::vec3( -0.2f, -1.0f, -0.3f));
    _shader->setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    _shader->setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    _shader->setVec3("dirLight.specular",glm::vec3( 0.5f, 0.5f, 0.5f));

    glActiveTexture(GL_TEXTURE0);
    _texture->bind();

    _model->draw();
}

void Helicopter::initShader2() {
    const char* vertCode =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aNormal;\n"
            "layout (location = 2) in vec2 aTexCoords;\n"
            "\n"
            "out vec3 FragPos;\n"
            "out vec3 Normal;\n"
            "out vec2 TexCoords;\n"
            "\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
            "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
            "    TexCoords = aTexCoords;\n"
            "\n"
            "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
            "}";

    const char* fragCode =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "\n"
            "struct Material {\n"
            "    sampler2D diffuse;\n"
            "    sampler2D specular;\n"
            "    float shininess;\n"
            "};\n"
            "\n"
            "struct DirLight {\n"
            "    vec3 direction;\n"
            "\n"
            "    vec3 ambient;\n"
            "    vec3 diffuse;\n"
            "    vec3 specular;\n"
            "};\n"
            "struct SpotLight {\n"
            "    vec3 position;\n"
            "    vec3 direction;\n"
            "    float cutOff;\n"
            "    float outerCutOff;\n"
            "\n"
            "    float constant;\n"
            "    float linear;\n"
            "    float quadratic;\n"
            "\n"
            "    vec3 ambient;\n"
            "    vec3 diffuse;\n"
            "    vec3 specular;\n"
            "};\n"
            "in vec3 FragPos;\n"
            "in vec3 Normal;\n"
            "in vec2 TexCoords;\n"
            "\n"
            "uniform vec3 viewPos;\n"
            "uniform DirLight dirLight;\n"
            "// uniform PointLight pointLights[NR_POINT_LIGHTS];\n"
            "uniform SpotLight spotLight;\n"
            "uniform Material material;\n"
            "\n"
            "// function prototypes\n"
            "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
            "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
            "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
            "\n"
            "void main()\n"
            "{\n"
            "    // properties\n"
            "    vec3 norm = normalize(Normal);\n"
            "    vec3 viewDir = normalize(viewPos - FragPos);\n"
            "\n"
            "    // phase 1: directional lighting\n"
            "    vec3 result = CalcDirLight(dirLight, norm, viewDir);\n"
            "\n"
            "    // phase 2: point lights\n"
            "//     for(int i = 0; i < NR_POINT_LIGHTS; i++)\n"
            "//         result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\n"
            "\n"
            "    // phase 3: spot light\n"
            "    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);\n"
            "\n"
            "    FragColor = vec4(result, 1.0);\n"
            "}\n"
            "\n"
            "// calculates the color when using a directional light.\n"
            "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n"
            "{\n"
            "    vec3 lightDir = normalize(-light.direction);\n"
            "    // diffuse shading\n"
            "    float diff = max(dot(normal, lightDir), 0.0);\n"
            "    // specular shading\n"
            "    vec3 reflectDir = reflect(-lightDir, normal);\n"
            "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
            "    // combine results\n"
            "    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n"
            "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n"
            "    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n"
            "    return (ambient + diffuse + specular);\n"
            "}\n"
            "\n"
            "// calculates the color when using a spot light.\n"
            "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
            "{\n"
            "    vec3 lightDir = normalize(light.position - fragPos);\n"
            "    // diffuse shading\n"
            "    float diff = max(dot(normal, lightDir), 0.0);\n"
            "    // specular shading\n"
            "    vec3 reflectDir = reflect(-lightDir, normal);\n"
            "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
            "    // attenuation\n"
            "    float distance = length(light.position - fragPos);\n"
            "    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
            "    // spotlight intensity\n"
            "    float theta = dot(lightDir, normalize(-light.direction));\n"
            "    float epsilon = light.cutOff - light.outerCutOff;\n"
            "    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);\n"
            "    // combine results\n"
            "    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n"
            "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n"
            "    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n"
            "    ambient *= attenuation * intensity;\n"
            "    diffuse *= attenuation * intensity;\n"
            "    specular *= attenuation * intensity;\n"
            "    return (ambient + diffuse + specular);\n"
            "}";

    _shader.reset(new Shader(vertCode, fragCode));
}

void Helicopter::rotate2Front(float angle) {
    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    _rotation *= glm::axisAngleMatrix(rotationAxis, angle);

}
