#include "core.h"

const std::vector<std::string> skyboxTexturePaths = {
        "../../../media/LakeView/Right_Tex.jpg",
        "../../../media/LakeView/Left_Tex.jpg",
        "../../../media/LakeView/Down_Tex.jpg",
        "../../../media/LakeView/Up_Tex.jpg",
        "../../../media/LakeView/Front_Tex.jpg",
        "../../../media/LakeView/Back_Tex.jpg"
};
const std::string lightingVsFile("../../../projects/final_project/multiple_lights2.vs");
const std::string lightingFsFile("../../../projects/final_project/multiple_lights2.fs");
const std::string lightingVsFile_noTexture("../../../projects/final_project/ballvs.vs");
const std::string lightingFsFile_noTexture("../../../projects/final_project/ballfs.fs");
glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  200.0f,  2.0f),
    };
Core::Core() {
    _windowTitle = "Final Project";
    /*init lighting shader*/
    _lightingShader=(new Shader(lightingVsFile,lightingFsFile));
    _lightingShader_noTexture=(new Shader(lightingVsFile_noTexture,lightingFsFile_noTexture));
    /* Init SkyBox. */
    _skybox.reset(new SkyBox(skyboxTexturePaths));

    /* init Camera. */
    _camera.reset(new PerspectiveCamera(glm::radians(50.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
    _camera->position.z = 10.0f;

    /* init ball*/
    _ball.reset(new ballModel());
    /* init cylinder*/
    _cone = new Cylinder(0.0f,10.0f,5.0f,3,glm::vec3(-230.0f, -46.5f, 300.0f));

    _cylinder = new Cylinder(5.0f,5.0f,5.0f,40, glm::vec3(50.f, 0.f, 100.f));

    _coneFrustum = new Cylinder(5.0f,10.0f,5.0f,40, glm::vec3(-50.f, 50.f, 0.f));

    _prism = new Cylinder(5.0f,5.0f,5.0f,6, glm::vec3(-50.f, 50.f, 50.f));

    _pyramid = new Cylinder(0.0f,10.0f,15.0f,7, glm::vec3(-50.f, -50.f, -50.f));

    _frustum = new Cylinder(5.0f,10.0f,5.0f,8, glm::vec3(500.f, 50.f, -100.f));

    /* init House. */
    _house.reset(new House());
    /* init Lamp. */
    _lamp.reset(new Lamp());
}

Core::~Core() {
    delete(_lightingShader);
    delete(_lightingShader_noTexture);
}

void Core::renderFrame() {
    showFpsInWindowTitle();

    glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glm::mat4 projection = _camera->getProjectionMatrix();
    glm::mat4 view = _camera->getViewMatrix();
    glm::vec3 position=_camera->position;
    glm::vec3 front=_camera->getFront();
    /* init light shader*/

    glm::vec3 lightColor = glm::vec3(0.2f+abs(0.5*sin((float)0.5*glfwGetTime())),0.2f+abs(0.5*sin((float)0.5*glfwGetTime())),0.2f+abs(0.5*sin((float)0.5*glfwGetTime())));
    
    _lightingShader->use();
    _lightingShader->setVec3("viewPos", _camera->position);
    _lightingShader->setInt("material.diffuse", 0);
    _lightingShader->setInt("material.specular", 1);
    _lightingShader->setFloat("material.shininess", 32.0f);
    // directional light
    _lightingShader->setVec3("dirLight.direction",glm::vec3( -0.2f, -1.0f, -0.3f));
    _lightingShader->setVec3("dirLight.ambient", lightColor );
    _lightingShader->setVec3("dirLight.diffuse", lightColor*0.5f);
    _lightingShader->setVec3("dirLight.specular",glm::vec3(0.5f, 0.5f, 0.5f));
    // point light 1
    _lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
    _lightingShader->setVec3("pointLights[0].ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    _lightingShader->setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    _lightingShader->setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    _lightingShader->setFloat("pointLights[0].constant", 1.0f);
    _lightingShader->setFloat("pointLights[0].linear", 0.09);
    _lightingShader->setFloat("pointLights[0].quadratic", 0.032);
        // spotLight
    _lightingShader->setVec3("spotLight.position",_camera->position);
    _lightingShader->setVec3("spotLight.direction",_camera->getFront());
    _lightingShader->setVec3("spotLight.ambient",glm::vec3( 0.1f, 0.1f, 0.1f));
    _lightingShader->setVec3("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    _lightingShader->setVec3("spotLight.specular",glm::vec3( 1.0f, 1.0f, 1.0f));
    _lightingShader->setFloat("spotLight.constant", 1.0f);
    _lightingShader->setFloat("spotLight.linear", 0.09);
    _lightingShader->setFloat("spotLight.quadratic", 0.032);
    _lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    _lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    _lightingShader_noTexture->use();
    _lightingShader_noTexture->setVec3("viewPos", _camera->position);
    // directional light
    _lightingShader_noTexture->setVec3("dirLight.direction",glm::vec3( -0.2f, -1.0f, -0.3f));
    _lightingShader_noTexture->setVec3("dirLight.ambient",lightColor );
    _lightingShader_noTexture->setVec3("dirLight.diffuse", lightColor*0.5f);
    _lightingShader_noTexture->setVec3("dirLight.specular",glm::vec3(0.5f, 0.5f, 0.5f));
    // point light 1
    _lightingShader_noTexture->setVec3("pointLights[0].position", pointLightPositions[0]);
    _lightingShader_noTexture->setVec3("pointLights[0].ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    _lightingShader_noTexture->setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    _lightingShader_noTexture->setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    _lightingShader_noTexture->setFloat("pointLights[0].constant", 1.0f);
    _lightingShader_noTexture->setFloat("pointLights[0].linear", 0.09);
    _lightingShader_noTexture->setFloat("pointLights[0].quadratic", 0.032);
        // spotLight
    _lightingShader_noTexture->setVec3("spotLight.position",_camera->position);
    _lightingShader_noTexture->setVec3("spotLight.direction",_camera->getFront());
    _lightingShader_noTexture->setVec3("spotLight.ambient",glm::vec3( 0.1f, 0.1f, 0.1f));
    _lightingShader_noTexture->setVec3("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    _lightingShader_noTexture->setVec3("spotLight.specular",glm::vec3( 1.0f, 1.0f, 1.0f));
    _lightingShader_noTexture->setFloat("spotLight.constant", 1.0f);
    _lightingShader_noTexture->setFloat("spotLight.linear", 0.09);
    _lightingShader_noTexture->setFloat("spotLight.quadratic", 0.032);
    _lightingShader_noTexture->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    _lightingShader_noTexture->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    _helicopter.draw(_lightingShader,projection, view);
    // _helicopter.draw(projection, view, _camera->position, glm::normalize(-_camera->position), _camera->position);

    _house->DrawHouse(_lightingShader,projection, view);

    _door.draw(projection, view);

    _lamp->draw(projection, view);

    _horse.draw(_lightingShader,projection, view);

    _ball->draw(_lightingShader_noTexture,projection, view);

    _cone->draw(_lightingShader_noTexture,projection, view);

    _cylinder->draw(_lightingShader_noTexture,projection, view);

    _prism->draw(_lightingShader_noTexture,projection, view);

    _pyramid->draw(_lightingShader_noTexture,projection, view);

    _frustum->draw(_lightingShader_noTexture,projection, view);

    _coneFrustum->draw(_lightingShader_noTexture,projection, view);

    _skybox->draw(projection, -view);

}

void Core::handleInput() {

    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    const float cameraMoveSpeed = 0.1f * deltaTime;
    const float cameraRotateSpeed = 0.02f;

    if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
        glfwSetWindowShouldClose(_window, true);
        return ;
    }

    if (_keyboardInput.keyStates[GLFW_KEY_E] != GLFW_RELEASE) {
        std::cout << "E" << std::endl;
        if (!_door.isOpen&& distance(_helicopter.getPosition(),_door.getPosition())<200.0f) {
            _door.OpenDoor();
        }
        else if (_door.isOpen && distance(_helicopter.getPosition(), _door.getPosition()) < 200.0f)
        {
            _door.CloseDoor();
        }
    }

    glm::vec3 cameraFront = glm::normalize(_camera->getFront());
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::vec3 offset;

    if (firstBind) {
        bindCamera2H(_helicopter.getPosition(), _helicopter.isFPS());
        firstBind = false;
    }

    _helicopter.resetTilt();

    if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
        std::cout << "W" << std::endl;
        if (viewChange) {
            _helicopter.rotate2Front(rotateAngle);
            viewChange = false;
            rotateAngle = 0;
        }
        offset = cameraMoveSpeed * cameraFront;
        _helicopter.move(offset);
        _helicopter._Box->boxMove(offset);
        if (!_door.isOpen&&_house->_Box->checkCollision(*(_helicopter._Box))) {
            _helicopter.move(-offset);
            _helicopter._Box->boxMove(-offset);
        }
        bindCamera2H(_helicopter.getPosition(), _helicopter.isFPS());
        _helicopter.tiltFront();

    }

    if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
        std::cout << "A" << std::endl;
        if (viewChange) {
            _helicopter.rotate2Front(rotateAngle);
            viewChange = false;
            rotateAngle = 0;
        }
        offset = -glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraMoveSpeed;
        _helicopter.move(offset);
        _helicopter._Box->boxMove(offset);
        if (!_door.isOpen&&_house->_Box->checkCollision(*(_helicopter._Box))) {
            _helicopter.move(-offset);
            _helicopter._Box->boxMove(-offset);
        }
        bindCamera2H(_helicopter.getPosition(), _helicopter.isFPS());
        _helicopter.tiltLeft();

    }

    if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
        std::cout << "S" << std::endl;
        if (viewChange) {
            _helicopter.rotate2Front(rotateAngle);
            viewChange = false;
            rotateAngle = 0;
        }
        offset = -cameraMoveSpeed * cameraFront;
        _helicopter.move(offset);
        _helicopter._Box->boxMove(offset);
        if (!_door.isOpen&&_house->_Box->checkCollision(*(_helicopter._Box))) {
            _helicopter.move(-offset);
            _helicopter._Box->boxMove(-offset);
        }
        bindCamera2H(_helicopter.getPosition(), _helicopter.isFPS());
        _helicopter.tiltBack();

    }

    if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
        std::cout << "D" << std::endl;
        if (viewChange) {
            _helicopter.rotate2Front(rotateAngle);
            viewChange = false;
            rotateAngle = 0;
        }
        offset = glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraMoveSpeed;
        _helicopter.move(offset);
        _helicopter._Box->boxMove(offset);
        if (!_door.isOpen&&_house->_Box->checkCollision(*(_helicopter._Box))) {
            _helicopter.move(-offset);
            _helicopter._Box->boxMove(-offset);
        }
        bindCamera2H(_helicopter.getPosition(), _helicopter.isFPS());
        _helicopter.tiltRight();

    }

    if (_keyboardInput.keyStates[GLFW_KEY_LEFT_SHIFT] != GLFW_RELEASE) {
        std::cout << "Up" << std::endl;
        offset = cameraMoveSpeed * cameraUp;
        _helicopter.move(offset);
        _helicopter._Box->boxMove(offset);
        if (_house->_Box->checkCollision(*(_helicopter._Box))) {
            _helicopter.move(-offset);
            _helicopter._Box->boxMove(-offset);
        }
        bindCamera2H(_helicopter.getPosition(), _helicopter.isFPS());
    }

    if (_keyboardInput.keyStates[GLFW_KEY_SPACE] != GLFW_RELEASE) {
        std::cout << "Down" << std::endl;
        offset = -cameraMoveSpeed * cameraUp;
        _helicopter.move(offset);
        _helicopter._Box->boxMove(offset);
        if (_house->_Box->checkCollision(*(_helicopter._Box))) {
            _helicopter.move(-offset);
            _helicopter._Box->boxMove(-offset);
        }
        bindCamera2H(_helicopter.getPosition(), _helicopter.isFPS());
    }

    if (_keyboardInput.keyStates[GLFW_KEY_N] != GLFW_RELEASE) {
        std::cout << "Q" << std::endl;
        if (!_helicopter.isFPS()) {
            _helicopter.ViewPattern_FPS();

            _camera->position += glm::vec3(0.f, -5.f, -10.f);
            _helicopter.bindPosition(_camera->position);
        }
    }

    if (_keyboardInput.keyStates[GLFW_KEY_M] != GLFW_RELEASE) {
        std::cout << "E" << std::endl;
        if (_helicopter.isFPS()) {
            _helicopter.ViewPattern_TPS();

            _camera->position -= glm::vec3(0.f, -5.f, -10.f);
            _helicopter.bindPosition(_camera->position);
        }

    }


    // Adapt different screens.
    float currWidth, currHeight;
    currWidth = 2560;
    currHeight = 1440;

    if (_keyboardInput.keyStates[GLFW_KEY_P] != GLFW_RELEASE) {
        if (!_screenShot.function(currWidth, currHeight)) {
            std::cout << "Screen shot failed!" << std::endl;
        } else {
            std::cout << "Screen shot success!" << std::endl;
        };
    }

    if (firstMove) {
        _mouseInput.move.xOld = _mouseInput.move.xCurrent;
        _mouseInput.move.yOld = _mouseInput.move.yCurrent;
        firstMove = false;
    }

    if (_mouseInput.move.xCurrent != _mouseInput.move.xOld || rotation_flag_X) {
        viewChange = true;
        std::cout << "mouse move in x direction" << std::endl;
        float deltaX;

        if (_mouseInput.move.xCurrent < 0.05f * currWidth) {
            deltaX = static_cast<float>(-25.f);
            rotation_flag_X = true;
        } else if (_mouseInput.move.xCurrent > 0.45f * currWidth) {
            deltaX = static_cast<float>(25.f);
            rotation_flag_X = true;
        } else {
            deltaX = static_cast<float>(2 * (_mouseInput.move.xCurrent - _mouseInput.move.xOld));
            rotation_flag_X = false;
        }

        const float angle = -cameraRotateSpeed * _deltaTime * deltaX;
        const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };

        bindCamera2H(_helicopter.getPosition(), axis, angle, _helicopter.isFPS());

        _mouseInput.move.xOld = _mouseInput.move.xCurrent;

        rotateAngle += angle;

    }

    if (_mouseInput.move.yCurrent != _mouseInput.move.yOld || rotation_flag_Y) {
        std::cout << "mouse move in y direction" << std::endl;
        // TODO: rotate around local right
        float deltaY;

        if (_mouseInput.move.yCurrent < 0.1f * currHeight) {
            deltaY = static_cast<float>(-25.f);
            rotation_flag_Y = true;
        } else if (_mouseInput.move.yCurrent > 0.45 * currHeight) {
            deltaY = static_cast<float>(25.f);
            rotation_flag_Y = true;
        } else {
            deltaY = static_cast<float>(2 * (_mouseInput.move.yCurrent - _mouseInput.move.yOld));
            rotation_flag_Y = false;
        }

        const float angle = -cameraRotateSpeed * _deltaTime * deltaY;
        const glm::vec3 axis = _camera->getRight();

        glm::qua<float> temp = glm::qua<float>(cos(angle/2), axis*sin(angle/2)) * _camera->rotation;
        _camera->rotation = temp;
        _mouseInput.move.yOld = _mouseInput.move.yCurrent;

    }
}

void Core::bindCamera2H(glm::vec3 hPosition, bool isFPS) {
    glm::vec3 cameraFront = _camera->getFront();

    glm::vec3 moveUp;
    glm::vec3 moveBack;

    if (isFPS) {
        moveUp = glm::vec3(0.f, 0.5f, 0.f);
        moveBack = glm::vec3(0.f, 0.f, 0.f);
    } else {
        moveUp = glm::vec3(0.f, 5.f, 0.f);
        moveBack = -cameraFront * 10.f;
    }

    _camera->position = hPosition + moveUp + moveBack;

}

void Core::bindCamera2H(glm::vec3 hPosition, glm::vec3 axis, float angle, bool isFPS) {
    glm::qua<float> temp = glm::qua<float>(cos(angle/2), axis*sin(angle/2)) * _camera->rotation;
    _camera->rotation = temp;

    glm::vec3 cameraFront = _camera->getFront();

    glm::vec3 moveUp;
    glm::vec3 moveBack;

    if (isFPS) {
        moveUp = glm::vec3(0.f, 0.5f, 0.f);
        moveBack = glm::vec3(0.f, 0.f, 0.f);
    } else {
        moveUp = glm::vec3(0.f, 5.f, 0.f);
        moveBack = -cameraFront * 10.f;
    }

    _camera->position = hPosition + moveUp + moveBack;

}
