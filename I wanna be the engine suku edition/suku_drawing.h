#pragma once
#include "framework.h"
#include "suku_foundation.h"
#include "game_saving.h"


namespace suku
{
#define SAFE_ADDREF(P) if(P) P->AddRef();
#define SAFE_RELEASE(P) if(P){P -> Release(); P = nullptr;}

	class Transform;
	class Shape;
	class Bitmap;
	class Color;
	class Effect;

	extern ID2D1Factory* pD2DFactory;
	extern ID2D1HwndRenderTarget* pMainRenderTarget;
	extern IWICImagingFactory* pIWICFactory;

	void suku_drawing_init(HWND _hWnd);
	void suku_drawing_uninit();

	class Transform
	{
	public:
		D2D1::Matrix3x2F matrix;

		Transform();
		Transform(D2D1::Matrix3x2F _matrix);

		void transformPoint(float* _x, float* _y);
		Vector transformPoint(float _x, float _y);
		Transform invertTransform();

		Transform operator +(Transform _x);	//recommend using this
		Transform operator *(Transform _x);
		void operator =(Transform _x);
	};

	Transform translation(float _shiftX, float _shiftY);
	Transform rotation(float _rotateCenterX, float _rotateCenterY, float _angle);
	Transform scale(float _centerX, float _centerY, float _scaleX, float _scaleY);
	Transform skew(float _centerX, float _centerY, float _angleX, float _angleY);
	void setPaintingTransform(Transform _transform);

	class Shape
	{
	public:
		Transform transform;
		ID2D1TransformedGeometry* currentGeometry;
		ID2D1Geometry* originalGeometry;
		//ID2D1Brush* fillBrush;
		//ID2D1Brush* outlineBrush;
		//ID2D1StrokeStyle* outlineStrokeStyle;
		//float outlineWidth;

		//Shape& operator= (Shape& _x)const = delete;

		Shape();
		Shape(const Shape& _x);
		Shape(Shape&& _x)noexcept;
		Shape(ID2D1Geometry* _geometry, Transform _transform = Transform());
		//Shape(ID2D1Geometry* _geometry, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush,
		//	float _edgeWidth = 1.0, ID2D1StrokeStyle* _outlineStrokeStyle = nullptr);
		void join();
		~Shape();

		void setOriginalGeometry(ID2D1Geometry* _geometry);
		void setTransform(Transform _transform);
		void paint(float _x, float _y,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		void paint(float _x, float _y, Transform _paintingTransform,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		void paint(Transform _paintingTransform,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);

		bool isCrashed(Shape& _x);

		Shape& operator= (const Shape& _x);
		Shape& operator= (Shape&& _x)noexcept;
		Shape operator-(const Shape& _x);
		Shape operator&(const Shape& _x);
		Shape operator|(const Shape& _x);
		Shape operator^(const Shape& _x);

		bool operator==(const Shape& other) const = default;
	};

	class SquareShape :public Shape
	{
	public:
		const float length, startX, startY;
		SquareShape(float _length, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	class RectangleShape :public Shape
	{
	public:
		const float width, height, startX, startY;
		RectangleShape(float _width, float _height, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	class CircleShape :public Shape
	{
	public:
		const float radius, startX, startY;
		CircleShape(float _radius, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	class EllipseShape :public Shape
	{
	public:
		const float radiusX, radiusY, startX, startY;
		EllipseShape(float _radiusX, float _radiusY, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	class Bitmap
	{
	public:
		Bitmap();
		//Create an empty bitmap
		Bitmap(UINT _width, UINT _height);
		//Create bitmap from file
		Bitmap(String _url);
		Bitmap(String _url, UINT _x, UINT _y, UINT _width, UINT _height);
		//Create bitmap from Color[][]
		Bitmap(Color** _pixels, UINT _width, UINT _height);
		Bitmap(Color** _pixels, UINT _x, UINT _y, UINT _width, UINT _height);
		Bitmap(IWICBitmap* _wicBitmap);
		Bitmap(const Bitmap& _otherBitmap);
		Bitmap(Bitmap&& _otherBitmap)noexcept;
		~Bitmap();

		UINT getPixelByte();

		void paint(float _x, float _y, float _alpha = 1.0f)const;
		void paint(float _x, float _y, Transform _transform, float _alpha = 1.0f)const;
		void paint(Transform _transform, float _alpha = 1.0f)const;
		UINT getWidth()const;
		UINT getHeight()const;
		std::pair<UINT, UINT> getSize()const;

		//This will create a new piece of memory; remember to use free2D() to delete it after using!
		Color** getPixelDetail()const;

		//This will not create a new piece of memory
		BYTE* getDataPointer()const;

		Bitmap& operator=(const Bitmap& _bitmap);
		Bitmap& operator=(Bitmap&& _bitmap)noexcept;

		void updatePixelDetail(Color** _detail);
		void updatePixelDetail(Color** _detail, UINT _startX, UINT _startY);
		void changePixelDetailRough(std::function<void(Color&)> _changingFunction);

		//Parameters: x, y, the corresponding color of position (x,y)
		void changePixelDetail(std::function<void(UINT, UINT, Color&)> _changingFunction);

		//Parameters: x, y, the corresponding color of position (x,y)
		void viewPixelDetail(std::function<void(UINT, UINT, const Color&)> _viewFunction)const;
	private:
		UINT bytesPerPixel_;
		UINT width_, height_;
		ID2D1Bitmap* d2d1Bitmap_;
		IWICBitmap* wicBitmap_;
	};

	class Color
	{
	public:
		float alpha;	//range: 0.0f ~ 1.0f

		Color();
		Color(float _r, float _g, float _b, float _alpha = 1.0f);

		inline float r()const { return r_; }
		inline float g()const { return g_; }
		inline float b()const { return b_; }
		float h()const;
		float s()const;
		float v()const;
		inline void setR(float _r) { r_ = _r; }
		inline void setG(float _g) { g_ = _g; }
		inline void setB(float _b) { b_ = _b; }
		inline void setRGB(float _r, float _g, float _b) { r_ = _r, g_ = _g, b_ = _b; }
		void setH(float _h);
		void setS(float _s);
		void setV(float _v);
		void setHSV(float _h, float _s, float _v);
	private:
		float r_, g_, b_;	//range: 0.0f ~ 255.0f
	};

	class Effect
	{
	public:

	};

	void SwapByte(BYTE* _a, BYTE* _b);

	void beginDraw(HWND hWnd);
	void endDraw();
	void clearScreen();

	HRESULT loadWICBitmap(
		IWICBitmap** _pWicBitmap,
		const wchar_t* uri	//absolute path
	);
	HRESULT loadWICBitmap(
		IWICBitmap** _pWicBitmap,
		const wchar_t* _uri,	//absolute path
		UINT _x, UINT _y, UINT _width, UINT _height
	);

	//Due to technical reason, _width and _height in CreateWICBitmap() must <= 2048
	HRESULT createWICBitmap(
		IWICBitmap** _pWicBitmap,
		UINT _width,
		UINT _height
	);

	HRESULT getD2DBitmap(
		IWICBitmap** _pWicBitmap,
		ID2D1Bitmap** _pD2dBitmap,
		int _width,
		int _height
	);

	//This will create a new piece of memory; remember to use free2D() to delete it after using!
	Color** getPixelDetailFromWICBitmap(IWICBitmap* _sourceBitmap);

	//This will create a new piece of memory; remember to use free2D() to delete it after using!
	Color** getPixelDetailFromWICBitmap(IWICBitmap* _sourceBitmap, UINT _x, UINT _y, UINT _width, UINT _height);

	ID2D1Brush* createSolidColorBrush(const Color _color);

	std::pair<UINT, UINT> getSizeFromWICBitmap(IWICBitmap* _pBitmap, HRESULT* _pHResult = nullptr);
	void getHitAreaFromBitmap(bool** _pHitArea, const Bitmap& _pBitmap, float _alphaThreshold = 0.0f);
	//void drawBitmap(ID2D1Bitmap* _pBitmap, float _x, float _y,
	//	float _width, float _height, float _alpha,
	//	float _angle, float _rotatingCenterX, float _rotatingCenterY);
	void drawBitmap(ID2D1Bitmap* _pBitmap, float _x, float _y,
		float _width, float _height, float _alpha,
		Transform _transform = Transform());
	void drawBitmap(ID2D1Bitmap* _pBitmap,
		float _width, float _height, float _alpha,
		Transform _transform = Transform());
}