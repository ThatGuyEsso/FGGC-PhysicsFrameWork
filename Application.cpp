#include "Application.h"
#include "KD_Tree.h"
//imgui message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//passes windows messages to imgui
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) {
		return true;
	}

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool Application::HandleKeyboard(MSG msg)
{
	Vector3D cameraPosition = _camera->GetPosition();

	switch (msg.wParam)
	{
	case VK_UP:
		_cameraOrbitRadius = max(_cameraOrbitRadiusMin, _cameraOrbitRadius - (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_DOWN:
		_cameraOrbitRadius = min(_cameraOrbitRadiusMax, _cameraOrbitRadius + (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_RIGHT:
		_cameraOrbitAngleXZ -= _cameraSpeed;
		return true;
		break;

	case VK_LEFT:
		_cameraOrbitAngleXZ += _cameraSpeed;
		return true;
		break;
	}

	return false;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pVertexBuffer = nullptr;
	_pIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;
	CCWcullMode=nullptr;
	CWcullMode= nullptr;
	DSLessEqual = nullptr;
	RSCullNone = nullptr;
	 _WindowHeight = 0;
	 _WindowWidth = 0;


}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
    GetClientRect(_hWnd, &rc);
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }

	CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\stone.dds", nullptr, &_pTextureRV);
	CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\floor.dds", nullptr, &_pGroundTextureRV);
	//CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\Hercules_COLOR.dds", nullptr, &_pHerculesTextureRV);
	
    // Setup Camera
	Vector3D eye = Vector3D(0.0f, 2.0f, -1.0f);
	Vector3D at = Vector3D(0.0f, 2.0f, 0.0f);
	Vector3D up = Vector3D(0.0f, 1.0f, 0.0f);

	_camera = new Camera(eye, at, up, (float)_renderWidth, (float)_renderHeight, 0.01f, 200.0f);

	// Setup the scene's light
	basicLight.AmbientLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	basicLight.DiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	basicLight.SpecularLight = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	basicLight.SpecularPower = 20.0f;
	basicLight.LightVecW = XMFLOAT3(0.0f, 1.0f, -1.0f);

	//create new apperance instance

	


	GameObject* gameObject = new GameObject("Floor", new Transform(Vector3D(0.0f,0.5f, 0.0f),
		Vector3D(XMConvertToRadians(90.0f), 0.0f), Vector3D(15.0f, 15.0f, 15.0f)));
	gameObject->AddComponent(new Appearance(GetPlaneMesh(_pPlaneIndexBuffer, _pPlaneVertexBuffer), noSpecMaterial, _pGroundTextureRV));
	gameObject->AddComponent(new Graphics());


	gameObject->AddComponent(new RigidBody(gameObject->GetTransform(), Vector3D(), Vector3D(), false));
	gameObject->GetComponent<RigidBody>()->SetRigidBodyMode(RigidBody::BodyMode::Static);
	gameObject->CalculateCentreOfMass(PlaneVertices, 4);

	AABoxCollider* collider = new AABoxCollider(gameObject->GetTransform(), Vector3D());
	collider->SetHalfSize(Vector3D(10.0f, 0.25f, 10.0f));
	gameObject->AddComponent((Collider*)collider);
	_gameObjects.push_back(gameObject);


	for (auto i = 0; i < NUMBER_OF_CUBES; i++)
	{
		GameObject* gameObject = new GameObject("Sphere" + i);
		gameObject->AddComponent(new Appearance(GetSphereMesh(_pd3dDevice), shinyMaterial, _pTextureRV));
		gameObject->AddComponent(new Graphics());
		gameObject->SetScale(Vector3D(0.5f, 0.5f, 0.5f));
		gameObject->SetPosition(Vector3D(1.5f + (i * 1.5f), 1.0f, 1.0f));
		//gameObject->CalculateCentreOfMass(CubeVertices, 24);
		
		gameObject->AddComponent(new RigidBody(gameObject->GetTransform(), Vector3D(), Vector3D(), false));
		SphereCollider* collider = new SphereCollider(gameObject->GetTransform(),0.25f);
		gameObject->AddComponent((Collider*)collider);
		gameObject->GetComponent<RigidBody>()->SetInertiaTensorSphere(0.25f);
		_gameObjects.push_back(gameObject);
	
	}
	
	//gameObject = new GameObject("Cube" + _gameObjects.size()-1);
	//gameObject->AddComponent(new Appearance(GetCubeMesh(_pIndexBuffer,_pVertexBuffer), shinyMaterial, _pTextureRV));
	//gameObject->AddComponent(new Graphics());
	//gameObject->SetScale(Vector3D(0.25f, 0.25f, 0.25f));
	//gameObject->SetPosition(Vector3D(0.0f, 1.0f, 1.0f));
	////gameObject->CalculateCentreOfMass(CubeVertices, 24);

	//gameObject->AddComponent(new RigidBody(gameObject->GetTransform(), Vector3D(), Vector3D(), false));
	//collider = new AABoxCollider(gameObject->GetTransform(),Vector3D(0.2f, 0.2f, 0.2f));
	//gameObject->AddComponent((Collider*)collider);
	//gameObject->GetComponent<RigidBody>()->SetInertiaTensorBox(0.6,0.6f,0.6f);
	//_gameObjects.push_back(gameObject);

	//gameObject = new GameObject("Cube" + _gameObjects.size() - 1);
	//gameObject->AddComponent(new Appearance(GetCubeMesh(_pIndexBuffer, _pVertexBuffer), shinyMaterial, _pTextureRV));
	//gameObject->AddComponent(new Graphics());
	//gameObject->SetScale(Vector3D(0.2f, 0.2f, 0.2f));
	//gameObject->SetPosition(Vector3D(1.0f, 1.0f, 1.0f));
	////gameObject->CalculateCentreOfMass(CubeVertices, 24);

	//gameObject->AddComponent(new RigidBody(gameObject->GetTransform(), Vector3D(), Vector3D(), false));
	//collider = new AABoxCollider(gameObject->GetTransform(), Vector3D(0.2f, 0.2f, 0.2f));
	//gameObject->AddComponent((Collider*)collider);
	//gameObject->GetComponent<RigidBody>()->SetInertiaTensorBox(0.6, 0.6f, 0.6f);
	//_gameObjects.push_back(gameObject);
	return S_OK;
}

HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "VS", "vs_4_0", &pVSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{	
		pVSBlob->Release();
        return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "PS", "ps_4_0", &pPSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

    if (FAILED(hr))
        return hr;
	
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                        pVSBlob->GetBufferSize(), &_pVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
        return hr;

    // Set the input layout
    _pImmediateContext->IASetInputLayout(_pVertexLayout);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);

	return hr;
}

HRESULT Application::InitVertexBuffer()
{
	HRESULT hr;

    // Create vertex buffer
  
    D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = CubeVertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);

    if (FAILED(hr))
        return hr;

	// Create vertex buffer

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = PlaneVertices;

	hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneVertexBuffer);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Application::InitIndexBuffer()
{
	HRESULT hr;

    // Create index buffer
   
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;     
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = CubeIndices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);

    if (FAILED(hr))
        return hr;


	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = PlaneIndices;
	hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneIndexBuffer);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

void Application::AddThrust(Vector3D force)
{
	GameObject* object = GetSelectedGameObject();

	if(object->GetType()== "Cube"){
		object->GetComponent<RigidBody>()->ApplyForce(force);
	}
}

void Application::RotateGameObject(Vector3D rotation, float scale)
{
	GetSelectedGameObject()->GetTransform()->Rotate(rotation, scale);
}

GameObject* Application::GetSelectedGameObject()
{
	return _gameObjects[_activeGameObjectIndex];
}

void Application::ToggleGravity()
{
	_gameObjects[_activeGameObjectIndex]->GetComponent<RigidBody>()
		->ToggleGravity(!_gameObjects[_activeGameObjectIndex]->GetComponent<RigidBody>()->UsesGravity());
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    _hInst = hInstance;
    RECT rc = {0, 0, 960, 540};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(L"TutorialWindowClass", L"FGGC Semester 2 Framework", WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                         nullptr);
    if (!_hWnd)
		return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);

    return S_OK;
}

HRESULT Application::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	UINT sampleCount = 4;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _renderWidth;
    sd.BufferDesc.Height = _renderHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = sampleCount;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_renderWidth;
    vp.Height = (FLOAT)_renderHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

	InitShadersAndInputLayout();

	InitVertexBuffer();
	InitIndexBuffer();

    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

    if (FAILED(hr))
        return hr;

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = _renderWidth;
	depthStencilDesc.Height = _renderHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = sampleCount;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

	// Rasterizer
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	_pd3dDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CWcullMode);
	_input->instance();
	_guiManager = new GUIManager();
	_guiManager->Init(_hWnd, _pd3dDevice, _pImmediateContext);
    return S_OK;
}

void Application::Cleanup()
{
    if (_pImmediateContext) _pImmediateContext->ClearState();
	if (_pSamplerLinear) _pSamplerLinear->Release();

	if (_pTextureRV) _pTextureRV->Release();

	if (_pGroundTextureRV) _pGroundTextureRV->Release();

    if (_pConstantBuffer) _pConstantBuffer->Release();

    if (_pVertexBuffer) _pVertexBuffer->Release();
    if (_pIndexBuffer) _pIndexBuffer->Release();
	if (_pPlaneVertexBuffer) _pPlaneVertexBuffer->Release();
	if (_pPlaneIndexBuffer) _pPlaneIndexBuffer->Release();

    if (_pVertexLayout) _pVertexLayout->Release();
    if (_pVertexShader) _pVertexShader->Release();
    if (_pPixelShader) _pPixelShader->Release();
    if (_pRenderTargetView) _pRenderTargetView->Release();
    if (_pSwapChain) _pSwapChain->Release();
    if (_pImmediateContext) _pImmediateContext->Release();
    if (_pd3dDevice) _pd3dDevice->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();

	if (DSLessEqual) DSLessEqual->Release();
	if (RSCullNone) RSCullNone->Release();

	if (CCWcullMode) CCWcullMode->Release();
	if (CWcullMode) CWcullMode->Release();

	if (_camera)
	{
		delete _camera;
		_camera = nullptr;
	}

	for (auto gameObject : _gameObjects)
	{
		if (gameObject)
		{
			delete gameObject;
			gameObject = nullptr;
		}
	}
	_gameObjects.clear();
	_input = nullptr;
	_guiManager = nullptr;

}



void Application::CycleBetweenObjectByType(string type)
{
	//increment gameobjectIndex
	_activeGameObjectIndex++;

	//If index is longer than length of game object list loop back round
	if (_activeGameObjectIndex >= _gameObjects.size()) _activeGameObjectIndex = 0;


	if (_gameObjects[_activeGameObjectIndex]->GetType() != std::string(type)) {

		for (int i = _activeGameObjectIndex; i < _gameObjects.size(); i++) {
			DebugHelp().OutPutValue("Looking",i);
			if (_gameObjects[i]->GetType() == std::string(type)) {
				DebugHelp().OutPutText("New type found");
				_activeGameObjectIndex = i;
				break;
			}
		}



	}
	else {
		DebugHelp().OutPutText("Already has type");
	}

	DebugHelp().OutPutText(_gameObjects[_activeGameObjectIndex]->GetType().c_str());
}

void Application::RenderGUI()
{
	//Start new frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//call  GUI instances
	GetSelectedGameObject()->DisplayGUI();

	//render data from guis
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Application::Update()
{
	_guiManager->IsGuiTyping();
	_input->Update();
	// Update our time
	static float deltaTime = 0.0f;
	static DWORD dwTimeStart = 0;

	DWORD dwTimeCur = GetTickCount64();

	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;

	deltaTime += (dwTimeCur - dwTimeStart) / 1000.0f;//milli seconds elapsed

	if (deltaTime < FPS) {
		return; // wait  till 60 frames have passed
	}
	deltaTime = 1.0f/deltaTime;//frame per second


	// Move gameobject
	if (_input->GetKey('W'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(0.0f, 0.5f, 0.0f), deltaTime);
	
	}
	if (_input->GetKey('D'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(5.0f, 0.0f, 0.0f), deltaTime);

	}
	if (_input->GetKey('A'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(-5.0f, 0.0f, 0.0f), deltaTime);

	}
	if (_input->GetKey('S'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(0.0f, -0.5f, 0.0f), deltaTime);

	}

	if (_input->GetKey('E'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(0.5f, 0.5f, 0.0f), deltaTime);

	}
	if (_input->GetKey('Q'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(-0.5f, 0.5f, 0.0f), deltaTime);

	}

	if (_input->GetKey('X'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(0.5f, -0.5f, 0.0f), deltaTime);

	}

	if (_input->GetKey('Z'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyRotForce(Vector3D(0.0f, 0.0f, 10000.0f), Vector3D(-0.5f, -0.5f, 0.0f), deltaTime);

	}

	// Move gameobject
	if (_input->GetKey('I'))
	{
		RigidBody* rb = GetSelectedGameObject()->GetComponent<RigidBody>();

		if (rb) {

			rb->ApplyForce(Vector3D(0.0f, 0.0f, 2000.0f)* deltaTime);
		}

	}
	if (_input->GetKey('K'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyForce(Vector3D(0.0f, 0.0f, -2000.0f) * deltaTime);

	}
	if (_input->GetKey('L'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyForce(Vector3D(2000.0f, 0.0f, 0.0f) * deltaTime);

	}
	if (_input->GetKey('J'))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyForce(Vector3D(-2000.0f, 0.0f, 0.0f) * deltaTime);

	}
	if (_input->GetKey('G'))
	{
		ToggleGravity();

	}
	if (_input->GetKey('Y'))
	{

		_gameObjects[_activeGameObjectIndex]->RemoveComponent<Graphics>();


	}
	if (_input->GetKey('U'))
	{

		_gameObjects[_activeGameObjectIndex]->AddComponent(new Graphics());

	}

	if (_input->GetKey('T'))
	{
		_gameObjects[_activeGameObjectIndex]->RemoveComponent<Appearance>();
		_gameObjects[_activeGameObjectIndex]->AddComponent(new Appearance(GetCubeMesh(_pIndexBuffer, _pVertexBuffer), shinyMaterial, _pTextureRV));

	}

	if (_input->GetKey('R'))
	{
		_gameObjects[_activeGameObjectIndex]->RemoveComponent<Appearance>();
		_gameObjects[_activeGameObjectIndex]->AddComponent(new Appearance(GetSphereMesh(_pd3dDevice), shinyMaterial, _pTextureRV));

	}




	if (_input->GetKey(VK_SHIFT))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyForce(Vector3D(0.0f, -5000.0f, 0.0f) * deltaTime);

	}
	if (_input->GetKey(VK_SPACE))
	{
		GetSelectedGameObject()->GetComponent<RigidBody>()
			->ApplyForce(Vector3D(0.0f, 5000.0f, 0.0f) * deltaTime);

	}



	
	if (_input->GetKey('C')) {
		CycleBetweenObjectByType(std::string("Moveable"));
	}

	// Update camera
	float angleAroundZ = XMConvertToRadians(_cameraOrbitAngleXZ);

	float x = _cameraOrbitRadius * cos(angleAroundZ);
	float z = _cameraOrbitRadius * sin(angleAroundZ);

	Vector3D cameraPos = _camera->GetPosition();
	cameraPos.x = x;
	cameraPos.z = z;

	_camera->SetPosition(cameraPos);
	_camera->Update();

	// Update objects
	for (auto gameObject : _gameObjects)
	{
		gameObject->Update(deltaTime);
	}

	//resets time since last frame
	deltaTime -= FPS;


	
	KD_Tree* tree = new KD_Tree();

	tree->AddNodeRecursive(_gameObjects, KD_Tree::Axis::X_axis, nullptr);

	std::vector<KD_Tree::KD_Node*> leaves = tree->GetLeaves();

	for (KD_Tree::KD_Node* node : leaves) {
		//If object set doesnt have more than 2 objects don't check for collisions
		if (node->objectSet.size() > 1) {
			for (int i = 0; i < node->objectSet.size(); i++) {
				Collider* currCollider;

				if ((currCollider = node->objectSet[i]->GetComponent<Collider>()) != nullptr) {
					for (int y = 0; y < node->objectSet.size(); y++) {

						//Don't check itself
						if (i != y) {
							if (currCollider->CollisionCheck(node->objectSet[y]->GetComponent<Collider>())) {

								DebugHelp().OutPutText("COLLISION");


							}
						}
					}
				}

			}
		}
	}


}

void Application::Draw()
{
    //
    // Clear buffers
    //

	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
	_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //
    // Setup buffers and render scene
    //

	_pImmediateContext->IASetInputLayout(_pVertexLayout);

	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);

	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);

    ConstantBuffer cb;

	XMFLOAT4X4 viewAsFloats = _camera->GetView();
	XMFLOAT4X4 projectionAsFloats = _camera->GetProjection();

	XMMATRIX view = XMLoadFloat4x4(&viewAsFloats);
	XMMATRIX projection = XMLoadFloat4x4(&projectionAsFloats);

	cb.View = XMMatrixTranspose(view);
	cb.Projection = XMMatrixTranspose(projection);
	
	cb.light = basicLight;
	cb.EyePosW = XMFLOAT3(_camera->GetPosition().x, _camera->GetPosition().y, _camera->GetPosition().z);

	// Render all scene objects
	for (auto gameObject : _gameObjects)
	{
		// Get render material
		Appearance* currAppearance = gameObject->GetComponent<Appearance>();
		if (currAppearance) {

			Material material = currAppearance->GetMaterial();
			// Copy material to shader
			cb.surface.AmbientMtrl = material.ambient;
			cb.surface.DiffuseMtrl = material.diffuse;
			cb.surface.SpecularMtrl = material.specular;

			// Set world matrix
			cb.World = XMMatrixTranspose(gameObject->GetWorldMatrix());

			// Set texture
			if (currAppearance->HasTexture())
			{
				ID3D11ShaderResourceView* textureRV = currAppearance->GetTextureRV();
				_pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
				cb.HasTexture = 1.0f;
			}
			else
			{
				cb.HasTexture = 0.0f;
			}

			// Update constant buffer
			_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

			// Draw object
			Graphics* renderer = gameObject->GetComponent<Graphics>();
			if(renderer)
				renderer->Draw(_pImmediateContext);
		}


	}
	RenderGUI();
    //
    // Present our back buffer to our front buffer
    //
    _pSwapChain->Present(0, 0);
}