#include "suku_drawing.h"
#include "suku_foundation.h"
#include "game_saving.h"

namespace suku
{
	ID2D1Factory* pD2DFactory = nullptr;	// Direct2D factory
	ID2D1HwndRenderTarget* pMainRenderTarget = nullptr;	// Render target
	IWICImagingFactory* pIWICFactory;

	void suku_drawing_init(HWND _hWnd)
	{
		// Init WIC resource
		HRESULT hr = CoInitialize(nullptr);
		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(
				CLSID_WICImagingFactory1,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&pIWICFactory)
			);
		}

		// Init D2D resource

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		if (FAILED(hr))
		{
			ERRORWINDOW_GLOBAL("Failed to create D2D factory");
			return;
		}

		RECT rc;
		GetClientRect(_hWnd, &rc);

		hr = pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				_hWnd,
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
			),
			&pMainRenderTarget
		);
		if (FAILED(hr))
		{
			ERRORWINDOW_GLOBAL("Failed to create render target");
			return;
		}
	}

	void suku_drawing_uninit()
	{
		CoUninitialize();
		SAFE_RELEASE(pMainRenderTarget);
		SAFE_RELEASE(pD2DFactory);
	}

	Transform translation(float _shiftX, float _shiftY)
	{
		return Transform(D2D1::Matrix3x2F::Translation(_shiftX, _shiftY));
	}

	Transform rotation(float _rotateCenterX, float _rotateCenterY, float _angle)
	{
		return Transform(D2D1::Matrix3x2F::Rotation(_angle, D2D1::Point2F(_rotateCenterX, _rotateCenterY)));
	}

	Transform scale(float _centerX, float _centerY, float _scaleX, float _scaleY)
	{
		return Transform(D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(_scaleX, _scaleY),
			D2D1::Point2F(_centerX, _centerY)
		));
	}

	Transform skew(float _centerX, float _centerY, float _angleX, float _angleY)
	{
		return Transform(D2D1::Matrix3x2F::Skew(
			_angleX, _angleY,
			D2D1::Point2F(_centerX, _centerY)
		));
	}

	void setPaintingTransform(Transform _transform)
	{
		pMainRenderTarget->SetTransform(_transform.matrix);
	}

	void beginDraw(HWND hWnd)
	{
		pMainRenderTarget->BeginDraw();
		pMainRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}

	void PaintLayer::newLayer(UINT _width, UINT _height)
	{
		if (!pMainRenderTarget)
		{
			ERRORWINDOW("Main render target is null");
			return;
		}
		if (pBitmapRenderTarget_)
		{
			SAFE_RELEASE(pBitmapRenderTarget_);
		}
		HRESULT hr = pMainRenderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF((FLOAT)_width, (FLOAT)_height),
			&pBitmapRenderTarget_
		);
		if (FAILED(hr))
			ERRORWINDOW("Failed to create compatible render target");
	}

	void PaintLayer::clear(Color _backgroundcolor)
	{
		pBitmapRenderTarget_->Clear(D2D1::ColorF(
			_backgroundcolor.r(),
			_backgroundcolor.g(),
			_backgroundcolor.b(),
			_backgroundcolor.alpha
		));
	}

	void PaintLayer::beginDraw()
	{
		pBitmapRenderTarget_->BeginDraw();
	}

	Bitmap* PaintLayer::endDraw()
	{
		pBitmapRenderTarget_->EndDraw();
		ID2D1Bitmap* pBitmap = nullptr;
		HRESULT hr = pBitmapRenderTarget_->GetBitmap(&pBitmap);
		if (FAILED(hr))
		{
			ERRORWINDOW("Failed to get bitmap from bitmap render target");
			return nullptr;
		}
		return new Bitmap(pBitmap);
	}

	void PaintLayer::paintBitmap(const Bitmap& _bitmap, Transform _transform, float _alpha)
	{
		pBitmapRenderTarget_->SetTransform(_transform.matrix);
		pBitmapRenderTarget_->DrawBitmap(_bitmap.d2dBitmap_, D2D1::RectF(0, 0, _bitmap.getWidth(), _bitmap.getHeight()), _alpha);
	}

	void PaintLayer::paintShape(const Shape& _shape, Transform _transform, float _alpha)
	{
		// WIP
	}

	void endDraw()
	{
		pMainRenderTarget->EndDraw();
	}

	void clearScreen()
	{
		pMainRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}

	void SwapByte(BYTE* _a, BYTE* _b)
	{
		BYTE t = *_a;
		*_a = *_b;
		*_b = t;
	}

	ID2D1Bitmap* GetBlendedBitmapFromFile(
		IWICImagingFactory* pIWICFactory,
		ID2D1RenderTarget* pRenderTarget,
		const wchar_t* uri,
		const D2D1_COLOR_F& color)
	{
		ID2D1Bitmap* pBitmap = NULL;
		IWICBitmapDecoder* pDecoder = NULL;
		IWICBitmapFrameDecode* pSource = NULL;
		IWICBitmap* pWIC = NULL;
		IWICFormatConverter* pConverter = NULL;
		IWICBitmapScaler* pScaler = NULL;
		UINT					originalWidth = 0;
		UINT					originalHeight = 0;

		HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
			uri,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pSource->GetSize(&originalWidth, &originalHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateBitmapFromSourceRect(
				pSource, 0, 0, (UINT)originalWidth, (UINT)originalHeight, &pWIC);
		}

		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateFormatConverter(&pConverter);
		}
		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateBitmapScaler(&pScaler);
		}
		if (SUCCEEDED(hr))
		{
			hr = pScaler->Initialize(pWIC, (UINT)originalWidth, (UINT)originalHeight, WICBitmapInterpolationModeCubic);
		}
		if (SUCCEEDED(hr))
		{
			hr = pConverter->Initialize(
				pScaler,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = pRenderTarget->CreateBitmapFromWicBitmap(
				pConverter,
				NULL,
				&pBitmap
			);
		}

		SAFE_RELEASE(pConverter);
		SAFE_RELEASE(pScaler);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);

		return pBitmap;
	}

	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	)
	{
		HRESULT hr = S_OK;

		IWICBitmapDecoder* pDecoder = NULL;
		IWICBitmapFrameDecode* pSource = NULL;
		IWICStream* pStream = NULL;
		IWICFormatConverter* pConverter = NULL;
		IWICBitmapScaler* pScaler = NULL;
		IWICBitmap* pWIC = NULL;

		hr = pIWICFactory->CreateDecoderFromFilename(
			uri,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder
		);
		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pSource);
		}
		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateFormatConverter(&pConverter);
		}
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					float scalar = static_cast<float>(destinationHeight) / static_cast<float>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<float>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					float scalar = static_cast<float>(destinationWidth) / static_cast<float>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<float>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
					);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);
				}
			}
		}
		if (SUCCEEDED(hr) && pConverter != 0)
		{
			hr = pRenderTarget->CreateBitmapFromWicBitmap(
				pConverter,
				nullptr,
				ppBitmap
			);
		}
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);
		SAFE_RELEASE(pStream);
		SAFE_RELEASE(pConverter);
		SAFE_RELEASE(pScaler);
		return hr;
	}

	HRESULT loadWICBitmap(IWICBitmap** _ppWicBitmap, const wchar_t* _uri)
	{
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pSource = nullptr;
		UINT					originalWidth = 0;
		UINT					originalHeight = 0;

		HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
			_uri,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pSource->GetSize(&originalWidth, &originalHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateBitmapFromSourceRect(
				pSource, 0, 0, (UINT)originalWidth, (UINT)originalHeight, _ppWicBitmap);
		}

		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);
		return hr;
	}

	HRESULT loadWICBitmap(IWICBitmap** _ppWicBitmap, const wchar_t* _uri, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pSource = nullptr;

		HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
			_uri,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateBitmapFromSourceRect(
				pSource, _x, _y, _width, _height, _ppWicBitmap);
		}

		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);
		return hr;
	}

	HRESULT createWICBitmap(IWICBitmap** _ppWicBitmap, UINT _width, UINT _height)
	{
		IWICImagingFactory* pWicFactory = nullptr;
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pWicFactory));
		if (FAILED(hr)) return hr;

		hr = pWicFactory->CreateBitmap(
			_width,
			_height,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapCacheOnLoad,
			_ppWicBitmap);

		SAFE_RELEASE(pWicFactory);
		return hr;
	}

	HRESULT getD2DBitmap(IWICBitmap* _pWicBitmap, ID2D1Bitmap** _ppD2dBitmap)
	{
		if (!_pWicBitmap) return S_FALSE;

		IWICBitmapDecoder* pDecoder = NULL;
		IWICBitmapFrameDecode* pSource = NULL;
		IWICFormatConverter* pConverter = NULL;
		IWICBitmapScaler* pScaler = NULL;
		UINT width = 0, height = 0;
		HRESULT hr = S_OK;

		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateFormatConverter(&pConverter);
		}
		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateBitmapScaler(&pScaler);
		}
		if (SUCCEEDED(hr))
		{
			hr = _pWicBitmap->GetSize(&width, &height);
		}
		if (SUCCEEDED(hr))
		{
			hr = pScaler->Initialize(_pWicBitmap, width, height, WICBitmapInterpolationModeCubic);
		}
		if (SUCCEEDED(hr))
		{
			hr = pConverter->Initialize(
				pScaler,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = pMainRenderTarget->CreateBitmapFromWicBitmap(
				pConverter,
				NULL,
				_ppD2dBitmap
			);
		}

		SAFE_RELEASE(pConverter);
		SAFE_RELEASE(pScaler);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);

		return hr;
	}

	HRESULT getWICBitmap(ID2D1Bitmap* _pD2dBitmap, IWICBitmap** _ppWicBitmap)
	{
		if (!_pD2dBitmap || !_ppWicBitmap)
			return E_POINTER;

		*_ppWicBitmap = nullptr;

		D2D1_SIZE_U size = _pD2dBitmap->GetPixelSize();
		HRESULT hr;

		IWICBitmap* pWicBitmap = nullptr;
		hr = createWICBitmap(&pWicBitmap, size.width, size.height);
		if (FAILED(hr)) return hr;

		ID2D1Bitmap* pDstD2dBitmap = nullptr;
		hr = pMainRenderTarget->CreateBitmapFromWicBitmap(pWicBitmap, nullptr, &pDstD2dBitmap);
		if (FAILED(hr)) { pWicBitmap->Release(); return hr; }

		hr = pDstD2dBitmap->CopyFromBitmap(
			nullptr,
			_pD2dBitmap,
			nullptr
		);

		if (SUCCEEDED(hr))
		{
			*_ppWicBitmap = pWicBitmap;
		}
		else
		{
			pWicBitmap->Release();
		}

		pDstD2dBitmap->Release();
		return hr;
	}

	Color** getPixelDetailFromWICBitmap(IWICBitmap* _pWicBitmap)
	{
		if (_pWicBitmap == nullptr)
			return nullptr;
		auto [width, height] = getBitmapSize(_pWicBitmap);
		Color** pixelArrayPointer = new_memory_2d<Color>(width, height);
		if (pixelArrayPointer != nullptr)
		{
			IWICBitmapLock* pILock = nullptr;
			WICRect rcLock = { 0, 0, (int)width, (int)height };
			_pWicBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);

			UINT cbBufferSize = 0;
			BYTE* pv = nullptr;

			pILock->GetDataPointer(&cbBufferSize, &pv);
			if (pv)
			{
				for (unsigned int i = 0; i < width * 4; i += 4)
					for (unsigned int j = 0; j < height; j++)
					{
						UINT k = i + j * width * 4;
						if (k + 3 < cbBufferSize)
						{
							pixelArrayPointer[i / 4][j].setRGB(pv[k + 2], pv[k + 1], pv[k]);
							pixelArrayPointer[i / 4][j].alpha = (float)pv[k + 3] / 255.0f;
						}
					}
			}
			pILock->Release();
		}
		return pixelArrayPointer;
	}

	Color** getPixelDetailFromWICBitmap(IWICBitmap* _pWicBitmap, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		if (_pWicBitmap == nullptr)
			return nullptr;
		auto [width, height] = getBitmapSize(_pWicBitmap);
		if (_x + _width > width || _y + _height > height)
			return nullptr;
		Color** pixelArrayPointer = new_memory_2d<Color>(_width, _height);
		if (pixelArrayPointer != nullptr)
		{
			IWICBitmapLock* pILock = nullptr;
			WICRect rcLock = { 0, 0, (int)width, (int)height };
			_pWicBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);

			UINT cbBufferSize = 0;
			BYTE* pv = nullptr;

			pILock->GetDataPointer(&cbBufferSize, &pv);
			if (pv)
			{
				for (unsigned int i = _x * 4; i < (_x + _width) * 4; i += 4)
					for (unsigned int j = _y; j < _y + _height; j++)
					{
						UINT k = i + j * width * 4;
						if (k + 3 < cbBufferSize)
						{
							pixelArrayPointer[i / 4 - _x][j - _y].setRGB(pv[k + 2], pv[k + 1], pv[k]);
							pixelArrayPointer[i / 4 - _x][j - _y].alpha = (float)pv[k + 3] / 255.0f;
						}
					}
			}
			pILock->Release();
		}
		return pixelArrayPointer;
	}

	ID2D1Brush* createSolidColorBrush(const Color _color)
	{
		ID2D1SolidColorBrush* newBrush;
		pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r(), _color.g(), _color.b(), _color.alpha),
			&newBrush
		);
		return newBrush;
	}

	std::pair<UINT, UINT> getBitmapSize(IWICBitmap* _pBitmap, HRESULT* _pHResult)
	{
		if (!_pBitmap) return { 0,0 };
		UINT width, height;
		*_pHResult = _pBitmap->GetSize(&width, &height);
		return { width, height };
	}

	std::pair<UINT, UINT> getBitmapSize(ID2D1Bitmap* _pBitmap)
	{
		if (!_pBitmap) return { 0,0 };
		D2D1_SIZE_U size = _pBitmap->GetPixelSize();
		return { size.width, size.height };
	}

	std::pair<UINT, UINT> getBitmapSize(const Bitmap& _ppBitmap)
	{
		if (!_ppBitmap.isValid()) return { 0,0 };
		UINT width = _ppBitmap.getWidth();
		UINT height = _ppBitmap.getHeight();
		return { width, height };
	}

	void getHitAreaFromBitmap(bool** _ppHitArea, const Bitmap& _bitmap, float _alphaThreshold)
	{
		if (!_ppHitArea)
			return;
		UINT width = _bitmap.getWidth();
		BYTE alphaThreshold = (BYTE)(_alphaThreshold * 255.0f);
		_bitmap.viewPixelDetail([&](const UINT _x, const UINT _y, const Color& _color)
			{
				if (_color.alpha <= alphaThreshold)
					_ppHitArea[_x][_y] = 0;
				else _ppHitArea[_x][_y] = 1;
			});
	}
	/*
	void drawBitmap(ID2D1Bitmap* _ppBitmap, float _x, float _y, float _width, float _height, float _alpha,
		float _angle, float _rotatingCenterX, float _rotatingCenterY)
	{

		if (!_ppBitmap) return;

		D2D1::Matrix3x2F transformMatrix;

		transformMatrix = D2D1::Matrix3x2F::Rotation(_angle, D2D1::Point2F(_rotatingCenterX, _rotatingCenterY));
		if (_width < 0)
			transformMatrix.SetProduct(transformMatrix,
				D2D1::Matrix3x2F::Scale(-1.0, 1.0, D2D1::Point2F(_x - _width / 2, _y + _height / 2)));
		if (_height < 0)
			transformMatrix.SetProduct(transformMatrix,
				D2D1::Matrix3x2F::Scale(1.0, -1.0, D2D1::Point2F(_x + _width / 2, _y - _height / 2)));

		pMainRenderTarget->SetTransform(transformMatrix);
		pMainRenderTarget->drawBitmap(
			_ppBitmap,
			D2D1::RectF(
				_x,
				_y,
				_x + fabs(_width),
				_y + fabs(_height)),
			_alpha
		);
	}
	*/
	void drawBitmap(ID2D1RenderTarget* _renderTarget, ID2D1Bitmap* _pBitmap, float _x, float _y,
		float _width, float _height, float _alpha, Transform _transform)
	{
		if (!_pBitmap) return;
		setPaintingTransform(_transform);
		_renderTarget->DrawBitmap(
			_pBitmap,
			D2D1::RectF(
				_x, _y,
				_x + fabs(_width),
				_y + fabs(_height)
			),
			_alpha
		);
	}

	void drawBitmap(ID2D1RenderTarget* _renderTarget, ID2D1Bitmap* _pBitmap, float _width, float _height, float _alpha, Transform _transform)
	{
		if (!_pBitmap) return;
		setPaintingTransform(_transform);
		_renderTarget->DrawBitmap(
			_pBitmap,
			D2D1::RectF(
				0.0f, 0.0f,
				fabs(_width),
				fabs(_height)
			),
			_alpha
		);
	}

	Shape::Shape()
	{
		originalGeometry = nullptr;
		transform = Transform();
		currentGeometry = nullptr;
	}

	Shape::Shape(const Shape& _x)
	{
		transform = _x.transform;
		if (_x.originalGeometry)
		{
			originalGeometry = _x.originalGeometry;
			originalGeometry->AddRef();
			if (_x.currentGeometry)
			{
				currentGeometry = _x.currentGeometry;
				currentGeometry->AddRef();
			}
			else
			{
				pD2DFactory->
					CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
			}
		}
		else
		{
			originalGeometry = nullptr;
			currentGeometry = nullptr;
		}
	}

	Shape::Shape(Shape&& _x) noexcept
	{
		transform = _x.transform;
		originalGeometry = _x.originalGeometry;
		currentGeometry = _x.currentGeometry;
		_x.originalGeometry = nullptr;
		_x.currentGeometry = nullptr;
	}

	Shape::Shape(ID2D1Geometry* _geometry, Transform _transform)
	{
		originalGeometry = nullptr;
		transform = Transform();
		currentGeometry = nullptr;
		setOriginalGeometry(_geometry);
	}

	void Shape::join()
	{
		SAFE_ADDREF(originalGeometry);
		SAFE_ADDREF(currentGeometry);
	}

	Shape::~Shape()
	{
		SAFE_RELEASE(originalGeometry);
		SAFE_RELEASE(currentGeometry);
	}

	void Shape::setOriginalGeometry(ID2D1Geometry* _geometry)
	{
		SAFE_RELEASE(originalGeometry);
		SAFE_RELEASE(currentGeometry);
		if (_geometry)
		{
			_geometry->AddRef();
			originalGeometry = _geometry;
			pD2DFactory->CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
		}
		else
			originalGeometry = currentGeometry = nullptr;
	}

	void Shape::setTransform(Transform _transform)
	{
		transform = _transform;
		SAFE_RELEASE(currentGeometry);
		pD2DFactory->CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
	}

	void Shape::setFill(Color _color)
	{

	}

	void Shape::setOutline(Color _color)
	{
	}

	void Shape::paint(float _x, float _y, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth,
		ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y));
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y) + _paintingTransform);
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	void Shape::paint(Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(_paintingTransform);
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	Bitmap* Shape::paintOnBitmap(const Bitmap& _bitmap, float _x, float _y, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (!_bitmap.d2dBitmap_ || !currentGeometry)
			return nullptr;

		ID2D1BitmapRenderTarget* pBitmapRenderTarget = nullptr;
		HRESULT hr = pMainRenderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF((FLOAT)_bitmap.getWidth(), (FLOAT)_bitmap.getHeight()),
			&pBitmapRenderTarget
		);

		if (SUCCEEDED(hr) && pBitmapRenderTarget)
		{
			pBitmapRenderTarget->BeginDraw();
			pBitmapRenderTarget->DrawBitmap(_bitmap.d2dBitmap_, D2D1::RectF(0, 0, (FLOAT)_bitmap.getWidth(), (FLOAT)_bitmap.getHeight()));
			pBitmapRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(_x, _y));

			if (_outlineBrush)
				pBitmapRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);

			if (_fillBrush)
				pBitmapRenderTarget->FillGeometry(currentGeometry, _fillBrush);

			hr = pBitmapRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				ERRORWINDOW_GLOBAL("Render target needs to be recreated");
				pBitmapRenderTarget->Release();
				return nullptr;
			}

			ID2D1Bitmap* pBitmapResult = nullptr;
			Bitmap* resultBitmap = nullptr;
			hr = pBitmapRenderTarget->GetBitmap(&pBitmapResult);
			if (SUCCEEDED(hr) && pBitmapResult)
			{
				resultBitmap = new Bitmap(pBitmapResult);
				pBitmapResult->Release();
			}
			pBitmapRenderTarget->Release();
			return resultBitmap;
		}
		else
		{
			ERRORWINDOW_GLOBAL("Failed to create compatible render target for bitmap painting");
			return nullptr;
		}
	}

	void Shape::paint(float _x, float _y)
	{
		paint(_x, _y, fillBrush_, outlineBrush_, outlineWidth_, outlineStrokeStyle_);
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform)
	{
		paint(_x, _y, _paintingTransform, fillBrush_, outlineBrush_, outlineWidth_, outlineStrokeStyle_);
	}

	void Shape::paint(Transform _paintingTransform)
	{
		paint(_paintingTransform, fillBrush_, outlineBrush_, outlineWidth_, outlineStrokeStyle_);
	}

	bool Shape::isCrashed(Shape& _x)
	{
		D2D1_GEOMETRY_RELATION result;
		currentGeometry->CompareWithGeometry(_x.currentGeometry, nullptr, &result);
		if (result == D2D1_GEOMETRY_RELATION_DISJOINT)
			return false;
		else return true;
	}

	Shape& Shape::operator=(const Shape& _x)
	{
		transform = _x.transform;
		SAFE_RELEASE(originalGeometry);
		SAFE_RELEASE(currentGeometry);
		if (_x.originalGeometry)
		{
			originalGeometry = _x.originalGeometry;
			originalGeometry->AddRef();
			if (_x.currentGeometry)
			{
				currentGeometry = _x.currentGeometry;
				currentGeometry->AddRef();
			}
			else
			{
				pD2DFactory->
					CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
			}
		}
		else
		{
			originalGeometry = nullptr;
			currentGeometry = nullptr;
		}
		return (*this);
	}

	Shape& Shape::operator=(Shape&& _x) noexcept
	{
		transform = _x.transform;
		SAFE_RELEASE(originalGeometry);
		SAFE_RELEASE(currentGeometry);
		originalGeometry = _x.originalGeometry;
		currentGeometry = _x.currentGeometry;
		_x.originalGeometry = nullptr;
		_x.currentGeometry = nullptr;
		return (*this);
	}

	Shape Shape::operator-(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_EXCLUDE,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		SAFE_RELEASE(resGeometrySink);

		return Shape(resGeometry);
	}

	Shape Shape::operator&(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_INTERSECT,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		SAFE_RELEASE(resGeometrySink);

		return Shape(resGeometry);
	}

	Shape Shape::operator|(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_UNION,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		SAFE_RELEASE(resGeometrySink);

		return Shape(resGeometry);
	}

	Shape Shape::operator^(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_XOR,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		SAFE_RELEASE(resGeometrySink);

		return Shape(resGeometry);
	}

	SquareShape::SquareShape(float _length, float _startX, float _startY, Transform _transform)
		:length(_length), startX(_startX), startY(_startY)
	{
		ID2D1RectangleGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_startX, _startY,
				_startX + _length - 1.0f, _startY + _length - 1.0f
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		SAFE_RELEASE(newGeometry);
	}

	RectangleShape::RectangleShape(float _width, float _height, float _startX, float _startY, Transform _transform)
		:width(_width), height(_height), startX(_startX), startY(_startY)
	{
		ID2D1RectangleGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_startX, _startY,
				_startX + _width - 1.0f, _startY + _height - 1.0f
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		SAFE_RELEASE(newGeometry);
	}

	CircleShape::CircleShape(float _radius, float _startX, float _startY, Transform _transform)
		:radius(_radius), startX(_startX), startY(_startY)
	{
		ID2D1EllipseGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radius + _startX, _radius + _startY
				),
				_radius, _radius
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		SAFE_RELEASE(newGeometry);
	}

	EllipseShape::EllipseShape(float _radiusX, float _radiusY, float _startX, float _startY, Transform _transform)
		:radiusX(_radiusX), radiusY(_radiusY), startX(_startX), startY(_startY)
	{
		ID2D1EllipseGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radiusX + _startX, _radiusY + _startY
				),
				_radiusX, _radiusY
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		SAFE_RELEASE(newGeometry);
	}

	Transform::Transform()
	{
		matrix = D2D1::Matrix3x2F::Identity();
	}

	Transform::Transform(D2D1::Matrix3x2F _matrix)
	{
		matrix = _matrix;
	}

	void Transform::transformPoint(float* _pX, float* _pY)
	{
		auto transformedPoint = matrix.TransformPoint(D2D1::Point2F(*_pX, *_pY));
		*_pX = transformedPoint.x;
		*_pY = transformedPoint.y;
	}

	Vector Transform::transformPoint(float _x, float _y)
	{
		auto transformedPoint = matrix.TransformPoint(D2D1::Point2F(_x, _y));
		return { transformedPoint.x, transformedPoint.y };
	}

	Transform Transform::invertTransform()
	{
		D2D1::Matrix3x2F invertedMatrix = matrix;
		if (invertedMatrix.IsInvertible())
		{
			invertedMatrix.Invert();
			return Transform(invertedMatrix);
		}
		else
			return Transform();
	}

	Transform Transform::operator+(Transform _x)
	{
		//Transform resultTransform;
		//resultTransform.matrix.SetProduct(matrix, _x.matrix);
		//return resultTransform;
		return Transform(_x.matrix * matrix);
	}

	Transform Transform::operator*(Transform _x)
	{
		//Transform resultTransform;
		//resultTransform.matrix.SetProduct(matrix, _x.matrix);
		//return resultTransform;
		return Transform(matrix * _x.matrix);
	}

	void Transform::operator=(Transform _x)
	{
		matrix = _x.matrix;
	}

	float Color::h()const
	{
		float R = r_ / 255.0f;
		float G = g_ / 255.0f;
		float B = b_ / 255.0f;
		float maxValue = max(R, max(G, B));
		float minValue = min(R, min(G, B));
		float delta = maxValue - minValue;
		if (delta == 0)
			return 0.0f;
		else if (maxValue == R)
			return 60.0f * (G - B) / delta;
		else if (maxValue == G)
			return 60.0f * ((B - R) / delta + 2);
		else   // maxValue == B
			return 60.0f * ((R - G) / delta + 4);
	}

	float Color::s()const
	{
		float R = r_ / 255.0f;
		float G = g_ / 255.0f;
		float B = b_ / 255.0f;
		float maxValue = max(R, max(G, B));
		float minValue = min(R, min(G, B));
		float delta = maxValue - minValue;
		if (maxValue == 0.0f)
			return 0.0f;
		else	// maxValue != 0.0f
			return delta / maxValue;
	}

	float Color::v()const
	{
		float R = r_ / 255.0f;
		float G = g_ / 255.0f;
		float B = b_ / 255.0f;
		float maxValue = max(R, max(G, B));
		return maxValue;
	}

	void Color::setH(float _h)
	{
		setHSV(_h, s(), v());
	}

	void Color::setS(float _s)
	{
		setHSV(h(), _s, v());
	}

	void Color::setV(float _v)
	{
		setHSV(h(), s(), _v);
	}

	void Color::setHSV(float _h, float _s, float _v)
	{
		float C = _v * _s;
		float X = C * (1.0f - (float)fabs((int)(_h / 60) % 2 - 1));
		float m = _v - C;

		float R = 0, G = 0, B = 0;
		if (_h < 60)
			R = C, G = X;
		else if (_h < 120)
			R = X, G = C;
		else if (_h < 180)
			G = C, B = X;
		else if (_h < 240)
			G = X, B = C;
		else if (_h < 300)
			R = X, B = C;
		else // _h < 360
			R = C, B = X;

		r_ = (R + m) * 255.0f;
		g_ = (G + m) * 255.0f;
		b_ = (B + m) * 255.0f;
	}

	Bitmap::Bitmap()
	{
		bytesPerPixel_ = 0;
		width_ = height_ = 0;
		wicBitmap_ = nullptr;
		d2dBitmap_ = nullptr;
	}

	Bitmap::Bitmap(UINT _width, UINT _height)
	{
		HRESULT hr = createWICBitmap(&wicBitmap_, _width, _height);
		if (SUCCEEDED(hr))
		{
			isValid_ = true;
			auto [w, h] = getBitmapSize(wicBitmap_);
			width_ = w;
			height_ = h;
			bytesPerPixel_ = 0;
			getPixelByte();
			getD2DBitmap(wicBitmap_, &d2dBitmap_);
		}
		else
		{
			isValid_ = false;
			width_ = height_ = 0;
			d2dBitmap_ = nullptr;
			bytesPerPixel_ = 0;
		}
	}

	Bitmap::Bitmap(String _url)
	{
		auto hr = loadWICBitmap(&wicBitmap_, AbsolutePath(_url.content));
		if (SUCCEEDED(hr))
		{
			auto [w, h] = getBitmapSize(wicBitmap_, &hr);
			if (SUCCEEDED(hr))
			{
				isValid_ = true;
				width_ = w;
				height_ = h;
				bytesPerPixel_ = 0;
				getPixelByte();
				getD2DBitmap(wicBitmap_, &d2dBitmap_);
				return;
			}
		}
		isValid_ = false;
		width_ = height_ = 0;
		wicBitmap_ = nullptr;
		d2dBitmap_ = nullptr;
		bytesPerPixel_ = 0;
	}

	Bitmap::Bitmap(String _url, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		auto hr = loadWICBitmap(&wicBitmap_, AbsolutePath(_url.content), _x, _y, _width, _height);
		if (SUCCEEDED(hr))
		{
			auto [w, h] = getBitmapSize(wicBitmap_, &hr);
			if (SUCCEEDED(hr))
			{
				isValid_ = true;
				width_ = w;
				height_ = h;
				bytesPerPixel_ = 0;
				getPixelByte();
				getD2DBitmap(wicBitmap_, &d2dBitmap_);
				return;
			}
		}
		isValid_ = false;
		width_ = height_ = 0;
		wicBitmap_ = nullptr;
		d2dBitmap_ = nullptr;
		bytesPerPixel_ = 0;
	}

	Bitmap::Bitmap(Color** _pixels, UINT _width, UINT _height)
	{
		width_ = _width;
		height_ = _height;
		createWICBitmap(&wicBitmap_, _width, _height);
		bytesPerPixel_ = 0;
		getPixelByte();
		updatePixelDetail(_pixels);
	}

	Bitmap::Bitmap(Color** _pixels, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		width_ = _width;
		height_ = _height;
		createWICBitmap(&wicBitmap_, _width, _height);
		bytesPerPixel_ = 0;
		getPixelByte();
		updatePixelDetail(_pixels, _x, _y);
	}

	Bitmap::Bitmap(ID2D1Bitmap* _d2dBitmap)
	{
		if (!_d2dBitmap)
		{
			isValid_ = false;
			width_ = height_ = 0;
			wicBitmap_ = nullptr;
			d2dBitmap_ = nullptr;
			bytesPerPixel_ = 0;
			return;
		}
		SAFE_ADDREF(_d2dBitmap);
		d2dBitmap_ = _d2dBitmap;
		HRESULT hr = getWICBitmap(d2dBitmap_, &wicBitmap_);
		if (SUCCEEDED(hr))
		{
			isValid_ = true;
			auto [w, h] = getBitmapSize(wicBitmap_, &hr);
			if (SUCCEEDED(hr))
			{
				width_ = w;
				height_ = h;
				bytesPerPixel_ = 0;
				getPixelByte();
			}
			else
			{
				isValid_ = false;
				SAFE_RELEASE(wicBitmap_);
				SAFE_RELEASE(d2dBitmap_);
				bytesPerPixel_ = 0;
				width_ = height_ = 0;
			}
		}
		else
		{
			isValid_ = false;
			SAFE_RELEASE(wicBitmap_);
			SAFE_RELEASE(d2dBitmap_);
			bytesPerPixel_ = 0;
			width_ = height_ = 0;
		}
	}

	Bitmap::Bitmap(IWICBitmap* _wicBitmap)
	{
		if (!_wicBitmap)
		{
			isValid_ = false;
			width_ = height_ = 0;
			wicBitmap_ = nullptr;
			d2dBitmap_ = nullptr;
			bytesPerPixel_ = 0;
			return;
		}
		_wicBitmap->AddRef();
		wicBitmap_ = _wicBitmap;
		HRESULT hr;
		auto [w, h] = getBitmapSize(_wicBitmap, &hr);
		if (SUCCEEDED(hr))
		{
			isValid_ = true;
			width_ = w;
			height_ = h;
			bytesPerPixel_ = 0;
			getPixelByte();
			getD2DBitmap(wicBitmap_, &d2dBitmap_);
		}
		else
		{
			isValid_ = false;
			_wicBitmap->Release();
			wicBitmap_ = nullptr;
			d2dBitmap_ = nullptr;
			bytesPerPixel_ = 0;
			width_ = height_ = 0;
		}
	}

	Bitmap::Bitmap(const Bitmap& _otherBitmap)
	{
		if (!_otherBitmap.isValid())
		{
			isValid_ = false;
			width_ = height_ = 0;
			wicBitmap_ = nullptr;
			d2dBitmap_ = nullptr;
			bytesPerPixel_ = 0;
			return;
		}
		auto hr = createWICBitmap(&wicBitmap_, _otherBitmap.width_, _otherBitmap.height_);
		if (SUCCEEDED(hr))
		{
			isValid_ = true;
			bytesPerPixel_ = 0;
			width_ = _otherBitmap.width_;
			height_ = _otherBitmap.height_;
			getPixelByte();
			Color** pixelDetail = new_memory_2d<Color>(width_, height_);
			_otherBitmap.getPixelDetail(&pixelDetail);
			updatePixelDetail(pixelDetail);
			delete_memory_2d(pixelDetail, width_, height_);
		}
		else
		{
			isValid_ = false;
			width_ = height_ = 0;
			wicBitmap_ = nullptr;
			d2dBitmap_ = nullptr;
			bytesPerPixel_ = 0;
		}
	}

	Bitmap::Bitmap(Bitmap&& _otherBitmap)noexcept
	{
		if (!_otherBitmap.isValid())
		{
			isValid_ = false;
			width_ = height_ = 0;
			wicBitmap_ = nullptr;
			d2dBitmap_ = nullptr;
			bytesPerPixel_ = 0;
			return;
		}
		isValid_ = true;
		width_ = _otherBitmap.width_;
		height_ = _otherBitmap.height_;
		wicBitmap_ = _otherBitmap.wicBitmap_;
		d2dBitmap_ = _otherBitmap.d2dBitmap_;
		bytesPerPixel_ = _otherBitmap.bytesPerPixel_;

		_otherBitmap.wicBitmap_ = nullptr;
		_otherBitmap.d2dBitmap_ = nullptr;
		_otherBitmap.bytesPerPixel_ = 0;
	}

	Bitmap::~Bitmap()
	{
		SAFE_RELEASE(wicBitmap_);
		SAFE_RELEASE(d2dBitmap_);
	}

	bool Bitmap::isValid() const
	{
		return isValid_;
	}

	UINT Bitmap::getPixelByte()
	{
		if (bytesPerPixel_ != 0)
			return bytesPerPixel_;

		bytesPerPixel_ = 0;

		if (wicBitmap_ == nullptr)
			return 0;

		HRESULT hr;
		WICPixelFormatGUID pixelFormat;
		hr = wicBitmap_->GetPixelFormat(&pixelFormat);

		IWICComponentInfo* componentInfo = nullptr;
		if (SUCCEEDED(hr))
			hr = pIWICFactory->CreateComponentInfo(pixelFormat, &componentInfo);
		IWICPixelFormatInfo2* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
			hr = componentInfo->QueryInterface(IID_PPV_ARGS(&pixelFormatInfo));
		SAFE_RELEASE(componentInfo);
		if (SUCCEEDED(hr))
			pixelFormatInfo->GetBitsPerPixel(&bytesPerPixel_);
		bytesPerPixel_ /= 8;
		return bytesPerPixel_;
	}

	Bitmap& Bitmap::operator=(const Bitmap& _bitmap)
	{
		if (&_bitmap == this)
			return *this;
		if (!_bitmap.isValid())
		{
			WARNINGWINDOW("Trying to assign an invalid bitmap. The target bitmap won't change.");
			return *this;
		}

		SAFE_RELEASE(wicBitmap_);
		SAFE_RELEASE(d2dBitmap_);

		width_ = _bitmap.width_;
		height_ = _bitmap.height_;
		HRESULT hr = createWICBitmap(&wicBitmap_, width_, height_);
		bytesPerPixel_ = 0;
		if (SUCCEEDED(hr))
		{
			getPixelByte();
			Color** pixelDetail = new_memory_2d<Color>(width_, height_);
			_bitmap.getPixelDetail(&pixelDetail);
			updatePixelDetail(pixelDetail);
			delete_memory_2d(pixelDetail, width_, height_);
		}

		return *this;
	}

	Bitmap& Bitmap::operator=(Bitmap&& _bitmap) noexcept
	{
		if (&_bitmap == this)
			return *this;

		SAFE_RELEASE(wicBitmap_);
		SAFE_RELEASE(d2dBitmap_);

		width_ = _bitmap.width_;
		height_ = _bitmap.height_;
		wicBitmap_ = _bitmap.wicBitmap_;
		d2dBitmap_ = _bitmap.d2dBitmap_;
		bytesPerPixel_ = _bitmap.bytesPerPixel_;

		_bitmap.wicBitmap_ = nullptr;
		_bitmap.d2dBitmap_ = nullptr;
		_bitmap.bytesPerPixel_ = 0;

		return *this;
	}

	void Bitmap::paint(float _x, float _y, float _alpha)const
	{
		drawBitmap(pMainRenderTarget, d2dBitmap_, (float)width_, (float)height_, _alpha, translation(_x, _y));
	}

	void Bitmap::paint(float _x, float _y, Transform _transform, float _alpha)const
	{
		drawBitmap(pMainRenderTarget, d2dBitmap_, (float)width_, (float)height_, _alpha, translation(_x, _y) + _transform);
	}

	void Bitmap::paint(Transform _transform, float _alpha) const
	{
		drawBitmap(pMainRenderTarget, d2dBitmap_, (float)width_, (float)height_, _alpha, _transform);
	}

	UINT Bitmap::getWidth()const
	{
		return width_;
	}

	UINT Bitmap::getHeight()const
	{
		return height_;
	}

	std::pair<UINT, UINT> Bitmap::getSize()const
	{
		return std::make_pair(width_, height_);
	}

	void Bitmap::getPixelDetail(Color*** _pColorArray)const
	{
		if (_pColorArray == nullptr) {
			WARNINGWINDOW("Pointer to store pixel detail is null");
			return;
		}
		if (!isValid()) {
			WARNINGWINDOW("Bitmap is not valid");
			return;
		}

		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockRead, &pILock);

		UINT stride;
		UINT cbBufferSize;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock->GetStride(&stride);

		Color** pixelArrayPointer = *_pColorArray;
		if (pv != nullptr)
		{
			if (bytesPerPixel_ == 3)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel_;

						pixelArrayPointer[i][j].setRGB(*(pixelData + 2), *(pixelData + 1), *pixelData);
						pixelArrayPointer[i][j].alpha = 1.0f;
					}
			}
			else if (bytesPerPixel_ == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel_;

						pixelArrayPointer[i][j].setRGB(*(pixelData + 2), *(pixelData + 1), *pixelData);
						pixelArrayPointer[i][j].alpha = (float)(*(pixelData + 3)) / 255.0f;
					}
			}
		}
		pILock->Release();
	}

	BYTE* Bitmap::getDataPointer()const
	{
		if (wicBitmap_ == nullptr)
			return nullptr;
		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock->Release();
		return pv;
	}

	void Bitmap::updatePixelDetail(Color** _detail)
	{
		if (_detail == nullptr || wicBitmap_ == nullptr)
			return;

		SAFE_RELEASE(d2dBitmap_);

		UINT bytesPerPixel = getPixelByte();

		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT stride;
		UINT cbBufferSize;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock->GetStride(&stride);

		if (pv != nullptr)
		{
			if (bytesPerPixel == 3)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						*(pixelData + 2) = (BYTE)_detail[i][j].r();
						*(pixelData + 1) = (BYTE)_detail[i][j].g();
						*pixelData = (BYTE)_detail[i][j].b();
					}
			}
			else if (bytesPerPixel == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						*(pixelData + 3) = (BYTE)(_detail[i][j].alpha * 255.0f);
						*(pixelData + 2) = (BYTE)_detail[i][j].r();
						*(pixelData + 1) = (BYTE)_detail[i][j].g();
						*pixelData = (BYTE)_detail[i][j].b();
					}
			}
		}
		pILock->Release();
		getD2DBitmap(wicBitmap_, &d2dBitmap_);
	}

	void Bitmap::updatePixelDetail(Color** _detail, UINT _startX, UINT _startY)
	{
		if (_detail == nullptr || wicBitmap_ == nullptr)
			return;

		UINT bytesPerPixel = getPixelByte();

		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT stride;
		UINT cbBufferSize;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock->GetStride(&stride);

		pILock->GetDataPointer(&cbBufferSize, &pv);
		if (pv != nullptr)
		{
			if (bytesPerPixel == 3)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						*(pixelData + 2) = (BYTE)_detail[_startY + i][_startX + j].r();
						*(pixelData + 1) = (BYTE)_detail[_startY + i][_startX + j].g();
						*pixelData = (BYTE)_detail[_startY + i][_startX + j].b();
					}
			}
			else if (bytesPerPixel == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						*(pixelData + 3) = (BYTE)(_detail[_startY + i][_startX + j].alpha * 255.0f);
						*(pixelData + 2) = (BYTE)_detail[_startY + i][_startX + j].r();
						*(pixelData + 1) = (BYTE)_detail[_startY + i][_startX + j].g();
						*pixelData = (BYTE)_detail[_startY + i][_startX + j].b();
					}
			}
		}
		pILock->Release();
		getD2DBitmap(wicBitmap_, &d2dBitmap_);
	}

	void Bitmap::changePixelDetailRough(std::function<void(Color&)> _function)
	{
		if (wicBitmap_ == nullptr)
			return;

		UINT bytesPerPixel = getPixelByte();

		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT stride;
		UINT cbBufferSize;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock->GetStride(&stride);

		if (pv != nullptr)
		{
			if (bytesPerPixel == 3)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						Color pixelColor;
						pixelColor.setRGB(*(pixelData + 2), *(pixelData + 1), *(pixelData));
						pixelColor.alpha = 1.0f;
						_function(pixelColor);
						*(pixelData + 2) = (BYTE)pixelColor.r();
						*(pixelData + 1) = (BYTE)pixelColor.g();
						*pixelData = (BYTE)pixelColor.b();
					}
			}
			else if (bytesPerPixel == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						Color pixelColor;
						pixelColor.setRGB(*(pixelData + 2), *(pixelData + 1), *(pixelData));
						pixelColor.alpha = (float)*(pixelData + 3) / 255.0f;
						_function(pixelColor);
						*(pixelData + 3) = (BYTE)(pixelColor.alpha * 255.0f);
						*(pixelData + 2) = (BYTE)pixelColor.r();
						*(pixelData + 1) = (BYTE)pixelColor.g();
						*pixelData = (BYTE)pixelColor.b();
					}
			}
		}
		pILock->Release();
		SAFE_RELEASE(d2dBitmap_);
		getD2DBitmap(wicBitmap_, &d2dBitmap_);
	}

	void Bitmap::changePixelDetail(std::function<void(UINT, UINT, Color&)> _function)
	{
		if (wicBitmap_ == nullptr)
			return;

		UINT bytesPerPixel = getPixelByte();

		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT stride;
		UINT cbBufferSize;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock->GetStride(&stride);

		if (pv != nullptr)
		{
			if (bytesPerPixel == 3)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						Color pixelColor;
						pixelColor.setRGB(*(pixelData + 2), *(pixelData + 1), *(pixelData));
						pixelColor.alpha = 1.0f;
						_function(j, i, pixelColor);
						*(pixelData + 2) = (BYTE)pixelColor.r();
						*(pixelData + 1) = (BYTE)pixelColor.g();
						*pixelData = (BYTE)pixelColor.b();
					}
			}
			else if (bytesPerPixel == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						Color pixelColor;
						pixelColor.setRGB(*(pixelData + 2), *(pixelData + 1), *(pixelData));
						pixelColor.alpha = (float)*(pixelData + 3) / 255.0f;
						_function(j, i, pixelColor);
						*(pixelData + 3) = (BYTE)(pixelColor.alpha * 255.0f);
						*(pixelData + 2) = (BYTE)pixelColor.r();
						*(pixelData + 1) = (BYTE)pixelColor.g();
						*pixelData = (BYTE)pixelColor.b();
					}
			}
		}
		pILock->Release();
		SAFE_RELEASE(d2dBitmap_);
		getD2DBitmap(wicBitmap_, &d2dBitmap_);
	}

	void Bitmap::viewPixelDetail(std::function<void(UINT, UINT, const Color&)> _viewFunction)const
	{
		if (wicBitmap_ == nullptr)
			return;

		if (bytesPerPixel_ == 0)
			return;

		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT stride;
		UINT cbBufferSize;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock->GetStride(&stride);

		if (pv != nullptr)
		{
			if (bytesPerPixel_ == 3)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel_;

						Color pixelColor;
						pixelColor.setRGB(*(pixelData + 2), *(pixelData + 1), *(pixelData));
						pixelColor.alpha = 1.0f;
						_viewFunction(j, i, pixelColor);
					}
			}
			else if (bytesPerPixel_ == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel_;

						Color pixelColor;
						pixelColor.setRGB(*(pixelData + 2), *(pixelData + 1), *(pixelData));
						pixelColor.alpha = (float)*(pixelData + 3) / 255.0f;
						_viewFunction(j, i, pixelColor);
					}
			}
		}
		pILock->Release();
	}
}