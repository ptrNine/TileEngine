#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <SFML/Graphics.hpp>
#include "../base/types.hpp"
#include "component.hpp"


class CWindow : public sf::RenderWindow{

private:
	typedef sf::RenderWindow inherited;

protected:
	std::vector<CComponent*> _components;			// главный контейнер компонентов
	std::vector<CComponent*> _addingComponents;		// контейнер с компонентами, которые надо добавить
	sf::Vector2u			_lastSize;
	sf::Event				_inputText;
	bool					_isActive;



public:
	CWindow();
	CWindow(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
	explicit CWindow(sf::WindowHandle handle, const sf::ContextSettings& settings = sf::ContextSettings());

	void	operateGui();
	void	addComponent(CComponent* component);

	bool					isActive() const;

	

protected:
	void onCreate() override;
	void onResize() override;
};


#endif // !_WINDOW_HPP_
