#pragma once
#include "UIElement.h"
#include "suku_config/config_element.h"
#include "suku_foundation/input.h"

namespace suku
{
	class OptionPool;

	class OptionElement : public UIElement
	{
	public:
		OptionElement* prev = nullptr;
		OptionElement* next = nullptr;

		template<typename T> OptionElement(
			ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height, String _info = String(), String _infoOnChanging = String());
		template<typename T> OptionElement(ConfigElement<T>& _bindedConfig, String _label) : OptionElement(_bindedConfig, _label, 0, 0, 256, 32) {}
		template<typename T> OptionElement(ConfigElement<T>& _bindedConfig, String _label, String _info, String _infoOnChanging = String()) : OptionElement(_bindedConfig, _label, 0, 0, 256, 32, _info, _infoOnChanging) {}

		virtual void update() override { onUpdateFunc_(this); }
		virtual bool onPaint() override { return onPaintFunc_(this); }
	private:
		friend class OptionPool;
		String label_;
		std::function<void(OptionElement*)> onUpdateFunc_;
		std::function<bool(OptionElement*)> onPaintFunc_;
		bool hasInfo_ = false;
		bool hasInfoOnChanging_ = false;
		String info_;
		String infoOnChanging_;
		bool isSelected_ = false;
		int originalHeight_;
	};

	template<typename T>
	inline OptionElement::OptionElement(ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height, String _info, String _infoOnChanging)
		: UIElement(_x, _y, _width, _height), label_(_label), originalHeight_(_height)
	{
		if (!_info.isEmpty())
		{
			hasInfo_ = true;
			info_ = _info;
		}

		if (!_infoOnChanging.isEmpty())
		{
			hasInfoOnChanging_ = true;
			infoOnChanging_ = _infoOnChanging;
		}

		onUpdateFunc_ = [&_bindedConfig](OptionElement* _element)
			{
				if (_element->hasInfo_ && _element->isSelected_)
					_element->height_ = _element->originalHeight_ + 24;
				else
					_element->height_ = _element->originalHeight_;

				if (!_element->isSelected_)
					return;

				if (input::isKeyDown(VK_LEFT))
				{
					if (_bindedConfig.hasRangeConstraint())
					{
						auto range = _bindedConfig.getRange();
						T newValue = _bindedConfig.getValue() - 1;
						if (newValue < range.first)
							newValue = range.first;
						_bindedConfig.setValue(newValue);
					}
					else if (_bindedConfig.hasListConstraint())
					{
						auto list = _bindedConfig.getValueList();
						auto it = std::find(list.begin(), list.end(), _bindedConfig.value());
						if (it != list.end() && it != list.begin())
							_bindedConfig.setValue(*(it - 1));
					}
					else
					{
						if constexpr (std::is_same_v<bool, T>)
						{
							_bindedConfig.setValue(!_bindedConfig.getValue());
						}
						else if constexpr (std::is_arithmetic_v<T>)
						{
							_bindedConfig.setValue(_bindedConfig.getValue() - 1);
						}
					}
				}
				else if (input::isKeyDown(VK_RIGHT))
				{
					if (_bindedConfig.hasRangeConstraint())
					{
						auto range = _bindedConfig.getRange();
						T newValue = _bindedConfig.getValue() + 1;
						if (newValue > range.second)
							newValue = range.second;
						_bindedConfig.setValue(newValue);
					}
					else if (_bindedConfig.hasListConstraint())
					{
						auto list = _bindedConfig.getValueList();
						auto it = std::find(list.begin(), list.end(), _bindedConfig.value());
						if (it != list.end() && it != list.end() - 1)
							_bindedConfig.setValue(*(it + 1));
					}
					else
					{
						if constexpr (std::is_same_v<bool, T>)
						{
							_bindedConfig.setValue(!_bindedConfig.getValue());
						}
						else if constexpr (std::is_arithmetic_v<T>)
						{
							_bindedConfig.setValue(_bindedConfig.getValue() + 1);
						}
					}
				}
			};
		onPaintFunc_ = [&_bindedConfig](OptionElement* _element)
			{
				RectangleShape area(_element->getWidth(), _element->getHeight());
				area.setFill(_element->isSelected_ ? Color(128, 128, 128, 0.5f) : Color(64, 64, 64, 0.5f));
				area.paint(_element->x, _element->y, _element->transform);
				Text text("Consolas", _element->originalHeight_ / 2, TextAlign::MiddleRight);
				text.setBrush(Color(255, 255, 255));
				text.textContent = std::to_wstring(_bindedConfig.value());
				text.paint(_element->x + 8, _element->y + 8, _element->getWidth() - 16, _element->originalHeight_ - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				text.textContent = _element->label_;
				text.setTextAlign(TextAlign::MiddleLeft);
				text.paint(_element->x + 8, _element->y + 8, _element->getWidth() - 16, _element->originalHeight_ - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				if (_element->hasInfo_ && _element->isSelected_)
				{
					Text infoText("Consolas", 16, TextAlign::TopLeft);
					infoText.setBrush(Color(192, 192, 192));
					infoText.textContent = _element->info_;
					infoText.paint(_element->x + 8, _element->y + _element->originalHeight_ + 4, _element->getWidth() - 16, 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				}
				return false;
			};
	}
}