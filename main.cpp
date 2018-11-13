
#include <SFML/Graphics.hpp>
#include "gui/window.hpp"
#include "gui/panel.h"
#include "gui/label.hpp"
#include "gui/button.hpp"
#include "base/timer.hpp"
#include "base/AnimatedRect.hpp"
#include "textureManager.hpp"
#include "fontManager.hpp"
#include "base/fps_counter.hpp"
#include "GameBase.hpp"
#include "base/logger.hpp"
#include "gui/tests/mouse_tests.hpp"
#include "base/io/tests/keyboard_tests.hpp"
#include "gui/button_array.hpp"
#include "gui/edit.hpp"
#include "base/io/mouse.hpp"

#include "game/objects/prop.hpp"
#include "game/collisions/collisionRectangle.hpp"
#include "game/objects/bulletManager.hpp"
#include "gui/select.hpp"
#include "gui/scrollBar.hpp"



int main() {
    //EXAMPLE

    CWindow window(sf::VideoMode(1200, 600), "mda", sf::Style::Close | sf::Style::Resize);
    CEngineKernel kernel = CEngineKernel(&window);
    kernel.setDrawCollisions(true);
    kernel.load_map("super_duper_map");
    CEdit edit = CEdit("adfasdas", sf::FloatRect(100, 100, 200, 100), CLabel::TextAligment::centered, CLabel::Aligment::movable);
    CSelect select = CSelect(sf::FloatRect(400, 100, 200, 50), CComponent::Aligment::movable);
    select.addOption("option1");
    select.addOption("option2");
    select.addOption("option3");
    select.addOption("option4");


    CScrollBar scrollBar = CScrollBar(sf::FloatRect(700, 100, 30, 200));
    CScrollBar scrollBar2 = CScrollBar(sf::FloatRect(750, 100, 100, 20), true, CScrollBar::Aligment::none);

    CButton but = CButton("dfff", sf::FloatRect(800, 200, 100, 50), CButton::Aligment::movable);
    but.setAction([&]() {
        scrollBar.setSize(scrollBar.getSize() + sf::Vector2f(0.0f, 1.5f));
    });

    window.addComponent(&edit);
    window.addComponent(&select);
    window.addComponent(&scrollBar);
    window.addComponent(&scrollBar2);
    window.addComponent(&but);
    auto anim_texture = CTextureManager::inst().load("dummy_dude.png");

    sf::Vector2u size(40, 50);

    CAnimatedRect dude_down(sf::Vector2f(80.f, 100.f));
    dude_down.create(anim_texture, size, 120, 6);
    dude_down.setPosition(window.getView().getCenter());
    CAnimatedRect dude_up(sf::Vector2f(80.f, 100.f));
    dude_up.create(anim_texture, size, 120, 6, sf::Vector2u(0, 50));
    dude_up.setPosition(window.getView().getCenter());
    CAnimatedRect dude_right(sf::Vector2f(80.f, 100.f));
    dude_right.create(anim_texture, size, 120, 6, sf::Vector2u(0, 100));
    dude_right.setPosition(window.getView().getCenter());
    CAnimatedRect dude_left(sf::Vector2f(80.f, 100.f));
    dude_left.create(anim_texture, size, 120, 6, sf::Vector2u(0, 150));
    dude_left.setPosition(window.getView().getCenter());
    auto* dr = dynamic_cast<sf::Drawable*>(&dude_left);

    /*
    CPanel panel(sf::FloatRect(0.f, 0.f, 700.f, 400.f), CPanel::Aligment::resizable);
    panel.setFilling(sf::Color(127, 127, 127, 127));
    CButton button("mda", sf::FloatRect(100.f, 200.f, 200.f, 40.f), CButton::Aligment::movable);
    button.setTextColor(sf::Color(166, 0, 0, 255));


    CButtonArray btnArr(sf::FloatRect(400.f, 100.f, 100.f, 40.f), 1, CButtonArray::Aligment::resizable);
    btnArr.setThemeFromButton(button);
    btnArr.setBorderSize(2.f);
    btnArr.addButton("kkek", []() {
        printf("heh\n");
    });
    btnArr.addButton("k2", []() {
        printf("h3424eh\n");
    });
    btnArr.addButton("22", []() {
        printf("he24244242h\n");
    });
    btnArr.addButton("44", [&]() {
        printf("he42424h\n");
        CButton* btn = new CButton("new", sf::FloatRect(520.f, 100.f, 100.f, 40.f), CButton::Aligment::movable);
        panel.addComponent(btn);
    });

    CButton button2(
        "align",
        sf::FloatRect(300.f, 200.f, 200.f, 40.f),
        CButton::Aligment::movable
    );

    button.setTextAligment(CButton::TextAligment::centered);

    CLabel label(
        "zva",
        sf::FloatRect(20.f, 140.f, 200.f, 100.f),
        CLabel::TextAligment::sizeAdaptive,
        CLabel::Aligment::none
    );
    CEdit edit(
        "edit",
        sf::FloatRect(200.f, 140.f, 200.f, 100.f),
        CLabel::TextAligment::centered,
        CLabel::Aligment::none
    );

    btnArr.addButton("edit", [&]() {
        label.setEditiable(true);
    });

    CAnimatedRect anim(sf::Vector2f(256, 256));
    anim.create(CTextureManager::inst().load("shells.png"), sf::Vector2u(64, 64), 40, 32);
    anim.setPosition(sf::Vector2f(300.f, 300.f));

    button.setAction([&]() {
        label.addText("kffdsfsgdfdsf\nfff");
        label.setFontSize(60);
        button.move(sf::Vector2f(3.f, 3.f));
    });

    button2.setAction([&]() {
        label.setTextAligment(CLabel::TextAligment::centered);
    });

    window.addComponent(&panel);
    panel.addComponent(&label);
    panel.addComponent(&button);
    panel.addComponent(&button2);
    panel.addComponent(&btnArr);
    panel.addComponent(&edit);

    */


//	CWindow window(sf::VideoMode(1200, 600), "DayXYZ", sf::Style::Close | sf::Style::Resize);

    //CMouseClickTest test(&window);
    //test.run();
    //CKeyboardClickTest test(&window);
    //test.run();

    /*
    CEngineKernel* ker = new CEngineKernel(&window);
    ker->load_map("super_duper_map");

    auto viewPoint = sf::Vector2f(0.0f, 0);
    */
    sf::Font font = base::FontManager().getFont("font_anonymous");
    CLabel systemInfo;
    systemInfo.setFont(font);
    systemInfo.setAligment(CLabel::Aligment::none);
    systemInfo.setTextAligment(CLabel::TextAligment::sizeAdaptive);
    systemInfo.setPos(sf::Vector2f(5.0f, 5.0f));

    window.addComponent(&systemInfo);
    //*/
    sf::Vector2f viewpos = { 0.f, 0.f };



    ///// PROP TEST ///////////////////////////
    sf::RectangleShape propshape;
    sf::Texture* proptexture = CTextureManager::inst().load("rx_only.png");
    proptexture->setSmooth(true);
    propshape.setTexture(proptexture);
    //propshape.setPosition(0.0f, 0.0f);
    //propshape.setSize(sf::Vector2f(100.0f, 100.0f));

    sf::RectangleShape bulletshape;
    bulletshape.setFillColor(sf::Color(100, 255, 255));
    //bulletshape.setSize(sf::Vector2f(5, 50));

    auto* prop = new CProp();
    prop->setPos(0.0, 0.0);
    prop->setSize(vector2d(0.6, 0.6));
    prop->setShape(propshape);
    prop->getCollisionRect()->setShift(vector2d(0.13, 0.1));
    prop->getCollisionRect()->setSize(vector2d(0.34, 0.48));
    kernel.addObject(prop);
    prop->setDir(vector2d(-0.6, 0.8));
    prop->setSpeed(4.0);

    //CCollisionPoint* cprop = new CCollisionRectangle(prop, vector2d(0.4, 0.6), vector2d(0.1, 0.1));
    //CCollisionManager::inst().add(cprop);


    auto last_pos = viewpos;

    ////////////////////////////////////////// ИГРОВОЙ ЦИКЛ///////////////////////////////////////////
    double	timestep = 0.0;
    auto	last_timestamp = zog::timer().timestamp();
    while (window.isOpen()) {
        auto timestamp = zog::timer().timestamp();
        timestep = (timestamp - last_timestamp).sec();
        last_timestamp = timestamp;

        window.clear();

        if (zog::keyboard().isHolding(sf::Keyboard::D)) {
            viewpos.x += 0.04f;
        }
        if (zog::keyboard().isHolding(sf::Keyboard::A)) {
            viewpos.x -= 0.04f;
        }
        if (zog::keyboard().isHolding(sf::Keyboard::W)) {
            viewpos.y -= 0.04f;
        }
        if (zog::keyboard().isHolding(sf::Keyboard::S)) {
            viewpos.y += 0.04f;
        }
        if (zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Subtract)) {
            kernel.setCameraXFactor(kernel.getCameraXFactor() + 1);
        }
        if (zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Add)) {
            kernel.setCameraXFactor(kernel.getCameraXFactor() - 1);
        }
        if (zog::keyboard().isHolding(sf::Keyboard::Right)) {
            auto pos = prop->getPos();
            pos.add(vector2d(0.01, 0.0));
            prop->setPos(pos);
        }
        if (zog::keyboard().isHolding(sf::Keyboard::Left)) {
            auto pos = prop->getPos();
            pos.add(vector2d(-0.01, 0.0));
            prop->setPos(pos);
        }
        if (zog::keyboard().isHolding(sf::Keyboard::Up)) {
            auto pos = prop->getPos();
            pos.addY(-0.01);
            prop->setPos(pos);
        }
        if (zog::keyboard().isHolding(sf::Keyboard::Down)) {
            auto pos = prop->getPos();
            pos.addY(0.01);
            prop->setPos(pos);
        }
        if (zog::mouse().isPressed(0)) {
            auto bullet = CBulletManager::inst().add(viewpos, vector2d(zog::mouse().getPositionF()) - vector2d(window.getView().getCenter()), 100.0, nullptr);
            bullet->setSize(vector2d(0.1, 2.0));
            bullet->setPhysicParams(1.0, 1000.0);
        }



        //ker->update(viewPoint, 0.2);
        //ker->draw(viewPoint);

        shared_str info;
        info.sprintf("window size x: %i y: %i FPS = %f Objects: %i  PxlToM: %i Bullets: %i\n", window.getSize().x, window.getSize().y, zog::FPS(), kernel.infoObjectsInView(), kernel.getCameraXFactor(), CBulletManager::inst().getCount());
        systemInfo.setText(info.c_str(), 14, sf::Color(255, 255, 255, 255));

        kernel.update(vector2d(viewpos), timestep);


        // update physics
        //CCollisionManager::inst().alreadyTestedReset();
        //cprop->updatePreCollisionRect(vector2d(2.0, 4.9));
        //cprop->test();
        //CCollisionManager::inst().solveCollisions();


        kernel.draw(vector2d(viewpos));

        //if (last_pos.y > viewpos.y)
        //	window.draw(dude_up);
        //else if (last_pos.y < viewpos.y)
        //	window.draw(dude_down);
        //else if (last_pos.x < viewpos.x)
        //	window.draw(dude_right);
        //else
        //	window.draw(*dr);



        window.operateGui();

        window.display();
        //viewPoint.x += 0.01f;
        //viewPoint.y += 0.01f;

        last_pos = viewpos;
        //zog::sleep(200);
    }
    return 0;
}