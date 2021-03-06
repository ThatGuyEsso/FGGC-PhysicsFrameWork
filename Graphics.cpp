#include "Graphics.h"
#include "GameObject.h"
Graphics::Graphics()
{
	_type = ComponentType::Renderer;
	
}
void Graphics::UpdateComponent(float deltaTime)
{
	
}

void Graphics::DrawGUI()
{
	ImGui::SetNextWindowSize(ImVec2(500.0f, 10.0f));
	if (ImGui::Begin("Graphics Renderer")) {
	
	}
	ImGui::End();
}

void Graphics::Draw(ID3D11DeviceContext* pImmediateContext)
{
	Appearance * appearance= _owner->GetComponent<Appearance>();
	//check if there is something to draw
	if (appearance !=nullptr) {

		// Set vertex and index buffers
		pImmediateContext->IASetVertexBuffers(0, 1, appearance->GetVertexBuffer(), appearance->GetVertexStride(), appearance->GetVertexBufferOffset());
		pImmediateContext->IASetIndexBuffer(appearance->GetGeomentry().indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		pImmediateContext->DrawIndexed(appearance->GetGeomentry().numberOfIndices, 0, 0);
	}



}
