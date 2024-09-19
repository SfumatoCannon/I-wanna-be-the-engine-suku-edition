#include "suku_drawing.h"
#include "suku_maths.h"

namespace suku
{
	ID2D1Factory* g_pD2DFactory = NULL;	// Direct2D factory
	ID2D1HwndRenderTarget* g_pRenderTarget = NULL;	// Render target
	IWICImagingFactory* g_pIWICFactory;

	TCHAR strPath[MAX_PATH + 1];
	size_t Path_len;

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
		g_pRenderTarget->SetTransform(_transform.matrix);
	}

	void beginDraw(HWND hWnd)
	{
		createD2DResource(hWnd);
		g_pRenderTarget->BeginDraw();
		g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}

	void endDraw()
	{
		g_pRenderTarget->EndDraw();
	}

	void clearScreen()
	{
		g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}


	LPCTSTR AbsolutePath(LPCTSTR relative_path)
	{
		if (!relative_path) return nullptr;
		static TCHAR result[MAX_PATH + 1] = { 0 };
		result[0] = L'\0';
		lstrcat(result, strPath);
		lstrcat(result, relative_path);
		return result;
	}

	void SwapByte(BYTE* _a, BYTE* _b)
	{
		BYTE t = *_a;
		*_a = *_b;
		*_b = t;
	}

	void createD2DResource(HWND hWnd)
	{
		if (!g_pRenderTarget)
		{
			HRESULT hr;

			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
			if (FAILED(hr))
			{
				MessageBox(hWnd, _T("Create D2D factory failed!"), _T("Error"), 0);
				return;
			}

			RECT rc;
			GetClientRect(hWnd, &rc);

			hr = g_pD2DFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(
					hWnd,
					D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
				),
				&g_pRenderTarget
			);
			if (FAILED(hr))
			{
				MessageBox(hWnd, _T("Create render target failed!"), _T("Error"), 0);
				return;
			}

		}
	}

	ID2D1Bitmap* GetBlendedBitmapFromFile(
		IWICImagingFactory* pIWICFactory,
		ID2D1RenderTarget* pRenderTarget,
		LPCTSTR uri,
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

	HRESULT loadWICBitmap(IWICBitmap** _pWicBitmap, LPCTSTR uri)
	{
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pSource = nullptr;
		UINT					originalWidth = 0;
		UINT					originalHeight = 0;

		HRESULT hr = g_pIWICFactory->CreateDecoderFromFilename(
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
			hr = g_pIWICFactory->CreateBitmapFromSourceRect(
				pSource, 0, 0, (UINT)originalWidth, (UINT)originalHeight, _pWicBitmap);
		}

		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);
		return hr;
	}

	HRESULT loadWICBitmap(IWICBitmap** _pWicBitmap, LPCTSTR uri, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pSource = nullptr;

		HRESULT hr = g_pIWICFactory->CreateDecoderFromFilename(
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
			hr = g_pIWICFactory->CreateBitmapFromSourceRect(
				pSource, _x, _y, _width, _height, _pWicBitmap);
		}

		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);
		return hr;
	}

	HRESULT createWICBitmap(IWICBitmap** _pWicBitmap, UINT _width, UINT _height)
	{
		if (_width > 1600 || _height > 1216)
			return E_FAIL;
		HRESULT hr = loadWICBitmap(_pWicBitmap, AbsolutePath(_T("Image\\tempBlank.png")),
			0, 0, _width, _height);
		return hr;
	}

	HRESULT getD2DBitmap(IWICBitmap** _pWicBitmap, ID2D1Bitmap** _pD2dBitmap, int _width, int _height)
	{
		if (!(*_pWicBitmap)) return S_FALSE;

		IWICBitmapDecoder* pDecoder = NULL;
		IWICBitmapFrameDecode* pSource = NULL;
		IWICFormatConverter* pConverter = NULL;
		IWICBitmapScaler* pScaler = NULL;
		UINT width = 0, height = 0;
		HRESULT hr = S_OK;

		if (SUCCEEDED(hr))
		{
			hr = g_pIWICFactory->CreateFormatConverter(&pConverter);
		}
		if (SUCCEEDED(hr))
		{
			hr = g_pIWICFactory->CreateBitmapScaler(&pScaler);
		}
		if (SUCCEEDED(hr))
		{
			hr = (*_pWicBitmap)->GetSize(&width, &height);
		}
		if (SUCCEEDED(hr))
		{
			hr = pScaler->Initialize(*_pWicBitmap, _width, _height, WICBitmapInterpolationModeCubic);
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
			hr = g_pRenderTarget->CreateBitmapFromWicBitmap(
				pConverter,
				NULL,
				_pD2dBitmap
			);
		}

		SAFE_RELEASE(pConverter);
		SAFE_RELEASE(pScaler);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);

		return hr;
	}

	Color** getPixelDetailFromWICBitmap(IWICBitmap* _sourceBitmap)
	{
		if (_sourceBitmap == nullptr)
			return nullptr;
		auto [width, height] = getSizeFromWICBitmap(_sourceBitmap);
		Color** pixelArrayPointer = malloc2D<Color>(width, height);
		if (pixelArrayPointer != nullptr)
		{
			IWICBitmapLock* pILock = nullptr;
			WICRect rcLock = { 0, 0, (int)width, (int)height };
			_sourceBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);

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

	Color** getPixelDetailFromWICBitmap(IWICBitmap* _sourceBitmap, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		if (_sourceBitmap == nullptr)
			return nullptr;
		auto [width, height] = getSizeFromWICBitmap(_sourceBitmap);
		if (_x + _width > width || _y + _height > height)
			return nullptr;
		Color** pixelArrayPointer = malloc2D<Color>(_width, _height);
		if (pixelArrayPointer != nullptr)
		{
			IWICBitmapLock* pILock = nullptr;
			WICRect rcLock = { 0, 0, (int)width, (int)height };
			_sourceBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);

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
		g_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r(), _color.g(), _color.b(), _color.alpha),
			&newBrush
		);
		return newBrush;
	}

	std::pair<UINT, UINT> getSizeFromWICBitmap(IWICBitmap* _pBitmap, HRESULT* _pHResult)
	{
		if (!_pBitmap) return { 0,0 };
		UINT width, height;
		*_pHResult = _pBitmap->GetSize(&width, &height);
		return { width, height };
	}

	void getHitAreaFromBitmap(bool** _pHitArea, IWICBitmap* pBitmap, BYTE _alphaThreshold)
	{
		if (!pBitmap) return;
		if (!_pHitArea) return;

		IWICBitmapLock* pILock = nullptr;
		UINT width, height;
		pBitmap->GetSize(&width, &height);
		WICRect rcLock = { 0, 0, (int)width, (int)height };
		pBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		if (pv)
		{
			for (unsigned int i = 0; i < width * 4; i += 4)
				for (unsigned int j = 0; j < height; j++)
				{
					UINT k = i + j * width * 4;

					if (k + 3 < cbBufferSize && pv[k + 3] <= _alphaThreshold)
						_pHitArea[i / 4][j] = 0;
					else _pHitArea[i / 4][j] = 1;
				}
		}
		pILock->Release();
	}

	void getHitAreaFromBitmap(bool** _pHitArea, const Bitmap& _bitmap, BYTE _alphaThreshold)
	{
		if (!_pHitArea)
			return;
		UINT width = _bitmap.getWidth();
		_bitmap.viewPixelDetail([&](const BYTE* _pv, const UINT _x, const UINT _y, const UINT _k)
			{
				if (_pv[_k + 3] <= _alphaThreshold)
					_pHitArea[_x][_y] = 0;
				else _pHitArea[_x][_y] = 1;
			});
	}
	/*
	void drawBitmap(ID2D1Bitmap* _pBitmap, float _x, float _y, float _width, float _height, float _alpha,
		float _angle, float _rotatingCenterX, float _rotatingCenterY)
	{

		if (!_pBitmap) return;

		D2D1::Matrix3x2F transformMatrix;

		transformMatrix = D2D1::Matrix3x2F::Rotation(_angle, D2D1::Point2F(_rotatingCenterX, _rotatingCenterY));
		if (_width < 0)
			transformMatrix.SetProduct(transformMatrix,
				D2D1::Matrix3x2F::Scale(-1.0, 1.0, D2D1::Point2F(_x - _width / 2, _y + _height / 2)));
		if (_height < 0)
			transformMatrix.SetProduct(transformMatrix,
				D2D1::Matrix3x2F::Scale(1.0, -1.0, D2D1::Point2F(_x + _width / 2, _y - _height / 2)));

		g_pRenderTarget->SetTransform(transformMatrix);
		g_pRenderTarget->drawBitmap(
			_pBitmap,
			D2D1::RectF(
				_x,
				_y,
				_x + fabs(_width),
				_y + fabs(_height)),
			_alpha
		);
	}
	*/
	void drawBitmap(ID2D1Bitmap* _pBitmap, float _x, float _y,
		float _width, float _height, float _alpha, Transform _transform)
	{
		if (!_pBitmap) return;
		setPaintingTransform(_transform);
		g_pRenderTarget->DrawBitmap(
			_pBitmap,
			D2D1::RectF(
				_x, _y,
				_x + fabs(_width),
				_y + fabs(_height)
			),
			_alpha
		);
	}

	void drawBitmap(ID2D1Bitmap* _pBitmap, float _width, float _height, float _alpha, Transform _transform)
	{
		if (!_pBitmap) return;
		setPaintingTransform(_transform);
		g_pRenderTarget->DrawBitmap(
			_pBitmap,
			D2D1::RectF(
				0.0f, 0.0f,
				fabs(_width),
				fabs(_height)
			),
			_alpha
		);
	}

	void cleanup()
	{
		CoUninitialize();
		SAFE_RELEASE(g_pRenderTarget);
		SAFE_RELEASE(g_pD2DFactory);
	}

	Shape::Shape()
	{
		originalGeometry = nullptr;
		transform = Transform();
		currentGeometry = nullptr;
	}


	Shape::Shape(ID2D1Geometry* _geometry)
	{
		transform = Transform();
		setOriginalGeometry(_geometry);
	}

	void Shape::setOriginalGeometry(ID2D1Geometry* _geometry)
	{
		originalGeometry = _geometry;
		g_pD2DFactory->CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
	}

	void Shape::setTransform(Transform _transform)
	{
		transform = _transform;
		g_pD2DFactory->CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
	}

	void Shape::paint(float _x, float _y, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth,
		ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y));
			if (_outlineBrush != nullptr)
				g_pRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				g_pRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y) + _paintingTransform);
			if (_outlineBrush != nullptr)
				g_pRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				g_pRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	void Shape::paint(Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(_paintingTransform);
			if (_outlineBrush != nullptr)
				g_pRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				g_pRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	bool Shape::isCrashed(Shape& _x)
	{
		D2D1_GEOMETRY_RELATION result;
		currentGeometry->CompareWithGeometry(_x.currentGeometry, nullptr, &result);
		if (result == D2D1_GEOMETRY_RELATION_DISJOINT)
			return false;
		else return true;
	}

	void Shape::operator=(Shape& _x)
	{
		originalGeometry = _x.originalGeometry;
		transform = _x.transform;
		currentGeometry = _x.currentGeometry;
	}

	Shape Shape::operator-(Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		g_pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_EXCLUDE,
				NULL,
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

	Shape Shape::operator&(Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		g_pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_INTERSECT,
				NULL,
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

	Shape Shape::operator|(Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		g_pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_UNION,
				NULL,
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

	Shape Shape::operator^(Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		g_pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_XOR,
				NULL,
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

	SquareShape::SquareShape(float _length, float _beginningX, float _beginningY)
	{
		ID2D1RectangleGeometry* newGeometry;
		HRESULT hr;
		hr = g_pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_beginningX, _beginningY,
				_beginningX + _length - 1.0f, _beginningY + _length - 1.0f
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
	}

	RectangleShape::RectangleShape(float _width, float _height, float _beginningX, float _beginningY)
	{
		ID2D1RectangleGeometry* newGeometry;
		HRESULT hr;
		hr = g_pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_beginningX, _beginningY,
				_beginningX + _width - 1.0f, _beginningY + _height - 1.0f
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
	}

	CircleShape::CircleShape(float _radius, float _beginningX, float _beginningY)
	{
		ID2D1EllipseGeometry* newGeometry;
		HRESULT hr;
		hr = g_pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radius + _beginningX, _radius + _beginningY
				),
				_radius, _radius
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
	}

	EllipseShape::EllipseShape(float _radiusX, float _radiusY, float _beginningX, float _beginningY)
	{
		ID2D1EllipseGeometry* newGeometry;
		HRESULT hr;
		hr = g_pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radiusX + _beginningX, _radiusY + _beginningY
				),
				_radiusX, _radiusY
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
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

	Color::Color()
	{
		alpha = 1.0f;
		r_ = g_ = b_ = 0.0f;
	}

	Color::Color(float _r, float _g, float _b, float _alpha)
	{
		alpha = _alpha;
		r_ = _r;
		g_ = _g;
		b_ = _b;
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

	Bitmap::Bitmap(UINT _width, UINT _height)
	{
		//width_ = _width;
		//height_ = _height;
		//d2d1Bitmap_ = nullptr;
		//wicBitmap_ = nullptr;
		//D2D1_BITMAP_PROPERTIES bitmapProperties = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
		//HRESULT hr;
		//hr = g_pRenderTarget->CreateBitmap(
		//	D2D1::SizeU(_width, _height),
		//	bitmapProperties,
		//	&d2d1Bitmap_
		//);
		//if (SUCCEEDED(hr))
		//	createWICBitmap(&wicBitmap_, _width, _height);
		//if (SUCCEEDED(hr))
		//{
		//	ID2D1RenderTarget* pTempRenderTarget = nullptr;
		//	hr = g_pD2DFactory->CreateWicBitmapRenderTarget(wicBitmap_, D2D1::RenderTargetProperties(), &pTempRenderTarget);
		//	if (SUCCEEDED(hr))
		//	{
		//		pTempRenderTarget->BeginDraw();
		//		pTempRenderTarget->DrawBitmap(d2d1Bitmap_);
		//		pTempRenderTarget->EndDraw();

		//		// 释放临时的 ID2D1RenderTarget 对象
		//		pTempRenderTarget->Release();
		//	}
		//}
		HRESULT hr = createWICBitmap(&wicBitmap_, _width, _height);
		if (SUCCEEDED(hr))
		{
			auto [w, h] = getSizeFromWICBitmap(wicBitmap_);
			width_ = w;
			height_ = h;
			getD2DBitmap(&wicBitmap_, &d2d1Bitmap_, _width, _height);
		}
		else
		{
			width_ = height_ = 0;
			d2d1Bitmap_ = nullptr;
		}
	}

	Bitmap::Bitmap(LPCTSTR _url)
	{
		auto hr = loadWICBitmap(&wicBitmap_, AbsolutePath(_url));
		if (SUCCEEDED(hr))
		{
			auto [w, h] = getSizeFromWICBitmap(wicBitmap_, &hr);
			if (SUCCEEDED(hr))
			{
				width_ = w;
				height_ = h;
				getD2DBitmap(&wicBitmap_, &d2d1Bitmap_, w, h);
				return;
			}
		}
		width_ = height_ = 0;
		wicBitmap_ = nullptr;
		d2d1Bitmap_ = nullptr;
	}

	Bitmap::Bitmap(LPCTSTR _url, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		loadWICBitmap(&wicBitmap_, AbsolutePath(_url), _x, _y, _width, _height);
		auto [w, h] = getSizeFromWICBitmap(wicBitmap_);
		width_ = w;
		height_ = h;
		getD2DBitmap(&wicBitmap_, &d2d1Bitmap_, w, h);
		//createWICBitmap(&wicBitmap_, _width, _height);

		//IWICBitmap* originalBitmap;
		//loadWICBitmap(&originalBitmap, AbsolutePath(_url));
		//Color** pixels = getPixelDetailFromWICBitmap(originalBitmap, _x, _y, _width, _height);
		//originalBitmap->Release();

		//updatePixelDetail(pixels);
	}

	Bitmap::Bitmap(Color** _pixels, UINT _width, UINT _height)
	{
		width_ = _width;
		height_ = _height;
		createWICBitmap(&wicBitmap_, _width, _height);
		updatePixelDetail(_pixels);
	}

	Bitmap::Bitmap(Color** _pixels, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		width_ = _width;
		height_ = _height;
		createWICBitmap(&wicBitmap_, _width, _height);
		updatePixelDetail(_pixels, _x, _y);
	}

	Bitmap::Bitmap(const Bitmap& _otherBitmap)
	{
		auto [w, h] = _otherBitmap.getSize();
		width_ = w;
		height_ = h;
		createWICBitmap(&wicBitmap_, w, h);

		Color** x = _otherBitmap.getPixelDetail();
		updatePixelDetail(x);
		free2D(x, w, h);
	}

	Bitmap::~Bitmap()
	{
		if (wicBitmap_)
			wicBitmap_->Release();
		if (d2d1Bitmap_)
			d2d1Bitmap_->Release();
	}

	Bitmap& Bitmap::operator=(const Bitmap& _bitmap)
	{
		if (&_bitmap == this)
			return *this;

		if (wicBitmap_)
			wicBitmap_->Release();
		if (d2d1Bitmap_)
			d2d1Bitmap_->Release();
		auto [w, h] = _bitmap.getSize();
		width_ = w;
		height_ = h;
		HRESULT hr = createWICBitmap(&wicBitmap_, w, h);

		auto x = _bitmap.getPixelDetail();
		updatePixelDetail(x);
		free2D(x, w, h);

		return *this;
	}

	void Bitmap::paint(float _x, float _y, float _alpha)const
	{
		drawBitmap(d2d1Bitmap_, (float)width_, (float)height_, _alpha, translation(_x, _y));
	}

	void Bitmap::paint(float _x, float _y, Transform _transform, float _alpha)const
	{
		drawBitmap(d2d1Bitmap_, (float)width_, (float)height_, _alpha, translation(_x, _y) + _transform);
	}

	void Bitmap::paint(Transform _transform, float _alpha) const
	{
		drawBitmap(d2d1Bitmap_, (float)width_, (float)height_, _alpha, _transform);
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

	Color** Bitmap::getPixelDetail()const
	{
		if (wicBitmap_ == nullptr)
			return nullptr;
		Color** pixelArrayPointer = malloc2D<Color>(width_, height_);
		if (pixelArrayPointer != nullptr)
		{
			WICPixelFormatGUID pixelFormat;
			wicBitmap_->GetPixelFormat(&pixelFormat);

			UINT bytesPerPixel = 0;
			// 获取每个像素所占的字节数
			IWICComponentInfo* componentInfo;
			g_pIWICFactory->CreateComponentInfo(pixelFormat, &componentInfo);
			IWICPixelFormatInfo2* pixelFormatInfo;
			componentInfo->QueryInterface(IID_PPV_ARGS(&pixelFormatInfo));
			componentInfo->Release();
			pixelFormatInfo->GetBitsPerPixel(&bytesPerPixel);
			bytesPerPixel /= 8;

			IWICBitmapLock* pILock = nullptr;
			WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
			wicBitmap_->Lock(&rcLock, WICBitmapLockRead, &pILock);

			UINT stride;
			UINT cbBufferSize;
			BYTE* pv = nullptr;

			pILock->GetDataPointer(&cbBufferSize, &pv);
			pILock->GetStride(&stride);


			if (pv != nullptr)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						pixelArrayPointer[i][j].setRGB(pixelData[2], pixelData[1], pixelData[0]);
						if (bytesPerPixel == 4)
							pixelArrayPointer[i][j].alpha = (float)pixelData[3] / 255.0f;
						else
							pixelArrayPointer[i][j].alpha = 1.0f;
					}
			}
			pILock->Release();
		}
		return pixelArrayPointer;
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

		WICPixelFormatGUID pixelFormat;
		wicBitmap_->GetPixelFormat(&pixelFormat);

		UINT bytesPerPixel = 0;
		// 获取每个像素所占的字节数
		IWICComponentInfo* componentInfo;
		g_pIWICFactory->CreateComponentInfo(pixelFormat, &componentInfo);
		IWICPixelFormatInfo2* pixelFormatInfo;
		componentInfo->QueryInterface(IID_PPV_ARGS(&pixelFormatInfo));
		componentInfo->Release();
		pixelFormatInfo->GetBitsPerPixel(&bytesPerPixel);
		bytesPerPixel /= 8;

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
			for (unsigned int i = 0; i < height_; i++)
				for (unsigned int j = 0; j < width_; j++)
				{
					BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

					pixelData[2] = (BYTE)_detail[i][j].r();
					pixelData[1] = (BYTE)_detail[i][j].g();
					pixelData[0] = (BYTE)_detail[i][j].b();

					if (bytesPerPixel == 4)
						pixelData[3] = (BYTE)(_detail[i][j].alpha * 255.0f);
					else pixelData[3] = 1.0f;
				}
		}
		pILock->Release();
		getD2DBitmap(&wicBitmap_, &d2d1Bitmap_, width_, height_);
	}

	void Bitmap::updatePixelDetail(Color** _detail, UINT _startX, UINT _startY)
	{
		if (_detail == nullptr || wicBitmap_ == nullptr)
			return;
		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		if (pv)
		{
			for (unsigned int i = 0; i < width_ * 4; i += 4)
				for (unsigned int j = 0; j < height_; j++)
				{
					UINT k = i + j * width_ * 4;
					if (k + 3 < cbBufferSize)
					{
						pv[k + 2] = (UINT)_detail[i / 4 + _startX][j + _startY].r();
						pv[k + 1] = (UINT)_detail[i / 4 + _startX][j + _startY].g();
						pv[k] = (UINT)_detail[i / 4 + _startX][j + _startY].b();
						pv[k + 3] = (UINT)(_detail[i / 4 + _startX][j + _startY].alpha * 255.0f);
					}
				}
		}
		pILock->Release();
		getD2DBitmap(&wicBitmap_, &d2d1Bitmap_, width_, height_);
	}

	void Bitmap::changePixelDetailRough(std::function<void(Color&)> _changingFunction)
	{
		if (wicBitmap_ == nullptr)
			return;
		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		if (pv)
		{
			for (unsigned int i = 0; i < width_ * 4; i += 4)
				for (unsigned int j = 0; j < height_; j++)
				{
					UINT k = i + j * width_ * 4;
					if (k + 3 < cbBufferSize)
					{
						Color pixelColor;
						pixelColor.setRGB(pv[k + 2], pv[k + 1], pv[k]);
						pixelColor.alpha = (float)pv[k + 3] / 255.0f;
						_changingFunction(pixelColor);
						pv[k + 2] = (UINT)pixelColor.r();
						pv[k + 1] = (UINT)pixelColor.g();
						pv[k] = (UINT)pixelColor.b();
						pv[k + 3] = (UINT)(pixelColor.alpha * 255.0f);
					}
				}
		}
		pILock->Release();
		getD2DBitmap(&wicBitmap_, &d2d1Bitmap_, width_, height_);
	}

	void Bitmap::changePixelDetail(std::function<void(BYTE*, UINT, UINT, UINT)> _changingFunction)
	{
		if (wicBitmap_ == nullptr)
			return;
		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, &pILock);

		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		if (pv)
		{
			for (unsigned int i = 0; i < width_ * 4; i += 4)
				for (unsigned int j = 0; j < height_; j++)
				{
					UINT k = i + j * width_ * 4;
					if (k + 3 < cbBufferSize)
						_changingFunction(pv, i / 4, j, k);
				}
		}
		pILock->Release();
		getD2DBitmap(&wicBitmap_, &d2d1Bitmap_, width_, height_);
	}

	void Bitmap::viewPixelDetail(std::function<void(const BYTE*, const UINT, const UINT, const UINT)> _viewFunction)const
	{
		if (wicBitmap_ == nullptr)
			return;
		IWICBitmapLock* pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockRead, &pILock);

		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		if (pv)
		{
			for (unsigned int i = 0; i < width_ * 4; i += 4)
				for (unsigned int j = 0; j < height_; j++)
				{
					UINT k = i + j * width_ * 4;
					if (k + 3 < cbBufferSize)
						_viewFunction(pv, i / 4, j, k);
				}
		}
		pILock->Release();
	}
}