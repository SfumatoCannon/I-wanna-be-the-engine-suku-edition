#include "bitmap.h"
#include "includes.h"
#include "../suku_foundation/includes.h"

namespace suku
{
	using Microsoft::WRL::ComPtr;
	using memory::Array2D;
	// Private functions declaration
	// ----------------------------------------------------------------------------
	ComPtr<ID2D1Bitmap> GetBlendedBitmapFromFile(
		const ComPtr<IWICImagingFactory>& pIWICFactory,
		const ComPtr<ID2D1RenderTarget>& pRenderTarget,
		const wchar_t* uri,
		const D2D1_COLOR_F& color);
	HRESULT LoadBitmapFromFile(
		const ComPtr<ID2D1RenderTarget>& pRenderTarget,
		const ComPtr<IWICImagingFactory>& pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ComPtr<ID2D1Bitmap>& ppBitmap);
	HRESULT loadWICBitmap(ComPtr<IWICBitmap>& _ppWicBitmap, const wchar_t* uri    /*absolute path*/);
	HRESULT loadWICBitmap(ComPtr<IWICBitmap>& _ppWicBitmap, const wchar_t* _url, /*absolute path*/
		UINT _x, UINT _y, UINT _width, UINT _height);
	HRESULT createWICBitmap(ComPtr<IWICBitmap>& _pWicBitmap, UINT _width, UINT _height);
	HRESULT getD2DBitmap(const ComPtr<IWICBitmap>& _pWicBitmap, ComPtr<ID2D1Bitmap>& _ppD2dBitmap);
	HRESULT getWICBitmap(const ComPtr<ID2D1Bitmap>& _pD2dBitmap, ComPtr<IWICBitmap>& _ppWicBitmap);
	std::pair<UINT, UINT> getBitmapSize(const ComPtr<IWICBitmap>& _pBitmap);
	std::pair<UINT, UINT> getBitmapSize(const ComPtr<ID2D1Bitmap>& _pBitmap);
	void drawBitmap(const ComPtr<ID2D1RenderTarget>& _renderTarget, const ComPtr<ID2D1Bitmap>& _pBitmap, float _x, float _y,
		float _width, float _height, float _alpha,
		Transform _transform = Transform());
	void drawBitmap(const ComPtr<ID2D1RenderTarget>& _renderTarget, const ComPtr<ID2D1Bitmap>& _pBitmap,
		float _width, float _height, float _alpha,
		Transform _transform = Transform());
	// ----------------------------------------------------------------------------
	// End of private functions declaration


	void Bitmap::refreshD2DBitmap()
	{
		if (d2dBitmapUpdateTag_)
		{
			release_safe(d2dBitmap_);
			ComPtr<ID2D1Bitmap> pD2d = nullptr;
			suku::getD2DBitmap(wicBitmap_.Get(), pD2d);
			d2dBitmap_.Attach(pD2d.Detach());
			d2dBitmapUpdateTag_ = false;
		}
	}

	void Bitmap::refreshPixelByte()
	{
		if (wicBitmap_ == nullptr)
		{
			// refresh wicBitmap
		}
		HRESULT hr;
		WICPixelFormatGUID pixelFormat;
		hr = wicBitmap_->GetPixelFormat(&pixelFormat);

		ComPtr<IWICComponentInfo> componentInfo = nullptr;
		auto pWICFactory = WICFactoryGlobal::getWICFactory();
		if (SUCCEEDED(hr))
			hr = pWICFactory->CreateComponentInfo(pixelFormat, componentInfo.GetAddressOf());
		ComPtr<IWICPixelFormatInfo2> pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
			hr = componentInfo->QueryInterface(IID_PPV_ARGS(&pixelFormatInfo));
		release_safe(componentInfo);
		if (SUCCEEDED(hr))
			pixelFormatInfo->GetBitsPerPixel(&bytesPerPixel_);
		bytesPerPixel_ /= 8;
	}

	Bitmap::Bitmap(UINT _width, UINT _height)
	{
		ComPtr<IWICBitmap> tmpWic;
		HRESULT hr = createWICBitmap(tmpWic, _width, _height);
		if (SUCCEEDED(hr))
		{
			isValid_ = true;
			auto [w, h] = getBitmapSize(tmpWic);
			width_ = w;
			height_ = h;
			bytesPerPixel_ = 0;
			getPixelByte();
			wicBitmap_.Attach(tmpWic.Detach());
			d2dBitmapUpdateTag_ = true;
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
		ComPtr<IWICBitmap> tmpWic;
		auto hr = loadWICBitmap(tmpWic, AbsolutePath(_url.content));
		if (SUCCEEDED(hr))
		{
			auto [w, h] = getBitmapSize(tmpWic);
			if (SUCCEEDED(hr))
			{
				isValid_ = true;
				width_ = w;
				height_ = h;
				bytesPerPixel_ = 0;
				wicBitmap_.Attach(tmpWic.Detach());
				getPixelByte();
				d2dBitmapUpdateTag_ = true;
				return;
			}
		}
		isValid_ = false;
		width_ = height_ = 0;
		wicBitmap_.Reset();
		d2dBitmap_.Reset();
		bytesPerPixel_ = 0;
	}

	Bitmap::Bitmap(String _url, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		ComPtr<IWICBitmap> tmpWic;
		auto hr = loadWICBitmap(tmpWic, AbsolutePath(_url.content), _x, _y, _width, _height);
		if (SUCCEEDED(hr))
		{
			auto [w, h] = getBitmapSize(tmpWic);
			if (SUCCEEDED(hr))
			{
				isValid_ = true;
				width_ = w;
				height_ = h;
				bytesPerPixel_ = 0;
				wicBitmap_.Attach(tmpWic.Detach());
				getPixelByte();
				d2dBitmapUpdateTag_ = true;
				return;
			}
		}
		isValid_ = false;
		width_ = height_ = 0;
		wicBitmap_.Reset();
		d2dBitmap_.Reset();
		bytesPerPixel_ = 0;
	}

	Bitmap::Bitmap(const Array2D<Color>& _pixels, UINT _width, UINT _height)
	{
		width_ = _width;
		height_ = _height;
		ComPtr<IWICBitmap> tmpWic;
		createWICBitmap(tmpWic, _width, _height);
		wicBitmap_.Attach(tmpWic.Detach());
		bytesPerPixel_ = 0;
		getPixelByte();
		updatePixelDetail(_pixels);
		d2dBitmapUpdateTag_ = true;
	}

	Bitmap::Bitmap(const Array2D<Color>& _pixels, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		width_ = _width;
		height_ = _height;
		ComPtr<IWICBitmap> tmpWic;
		createWICBitmap(tmpWic, _width, _height);
		wicBitmap_.Attach(tmpWic.Detach());
		bytesPerPixel_ = 0;
		getPixelByte();
		updatePixelDetail(_pixels, _x, _y);
		d2dBitmapUpdateTag_ = true;
	}

	Bitmap::Bitmap(ComPtr<ID2D1Bitmap> _d2dBitmap)
		:isValid_(false), width_(0), height_(0), bytesPerPixel_(0)
	{
		if (!_d2dBitmap)
			return;

		d2dBitmap_ = _d2dBitmap;
		auto [w, h] = getBitmapSize(d2dBitmap_);
		width_ = w;
		height_ = h;

		HRESULT hr = suku::getWICBitmap(d2dBitmap_, wicBitmap_);
		if (SUCCEEDED(hr))
		{
			refreshPixelByte();
			isValid_ = true;
		}
	}

	Bitmap::Bitmap(ComPtr<IWICBitmap> _wicBitmap)
		:isValid_(false), width_(0), height_(0), bytesPerPixel_(0)
	{
		if (!_wicBitmap)
			return;

		wicBitmap_ = _wicBitmap;
		auto [w, h] = getBitmapSize(wicBitmap_);
		isValid_ = true;
		width_ = w;
		height_ = h;
		d2dBitmapUpdateTag_ = true;
		refreshPixelByte();
	}

	Bitmap::Bitmap(const Bitmap& _otherBitmap)
		:isValid_(false), width_(0), height_(0), bytesPerPixel_(0)
	{
		if (!_otherBitmap.isValid())
			return;

		auto hr = createWICBitmap(wicBitmap_, _otherBitmap.width_, _otherBitmap.height_);
		if (SUCCEEDED(hr))
		{
			isValid_ = true;
			width_ = _otherBitmap.width_;
			height_ = _otherBitmap.height_;
			getPixelByte();
			Array2D<Color> pixelArray(width_, height_);
			_otherBitmap.getPixelDetail(pixelArray);
			updatePixelDetail(pixelArray);
		}
	}

	Bitmap::Bitmap(Bitmap&& _otherBitmap)noexcept
	{
		if (!_otherBitmap.isValid())
		{
			isValid_ = false;
			width_ = height_ = 0;
			wicBitmap_.Reset();
			d2dBitmap_.Reset();
			bytesPerPixel_ = 0;
			return;
		}
		isValid_ = true;
		width_ = _otherBitmap.width_;
		height_ = _otherBitmap.height_;
		wicBitmap_ = std::move(_otherBitmap.wicBitmap_);
		d2dBitmap_ = std::move(_otherBitmap.d2dBitmap_);
		bytesPerPixel_ = _otherBitmap.bytesPerPixel_;

		_otherBitmap.wicBitmap_.Reset();
		_otherBitmap.d2dBitmap_.Reset();
		_otherBitmap.bytesPerPixel_ = 0;
	}

	Bitmap::~Bitmap()
	{
		release_safe(wicBitmap_);
		release_safe(d2dBitmap_);
	}

	bool Bitmap::isValid() const
	{
		return isValid_;
	}

	UINT Bitmap::getPixelByte()
	{
		if (bytesPerPixel_ == 0)
			refreshPixelByte();
		return bytesPerPixel_;
	}

	ComPtr<ID2D1Bitmap> Bitmap::getD2DBitmap()
	{
		if (d2dBitmap_ == nullptr || d2dBitmapUpdateTag_)
			refreshD2DBitmap();
		return d2dBitmap_;
	}

	ComPtr<IWICBitmap> Bitmap::getWICBitmap()
	{
		return wicBitmap_;
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

		release_safe(wicBitmap_);
		release_safe(d2dBitmap_);

		width_ = _bitmap.width_;
		height_ = _bitmap.height_;
		HRESULT hr = createWICBitmap(wicBitmap_, width_, height_);
		bytesPerPixel_ = 0;
		if (SUCCEEDED(hr))
		{
			Array2D<Color> pixelArray(width_, height_);
			_bitmap.getPixelDetail(pixelArray);
			updatePixelDetail(pixelArray);
		}

		return *this;
	}

	Bitmap& Bitmap::operator=(Bitmap&& _bitmap) noexcept
	{
		if (&_bitmap == this)
			return *this;

		release_safe(wicBitmap_);
		release_safe(d2dBitmap_);

		width_ = _bitmap.width_;
		height_ = _bitmap.height_;
		wicBitmap_ = std::move(_bitmap.wicBitmap_);
		d2dBitmap_ = std::move(_bitmap.d2dBitmap_);
		bytesPerPixel_ = _bitmap.bytesPerPixel_;

		_bitmap.wicBitmap_.Reset();
		_bitmap.d2dBitmap_.Reset();
		_bitmap.bytesPerPixel_ = 0;

		return *this;
	}

	void Bitmap::paint(float _x, float _y, float _alpha)
	{
		refreshD2DBitmap();
		drawBitmap(pMainRenderTarget, d2dBitmap_, (float)_x, (float)_y, (float)width_, (float)height_, _alpha, translation(_x, _y));
	}

	void Bitmap::paint(float _x, float _y, Transform _transform, float _alpha)
	{
		refreshD2DBitmap();
		drawBitmap(pMainRenderTarget, d2dBitmap_, (float)_x, (float)_y, (float)width_, (float)height_, _alpha, translation(_x, _y) + _transform);
	}

	void Bitmap::paint(Transform _transform, float _alpha)
	{
		refreshD2DBitmap();
		drawBitmap(pMainRenderTarget, d2dBitmap_, 0.0f, 0.0f, (float)width_, (float)height_, _alpha, _transform);
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

	void Bitmap::getPixelDetail(Array2D<Color>& _pColorArray)const
	{
		if (!isValid()) {
			WARNINGWINDOW("Bitmap is not valid");
			return;
		}

		ComPtr<IWICBitmapLock> pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockRead, pILock.GetAddressOf());

		if (width_ != _pColorArray.getRows() || height_ != _pColorArray.getCols())
			_pColorArray.resize(width_, height_);

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

						_pColorArray(j, i).setRGB(*(pixelData + 2), *(pixelData + 1), *pixelData);
						_pColorArray(j, i).alpha = 1.0f;
					}
			}
			else if (bytesPerPixel_ == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel_;

						_pColorArray(j, i).setRGB(*(pixelData + 2), *(pixelData + 1), *pixelData);
						_pColorArray(j, i).alpha = (float)(*(pixelData + 3)) / 255.0f;
					}
			}
		}
		pILock.Reset();
	}

	BYTE* Bitmap::getDataPointer()const
	{
		if (wicBitmap_ == nullptr)
			return nullptr;
		ComPtr<IWICBitmapLock> pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, pILock.GetAddressOf());

		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		pILock->GetDataPointer(&cbBufferSize, &pv);
		pILock.Reset();
		return pv;
	}

	void Bitmap::updatePixelDetail(const Array2D<Color>& _detail)
	{
		if (_detail.isEmpty() || wicBitmap_ == nullptr)
			return;

		UINT bytesPerPixel = getPixelByte();

		ComPtr<IWICBitmapLock> pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, pILock.GetAddressOf());

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

						*(pixelData + 2) = (BYTE)_detail(j, i).r();
						*(pixelData + 1) = (BYTE)_detail(j, i).g();
						*pixelData = (BYTE)_detail(j, i).b();
					}
			}
			else if (bytesPerPixel == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						*(pixelData + 3) = (BYTE)(_detail(j, i).alpha * 255.0f);
						*(pixelData + 2) = (BYTE)_detail(j, i).r();
						*(pixelData + 1) = (BYTE)_detail(j, i).g();
						*pixelData = (BYTE)_detail(j, i).b();
					}
			}
		}
		pILock.Reset();
		d2dBitmapUpdateTag_ = true;
	}

	void Bitmap::updatePixelDetail(const Array2D<Color>& _detail, UINT _startX, UINT _startY)
	{
		if (_detail.isEmpty() || wicBitmap_ == nullptr)
			return;

		UINT bytesPerPixel = getPixelByte();

		ComPtr<IWICBitmapLock> pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, pILock.GetAddressOf());

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

						*(pixelData + 2) = (BYTE)_detail(_startX + j, _startY + i).r();
						*(pixelData + 1) = (BYTE)_detail(_startX + j, _startY + i).g();
						*pixelData = (BYTE)_detail(_startX + j, _startY + i).b();
					}
			}
			else if (bytesPerPixel == 4)
			{
				for (unsigned int i = 0; i < height_; i++)
					for (unsigned int j = 0; j < width_; j++)
					{
						BYTE* pixelData = pv + i * stride + j * bytesPerPixel;

						*(pixelData + 3) = (BYTE)(_detail(_startX + j, _startY + i).alpha * 255.0f);
						*(pixelData + 2) = (BYTE)_detail(_startX + j, _startY + i).r();
						*(pixelData + 1) = (BYTE)_detail(_startX + j, _startY + i).g();
						*pixelData = (BYTE)_detail(_startX + j, _startY + i).b();
					}
			}
		}
		pILock.Reset();
		d2dBitmapUpdateTag_ = true;
	}

	void Bitmap::changePixelDetailRough(std::function<void(Color&)> _function)
	{
		if (wicBitmap_ == nullptr)
			return;

		UINT bytesPerPixel = getPixelByte();

		ComPtr<IWICBitmapLock> pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, pILock.GetAddressOf());

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
		pILock.Reset();
		d2dBitmapUpdateTag_ = true;
	}

	void Bitmap::changePixelDetail(std::function<void(UINT, UINT, Color&)> _function)
	{
		if (wicBitmap_ == nullptr)
			return;

		UINT bytesPerPixel = getPixelByte();

		ComPtr<IWICBitmapLock> pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, pILock.GetAddressOf());

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
		pILock.Reset();
		d2dBitmapUpdateTag_ = true;
	}

	void Bitmap::viewPixelDetail(std::function<void(UINT, UINT, const Color&)> _viewFunction)const
	{
		if (wicBitmap_ == nullptr)
			return;

		if (bytesPerPixel_ == 0)
			return;

		ComPtr<IWICBitmapLock> pILock = nullptr;
		WICRect rcLock = { 0, 0, (int)width_, (int)height_ };
		wicBitmap_->Lock(&rcLock, WICBitmapLockWrite, pILock.GetAddressOf());

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
		pILock.Reset();
	}

	ComPtr<ID2D1Bitmap> GetBlendedBitmapFromFile(
		const ComPtr<IWICImagingFactory>& pIWICFactory,
		const ComPtr<ID2D1RenderTarget>& pRenderTarget,
		const wchar_t* uri,
		const D2D1_COLOR_F& color)
	{
		ComPtr<ID2D1Bitmap> pBitmap;
		ComPtr<IWICBitmapDecoder> pDecoder = NULL;
		ComPtr<IWICBitmapFrameDecode> pSource = NULL;
		ComPtr<IWICBitmap> pWIC = NULL;
		ComPtr<IWICFormatConverter> pConverter = NULL;
		ComPtr<IWICBitmapScaler> pScaler = NULL;
		UINT            originalWidth = 0;
		UINT            originalHeight = 0;

		HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
			uri,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, pSource.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			hr = pSource->GetSize(&originalWidth, &originalHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateBitmapFromSourceRect(
				pSource.Get(), 0, 0, (UINT)originalWidth, (UINT)originalHeight, pWIC.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateFormatConverter(pConverter.GetAddressOf());
		}
		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateBitmapScaler(pScaler.GetAddressOf());
		}
		if (SUCCEEDED(hr))
		{
			hr = pScaler->Initialize(pWIC.Get(), (UINT)originalWidth, (UINT)originalHeight, WICBitmapInterpolationModeCubic);
		}
		if (SUCCEEDED(hr))
		{
			hr = pConverter->Initialize(
				pScaler.Get(),
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
				pConverter.Get(),
				NULL,
				pBitmap.GetAddressOf()
			);
		}

		return pBitmap;
	}

	HRESULT LoadBitmapFromFile(
		const ComPtr<ID2D1RenderTarget>& pRenderTarget,
		const ComPtr<IWICImagingFactory>& pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ComPtr<ID2D1Bitmap>& ppBitmap
	)
	{
		HRESULT hr = S_OK;

		ComPtr<IWICBitmapDecoder> pDecoder = NULL;
		ComPtr<IWICBitmapFrameDecode> pSource = NULL;
		ComPtr<IWICStream> pStream = NULL;
		ComPtr<IWICFormatConverter> pConverter = NULL;
		ComPtr<IWICBitmapScaler> pScaler = NULL;
		ComPtr<IWICBitmap> pWIC = NULL;

		hr = pIWICFactory->CreateDecoderFromFilename(
			uri,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder
		);
		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, pSource.GetAddressOf());
		}
		if (SUCCEEDED(hr))
		{
			hr = pIWICFactory->CreateFormatConverter(pConverter.GetAddressOf());
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

				hr = pIWICFactory->CreateBitmapScaler(pScaler.GetAddressOf());
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource.Get(),
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
					);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler.Get(),
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
				pConverter.Get(),
				nullptr,
				ppBitmap.GetAddressOf()
			);
		}
		return hr;
	}

	HRESULT loadWICBitmap(ComPtr<IWICBitmap>& _ppWicBitmap, const wchar_t* _uri)
	{
		ComPtr<IWICBitmapDecoder> pDecoder = nullptr;
		ComPtr<IWICBitmapFrameDecode> pSource = nullptr;
		UINT            originalWidth = 0;
		UINT            originalHeight = 0;

		auto pWICFactory = WICFactoryGlobal::getWICFactory();
		HRESULT hr = pWICFactory->CreateDecoderFromFilename(
			_uri,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, pSource.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			hr = pSource->GetSize(&originalWidth, &originalHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pWICFactory->CreateBitmapFromSourceRect(
				pSource.Get(), 0, 0, (UINT)originalWidth, (UINT)originalHeight, _ppWicBitmap.GetAddressOf());
		}

		return hr;
	}

	HRESULT loadWICBitmap(ComPtr<IWICBitmap>& _ppWicBitmap, const wchar_t* _uri, UINT _x, UINT _y, UINT _width, UINT _height)
	{
		ComPtr<IWICBitmapDecoder> pDecoder = nullptr;
		ComPtr<IWICBitmapFrameDecode> pSource = nullptr;

		auto pWICFactory = WICFactoryGlobal::getWICFactory();
		HRESULT hr = pWICFactory->CreateDecoderFromFilename(
			_uri,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, pSource.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			hr = pWICFactory->CreateBitmapFromSourceRect(
				pSource.Get(), _x, _y, _width, _height, _ppWicBitmap.GetAddressOf());
		}

		return hr;
	}

	HRESULT createWICBitmap(ComPtr<IWICBitmap>& _pWicBitmap, UINT _width, UINT _height)
	{
		ComPtr<IWICImagingFactory> pWicFactory = nullptr;
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pWicFactory));
		if (FAILED(hr)) return hr;

		hr = pWicFactory->CreateBitmap(
			_width,
			_height,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapCacheOnLoad,
			_pWicBitmap.GetAddressOf());

		return hr;
	}

	HRESULT getD2DBitmap(const ComPtr<IWICBitmap>& _pWicBitmap, ComPtr<ID2D1Bitmap>& _ppD2dBitmap)
	{
		if (!_pWicBitmap) return S_FALSE;

		ComPtr<IWICBitmapDecoder> pDecoder = NULL;
		ComPtr<IWICBitmapFrameDecode> pSource = NULL;
		ComPtr<IWICFormatConverter> pConverter = NULL;
		ComPtr<IWICBitmapScaler> pScaler = NULL;
		UINT width = 0, height = 0;
		HRESULT hr = S_OK;

		auto pWICFactory = WICFactoryGlobal::getWICFactory();
		if (SUCCEEDED(hr))
		{
			hr = pWICFactory->CreateFormatConverter(pConverter.GetAddressOf());
		}
		if (SUCCEEDED(hr))
		{
			hr = pWICFactory->CreateBitmapScaler(pScaler.GetAddressOf());
		}
		if (SUCCEEDED(hr))
		{
			hr = _pWicBitmap->GetSize(&width, &height);
		}
		if (SUCCEEDED(hr))
		{
			hr = pScaler->Initialize(_pWicBitmap.Get(), width, height, WICBitmapInterpolationModeCubic);
		}
		if (SUCCEEDED(hr))
		{
			hr = pConverter->Initialize(
				pScaler.Get(),
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
				pConverter.Get(),
				NULL,
				_ppD2dBitmap.GetAddressOf()
			);
		}

		return hr;
	}

	HRESULT getWICBitmap(const ComPtr<ID2D1Bitmap>& _pD2dBitmap, ComPtr<IWICBitmap>& _ppWicBitmap)
	{
		if (!_pD2dBitmap || !_ppWicBitmap.GetAddressOf())
			return E_POINTER;

		_ppWicBitmap = nullptr;

		D2D1_SIZE_U size = _pD2dBitmap->GetPixelSize();
		HRESULT hr;

		ComPtr<IWICBitmap> pWicBitmap = nullptr;
		hr = createWICBitmap(pWicBitmap, size.width, size.height);
		if (FAILED(hr)) return hr;

		ComPtr<ID2D1Bitmap> pDstD2dBitmap = nullptr;
		hr = pMainRenderTarget->CreateBitmapFromWicBitmap(pWicBitmap.Get(), nullptr, &pDstD2dBitmap);
		if (FAILED(hr)) { return hr; }

		hr = pDstD2dBitmap->CopyFromBitmap(
			nullptr,
			_pD2dBitmap.Get(),
			nullptr
		);

		if (SUCCEEDED(hr))
		{
			_ppWicBitmap = pWicBitmap.Detach();
		}
		else
		{
			// pWicBitmap will be released by ComPtr destructor
		}

		// pDstD2dBitmap will be released by ComPtr destructor
		return hr;
	}

	std::pair<UINT, UINT> getBitmapSize(const ComPtr<IWICBitmap>& _wicBitmap)
	{
		if (!_wicBitmap)
		{
			WARNINGWINDOW_GLOBAL("WIC bitmap is null");
			return { 0,0 };
		}
		UINT width, height;
		HRESULT hr = _wicBitmap->GetSize(&width, &height);
		if (SUCCEEDED(hr))
			return { width, height };
		else
		{
			WARNINGWINDOW_GLOBAL("Failed to get WIC bitmap size");
			return { 0,0 };
		}
	}

	std::pair<UINT, UINT> getBitmapSize(const ComPtr<ID2D1Bitmap>& _d2dBitmap)
	{
		if (!_d2dBitmap)
		{
			WARNINGWINDOW_GLOBAL("D2D bitmap is null");
			return { 0,0 };
		}
		D2D1_SIZE_U size = _d2dBitmap->GetPixelSize();
		return { size.width, size.height };
	}

	void getHitAreaFromBitmap(memory::Array2D<bool>& _hitArea, const Bitmap& _bitmap, float _alphaThreshold)
	{
		auto [w, h] = _hitArea.getSize(); // rows -> width, cols -> height
		if (w != _bitmap.getWidth() || h != _bitmap.getHeight())
		{
			WARNINGWINDOW_GLOBAL("The size of hit area does not match the bitmap size.");
			return;
		}
		_bitmap.viewPixelDetail([&](const UINT _x, const UINT _y, const Color& _color)
			{
				if (_color.alpha <= _alphaThreshold)
					_hitArea(_x, _y) = 0;
				else _hitArea(_x, _y) = 1;
			});
	}

	void drawBitmap(const ComPtr<ID2D1RenderTarget>& _renderTarget, const ComPtr<ID2D1Bitmap>& _pBitmap, float _x, float _y,
		float _width, float _height, float _alpha, Transform _transform)
	{
		if (!_pBitmap) return;
		setPaintingTransform(_transform);
		_renderTarget->DrawBitmap(
			_pBitmap.Get(),
			D2D1::RectF(
				_x, _y,
				_x + fabs(_width),
				_y + fabs(_height)
			),
			_alpha
		);
	}

	void drawBitmap(const ComPtr<ID2D1RenderTarget>& _renderTarget, const ComPtr<ID2D1Bitmap>& _pBitmap, float _width, float _height, float _alpha, Transform _transform)
	{
		if (!_pBitmap) return;
		setPaintingTransform(_transform);
		_renderTarget->DrawBitmap(
			_pBitmap.Get(),
			D2D1::RectF(
				0.0f, 0.0f,
				fabs(_width),
				fabs(_height)
			),
			_alpha
		);
	}

	void RenderBitmap::paint() const
	{
		drawBitmap(pMainRenderTarget, d2dBitmap_, 
			0, 0, (float)GameWindow::width, (float)GameWindow::height, 1.0);
	}
}