#include "Graphics.h"
#include "GameObject.h"
Graphics::Graphics()
{
	_type = ComponentType::Renderer;
	_appearance = nullptr;
}
void Graphics::UpdateComponent(float deltaTime)
{
	
}

void Graphics::Draw(ID3D11DeviceContext* pImmediateContext)
{

	//check if there is something to draw
	if (_appearance!=nullptr) {

		// Set vertex and index buffers
		pImmediateContext->IASetVertexBuffers(0, 1, _appearance->GetVertexBuffer(), _appearance->GetVertexStride(), _appearance->GetVertexBufferOffset());
		pImmediateContext->IASetIndexBuffer(_appearance->GetGeomentry().indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		pImmediateContext->DrawIndexed(_appearance->GetGeomentry().numberOfIndices, 0, 0);
	}
	else {
		if (_owner) {

			_appearance = _owner->GetComponent<Appearance>();

		}
	}


}
