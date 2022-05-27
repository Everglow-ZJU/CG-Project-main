#include "house.h"
const std::string modelPath = "../../../media/CasaSimples_2.obj";//house model
const std::string texturePath = "../../../media/casaSimples_D_3.png";//house texture
House::House(){
	/* init House. */
    _model.reset(new Model(modelPath));
	_model->scale = glm::vec3(100.0f, 100.0f, 100.0f);
	/* init Texture. */
	_texture.reset(new Texture2D(texturePath));
	/* init Shader. */
	//initShader();

    _Box = new HouseBox();
}
House::~House(){

}
void House::DrawHouse(Shader* _houseshader,const glm::mat4& projection, const glm::mat4& view){
	_houseshader->use();
	_houseshader->setMat4("projection", projection);
	_houseshader->setMat4("view", view);
	_houseshader->setMat4("model", _model->getModelMatrix());
		glActiveTexture(GL_TEXTURE0);
		_texture->bind();
		glActiveTexture(GL_TEXTURE1);
		_texture->bind();
	_model->draw();

    if (_firstDraw) {
        _Box->findMinAndMax(_model->getVertex(),_model->getModelMatrix());// * projection* view);
        _firstDraw = false;
    }
}