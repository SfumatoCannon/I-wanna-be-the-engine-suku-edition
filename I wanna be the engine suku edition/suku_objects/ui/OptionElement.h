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
			ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height, String _info = String());
		template<typename T> OptionElement(ConfigElement<T>& _bindedConfig, String _label) : OptionElement(_bindedConfig, _label, 0, 0, 256, 32) {}
		template<typename T> OptionElement(ConfigElement<T>& _bindedConfig, String _label, String _info) : OptionElement(_bindedConfig, _label, 0, 0, 256, 32, _info) {}

		virtual void update() override { onUpdateFunc_(this); }
		virtual bool onPaint() override { return onPaintFunc_(this); }
	private:
		friend class OptionPool;
		String label_;
		Text labelText_;
		Text valueText_;
		std::function<void(OptionElement*)> onUpdateFunc_;
		std::function<bool(OptionElement*)> onPaintFunc_;
		bool hasInfo_ = false;
		String info_;
		Text infoText_;
		bool isSelected_ = false;
		int originalHeight_;
	};

	template<typename T>
	inline OptionElement::OptionElement(ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height, String _info)
		: UIElement(_x, _y, _width, _height), label_(_label), originalHeight_(_height),
		labelText_("Consolas", _height / 2, TextAlign::MiddleLeft), valueText_("Consolas", _height / 2, TextAlign::MiddleRight), infoText_("Consolas", 16, TextAlign::TopLeft)
	{
		if (!_info.isEmpty())
		{
			hasInfo_ = true;
			info_ = _info;
			infoText_.contentString = _info;
		}

		onUpdateFunc_ = [&_bindedConfig](OptionElement* _element)
			{
				if (_element->hasInfo_ && _element->isSelected_)
				{
					_element->height_ = _element->originalHeight_ + 8 + _element->infoText_.getContentHeight(_element->getWidth() - 16);
				}
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

				// label text
				_element->labelText_.setBrush(Color(255, 255, 255));
				_element->labelText_.contentString = _element->label_;
				_element->labelText_.paint(_element->x + 8, _element->y + 8, _element->getWidth() - 16, _element->originalHeight_ - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));

				// value text
				_element->valueText_.setBrush(Color(255, 255, 255));
				_element->valueText_.contentString = std::to_wstring(_bindedConfig.value());
				_element->valueText_.paint(_element->x + 8, _element->y + 8, _element->getWidth() - 16, _element->originalHeight_ - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				
				if (_element->hasInfo_ && _element->isSelected_)
				{
					float height = _element->infoText_.getContentHeight(_element->getWidth() - 16);
					_element->infoText_.paint(_element->x + 8, _element->y + _element->originalHeight_ + 4, _element->getWidth() - 16, height, graphics::createSolidColorBrush(Color(255, 255, 255)));
				}
				return false;
			};
	}
}