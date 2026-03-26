#pragma once

namespace suku
{


	namespace graphics
	{
		using Microsoft::WRL::ComPtr;

		class TextFactoryGlobal
		{
		public:
			static IDWriteFactory* getDWriteFactory()
			{
				static TextFactoryGlobal instance;
				return instance.pDWriteFactory_.Get();
			}
			TextFactoryGlobal(const TextFactoryGlobal&) = delete;
			TextFactoryGlobal& operator=(const TextFactoryGlobal&) = delete;
		private:
			TextFactoryGlobal();
			ComPtr<IDWriteFactory> pDWriteFactory_ = nullptr;
		};
	}
}