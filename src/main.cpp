#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <map>

const int WINDOW_HEIGHT = 700;
const int WINDOW_WIDTH = 1000;

const int SHAPE_ONE_HEIGHT = 100;
const int SHAPE_ONE_WIDTH = 100;

const int SHAPE_TWO_HEIGHT = 100;
const int SHAPE_TWO_WIDTH = 100;

int SHAPE_ONE_POSITION_X = 200;
int SHAPE_ONE_POSITION_Y = 100;
std::pair<int, int> shape_one_prev_pos(0, 0);

int SHAPE_TWO_POSITION_X = 500;
int SHAPE_TWO_POSITION_Y = 300;
std::pair<int, int> shape_two_prev_pos(0, 0);

struct shape
{
public:
    sf::RectangleShape rect;
    sf::RectangleShape bounding_box;
    std::pair<int, int> prev_pos;
    sf::Text X;
    sf::Text Y;
    shape(sf::RectangleShape rect_para, sf::RectangleShape bounding,
          sf::Text x_point, sf::Text y_point,
          int x, int y) : rect(rect_para),
                          bounding_box(bounding),
                          X(x_point),
                          Y(y_point),
                          prev_pos(std::make_pair(x, y)) {}
};

std::pair<float, float> detect_collision(shape *shape_one, shape *shape_two, bool prev = false)
{

    float dx, dy;
    if (prev) // change starting prev position
    {
        dx = abs(shape_one->prev_pos.first - shape_two->prev_pos.first);
        dy = abs(shape_one->prev_pos.second - shape_two->prev_pos.second);
    }
    else
    {
        dx = abs(shape_one->rect.getPosition().x - shape_two->rect.getPosition().x);
        dy = abs(shape_one->rect.getPosition().y - shape_two->rect.getPosition().y);
    }

    float w1 = shape_one->rect.getSize().x;
    float w2 = shape_two->rect.getSize().x;

    float h1 = shape_one->rect.getSize().y;
    float h2 = shape_two->rect.getSize().y;

    float ox = (w1 / 2) + (w2 / 2) - dx;
    float oy = (h1 / 2) + (h2 / 2) - dy;

    return std::make_pair(ox, oy);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "collision detection");
    sf::Font font;
    if (!font.loadFromFile("font/Oxygen-Regular.ttf"))
    {
        std::cout << "Cant load Fonts" << std::endl;
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("sound/ping.wav"))
    {
        std::cout << "Error loading mp3" << std::endl;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);

    sf::RectangleShape shape_one(sf::Vector2f(SHAPE_ONE_WIDTH, SHAPE_ONE_HEIGHT));
    sf::RectangleShape shape_two(sf::Vector2f(SHAPE_TWO_WIDTH, SHAPE_TWO_HEIGHT));

    sf::RectangleShape rect1;
    rect1.setSize(sf::Vector2f(SHAPE_ONE_WIDTH, SHAPE_ONE_HEIGHT));
    rect1.setOrigin(sf::Vector2f(SHAPE_ONE_WIDTH / 2, SHAPE_ONE_HEIGHT / 2));
    rect1.setPosition(SHAPE_ONE_POSITION_X, SHAPE_ONE_POSITION_Y);
    rect1.setFillColor(sf::Color(0, 0, 0, 0));
    rect1.setOutlineColor(sf::Color(0, 0, 0, 255));
    rect1.setOutlineThickness(1);

    sf::RectangleShape rect2;
    rect2.setSize(sf::Vector2f(SHAPE_TWO_WIDTH, SHAPE_TWO_HEIGHT));
    rect2.setOrigin(sf::Vector2f(SHAPE_TWO_WIDTH / 2, SHAPE_TWO_HEIGHT / 2));
    rect2.setPosition(SHAPE_TWO_POSITION_X, SHAPE_TWO_POSITION_Y);
    rect2.setFillColor(sf::Color(0, 0, 0, 0));
    rect2.setOutlineColor(sf::Color(0, 0, 0, 255));
    rect2.setOutlineThickness(1);

    sf::Text shape_one_center_x(std::to_string(SHAPE_ONE_POSITION_X), font, 15);
    sf::Text shape_one_center_y(std::to_string(SHAPE_ONE_POSITION_Y), font, 15);

    shape_one_center_x.setPosition(sf::Vector2f(SHAPE_ONE_POSITION_X - 25, SHAPE_ONE_POSITION_Y - 10));
    shape_one_center_y.setPosition(sf::Vector2f(SHAPE_ONE_POSITION_X + 10, SHAPE_ONE_POSITION_Y - 10));

    sf::Text shape_two_center_x(std::to_string(SHAPE_TWO_POSITION_X), font, 15);
    sf::Text shape_two_center_y(std::to_string(SHAPE_TWO_POSITION_Y), font, 15);

    shape_two_center_x.setPosition(sf::Vector2f(SHAPE_TWO_POSITION_X - 25, SHAPE_TWO_POSITION_Y - 10));
    shape_two_center_y.setPosition(sf::Vector2f(SHAPE_TWO_POSITION_X + 10, SHAPE_TWO_POSITION_Y - 10));

    shape_one_center_x.setColor(sf::Color::Black);
    shape_one_center_y.setColor(sf::Color::Black);
    shape_two_center_x.setColor(sf::Color::Black);
    shape_two_center_y.setColor(sf::Color::Black);

    shape shape1 = shape(shape_one, rect1, shape_one_center_x, shape_one_center_y, SHAPE_ONE_POSITION_X, SHAPE_ONE_POSITION_Y);
    shape shape2 = shape(shape_two, rect2, shape_two_center_x, shape_two_center_y, SHAPE_TWO_POSITION_X, SHAPE_TWO_POSITION_Y);

    shape1.rect.setOrigin(SHAPE_ONE_WIDTH / 2, SHAPE_ONE_HEIGHT / 2);
    shape2.rect.setOrigin(SHAPE_TWO_WIDTH / 2, SHAPE_TWO_HEIGHT / 2);

    shape1.rect.setFillColor(sf::Color(78, 245, 103));
    shape2.rect.setFillColor(sf::Color(247, 62, 62));

    // Status
    bool collision = false;
    sf::RectangleShape status;
    status.setSize(sf::Vector2f(200, WINDOW_HEIGHT));
    status.setPosition(WINDOW_WIDTH - 200, 0);
    status.setFillColor(sf::Color(3, 3, 3));
    status.setOutlineColor(sf::Color(0, 0, 0, 255));
    status.setOutlineThickness(1);
    sf::Text collision_text("Collision: ", font, 15);
    collision_text.setPosition(sf::Vector2f(WINDOW_WIDTH - 190, 10));
    collision_text.setColor(sf::Color::Red);

    sf::Text collision_text_x("Collision in x direction: ", font, 15);
    collision_text_x.setPosition(sf::Vector2f(WINDOW_WIDTH - 190, 50));
    collision_text_x.setColor(sf::Color::Red);

    sf::Text collision_text_y("Collision in y direction: ", font, 15);
    collision_text_y.setPosition(sf::Vector2f(WINDOW_WIDTH - 190, 90));
    collision_text_y.setColor(sf::Color::Red);

    sf::Text collision_text_area("Area of collision: ", font, 15);
    collision_text_area.setPosition(sf::Vector2f(WINDOW_WIDTH - 190, 130));
    collision_text_area.setColor(sf::Color::Red);

    sf::Text collision_status_text("false", font, 15);
    collision_status_text.setPosition(sf::Vector2f(WINDOW_WIDTH - 120, 10));
    collision_status_text.setColor(sf::Color::Red);

    sf::Text collision_status_text_x("0", font, 15);
    collision_status_text_x.setPosition(sf::Vector2f(WINDOW_WIDTH - 40, 50));
    collision_status_text_x.setColor(sf::Color::Red);

    sf::Text collision_status_text_y("0", font, 15);
    collision_status_text_y.setPosition(sf::Vector2f(WINDOW_WIDTH - 40, 90));
    collision_status_text_y.setColor(sf::Color::Red);

    sf::Text collision_status_text_area("0", font, 15);
    collision_status_text_area.setPosition(sf::Vector2f(WINDOW_WIDTH - 70, 130));
    collision_status_text_area.setColor(sf::Color::Red);

    sf::Text select_one("Select first: A", font, 15);
    select_one.setPosition(sf::Vector2f(WINDOW_WIDTH - 190, 170));
    select_one.setColor(sf::Color::Red);

    sf::Text select_two("Select second: S", font, 15);
    select_two.setPosition(sf::Vector2f(WINDOW_WIDTH - 190, 210));
    select_two.setColor(sf::Color::Red);

    sf::Text author("By: Aakarshan Raj", font, 15);
    author.setPosition(sf::Vector2f(WINDOW_WIDTH - 195, WINDOW_HEIGHT - 20));
    author.setColor(sf::Color::Red);

    sf::RectangleShape line1;
    line1.setSize(sf::Vector2f(200, 1));
    line1.setPosition(WINDOW_WIDTH - 200, 30);
    line1.setOutlineColor(sf::Color::Blue);

    sf::RectangleShape line2;
    line2.setSize(sf::Vector2f(200, 1));
    line2.setPosition(WINDOW_WIDTH - 200, 70);
    line2.setOutlineColor(sf::Color::Blue);

    sf::RectangleShape line3;
    line3.setSize(sf::Vector2f(200, 1));
    line3.setPosition(WINDOW_WIDTH - 200, 110);
    line3.setOutlineColor(sf::Color::Blue);

    sf::RectangleShape line4;
    line4.setSize(sf::Vector2f(200, 1));
    line4.setPosition(WINDOW_WIDTH - 200, 150);
    line4.setOutlineColor(sf::Color::Blue);

    shape1.rect.setPosition(SHAPE_ONE_POSITION_X, SHAPE_ONE_POSITION_Y);
    shape2.rect.setPosition(SHAPE_TWO_POSITION_X, SHAPE_TWO_POSITION_Y);

    bool draw_rect1 = false;
    bool draw_rect2 = false;

    bool play_ping = false;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {

                    window.close();
                }
                if (event.key.code == sf::Keyboard::A)
                {
                    draw_rect1 = true;
                    draw_rect2 = false;
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    draw_rect1 = false;
                    draw_rect2 = true;
                }
                if (draw_rect1)
                {

                    int moveX = 0;
                    int moveY = 0;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        moveY = -10;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        moveY = 10;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    {
                        moveX = -10;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        moveX = 10;
                    }

                    shape1.prev_pos = std::make_pair(shape1.rect.getPosition().x, shape1.rect.getPosition().y + moveY);

                    shape1.rect.setPosition(sf::Vector2f(shape1.rect.getPosition().x + moveX, shape1.rect.getPosition().y + moveY));
                    shape1.bounding_box.setPosition(sf::Vector2f(shape1.bounding_box.getPosition().x + moveX, shape1.bounding_box.getPosition().y + moveY));
                    shape1.X.setPosition(sf::Vector2f(shape1.X.getPosition().x + moveX, shape1.X.getPosition().y + moveY));
                    shape1.Y.setPosition(sf::Vector2f(shape1.Y.getPosition().x + moveX, shape1.Y.getPosition().y + moveY));

                    shape1.X.setString(std::to_string((int)shape1.rect.getPosition().x));
                    shape1.Y.setString(std::to_string((int)shape1.rect.getPosition().y));
                }
                else if (draw_rect2)
                {
                    int moveX = 0;
                    int moveY = 0;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        moveY = -10;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        moveY = 10;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    {
                        moveX = -10;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        moveX = 10;
                    }

                    shape2.prev_pos = std::make_pair(shape2.rect.getPosition().x, shape2.rect.getPosition().y);

                    shape2.rect.setPosition(sf::Vector2f(shape2.rect.getPosition().x + moveX, shape2.rect.getPosition().y + moveY));
                    shape2.bounding_box.setPosition(sf::Vector2f(shape2.bounding_box.getPosition().x + moveX, shape2.bounding_box.getPosition().y + moveY));
                    shape2.X.setPosition(sf::Vector2f(shape2.X.getPosition().x + moveX, shape2.X.getPosition().y + moveY));
                    shape2.Y.setPosition(sf::Vector2f(shape2.Y.getPosition().x + moveX, shape2.Y.getPosition().y + moveY));

                    shape2.X.setString(std::to_string((int)shape2.rect.getPosition().x));
                    shape2.Y.setString(std::to_string((int)shape2.rect.getPosition().y));
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (shape1.rect.getPosition().x - (SHAPE_ONE_WIDTH / 2) < event.mouseButton.x &&
                        shape1.rect.getPosition().x + (SHAPE_ONE_WIDTH / 2) > event.mouseButton.x &&
                        shape1.rect.getPosition().y - (SHAPE_ONE_HEIGHT / 2) < event.mouseButton.y &&
                        shape1.rect.getPosition().y + (SHAPE_ONE_HEIGHT / 2) > event.mouseButton.y)
                    {
                        draw_rect1 = true;
                        draw_rect2 = false;
                    }

                    if (shape2.rect.getPosition().x - (SHAPE_TWO_WIDTH / 2) < event.mouseButton.x &&
                        shape2.rect.getPosition().x + (SHAPE_TWO_WIDTH / 2) > event.mouseButton.x &&
                        shape2.rect.getPosition().y - (SHAPE_TWO_HEIGHT / 2) < event.mouseButton.y &&
                        shape2.rect.getPosition().y + (SHAPE_TWO_HEIGHT / 2) > event.mouseButton.y)
                    {
                        draw_rect2 = true;
                        draw_rect1 = false;
                    }
                    if (draw_rect1)
                    {
                        if (!(shape1.rect.getPosition().x - (SHAPE_ONE_WIDTH / 2) < event.mouseButton.x &&
                              shape1.rect.getPosition().x + (SHAPE_ONE_WIDTH / 2) > event.mouseButton.x &&
                              shape1.rect.getPosition().y - (SHAPE_ONE_HEIGHT / 2) < event.mouseButton.y &&
                              shape1.rect.getPosition().y + (SHAPE_ONE_HEIGHT / 2) > event.mouseButton.y))
                        {
                            draw_rect1 = false;
                        }
                    }
                    if (draw_rect2)
                    {
                        if (!((shape2.rect.getPosition().x - (SHAPE_TWO_WIDTH / 2) < event.mouseButton.x &&
                               shape2.rect.getPosition().x + (SHAPE_TWO_WIDTH / 2) > event.mouseButton.x &&
                               shape2.rect.getPosition().y - (SHAPE_TWO_HEIGHT / 2) < event.mouseButton.y &&
                               shape2.rect.getPosition().y + (SHAPE_TWO_HEIGHT / 2) > event.mouseButton.y)))
                        {
                            draw_rect2 = false;
                        }
                    }
                }
            }
        }

        // Collision Detection

        std::pair<float, float> check_collision = detect_collision(&shape1, &shape2);
        if (check_collision.first > 0)
        {
            collision_status_text_x.setString(std::to_string((int)check_collision.first));
            collision_status_text_x.setColor(sf::Color::Green);
            if (check_collision.second > 0)
            {

                // Collision Resolution
                std::pair<float, float> prev_overlap = detect_collision(&shape1, &shape2, true);

                if (prev_overlap.first > 0) // TOP-DOWN collision
                {
                    if (draw_rect1)
                    {
                        if (shape1.rect.getPosition().y > shape2.rect.getPosition().y) // TOP collision
                        {
                            shape1.rect.setPosition(sf::Vector2f(shape1.rect.getPosition().x, shape1.rect.getPosition().y + check_collision.second));
                            shape1.bounding_box.setPosition(sf::Vector2f(shape1.rect.getPosition().x, shape1.rect.getPosition().y));
                            shape1.X.setPosition(sf::Vector2f(shape1.X.getPosition().x, shape1.rect.getPosition().y));
                            shape1.Y.setPosition(sf::Vector2f(shape1.Y.getPosition().x, shape1.rect.getPosition().y));
                        }
                        else // DOWN collision
                        {
                            shape1.rect.setPosition(sf::Vector2f(shape1.rect.getPosition().x, shape1.rect.getPosition().y - check_collision.second));
                            shape1.bounding_box.setPosition(sf::Vector2f(shape1.rect.getPosition().x, shape1.rect.getPosition().y));
                            shape1.X.setPosition(sf::Vector2f(shape1.X.getPosition().x, shape1.rect.getPosition().y));
                            shape1.Y.setPosition(sf::Vector2f(shape1.Y.getPosition().x, shape1.rect.getPosition().y));
                        }
                    }
                    else if (draw_rect2)
                    {
                        if (shape2.rect.getPosition().y > shape1.rect.getPosition().y) // TOP
                        {
                            shape2.rect.setPosition(sf::Vector2f(shape2.rect.getPosition().x, shape2.rect.getPosition().y + check_collision.second));
                            shape2.bounding_box.setPosition(sf::Vector2f(shape2.rect.getPosition().x, shape2.rect.getPosition().y));
                            shape2.X.setPosition(sf::Vector2f(shape2.X.getPosition().x, shape2.rect.getPosition().y));
                            shape2.Y.setPosition(sf::Vector2f(shape2.Y.getPosition().x, shape2.rect.getPosition().y));
                        }
                        else // DOWN
                        {
                            shape2.rect.setPosition(sf::Vector2f(shape2.rect.getPosition().x, shape2.rect.getPosition().y - check_collision.second));
                            shape2.bounding_box.setPosition(sf::Vector2f(shape2.rect.getPosition().x, shape2.rect.getPosition().y));
                            shape2.X.setPosition(sf::Vector2f(shape2.X.getPosition().x, shape2.rect.getPosition().y));
                            shape2.Y.setPosition(sf::Vector2f(shape2.Y.getPosition().x, shape2.rect.getPosition().y));
                        }
                    }
                }
                else if (prev_overlap.second > 0) // RIGHT LEFT COLLISION
                {
                    if (draw_rect1)
                    {
                        if (shape1.rect.getPosition().x > shape2.rect.getPosition().x) // RIGHT collision
                        {
                            shape1.rect.setPosition(sf::Vector2f(shape1.rect.getPosition().x + check_collision.first, shape1.rect.getPosition().y));
                            shape1.bounding_box.setPosition(sf::Vector2f(shape1.rect.getPosition().x, shape1.rect.getPosition().y));
                            shape1.X.setPosition(sf::Vector2f(shape1.rect.getPosition().x - 25, shape1.X.getPosition().y));
                            shape1.Y.setPosition(sf::Vector2f(shape1.rect.getPosition().x + 10, shape1.Y.getPosition().y));
                        }
                        else // LEFT
                        {
                            shape1.rect.setPosition(sf::Vector2f(shape1.rect.getPosition().x - check_collision.first, shape1.rect.getPosition().y));
                            shape1.bounding_box.setPosition(sf::Vector2f(shape1.rect.getPosition().x, shape1.rect.getPosition().y));
                            shape1.X.setPosition(sf::Vector2f(shape1.rect.getPosition().x - 25, shape1.X.getPosition().y));
                            shape1.Y.setPosition(sf::Vector2f(shape1.rect.getPosition().x + 10, shape1.Y.getPosition().y));
                        }
                    }
                    else if (draw_rect2)
                    {
                        if (shape2.rect.getPosition().x > shape1.rect.getPosition().x) // RIGHT
                        {
                            shape2.rect.setPosition(sf::Vector2f(shape2.rect.getPosition().x + check_collision.first, shape2.rect.getPosition().y));
                            shape2.bounding_box.setPosition(sf::Vector2f(shape2.rect.getPosition().x, shape2.rect.getPosition().y));
                            shape2.X.setPosition(sf::Vector2f(shape2.rect.getPosition().x - 25, shape2.X.getPosition().y));
                            shape2.Y.setPosition(sf::Vector2f(shape2.rect.getPosition().x + 10, shape2.Y.getPosition().y));
                        }
                        else // LEFT
                        {
                            shape2.rect.setPosition(sf::Vector2f(shape2.rect.getPosition().x - check_collision.first, shape2.rect.getPosition().y));
                            shape2.bounding_box.setPosition(sf::Vector2f(shape2.rect.getPosition().x, shape2.rect.getPosition().y));
                            shape2.X.setPosition(sf::Vector2f(shape2.rect.getPosition().x - 25, shape2.X.getPosition().y));
                            shape2.Y.setPosition(sf::Vector2f(shape2.rect.getPosition().x + 10, shape2.Y.getPosition().y));
                        }
                    }
                }

                if (play_ping)
                    sound.play();

                play_ping = false;

                collision_status_text.setString("True");
                collision_status_text.setColor(sf::Color::Green);
                collision_status_text_y.setColor(sf::Color::Green);
                collision_status_text_y.setString(std::to_string((int)check_collision.second));

                collision_status_text_area.setColor(sf::Color::Green);
                collision_status_text_area.setString(std::to_string((int)(check_collision.first * check_collision.second)));
            }
            else
            {
                play_ping = true;

                collision_status_text_y.setColor(sf::Color::Red);
                collision_status_text.setString("False");
                collision_status_text.setColor(sf::Color::Red);
                collision_status_text_y.setString("0");

                collision_status_text_area.setColor(sf::Color::Red);
                collision_status_text_area.setString("0");
            }
        }
        else if (check_collision.first <= 0 && check_collision.second > 0)
        {
            collision_status_text_y.setString(std::to_string((int)check_collision.second));
            collision_status_text_y.setColor(sf::Color::Green);

            play_ping = true;
            collision_status_text_x.setString("0");
            collision_status_text_area.setString("0");

            collision_status_text_area.setColor(sf::Color::Red);
            collision_status_text_x.setColor(sf::Color::Red);
            collision_status_text.setString("False");
            collision_status_text.setColor(sf::Color::Red);
        }

        else
        {
            play_ping = true;

            collision_status_text_y.setString("0");
            collision_status_text_x.setString("0");
            collision_status_text_area.setString("0");

            collision_status_text_area.setColor(sf::Color::Red);
            collision_status_text_x.setColor(sf::Color::Red);
            collision_status_text_y.setColor(sf::Color::Red);

            collision_status_text.setString("False");
            collision_status_text.setColor(sf::Color::Red);
        }

        window.clear(sf::Color(156, 180, 219));

        window.draw(shape1.rect);
        window.draw(shape1.X);
        window.draw(shape1.Y);

        window.draw(shape2.rect);
        window.draw(shape2.X);
        window.draw(shape2.Y);

        // Rendering texts

        window.draw(status);
        window.draw(collision_text);
        window.draw(collision_status_text);

        window.draw(collision_text_x);
        window.draw(collision_text_y);
        window.draw(collision_text_area);

        window.draw(collision_status_text_x);
        window.draw(collision_status_text_y);
        window.draw(collision_status_text_area);
        window.draw(author);

        window.draw(select_one);
        window.draw(select_two);

        if (draw_rect1)
            window.draw(shape1.bounding_box);

        if (draw_rect2)
            window.draw(shape2.bounding_box);

        // Drawing lines
        window.draw(line1);
        window.draw(line2);
        window.draw(line3);
        window.draw(line4);

        window.display();
    }

    return 0;
}